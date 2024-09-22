// Fill out your copyright notice in the Description page of Project Settings.

#include "Slime/Character/FallingSlimeSlimeCharacter.h"

// Include headers in Engine module.
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Include headers in EnhancedInput module.
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Include headers in FallingSlime module.
#include "Collision/FallingSlimeCollisionSettings.h"
#include "Slime/Character/FallingSlimeSlimeCharacterMovementTypes.h"
#include "Player/FallingSlimePlayerCameraManager.h"
#include "Player/FallingSlimePlayerController.h"

// Include headers in Niagara module.
#include "NiagaraFunctionLibrary.h"

// Sets default values
AFallingSlimeSlimeCharacter::AFallingSlimeSlimeCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UFallingSlimeSlimeCharacterMovementComponent>(TEXT("CharMoveComp")))
{
	bUseControllerRotationYaw = false;

	SlimeCharacterMovement = CastChecked<UFallingSlimeSlimeCharacterMovementComponent>(GetCharacterMovement());
	
	if (SlimeCharacterMovement)
	{
		SlimeCharacterMovement->GravityScale = 1.6f;
		SlimeCharacterMovement->JumpZVelocity = 700.f;
		SlimeCharacterMovement->AirControl = 0.4f;
		SlimeCharacterMovement->JumpOffJumpZFactor = 0.1f;
		SlimeCharacterMovement->bApplyGravityWhileJumping = false;
		SlimeCharacterMovement->BrakingDecelerationFlying = 9000.f;
		SlimeCharacterMovement->bOrientRotationToMovement = true;
		SlimeCharacterMovement->StandingDownwardForceScale = 1.2f;

		SlimeCharacterMovement->OnJumpedByCharge.AddDynamic(this, &ThisClass::HandleJumpedByCharge);
		SlimeCharacterMovement->OnReachedMaxChargeForJump.AddDynamic(this, &ThisClass::HandleReachedMaxChargeForJump);
		SlimeCharacterMovement->OnStartChargingForJump.AddDynamic(this, &ThisClass::HandleStartChargingForJump);
		SlimeCharacterMovement->OnStopChargingForJump.AddDynamic(this, &ThisClass::HandleStopChargingForJump);
	}

	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->SetCapsuleHalfHeight(56.f);
		Capsule->SetCapsuleRadius(42.f);
		Capsule->SetCollisionProfileName(FallingSlimeCollisionPresets::SlimeProfileName);
	}

	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		MeshComp->SetGenerateOverlapEvents(true);
		MeshComp->SetCollisionProfileName(FallingSlimeCollisionPresets::SlimeMeshProfileName);

		FVector MeshLocation { 10.f, 0.f, -61.f };
		FRotator MeshRotation = FRotator{ 0.f, 270.f, 0.f };
		MeshComp->SetRelativeLocationAndRotation(MeshLocation, MeshRotation.Quaternion());
	}
}

bool AFallingSlimeSlimeCharacter::CanChargeForJump_Implementation()
{
	check(SlimeCharacterMovement);

	bool bCanChargeForJump = false;

	// キャラクターが歩行中であれば、チャージ可能
	bCanChargeForJump |= SlimeCharacterMovement->MovementMode == EMovementMode::MOVE_Walking;

	// キャラクターが貼りつき中であれば、チャージ可能
	bCanChargeForJump |= SlimeCharacterMovement->MovementMode == EMovementMode::MOVE_Custom && SlimeCharacterMovement->CustomMovementMode == (uint8)EFallingSlimeSlimeCharacterCustomMovementMode::Sticking;

	return bCanChargeForJump;
}

void AFallingSlimeSlimeCharacter::Metamorphose(TSubclassOf<AFallingSlimeSlimeCharacter> SlimeCharacterClassToMetamorphose)
{
	if (UWorld* World = GetWorld())
	{
		if (Controller)
		{
			UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
			// 衝突して新しいスライムのスポーン位置がずれてしまうため、コリジョンを無効化する
			CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			// Spawn slime character to metamorphose next.
			FTransform SpawnTransform = GetActorTransform();
			AFallingSlimeSlimeCharacter* SlimeCharacterToMetamorphose = World->SpawnActor<AFallingSlimeSlimeCharacter>(SlimeCharacterClassToMetamorphose, SpawnTransform);
			// 新しいスライムに速度をコピーする
			SlimeCharacterToMetamorphose->LaunchCharacter(GetVelocity(), true, true);
			SlimeCharacterToMetamorphose->AddMovementInput(ConsumeMovementInputVector());

			K2_OnMetamorphose(SlimeCharacterToMetamorphose);
			
			AController* OldController = Controller;
			const FRotator OldControlRotation = Controller->GetControlRotation();

			Controller->Possess(SlimeCharacterToMetamorphose);

			// 所有時にアクタの正面に ControlRotation を合わせてしまうため、修正する
			OldController->SetControlRotation(OldControlRotation);

			Destroy();
		}
	}
}

