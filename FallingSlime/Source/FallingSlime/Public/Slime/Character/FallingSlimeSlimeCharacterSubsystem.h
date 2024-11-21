// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"

// Include headers in FallingSlime module.
#include "FallingSlimeSlimeCharacter.h"
#include "FallingSlimeSlimeCharacterMetamorphosisData.h"

#include "FallingSlimeSlimeCharacterSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGSLIME_API UFallingSlimeSlimeCharacterMetamorphosisSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	AFallingSlimeSlimeCharacter* MetamorphoseSlimeCharacter(AFallingSlimeSlimeCharacter* OriginalSlimeCharacter, const UFallingSlimeSlimeCharacterMetamorphosisData* MetamorphosisData);
};
