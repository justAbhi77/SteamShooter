/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "eos_sessions_types.h"
#include "Core/EOSHelpers.h"
#include "EOSSessionsTypes.generated.h"

class UCoreSessions;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		ENUMS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
/** All possible states of an existing named session */
UENUM(BlueprintType)
enum class EEOSEOnlineSessionState : uint8
{
	/** An online session has not been created yet */
	EOS_OSS_NoSession = 0,
	/** An online session is in the process of being created */
	EOS_OSS_Creating = 1,
	/** Session has been created but the session hasn't started (pre match lobby) */
	EOS_OSS_Pending = 2,
	/** Session has been asked to start (may take time due to communication with backend) */
	EOS_OSS_Starting = 3,
	/** The current session has started. Sessions with join in progress disabled are no longer joinable */
	EOS_OSS_InProgress = 4,
	/** The session is still valid, but the session is no longer being played (post match lobby) */
	EOS_OSS_Ending = 5,
	/** The session is closed and any stats committed */
	EOS_OSS_Ended = 6,
	/** The session is being destroyed */
	EOS_OSS_Destroying = 7
};

/** Advertisement properties for a single attribute associated with a session */
UENUM(BlueprintType)
enum class EEOSESessionAttributeAdvertisementType : uint8
{
	/** Don't advertise via the online service */
	EOS_SAAT_DontAdvertise = 0,
	/** Advertise via the online service only */
	EOS_SAAT_Advertise = 1
};

/** Permission level gets more restrictive further down */
UENUM(BlueprintType)
enum class EEOSEOnlineSessionPermissionLevel : uint8
{
	/** Anyone can find this session as long as it isn't full */
	EOS_OSPF_PublicAdvertised = 0,
	/** Players who have access to presence can see this session */
	EOS_OSPF_JoinViaPresence = 1,
	/** Only players with invites registered can see this session */
	EOS_OSPF_InviteOnly = 2
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		STRUCTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

/** Handle to the sessions interface */
USTRUCT(BlueprintType)
struct FEOSHSessions
{
	GENERATED_BODY()
public:
	EOS_HSessions Handle;
public:
	explicit FEOSHSessions()
		: Handle(nullptr)
	{
	}

	FEOSHSessions(const EOS_HSessions& data)
		: Handle(data)
	{
	}

public:
	operator EOS_HSessions() { return Handle; }
};

/** Handle to the calls responsible for creating/updating sessions */
USTRUCT(BlueprintType)
struct FEOSHSessionModification
{
	GENERATED_BODY()
public:
	EOS_HSessionModification Handle;
public:
	explicit FEOSHSessionModification()
		: Handle(nullptr)
	{
	}

	FEOSHSessionModification(const EOS_HSessionModification& data)
		: Handle(data)
	{
	}

public:
	operator EOS_HSessionModification() const { return Handle; }
};

/** Handle to a single named session that exists locally */
USTRUCT(BlueprintType)
struct FEOSHActiveSession
{
	GENERATED_BODY()
public:
	EOS_HActiveSession Handle;
public:
	explicit FEOSHActiveSession()
		: Handle(nullptr)
	{
	}

	FEOSHActiveSession(const EOS_HActiveSession& data)
		: Handle(data)
	{
	}

public:
	operator EOS_HActiveSession() const { return Handle; }
};

/** Handle to a single session that may be part of a named session, search result, or invite */
USTRUCT(BlueprintType)
struct FEOSHSessionDetails
{
	GENERATED_BODY()
public:
	EOS_HSessionDetails Handle;
public:
	explicit FEOSHSessionDetails()
		: Handle(nullptr)
	{
	}

	FEOSHSessionDetails(const EOS_HSessionDetails& data)
		: Handle(data)
	{
	}

public:
	operator EOS_HSessionDetails() const { return Handle; }
};

/** Handle to the calls responsible for creating a search object */
USTRUCT(BlueprintType)
struct FEOSHSessionSearch
{
	GENERATED_BODY()
public:
	EOS_HSessionSearch Handle;
public:
	explicit FEOSHSessionSearch()
		: Handle(nullptr)
	{
	}

