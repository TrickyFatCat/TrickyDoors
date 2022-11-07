// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov


#include "DoorAuto.h"

#include "KeyringLibrary.h"
#include "KeyType.h"
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
}

void ADoorAuto::BeginPlay()
{
	Super::BeginPlay();

	ActivationTriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ADoorAuto::OnActivationTriggerBeginOverlap);
	ActivationTriggerComponent->OnComponentEndOverlap.AddDynamic(this, &ADoorAuto::OnActivationTriggerEndOverlap);
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
	
	if (bIsClosingDelayed)
	{
		if (StopAutoClosingTimer())
		{
			return;
		}
	}

	if (KeyClass && bRequiredKey)
	{
		if (UKeyringLibrary::ActorUseKey(OtherActor, KeyClass))
		{
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
