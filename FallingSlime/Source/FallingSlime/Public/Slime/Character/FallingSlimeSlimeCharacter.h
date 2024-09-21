// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Include headers in Engine module.
#include "GameFramework/Character.h"

// Include headers in EnhancedInput module.
#include "InputAction.h"
#include "InputMappingContext.h"

// Include headers in FallingSlime module.
#include "FallingSlimeSlimeCharacterMovementComponent.h"
#include "Player/FallingSlimePlayerCamera.h"

// Include headers in Niagara module.
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

#include "FallingSlimeSlimeCharacter.generated.h"

UCLASS()
class FALLINGSLIME_API AFallingSlimeSlimeCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AFallingSlimeSlimeCharacter(const FObjectInitializer& ObjectInitializer);

	// Begin APawn interfaces.

public:

	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// End APawn interfaces.

	// Begin ACharacter interfaces.

protected:

	virtual void Landed(const FHitResult& Hit) override;

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0);

	// End ACharacter interfaces.

public:

	UFallingSlimeSlimeCharacterMovementComponent* GetSlimeCharacterMovement() const { return SlimeCharacterMovement; }

	UFUNCTION(BlueprintNativeEvent, Category = "FallingSlime|SlimeCharacter", DisplayName = "CanChargeForJump")
	bool CanChargeForJump() const;

protected:

	virtual bool CanChargeForJump_Implementation();

protected:

	UFUNCTION(BlueprintNativeEvent, Category = "FallingSlime|SlimeCharacter", DisplayName = "OnStartChargingForJump")
	void HandleStartChargingForJump();

	virtual void HandleStartChargingForJump_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "FallingSlime|SlimeCharacter", DisplayName = "OnStopChargingForJump")
	void HandleStopChargingForJump();

	virtual void HandleStopChargingForJump_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "FallingSlime|SlimeCharacter", DisplayName = "OnReachedMaxChargeForJump")
	void HandleReachedMaxChargeForJump();

	virtual void HandleReachedMaxChargeForJump_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "FallingSlime|SlimeCharacter", DisplayName = "OnJumpedByCharge")
	void HandleJumpedByCharge();

	virtual void HandleJumpedByCharge_Implementation();

public:

	UFUNCTION(BlueprintCallable, Category = "FallingSlime|SlimeCharacter")
	void Metamorphose(TSubclassOf<AFallingSlimeSlimeCharacter> SlimeCharacterClassToMetamorphose);

	// Begin Interfaces used AFallingSlimeSlimeCharacter metamorphose into stone. 

public:

	virtual void StoneLanded(const FHitResult& Hit);

protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "FallingSlime|SlimeCharacter", DisplayName = "OnStoneLanded")
	void K2_OnStoneLanded(const FHitResult& Hit);

	// End Interfaces used AFallingSlimeSlimeCharacter metamorphose into stone.

private:

	void InputAction_Jump_Started(const FInputActionValue& InputActionValue);

	void InputAction_Jump_Completed(const FInputActionValue& InputActionValue);

	void InputAction_Look_Triggered(const FInputActionValue& InputActionValue);

	void InputAction_Move_Triggered(const FInputActionValue& InputActionValue);

	FVector CalcMovementInputForwardVector() const;

	FVector CalcMovementInputRightVector() const;

	// Begin components.

private:

	UPROPERTY(BlueprintReadOnly, Category = "FallingSlime|SlimeCharacter", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UFallingSlimeSlimeCharacterMovementComponent> SlimeCharacterMovement;

	// End components.

public:

	UPROPERTY(BlueprintReadWrite, Category = "FallingSlime|SlimeCharacter", EditDefaultsOnly)
	TSubclassOf<AFallingSlimePlayerCamera> PlayerCameraClassOnGround{ nullptr };

	UPROPERTY(BlueprintReadWrite, Category = "FallingSlime|SlimeCharacter", EditDefaultsOnly)
	TSubclassOf<AFallingSlimePlayerCamera> PlayerCameraClassOnAir{ nullptr };

private:

	UPROPERTY(Category = "FallingSlime|SlimeCharacter", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> InputMappingContext{ nullptr };

	UPROPERTY(Category = "FallingSlime|SlimeCharacter", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InputAction_Jump{ nullptr };

	UPROPERTY(Category = "FallingSlime|SlimeCharacter", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InputAction_Look{ nullptr };

	UPROPERTY(Category = "FallingSlime|SlimeCharacter", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InputAction_Move{ nullptr };

	UPROPERTY(Category = "FallingSlime|SlimeCharacter", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> NiagaraSystem_ChargingForJump{ nullptr };

	UPROPERTY(Category = "FallingSlime|SlimeCharacter", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> NiagaraSystem_Jumped{ nullptr };

	UPROPERTY(Category = "FallingSlime|SlimeCharacter", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> NiagaraSystem_Landed{ nullptr };

	UPROPERTY(Category = "FallingSlime|SlimeCharacter", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> NiagaraSystem_ReachedMaxChargeForJump{ nullptr };

	UPROPERTY(Category = "FallingSlime|SlimeCharacter", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> NiagaraSystem_StoneLanded{ nullptr };

	UPROPERTY(Category = "FallingSlime|SlimeCharacter", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> CameraShakeClass_ReachedMaxChargeForJump{ nullptr };

	UPROPERTY(Category = "FallingSlime|SlimeCharacter", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundWave> SoundWave_Landed{ nullptr };

private:

	UPROPERTY(BlueprintReadOnly, Category = "FallingSlime|SlimeCharacter", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> NiagaraComponent_ChargingForJump{ nullptr };

	UPROPERTY(BlueprintReadOnly, Category = "FallingSlime|SlimeCharacter", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraShakeBase> CameraShake_ReachedMaxChargeForJump{ nullptr };
};
