// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "DoorBase.h"
#include "InteractionInterface.h"
#include "InteractionLibrary.h"
#include "DoorInteractive.generated.h"

class ULockComponent;
class UBoxInteractionComponent;

/**
 * A door which requires interaction to be opened.
 */
UCLASS()
class TRICKYDOORS_API ADoorInteractive : public ADoorBase, public IInteractionInterface
{
	GENERATED_BODY()

public:
	ADoorInteractive();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<UBoxInteractionComponent> InteractionTriggerComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<ULockComponent> LockComponent = nullptr;

	/**Toggles if the door will close automatically after some time.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door")
	bool bIsClosingDelayed = false;

	/**Delay after which the opened door will close automatically.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door", meta=(EditCondition="bIsClosingDelayed"))
	float ClosingDelayDuration = 3.f;

	/**Data used for interaction.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Door|Interaction")
	FInteractionData InteractionData;

	/**Interaction messages for different states of the door.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Door|Interaction")
	TMap<EDoorState, FString> InteractionMessages{
		{EDoorState::Closed, "Open"},
		{EDoorState::Opened, "Close"},
		{EDoorState::Locked, "Unlock"}
	};

	/**The message in case the actor don't have a key to unlock the door.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Door|Interaction")
	FString CantUnlockMessage{"Can't unlock"};

private:
	bool bIsActorInTrigger = false;

	virtual bool FinishInteraction_Implementation(AActor* OtherActor) override;

	virtual void ChangeState(const ETimelineAnimationState NewAnimationState) override;

	void UpdateInteractionMessage(const AActor* Actor, const FString& NewMessage);

	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                           AActor* OtherActor,
	                           UPrimitiveComponent* OtherComp,
	                           int32 OtherBodyIndex,
	                           bool bFromSweep,
	                           const FHitResult& SweepResult);

	UFUNCTION()
	void OnActorAddedToQueue(const AActor* OtherActor);

	UFUNCTION()
	void OnActorRemovedFromQueue(const AActor* OtherActor);
};
