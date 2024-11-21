// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

// Include headers in Niagara module.
#include "NiagaraSystem.h"

// Include headers in FallingSlime module.
#include "FallingSlimeSlimeCharacter.h"

#include "FallingSlimeSlimeCharacterMetamorphosisData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class FALLINGSLIME_API UFallingSlimeSlimeCharacterMetamorphosisData : public UDataAsset
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	TSubclassOf<AFallingSlimeSlimeCharacter> GetSlimeCharacterClassToMetamorphose() const { return SlimeCharacterClassToMetamorphose; }

	UFUNCTION(BlueprintCallable)
	UNiagaraSystem* GetNiagaraSystem_Metamorphosed() const { return NiagaraSystem_Metamorphosed; }

private:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AFallingSlimeSlimeCharacter> SlimeCharacterClassToMetamorphose;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> NiagaraSystem_Metamorphosed { nullptr };
};
