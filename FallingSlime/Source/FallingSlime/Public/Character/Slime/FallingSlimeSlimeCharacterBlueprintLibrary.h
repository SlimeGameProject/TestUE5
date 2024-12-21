// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

// Include headers in FallingSlime module.
#include "FallingSlimeSlimeCharacter.h"
#include "FallingSlimeSlimeCharacterMetamorphosisTypes.h"

#include "FallingSlimeSlimeCharacterBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGSLIME_API UFallingSlimeSlimeCharacterBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/*
	* スライムを変身させる。
	* この関数ではアクタの置き換えやエフェクトの再生など外的な処理を担っている。
	*/
	UFUNCTION(BlueprintCallable, Category = "SlimeCharacter|Metamorphosis")
	static FFallingSlimeSlimeCharacterMetamorphosisResult TryMetamorphose(AFallingSlimeSlimeCharacter* OriginalSlimeCharacter, const UFallingSlimeSlimeCharacterMetamorphosisData* MetamorphosisData);
};