	FEOSHSessionSearch(const EOS_HSessionSearch& data)
		: Handle(data)
	{
	}

public:
	operator EOS_HSessionSearch() const { return Handle; }
};

/**
 * Input parameters for the EOS_Sessions_CreateSessionModification Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsCreateSessionModificationOptions
{
	GENERATED_BODY()
public:
	/** Name of the session to create */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString SessionName;
	/** Bucket id associated with the session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString BucketId;
	/** Maximum number of players allowed in the session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	int32 MaxPlayers;
	/** Local user id associated with the session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSProductUserId LocalUserId;
	/**
	 * If true than this session will be used as the session associated with presence.
	 * Only one session at a time can have this flag true.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	bool bPresenceEnabled;
	/**
	 * Optional session id - set to a globally unique value to override the backend assignment
	 * If not specified the backend service will assign one to the session.  Do not mix and match.
	 * This value can be of size [EOS_SESSIONMODIFICATION_MIN_SESSIONIDOVERRIDE_LENGTH, EOS_SESSIONMODIFICATION_MAX_SESSIONIDOVERRIDE_LENGTH]
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString SessionId;
	/**
	 * If true, sanctioned players can neither join nor register with this session and, in the case of join,
	 * will return EOS_EResult code EOS_Sessions_PlayerSanctioned
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	bool bSanctionsEnabled;
	/** 
	 * Array of platform IDs indicating the player platforms allowed to register with the session. Platform IDs are
	 * found in the EOS header file, e.g. EOS_OPT_Epic. For some platforms, the value will be in the EOS Platform specific
	 * header file. If null, the session will be unrestricted.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	TArray<int32> AllowedPlatformIds;
public:
	explicit FEOSSessionsCreateSessionModificationOptions()
		: MaxPlayers(0)
		, bPresenceEnabled(false)
		, bSanctionsEnabled(false)
	{
	}
};

/** The most recent version of the EOS_Sessions_UpdateSessionModification API. */
USTRUCT(BlueprintType)
struct FEOSSessionModificationSetAllowedPlatformIdsOptions
{
	GENERATED_BODY()
public:
	/** 
	 * Array of platform IDs indicating the player platforms allowed to register with the session. Platform IDs are
	 * found in the EOS header file, e.g. EOS_OPT_Epic. For some platforms, the value will be in the EOS Platform specific
	 * header file. If null, the session will be unrestricted.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	TArray<int32> AllowedPlatformIds;
public:
	explicit FEOSSessionModificationSetAllowedPlatformIdsOptions()
	{
	}
};

/** The most recent version of the EOS_Sessions_UpdateSessionModification API. */
USTRUCT(BlueprintType)
struct FEOSSessionsUpdateSessionModificationOptions
{
	GENERATED_BODY()
public:
	/** Name of the session to update */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString SessionName;
public:
	explicit FEOSSessionsUpdateSessionModificationOptions()
	{
	}
};

/**
 * Input parameters for the EOS_Sessions_SendInvite Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsSendInviteOptions
{
	GENERATED_BODY()
public:
	/** Name of the session associated with the invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString SessionName;
	/** The local user inviting */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSProductUserId LocalUserId;
	/** The remote user being invited */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSProductUserId TargetUserId;
public:
	explicit FEOSSessionsSendInviteOptions()
	{
	}
};

/**
 * Output parameters for the EOS_Sessions_SendInvite function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsSendInviteCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Sessions_SendInvite */
	void* ClientData;
public:
	explicit FEOSSessionsSendInviteCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSSessionsSendInviteCallbackInfo(const EOS_Sessions_SendInviteCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
	{
	}
};

/**
 * Input parameters for the EOS_Sessions_RejectInvite Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsRejectInviteOptions
{
	GENERATED_BODY()
public:
	/** The local user rejecting the invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSProductUserId LocalUserId;
	/** The invite id to reject */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString InviteId;
public:
	explicit FEOSSessionsRejectInviteOptions()
	{
	}
};

/**
 * Output parameters for the EOS_Sessions_RejectInvite function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsRejectInviteCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Sessions_RejectInvite */
	void* ClientData;
public:
	explicit FEOSSessionsRejectInviteCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSSessionsRejectInviteCallbackInfo(const EOS_Sessions_RejectInviteCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
	{
	}
};

/**
 * Input parameters for the EOS_Sessions_QueryInvites Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsQueryInvitesOptions
{
	GENERATED_BODY()
public:
	/** Local User Id to query invites */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSProductUserId LocalUserId;
public:
	explicit FEOSSessionsQueryInvitesOptions()
	{
	}
};

/**
 * Output parameters for the EOS_Sessions_QueryInvites function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsQueryInvitesCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Sessions_QueryInvites */
	void* ClientData;
	/** Local User Id that made the request */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSProductUserId LocalUserId;
public:
	explicit FEOSSessionsQueryInvitesCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSSessionsQueryInvitesCallbackInfo(const EOS_Sessions_QueryInvitesCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
	{
	}
};

/**
 * Input parameters for the EOS_Sessions_GetInviteCount Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsGetInviteCountOptions
{
	GENERATED_BODY()
public:
	/** Local User Id to query invites */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSProductUserId LocalUserId;
public:
	explicit FEOSSessionsGetInviteCountOptions()
	{
	}
};

/**
 * Input parameters for the EOS_Sessions_GetInviteIdByIndex Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsGetInviteIdByIndexOptions
{
	GENERATED_BODY()
public:
	/** Local user that has invites */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSProductUserId LocalUserId;
	/** Index of the invite id to retrieve */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	int32 Index;
public:
	explicit FEOSSessionsGetInviteIdByIndexOptions(): Index(0)
	{
	}
};

/**
 * Input parameters for the EOS_Sessions_CreateSessionSearch Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsCreateSessionSearchOptions
{
	GENERATED_BODY()
public:
	/** Max number of results to return */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	int32 MaxSearchResults;
public:
	explicit FEOSSessionsCreateSessionSearchOptions()
		: MaxSearchResults(50)
	{
	}
};

/** The most recent version of the EOS_Sessions_UpdateSession API. */
USTRUCT(BlueprintType)
struct FEOSSessionsUpdateSessionOptions
{
	GENERATED_BODY()
public:
	/** Builder handle */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSHSessionModification SessionModificationHandle;
public:
	explicit FEOSSessionsUpdateSessionOptions()
	{
	}
};

/**
 * Output parameters for the EOS_Sessions_UpdateSession function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsUpdateSessionCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Sessions_UpdateSession */
	void* ClientData;
	/** Name of the session that was created/modified */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString SessionName;
	/** Id of the session that was created/modified */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString SessionId;
public:
	explicit FEOSSessionsUpdateSessionCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSSessionsUpdateSessionCallbackInfo(const EOS_Sessions_UpdateSessionCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
	{
		const FUTF8ToTCHAR SessionNameChar(data.SessionName);
		SessionName = SessionNameChar.Get();

		const FUTF8ToTCHAR SessionIdChar(data.SessionId);
		SessionId = SessionIdChar.Get();
	}
};

/**
 * Input parameters for the EOS_Sessions_DestroySession function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsDestroySessionOptions
{
	GENERATED_BODY()
public:
	/** Name of the session to destroy */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString SessionName;
public:
	explicit FEOSSessionsDestroySessionOptions()
	{
	}
};

/**
 * Output parameters for the EOS_Sessions_DestroySession function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsDestroySessionCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Sessions_DestroySession */
	void* ClientData;
