// Fill out your copyright notice in the Description page of Project Settings.

#include "Stockpile/FallingSlimeStockpileItem.h"

#include "Stockpile/FallingSlimeStockpileComponent.h"

#include "Kismet/KismetMathLibrary.h"

int UFallingSlimeStockpileItem::IncreaseAmount(int AmountToIncrease)
{
	if (AmountToIncrease <= 0)
	{
		return 0;
	}

	int OldAmount{ Amount };

	SetAmount(Amount + AmountToIncrease);

	return Amount - OldAmount;
}

int UFallingSlimeStockpileItem::DecreaseAmount(int AmountToDecrease)
{
	if (AmountToDecrease <= 0)
	{
		return 0;
	}

	int OldAmount{ Amount };

	SetAmount(Amount - AmountToDecrease);

	return OldAmount - Amount;
}

void UFallingSlimeStockpileItem::SetAmount(int NewAmount)
{
	int OldAmount{ Amount };

	Amount = UKismetMathLibrary::Clamp(NewAmount, 0, MaxAmount);

	if (Amount != OldAmount)
	{
		int DeltaAmount = Amount - OldAmount;
		OnAmountChanged(DeltaAmount);
		K2_OnAmountChanged(DeltaAmount);

		if (Amount == MaxAmount)
		{
			OnReachedMaxAmount();
			K2_OnReachedMaxAmount();
		}
	}
}

void UFallingSlimeStockpileItem::StockpiledIn(UFallingSlimeStockpileComponent* InStockpileComponent)
{
	if (!InStockpileComponent)
	{
		return;
	}

	StockpileComponent = InStockpileComponent;

	OnStockpiledIn(StockpileComponent);
	K2_OnStockpiledIn(StockpileComponent);
}

void UFallingSlimeStockpileItem::Unstockpiled()
{
	if (!StockpileComponent)
	{
		return;
	}

	K2_OnStockpiledIn(StockpileComponent);
	OnUnstockpiled();

	StockpileComponent = nullptr;
}
