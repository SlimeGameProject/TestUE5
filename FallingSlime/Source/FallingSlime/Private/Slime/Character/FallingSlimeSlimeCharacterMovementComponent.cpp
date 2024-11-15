// Fill out your copyright notice in the Description page of Project Settings.

#include "Slime/Character/FallingSlimeSlimeCharacterMovementComponent.h"

// Include headers in Engine module.
#include "Kismet/KismetMathLibrary.h"

// Include headers in FallingSlime module.
#include "Slime/Character/FallingSlimeSlimeCharacter.h"
#include "Slime/Character/FallingSlimeSlimeCharacterMovementTypes.h"

UFallingSlimeSlimeCharacterMovementComponent::UFallingSlimeSlimeCharacterMovementComponent(const FObjectInitializer& ObjectInitializer)
{

}

void UFallingSlimeSlimeCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsChargingForJump && !UKismetMathLibrary::NearlyEqual_FloatFloat(ChargingTimeInSecondsForJump, MaxChargingTimeInSecondsForJump))
	{
		ChargingTimeInSecondsForJump += DeltaTime;

		if (ChargingTimeInSecondsForJump > MaxChargingTimeInSecondsForJump)
		{
			ChargingTimeInSecondsForJump = MaxChargingTimeInSecondsForJump;

			OnReachedMaxChargeForJump.Broadcast();
		}
	}
}

void UFallingSlimeSlimeCharacterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
	Super::PhysCustom(DeltaTime, Iterations);

	switch (CustomMovementMode)
	{
	case (uint8)EFallingSlimeSlimeCharacterCustomMovementMode::Sticking:
		PhysSticking(DeltaTime, Iterations);
		break;
	case (uint8)EFallingSlimeSlimeCharacterCustomMovementMode::StoneFalling:
		PhysStoneFalling(DeltaTime, Iterations);
		break;
	case (uint8)EFallingSlimeSlimeCharacterCustomMovementMode::StoneLanding:
		PhysStoneLanding(DeltaTime, Iterations);
		break;
	}
}

void UFallingSlimeSlimeCharacterMovementComponent::StartChargingForJump()
{
	if (IsChargingForJump())
	{
		return;
	}

	AFallingSlimeSlimeCharacter* SlimeCharacterOwner = Cast<AFallingSlimeSlimeCharacter>(GetCharacterOwner());

	if (SlimeCharacterOwner && SlimeCharacterOwner->CanChargeForJump())
	{
		bIsChargingForJump = true;
		ChargingTimeInSecondsForJump = 0.f;

		OnStartChargingForJump.Broadcast();
	}
}

void UFallingSlimeSlimeCharacterMovementComponent::StopChargingForJump()
{
	if (!IsChargingForJump())
	{
		return;
	}

	bIsChargingForJump = false;
	ChargingTimeInSecondsForJump = 0.f;

	OnStopChargingForJump.Broadcast();
}

void UFallingSlimeSlimeCharacterMovementComponent::JumpByCharge()
{
	if (!IsChargingForJump())
	{
		return;
	}

	float JumpSpeed = JumpBaseSpeed + JumpAdditionalSpeed * (ChargingTimeInSecondsForJump / MaxChargingTimeInSecondsForJump);

	FVector PawnOwnerForwardVector = UKismetMathLibrary::GetForwardVector(PawnOwner->GetControlRotation());
	FVector JumpForwardDirection = FVector{ PawnOwnerForwardVector.X, PawnOwnerForwardVector.Y, 0.f }.GetClampedToMaxSize(JumpForwardDirectionRate);
	FVector JumpUpDirection = FVector{ 0.f, 0.f, 1 - JumpForwardDirectionRate };

	Launch((JumpForwardDirection + JumpUpDirection) * JumpSpeed);

	OnJumpedByCharge.Broadcast();

	StopChargingForJump();
}

void UFallingSlimeSlimeCharacterMovementComponent::PhysSticking(float DeltaTime, int32 Iterations)
{
	// 張り付き機能は必要なるタイミングまで後回しにしておく
}

