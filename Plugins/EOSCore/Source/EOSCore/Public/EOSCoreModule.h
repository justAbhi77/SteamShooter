/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "Modules/ModuleInterface.h"
#include "eos_sdk.h"
#include "eos_auth_types.h"
#include "eos_friends_types.h"
#include "eos_userinfo_types.h"
#include "eos_presence_types.h"
#include "eos_connect_types.h"
#include "eos_sessions_types.h"
#include "eos_stats_types.h"
#include "eos_leaderboards_types.h"
#include "eos_metrics_types.h"
#include "eos_achievements_types.h"
#include "eos_p2p_types.h"
#include "eos_ecom_types.h"
#include "Modules/ModuleManager.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EOSCoreModule.generated.h"

class FOnlineSubsystemEOSCore;

class FEOSCoreModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool SupportsDynamicReloading() override { return true; }
public:
	static FEOSCoreModule* Get() { return FModuleManager::GetModulePtr<FEOSCoreModule>(FName("EOSCore")); }
};

UCLASS()
class EOSCORE_API UEOSCoreSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override {}
	virtual void Deinitialize() override {}
	virtual bool ShouldCreateSubsystem(UObject* Outer) const { return true; }
public:
	static FOnlineSubsystemEOSCore* GetSubsystem(UObject* WorldContextObject);
	static EOS_HPlatform GetPlatform(UObject* WorldContextObject);
	static EOS_HAuth GetAuthHandle(UObject* WorldContextObject);
	static EOS_HFriends GetFriendsHandle(UObject* WorldContextObject);
	static EOS_HUserInfo GetUserHandle(UObject* WorldContextObject);
	static EOS_HPresence GetPresenceHandle(UObject* WorldContextObject);
	static EOS_HConnect GetConnectHandle(UObject* WorldContextObject);
	static EOS_HSessions GetSessionsHandle(UObject* WorldContextObject);
	static EOS_HStats GetStatsHandle(UObject* WorldContextObject);
	static EOS_HLeaderboards GetLeaderboardsHandle(UObject* WorldContextObject);
	static EOS_HMetrics GetMetricsHandle(UObject* WorldContextObject);
	static EOS_HAchievements GetAchievementsHandle(UObject* WorldContextObject);
	static EOS_HP2P GetP2PHandle(UObject* WorldContextObject);
	static EOS_HEcom GetEcomHandle(UObject* WorldContextObject);
	static EOS_HLobby GetLobbyHandle(UObject* WorldContextObject);
	static EOS_HPlayerDataStorage GetPlayerDataStorageHandle(UObject* WorldContextObject);
	static EOS_HTitleStorage GetTitleStorageHandle(UObject* WorldContextObject);
	static EOS_HUI GetUiHandle(UObject* WorldContextObject);
	static EOS_HUserInfo GetUserInfoHandle(UObject* WorldContextObject);
	static EOS_HMods GetModsHandle(UObject* WorldContextObject);
	static EOS_HReports GetReportsHandle(UObject* WorldContextObject);
	static EOS_HSanctions GetSanctionssHandle(UObject* WorldContextObject);
	static EOS_HAntiCheatClient GetAntiCheatClientHandle(UObject* WorldContextObject);
	static EOS_HAntiCheatServer GetAntiCheatServerHandle(UObject* WorldContextObject);
	static EOS_HRTC GetRTCHandle(UObject* WorldContextObject);
	static EOS_HRTCAdmin GetRTCAdminHandle(UObject* WorldContextObject);
	static EOS_HProgressionSnapshot GetProgressionSnapshotHandle(UObject* WorldContextObject);
	static EOS_HCustomInvites GetCustomInvitesHandle(UObject* WorldContextObject);
};