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

UCLASS(meta = (DisplayName = "SlimeCharacterMovement"))
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

	UFUNCTION(BlueprintCallable, Category = "JumpByCharge")
	bool IsChargingForJump() const { return bIsChargingForJump; }

	UFUNCTION(BlueprintCallable, Category = "JumpByCharge")
	void StartChargingForJump();

	UFUNCTION(BlueprintCallable, Category = "JumpByCharge")
	void StopChargingForJump();

	UFUNCTION(BlueprintCallable, Category = "JumpByCharge")
	void JumpByCharge();

protected:

	virtual void PhysSticking(float DeltaTime, int32 Iterations);

	virtual void PhysStoneLanding(float DeltaTime, int32 Iterations);

	virtual void PhysStoneFalling(float DeltaTime, int32 Iterations);

	virtual void ProcessStoneLanded(const FHitResult& Hit, float RemainingTime, int32 Iterations);

	virtual void SetPostStoneLandedPhysics(const FHitResult& Hit);

	void StartStoneFalling(int32 Iterations, float remainingTime, float timeTick, const FVector& Delta, const FVector& subLoc);

public:

	UPROPERTY(BlueprintReadWrite, Category = "JumpByCharge", EditAnywhere)
	float JumpBaseSpeed{ 1100.f };

	UPROPERTY(BlueprintReadWrite, Category = "JumpByCharge", EditAnywhere)
	float JumpAdditionalSpeed{ 1000.f };

	// ジャンプ方向における進行方向と上方向の成分の比率（０～１）
	// ０の場合、ワールド座標系におけるZ軸の正の向きにジャンプする。
	// １の場合、ワールド座標系におけるスライムのXY平面における正面方向にジャンプする。
	UPROPERTY(BlueprintReadWrite, Category = "JumpByCharge", EditAnywhere)
	float JumpForwardDirectionRate = 0.6f;

	UPROPERTY(BlueprintReadWrite, Category = "JumpByCharge", EditAnywhere)
	float MaxChargingTimeInSecondsForJump{ 3.f };

	UPROPERTY(BlueprintAssignable, Category = "JumpByCharge")
	FOnJumpedByCharge OnJumpedByCharge;

	UPROPERTY(BlueprintAssignable, Category = "JumpByCharge")
	FOnReachedMaxChargeForJump OnReachedMaxChargeForJump;

	UPROPERTY(BlueprintAssignable, Category = "JumpByCharge")
	FOnStartChargingForJump OnStartChargingForJump;

	UPROPERTY(BlueprintAssignable, Category = "JumpByCharge")
	FOnStopChargingForJump OnStopChargingForJump;

	UPROPERTY(BlueprintReadWrite, Category = "StoneFalling", EditAnywhere)
	float Stone_FallVelocityZ{ 2500.f };

private:

	float ChargingTimeInSecondsForJump{ 0.f };

	bool bIsChargingForJump{ false };
};
