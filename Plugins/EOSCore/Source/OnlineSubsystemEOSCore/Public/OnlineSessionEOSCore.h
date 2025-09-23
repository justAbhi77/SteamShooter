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
#include "Misc/ScopeLock.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystemPackageEOSCore.h"
#if UE_VERSION_OLDER_THAN(5,1,0)
#include "LANBeacon.h"
#else
#include "Online/LANBeacon.h"
#endif
#include "OnlineSubsystemTypesEOSCore.h"
#include "eos_sessions_types.h"
#include "eos_metrics_types.h"

class FOnlineSubsystemEOSCore;

static char s_BucketId[EOSCORE_OSS_STRING_BUFFER_LENGTH + 1];

typedef TEOSCallback<EOS_Sessions_OnUpdateSessionCallback, EOS_Sessions_UpdateSessionCallbackInfo> FUpdateSessionCallback;

template <typename BaseStruct>
struct TNamedSessionOptions :
	public BaseStruct
{
	char SessionNameAnsi[EOSCORE_OSS_STRING_BUFFER_LENGTH + 1];

	TNamedSessionOptions(const char* InSessionNameAnsi)
		: BaseStruct()
	{
		FCStringAnsi::Strncpy(SessionNameAnsi, InSessionNameAnsi, EOSCORE_OSS_STRING_BUFFER_LENGTH + 1);
		this->SessionName = SessionNameAnsi;
	}
};

struct FSessionCreateOptions :
	public TNamedSessionOptions<EOS_Sessions_CreateSessionModificationOptions>
{
	FSessionCreateOptions(const char* InSessionNameAnsi) :
		TNamedSessionOptions<EOS_Sessions_CreateSessionModificationOptions>(InSessionNameAnsi)
	{
		ApiVersion = EOS_SESSIONS_CREATESESSIONMODIFICATION_API_LATEST;
		BucketId = s_BucketId;
	}
};

struct FSendSessionInviteOptions :
	public TNamedSessionOptions<EOS_Sessions_SendInviteOptions>
{
	FSendSessionInviteOptions(const char* InSessionNameAnsi) :
		TNamedSessionOptions<EOS_Sessions_SendInviteOptions>(InSessionNameAnsi)
	{
		ApiVersion = EOS_SESSIONS_SENDINVITE_API_LATEST;
	}
};

struct FJoinSessionOptions :
	public TNamedSessionOptions<EOS_Sessions_JoinSessionOptions>
{
	FJoinSessionOptions(const char* InSessionNameAnsi) :
		TNamedSessionOptions<EOS_Sessions_JoinSessionOptions>(InSessionNameAnsi)
	{
		ApiVersion = EOS_SESSIONS_JOINSESSION_API_LATEST;
	}
};

struct FSessionDestroyOptions :
	public TNamedSessionOptions<EOS_Sessions_DestroySessionOptions>
{
	FSessionDestroyOptions(const char* InSessionNameAnsi) :
		TNamedSessionOptions<EOS_Sessions_DestroySessionOptions>(InSessionNameAnsi)
	{
		ApiVersion = EOS_SESSIONS_DESTROYSESSION_API_LATEST;
	}
};


struct FSessionUpdateOptions :
	public TNamedSessionOptions<EOS_Sessions_UpdateSessionModificationOptions>
{
	FSessionUpdateOptions(const char* InSessionNameAnsi) :
		TNamedSessionOptions<EOS_Sessions_UpdateSessionModificationOptions>(InSessionNameAnsi)
	{
		ApiVersion = EOS_SESSIONS_UPDATESESSIONMODIFICATION_API_LATEST;
	}
};

struct FSessionStartOptions :
	public TNamedSessionOptions<EOS_Sessions_StartSessionOptions>
{
	FSessionStartOptions(const char* InSessionNameAnsi) :
		TNamedSessionOptions<EOS_Sessions_StartSessionOptions>(InSessionNameAnsi)
	{
		ApiVersion = EOS_SESSIONS_STARTSESSION_API_LATEST;
	}
};

struct FSessionEndOptions :
	public TNamedSessionOptions<EOS_Sessions_EndSessionOptions>
{
	FSessionEndOptions(const char* InSessionNameAnsi) :
		TNamedSessionOptions<EOS_Sessions_EndSessionOptions>(InSessionNameAnsi)
	{
		ApiVersion = EOS_SESSIONS_ENDSESSION_API_LATEST;
	}
};

