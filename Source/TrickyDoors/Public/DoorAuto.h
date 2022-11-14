// MIT License Copyright (c) 2022 Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "DoorBase.h"
#include "DoorAuto.generated.h"

class UBoxComponent;
class UKeyType;

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
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Coponents")
	UBoxComponent* ActivationTriggerComponent = nullptr;

	/**Toggles if the door requires a key to open it.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door")
	bool bRequiredKey = false;

	/**Key class which the actor must have to open the door.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door", meta=(EditCondition="bRequiredKey"))
	TSubclassOf<UKeyType> KeyClass = nullptr;

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