void UFallingSlimeSlimeCharacterMovementComponent::PhysStoneLanding(float DeltaTime, int32 Iterations)
{
	if (DeltaTime < MIN_TICK_TIME)
	{
		return;
	}

	if (!CharacterOwner || (!CharacterOwner->Controller && !bRunPhysicsWithNoController))
	{
		Acceleration = FVector::ZeroVector;
		Velocity = FVector::ZeroVector;

		return;
	}

	if (!UpdatedComponent || !UpdatedComponent->IsQueryCollisionEnabled())
	{
		return;
	}

	Iterations = 1;
	const float TimeTick = DeltaTime;
	float RemainingTime = 0.f;

	// UCharacterMovementComponent の実装に倣って、現在の値を保持しておく（以降の関数呼び出しによって値が変化する可能性がある）
	UPrimitiveComponent* const OldBase = GetMovementBase();
	const FVector OldLocation = UpdatedComponent->GetComponentLocation();
	const FFindFloorResult OldFloor = CurrentFloor;

	// 地面の情報を更新する
	FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, true, NULL);

	if (!CurrentFloor.IsWalkableFloor())
	{
		// 床がなければ、岩状態での落下を開始する
		StartStoneFalling(Iterations, RemainingTime, TimeTick, FVector::ZeroVector, OldLocation);

		return;
	}

	AdjustFloorHeight();
	SetBase(CurrentFloor.HitResult.Component.Get(), CurrentFloor.HitResult.BoneName);

	// 移動した距離に合わせて速度を計算する
	Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / TimeTick;
	
	MaintainHorizontalGroundVelocity();
}

void UFallingSlimeSlimeCharacterMovementComponent::PhysStoneFalling(float DeltaTime, int32 Iterations)
{
	if (DeltaTime < MIN_TICK_TIME)
	{
		return;
	}

	float RemainingTime = DeltaTime;

	while ((RemainingTime >= MIN_TICK_TIME) && (Iterations < MaxSimulationIterations))
	{
		Iterations++;

		float TimeTick = GetSimulationTimeStep(RemainingTime, Iterations);
		RemainingTime -= TimeTick;

		const FQuat PawnRotation = UpdatedComponent->GetComponentQuat();

		Velocity = FVector{ 0.f, 0.f, -Stone_FallVelocityZ };

		FHitResult Hit(1.f);
		SafeMoveUpdatedComponent(Velocity * TimeTick, PawnRotation, true, Hit);

		if (!HasValidData())
		{
			return;
		}

		float SubTimeTickRemaining = TimeTick * (1.f - Hit.Time);

		if (Hit.bBlockingHit && IsValidLandingSpot(UpdatedComponent->GetComponentLocation(), Hit))
		{
			// 地面に衝突した場合、StoneLanding に移行する
			RemainingTime += SubTimeTickRemaining;
			ProcessStoneLanded(Hit, RemainingTime, Iterations);

			return;
		}
	}
}

void UFallingSlimeSlimeCharacterMovementComponent::ProcessStoneLanded(const FHitResult& Hit, float RemainingTime, int32 Iterations)
{
	if (AFallingSlimeSlimeCharacter* SlimeCharacterOwner = Cast<AFallingSlimeSlimeCharacter>(CharacterOwner))
	{
		SlimeCharacterOwner->StoneLanded(Hit);
	}

	if (MovementMode == EMovementMode::MOVE_Custom && CustomMovementMode == (uint8)EFallingSlimeSlimeCharacterCustomMovementMode::StoneFalling)
	{
		SetPostStoneLandedPhysics(Hit);
	}

	StartNewPhysics(RemainingTime, Iterations);
}

void UFallingSlimeSlimeCharacterMovementComponent::SetPostStoneLandedPhysics(const FHitResult& Hit)
{
	SetMovementMode(EMovementMode::MOVE_Custom, (uint8)EFallingSlimeSlimeCharacterCustomMovementMode::StoneLanding);
}

void UFallingSlimeSlimeCharacterMovementComponent::StartStoneFalling(int32 Iterations, float RemainingTime, float TimeTick, const FVector& Delta, const FVector& SubLocation)
{
	const float DesiredDist = Delta.Size();
	const float ActualDist = (UpdatedComponent->GetComponentLocation() - SubLocation).Size2D();
	RemainingTime = (DesiredDist < UE_KINDA_SMALL_NUMBER)
		? 0.f
		: RemainingTime + TimeTick * (1.f - FMath::Min(1.f, ActualDist / DesiredDist));

	SetMovementMode(MOVE_Custom, (uint8)EFallingSlimeSlimeCharacterCustomMovementMode::StoneFalling);

	StartNewPhysics(RemainingTime, Iterations);
}
