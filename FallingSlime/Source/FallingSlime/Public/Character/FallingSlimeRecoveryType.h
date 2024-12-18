// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"

#include "FallingSlimeRecoveryType.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Falling Slime Recovery Type"))
class FALLINGSLIME_API UFallingSlimeRecoveryType : public UDamageType
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "FallingSlime|Health")
	float TakeRecovery(AActor* RecoveriedActor, float RecoveryAmount, AController* InvestigatedBy, AActor* RecoveryCauser);
};
