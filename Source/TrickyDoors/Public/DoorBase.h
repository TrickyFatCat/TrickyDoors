// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "TimelineAnimationComponent.h"
#include "GameFramework/Actor.h"
#include "TimelineAnimationComponent.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door")
	EDoorState InitialState = EDoorState::Closed;

	UFUNCTION(BlueprintCallable, Category="Door")
	void Open();

	UFUNCTION(BlueprintCallable, Category="Door")
	void Close();

	UFUNCTION(BlueprintCallable, Category="Door")
	void SetIsLocked(const bool bIsLocked);

	UFUNCTION(BlueprintCallable, Category="Door")
	void SetIsEnabled(const bool bIsEnabled);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UTimelineAnimationComponent* DoorAnimationComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door")
	EDoorState CurrentState = EDoorState::Closed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Door")
	EDoorState PreviousState = EDoorState::Closed;

	UFUNCTION()
	void ChangeState(const ETimelineAnimationState NewAnimationState);
};
