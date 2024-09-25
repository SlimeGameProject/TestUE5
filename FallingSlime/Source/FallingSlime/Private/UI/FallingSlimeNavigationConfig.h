// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Framework/Application/NavigationConfig.h"

/**
 * 
 */
class FFallingSlimeNavigationConfig : public FNavigationConfig
{
public:

	FFallingSlimeNavigationConfig();

public:

	virtual EUINavigationAction GetNavigationActionFromKey(const FKeyEvent& InKeyEvent) const;
};
