// Fill out your copyright notice in the Description page of Project Settings.

#include "Stockpile/Items/FallingSlimeStockpileItem_MetamorphoseSlimeCharacter.h"

// Include headers in FallingSlime module.
#include "Character/Slime/FallingSlimeSlimeCharacter.h"
#include "Character/Slime/FallingSlimeSlimeCharacterBlueprintLibrary.h"
#include "Stockpile/FallingSlimeStockpileComponent.h"

void UFallingSlimeStockpileItem_MetamorphoseSlimeCharacter::OnReachedMaxAmount()
{
	UFallingSlimeStockpileComponent* StockpileComp = GetStockpileComponent();

	if (!StockpileComp)
	{
		return;
	}

	if (AFallingSlimeSlimeCharacter* SlimeCharacter = Cast<AFallingSlimeSlimeCharacter>(StockpileComp->GetOwner()))
	{
		FFallingSlimeSlimeCharacterMetamorphosisResult Result = UFallingSlimeSlimeCharacterBlueprintLibrary::TryMetamorphose(SlimeCharacter, MetamorphosisData);

		if (Result.bHasMetamorphosed)
		{
			// これ以降 GetStockpileComponent() による戻り値は null になっていることに注意する。
			StockpileComp->UnstockpileItem(GetClass(), GetMaxAmount());
		}
	}
}
