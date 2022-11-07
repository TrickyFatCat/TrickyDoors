// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov


#include "DoorBase.h"

#include "TimelineAnimationComponent.h"
#include "TimerManager.h"


ADoorBase::ADoorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	DoorRootComponent = CreateDefaultSubobject<USceneComponent>("DoorRoot");
	SetRootComponent(DoorRootComponent);
	
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

	DoorAnimationComponent->OnAnimationFinished.AddDynamic(this, &ADoorBase::ChangeState);
	CurrentState = InitialState;

	switch (CurrentState)
	{
	case EDoorState::Locked:
		SetIsLocked(true);
		break;

	case EDoorState::Disabled:
		SetIsEnabled(false);
		break;

	default:
		break;
	}
}

void ADoorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADoorBase::Open()
{
	switch (CurrentState)
	{
	case EDoorState::Closed:
		DoorAnimationComponent->Start();
		PreviousState = CurrentState;
		CurrentState = EDoorState::Transition;
		OnDoorChangedState(CurrentState);
		OnStateChanged.Broadcast(CurrentState);
		break;

	case EDoorState::Transition:
		if (bIsReversible)
		{
			DoorAnimationComponent->Reverse();
			OnReversed.Broadcast();
		}

	default:
		break;
	}
}

void ADoorBase::Close()
{
	switch (CurrentState)
	{
	case EDoorState::Opened:
		DoorAnimationComponent->Start();
		PreviousState = CurrentState;
		CurrentState = EDoorState::Transition;
		OnDoorChangedState(CurrentState);
		OnStateChanged.Broadcast(CurrentState);
		break;

	case EDoorState::Transition:
		if (bIsReversible)
		{
			DoorAnimationComponent->Reverse();
			OnReversed.Broadcast();
		}
		break;

	default:
		break;
	}
}

void ADoorBase::SetIsLocked(const bool bIsLocked)
{
	if (CurrentState == EDoorState::Transition || CurrentState == EDoorState::Disabled || CurrentState == EDoorState::Opened)
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

	OnDoorChangedState(CurrentState);
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

	OnDoorChangedState(CurrentState);
	OnStateChanged.Broadcast(CurrentState);
}

FTimerHandle ADoorBase::GetAutoClosingTimer() const
{
	return AutoClosingTimer;
}

void ADoorBase::StartAutoClosingTimer(const float Duration)
{
	if (!GetWorld() || Duration <= 0.f)
	{
		return;
	}

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (!TimerManager.IsTimerActive(AutoClosingTimer))
	{
		TimerManager.SetTimer(AutoClosingTimer, this, &ADoorBase::Close, Duration);
	}
}

bool ADoorBase::StopAutoClosingTimer()
{
	if (!GetWorld())
	{
		return false;
	}

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (TimerManager.IsTimerActive(AutoClosingTimer))
	{
		TimerManager.ClearTimer(AutoClosingTimer);
		return true;
	}

	return false;
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

	default:
		break;
	}

	OnDoorChangedState(CurrentState);
	OnStateChanged.Broadcast(CurrentState);
}
