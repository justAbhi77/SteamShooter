/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "Achievements/EOSAchievementsAsyncActions.h"
#include "Achievements/EOSAchievements.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLibrary.h"
#include "Core/EOSCoreLogging.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreAchievementsQueryDefinitions
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreAchievementsQueryDefinitions* UEOSCoreAchievementsQueryDefinitions::EOSAchievementsQueryDefinitionsAsync(UObject* WorldContextObject, FEOSAchievementsQueryDefinitionsOptions Options)
{
	if (UEOSCoreSubsystem::GetAchievementsHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreAchievementsQueryDefinitions>();
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

void UEOSCoreAchievementsQueryDefinitions::Activate()
{
	Super::Activate();

	UCoreAchievements::GetAchievements(m_WorldContextObject)->EOSAchievementsQueryDefinitions(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreAchievementsQueryPlayerAchievements
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreAchievementsQueryPlayerAchievements* UEOSCoreAchievementsQueryPlayerAchievements::EOSAchievementsQueryPlayerAchievementsAsync(UObject* WorldContextObject, FEOSAchievementsQueryPlayerAchievementsOptions Options)
{
	if (UEOSCoreSubsystem::GetAchievementsHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreAchievementsQueryPlayerAchievements>();

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

void UEOSCoreAchievementsQueryPlayerAchievements::Activate()
{
	Super::Activate();

	UCoreAchievements::GetAchievements(m_WorldContextObject)->EOSAchievementsQueryPlayerAchievements(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreAchievementsUnlockAchievements
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreAchievementsUnlockAchievements* UEOSCoreAchievementsUnlockAchievements::EOSAchievementsUnlockAchievements(UObject* WorldContextObject, FEOSAchievementsUnlockAchievementsOptions Options)
{
	if (UEOSCoreSubsystem::GetAchievementsHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreAchievementsUnlockAchievements>();
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

void UEOSCoreAchievementsUnlockAchievements::Activate()
{
	Super::Activate();

	UCoreAchievements::GetAchievements(m_WorldContextObject)->EOSAchievementsUnlockAchievements(m_WorldContextObject, m_Options, m_Callback);
}
