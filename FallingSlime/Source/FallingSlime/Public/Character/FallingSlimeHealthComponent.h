// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FallingSlimeHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthPointChanged, float, OldHealthPoint, float, NewHealthPoint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMaxHealthPointChanged, float, OldMaxHealthPoint, float, NewMaxHealthPoint);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent, DisplayName = "Falling Slime Health Component"))
class FALLINGSLIME_API UFallingSlimeHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFallingSlimeHealthComponent();

public:

	float GetHealthPoint() { return HealthPoint; }

	float GetMaxHealthPoint() { return MaxHealthPoint; }

	UFUNCTION(BlueprintCallable, Category = "FallingSlime|Health")
	float SetHealthPoint(float NewHealthPoint);

	UFUNCTION(BlueprintCallable, Category = "FallingSlime|Health")
	float SetMaxHealthPoint(float NewMaxHealthPoint);

public:

	UPROPERTY(BlueprintAssignable, Category = "FallingSlime|Health")
	FOnHealthPointChanged OnHealthPointChanged;

	UPROPERTY(BlueprintAssignable, Category = "FallingSlime|Health")
	FOnMaxHealthPointChanged OnMaxHealthPointChanged;

private:

	UPROPERTY(BlueprintReadOnly, Category = "FallingSlime|Health", EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float HealthPoint{ 0.f };

	UPROPERTY(BlueprintReadOnly, Category = "FallingSlime|Health", EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float MaxHealthPoint{ 0.f };
};
