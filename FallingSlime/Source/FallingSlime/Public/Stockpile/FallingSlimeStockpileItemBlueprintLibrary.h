// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "FallingSlimeStockpileComponent.h"
#include "FallingSlimeStockpileItemComponent.h"

#include "FallingSlimeStockpileItemBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGSLIME_API UFallingSlimeStockpileItemBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Stockpile")
	static bool TryStockpileItem(UFallingSlimeStockpileComponent* StockpileComponent, UFallingSlimeStockpileItemComponent* StockpileItemComponent);
};