public:
	explicit FEOSSessionsDestroySessionCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSSessionsDestroySessionCallbackInfo(const EOS_Sessions_DestroySessionCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSSessionsJoinSessionOptions
{
	GENERATED_BODY()
public:
	/** Name of the session to create after joining session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString SessionName;
	/** Session handle to join */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSHSessionDetails SessionHandle;
	/** Local user joining the session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSProductUserId LocalUserId;
	/**
	 * If true than this session will be used as the session associated with presence.
	 * Only one session at a time can have this flag true.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	bool bPresenceEnabled;
public:
	explicit FEOSSessionsJoinSessionOptions()
		: bPresenceEnabled(false)
	{
	}
};

/**
 * Output parameters for the EOS_Sessions_JoinSession function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsJoinSessionCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Sessions_JoinSession */
	void* ClientData;
public:
	explicit FEOSSessionsJoinSessionCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSSessionsJoinSessionCallbackInfo(const EOS_Sessions_JoinSessionCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
	{
	}
};

/**
 * Input parameters for the EOS_Sessions_StartSessionOptions Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsStartSessionOptions
{
	GENERATED_BODY()
public:
	/** Name of the session to set in progress */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString SessionName;
public:
	explicit FEOSSessionsStartSessionOptions()
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSSessionsStartSessionCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Sessions_StartSession */
	void* ClientData;
public:
	explicit FEOSSessionsStartSessionCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSSessionsStartSessionCallbackInfo(const EOS_Sessions_StartSessionCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
	{
	}
};

/**
 * Input parameters for the EOS_Sessions_EndSessionOptions Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsEndSessionOptions
{
	GENERATED_BODY()
public:
	/** Name of the session to set as no long in progress */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString SessionName;
public:
	explicit FEOSSessionsEndSessionOptions()
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSSessionsEndSessionCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Sessions_EndSession */
	void* ClientData;
public:
	explicit FEOSSessionsEndSessionCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSSessionsEndSessionCallbackInfo(const EOS_Sessions_EndSessionCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
	{
	}
};

/**
 * Input parameters for the EOS_Sessions_RegisterPlayers Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsRegisterPlayersOptions
{
	GENERATED_BODY()
public:
	/** Name of the session for which to register players */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString SessionName;
	/** Array of players to register with the session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	TArray<FEOSProductUserId> PlayersToRegister;
public:
	explicit FEOSSessionsRegisterPlayersOptions()
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSSessionsRegisterPlayersCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Sessions_RegisterPlayers */
	void* ClientData;
	/** The players that were successfully registered */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	TArray<FEOSProductUserId> RegisteredPlayers;
	/** The number of players successfully registered */
	/** The players that failed to register because they are sanctioned */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	TArray<FEOSProductUserId> SanctionedPlayers;
public:
	explicit FEOSSessionsRegisterPlayersCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSSessionsRegisterPlayersCallbackInfo(const EOS_Sessions_RegisterPlayersCallbackInfo& Data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(Data.ResultCode))
		  , ClientData(Data.ClientData)
	{
		for (uint32_t i = 0; i < Data.RegisteredPlayersCount; i++)
		{
			RegisteredPlayers.Add(Data.RegisteredPlayers[i]);
		}

		for (uint32_t i = 0; i < Data.SanctionedPlayersCount; i++)
		{
			SanctionedPlayers.Add(Data.SanctionedPlayers[i]);
		}
	}
};

/**
 * Input parameters for the EOS_Sessions_UnregisterPlayers Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsUnregisterPlayersOptions
{
	GENERATED_BODY()
public:
	/** Name of the session for which to unregister players */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString SessionName;
	/** Array of players to unregister from the session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	TArray<FEOSProductUserId> PlayersToUnregister;
public:
	explicit FEOSSessionsUnregisterPlayersOptions()
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSSessionsUnregisterPlayersCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Sessions_UnregisterPlayers */
	void* ClientData;
	/** The players that successfully unregistered */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	TArray<FEOSProductUserId> UnregisteredPlayers;
public:
	explicit FEOSSessionsUnregisterPlayersCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSSessionsUnregisterPlayersCallbackInfo(const EOS_Sessions_UnregisterPlayersCallbackInfo& Data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(Data.ResultCode))
		  , ClientData(Data.ClientData)
	{
		for (uint32_t i = 0; i < Data.UnregisteredPlayersCount; i++)
		{
			UnregisteredPlayers.Add(Data.UnregisteredPlayers[i]);
		}
	}
};

/**
 * Input parameters for the EOS_SessionModification_SetBucketId Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionModificationSetBucketIdOptions
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString BucketId;
public:
	explicit FEOSSessionModificationSetBucketIdOptions()
	{
	}
};

/**
 * Input parameters for the EOS_SessionModification_SetHostAddress Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionModificationSetHostAddressOptions
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString HostAddress;
public:
	explicit FEOSSessionModificationSetHostAddressOptions()
	{
	}
};

/**
 * Input parameters for the EOS_SessionModification_SetPermissionLevel Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionModificationSetPermissionLevelOptions
{
	GENERATED_BODY()
public:
	/** Permission level to set on the session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	EEOSEOnlineSessionPermissionLevel PermissionLevel;
public:
	explicit FEOSSessionModificationSetPermissionLevelOptions()
		: PermissionLevel(EEOSEOnlineSessionPermissionLevel::EOS_OSPF_PublicAdvertised)
	{
	}
};

/**
 * Input parameters for the EOS_SessionModification_SetJoinInProgressAllowed Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionModificationSetJoinInProgressAllowedOptions
{
	GENERATED_BODY()
public:
	/** Does the session allow join in progress */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	bool bAllowJoinInProgress;
public:
	explicit FEOSSessionModificationSetJoinInProgressAllowedOptions()
		: bAllowJoinInProgress(false)
	{
	}
};

/**
 * Input parameters for the EOS_SessionModification_SetMaxPlayers Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionModificationSetMaxPlayersOptions
{
	GENERATED_BODY()
public:
	/** Max number of players to allow in the session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	int32 MaxPlayers;
public:
	explicit FEOSSessionModificationSetMaxPlayersOptions()
		: MaxPlayers(10)
	{
	}
};

/**
 * Input parameters for the EOS_SessionModification_SetInvitesAllowed Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionModificationSetInvitesAllowedOptions
{
	GENERATED_BODY()
public:
	/** If true then invites can currently be sent for the associated session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	bool bInvitesAllowed;
public:
	explicit FEOSSessionModificationSetInvitesAllowedOptions()
		: bInvitesAllowed(false)
	{
	}
};

/**
 * Contains information about both session and search parameter attribution
 */
