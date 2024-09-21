// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/FallingSlimeHealthComponent.h"

// Include headers in Engine module.
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UFallingSlimeHealthComponent::UFallingSlimeHealthComponent()
	: Super()
{
	PrimaryComponentTick.bCanEverTick = false;
}

float UFallingSlimeHealthComponent::SetHealthPoint(float NewHealthPoint)
{
	const float OldHealthPoint = HealthPoint;

	HealthPoint = UKismetMathLibrary::FClamp(NewHealthPoint, 0.f, MaxHealthPoint);

	if (!UKismetMathLibrary::NearlyEqual_FloatFloat(OldHealthPoint, HealthPoint))
	{
		OnHealthPointChanged.Broadcast(OldHealthPoint, HealthPoint);
	}

	return HealthPoint;
}

float UFallingSlimeHealthComponent::SetMaxHealthPoint(float NewMaxHealthPoint)
{
	const float OldMaxHealthPoint = MaxHealthPoint;

	MaxHealthPoint = UKismetMathLibrary::FMax(NewMaxHealthPoint, 0.f);

	if (!UKismetMathLibrary::NearlyEqual_FloatFloat(OldMaxHealthPoint, MaxHealthPoint))
	{
		OnMaxHealthPointChanged.Broadcast(OldMaxHealthPoint, MaxHealthPoint);

		if (HealthPoint > MaxHealthPoint)
		{
			SetHealthPoint(MaxHealthPoint);
		}
	}

	return MaxHealthPoint;
}
