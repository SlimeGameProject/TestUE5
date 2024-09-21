// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/FallingSlimePlayerCamera_ThirdPerson.h"

// Include headers in FallingSlime module.
#include "Player/FallingSlimePlayerController.h"

AFallingSlimePlayerCamera_ThirdPerson::AFallingSlimePlayerCamera_ThirdPerson(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (UCameraComponent* CameraComp = GetCamera())
	{
		DefaultSceneRoot = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("DefaultSceneRoot"));

		SetRootComponent(DefaultSceneRoot);

		CameraArm = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraArm"));

		CameraArm->AttachToComponent(DefaultSceneRoot, FAttachmentTransformRules::KeepRelativeTransform);
		CameraArm->bEnableCameraRotationLag = true;

		CameraComp->AttachToComponent(CameraArm, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void AFallingSlimePlayerCamera_ThirdPerson::UpdateCamera()
{
	Super::UpdateCamera();

	if (AFallingSlimePlayerController* PC = GetPlayerController())
	{
		if (APawn* TargetPawn = PC->GetPawn())
		{
			FRotator NewActorRotation = TargetPawn->GetActorRotation();

			if (CameraArm->bUsePawnControlRotation)
			{
				FRotator ControlRotation = PC->GetControlRotation();

				if (CameraArm->bInheritPitch)
				{
					NewActorRotation.Pitch = ControlRotation.Pitch;
				}

				if (CameraArm->bInheritYaw)
				{
					NewActorRotation.Yaw = ControlRotation.Yaw;
				}

				if (CameraArm->bInheritRoll)
				{
					NewActorRotation.Roll = ControlRotation.Roll;
				}
			};

			SetActorRotation(NewActorRotation);
		}
	}
}