USTRUCT(BlueprintType)
struct FEOSSessionsAttributeData
{
	GENERATED_BODY()
	EOS_Sessions_AttributeData AttributeData;
public:
	explicit FEOSSessionsAttributeData(): AttributeData()
	{
	}

	FEOSSessionsAttributeData(const EOS_Sessions_AttributeData& data)
		: AttributeData(data)
	{
	}
};

/**
 * Input parameters for the EOS_ActiveSession_CopyInfo Function.
 */
USTRUCT(BlueprintType)
struct FEOSActiveSessionCopyInfoOptions
{
	GENERATED_BODY()
public:
	explicit FEOSActiveSessionCopyInfoOptions()
	{
	}
};

/**
 * Input parameters for the EOS_ActiveSession_GetRegisteredPlayerCount Function.
 */
USTRUCT(BlueprintType)
struct FEOSActiveSessionGetRegisteredPlayerCountOptions
{
	GENERATED_BODY()
public:
	explicit FEOSActiveSessionGetRegisteredPlayerCountOptions()
	{
	}
};

/**
 * Input parameters for the EOS_ActiveSession_GetRegisteredPlayerByIndex Function.
 */
USTRUCT(BlueprintType)
struct FEOSActiveSessionGetRegisteredPlayerByIndexOptions
{
	GENERATED_BODY()
public:
	/** Index of the registered player to retrieve */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	int32 PlayerIndex;
public:
	explicit FEOSActiveSessionGetRegisteredPlayerByIndexOptions()
		: PlayerIndex(0)
	{
	}
};

/**
 *  An attribution value and its advertisement setting stored with a session.
 */
USTRUCT(BlueprintType)
struct FEOSSessionDetailsAttribute
{
	GENERATED_BODY()
public:
	/** Key/Value pair describing the attribute */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSSessionsAttributeData Data;
	/** Is this attribution advertised with the backend or simply stored locally */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	EEOSESessionAttributeAdvertisementType AdvertisementType;
public:
	explicit FEOSSessionDetailsAttribute()
		: AdvertisementType(EEOSESessionAttributeAdvertisementType::EOS_SAAT_Advertise)
	{
	}

	FEOSSessionDetailsAttribute(const EOS_SessionDetails_Attribute& data)
		: Data(*data.Data)
		  , AdvertisementType(static_cast<EEOSESessionAttributeAdvertisementType>(data.AdvertisementType))
	{
	}
};

/**
 * Input parameters for the EOS_SessionModification_AddAttribute Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionModificationAddAttributeOptions
{
	GENERATED_BODY()
public:
	/** Key/Value pair describing the attribute to add to the session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSSessionsAttributeData Data;
	/** Is this attribution advertised with the backend or simply stored locally */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	EEOSESessionAttributeAdvertisementType AdvertisementType;
public:
	explicit FEOSSessionModificationAddAttributeOptions()
		: AdvertisementType(EEOSESessionAttributeAdvertisementType::EOS_SAAT_Advertise)
	{
	}
};

/**
 * Input parameters for the EOS_SessionModification_RemoveAttribute Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionModificationRemoveAttributeOptions
{
	GENERATED_BODY()
public:
	/** Session attribute to remove from the session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString Key;
public:
	explicit FEOSSessionModificationRemoveAttributeOptions()
	{
	}
};

/**
 * Input parameters for the EOS_SessionSearch_SetMaxResults Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionSearchSetMaxResultsOptions
{
	GENERATED_BODY()
public:
	/** Maximum number of search results returned with this query, may not exceed EOS_SESSIONS_MAX_SEARCH_RESULTS */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	int32 MaxSearchResults;
public:
	explicit FEOSSessionSearchSetMaxResultsOptions()
		: MaxSearchResults(50)
	{
	}
};

/**
 * Input parameters for the EOS_SessionSearch_Find Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionSearchFindOptions
{
	GENERATED_BODY()
public:
	/** Local user who is searching */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSProductUserId LocalUserId;
public:
	explicit FEOSSessionSearchFindOptions()
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSSessionSearchFindCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	EOSResult ResultCode;
	/** Context that was passed into EOS_SessionSearch_Find */
	void* ClientData;
public:
	explicit FEOSSessionSearchFindCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSSessionSearchFindCallbackInfo(const EOS_SessionSearch_FindCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
	{
	}
};

/**
 * Input parameters for the EOS_SessionSearch_GetSearchResultCount Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionSearchGetSearchResultCountOptions
{
	GENERATED_BODY()
public:
	explicit FEOSSessionSearchGetSearchResultCountOptions()
	{
	}
};

/**
 * Input parameters for the EOS_SessionSearch_CopySearchResultByIndex Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionSearchCopySearchResultByIndexOptions
{
	GENERATED_BODY()
public:
	/**
	* The index of the session to retrieve within the completed search query
	* @see EOS_SessionSearch_GetSearchResultCount
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	int32 SessionIndex;
public:
	explicit FEOSSessionSearchCopySearchResultByIndexOptions()
		: SessionIndex(0)
	{
	}
};

/**
 * Input parameters for the EOS_SessionSearch_SetSessionId Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionSearchSetSessionIdOptions
{
	GENERATED_BODY()
public:
	/** Search sessions for a specific session id, returning at most one session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString SessionId;
public:
	explicit FEOSSessionSearchSetSessionIdOptions()
	{
	}
};

/**
 * Input parameters for the EOS_SessionSearch_SetTargetUserId Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionSearchSetTargetUserIdOptions
{
	GENERATED_BODY()
public:
	/** Search sessions for given user, returning any sessions where this player is currently registered */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSProductUserId TargetUserId;
