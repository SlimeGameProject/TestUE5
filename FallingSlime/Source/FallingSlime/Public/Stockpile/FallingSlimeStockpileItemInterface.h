// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Controller.h"
#include "UObject/Interface.h"

#include "FallingSlimeStockpileItemInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UFallingSlimeStockpileItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class FALLINGSLIME_API IFallingSlimeStockpileItemInterface
{
	GENERATED_BODY()

public:

	virtual void OnStockpiledBy(AController* Instigator) {};
	
	virtual bool CanBeStockpiledBy(AController* Instigator) { return true; };

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Stockpile", meta = (DisplayName = "OnStockpiledBy"))
	void K2_OnStockpiledBy(AController* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Stockpile", meta = (DisplayName = "CanBeStockpiledBy"))
	bool K2_CanBeStockpiledBy(AController* Instigator);
};
