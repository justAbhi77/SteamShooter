// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class EOSCoreWeb : ModuleRules
{
	public EOSCoreWeb(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		PrivatePCHHeaderFile = "Private/EOSCoreWebPrivatePCH.h";
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"HTTP",
				"Core",
				"Engine",
				"Slate",
				"SlateCore",
				"CoreUObject",
				"Json",
				"Serialization",
				"Projects",
				"ApplicationCore"
			}
			
		);
		
		PublicIncludePaths.Add(ModuleDirectory + "/Private");
	}
}