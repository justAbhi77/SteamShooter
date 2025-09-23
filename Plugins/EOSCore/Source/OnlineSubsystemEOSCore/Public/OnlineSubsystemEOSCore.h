/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemNames.h"
#include "OnlineSubsystemImpl.h"
#include "eos_sdk.h"
#include "eos_logging.h"

DECLARE_DELEGATE_OneParam(FOnEOSCoreLog, const EOS_LogMessage*);
DECLARE_STATS_GROUP(TEXT("EOS"), STATGROUP_EOS, STATCAT_Advanced);

class FOnlineIdentityEOSCore;
typedef TSharedPtr<class FOnlineIdentityEOSCore, ESPMode::ThreadSafe> FOnlineIdentityEOSCorePtr;

class FOnlineUserEOS;
typedef TSharedPtr<class FOnlineUserEOS, ESPMode::ThreadSafe> FOnlineUserEOSPtr;

class FOnlineSessionEOSCore;
typedef TSharedPtr<class FOnlineSessionEOSCore, ESPMode::ThreadSafe> FOnlineSessionEOSCorePtr;

class FOnlineStatsEOSCore;
typedef TSharedPtr<class FOnlineStatsEOSCore, ESPMode::ThreadSafe> FOnlineStatsEOSCorePtr;

class FOnlineLeaderboardsEOSCore;
typedef TSharedPtr<class FOnlineLeaderboardsEOSCore, ESPMode::ThreadSafe> FOnlineLeaderboardsEOSCorePtr;

class FOnlineAchievementsEOSCore;
typedef TSharedPtr<class FOnlineAchievementsEOSCore, ESPMode::ThreadSafe> FOnlineAchievementsEOSCorePtr;

class FOnlineStoreEOSCore;
typedef TSharedPtr<class FOnlineStoreEOSCore, ESPMode::ThreadSafe> FOnlineStoreEOSCorePtr;

class FOnlineVoiceEOSCore;
typedef TSharedPtr<class FOnlineVoiceImpl, ESPMode::ThreadSafe> FOnlineVoiceEOSCorePtr;

#ifndef EOSCORE_PRODUCTNAME_MAX_BUFFER_LEN
#define EOSCORE_PRODUCTNAME_MAX_BUFFER_LEN 64
#endif

#ifndef EOSCORE_PRODUCTVERSION_MAX_BUFFER_LEN
#define EOSCORE_PRODUCTVERSION_MAX_BUFFER_LEN 64
#endif

#define EOSCORE_SUBSYSTEM FName(TEXT("EOSCORE"))

