// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

// Include headers in FallingSlime module.
#include "FallingSlimeStockpileItem.h"

#include "FallingSlimeStockpileComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStockpileItemAdded, UFallingSlimeStockpileItem*, AddedStockpileitem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStockpileItemRemoved, UFallingSlimeStockpileItem*, RemovedStockpileitem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStockpileItemAmountChanged, UFallingSlimeStockpileItem*, StockpileItem, int, Amount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStockpileItemReachedMaxAmount, UFallingSlimeStockpileItem*, StockpileItem);

USTRUCT(BlueprintType)
struct FFallingSlimeStockpileItemEntry
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TObjectPtr<UFallingSlimeStockpileItem> StockpileItem;

public:

	inline bool operator==(FFallingSlimeStockpileItemEntry Other) const
	{
		return StockpileItem == Other.StockpileItem;
	}
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class FALLINGSLIME_API UFallingSlimeStockpileComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Stockpile")
	void StockpileItem(TSubclassOf<UFallingSlimeStockpileItem> StockpileItemClass, int Amount);

	UFUNCTION(BlueprintCallable, Category = "Stockpile")
	void UnstockpileItem(TSubclassOf<UFallingSlimeStockpileItem> StockpileItemClass, int Amount);

protected:

	void StockpileItemAdded(UFallingSlimeStockpileItem* AddedStockpileItem);

	void StockpileItemRemoved(UFallingSlimeStockpileItem* RemovedStockpileItem);

	void StockpileItemAmountChanged(UFallingSlimeStockpileItem* StockpileItem, int Amount);

	void StockpileItemReachedMaxAmount(UFallingSlimeStockpileItem* StockpileItem);

public:

	UPROPERTY(BlueprintAssignable, Category = "Stockpile")
	FOnStockpileItemAmountChanged OnStockpileItemAmountChanged;

	UPROPERTY(BlueprintAssignable, Category = "Stockpile")
	FOnStockpileItemReachedMaxAmount OnStockpileItemReachedMaxAmount;

	UPROPERTY(BlueprintAssignable, Category = "Stockpile")
	FOnStockpileItemAdded OnStockpileItemAdded;

	UPROPERTY(BlueprintAssignable, Category = "Stockpile")
	FOnStockpileItemRemoved OnStockpileItemRemoved;

private:

	UPROPERTY()
	TArray<FFallingSlimeStockpileItemEntry> StockpileItems;
};