struct FEndMetricsOptions :
	public EOS_Metrics_EndPlayerSessionOptions
{
	char ExternalIdAnsi[EOSCORE_OSS_STRING_BUFFER_LENGTH + 1];

	FEndMetricsOptions() :
		EOS_Metrics_EndPlayerSessionOptions()
	{
		ApiVersion = EOS_METRICS_ENDPLAYERSESSION_API_LATEST;
		AccountId.External = ExternalIdAnsi;
	}
};

struct FSessionSearchEOSCore
{
	EOS_HSessionSearch SearchHandle;

	FSessionSearchEOSCore(EOS_HSessionSearch InSearchHandle)
		: SearchHandle(InSearchHandle)
	{
	}

	virtual ~FSessionSearchEOSCore()
	{
		EOS_SessionSearch_Release(SearchHandle);
	}
};

class FOnlineSessionEOSCore : public IOnlineSession
{
public:
	FOnlineSessionEOSCore() = delete;
	virtual ~FOnlineSessionEOSCore() = default;

PACKAGE_SCOPE:
	FOnlineSessionEOSCore(FOnlineSubsystemEOSCore* InSubsystem)
		: m_CurrentSessionSearch(nullptr)
		  , m_SessionSearchStartInSeconds(0)
		  , m_Subsystem(InSubsystem)
	{
	}

public:
	virtual TSharedPtr<const FUniqueNetId> CreateSessionIdFromString(const FString& SessionIdStr) override;

	FNamedOnlineSession* GetNamedSession(FName SessionName) override
	{
		FScopeLock ScopeLock(&m_SessionLock);
		for (int32 SearchIndex = 0; SearchIndex < m_Sessions.Num(); SearchIndex++)
		{
			if (m_Sessions[SearchIndex].SessionName == SessionName)
			{
				return &m_Sessions[SearchIndex];
			}
		}
		return nullptr;
	}

	virtual void RemoveNamedSession(FName SessionName) override
	{
		FScopeLock ScopeLock(&m_SessionLock);
		for (int32 SearchIndex = 0; SearchIndex < m_Sessions.Num(); SearchIndex++)
		{
			if (m_Sessions[SearchIndex].SessionName == SessionName)
			{
				m_Sessions.RemoveAtSwap(SearchIndex);
				return;
			}
		}
	}

	virtual EOnlineSessionState::Type GetSessionState(FName SessionName) const override
	{
		FScopeLock ScopeLock(&m_SessionLock);
		for (int32 SearchIndex = 0; SearchIndex < m_Sessions.Num(); SearchIndex++)
		{
			if (m_Sessions[SearchIndex].SessionName == SessionName)
			{
				return m_Sessions[SearchIndex].SessionState;
			}
		}

		return EOnlineSessionState::NoSession;
	}

	virtual bool HasPresenceSession() override
	{
		FScopeLock ScopeLock(&m_SessionLock);
		for (int32 SearchIndex = 0; SearchIndex < m_Sessions.Num(); SearchIndex++)
		{
			if (m_Sessions[SearchIndex].SessionSettings.bUsesPresence)
			{
				return true;
			}
		}

		return false;
	}

