// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/FallingSlimePlayerCamera.h"

// Include headers in Core module.
#include "Math/UnrealMathUtility.h"

// Include headers in FallingSlime module.
#include "Player/FallingSlimePlayerController.h"

// Sets default values
AFallingSlimePlayerCamera::AFallingSlimePlayerCamera(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	Camera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("Camera"));

	SetRootComponent(Camera);
}

// Called every frame
void AFallingSlimePlayerCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCamera();
}

void AFallingSlimePlayerCamera::PossessedBy(AFallingSlimePlayerController* NewPlayerController)
{
	PlayerController = NewPlayerController;

	if (APawn* TargetPawn = PlayerController->GetPawn())
	{
		SetActorLocation(TargetPawn->GetActorLocation());
		SetActorRotation(TargetPawn->GetActorRotation());
	}

	OnPossessedBy(PlayerController);
}

void AFallingSlimePlayerCamera::Unpossessed()
{
	AFallingSlimePlayerController* const OldPlayerController = PlayerController;

	PlayerController = nullptr;

	OnUnpossessed(OldPlayerController);
}

void AFallingSlimePlayerCamera::UpdateCamera()
{
	if (PlayerController)
	{
		if (APawn* TargetPawn = PlayerController->GetPawn())
		{
			SetActorLocation(TargetPawn->GetActorLocation());
			SetActorRotation(TargetPawn->GetActorRotation());
		}
	}
}

void AFallingSlimePlayerCamera::OnPossessedBy(AFallingSlimePlayerController* NewPlayerController) {}

void AFallingSlimePlayerCamera::OnUnpossessed(AFallingSlimePlayerController* OldPlayerController) {}

