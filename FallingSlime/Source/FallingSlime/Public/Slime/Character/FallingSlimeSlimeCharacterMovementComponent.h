// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Include headers in Engine module.
#include "GameFramework/CharacterMovementComponent.h"

#include "FallingSlimeSlimeCharacterMovementComponent.generated.h"

class AFallingSlimeSlimeCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartChargingForJump);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopChargingForJump);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJumpedByCharge);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReachedMaxChargeForJump);

UCLASS(meta = (DisplayName = "Slime Character Movement"))
class FALLINGSLIME_API UFallingSlimeSlimeCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	UFallingSlimeSlimeCharacterMovementComponent(const FObjectInitializer& ObjectInitializer);

	// Begin UActorComponent interfaces.

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// End UActorComponent interfaces.

	// Begin UCharacterMovementComponent interfaces.

protected:

	virtual void PhysCustom(float DeltaTime, int32 Iterations) override;

	// End UCharacterMovementComponent interfaces.

public:

	UFUNCTION(BlueprintCallable, Category = "FallingSlime|SlimeCharacterMovement")
	bool IsChargingForJump() const { return bIsChargingForJump; }

	UFUNCTION(BlueprintCallable, Category = "FallingSlime|SlimeCharacterMovement")
	void StartChargingForJump();

	UFUNCTION(BlueprintCallable, Category = "FallingSlime|SlimeCharacterMovement")
	void StopChargingForJump();

	UFUNCTION(BlueprintCallable, Category = "FallingSlime|SlimeCharacterMovement")
	void JumpByCharge();

protected:

	virtual void PhysSticking(float DeltaTime, int32 Iterations);

	virtual void PhysStoneLanding(float DeltaTime, int32 Iterations);

	void MoveAlongFloor2(const FVector& InVelocity, float DeltaSeconds, FStepDownResult* OutStepDownResult);

	virtual void PhysStoneFalling(float DeltaTime, int32 Iterations);

	virtual void ProcessStoneLanded(const FHitResult& Hit, float RemainingTime, int32 Iterations);

	virtual void SetPostStoneLandedPhysics(const FHitResult& Hit);

public:

	UPROPERTY(BlueprintReadWrite, Category = "FallingSlime|SlimeCharacterMovement", EditAnywhere)
	float JumpBaseSpeed{ 1100.f };

	UPROPERTY(BlueprintReadWrite, Category = "FallingSlime|SlimeCharacterMovement", EditAnywhere)
	float JumpAdditionalSpeed{ 1000.f };

	UPROPERTY(BlueprintReadWrite, Category = "FallingSlime|SlimeCharacterMovement", EditAnywhere)
	float MaxChargingTimeInSecondsForJump{ 3.f };

	UPROPERTY(BlueprintAssignable, Category = "FallingSlime|SlimeCharacterMovement")
	FOnJumpedByCharge OnJumpedByCharge;

	UPROPERTY(BlueprintAssignable, Category = "FallingSlime|SlimeCharacterMovement")
	FOnReachedMaxChargeForJump OnReachedMaxChargeForJump;

	UPROPERTY(BlueprintAssignable, Category = "FallingSlime|SlimeCharacterMovement")
	FOnStartChargingForJump OnStartChargingForJump;

	UPROPERTY(BlueprintAssignable, Category = "FallingSlime|SlimeCharacterMovement")
	FOnStopChargingForJump OnStopChargingForJump;

	UPROPERTY(BlueprintReadWrite, Category = "FallingSlime|SlimeCharacterMovement", EditAnywhere)
	float Stone_FallVelocityZ{ 2500.f };

private:

	float ChargingTimeInSecondsForJump{ 0.f };

	bool bIsChargingForJump{ false };
};
