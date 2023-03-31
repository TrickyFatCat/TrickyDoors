// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "DoorBase.h"
#include "DoorAuto.generated.h"

class UBoxComponent;
class ULockComponent;

/**
 * A door which can be opened automatically by entering a trigger. 
 */
UCLASS()
class TRICKYDOORS_API ADoorAuto : public ADoorBase
{
	GENERATED_BODY()

public:
	ADoorAuto();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Coponents")
	TObjectPtr<UBoxComponent> ActivationTriggerComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Coponents")
	TObjectPtr<ULockComponent> LockComponent = nullptr;
	
	/**Toggles if the door will close automatically after some time.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door")
	bool bIsClosingDelayed = false;

	/**Delay after which the opened door will close automatically.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door", meta=(EditCondition="bIsClosingDelayed"))
	float ClosingDelayDuration = 3.f;

private:
	UFUNCTION()
	void OnActivationTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                                     AActor* OtherActor,
	                                     UPrimitiveComponent* OtherComp,
	                                     int32 OtherBodyIndex,
	                                     bool bFromSweep,
	                                     const FHitResult& SweepResult);

	UFUNCTION()
	void OnActivationTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,
	                                   AActor* OtherActor,
	                                   UPrimitiveComponent* OtherComp,
	                                   int32 OtherBodyIndex);
};
