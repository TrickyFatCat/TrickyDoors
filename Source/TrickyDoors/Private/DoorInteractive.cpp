// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov


#include "DoorInteractive.h"

#include "KeyringLibrary.h"
#include "KeyType.h"
#include "Components/BoxComponent.h"

ADoorInteractive::ADoorInteractive()
{
	InteractionTriggerComponent = CreateDefaultSubobject<UBoxComponent>("InteractionTrigger");
	InteractionTriggerComponent->SetupAttachment(GetRootComponent());
	UInteractionLibrary::SetTriggerDefaultCollision(InteractionTriggerComponent);
}

void ADoorInteractive::BeginPlay()
{
	InteractionTriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ADoorInteractive::OnTriggerBeginOverlap);
	InteractionTriggerComponent->OnComponentEndOverlap.AddDynamic(this, &ADoorInteractive::OnTriggerEndOverlap);
	Super::BeginPlay();
}

bool ADoorInteractive::FinishInteraction_Implementation(AActor* OtherActor)
{
	switch (CurrentState)
	{
	case EDoorState::Closed:
		UpdateInteractionMessage(OtherActor, InteractionMessages[EDoorState::Opened]);
		Open();

		if (!bIsReversible)
		{
			InteractionTriggerComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		break;

	case EDoorState::Opened:
		UpdateInteractionMessage(OtherActor, InteractionMessages[EDoorState::Closed]);
		Close();
		
		if (!bIsReversible)
		{
			InteractionTriggerComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		break;

	case EDoorState::Locked:
		if (UKeyringLibrary::ActorHasKey(OtherActor, KeyClass))
		{
			SetIsLocked(false);
			UpdateInteractionMessage(OtherActor, InteractionMessages[EDoorState::Closed]);
		}
		break;

	case EDoorState::Transition:
		if (bIsReversible)
		{
			if (DoorAnimationComponent->Reverse())
			{
				switch (DoorAnimationComponent->GetTargetState())
				{
				case ETimelineAnimationState::Begin:
					UpdateInteractionMessage(OtherActor, InteractionMessages[EDoorState::Closed]);
					break;

				case ETimelineAnimationState::End:
					UpdateInteractionMessage(OtherActor, InteractionMessages[EDoorState::Opened]);
					break;
				}
			}
		}
		break;

	default:
		break;
	}

	return true;
}

void ADoorInteractive::ChangeState(const ETimelineAnimationState NewAnimationState)
{
	switch (NewAnimationState)
	{
	case ETimelineAnimationState::Begin:
	case ETimelineAnimationState::End:
		InteractionTriggerComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		break;
		
	default:
		break;
	}

	Super::ChangeState(NewAnimationState);
}

void ADoorInteractive::UpdateInteractionMessage(const AActor* Actor, const FString& NewMessage)
{
	InteractionData.InteractionMessage = NewMessage;
	UInteractionLibrary::UpdateInteractionMessage(Actor, this, NewMessage);
}

void ADoorInteractive::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                             AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp,
                                             int32 OtherBodyIndex,
                                             bool bFromSweep,
                                             const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor))
	{
		return;
	}

	UInteractionLibrary::AddToInteractionQueue(OtherActor, this, InteractionData);

	switch (CurrentState)
	{
	case EDoorState::Closed:
	case EDoorState::Opened:
		UpdateInteractionMessage(OtherActor, InteractionMessages[CurrentState]);

	case EDoorState::Locked:
		if (bRequiredKey && KeyClass)
		{
			if (UKeyringLibrary::ActorHasKey(OtherActor, KeyClass))
			{
				UpdateInteractionMessage(OtherActor, InteractionMessages[CurrentState]);
			}
			else
			{
				UpdateInteractionMessage(OtherActor, CantUnlockMessage);
			}
		}
		break;

	default:
		break;
	}
}

void ADoorInteractive::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                           AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp,
                                           int32 OtherBodyIndex)
{
	if (!IsValid(OtherActor))
	{
		return;
	}

	UInteractionLibrary::RemoveFromInteractionQueue(OtherActor, this);
}
