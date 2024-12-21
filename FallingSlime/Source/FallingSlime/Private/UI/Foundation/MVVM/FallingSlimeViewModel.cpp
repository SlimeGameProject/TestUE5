// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Foundation/MVVM/FallingSlimeViewModel.h"

void UFallingSlimeViewModel::Registered()
{
	if (bHasRegistered)
	{
		return;
	}

	bHasRegistered = true;

	OnRegistered();
}

void UFallingSlimeViewModel::Unregistered()
{
	if (!bHasRegistered)
	{
		return;
	}

	OnUnregistered();

	bHasRegistered = false;
}

void UFallingSlimeViewModel::OnRegistered()
{
	K2_OnRegistered();
}

void UFallingSlimeViewModel::OnUnregistered()
{
	K2_OnUnregistered();
}
