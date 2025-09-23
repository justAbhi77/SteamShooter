/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

using System;
using System.IO;
using UnrealBuildTool;

public class EOSLibrary : ModuleRules
{
	public string SDKBaseDir
	{
		get { return ModuleDirectory; }
	}

	public virtual string SDKBinariesDir
	{
		get
		{
			if (Target.Platform == UnrealTargetPlatform.Android)
			{
				if (Target.Version.MajorVersion == 5 && Target.Version.MinorVersion >= 1)
				{
					return Path.Combine(SDKBaseDir, "Bin", "Android");
				}
				else
				{
					return Path.Combine(SDKBaseDir, "Bin", "Android");
				}
			}
			else if (Target.Platform == UnrealTargetPlatform.IOS)
			{
				return Path.Combine(SDKBaseDir, "Bin", "IOS");
			}

			return Path.Combine(SDKBaseDir, "Bin");
		}
	}

	public virtual string SDKLibsDir
	{
		get { return Path.Combine(SDKBaseDir, "Lib"); }
	}

	public virtual string SDKIncludesDir
	{
		get
		{
			if (Target.Platform == UnrealTargetPlatform.Android)
			{
				return Path.Combine(SDKBinariesDir, "include");
			}
			else if (Target.Platform == UnrealTargetPlatform.IOS)
			{
				return Path.Combine(SDKBinariesDir, "EOSSDK.framework", "Headers");
			}

			return Path.Combine(SDKBaseDir, "Include");
		}
	}

	public EOSLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		PublicIncludePaths.Add(SDKIncludesDir);
		PublicSystemIncludePaths.Add(SDKIncludesDir);
		PrivateIncludePaths.Add(SDKIncludesDir);
		PublicDefinitions.Add("EOSSDK_USE_PROJECT_BINARY=1");

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, "EOSSDK-Win64-Shipping.lib"));
			PublicDelayLoadDLLs.Add("EOSSDK-Win64-Shipping.dll");
			RuntimeDependencies.Add(Path.Combine(SDKBinariesDir, "EOSSDK-Win64-Shipping.dll"));
		}
		else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			PrivateRuntimeLibraryPaths.Add(SDKBinariesDir);
			PublicAdditionalLibraries.Add(Path.Combine(SDKBinariesDir, "libEOSSDK-Linux-Shipping.so"));
			RuntimeDependencies.Add(Path.Combine("$(TargetOutputDir)", "libEOSSDK-Linux-Shipping.so"), Path.Combine(SDKBinariesDir, "libEOSSDK-Linux-Shipping.so"));
			RuntimeDependencies.Add(Path.Combine(SDKBinariesDir, "libEOSSDK-Linux-Shipping.so"));
		}
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			PublicDelayLoadDLLs.Add(Path.Combine(SDKBinariesDir, "libEOSSDK-Mac-Shipping.dylib"));
			RuntimeDependencies.Add(Path.Combine(SDKBinariesDir, "libEOSSDK-Mac-Shipping.dylib"));
		}
		else if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			PublicAdditionalFrameworks.Add(new Framework("EOSSDK", SDKBinariesDir, "", true));
			AdditionalPropertiesForReceipt.Add("IOSPlugin", Path.Combine(SDKBaseDir, "LibEOSCore_UPL.xml"));
		} 
		else if (Target.Platform == UnrealTargetPlatform.Android)
		{
			PublicAdditionalLibraries.Add(Path.Combine(SDKBinariesDir, "static-stdc++", "libs", "arm64-v8a", "libEOSSDK.so"));
			PublicAdditionalLibraries.Add(Path.Combine(SDKBinariesDir, "static-stdc++", "libs", "armeabi-v7a", "libEOSSDK.so"));

			AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(SDKBaseDir, "LibEOSCore_APL.xml"));
		}
	}
}
