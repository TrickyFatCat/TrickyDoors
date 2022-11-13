// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "TimelineAnimationComponent.h"
#include "GameFramework/Actor.h"
#include "TimelineAnimationComponent.h"
#include "TimerManager.h"
#include "DoorBase.generated.h"


UENUM()
enum class EDoorState : uint8
{
	Closed,
	Opened,
	Transition,
	Locked,
	Disabled
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChangedSignature, EDoorState, NewState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReversedSignature);

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

	UPROPERTY(BlueprintAssignable, Category="Door")
	FOnStateChangedSignature OnStateChanged;

	UPROPERTY(BlueprintAssignable, Category="Door")
	FOnReversedSignature OnReversed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door")
	EDoorState InitialState = EDoorState::Closed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door")
	bool bIsReversible = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door")
	bool bCalculateSwingDirection = false;

	UFUNCTION(BlueprintCallable, Category="Door")
	void Open();

	UFUNCTION(BlueprintCallable, Category="Door")
	void Close();

	UFUNCTION(BlueprintCallable, Category="Door")
	void SetIsLocked(const bool bIsLocked);

	UFUNCTION(BlueprintCallable, Category="Door")
	void SetIsEnabled(const bool bIsEnabled);

	UFUNCTION(BlueprintGetter, Category="Door")
	FTimerHandle GetAutoClosingTimer() const;
	
	UFUNCTION(BlueprintCallable, Category="Door")
	void StartAutoClosingTimer(const float Duration);

	UFUNCTION(BlueprintCallable, Category="Door")
	bool StopAutoClosingTimer();

	UFUNCTION(BlueprintCallable, Category="Door")
	void CalculateSwingDirection(const AActor* Actor);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* DoorRootComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UTimelineAnimationComponent* DoorAnimationComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door")
	EDoorState CurrentState = EDoorState::Closed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door")
	EDoorState PreviousState = EDoorState::Closed;

	UPROPERTY(BlueprintGetter=GetAutoClosingTimer, Category="Door")
	FTimerHandle AutoClosingTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door")
	int32 SwingDirection = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door")
	int32 PrevSwingDirection = 0.f;
	
	UFUNCTION()
	virtual void ChangeState(const ETimelineAnimationState NewAnimationState);

	UFUNCTION(BlueprintImplementableEvent, Category="Door")
	void OnDoorChangedState(const EDoorState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category="Door")
	void OnDoorAnimationReversed();
};
