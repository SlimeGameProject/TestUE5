// Fill out your copyright notice in the Description page of Project Settings.

#include "Modules/ModuleManager.h"

class FFallingSlimeModule : public FDefaultGameModuleImpl
{
public:

	virtual void StartupModule() override
	{

	}
};

IMPLEMENT_PRIMARY_GAME_MODULE(FFallingSlimeModule, FallingSlime, "FallingSlime" );
