// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/FallingSlimePlayerCameraManager.h"

AFallingSlimePlayerCameraManager::AFallingSlimePlayerCameraManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ViewPitchMin = -10.f;
	ViewPitchMax = 25.f;
}
