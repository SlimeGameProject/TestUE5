// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "FallingSlimeViewModel.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class FALLINGSLIME_API UFallingSlimeViewModel : public UObject
{
	GENERATED_BODY()

public:

	void Registered();

	void Unregistered();

protected:

	virtual void OnRegistered();

	virtual void OnUnregistered();

	UFUNCTION(BlueprintImplementableEvent, Category = "MVVM", meta = (DisplayName = "OnRegistered"))
	void K2_OnRegistered();

	UFUNCTION(BlueprintImplementableEvent, Category = "MVVM", meta = (DisplayName = "OnUnregistered"))
	void K2_OnUnregistered();

private:

	bool bHasRegistered = false;
};
