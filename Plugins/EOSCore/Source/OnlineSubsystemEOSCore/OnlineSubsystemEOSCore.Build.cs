// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class OnlineSubsystemEOSCore : ModuleRules
{
	public OnlineSubsystemEOSCore(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivatePCHHeaderFile = "Private/OnlineSubsystemEOSCorePrivatePCH.h";

        PublicDefinitions.Add("ONLINESUBSYSTEMEOSCORE_PACKAGE=1");
        PrivateDefinitions.Add("ONLINESUBSYSTEMEOSCORE_PACKAGE=1");

        PublicDependencyModuleNames.AddRange(
			new string[] {
				"OnlineSubsystemUtils",
				"NetCore"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"Sockets",
				"OnlineSubsystem",
				"Json",
                "EOSLibrary",
                "Projects",
                "PacketHandler",
                "EOSCoreShared",
                "NetCore"
			}
		);

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });
        }

        if (Target.bBuildEditor == true)
        {
            PrivateDependencyModuleNames.Add("UnrealEd");
        }

        if (Target.Version.MinorVersion >= 26 || Target.Version.MajorVersion == 5)
        {
            PrivateDependencyModuleNames.Add("DeveloperSettings");
        }
        
        if (Target.Version.MajorVersion == 5 && Target.Version.MinorVersion >= 1)
        {
	        PrivateDependencyModuleNames.Add("OnlineBase");
        }
    }
}
