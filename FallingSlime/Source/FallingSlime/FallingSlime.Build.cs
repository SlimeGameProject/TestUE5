// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class FallingSlime : ModuleRules
{
	public FallingSlime(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"EnhancedInput",
			"InputCore",
			"Niagara",
			"Slate",
			"UMG",
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
