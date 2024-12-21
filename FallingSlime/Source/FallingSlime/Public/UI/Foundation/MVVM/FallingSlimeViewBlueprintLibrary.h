// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Include headers in UMG module.
#include "Blueprint/UserWidget.h"

// Include headers in FallingSlime module.
#include "FallingSlimeViewExtension.h"
#include "FallingSlimeViewModel.h"
#include "UI/FallingSlimeUserWidget.h"

#include "FallingSlimeViewBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGSLIME_API UFallingSlimeViewBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "MVVM")
	static void RegisterViewModel(UFallingSlimeUserWidget* Widget, UFallingSlimeViewModel* ViewModel);

	UFUNCTION(BlueprintCallable, Category = "MVVM")
	static void UnregiserViewModel(UFallingSlimeUserWidget* Widget, UFallingSlimeViewModel* ViewModel);

	UFUNCTION(BlueprintCallable, Category = "MVVM")
	static UFallingSlimeViewModel* FindViewModelByClass(UFallingSlimeUserWidget* Widget, TSubclassOf<UFallingSlimeViewModel> ViewModelClass);
};
