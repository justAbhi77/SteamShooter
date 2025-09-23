/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include <Misc/EngineVersionComparison.h>
#if UE_VERSION_NEWER_THAN(4,27,2)
#include <Online/CoreOnline.h>
#else
#include <UObject/CoreOnline.h>
#endif
#include "Interfaces/OnlineAchievementsInterface.h"
#include "OnlineSubsystemPackageEOSCore.h"
#include "OnlineSubsystemTypesEOSCore.h"
#include "eos_achievements_types.h"

class FOnlineSubsystemEOSCore;

class FOnlineAchievementsEOSCore :
	public IOnlineAchievements
{
public:
	FOnlineAchievementsEOSCore() = delete;
	virtual ~FOnlineAchievementsEOSCore() = default;

	// IOnlineAchievements Interface
	virtual void WriteAchievements(const FUniqueNetId& PlayerId, FOnlineAchievementsWriteRef& WriteObject, const FOnAchievementsWrittenDelegate& Delegate = FOnAchievementsWrittenDelegate()) override;
	virtual void QueryAchievements(const FUniqueNetId& PlayerId, const FOnQueryAchievementsCompleteDelegate& Delegate = FOnQueryAchievementsCompleteDelegate()) override;
	virtual void QueryAchievementDescriptions(const FUniqueNetId& PlayerId, const FOnQueryAchievementsCompleteDelegate& Delegate = FOnQueryAchievementsCompleteDelegate()) override;
	virtual EOnlineCachedResult::Type GetCachedAchievement(const FUniqueNetId& PlayerId, const FString& AchievementId, FOnlineAchievement& OutAchievement) override;
	virtual EOnlineCachedResult::Type GetCachedAchievements(const FUniqueNetId& PlayerId, TArray<FOnlineAchievement>& OutAchievements) override;
	virtual EOnlineCachedResult::Type GetCachedAchievementDescription(const FString& AchievementId, FOnlineAchievementDesc& OutAchievementDesc) override;
#if !UE_BUILD_SHIPPING
	virtual bool ResetAchievements(const FUniqueNetId& PlayerId) override;
#endif
	// ~IOnlineAchievements Interface

PACKAGE_SCOPE:
	FOnlineAchievementsEOSCore(FOnlineSubsystemEOSCore* InSubsystem)
		: EOSSubsystem(InSubsystem)
	{
	}

private:
	FOnlineSubsystemEOSCore* EOSSubsystem;
	TMap<FString, TSharedPtr<TArray<FOnlineAchievement>>> CachedAchievementsMap;
	TArray<FOnlineAchievementDesc> CachedAchievementDefinitions;
	TMap<FString, FOnlineAchievementDesc*> CachedAchievementDefinitionsMap;
};

typedef TSharedPtr<FOnlineAchievementsEOSCore, ESPMode::ThreadSafe> FOnlineAchievementsEOSCorePtr;
