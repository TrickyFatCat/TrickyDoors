// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "DoorAuto.h"

#include "LockComponent.h"
#include "Components/BoxComponent.h"

ADoorAuto::ADoorAuto()
{
	bIsReversible = true;

	ActivationTriggerComponent = CreateDefaultSubobject<UBoxComponent>("ActivationTrigger");
	ActivationTriggerComponent->SetupAttachment(GetRootComponent());
	ActivationTriggerComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ActivationTriggerComponent->SetCollisionObjectType(ECC_WorldDynamic);
	ActivationTriggerComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	ActivationTriggerComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	LockComponent = CreateDefaultSubobject<ULockComponent>("LockComponent");
}

void ADoorAuto::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (LockComponent)
	{
		LockComponent->bLockedOnStart = InitialState == EDoorState::Locked;
	}
}

void ADoorAuto::BeginPlay()
{
	Super::BeginPlay();

	ActivationTriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ADoorAuto::OnActivationTriggerBeginOverlap);
	ActivationTriggerComponent->OnComponentEndOverlap.AddDynamic(this, &ADoorAuto::OnActivationTriggerEndOverlap);

	LockComponent->bLockedOnStart = CurrentState == EDoorState::Locked;
}

void ADoorAuto::OnActivationTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
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

	if (bCalculateSwingDirection && CurrentState == EDoorState::Closed)
	{
		CalculateSwingDirection(OtherActor);
	}

	if (bIsClosingDelayed)
	{
		if (StopAutoClosingTimer())
		{
			return;
		}
	}

	if (LockComponent->GetIsLocked())
	{
		if (LockComponent->Unlock(OtherActor))
		{
			CalculateSwingDirection(OtherActor);
			SetIsLocked(LockComponent->GetIsLocked());
			Open();
		}
	}
	else
	{
		Open();
	}
}

void ADoorAuto::OnActivationTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                              AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp,
                                              int32 OtherBodyIndex)
{
	if (bIsClosingDelayed)
	{
		StartAutoClosingTimer(ClosingDelayDuration);
	}
	else
	{
		Close();
	}
}