public:
	explicit FEOSSessionSearchSetTargetUserIdOptions()
	{
	}
};

/**
 * Input parameters for the EOS_SessionSearch_SetParameter Function.
 *
 * A search key may be set more than once to make multiple comparisons
 * The two comparisons are AND'd together
 * (ie, Key GREATER_THAN 5, Key NOT_EQUALS 10)
 */
USTRUCT(BlueprintType)
struct FEOSSessionSearchSetParameterOptions
{
	GENERATED_BODY()
public:
	/** Search parameter describing a key and a value to compare */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSSessionsAttributeData Parameter;
	/** The type of comparison to make against the search parameter */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	EEOSEComparisonOp ComparisonOp;
public:
	explicit FEOSSessionSearchSetParameterOptions()
		: ComparisonOp(EEOSEComparisonOp::EOS_CO_EQUAL)
	{
	}
};

/**
 * Input parameters for the EOS_SessionSearch_RemoveParameter Function.
 *
 * Removal requires both the key and its comparator in order to remove as the same key can be used in more than one operation
 */
USTRUCT(BlueprintType)
struct FEOSSessionSearchRemoveParameterOptions
{
	GENERATED_BODY()
public:
	/** Search parameter key to remove from the search */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString Key;
	/** The type of comparison to make against the search parameter */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	EEOSEComparisonOp ComparisonOp;
public:
	explicit FEOSSessionSearchRemoveParameterOptions()
		: ComparisonOp(EEOSEComparisonOp::EOS_CO_EQUAL)
	{
	}
};

/** Common settings associated with a single session */
USTRUCT(BlueprintType)
struct FEOSSessionDetailsSettings
{
	GENERATED_BODY()
public:
	/** The main indexed parameter for this session, can be any string (ie "Region:GameMode") */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString BucketId;
	/** Number of total players allowed in the session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	int32 NumPublicConnections;
	/** Are players allowed to join the session while it is in the "in progress" state */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	bool bAllowJoinInProgress;
	/** Permission level describing allowed access to the session when joining or searching for the session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	EEOSEOnlineSessionPermissionLevel PermissionLevel;
	/** Are players allowed to send invites for the session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	bool bInvitesAllowed;
	/** Are sanctioned players allowed to join - sanctioned players will be rejected if set to true */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	bool bSanctionsEnabled;
	/** 
	 * Array of platform IDs indicating the player platforms allowed to register with the session. Platform IDs are
	 * found in the EOS header file, e.g. EOS_OPT_Epic. For some platforms, the value will be in the EOS Platform specific
	 * header file. If null, the session will be unrestricted.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	TArray<int32> AllowedPlatformIds;
public:
	explicit FEOSSessionDetailsSettings()
		: NumPublicConnections(0)
		  , bAllowJoinInProgress(false)
		  , PermissionLevel(EEOSEOnlineSessionPermissionLevel::EOS_OSPF_PublicAdvertised)
		  , bInvitesAllowed(false)
		  , bSanctionsEnabled(false)
	{
	}

	FEOSSessionDetailsSettings(const EOS_SessionDetails_Settings& Data)
		: NumPublicConnections(Data.NumPublicConnections)
		  , bAllowJoinInProgress((Data.bAllowJoinInProgress > 0))
		  , PermissionLevel(static_cast<EEOSEOnlineSessionPermissionLevel>(Data.PermissionLevel))
		  , bInvitesAllowed((Data.bInvitesAllowed > 0))
		  , bSanctionsEnabled(Data.bSanctionsEnabled > 0)
	{
		const FUTF8ToTCHAR BucketIdChar(Data.BucketId);
		BucketId = BucketIdChar.Get();
		for (uint32_t i=0; i<Data.AllowedPlatformIdsCount; i++)
		{
			AllowedPlatformIds.Add(Data.AllowedPlatformIds[i]);
		}
	}
};

/** Internal details about a session, found on both active sessions and within search results */
USTRUCT(BlueprintType)
struct FEOSSessionDetailsInfo
{
	GENERATED_BODY()
public:
	/** Session id assigned by the backend service */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString SessionId;
	/** IP address of this session as visible by the backend service */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString HostAddress;
	/** Number of remaining open spaces on the session (NumPublicConnections - RegisteredPlayers */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	int32 NumOpenPublicConnections;
	/** Reference to the additional settings associated with this session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSSessionDetailsSettings Settings;
	/** The Product User ID of the session owner. Null if the session is not owned by a user. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSProductUserId OwnerUserId;
	/** The client id of the session owner. Null if the session is not owned by a server. The session is owned by a server if EOS_Platform_Options::bIsServer is EOS_TRUE. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString OwnerServerClientId;
public:
	explicit FEOSSessionDetailsInfo()
		: NumOpenPublicConnections(10)
	{
	}

	FEOSSessionDetailsInfo(const EOS_SessionDetails_Info& Data)
		: NumOpenPublicConnections(Data.NumOpenPublicConnections)
		, Settings(*Data.Settings)
		, OwnerUserId(Data.OwnerUserId)
		, OwnerServerClientId(UTF8_TO_TCHAR(Data.OwnerServerClientId))
	{
		const FUTF8ToTCHAR SessionIdChar(Data.SessionId);
		SessionId = SessionIdChar.Get();

		const FUTF8ToTCHAR HostAddressChar(Data.HostAddress);
		HostAddress = HostAddressChar.Get();
	}
};

/**
 * Input parameters for the EOS_SessionDetails_CopyInfo Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionDetailsCopyInfoOptions
{
	GENERATED_BODY()
public:
	explicit FEOSSessionDetailsCopyInfoOptions()
	{
	}
};

/**
 * Input parameters for the EOS_SessionDetails_GetSessionAttributeCount Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionDetailsGetSessionAttributeCountOptions
{
	GENERATED_BODY()
public:
	explicit FEOSSessionDetailsGetSessionAttributeCountOptions()
	{
	}
};

/**
 * Input parameters for the EOS_SessionDetails_CopySessionAttributeByIndex Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionDetailsCopySessionAttributeByIndexOptions
{
	GENERATED_BODY()
public:
	/**
	 * The index of the attribute to retrieve
	 * @see EOS_SessionDetails_GetSessionAttributeCount
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	int32 AttrIndex;
public:
	explicit FEOSSessionDetailsCopySessionAttributeByIndexOptions()
		: AttrIndex(0)
	{
	}
};

/**
 * Input parameters for the EOS_SessionDetails_CopySessionAttributeByKey Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionDetailsCopySessionAttributeByKeyOptions
{
	GENERATED_BODY()
public:
	/** The name of the key to get the session attribution for
	* @see EOS_SessionModification_AddAttribute
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString AttrKey;
public:
	explicit FEOSSessionDetailsCopySessionAttributeByKeyOptions()
	{
	}
};

/**
 * Top level details about an active session
 */
USTRUCT(BlueprintType)
struct FEOSActiveSessionInfo
{
	GENERATED_BODY()
public:
	/** Name of the session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString SessionName;
	/** Local user who created or joined the session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSProductUserId LocalUserId;
	/** Current state of the session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	EEOSEOnlineSessionState State;
	/** Session details */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSSessionDetailsInfo SessionDetails;
public:
	explicit FEOSActiveSessionInfo()
		: State(EEOSEOnlineSessionState::EOS_OSS_Creating)
	{
	}

