// Fill out your copyright notice in the Description page of Project Settings.

#include "FallingSlimeGameInstance.h"

#include "UI/FallingSlimeNavigationConfig.h"

void UFallingSlimeGameInstance::Init()
{
	Super::Init();

	FSlateApplication::Get().SetNavigationConfig(MakeShared<FFallingSlimeNavigationConfig>());
}
