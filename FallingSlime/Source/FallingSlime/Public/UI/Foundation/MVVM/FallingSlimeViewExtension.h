// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Extensions/UserWidgetExtension.h"

#include "FallingSlimeViewModel.h"

#include "FallingSlimeViewExtension.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGSLIME_API UFallingSlimeViewExtension : public UUserWidgetExtension
{
	GENERATED_BODY()

public:

	void RegisterViewModel(UFallingSlimeViewModel* ViewModel);

	bool UnregisterViewModel(UFallingSlimeViewModel* ViewModel);

	void GetAllViewModels(TArray<UFallingSlimeViewModel*>& ViewModels) const;

	UFallingSlimeViewModel* FindViewModelByClass(TSubclassOf<UFallingSlimeViewModel> ViewModelClass) const;

private:

	UPROPERTY()
	TArray<TObjectPtr<UFallingSlimeViewModel>> ViewModels;
};