class ONLINESUBSYSTEMEOSCORE_API FOnlineSubsystemEOSCore : public FOnlineSubsystemImpl
{
public:
	virtual ~FOnlineSubsystemEOSCore() = default;
private:
	FOnlineSubsystemEOSCore() = delete;
PACKAGE_SCOPE:
	explicit FOnlineSubsystemEOSCore(FName InInstanceName)
		: FOnlineSubsystemImpl(EOSCORE_SUBSYSTEM, InInstanceName)
		  , m_EOSPlatformHandle(nullptr)
		  , m_AuthHandle(nullptr)
		  , m_FriendsHandle(nullptr)
		  , m_UserInfoHandle(nullptr)
		  , m_PresenceHandle(nullptr)
		  , m_ConnectHandle(nullptr)
		  , m_SessionsHandle(nullptr)
		  , m_StatsHandle(nullptr)
		  , m_LeaderboardsHandle(nullptr)
		  , m_MetricsHandle(nullptr)
		  , m_AchievementsHandle(nullptr)
		  , m_P2PHandle(nullptr)
		  , m_EcomHandle(nullptr)
		  , m_LobbyHandle(nullptr)
		  , m_PlayerDataStorageHandle(nullptr)
		  , m_TitleStorageHandle(nullptr)
		  , m_UiHandle(nullptr)
		  , m_ModsHandle(nullptr)
		  , m_ReportsHandle(nullptr)
		  , m_SanctionsHandle(nullptr)
		  , m_AntiCheatClientHandle(nullptr)
		  , m_AntiCheatServerHandle(nullptr)
		  , m_KWSHandle(nullptr)
		  , m_HRTCHandle(nullptr)
		  , m_HRTCAdminHandle(nullptr)
		  , m_IdentityInterfacePtr(nullptr)
		  , m_UserInterfacePtr(nullptr)
		  , m_SessionInterfacePtr(nullptr)
		  , m_LeaderboardsInterfacePtr(nullptr)
		  , m_AchievementsInterfacePtr(nullptr)
		  , m_StoreInterfacePtr(nullptr)
	      , m_VoiceInterface(nullptr)
		  , bWasLaunchedByEGS(false)
	      , bVoiceInterfaceInitialized(false)
	{
		StopTicker();
	}

public:
	// IOnlineSubsystem
	virtual IOnlineSessionPtr GetSessionInterface() const override;
	virtual IOnlineFriendsPtr GetFriendsInterface() const override;
	virtual IOnlineSharedCloudPtr GetSharedCloudInterface() const override;
	virtual IOnlineUserCloudPtr GetUserCloudInterface() const override;
	virtual IOnlineEntitlementsPtr GetEntitlementsInterface() const override;
	virtual IOnlineLeaderboardsPtr GetLeaderboardsInterface() const override;
	virtual IOnlineVoicePtr GetVoiceInterface() const override;
	virtual IOnlineExternalUIPtr GetExternalUIInterface() const override;
	virtual IOnlineIdentityPtr GetIdentityInterface() const override;
	virtual IOnlineTitleFilePtr GetTitleFileInterface() const override;
	virtual IOnlineStoreV2Ptr GetStoreV2Interface() const override;
	virtual IOnlinePurchasePtr GetPurchaseInterface() const override;
	virtual IOnlineAchievementsPtr GetAchievementsInterface() const override;
	virtual IOnlineUserPtr GetUserInterface() const override;
	virtual IOnlinePresencePtr GetPresenceInterface() const override;
	virtual FText GetOnlineServiceName() const override;
	virtual IOnlineStatsPtr GetStatsInterface() const override;
	virtual bool Exec(class UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;

	virtual IOnlineGroupsPtr GetGroupsInterface() const override { return nullptr; }
	virtual IOnlinePartyPtr GetPartyInterface() const override { return nullptr; }
	virtual IOnlineTimePtr GetTimeInterface() const override { return nullptr; }
	virtual IOnlineEventsPtr GetEventsInterface() const override { return nullptr; }
	virtual IOnlineSharingPtr GetSharingInterface() const override { return nullptr; }
	virtual IOnlineMessagePtr GetMessageInterface() const override { return nullptr; }
	virtual IOnlineChatPtr GetChatInterface() const override { return nullptr; }
	virtual IOnlineTurnBasedPtr GetTurnBasedInterface() const override { return nullptr; }
	virtual IOnlineTournamentPtr GetTournamentInterface() const override { return nullptr; }
	//~IOnlineSubsystem

public:
	virtual bool Init() override;
	virtual bool Shutdown() override;
	virtual FString GetAppId() const override;
	static bool ModuleInit();
	static EOS_PlatformHandle* PlatformCreate();
	virtual bool IsEnabled() const override;
	virtual bool Tick(float DeltaTime) override;

PACKAGE_SCOPE:
	char m_ProductNameAnsi[EOSCORE_PRODUCTNAME_MAX_BUFFER_LEN];
	char m_ProductVersionAnsi[EOSCORE_PRODUCTVERSION_MAX_BUFFER_LEN];

PACKAGE_SCOPE:
	EOS_HPlatform m_EOSPlatformHandle;
	EOS_HAuth m_AuthHandle;
	EOS_HFriends m_FriendsHandle;
	EOS_HUserInfo m_UserInfoHandle;
	EOS_HPresence m_PresenceHandle;
	EOS_HConnect m_ConnectHandle;
	EOS_HSessions m_SessionsHandle;
	EOS_HStats m_StatsHandle;
	EOS_HLeaderboards m_LeaderboardsHandle;
	EOS_HMetrics m_MetricsHandle;
	EOS_HAchievements m_AchievementsHandle;
	EOS_HP2P m_P2PHandle;
	EOS_HEcom m_EcomHandle;
	EOS_HLobby m_LobbyHandle;
	EOS_HPlayerDataStorage m_PlayerDataStorageHandle;
	EOS_HTitleStorage m_TitleStorageHandle;
	EOS_HUI m_UiHandle;
	EOS_HMods m_ModsHandle;
	EOS_HReports m_ReportsHandle;
	EOS_HSanctions m_SanctionsHandle;
	EOS_HAntiCheatClient m_AntiCheatClientHandle;
	EOS_HAntiCheatServer m_AntiCheatServerHandle;
	EOS_HKWS m_KWSHandle;
	EOS_HRTC m_HRTCHandle;
	EOS_HRTCAdmin m_HRTCAdminHandle;
	EOS_HProgressionSnapshot m_ProgressionSnapshotHandle;
	EOS_HCustomInvites m_CustomInvitesHandle;
PACKAGE_SCOPE:
	FOnlineIdentityEOSCorePtr m_IdentityInterfacePtr;
	FOnlineUserEOSPtr m_UserInterfacePtr;
	FOnlineSessionEOSCorePtr m_SessionInterfacePtr;
	FOnlineStatsEOSCorePtr m_StatsInterfacePtr;
	FOnlineLeaderboardsEOSCorePtr m_LeaderboardsInterfacePtr;
	FOnlineAchievementsEOSCorePtr m_AchievementsInterfacePtr;
	FOnlineStoreEOSCorePtr m_StoreInterfacePtr;
	mutable FOnlineVoiceEOSCorePtr m_VoiceInterface;
PACKAGE_SCOPE:
	bool bWasLaunchedByEGS;
	mutable bool bVoiceInterfaceInitialized;
	static FOnEOSCoreLog s_OnEOSCoreLog;
PACKAGE_SCOPE:
	TSharedPtr<class FSocketSubsystemEOSCore, ESPMode::ThreadSafe> m_SocketSubsystem;
};

typedef TSharedPtr<FOnlineSubsystemEOSCore, ESPMode::ThreadSafe> FOnlineSubsystemEOSCorePtr;