	// IOnlineSession Interface
public:
	virtual bool CreateSession(int32 HostingPlayerNum, FName SessionName, const FOnlineSessionSettings& NewSessionSettings) override;
	virtual bool CreateSession(const FUniqueNetId& HostingPlayerId, FName SessionName, const FOnlineSessionSettings& NewSessionSettings) override;
	virtual bool StartSession(FName SessionName) override;
	virtual bool UpdateSession(FName SessionName, FOnlineSessionSettings& UpdatedSessionSettings, bool bShouldRefreshOnlineData = true) override;
	virtual bool EndSession(FName SessionName) override;
	virtual bool DestroySession(FName SessionName, const FOnDestroySessionCompleteDelegate& CompletionDelegate = FOnDestroySessionCompleteDelegate()) override;
	virtual bool IsPlayerInSession(FName SessionName, const FUniqueNetId& UniqueId) override;
	virtual bool StartMatchmaking(const TArray<TSharedRef<const FUniqueNetId>>& LocalPlayers, FName SessionName, const FOnlineSessionSettings& NewSessionSettings,
	                              TSharedRef<FOnlineSessionSearch>& SearchSettings) override;
	virtual bool CancelMatchmaking(int32 SearchingPlayerNum, FName SessionName) override;
	virtual bool CancelMatchmaking(const FUniqueNetId& SearchingPlayerId, FName SessionName) override;
	virtual bool FindSessions(int32 SearchingPlayerNum, const TSharedRef<FOnlineSessionSearch>& SearchSettings) override;
	virtual bool FindSessions(const FUniqueNetId& SearchingPlayerId, const TSharedRef<FOnlineSessionSearch>& SearchSettings) override;
	virtual bool FindSessionById(const FUniqueNetId& SearchingUserId, const FUniqueNetId& SessionId, const FUniqueNetId& FriendId,
	                             const FOnSingleSessionResultCompleteDelegate& CompletionDelegate) override;
	virtual bool CancelFindSessions() override;
	virtual bool PingSearchResults(const FOnlineSessionSearchResult& SearchResult) override;
	virtual bool JoinSession(int32 PlayerNum, FName SessionName, const FOnlineSessionSearchResult& DesiredSession) override;
	virtual bool JoinSession(const FUniqueNetId& PlayerId, FName SessionName, const FOnlineSessionSearchResult& DesiredSession) override;
	virtual bool FindFriendSession(int32 LocalUserNum, const FUniqueNetId& Friend) override;
	virtual bool FindFriendSession(const FUniqueNetId& LocalUserId, const FUniqueNetId& Friend) override;
	virtual bool FindFriendSession(const FUniqueNetId& LocalUserId, const TArray<TSharedRef<const FUniqueNetId>>& FriendList) override;
	virtual bool SendSessionInviteToFriend(int32 LocalUserNum, FName SessionName, const FUniqueNetId& Friend) override;
	virtual bool SendSessionInviteToFriend(const FUniqueNetId& LocalUserId, FName SessionName, const FUniqueNetId& Friend) override;
	virtual bool SendSessionInviteToFriends(int32 LocalUserNum, FName SessionName, const TArray<TSharedRef<const FUniqueNetId>>& Friends) override;
	virtual bool SendSessionInviteToFriends(const FUniqueNetId& LocalUserId, FName SessionName, const TArray<TSharedRef<const FUniqueNetId>>& Friends) override;
	virtual bool GetResolvedConnectString(FName SessionName, FString& ConnectInfo, FName PortType) override;
	virtual bool GetResolvedConnectString(const FOnlineSessionSearchResult& SearchResult, FName PortType, FString& ConnectInfo) override;
	virtual FOnlineSessionSettings* GetSessionSettings(FName SessionName) override;
	virtual bool RegisterPlayer(FName SessionName, const FUniqueNetId& PlayerId, bool bWasInvited) override;
	virtual bool RegisterPlayers(FName SessionName, const TArray<TSharedRef<const FUniqueNetId>>& Players, bool bWasInvited = false) override;
	virtual bool UnregisterPlayer(FName SessionName, const FUniqueNetId& PlayerId) override;
	virtual bool UnregisterPlayers(FName SessionName, const TArray<TSharedRef<const FUniqueNetId>>& Players) override;
	virtual void RegisterLocalPlayer(const FUniqueNetId& PlayerId, FName SessionName, const FOnRegisterLocalPlayerCompleteDelegate& Delegate) override;
	virtual void UnregisterLocalPlayer(const FUniqueNetId& PlayerId, FName SessionName, const FOnUnregisterLocalPlayerCompleteDelegate& Delegate) override;
	virtual int32 GetNumSessions() override;
	virtual void DumpSessionState() override;
	// ~IOnlineSession Interface

PACKAGE_SCOPE:
	mutable FCriticalSection m_SessionLock;
	TArray<FNamedOnlineSession> m_Sessions;
	TSharedPtr<FOnlineSessionSearch> m_CurrentSessionSearch;
	double m_SessionSearchStartInSeconds;

PACKAGE_SCOPE:
	void Tick(float DeltaTime);

PACKAGE_SCOPE:
	// IOnlineSession
	class FNamedOnlineSession* AddNamedSession(FName SessionName, const FOnlineSessionSettings& SessionSettings) override
	{
		FScopeLock ScopeLock(&m_SessionLock);
		return new(m_Sessions) FNamedOnlineSession(SessionName, SessionSettings);
	}

	class FNamedOnlineSession* AddNamedSession(FName SessionName, const FOnlineSession& Session) override
	{
		FScopeLock ScopeLock(&m_SessionLock);
		return new(m_Sessions) FNamedOnlineSession(SessionName, Session);
	}
	
