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
	FVector JumpForwardDirection = FVector{ PawnOwnerForwardVector.X, PawnOwnerForwardVector.Y, 0.f }.GetClampedToMaxSize(0.6f);
	FVector JumpUpDirection = FVector{ 0.f, 0.f, 0.4f };

	Launch((JumpForwardDirection + JumpUpDirection) * JumpSpeed);

	OnJumpedByCharge.Broadcast();

	StopChargingForJump();
}

bool UFallingSlimeSlimeCharacterMovementComponent::CheckStoneFall(const FFindFloorResult& OldFloor, const FHitResult& Hit, const FVector& Delta, const FVector& OldLocation, float remainingTime, float timeTick, int32 Iterations, bool bMustJump)
{
	if (!HasValidData())
	{
		return false;
	}

	if (bMustJump || CanWalkOffLedges())
	{
		HandleWalkingOffLedge(OldFloor.HitResult.ImpactNormal, OldFloor.HitResult.Normal, OldLocation, timeTick);

		if (UpdatedComponent && MovementMode == EMovementMode::MOVE_Custom && CustomMovementMode == (uint8)EFallingSlimeSlimeCharacterCustomMovementMode::StoneLanding)
		{
			// If still walking, then fall. If not, assume the user set a different mode they want to keep.
			StartStoneFalling(Iterations, remainingTime, timeTick, Delta, OldLocation);
		}

		return true;
	}
	return false;
}

void UFallingSlimeSlimeCharacterMovementComponent::PhysSticking(float DeltaTime, int32 Iterations)
{
	// 張り付き機能は必要なるタイミングまで後回しにする
}

void UFallingSlimeSlimeCharacterMovementComponent::PhysStoneLanding(float DeltaTime, int32 Iterations)
{
	if (DeltaTime < MIN_TICK_TIME)
	{
		return;
	}

	if (!CharacterOwner || (!CharacterOwner->Controller && !bRunPhysicsWithNoController && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() && (CharacterOwner->GetLocalRole() != ROLE_SimulatedProxy)))
	{
		Acceleration = FVector::ZeroVector;
		Velocity = FVector::ZeroVector;
		return;
	}

	if (!UpdatedComponent->IsQueryCollisionEnabled())
	{
		return;
	}

	bJustTeleported = false;
	bool bCheckedFall = false;
	bool bTriedLedgeMove = false;
	float RemainingTime = 0.f;

	Iterations = 1;

	const float TimeTick = DeltaTime;

	// Save current values
	UPrimitiveComponent* const OldBase = GetMovementBase();
	const FVector PreviousBaseLocation = (OldBase != NULL) ? OldBase->GetComponentLocation() : FVector::ZeroVector;
	const FVector OldLocation = UpdatedComponent->GetComponentLocation();
	const FFindFloorResult OldFloor = CurrentFloor;

	FStepDownResult StepDownResult;

	// Update floor.
	// StepUp might have already done it for us.
	if (StepDownResult.bComputedFloor)
	{
		CurrentFloor = StepDownResult.FloorResult;
	}
	else
	{
		FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, true, NULL);
	}

	const bool bCheckLedges = !CanWalkOffLedges();

	if (!CurrentFloor.IsWalkableFloor())
	{
		// see if it is OK to jump
		// @todo collision : only thing that can be problem is that oldbase has world collision on
		bool bMustJump = (OldBase == NULL || (!OldBase->IsQueryCollisionEnabled() && MovementBaseUtility::IsDynamicBase(OldBase)));

		if ((bMustJump || !bCheckedFall) && CheckStoneFall(OldFloor, CurrentFloor.HitResult, FVector::ZeroVector, OldLocation, RemainingTime, TimeTick, Iterations, bMustJump))
		{
			return;
		}

		// revert this move
		RevertMove(OldLocation, OldBase, PreviousBaseLocation, OldFloor, true);
		RemainingTime = 0.f;

		return;
	}

	// Validate the floor check
	if (CurrentFloor.IsWalkableFloor())
	{
		if (ShouldCatchAir(OldFloor, CurrentFloor))
		{
			HandleWalkingOffLedge(OldFloor.HitResult.ImpactNormal, OldFloor.HitResult.Normal, OldLocation, TimeTick);

			// If still walking, then fall. If not, assume the user set a different mode they want to keep.
			StartStoneFalling(Iterations, RemainingTime, TimeTick, FVector::ZeroVector, OldLocation);

			return;
		}

		AdjustFloorHeight();
		SetBase(CurrentFloor.HitResult.Component.Get(), CurrentFloor.HitResult.BoneName);
	}
	else if (CurrentFloor.HitResult.bStartPenetrating)
	{
		// The floor check failed because it started in penetration
		// We do not want to try to move downward because the downward sweep failed, rather we'd like to try to pop out of the floor.
		FHitResult Hit(CurrentFloor.HitResult);
		Hit.TraceEnd = Hit.TraceStart + FVector(0.f, 0.f, MAX_FLOOR_DIST);
		const FVector RequestedAdjustment = GetPenetrationAdjustment(Hit);

		ResolvePenetration(RequestedAdjustment, Hit, UpdatedComponent->GetComponentQuat());
	}

	// Make velocity reflect actual move
	if (!bJustTeleported && TimeTick >= MIN_TICK_TIME)
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / TimeTick;
	}
	
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
	// start falling 
	const float DesiredDist = Delta.Size();
	const float ActualDist = (UpdatedComponent->GetComponentLocation() - SubLocation).Size2D();
	RemainingTime = (DesiredDist < UE_KINDA_SMALL_NUMBER)
		? 0.f
		: RemainingTime + TimeTick * (1.f - FMath::Min(1.f, ActualDist / DesiredDist));

	SetMovementMode(MOVE_Custom, (uint8)EFallingSlimeSlimeCharacterCustomMovementMode::StoneFalling);

	StartNewPhysics(RemainingTime, Iterations);
}
