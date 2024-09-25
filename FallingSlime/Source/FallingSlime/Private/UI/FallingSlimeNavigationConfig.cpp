// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FallingSlimeNavigationConfig.h"

FFallingSlimeNavigationConfig::FFallingSlimeNavigationConfig()
	: FNavigationConfig()
{
}

EUINavigationAction FFallingSlimeNavigationConfig::GetNavigationActionFromKey(const FKeyEvent& InKeyEvent) const
{
	const FKey& InKey = InKeyEvent.GetKey();

	if (InKey == EKeys::Enter || InKey == EKeys::SpaceBar || InKey == EKeys::Gamepad_FaceButton_Right/* || InKey == EKeys::Virtual_Accept*/)
	{
		// By default, enter, space, and gamepad accept are all counted as accept
		return EUINavigationAction::Accept;
	}
	else if (InKey == EKeys::Escape || InKey == EKeys::Gamepad_FaceButton_Bottom/* || InKey == EKeys::Virtual_Back*/)
	{
		// By default, escape and gamepad back count as leaving current scope
		return EUINavigationAction::Back;
	}

	return EUINavigationAction::Invalid;
}
