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
#include "Interfaces/OnlineStatsInterface.h"
#include "OnlineSubsystemPackageEOSCore.h"
#include "OnlineSubsystemTypesEOSCore.h"
#include "eos_stats_types.h"

class FOnlineSubsystemEOSCore;

class FOnlineStatsEOSCore : public IOnlineStats
{
public:
	FOnlineStatsEOSCore() = delete;
	virtual ~FOnlineStatsEOSCore() = default;

public:
	// IOnlineStats Interface
	virtual void QueryStats(const TSharedRef<const FUniqueNetId> LocalUserId, const TSharedRef<const FUniqueNetId> StatsUser, const FOnlineStatsQueryUserStatsComplete& Delegate) override;
	virtual void QueryStats(const TSharedRef<const FUniqueNetId> LocalUserId, const TArray<TSharedRef<const FUniqueNetId>>& StatUsers, const TArray<FString>& StatNames,
	                        const FOnlineStatsQueryUsersStatsComplete& Delegate) override;
	virtual TSharedPtr<const FOnlineStatsUserStats> GetStats(const TSharedRef<const FUniqueNetId> StatsUserId) const override;
	virtual void UpdateStats(const TSharedRef<const FUniqueNetId> LocalUserId, const TArray<FOnlineStatsUserUpdatedStats>& UpdatedUserStats, const FOnlineStatsUpdateStatsComplete& Delegate) override;
#if !UE_BUILD_SHIPPING
	virtual void ResetStats(const TSharedRef<const FUniqueNetId> StatsUserId) override;
#endif
	// ~IOnlineStats Interface

PACKAGE_SCOPE:
	FOnlineStatsEOSCore(FOnlineSubsystemEOSCore* InSubsystem)
		: m_Subsystem(InSubsystem)
	{
	}

private:
	void WriteStats(EOS_ProductUserId LocalUserId, EOS_ProductUserId UserId, const FOnlineStatsUserUpdatedStats& PlayerStats);

private:
	FOnlineSubsystemEOSCore* m_Subsystem;
	TUniqueNetIdMap<TSharedRef<FOnlineStatsUserStats>> m_StatsCache;
};

typedef TSharedPtr<FOnlineStatsEOSCore, ESPMode::ThreadSafe> FOnlineStatsEOSCorePtr;
