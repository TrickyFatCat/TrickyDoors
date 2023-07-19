// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "TimelineAnimationComponent.h"
#include "GameFramework/Actor.h"
#include "TimelineAnimationComponent.h"
#include "TimerManager.h"
#include "DoorBase.generated.h"


UENUM(BlueprintType)
enum class EDoorState : uint8
{
	Closed,
	Opened,
	Transition,
	Locked,
	Disabled
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChangedSignature, EDoorState, NewState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorReversedSignature);

/**
 * A base door class which contains the basic door logic.
 */
UCLASS()
class TRICKYDOORS_API ADoorBase : public AActor
{
	GENERATED_BODY()

public:
	ADoorBase();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/**Called when current state was changed.*/
	UPROPERTY(BlueprintAssignable, Category="Door")
	FOnStateChangedSignature OnStateChanged;

	/**Called when the door animation was reversed.*/
	UPROPERTY(BlueprintAssignable, Category="Door")
	FOnDoorReversedSignature OnReversed;

	/**The door state on begin play.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door")
	EDoorState InitialState = EDoorState::Closed;

	/**Toggles if the door animation can be reversed.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door")
	bool bIsReversible = false;

	/**If true, the rotation offset can be recalculated using the CalculateSwingDirection function.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door")
	bool bCalculateSwingDirection = false;

	/**Opens the door.*/
	UFUNCTION(BlueprintCallable, Category="Door")
	void Open();

	/**Closes the door.*/
	UFUNCTION(BlueprintCallable, Category="Door")
	void Close();

	/**Sets if the door is locked or not.*/
	UFUNCTION(BlueprintCallable, Category="Door")
	void SetIsLocked(const bool bIsLocked);

	/**Sets if the door is enabled.*/
	UFUNCTION(BlueprintCallable, Category="Door")
	void SetIsEnabled(const bool bIsEnabled);
	
	UFUNCTION(BlueprintGetter, Category="Door")
	FTimerHandle GetAutoClosingTimer() const;

	/**Starts the timer after which the door will be closed automatically.*/
	UFUNCTION(BlueprintCallable, Category="Door")
	void StartAutoClosingTimer(const float Duration);

	/**Stops autoclosing timer.*/
	UFUNCTION(BlueprintCallable, Category="Door")
	bool StopAutoClosingTimer();

	/**Calculates new rotation offset relative to the given actor.**/
	UFUNCTION(BlueprintCallable, Category="Door")
	void CalculateSwingDirection(const AActor* Actor);

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<USceneComponent> DoorRootComponent = nullptr;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<UTimelineAnimationComponent> DoorAnimationComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door")
	EDoorState CurrentState = EDoorState::Closed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door")
	EDoorState PreviousState = EDoorState::Closed;

	UPROPERTY(BlueprintGetter=GetAutoClosingTimer, Category="Door")
	FTimerHandle AutoClosingTimer;

	int32 SwingDirection = 0.f;

	int32 PrevSwingDirection = 0.f;
	
	UFUNCTION()
	virtual void ChangeState(const ETimelineAnimationState NewAnimationState);

	/**Called when the current state was changed.*/
	UFUNCTION(BlueprintImplementableEvent, Category="Door")
	void OnDoorChangedState(const EDoorState NewState);

	/**Called when the door animation was reversed.*/
	UFUNCTION(BlueprintImplementableEvent, Category="Door")
	void OnDoorAnimationReversed();
};
