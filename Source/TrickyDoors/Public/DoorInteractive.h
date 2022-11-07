// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "DoorBase.h"
#include "InteractionInterface.h"
#include "DoorInteractive.generated.h"

class UBoxInteractionTrigger;
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
	UBoxInteractionTrigger* InteractionTriggerComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door")
	bool bRequiredKey = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door", meta=(EditCondition="bRequiredKey"))
	TSubclassOf<UKeyType> KeyClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Door")
	TMap<EDoorState, FString> InteractionMessagesMap{
			{EDoorState::Closed, "Open"},
			{EDoorState::Opened, "Close"},
			{EDoorState::Locked, "Unlock"}
	};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Door")
	FString CantUnlockMessage{"Can't unlock"};

private:
	virtual bool Interact_Implementation(AActor* OtherActor) override;

	virtual void ChangeState(const ETimelineAnimationState NewAnimationState) override;
	
	UFUNCTION()
	void OnInteractionTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                                      AActor* OtherActor,
	                                      UPrimitiveComponent* OtherComp,
	                                      int32 OtherBodyIndex,
	                                      bool bFromSweep,
	                                      const FHitResult& SweepResult);

	
};