	FEOSActiveSessionInfo(const EOS_ActiveSession_Info& data)
		: LocalUserId(data.LocalUserId)
		  , State(static_cast<EEOSEOnlineSessionState>(data.State))
		  , SessionDetails(*data.SessionDetails)
	{
		const FUTF8ToTCHAR SessionNameChar(data.SessionName);
		SessionName = SessionNameChar.Get();
	}
};

/**
 * Input parameters for the EOS_Sessions_CopyActiveSessionHandle Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsCopyActiveSessionHandleOptions
{
	GENERATED_BODY()
public:
	/** Name of the session for which to retrieve a session handle */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString SessionName;
public:
	explicit FEOSSessionsCopyActiveSessionHandleOptions()
	{
	}
};

/** The most recent version of the EOS_Sessions_AddNotifySessionInviteReceived API. */
USTRUCT(BlueprintType)
struct FEOSSessionsAddNotifySessionInviteReceivedOptions
{
	GENERATED_BODY()
public:
	explicit FEOSSessionsAddNotifySessionInviteReceivedOptions()
	{
	}
};

/**
 * Output parameters for the EOS_Sessions_OnSessionInviteReceivedCallback Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsSessionInviteReceivedCallbackInfo
{
	GENERATED_BODY()
public:
	/** Context that was passed into EOS_Sessions_AddNotifySessionInviteReceived */
	void* ClientData;
	/** User that received the invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSProductUserId LocalUserId;
	/** Target user that sent the invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSProductUserId TargetUserId;
	/** Invite Id used to retrieve the actual session details */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString InviteId;
public:
	explicit FEOSSessionsSessionInviteReceivedCallbackInfo()
		: ClientData(nullptr)
	{
	}

	FEOSSessionsSessionInviteReceivedCallbackInfo(const EOS_Sessions_SessionInviteReceivedCallbackInfo& data)
		: ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
		  , TargetUserId(data.TargetUserId)
	{
		const FUTF8ToTCHAR InviteIdChar(data.InviteId);
		InviteId = InviteIdChar.Get();
	}
};

/**
 * Output parameters for the EOS_Sessions_OnSessionInviteRejectedCallback function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsSessionInviteRejectedCallbackInfo
{
	GENERATED_BODY()
public:
	/** The invite ID */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString InviteId;
	/** The Product User ID of the local user who rejected the invitation */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSProductUserId LocalUserId;
	/** The Product User ID of the user who sent the invitation */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSProductUserId TargetUserId;
	/** Optional session ID */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString SessionId;
public:
	explicit FEOSSessionsSessionInviteRejectedCallbackInfo()
	{
	}

	FEOSSessionsSessionInviteRejectedCallbackInfo(const EOS_Sessions_SessionInviteRejectedCallbackInfo& Data)
	 	: LocalUserId(Data.LocalUserId)
	 	, TargetUserId(Data.TargetUserId)
	{
		const FUTF8ToTCHAR InviteIdChar(Data.InviteId);
		InviteId = InviteIdChar.Get();
		
		const FUTF8ToTCHAR SessionIdChar(Data.SessionId);
		SessionId = SessionIdChar.Get();
	}
};

/**
 * Output parameters for the EOS_Sessions_OnLeaveSessionRequestedCallback Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsLeaveSessionRequestedCallbackInfo
{
	GENERATED_BODY()
public:
	/** The Product User ID of the local user who received the leave session notification. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSProductUserId LocalUserId;
	/** Name of the session associated with the leave session request. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString SessionName;
public:
	explicit FEOSSessionsLeaveSessionRequestedCallbackInfo()
	{
	}

	FEOSSessionsLeaveSessionRequestedCallbackInfo(const EOS_Sessions_LeaveSessionRequestedCallbackInfo& Data)
		 : LocalUserId(Data.LocalUserId)
	{
		const FUTF8ToTCHAR SessionNameChar(Data.SessionName);
		SessionName = SessionNameChar.Get();
	}
};

/**
 * Output parameters for the EOS_Sessions_OnLeaveSessionRequestedCallback Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsSendSessionNativeInviteRequestedCallbackInfo
{
	GENERATED_BODY()
public:
	/**
	 * Identifies this event which will need to be acknowledged with EOS_UI_AcknowledgeEventId().
	 * @see EOS_UI_AcknowledgeEventId
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSUIEventId UiEventId;
	/** The Product User ID of the local user who is inviting. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSProductUserId LocalUserId;
	/**
	 * The Native Platform Account Type. If only a single integrated platform is configured then
	 * this will always reference that platform.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString TargetNativeAccountType;
	/** The Native Platform Account ID of the target user being invited. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString TargetUserNativeAccountId;
	/** Session ID that the user is being invited to */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString SessionId;
