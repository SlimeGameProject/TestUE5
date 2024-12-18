// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "UObject/Interface.h"

// Include headers in FallingSlime module.
#include "FallingSlimeStockpileItemDefinition.h" 
#include "FallingSlimeStockpileItemInterface.h"

#include "FallingSlimeStockpileItemComponent.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FALLINGSLIME_API UFallingSlimeStockpileItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// Sets default values for this component's properties
	UFallingSlimeStockpileItemComponent();

public:

	UFUNCTION(BlueprintCallable, Category = "Stockpile")
	TSubclassOf<UFallingSlimeStockpileItem> GetStockpileItemClass() const
	{
		return StockpileItemDefinition ? StockpileItemDefinition->GetStockpileItemClass() : nullptr;
	}

	UFUNCTION(BlueprintCallable, Category = "Stockpile")
	int GetStockpileAmount() const { return StockpileAmount; }

public:

	void OnStockpiledBy(AController* Instigator) const;

	bool CanBeStockpiledBy(AController* Instigator) const;

private:

	UPROPERTY(Category = "Stockpile", EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<UFallingSlimeStockpileItemDefinition> StockpileItemDefinition;

	UPROPERTY(Category = "Stockpile", EditAnywhere, meta = (AllowPrivateAccess = true))
	int StockpileAmount{ 0 };
};
