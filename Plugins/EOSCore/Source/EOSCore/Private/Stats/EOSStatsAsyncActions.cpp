/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "Stats/EOSStatsAsyncActions.h"
#include "Stats/EOSStats.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLibrary.h"
#include "Core/EOSCoreLogging.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreStatsIngestStat
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreStatsIngestStat* UEOSCoreStatsIngestStat::EOSStatsIngestStatAsync(UObject* WorldContextObject, FEOSStatsIngestStatOptions Options)
{
	if (UEOSCoreSubsystem::GetStatsHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreStatsIngestStat>();
		AsyncObject->RegisterWithGameInstance(GameInstance);
		AsyncObject->m_Options = Options;
		AsyncObject->m_WorldContextObject = WorldContextObject;

		return AsyncObject;
	}
	else
	{
		LogError("Plugin not initialized!");
	}

	return nullptr;
}

void UEOSCoreStatsIngestStat::Activate()
{
	Super::Activate();
	UCoreStats::GetStats(m_WorldContextObject)->EOSStatsIngestStat(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreStatsQueryStats
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreStatsQueryStats* UEOSCoreStatsQueryStats::EOSStatsQueryStatsAsync(UObject* WorldContextObject, FEOSStatsQueryStatsOptions Options)
{
	if (UEOSCoreSubsystem::GetStatsHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreStatsQueryStats>();
		AsyncObject->RegisterWithGameInstance(GameInstance);
		AsyncObject->m_Options = Options;
		AsyncObject->m_WorldContextObject = WorldContextObject;

		return AsyncObject;
	}
	else
	{
		LogError("Plugin not initialized!");
	}

	return nullptr;
}

void UEOSCoreStatsQueryStats::Activate()
{
	Super::Activate();
	UCoreStats::GetStats(m_WorldContextObject)->EOSStatsQueryStats(m_WorldContextObject, m_Options, m_Callback);
}
