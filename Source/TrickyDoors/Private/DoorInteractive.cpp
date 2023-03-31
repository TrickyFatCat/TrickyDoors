// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "DoorInteractive.h"

#include "BoxInteractionComponent.h"
#include "LockComponent.h"
#include "Components/BoxComponent.h"

ADoorInteractive::ADoorInteractive()
{
	InteractionTriggerComponent = CreateDefaultSubobject<UBoxInteractionComponent>("InteractionTrigger");
	InteractionTriggerComponent->SetupAttachment(GetRootComponent());
	UInteractionLibrary::SetTriggerDefaultCollision(ToRawPtr(InteractionTriggerComponent));

	LockComponent = CreateDefaultSubobject<ULockComponent>("LockComponent");
}

void ADoorInteractive::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (LockComponent)
	{
		LockComponent->bLockedOnStart = InitialState == EDoorState::Locked;
	}

	if (InteractionTriggerComponent)
	{
		InteractionTriggerComponent->SetInteractionData(InteractionData);
	}
}

void ADoorInteractive::BeginPlay()
{
	InteractionTriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ADoorInteractive::OnTriggerBeginOverlap);
	InteractionTriggerComponent->OnActorAdded.AddDynamic(this, &ADoorInteractive::OnActorAddedToQueue);
	InteractionTriggerComponent->OnActorRemoved.AddDynamic(this, &ADoorInteractive::OnActorRemovedFromQueue);
	Super::BeginPlay();
}

bool ADoorInteractive::FinishInteraction_Implementation(AActor* OtherActor)
{
	switch (CurrentState)
	{
	case EDoorState::Closed:
		UpdateInteractionMessage(OtherActor, InteractionMessages[EDoorState::Opened]);

		if (bCalculateSwingDirection)
		{
			CalculateSwingDirection(OtherActor);
		}

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
		if (LockComponent->Unlock(OtherActor))
		{
			SetIsLocked(false);
			UpdateInteractionMessage(OtherActor, InteractionMessages[EDoorState::Closed]);
		}
		else
		{
			return false;
		}
		break;

	case EDoorState::Transition:
		if (!bIsReversible)
		{
			return false;
		}

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

	if (CurrentState == EDoorState::Opened && !bIsActorInTrigger)
	{
		StartAutoClosingTimer(ClosingDelayDuration);
	}
}

void ADoorInteractive::UpdateInteractionMessage(const AActor* Actor, const FString& NewMessage)
{
	InteractionData.InteractionMessage = NewMessage;
	InteractionTriggerComponent->SetInteractionMessage(Actor, NewMessage);
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
	
	switch (CurrentState)
	{
	case EDoorState::Closed:
	case EDoorState::Opened:
		UpdateInteractionMessage(OtherActor, InteractionMessages[CurrentState]);

	case EDoorState::Locked:
		if (LockComponent->CanUseLock(OtherActor))
		{
			UpdateInteractionMessage(OtherActor, InteractionMessages[CurrentState]);
		}
		else
		{
			UpdateInteractionMessage(OtherActor, CantUnlockMessage);
		}
		break;

	default:
		break;
	}
}

void ADoorInteractive::OnActorAddedToQueue(const AActor* OtherActor)
{
	if (!IsValid(OtherActor))
	{
		return;
	}

	bIsActorInTrigger = true;
	StopAutoClosingTimer();
}

void ADoorInteractive::OnActorRemovedFromQueue(const AActor* OtherActor)
{
	if (!IsValid(OtherActor))
	{
		return;
	}

	bIsActorInTrigger = false;

	if (CurrentState == EDoorState::Opened)
	{
		StartAutoClosingTimer(ClosingDelayDuration);
	}
}
