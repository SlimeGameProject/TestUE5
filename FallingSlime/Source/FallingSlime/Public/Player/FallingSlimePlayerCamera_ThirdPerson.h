// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/FallingSlimePlayerCamera.h"

#include "FallingSlimePlayerCamera_ThirdPerson.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGSLIME_API AFallingSlimePlayerCamera_ThirdPerson : public AFallingSlimePlayerCamera
{
	GENERATED_BODY()

public:

	AFallingSlimePlayerCamera_ThirdPerson(const FObjectInitializer& ObjectInitializer);

public:

	USpringArmComponent* GetCameraArm() { return CameraArm; }

protected:

	// Begin AFallingSlimePlayerCamera interfaces.

	virtual void UpdateCamera() override;

	// End AFallingSlimePlayerCamera interfaces.

protected:

	// Begin components

	UPROPERTY(BlueprintReadOnly, Category = "FallingSlime|PlayerCamera", VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> DefaultSceneRoot{ nullptr };

	UPROPERTY(BlueprintReadOnly, Category = "FallingSlime|PlayerCamera", VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraArm{ nullptr };

	// End components
};
