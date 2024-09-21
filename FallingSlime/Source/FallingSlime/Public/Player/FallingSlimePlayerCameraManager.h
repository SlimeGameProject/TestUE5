// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "FallingSlimePlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGSLIME_API AFallingSlimePlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:

	AFallingSlimePlayerCameraManager(const FObjectInitializer& ObjectInitializer);
};
