/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "Leaderboards/EOSLeaderboardsAsyncActions.h"
#include "Leaderboards/EOSLeaderboards.h"
#include "Core/EOSCorePluginPrivatePCH.h" 
#include "Core/EOSCoreLibrary.h"
#include "Core/EOSCoreLogging.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreLeaderboardsQueryLeaderboardDefinitions
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreLeaderboardsQueryLeaderboardDefinitions* UEOSCoreLeaderboardsQueryLeaderboardDefinitions::EOSLeaderboardsQueryLeaderboardDefinitionsAsync(UObject* WorldContextObject, FEOSLeaderboardsQueryLeaderboardDefinitionsOptions Options)
{
	if (UEOSCoreSubsystem::GetLeaderboardsHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreLeaderboardsQueryLeaderboardDefinitions>();
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

void UEOSCoreLeaderboardsQueryLeaderboardDefinitions::Activate()
{
	Super::Activate();
	UCoreLeaderboards::GetLeaderboards(m_WorldContextObject)->EOSLeaderboardsQueryLeaderboardDefinitions(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreLeaderboardsQueryLeaderboardRanks
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreLeaderboardsQueryLeaderboardRanks* UEOSCoreLeaderboardsQueryLeaderboardRanks::EOSLeaderboardsQueryLeaderboardRanksAsync(UObject* WorldContextObject, FEOSLeaderboardsQueryLeaderboardRanksOptions Options)
{
	if (UEOSCoreSubsystem::GetLeaderboardsHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreLeaderboardsQueryLeaderboardRanks>();
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

void UEOSCoreLeaderboardsQueryLeaderboardRanks::Activate()
{
	Super::Activate();
	UCoreLeaderboards::GetLeaderboards(m_WorldContextObject)->EOSLeaderboardsQueryLeaderboardRanks(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreLeaderboardsQueryLeaderboardUserScores
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreLeaderboardsQueryLeaderboardUserScores* UEOSCoreLeaderboardsQueryLeaderboardUserScores::EOSLeaderboardsQueryLeaderboardUserScoresAsync(UObject* WorldContextObject, FEOSLeaderboardsQueryLeaderboardUserScoresOptions Options)
{
	if (UEOSCoreSubsystem::GetLeaderboardsHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreLeaderboardsQueryLeaderboardUserScores>();
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

void UEOSCoreLeaderboardsQueryLeaderboardUserScores::Activate()
{
	Super::Activate();
	UCoreLeaderboards::GetLeaderboards(m_WorldContextObject)->EOSLeaderboardsQueryLeaderboardUserScores(m_WorldContextObject, m_Options, m_Callback);
}
