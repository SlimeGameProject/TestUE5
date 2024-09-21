// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/FallingSlimePlayerController.h"

// Include headers in EnhancedInput module.
#include "EnhancedInputSubsystems.h"

// Include headers in FallingSlime module.
#include "Player/FallingSlimePlayerCamera.h"
#include "Player/FallingSlimePlayerCameraManager.h"

AFallingSlimePlayerController::AFallingSlimePlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoManageActiveCameraTarget = false;
	PlayerCameraManagerClass = AFallingSlimePlayerCameraManager::StaticClass();
}

void AFallingSlimePlayerController::PossessCamera(TSubclassOf<AFallingSlimePlayerCamera> PlayerCameraClass)
{
	AFallingSlimePlayerCamera* OldPlayerCamera = PlayerCamera;

	if (OldPlayerCamera)
	{
		if (OldPlayerCamera->GetClass() == PlayerCameraClass)
		{
			return;
		}

		OldPlayerCamera->Unpossessed();
	}

	PlayerCamera = GetWorld()->SpawnActor<AFallingSlimePlayerCamera>(PlayerCameraClass);

	PlayerCamera->PossessedBy(this);

	ResetCameraMode();

	SetViewTargetWithBlend(PlayerCamera, 0.1f, EViewTargetBlendFunction::VTBlend_Linear, 0.f, true);

	if (OldPlayerCamera)
	{
		OldPlayerCamera->Destroy();
		OldPlayerCamera = nullptr;
	}
}
