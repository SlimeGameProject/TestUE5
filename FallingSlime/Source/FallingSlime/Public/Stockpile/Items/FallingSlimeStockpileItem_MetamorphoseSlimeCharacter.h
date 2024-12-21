// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Include headers in FallingSlime module.
#include "Character/Slime/FallingSlimeSlimeCharacter.h"
#include "Character/Slime/FallingSlimeSlimeCharacterMetamorphosisTypes.h"
#include "Stockpile/FallingSlimeStockpileItem.h"

#include "FallingSlimeStockpileItem_MetamorphoseSlimeCharacter.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGSLIME_API UFallingSlimeStockpileItem_MetamorphoseSlimeCharacter : public UFallingSlimeStockpileItem
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	TSubclassOf<AFallingSlimeSlimeCharacter> GetSlimeCharacterClassToMetamorphose() const
	{
		return MetamorphosisData ? MetamorphosisData->GetSlimeCharacterClassToMetamorphose() : nullptr;
	}

protected:

	virtual void OnReachedMaxAmount() override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Metamorphosis")
	TObjectPtr<UFallingSlimeSlimeCharacterMetamorphosisData> MetamorphosisData;
};
