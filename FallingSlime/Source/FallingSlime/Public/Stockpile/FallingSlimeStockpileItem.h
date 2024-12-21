// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "FallingSlimeStockpileItem.generated.h"

class UFallingSlimeStockpileComponent;

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class FALLINGSLIME_API UFallingSlimeStockpileItem : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Stockpile")
	int GetAmount() const { return Amount; }

	UFUNCTION(BlueprintCallable, Category = "Stockpile")
	int GetMaxAmount() const { return MaxAmount; }

	int IncreaseAmount(int AmountToIncrease);

	int DecreaseAmount(int AmountToDecrease);

protected:

	void SetAmount(int NewAmount);

	virtual void OnAmountChanged(int DeltaAmount) {}

	virtual void OnReachedMaxAmount() {}

	UFUNCTION(BlueprintImplementableEvent, Category = "Stockpile", DisplayName = "OnAmountChanged")
	void K2_OnAmountChanged(int DeltaAmount);

	UFUNCTION(BlueprintImplementableEvent, Category = "Stockpile", DisplayName = "OnReachedMaxAmount")
	void K2_OnReachedMaxAmount();

public:

	// Begin event methods called by UFallingSlimeStockpileComponent

	void StockpiledIn(UFallingSlimeStockpileComponent* InStockpileComponent);

	void Unstockpiled();

	// End event methods called by UFallingSlimeStockpileComponent

	UFUNCTION(BlueprintCallable)
	UFallingSlimeStockpileComponent* GetStockpileComponent() const { return StockpileComponent; }

protected:

	virtual void OnStockpiledIn(UFallingSlimeStockpileComponent* InStockpileComponent) {}

	virtual void OnUnstockpiled() {}

	UFUNCTION(BlueprintImplementableEvent, Category = "Stockpile", DisplayName = "OnStockpiledIn")
	void K2_OnStockpiledIn(UFallingSlimeStockpileComponent* InStockpileComponent);

	UFUNCTION(BlueprintImplementableEvent, Category = "Stockpile", DisplayName = "OnUnstockpiledFrom")
	void K2_OnUnstockpiled();

private:

	UPROPERTY()
	TObjectPtr<UFallingSlimeStockpileComponent> StockpileComponent;

	UPROPERTY()
	int Amount = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Stockpile", meta = (AllowPrivateAccess = true))
	int MaxAmount = 0;
};