void AFallingSlimeSlimeCharacter::StoneLanded(const FHitResult& Hit)
{
	if (NiagaraSystem_StoneLanded)
	{
		UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
		FVector SpawnLocation = CapsuleComp->GetComponentLocation() - FVector{ 0.f, 0.f, CapsuleComp->GetScaledCapsuleHalfHeight() };
		FRotator SpawnRotation = CapsuleComp->GetComponentRotation();

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			/* WorldContextObject = */ this,
			/* SystemTemplate = */ NiagaraSystem_StoneLanded,
			SpawnLocation,
			SpawnRotation);
	}



	K2_OnStoneLanded(Hit);
}

void AFallingSlimeSlimeCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AFallingSlimePlayerController* NewPlayerController = Cast<AFallingSlimePlayerController>(NewController))
	{
		if (ULocalPlayer* LocalPlayer = NewPlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				EnhancedInputLocalPlayerSubsystem->AddMappingContext(InputMappingContext, 0);
			}
		}

		NewPlayerController->PossessCamera(PlayerCameraClassOnGround);
	}
}

// Called to bind functionality to input
void AFallingSlimeSlimeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (InputAction_Jump)
		{
			EnhancedInputComponent->BindAction(InputAction_Jump, ETriggerEvent::Started, this, &ThisClass::InputAction_Jump_Started);
			EnhancedInputComponent->BindAction(InputAction_Jump, ETriggerEvent::Completed, this, &ThisClass::InputAction_Jump_Completed);
		}

		if (InputAction_Look)
		{
			EnhancedInputComponent->BindAction(InputAction_Look, ETriggerEvent::Triggered, this, &ThisClass::InputAction_Look_Triggered);
		}

		if (InputAction_Move)
		{
			EnhancedInputComponent->BindAction(InputAction_Move, ETriggerEvent::Triggered, this, &ThisClass::InputAction_Move_Triggered);
		}
	}
}

void AFallingSlimeSlimeCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (NiagaraSystem_Landed)
	{
		UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
		FVector SpawnLocation = CapsuleComp->GetComponentLocation() - FVector{ 0.f, 0.f, CapsuleComp->GetScaledCapsuleHalfHeight() };
		FRotator SpawnRotation = CapsuleComp->GetComponentRotation();

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			/* WorldContextObject = */ this,
			/* SystemTemplate = */ NiagaraSystem_Landed,
			SpawnLocation,
			SpawnRotation);
	}

	if (SoundWave_Landed)
	{
		UGameplayStatics::PlaySound2D(this, SoundWave_Landed);
	}
}

void AFallingSlimeSlimeCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (AFallingSlimePlayerController* PC = GetController<AFallingSlimePlayerController>())
	{
		switch (SlimeCharacterMovement->MovementMode)
		{
		case EMovementMode::MOVE_Walking:
			PC->PossessCamera(PlayerCameraClassOnGround);
			break;
		case EMovementMode::MOVE_Falling:
			PC->PossessCamera(PlayerCameraClassOnAir);
			break;
		case EMovementMode::MOVE_Custom:
			switch (SlimeCharacterMovement->CustomMovementMode)
			{
			case (uint8)EFallingSlimeSlimeCharacterCustomMovementMode::StoneFalling:
				PC->PossessCamera(PlayerCameraClassOnAir);
				break;
			case (uint8)EFallingSlimeSlimeCharacterCustomMovementMode::StoneLanding:
				PC->PossessCamera(PlayerCameraClassOnGround);
				break;
			default:
				break;
			}
		}
	}
}

void AFallingSlimeSlimeCharacter::HandleStartChargingForJump_Implementation()
{
	if (!NiagaraComponent_ChargingForJump)
	{
		USkeletalMeshComponent* MeshComp = GetMesh();

		NiagaraComponent_ChargingForJump = UNiagaraFunctionLibrary::SpawnSystemAttached(
			/* SystemTemplate = */ NiagaraSystem_ChargingForJump,
			/* AttachToComponent = */ MeshComp,
			/* AttachPointName = */ NAME_None,
			/* Location = */ FVector{},
			/* Rotation = */ FRotator{},
			/* LocationType = */ EAttachLocation::KeepRelativeOffset,
			/* bAutoDestroy = */ false);
	}
}

void AFallingSlimeSlimeCharacter::HandleStopChargingForJump_Implementation()
{
	if (NiagaraComponent_ChargingForJump)
	{
		NiagaraComponent_ChargingForJump->DestroyComponent();
		NiagaraComponent_ChargingForJump = nullptr;
	}

	if (CameraShake_ReachedMaxChargeForJump)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			if (APlayerCameraManager* PlayerCameraManager = PlayerController->PlayerCameraManager)
			{
				PlayerCameraManager->StopCameraShake(CameraShake_ReachedMaxChargeForJump);
			}
		}

		CameraShake_ReachedMaxChargeForJump = nullptr;
	}
}

