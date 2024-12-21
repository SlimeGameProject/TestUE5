// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Foundation/MVVM/FallingSlimeViewExtension.h"

void UFallingSlimeViewExtension::RegisterViewModel(UFallingSlimeViewModel* ViewModel)
{
	if (!ViewModel)
	{
		return;
	}

	if (ViewModels.Contains(ViewModel))
	{
		return;
	}

	ViewModels.Emplace(ViewModel);

	ViewModel->Registered();
}

bool UFallingSlimeViewExtension::UnregisterViewModel(UFallingSlimeViewModel* ViewModel)
{
	if (!ViewModel)
	{
		return false;
	}

	if (!ViewModels.Contains(ViewModel))
	{
		return false;
	}

	bool bUnregistered = ViewModels.RemoveSingleSwap(ViewModel) == 1;

	if (bUnregistered)
	{
		ViewModel->Unregistered();
	}

	return bUnregistered;
}

void UFallingSlimeViewExtension::GetAllViewModels(TArray<UFallingSlimeViewModel*>& Results) const
{
	Results.Empty();

	for (UFallingSlimeViewModel* ViewModel : ViewModels)
	{
		Results.Emplace(ViewModel);
	}
}

UFallingSlimeViewModel* UFallingSlimeViewExtension::FindViewModelByClass(TSubclassOf<UFallingSlimeViewModel> ViewModelClass) const
{
	for (UFallingSlimeViewModel* ViewModel : ViewModels)
	{
		if (ViewModel->GetClass() == ViewModelClass)
		{
			return ViewModel;
		}
	}

	return nullptr;
}
