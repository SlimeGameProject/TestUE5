// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "FallingSlimeStockpileItem.h"

#include "FallingSlimeStockpileItemDefinition.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FALLINGSLIME_API UFallingSlimeStockpileItemDefinition : public UDataAsset
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Stockpile")
	TSubclassOf<UFallingSlimeStockpileItem> GetStockpileItemClass() const { return StockpileItemClass; }

private:

	UPROPERTY(EditInstanceOnly, Category = "Stockpile")
	TSubclassOf<UFallingSlimeStockpileItem> StockpileItemClass;
};
