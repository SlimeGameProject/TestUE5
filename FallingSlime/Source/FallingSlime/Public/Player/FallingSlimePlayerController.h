// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "FallingSlimePlayerController.generated.h"

class AFallingSlimePlayerCamera;

/**
 * 
 */
UCLASS()
class FALLINGSLIME_API AFallingSlimePlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AFallingSlimePlayerController(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "FallingSlime|PlayerController")
	void PossessCamera(TSubclassOf<AFallingSlimePlayerCamera> PlayerCameraClass);

private:

	UPROPERTY(BlueprintReadOnly, Category = "FallingSlime|PlayerController", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AFallingSlimePlayerCamera> PlayerCamera{ nullptr };
};
