// Fill out your copyright notice in the Description page of Project Settings.

#include "Stockpile/FallingSlimeStockpileItemComponent.h"

UFallingSlimeStockpileItemComponent::UFallingSlimeStockpileItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UFallingSlimeStockpileItemComponent::OnStockpiledBy(AController* Instigator) const
{
	if (AActor* Owner = GetOwner())
	{
		if (IFallingSlimeStockpileItemInterface* StockpileItemInterface = Cast<IFallingSlimeStockpileItemInterface>(Owner))
		{
			StockpileItemInterface->OnStockpiledBy(Instigator);
		}

		if (Owner->Implements<UFallingSlimeStockpileItemInterface>())
		{
			IFallingSlimeStockpileItemInterface::Execute_K2_OnStockpiledBy(Owner, Instigator);
		}
	}
}

bool UFallingSlimeStockpileItemComponent::CanBeStockpiledBy(AController* Instigator) const
{
	if (!StockpileItemDefinition)
	{
		return false;
	}

	bool bCanBeStockpiledBy = true;

	if (AActor* Owner = GetOwner())
	{
		if (IFallingSlimeStockpileItemInterface* StockpileItemInterface = Cast<IFallingSlimeStockpileItemInterface>(Owner))
		{
			bCanBeStockpiledBy &= StockpileItemInterface->CanBeStockpiledBy(Instigator);
		}

		// 
		if (Owner->Implements<UFallingSlimeStockpileItemInterface>())
		{
			bCanBeStockpiledBy &= IFallingSlimeStockpileItemInterface::Execute_K2_CanBeStockpiledBy(Owner, Instigator);
		}
	}

	return bCanBeStockpiledBy;
}
