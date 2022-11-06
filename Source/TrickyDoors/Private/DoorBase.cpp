// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov


#include "DoorBase.h"

#include "TimelineAnimationComponent.h"


ADoorBase::ADoorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	DoorAnimationComponent = CreateDefaultSubobject<UTimelineAnimationComponent>("DoorAnimation");
}

void ADoorBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	switch (InitialState)
	{
	case EDoorState::Opened:
		DoorAnimationComponent->InitialState = ETimelineAnimationState::End;
		break;

	case EDoorState::Closed:
		DoorAnimationComponent->InitialState = ETimelineAnimationState::Begin;
		break;

	default:
		break;
	}
}

void ADoorBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentState = InitialState;
}

void ADoorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADoorBase::Open()
{
	if (CurrentState != EDoorState::Closed)
	{
		return;
	}

	DoorAnimationComponent->Start();
}

void ADoorBase::Close()
{
	if (CurrentState != EDoorState::Opened)
	{
		return;
	}

	DoorAnimationComponent->Start();
}

void ADoorBase::SetIsLocked(const bool bIsLocked)
{
	if (CurrentState == EDoorState::Transition || CurrentState == EDoorState::Disabled)
	{
		return;
	}

	if (bIsLocked)
	{
		if (CurrentState == EDoorState::Locked)
		{
			return;
		}

		PreviousState = CurrentState;
		CurrentState = EDoorState::Locked;
	}
	else
	{
		if (CurrentState != EDoorState::Locked)
		{
			return;
		}

		CurrentState = PreviousState;
		PreviousState = EDoorState::Locked;
	}

	OnStateChanged.Broadcast(CurrentState);
}

void ADoorBase::SetIsEnabled(const bool bIsEnabled)
{
	if (bIsEnabled)
	{
		if (CurrentState != EDoorState::Disabled)
		{
			return;
		}

		if (PreviousState == EDoorState::Transition)
		{
			DoorAnimationComponent->Resume();
		}

		CurrentState = PreviousState;
		PreviousState = EDoorState::Disabled;
		SetActorTickEnabled(true);
	}
	else
	{
		if (CurrentState == EDoorState::Disabled)
		{
			return;
		}

		if (CurrentState == EDoorState::Transition)
		{
			DoorAnimationComponent->Pause();
		}

		PreviousState = CurrentState;
		CurrentState = EDoorState::Disabled;
		SetActorTickEnabled(false);
	}

	OnStateChanged.Broadcast(CurrentState);
}

void ADoorBase::ChangeState(const ETimelineAnimationState NewAnimationState)
{
	PreviousState = CurrentState;

	switch (NewAnimationState)
	{
	case ETimelineAnimationState::Begin:
		CurrentState = EDoorState::Closed;
		break;

	case ETimelineAnimationState::End:
		CurrentState = EDoorState::Opened;
		break;

	case ETimelineAnimationState::Transition:
		CurrentState = EDoorState::Transition;
		break;
		
	default:
		break;
	}
	
	OnStateChanged.Broadcast(CurrentState);
}
