// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "DoorBase.h"
#include "InteractionInterface.h"
#include "InteractionLibrary.h"
#include "DoorInteractive.generated.h"

class UBoxComponent;
class UKeyType;

/**
 * 
 */
UCLASS()
class TRICKYDOORS_API ADoorInteractive : public ADoorBase, public IInteractionInterface
{
	GENERATED_BODY()

public:
	ADoorInteractive();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UBoxComponent* InteractionTriggerComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door|Key")
	bool bRequiredKey = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door|Key", meta=(EditCondition="bRequiredKey"))
	TSubclassOf<UKeyType> KeyClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Door|Interaction")
	FInteractionData InteractionData;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Door|Interaction")
	TMap<EDoorState, FString> InteractionMessages{
			{EDoorState::Closed, "Open"},
			{EDoorState::Opened, "Close"},
			{EDoorState::Locked, "Unlock"}
	};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Door|Interaction")
	FString CantUnlockMessage{"Can't unlock"};



private:
	virtual bool FinishInteraction_Implementation(AActor* OtherActor) override;

	virtual void ChangeState(const ETimelineAnimationState NewAnimationState) override;

	void UpdateInteractionMessage(const AActor* Actor, const FString& NewMessage);


	UFUNCTION()
	void OnInteractionTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                                      AActor* OtherActor,
	                                      UPrimitiveComponent* OtherComp,
	                                      int32 OtherBodyIndex,
	                                      bool bFromSweep,
	                                      const FHitResult& SweepResult);

	UFUNCTION()
	void OnInteractionTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,
	                                    AActor* OtherActor,
	                                    UPrimitiveComponent* OtherComp,
	                                    int32 OtherBodyIndex);
};
