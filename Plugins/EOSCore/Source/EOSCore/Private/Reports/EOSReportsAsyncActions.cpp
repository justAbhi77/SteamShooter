/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "Reports/EOSReportsAsyncActions.h"
#include "Reports/EOSReports.h"
#include "Core/EOSCorePluginPrivatePCH.h" 
#include "Core/EOSCoreLibrary.h"
#include "Core/EOSCoreLogging.h"

UEOSReportsSendPlayerBehaviorReport* UEOSReportsSendPlayerBehaviorReport::EOSReportsSendPlayerBehaviorReportAsync(
	UObject* WorldContextObject, FEOSReportsSendPlayerBehaviorReportOptions options)
{
	if (UEOSCoreSubsystem::GetLobbyHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSReportsSendPlayerBehaviorReport>();
		AsyncObject->RegisterWithGameInstance(GameInstance);
		AsyncObject->Options = options;
		AsyncObject->m_WorldContextObject = WorldContextObject;

		return AsyncObject;
	}
	else
	{
		LogError("Plugin not initialized!");
	}

	return nullptr;
}

void UEOSReportsSendPlayerBehaviorReport::Activate()
{
	Super::Activate();
	UCoreReports::GetReports(m_WorldContextObject)->EOSReportsSendPlayerBehaviorReport(m_WorldContextObject, Options, Callback);
}
