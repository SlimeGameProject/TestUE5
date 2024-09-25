// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FallingSlimeGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGSLIME_API UFallingSlimeGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual void Init() override;
};
