// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "DoorBase.h"
#include "DoorAuto.generated.h"

class UBoxComponent;
class UKeyType;
/**
 * 
 */
UCLASS()
class TRICKYDOORS_API ADoorAuto : public ADoorBase
{
	GENERATED_BODY()

public:
	ADoorAuto();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Coponents")
	UBoxComponent* ActivationTriggerComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door")
	bool bRequiredKey = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door", meta=(EditCondition="bRequiredKey"))
	TSubclassOf<UKeyType> KeyClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door")
	bool bIsClosingDelayed = false;

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
