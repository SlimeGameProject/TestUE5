// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Foundation/MVVM/FallingSlimeViewBlueprintLibrary.h"

void UFallingSlimeViewBlueprintLibrary::RegisterViewModel(UFallingSlimeUserWidget* Widget, UFallingSlimeViewModel* ViewModel)
{ 
	if (!Widget || !ViewModel)
	{
		return;
	}

	UFallingSlimeViewExtension* ViewExtension = Widget->GetExtension<UFallingSlimeViewExtension>();

	if (!ViewExtension)
	{
		ViewExtension = Widget->AddExtension<UFallingSlimeViewExtension>();
	}

	ViewExtension->RegisterViewModel(ViewModel);
}

void UFallingSlimeViewBlueprintLibrary::UnregiserViewModel(UFallingSlimeUserWidget* Widget, UFallingSlimeViewModel* ViewModel)
{
	if (!Widget || !ViewModel)
	{
		return;
	}

	UFallingSlimeViewExtension* ViewExtension = Widget->GetExtension<UFallingSlimeViewExtension>();

	if (ViewExtension)
	{
		ViewExtension->UnregisterViewModel(ViewModel);
	}
}

UFallingSlimeViewModel* UFallingSlimeViewBlueprintLibrary::FindViewModelByClass(UFallingSlimeUserWidget* Widget, TSubclassOf<UFallingSlimeViewModel> ViewModelClass)
{
	if (!Widget)
	{
		return nullptr;
	}

	UFallingSlimeViewExtension* ViewExtension = Widget->GetExtension<UFallingSlimeViewExtension>();

	if (!ViewExtension)
	{
		return nullptr;
	}

	return ViewExtension->FindViewModelByClass(ViewModelClass);
}
