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
#include "Interfaces/OnlineLeaderboardInterface.h"
#include "OnlineSubsystemPackageEOSCore.h"
#include "OnlineSubsystemTypesEOSCore.h"
#include "eos_leaderboards_types.h"

class FOnlineSubsystemEOSCore;

#define EOSCORE_MAX_NUM_RANKINGS 1000

class FOnlineLeaderboardsEOSCore : public IOnlineLeaderboards
{
public:
	FOnlineLeaderboardsEOSCore() = delete;
	virtual ~FOnlineLeaderboardsEOSCore() = default;
public:
// IOnlineLeaderboards Interface
	virtual bool ReadLeaderboards(const TArray< TSharedRef<const FUniqueNetId> >& Players, FOnlineLeaderboardReadRef& ReadObject) override;
	virtual bool ReadLeaderboardsForFriends(int32 LocalUserNum, FOnlineLeaderboardReadRef& ReadObject) override;
	virtual bool ReadLeaderboardsAroundRank(int32 Rank, uint32 Range, FOnlineLeaderboardReadRef& ReadObject) override;
	virtual bool ReadLeaderboardsAroundUser(TSharedRef<const FUniqueNetId> Player, uint32 Range, FOnlineLeaderboardReadRef& ReadObject) override;
	virtual void FreeStats(FOnlineLeaderboardRead& ReadObject) override;
	virtual bool WriteLeaderboards(const FName& SessionName, const FUniqueNetId& Player, FOnlineLeaderboardWrite& WriteObject) override;
	virtual bool FlushLeaderboards(const FName& SessionName) override;
	virtual bool WriteOnlinePlayerRatings(const FName& SessionName, int32 LeaderboardId, const TArray<FOnlinePlayerScore>& PlayerScores) override;
// ~IOnlineLeaderboards Interface

PACKAGE_SCOPE:
	FOnlineLeaderboardsEOSCore(FOnlineSubsystemEOSCore* InSubsystem)
		: m_Subsystem(InSubsystem)
	{}
private:
	FOnlineSubsystemEOSCore* m_Subsystem;
};

typedef TSharedPtr<FOnlineLeaderboardsEOSCore, ESPMode::ThreadSafe> FOnlineLeaderboardsEOSCorePtr;