	void CheckPendingSessionInvite();
	void RegisterLocalPlayers(class FNamedOnlineSession* Session);
	void Init(const char* InBucketId);
public:
	void CopySearchResult(EOS_HSessionDetails SessionHandle, const EOS_SessionDetails_Info* SessionInfo, FOnlineSession& SessionSettings) const;
	void CopyAttributes(EOS_HSessionDetails SessionHandle, FOnlineSession& OutSession) const;
private:
	uint32 CreateEOSSession(int32 HostingPlayerNum, FNamedOnlineSession* Session);
	uint32 JoinEOSSession(int32 PlayerNum, FNamedOnlineSession* Session, const FOnlineSession* SearchSession);
	uint32 StartEOSSession(FNamedOnlineSession* Session);
	uint32 UpdateEOSSession(FNamedOnlineSession* Session, FOnlineSessionSettings& UpdatedSessionSettings);
	uint32 EndEOSSession(FNamedOnlineSession* Session);
	uint32 DestroyEOSSession(FNamedOnlineSession* Session, const FOnDestroySessionCompleteDelegate& CompletionDelegate);
	uint32 FindEOSSession(int32 SearchingPlayerNum, const TSharedRef<FOnlineSessionSearch>& SearchSettings);
	bool SendSessionInvite(FName SessionName, EOS_ProductUserId SenderId, EOS_ProductUserId ReceiverId) const;

	void BeginSessionAnalytics(const FNamedOnlineSession* Session) const;

	void UpdatePresenceJoinInfo(int32 LocalUserNum, FNamedOnlineSession* Session) const;

	void EndSessionAnalytics() const;

	void AddSearchResult(EOS_HSessionDetails SessionHandle, const TSharedRef<FOnlineSessionSearch>& SearchSettings);
	void AddSearchAttribute(EOS_HSessionSearch SearchHandle, const EOS_Sessions_AttributeData* Attribute, EOS_EOnlineComparisonOp ComparisonOp) const;

	void SetPermissionLevel(EOS_HSessionModification SessionModHandle, const FNamedOnlineSession* Session) const;
	void SetJoinInProgress(EOS_HSessionModification SessionModHandle, const FNamedOnlineSession* Session) const;
	void AddAttribute(EOS_HSessionModification SessionModHandle, const EOS_Sessions_AttributeData* Attribute);
	void SetAttributes(EOS_HSessionModification SessionModHandle, FNamedOnlineSession* Session);
	uint32 SharedSessionUpdate(EOS_HSessionModification SessionModHandle, FNamedOnlineSession* Session, FUpdateSessionCallback* Callback);

	void AppendSessionToPacket(class FNboSerializeToBufferEOSCore& Packet, const class FOnlineSession* Session) const;
	static void AppendSessionSettingsToPacket(class FNboSerializeToBufferEOSCore& Packet, const FOnlineSessionSettings* SessionSettings);
	void ReadSessionFromPacket(class FNboSerializeFromBufferEOSCore& Packet, class FOnlineSession* Session);
	void ReadSettingsFromPacket(class FNboSerializeFromBufferEOSCore& Packet, FOnlineSessionSettings& SessionSettings);
	void OnValidQueryPacketReceived(uint8* PacketData, int32 PacketLength, uint64 ClientNonce);
	static void SetPortFromNetDriver(const FOnlineSubsystemEOSCore& Subsystem, const TSharedPtr<FOnlineSessionInfo>& SessionInfo);
	bool IsHost(const FNamedOnlineSession& Session) const;
	void RegisterVoice(const FUniqueNetId& PlayerId);
	void UnregisterVoice(const FUniqueNetId& PlayerId);
	void DestroyLocalVOIPComponents();
	// LAN stuff
private:
	void TickLAN(float DeltaTime) const;
	uint32 CreateLANSession(int32 HostingPlayerNum, FNamedOnlineSession* Session);
	uint32 JoinLANSession(int32 PlayerNum, class FNamedOnlineSession* Session, const class FOnlineSession* SearchSession);
	uint32 FindLANSession();
	void OnSearchingTimeout();
	void OnValidResponsePacketReceived(uint8* PacketData, int32 PacketLength);
	
private:
	FOnlineSubsystemEOSCore* m_Subsystem;
	TSharedPtr<FLANSession> m_LANSession;
	TSharedPtr<FSessionSearchEOSCore> m_CurrentSearchHandle;
	EOS_NotificationId m_SessionInviteAcceptedId;
	EOS_NotificationId m_SessionInviteReceivedId;
	FCallbackBase* m_SessionInviteAcceptedCallback;
	FCallbackBase* m_SessionInviteReceivedCallback;
private:
	bool bIsDedicatedServer;
	bool bIsUsingP2PSockets;
};

typedef TSharedPtr<FOnlineSessionEOSCore, ESPMode::ThreadSafe> FOnlineSessionEOSCorePtr;
