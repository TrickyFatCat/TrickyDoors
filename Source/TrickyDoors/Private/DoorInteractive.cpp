// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov


#include "DoorInteractive.h"

#include "KeyringLibrary.h"
#include "KeyType.h"
#include "InteractionTriggers/BoxInteractionTrigger.h"

ADoorInteractive::ADoorInteractive()
{
	InteractionTriggerComponent = CreateDefaultSubobject<UBoxInteractionTrigger>("InteractionTrigger");
	InteractionTriggerComponent->SetupAttachment(GetRootComponent());
}

void ADoorInteractive::BeginPlay()
{
	Super::BeginPlay();

	InteractionTriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ADoorInteractive::OnInteractionTriggerBeginOverlap);
}

bool ADoorInteractive::Interact_Implementation(AActor* OtherActor)
{
	bool bResult = false;

	switch (CurrentState)
	{
	case EDoorState::Closed:
		Open();
		bResult = true;
		break;

	case EDoorState::Opened:
		Close();
		bResult = true;
		break;

	case EDoorState::Locked:
		if (UKeyringLibrary::ActorHasKey(OtherActor, KeyClass))
		{
			SetIsLocked(false);
			InteractionTriggerComponent->SetInteractionMessage(InteractionMessagesMap[EDoorState::Closed]);
		}
		break;

	default:
		break;
	}
	return bResult;
}

void ADoorInteractive::ChangeState(const ETimelineAnimationState NewAnimationState)
{
	switch (NewAnimationState)
	{
	case ETimelineAnimationState::Begin:
	case ETimelineAnimationState::End:
		InteractionTriggerComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		break;
		
	case ETimelineAnimationState::Transition:
		InteractionTriggerComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;

	default:
		break;
	}
	
	Super::ChangeState(NewAnimationState);
}

void ADoorInteractive::OnInteractionTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
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

	switch (CurrentState)
	{
	case EDoorState::Closed:
	case EDoorState::Opened:
		InteractionTriggerComponent->SetInteractionMessage(InteractionMessagesMap[CurrentState]);

	case EDoorState::Locked:
		if (bRequiredKey && KeyClass)
		{
			if (UKeyringLibrary::ActorHasKey(OtherActor, KeyClass))
			{
				InteractionTriggerComponent->SetInteractionMessage(InteractionMessagesMap[CurrentState]);
			}
			else
			{
				InteractionTriggerComponent->SetInteractionMessage(CantUnlockMessage);
			}
		}
		break;

	default:
		break;
	}
}
