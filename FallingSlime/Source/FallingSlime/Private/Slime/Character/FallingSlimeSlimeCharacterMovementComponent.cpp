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
		SetMovementMode(MOVE_Walking);
		return;
	}

	bJustTeleported = false;
	bool bCheckedFall = false;
	bool bTriedLedgeMove = false;
	float remainingTime = 0.f;

	Iterations++;

	bJustTeleported = false;

	const float timeTick = DeltaTime;

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

	// Validate the floor check
	if (CurrentFloor.IsWalkableFloor())
	{
		if (ShouldCatchAir(OldFloor, CurrentFloor))
		{
			HandleWalkingOffLedge(OldFloor.HitResult.ImpactNormal, OldFloor.HitResult.Normal, OldLocation, timeTick);

			if (IsMovingOnGround())
			{
				// If still walking, then fall. If not, assume the user set a different mode they want to keep.
				StartFalling(Iterations, remainingTime, timeTick, FVector::ZeroVector, OldLocation);
			}

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
	if (!bJustTeleported && timeTick >= MIN_TICK_TIME)
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / timeTick;
	}
	
	MaintainHorizontalGroundVelocity();
}


void UFallingSlimeSlimeCharacterMovementComponent::MoveAlongFloor2(const FVector& InVelocity, float DeltaSeconds, FStepDownResult* OutStepDownResult)
{
	if (!CurrentFloor.IsWalkableFloor())
	{
		return;
	}

	// Move along the current floor
	const FVector Delta = FVector(InVelocity.X, InVelocity.Y, 0.f) * DeltaSeconds;
	FHitResult Hit(1.f);
	FVector RampVector = ComputeGroundMovementDelta(Delta, CurrentFloor.HitResult, CurrentFloor.bLineTrace);
	SafeMoveUpdatedComponent(RampVector, UpdatedComponent->GetComponentQuat(), true, Hit);
	float LastMoveTimeSlice = DeltaSeconds;

	if (Hit.bStartPenetrating)
	{
		// Allow this hit to be used as an impact we can deflect off, otherwise we do nothing the rest of the update and appear to hitch.
		HandleImpact(Hit);
		SlideAlongSurface(Delta, 1.f, Hit.Normal, Hit, true);

		if (Hit.bStartPenetrating)
		{
			OnCharacterStuckInGeometry(&Hit);
		}
	}
	else if (Hit.IsValidBlockingHit())
	{
		// We impacted something (most likely another ramp, but possibly a barrier).
		float PercentTimeApplied = Hit.Time;
		if ((Hit.Time > 0.f) && (Hit.Normal.Z > UE_KINDA_SMALL_NUMBER) && IsWalkable(Hit))
		{
			// Another walkable ramp.
			const float InitialPercentRemaining = 1.f - PercentTimeApplied;
			RampVector = ComputeGroundMovementDelta(Delta * InitialPercentRemaining, Hit, false);
			LastMoveTimeSlice = InitialPercentRemaining * LastMoveTimeSlice;
			SafeMoveUpdatedComponent(RampVector, UpdatedComponent->GetComponentQuat(), true, Hit);

			const float SecondHitPercent = Hit.Time * InitialPercentRemaining;
			PercentTimeApplied = FMath::Clamp(PercentTimeApplied + SecondHitPercent, 0.f, 1.f);
		}

		if (Hit.IsValidBlockingHit())
		{
			if (CanStepUp(Hit) || (CharacterOwner->GetMovementBase() != nullptr && Hit.HitObjectHandle == CharacterOwner->GetMovementBase()->GetOwner()))
			{
				// hit a barrier, try to step up
				const FVector PreStepUpLocation = UpdatedComponent->GetComponentLocation();
				const FVector GravDir(0.f, 0.f, -1.f);
				if (!StepUp(GravDir, Delta * (1.f - PercentTimeApplied), Hit, OutStepDownResult))
				{
					HandleImpact(Hit, LastMoveTimeSlice, RampVector);
					SlideAlongSurface(Delta, 1.f - PercentTimeApplied, Hit.Normal, Hit, true);
				}
				else
				{
					if (!bMaintainHorizontalGroundVelocity)
					{
						// Don't recalculate velocity based on this height adjustment, if considering vertical adjustments. Only consider horizontal movement.
						bJustTeleported = true;
						const float StepUpTimeSlice = (1.f - PercentTimeApplied) * DeltaSeconds;
						if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() && StepUpTimeSlice >= UE_KINDA_SMALL_NUMBER)
						{
							Velocity = (UpdatedComponent->GetComponentLocation() - PreStepUpLocation) / StepUpTimeSlice;
							Velocity.Z = 0;
						}
					}
				}
			}
			else if (Hit.Component.IsValid() && !Hit.Component.Get()->CanCharacterStepUp(CharacterOwner))
			{
				HandleImpact(Hit, LastMoveTimeSlice, RampVector);
				SlideAlongSurface(Delta, 1.f - PercentTimeApplied, Hit.Normal, Hit, true);
			}
		}
	}
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
	if (AFallingSlimeSlimeCharacter* SlimeCharacterOwner = Cast<AFallingSlimeSlimeCharacter>(CharacterOwner))
	{
		/*const FVector PreImpactAccel = Acceleration + (IsFalling() ? -GetGravityDirection() * GetGravityZ() : FVector::ZeroVector);
		const FVector PreImpactVelocity = Velocity;*/

		SetMovementMode(EMovementMode::MOVE_Custom, (uint8)EFallingSlimeSlimeCharacterCustomMovementMode::StoneLanding);

		//ApplyImpactPhysicsForces(Hit, PreImpactAccel, PreImpactVelocity);
	}
}
