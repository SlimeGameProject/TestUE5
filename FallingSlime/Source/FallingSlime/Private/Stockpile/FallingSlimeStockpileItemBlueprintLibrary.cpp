// Fill out your copyright notice in the Description page of Project Settings.

#include "Stockpile/FallingSlimeStockpileItemBlueprintLibrary.h"

bool UFallingSlimeStockpileItemBlueprintLibrary::TryStockpileItem(UFallingSlimeStockpileComponent* StockpileComponent, UFallingSlimeStockpileItemComponent* StockpileItemComponent)
{
	if (!StockpileComponent || !StockpileItemComponent)
	{
		return false;
	}

	AController* Instigator = nullptr;

	if (APawn* PawnOwner = Cast<APawn>(StockpileComponent->GetOwner()))
	{
		Instigator = PawnOwner->GetController();
	}

	if (!StockpileItemComponent->CanBeStockpiledBy(Instigator))
	{
		return false;
	}

	TSubclassOf<UFallingSlimeStockpileItem> StockpileItemClass = StockpileItemComponent->GetStockpileItemClass();
	int StockpileAmount = StockpileItemComponent->GetStockpileAmount();

	StockpileComponent->StockpileItem(StockpileItemClass, StockpileAmount);

	StockpileItemComponent->OnStockpiledBy(Instigator);

	return true;
}