public:
	explicit FEOSSessionsSendSessionNativeInviteRequestedCallbackInfo()
	{
	}

	FEOSSessionsSendSessionNativeInviteRequestedCallbackInfo(const EOS_Sessions_SendSessionNativeInviteRequestedCallbackInfo& Data)
		 : LocalUserId(Data.LocalUserId)
	{
		const FUTF8ToTCHAR TargetNativeAccountTypeChar(Data.TargetNativeAccountType);
		TargetNativeAccountType = TargetNativeAccountTypeChar.Get();

		const FUTF8ToTCHAR TargetUserNativeAccountIdChar(Data.TargetUserNativeAccountId);
		TargetUserNativeAccountId = TargetUserNativeAccountIdChar.Get();

		const FUTF8ToTCHAR SessionIdChar(Data.SessionId);
		SessionId = SessionIdChar.Get();
	}
};

/** The most recent version of the EOS_Sessions_AddNotifySessionInviteAccepted API. */
USTRUCT(BlueprintType)
struct FEOSSessionsAddNotifySessionInviteAcceptedOptions
{
	GENERATED_BODY()
public:
	explicit FEOSSessionsAddNotifySessionInviteAcceptedOptions()
	{
	}
};

/**
 * Output parameters for the EOS_Sessions_OnSessionInviteAcceptedCallback Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsSessionInviteAcceptedCallbackInfo
{
	GENERATED_BODY()
public:
	/** Context that was passed into EOS_Sessions_AddNotifySessionInviteReceived */
	void* ClientData;
	/** Session Id that should be used for joining */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString SessionId;
	/** User that accepted the invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSProductUserId LocalUserId;
	/** Target user that sent the invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSProductUserId TargetUserId;
	/** Invite id that was accepted */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString InviteId;
public:
	explicit FEOSSessionsSessionInviteAcceptedCallbackInfo()
		: ClientData(nullptr)
	{
	}

	FEOSSessionsSessionInviteAcceptedCallbackInfo(const EOS_Sessions_SessionInviteAcceptedCallbackInfo& data)
		: ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
		  , TargetUserId(data.TargetUserId)
	{
		const FUTF8ToTCHAR SessionIdChar(data.SessionId);
		SessionId = SessionIdChar.Get();

		const FUTF8ToTCHAR InviteIdChar(data.InviteId);
		InviteId = InviteIdChar.Get();
	}
};

/**
 * Output parameters for the EOS_Sessions_OnJoinSessionAcceptedCallback Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsJoinSessionAcceptedCallbackInfo
{
	GENERATED_BODY()
public:
	/** Context that was passed into EOS_Sessions_AddNotifyJoinSessionAccepted */
	void* ClientData;
	/** User that initialized the join game */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSProductUserId LocalUserId;
	/**
	 * The UI Event associated with this Join Game event.
	 * This should be used with EOS_Sessions_CopySessionHandleByUiEventId to get a handle to be used
	 * when calling EOS_Sessions_JoinSession.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSUIEventId UiEventId;
public:
	explicit FEOSSessionsJoinSessionAcceptedCallbackInfo()
		: ClientData(nullptr)
	{
	}

	FEOSSessionsJoinSessionAcceptedCallbackInfo(const EOS_Sessions_JoinSessionAcceptedCallbackInfo& data)
		: ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
		  , UiEventId(data.UiEventId)
	{
	}
};

/**
 * Input parameters for the EOS_Sessions_CopySessionHandleByInviteId Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsCopySessionHandleByInviteIdOptions
{
	GENERATED_BODY()
public:
	/** Invite id for which to retrieve a session handle */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString InviteId;
public:
	explicit FEOSSessionsCopySessionHandleByInviteIdOptions()
	{
	}
};

/**
 * Input parameters for the EOS_Sessions_CopySessionHandleByUiEventId Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsCopySessionHandleByUiEventIdOptions
{
	GENERATED_BODY()
public:
	/** UI Event associated with the session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSUIEventId UiEventId;
public:
	explicit FEOSSessionsCopySessionHandleByUiEventIdOptions()
	{
	}
};

/**
 * Input parameters for the EOS_Sessions_CopySessionHandleForPresence Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsCopySessionHandleForPresenceOptions
{
	GENERATED_BODY()
public:
	/** Local user id associated with the session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSProductUserId LocalUserId;
public:
	explicit FEOSSessionsCopySessionHandleForPresenceOptions()
	{
	}
};

/**
 * Input parameters for the EOS_Sessions_IsUserInSession Function.
 */
