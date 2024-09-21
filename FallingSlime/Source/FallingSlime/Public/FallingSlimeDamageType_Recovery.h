// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"

#include "FallingSlimeDamageType_Recovery.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGSLIME_API UFallingSlimeDamageType_Recovery : public UDamageType
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "FallingSlime|Health")
	float TakeRecovery(AActor* RecoveriedActor, float RecoveryAmount, AController* InvestigatedBy, AActor* RecoveryCauser);
};
