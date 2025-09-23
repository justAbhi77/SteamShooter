/**
* Copyright (C) 2017-2022 | eelDev AB
*
* Official Steamworks Documentation: https://partner.steamgames.com/doc/api/
*/

using System;
using System.IO;
using UnrealBuildTool;

public class EOSCoreEditor : ModuleRules
{
    public EOSCoreEditor(ReadOnlyTargetRules rules) : base(rules)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
        PrivateDependencyModuleNames.AddRange(
            new string[]
                {
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "Slate",
                    "SlateCore",
                    "RenderCore",
                    "Projects",
                    "HTTP",
                    "EditorStyle",
                    "WebBrowser"
                }
            );
    }
}