USTRUCT(BlueprintType)
struct FEOSSessionsIsUserInSessionOptions
{
	GENERATED_BODY()
public:
	/** Active session name to search within */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString SessionName;
	/** Product User Id to search for in the session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FEOSProductUserId TargetUserId;
public:
	explicit FEOSSessionsIsUserInSessionOptions()
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSSessionsDumpSessionStateOptions
{
	GENERATED_BODY()
public:
	/** Name of the session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sessions")
	FString SessionName;
public:
	explicit FEOSSessionsDumpSessionStateOptions()
	{
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		DELEGATES
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSessionSessionSendInviteCallback, const FEOSSessionsSendInviteCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSessionRejectInviteCallback, const FEOSSessionsRejectInviteCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSessionQueryInvitesCallback, const FEOSSessionsQueryInvitesCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSessionUpdateSessionCallback, const FEOSSessionsUpdateSessionCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSessionDestroySessionCallback, const FEOSSessionsDestroySessionCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSessionJoinSessionCallback, const FEOSSessionsJoinSessionCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSessionStartSessionCallback, const FEOSSessionsStartSessionCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSessionEndSessionCallback, const FEOSSessionsEndSessionCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSessionRegisterPlayersCallback, const FEOSSessionsRegisterPlayersCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSessionUnregisterPlayersCallback, const FEOSSessionsUnregisterPlayersCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSessionFindCallback, const FEOSSessionSearchFindCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSessionInviteReceivedCallback, const FEOSSessionsSessionInviteReceivedCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSessionInviteAcceptedCallback, const FEOSSessionsSessionInviteAcceptedCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSessionJoinSessionAcceptedCallback, const FEOSSessionsJoinSessionAcceptedCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSessionInviteRejectedCallback, const FEOSSessionsSessionInviteRejectedCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSendSessionNativeInviteRequestedCallback, const FEOSSessionsSendSessionNativeInviteRequestedCallbackInfo&, Data);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionSessionSendInviteCallbackDelegate, const FEOSSessionsSendInviteCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionRejectInviteCallbackDelegate, const FEOSSessionsRejectInviteCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionQueryInvitesCallbackDelegate, const FEOSSessionsQueryInvitesCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionUpdateSessionCallbackDelegate, const FEOSSessionsUpdateSessionCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionDestroySessionCallbackDelegate, const FEOSSessionsDestroySessionCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionJoinSessionCallbackDelegate, const FEOSSessionsJoinSessionCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionStartSessionCallbackDelegate, const FEOSSessionsStartSessionCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionEndSessionCallbackDelegate, const FEOSSessionsEndSessionCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionRegisterPlayersCallbackDelegate, const FEOSSessionsRegisterPlayersCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionUnregisterPlayersCallbackDelegate, const FEOSSessionsUnregisterPlayersCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionFindCallbackDelegate, const FEOSSessionSearchFindCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionInviteReceivedCallbackDelegate, const FEOSSessionsSessionInviteReceivedCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionInviteAcceptedCallbackDelegate, const FEOSSessionsSessionInviteAcceptedCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionJoinSessionAcceptedCallbackDelegate, const FEOSSessionsJoinSessionAcceptedCallbackInfo&, Data);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		CALLBACK OBJECTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

struct FUpdateSessionCallback
{
public:
	FOnSessionUpdateSessionCallback m_Callback;
public:
	FUpdateSessionCallback(const FOnSessionUpdateSessionCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FUpdateSessionCallback()
	{
		m_Callback.Unbind();
	}
};

struct FDestroySessionCallback
{
public:
	FOnSessionDestroySessionCallback m_Callback;
public:
	FDestroySessionCallback(const FOnSessionDestroySessionCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FDestroySessionCallback()
	{
		m_Callback.Unbind();
	}
};

struct FJoinSessionCallback
{
public:
	FOnSessionJoinSessionCallback m_Callback;
public:
	FJoinSessionCallback(const FOnSessionJoinSessionCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FJoinSessionCallback()
	{
		m_Callback.Unbind();
	}
};

struct FStartSessionCallback
{
public:
	FOnSessionStartSessionCallback m_Callback;
public:
	FStartSessionCallback(const FOnSessionStartSessionCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FStartSessionCallback()
	{
		m_Callback.Unbind();
	}
};

struct FEndSessionCallback
{
public:
	FOnSessionEndSessionCallback m_Callback;
public:
	FEndSessionCallback(const FOnSessionEndSessionCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FEndSessionCallback()
	{
		m_Callback.Unbind();
	}
};

struct FRegisterPlayersCallback
{
public:
	FOnSessionRegisterPlayersCallback m_Callback;
public:
	FRegisterPlayersCallback(const FOnSessionRegisterPlayersCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FRegisterPlayersCallback()
	{
		m_Callback.Unbind();
	}
};

struct FUnregisterPlayersCallback
{
public:
	FOnSessionUnregisterPlayersCallback m_Callback;
public:
	FUnregisterPlayersCallback(const FOnSessionUnregisterPlayersCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FUnregisterPlayersCallback()
	{
		m_Callback.Unbind();
	}
};

struct FSendInviteCallback
{
public:
	FOnSessionSessionSendInviteCallback m_Callback;
public:
	FSendInviteCallback(const FOnSessionSessionSendInviteCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FSendInviteCallback()
	{
		m_Callback.Unbind();
	}
};

struct FRejectInviteCallback
{
public:
	FOnSessionRejectInviteCallback m_Callback;
public:
	FRejectInviteCallback(const FOnSessionRejectInviteCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FRejectInviteCallback()
	{
		m_Callback.Unbind();
	}
};

struct FQueryInvitesCallback
{
public:
	FOnSessionQueryInvitesCallback m_Callback;
public:
	FQueryInvitesCallback(const FOnSessionQueryInvitesCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FQueryInvitesCallback()
	{
		m_Callback.Unbind();
	}
};

struct FSessionFindCallback
{
public:
	FOnSessionFindCallback m_Callback;
public:
	FSessionFindCallback(const FOnSessionFindCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FSessionFindCallback()
	{
		m_Callback.Unbind();
	}
};

struct FSessionInviteReceivedCallback
{
public:
	FSessionInviteReceivedCallback(const FOnSessionInviteReceivedCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FSessionInviteReceivedCallback()
	{
		m_Callback.Unbind();
	}

public:
	FOnSessionInviteReceivedCallback m_Callback;
};

struct FSessionInviteAcceptedCallback
{
public:
	FSessionInviteAcceptedCallback(const FOnSessionInviteAcceptedCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FSessionInviteAcceptedCallback()
	{
		m_Callback.Unbind();
	}

public:
	FOnSessionInviteAcceptedCallback m_Callback;
};

struct FSessionJoinSessionAcceptedCallback
{
public:
	FSessionJoinSessionAcceptedCallback(const FOnSessionJoinSessionAcceptedCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FSessionJoinSessionAcceptedCallback()
	{
		m_Callback.Unbind();
	}

public:
	FOnSessionJoinSessionAcceptedCallback m_Callback;
};
