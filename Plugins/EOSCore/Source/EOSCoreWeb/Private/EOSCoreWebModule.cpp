/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "EOSCoreWebModule.h"
#include "EOSCoreWebPrivatePCH.h"

IMPLEMENT_MODULE(FEOSCoreWebModule, EOSCoreWeb);

#define LOCTEXT_NAMESPACE "FEOSCoreWebModule"

DEFINE_LOG_CATEGORY(LogEOSCoreWeb);

void FEOSCoreWebModule::StartupModule()
{
	const TSharedPtr<IPlugin> PluginPtr = IPluginManager::Get().FindPlugin("EOSCoreWeb");

	if (PluginPtr)
	{
		const FString PluginVersion = PluginPtr->GetDescriptor().VersionName;

		UE_LOG(LogTemp, Log, TEXT("--------------------------------------------------------------------------------"));
		UE_LOG(LogTemp, Log, TEXT("Using EOSCoreWeb Version: %s"), *PluginVersion);
		UE_LOG(LogTemp, Log, TEXT("--------------------------------------------------------------------------------"));
	}
}

void FEOSCoreWebModule::ShutdownModule()
{
}



#undef LOCTEXT_NAMESPACE
