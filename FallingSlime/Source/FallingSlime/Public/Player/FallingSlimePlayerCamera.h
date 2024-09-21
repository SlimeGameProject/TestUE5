// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Include headers in Engine module.
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "FallingSlimePlayerCamera.generated.h"

class AFallingSlimePlayerController;

UCLASS()
class FALLINGSLIME_API AFallingSlimePlayerCamera : public AActor
{
	GENERATED_BODY()
	
public:

	AFallingSlimePlayerCamera(const FObjectInitializer& ObjectInitializer);

public:

	// Begin AActor interfaces.

	virtual void Tick(float DeltaTime) override;

	// End AActor interfaces.

public:

	AFallingSlimePlayerController* GetPlayerController() const { return PlayerController; }

	UCameraComponent* GetCamera() const { return Camera; }

	void PossessedBy(AFallingSlimePlayerController* NewPlayerController);

	void Unpossessed();

protected:

	virtual void UpdateCamera();

	virtual void OnPossessedBy(AFallingSlimePlayerController* NewPlayerController);

	virtual void OnUnpossessed(AFallingSlimePlayerController* OldPlayerController);

private:

	// Begin components

	UPROPERTY(BlueprintReadOnly, Category = "FallingSlime|PlayerCamera", VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera{ nullptr };

	// End components

private:

	UPROPERTY(BlueprintReadOnly, Category = "FallingSlime|PlayerCamera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AFallingSlimePlayerController> PlayerController{ nullptr };
};