void AFallingSlimeSlimeCharacter::HandleReachedMaxChargeForJump_Implementation()
{
	if (NiagaraComponent_ChargingForJump)
	{
		NiagaraComponent_ChargingForJump->DestroyComponent();
		NiagaraComponent_ChargingForJump = nullptr;
	}

	if (CameraShakeClass_ReachedMaxChargeForJump.Get())
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			if (APlayerCameraManager* PlayerCameraManager = PlayerController->PlayerCameraManager)
			{
				CameraShake_ReachedMaxChargeForJump = PlayerCameraManager->StartCameraShake(CameraShakeClass_ReachedMaxChargeForJump);
			}
		}
	}

	if (NiagaraSystem_ReachedMaxChargeForJump)
	{
		USkeletalMeshComponent* MeshComp = GetMesh();

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			/* WorldContextObject = */ this,
			/* SystemTemplate = */ NiagaraSystem_ReachedMaxChargeForJump,
			/* Location = */ MeshComp->GetComponentLocation(),
			/* Rotation = */ MeshComp->GetComponentRotation());
	}
}

void AFallingSlimeSlimeCharacter::HandleJumpedByCharge_Implementation()
{
	if (NiagaraSystem_Jumped)
	{
		UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
		FVector SpawnLocation = CapsuleComp->GetComponentLocation() - FVector{ 0.f, 0.f, CapsuleComp->GetScaledCapsuleHalfHeight() };
		FRotator SpawnRotation = CapsuleComp->GetComponentRotation();

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			/* WorldContextObject = */ this,
			/* SystemTemplate = */ NiagaraSystem_Jumped,
			SpawnLocation,
			SpawnRotation);
	}
}

void AFallingSlimeSlimeCharacter::InputAction_Jump_Started(const FInputActionValue& InputActionValue)
{
	check(SlimeCharacterMovement);

	SlimeCharacterMovement->StartChargingForJump();
}

void AFallingSlimeSlimeCharacter::InputAction_Jump_Completed(const FInputActionValue& InputActionValue)
{
	check(SlimeCharacterMovement);

	SlimeCharacterMovement->JumpByCharge();
}

void AFallingSlimeSlimeCharacter::InputAction_Look_Triggered(const FInputActionValue& InputActionValue)
{
	FInputActionValue::Axis2D LookInputValue = InputActionValue.Get<FInputActionValue::Axis2D>();

	AddControllerYawInput(LookInputValue.X);
	AddControllerPitchInput(LookInputValue.Y);
}

void AFallingSlimeSlimeCharacter::InputAction_Move_Triggered(const FInputActionValue& InputActionValue)
{
	check(SlimeCharacterMovement);

	FInputActionValue::Axis2D MoveInputValue = InputActionValue.Get<FInputActionValue::Axis2D>();

	if (!SlimeCharacterMovement->IsChargingForJump())
	{
		AddMovementInput(CalcMovementInputForwardVector(), MoveInputValue.X);
		AddMovementInput(CalcMovementInputRightVector(), MoveInputValue.Y);
	}
}

FVector AFallingSlimeSlimeCharacter::CalcMovementInputForwardVector() const
{
	check(SlimeCharacterMovement);

	auto GetMovementForwardVector_Default = [this]() {
		return UKismetMathLibrary::GetForwardVector(FRotator{ 0.f, GetControlRotation().Yaw, 0.f });
		};

	switch (SlimeCharacterMovement->MovementMode)
	{
	case EMovementMode::MOVE_Custom:
		switch (SlimeCharacterMovement->CustomMovementMode)
		{
		case (uint8)EFallingSlimeSlimeCharacterCustomMovementMode::Sticking:
			return GetActorUpVector();
		default:
			return GetMovementForwardVector_Default();
		}
	default:
		return GetMovementForwardVector_Default();
	}
}

FVector AFallingSlimeSlimeCharacter::CalcMovementInputRightVector() const
{
	check(SlimeCharacterMovement);

	auto GetMovementRightVector_Default = [this]() {
		return UKismetMathLibrary::GetRightVector(FRotator{ 0.f, GetControlRotation().Yaw, 0.f });
		};

	switch (SlimeCharacterMovement->MovementMode)
	{
	case EMovementMode::MOVE_Custom:
		switch (SlimeCharacterMovement->CustomMovementMode)
		{
		case (uint8)EFallingSlimeSlimeCharacterCustomMovementMode::Sticking:
			if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
			{
				if (APlayerCameraManager* PlayerCameraManager = PlayerController->PlayerCameraManager)
				{
					FVector CameraForwardVector = UKismetMathLibrary::GetForwardVector(PlayerCameraManager->GetCameraRotation());

					// カメラとポーンのなす角が 90° 以上、270° 以下の場合、横方向の操作を反転する
					return UKismetMathLibrary::Vector_CosineAngle2D(GetActorForwardVector(), CameraForwardVector) > UKismetMathLibrary::Cos(90.f)
						? GetActorRightVector()
						: -GetActorRightVector();
				}
			}

			return GetActorRightVector();
		default:
			return GetMovementRightVector_Default();
		}
	default:
		return GetMovementRightVector_Default();
	}
}

