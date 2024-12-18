// Fill out your copyright notice in the Description page of Project Settings.

#include "Stockpile/FallingSlimeStockpileComponent.h"

void UFallingSlimeStockpileComponent::StockpileItem(TSubclassOf<UFallingSlimeStockpileItem> StockpileItemClass, int StockpileItemAmount)
{
	if (!StockpileItemClass || StockpileItemAmount <= 0)
	{
		return;
	}

	UFallingSlimeStockpileItem* StockpileItem = nullptr;

	for (FFallingSlimeStockpileItemEntry& Entry : StockpileItems)
	{
		if (Entry.StockpileItem->GetClass() != StockpileItemClass)
		{
			continue;
		}

		StockpileItem = Entry.StockpileItem;

		break;
	}

	if (!StockpileItem)
	{
		StockpileItem = NewObject<UFallingSlimeStockpileItem>(this, StockpileItemClass);

		StockpileItem->StockpiledIn(this);

		FFallingSlimeStockpileItemEntry NewEntry{ StockpileItem };
		StockpileItems.Emplace(NewEntry);

		StockpileItemAdded(StockpileItem);
	}

	int IncreasedAmount = StockpileItem->IncreaseAmount(StockpileItemAmount);

	if (IncreasedAmount > 0)
	{
		StockpileItemAmountChanged(StockpileItem, IncreasedAmount);

		if (StockpileItem->GetAmount() == StockpileItem->GetMaxAmount())
		{
			StockpileItemReachedMaxAmount(StockpileItem);
		}
	}
}

void UFallingSlimeStockpileComponent::UnstockpileItem(TSubclassOf<UFallingSlimeStockpileItem> UnstockpileItemClass, int UnstockpileItemAmount)
{
	FFallingSlimeStockpileItemEntry* EntryToUnstockpile = nullptr;
	int EntryIndexToUnstockpile = -1;

	for (int Index = 0; Index < StockpileItems.Num(); Index += 1)
	{
		UClass* StockpileItemClass = StockpileItems[Index].StockpileItem->GetClass();

		if (StockpileItemClass != UnstockpileItemClass)
		{
			continue;
		}

		EntryToUnstockpile = &StockpileItems[Index];
		EntryIndexToUnstockpile = Index;

		break;
	}

	if (!EntryToUnstockpile)
	{
		return;
	}

	UFallingSlimeStockpileItem* UnstockpileItem = EntryToUnstockpile->StockpileItem;
	int DecreasedAmount = UnstockpileItem->DecreaseAmount(UnstockpileItemAmount);

	if (DecreasedAmount > 0)
	{
		StockpileItemAmountChanged(UnstockpileItem, -DecreasedAmount);

		// 集めたアイテムが 0 になれば、コンテナから削除する
		if (UnstockpileItem->GetAmount() == 0)
		{
			UnstockpileItem->Unstockpiled();

			StockpileItems.RemoveAtSwap(EntryIndexToUnstockpile);
			StockpileItemRemoved(UnstockpileItem);
		}
	}
}

void UFallingSlimeStockpileComponent::StockpileItemAdded(UFallingSlimeStockpileItem* AddedStockpileItem)
{
	OnStockpileItemAdded.Broadcast(AddedStockpileItem);
}

void UFallingSlimeStockpileComponent::StockpileItemRemoved(UFallingSlimeStockpileItem* RemovedStockpileItem)
{
	OnStockpileItemRemoved.Broadcast(RemovedStockpileItem);
}

void UFallingSlimeStockpileComponent::StockpileItemAmountChanged(UFallingSlimeStockpileItem* StockpileItem, int Amount)
{
	OnStockpileItemAmountChanged.Broadcast(StockpileItem, Amount);
}

void UFallingSlimeStockpileComponent::StockpileItemReachedMaxAmount(UFallingSlimeStockpileItem* StockpileItem)
{
	OnStockpileItemReachedMaxAmount.Broadcast(StockpileItem);
}
