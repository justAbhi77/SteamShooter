/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "eos_lobby_types.h"
#include "Core/EOSHelpers.h"
#include "EOSLobbyTypes.generated.h"

class UCoreLobby;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		ENUMS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
/** Permission level gets more restrictive further down */
UENUM(BlueprintType)
enum class EEOSELobbyPermissionLevel : uint8
{
	/** Anyone can find this lobby as long as it isn't full */
	EOS_LPL_PUBLICADVERTISED = 0,
	/** Players who have access to presence can see this lobby */
	EOS_LPL_JOINVIAPRESENCE = 1,
	/** Only players with invites registered can see this lobby */
	EOS_LPL_INVITEONLY = 2
};


/** Advertisement properties for a single attribute associated with a lobby */
UENUM(BlueprintType)
enum class EEOSELobbyAttributeVisibility : uint8
{
	/*&, data is visible outside the lobby */
	EOS_LAT_PUBLIC = 0,
	/** Only members in the lobby can see this data */
	EOS_LAT_PRIVATE = 1
};

/** Various types of lobby member updates */
UENUM(BlueprintType)
enum class EEOSELobbyMemberStatus : uint8
{
	/** The user has joined the lobby */
	EOS_LMS_JOINED = 0,
	/** The user has explicitly left the lobby */
	EOS_LMS_LEFT = 1,
	/** The user has unexpectedly left the lobby */
	EOS_LMS_DISCONNECTED = 2,
	/** The user has been kicked from the lobby */
	EOS_LMS_KICKED = 3,
	/** The user has been promoted to lobby owner */
	EOS_LMS_PROMOTED = 4,
	/** The lobby has been closed and user has been removed */
	EOS_LMS_CLOSED = 5
};


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		STRUCTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
/** Handle to a single lobby */
USTRUCT(BlueprintType)
struct FEOSHLobbyDetails
{
	GENERATED_BODY()
public:
	EOS_HLobbyDetails Handle;
public:
	explicit FEOSHLobbyDetails()
		: Handle(nullptr)
	{
	}

	FEOSHLobbyDetails(const EOS_HLobbyDetails& data)
		: Handle(data)
	{
	}

public:
	operator EOS_HLobbyDetails() const
	{
		return Handle;
	}
};

/** Handle to a lobby modification object */
USTRUCT(BlueprintType)
struct FEOSHLobbyModification
{
	GENERATED_BODY()
public:
	EOS_HLobbyModification Handle;
public:
	explicit FEOSHLobbyModification()
		: Handle(nullptr)
	{
	}

	FEOSHLobbyModification(const EOS_HLobbyModification& data)
		: Handle(data)
	{
	}

public:
	operator EOS_HLobbyModification() const
	{
		return Handle;
	}
};

USTRUCT(BlueprintType)
struct FEOSLobbyDetailsInfo
{
	GENERATED_BODY()
public:
	/** Lobby id */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
	/** Current owner of the lobby */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LobbyOwnerUserId;
	/** Permission level of the lobby */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EEOSELobbyPermissionLevel PermissionLevel;
	/** Current available space */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	int32 AvailableSlots;
	/** Max allowed members in the lobby */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	int32 MaxMembers;
	/** Are invites allowed */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	bool bAllowInvites;
	/** The main indexed parameter for this lobby, can be any string (ie "Region:GameMode") */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString BucketId;
	/** Is host migration allowed */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	bool bAllowHostMigration;
	/** Was a Real-Time Communication (RTC) room enabled at lobby creation? */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	bool bRTCRoomEnabled;
	/** Is EOS_Lobby_JoinLobbyById allowed */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	bool bAllowJoinById;
	/** Does rejoining after being kicked require an invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	bool bRejoinAfterKickRequiresInvite;
	/** If true, this lobby will be associated with the local user's presence information. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	bool bPresenceEnabled;
	/** 
	 * Array of platform IDs indicating the player platforms allowed to register with the session. Platform IDs are
	 * found in the EOS header file, e.g. EOS_OPT_Epic. For some platforms, the value will be in the EOS Platform specific
	 * header file. If null, the lobby will be unrestricted.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	TArray<int32> AllowedPlatformIds;
public:
	FEOSLobbyDetailsInfo()
		: PermissionLevel(EEOSELobbyPermissionLevel::EOS_LPL_PUBLICADVERTISED)
		, AvailableSlots(0)
		, MaxMembers(0)
		, bAllowInvites(false)
		, bAllowHostMigration(false)
		, bRTCRoomEnabled(false)
		, bAllowJoinById(false)
		, bRejoinAfterKickRequiresInvite(false)
		, bPresenceEnabled(false)
	{
	}

	FEOSLobbyDetailsInfo(const EOS_LobbyDetails_Info& Data)
		: LobbyOwnerUserId(Data.LobbyOwnerUserId)
		, PermissionLevel(static_cast<EEOSELobbyPermissionLevel>(Data.PermissionLevel))
		, AvailableSlots(Data.AvailableSlots)
		, MaxMembers(Data.MaxMembers)
		, bAllowInvites(Data.bAllowInvites > 0)
		, bAllowHostMigration((Data.bAllowHostMigration > 0))
		, bRTCRoomEnabled((Data.bRTCRoomEnabled > 0))
		, bAllowJoinById(Data.bAllowJoinById > 0)
		, bRejoinAfterKickRequiresInvite(Data.bRejoinAfterKickRequiresInvite > 0)
		, bPresenceEnabled(Data.bPresenceEnabled > 0)
	{
		const FUTF8ToTCHAR LobbyIdChar(Data.LobbyId);
		LobbyId = LobbyIdChar.Get();

		const FUTF8ToTCHAR BucketIdChar(Data.BucketId);
		BucketId = BucketIdChar.Get();

		for (uint32_t i = 0; i<Data.AllowedPlatformIdsCount; i++)
		{
			AllowedPlatformIds.Add(Data.AllowedPlatformIdsCount);
		}
	}
};

USTRUCT(BlueprintType)
struct FEOSLobbyLocalRTCOptions
{
	GENERATED_BODY()
public:
	/** Flags for the local user in this room. The default is 0 if this struct is not specified. @see EOS_RTC_JoinRoomOptions::Flags */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	int32 Flags;
	/**
	* Set to EOS_TRUE to enable Manual Audio Input. If manual audio input is enabled, audio recording is not started and the audio buffers
	* must be passed manually using EOS_RTCAudio_SendAudio. The default is EOS_FALSE if this struct is not specified.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	bool bUseManualAudioInput;
	/**
	* Set to EOS_TRUE to enable Manual Audio Output. If manual audio output is enabled, audio rendering is not started and the audio buffers
	* must be received with EOS_RTCAudio_AddNotifyAudioBeforeRender and rendered manually. The default is EOS_FALSE if this struct is not
	* specified.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	bool bUseManualAudioOutput;
	/**
	* Set to EOS_TRUE to start the outgoing audio stream muted by when first connecting to the RTC room. It must be manually unmuted with a
	* call to EOS_RTCAudio_UpdateSending. If manual audio output is enabled, this value is ignored. The default is EOS_FALSE if this struct
	* is not specified.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	bool bLocalAudioDeviceInputStartsMuted;
public:
	FEOSLobbyLocalRTCOptions()
		: Flags(0)
		  , bUseManualAudioInput(false)
		  , bUseManualAudioOutput(false)
		  , bLocalAudioDeviceInputStartsMuted(false)
	{
	}

	FEOSLobbyLocalRTCOptions(const EOS_Lobby_LocalRTCOptions& data)
		: Flags(data.Flags)
		  , bUseManualAudioInput((data.bUseManualAudioInput > 0))
		  , bUseManualAudioOutput((data.bUseManualAudioOutput > 0))
		  , bLocalAudioDeviceInputStartsMuted((data.bLocalAudioDeviceInputStartsMuted > 0))
	{
	}

public:
	operator EOS_Lobby_LocalRTCOptions() const
	{
		EOS_Lobby_LocalRTCOptions Data;

		Data.ApiVersion = EOS_LOBBY_LOCALRTCOPTIONS_API_LATEST;
		Data.Flags = Flags;
		Data.bUseManualAudioInput = bUseManualAudioInput;
		Data.bUseManualAudioOutput = bUseManualAudioOutput;
		Data.bLocalAudioDeviceInputStartsMuted = bLocalAudioDeviceInputStartsMuted;

		return Data;
	}
};

/**
 * Input parameters for the EOS_Lobby_CreateLobby Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyCreateLobbyOptions
{
	GENERATED_BODY()
public:
	/** Local user creating the lobby */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
	/** Max members allowed in the lobby */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	int32 MaxLobbyMembers;
	/** The initial permission level of the lobby */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EEOSELobbyPermissionLevel PermissionLevel;
	/** If true, this lobby will be associated with presence information. A user's presence can only be associated with one lobby at a time.
	* This affects the ability of the Social Overlay to show game related actions to take in the user's social graph.
	*
	* @note The Social Overlay can handle only one of the following three options at a time:
	* * using the bPresenceEnabled flags within the Sessions interface
	* * using the bPresenceEnabled flags within the Lobby interface
	* * using EOS_PresenceModification_SetJoinInfo
	*
	* @see EOS_PresenceModification_SetJoinInfoOptions
	* @see EOS_Lobby_JoinLobbyOptions
	* @see EOS_Sessions_CreateSessionModificationOptions
	* @see EOS_Sessions_JoinSessionOptions
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	bool bPresenceEnabled;
	/** Are members of the lobby allowed to invite others */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	bool bAllowInvites;
	/** Bucket ID associated with the lobby */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString BucketId;
	/** 
	* Is host migration allowed (will the lobby stay open if the original host leaves?) 
	* NOTE: EOS_Lobby_PromoteMember is still allowed regardless of this setting 
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	bool bDisableHostMigration;
	/**
	* Creates a real-time communication (RTC) room for all members of this lobby. All members of the lobby will automatically join the RTC
	* room when they connect to the lobby and they will automatically leave the RTC room when they leave or are removed from the lobby.
	* While the joining and leaving of the RTC room is automatic, applications will still need to use the EOS RTC interfaces to handle all
	* other functionality for the room.
	*
	* @see EOS_Lobby_GetRTCRoomName
	* @see EOS_Lobby_AddNotifyRTCRoomConnectionChanged
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	bool bEnableRTCRoom;
	/**
	* (Optional) Allows the local application to set local audio options for the RTC Room if it is enabled. Set this to NULL if the RTC
	* RTC room is disabled or you would like to use the defaults.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSLobbyLocalRTCOptions LocalRTCOptions;
	/**
	* (Optional) Set to a globally unique value to override the backend assignment
	* If not specified the backend service will assign one to the lobby.  Do not mix and match override and non override settings.
	* This value can be of size [EOS_LOBBY_MIN_LOBBYIDOVERRIDE_LENGTH, EOS_LOBBY_MAX_LOBBYIDOVERRIDE_LENGTH]
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
	/**
	 * Is EOS_Lobby_JoinLobbyById allowed.
	 * This is provided to support cases where an integrated platform's invite system is used.
	 * In these cases the game should provide the lobby ID securely to the invited player.  Such as by attaching the
	 * lobby ID to the integrated platform's session data or sending the lobby ID within the invite data.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	bool bEnableJoinById;
	/**
	 * Does rejoining after being kicked require an invite?
	 * When this is set, a kicked player cannot return to the session even if the session was set with 
	 * EOS_LPL_PUBLICADVERTISED.  When this is set, a player with invite privileges must use EOS_Lobby_SendInvite to
	 * allow the kicked player to return to the session.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	bool bRejoinAfterKickRequiresInvite;
	/** 
	 * Array of platform IDs indicating the player platforms allowed to register with the session. Platform IDs are
	 * found in the EOS header file, e.g. EOS_OPT_Epic. For some platforms, the value will be in the EOS Platform specific
	 * header file. If null, the lobby will be unrestricted.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	TArray<int32> AllowedPlatformIds;
	/** Number of platform IDs in the array */
	/** 
	* This value indicates whether or not the lobby owner allows crossplay interactions. If false, the lobby owner
	* will be treated as allowing crossplay. If it is set to true, AllowedPlatformIds must have a single entry that matches 
	* the platform of the lobby owner.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	bool bCrossplayOptOut;
public:
	explicit FEOSLobbyCreateLobbyOptions()
		: MaxLobbyMembers(10)
		, PermissionLevel(EEOSELobbyPermissionLevel::EOS_LPL_PUBLICADVERTISED)
		, bPresenceEnabled(true)
		, bAllowInvites(true)
		, BucketId("MyBucket")
		, bDisableHostMigration(false)
		, bEnableRTCRoom(false)
		, bEnableJoinById(false)
		, bRejoinAfterKickRequiresInvite(false)
		, bCrossplayOptOut(false)
	{
	}
};

/**
 * Output parameters for the EOS_Lobby_CreateLobby function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyCreateLobbyCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Lobby_CreateLobby */
	void* ClientData;
	/** Newly created lobby id */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
public:
public:
	FEOSLobbyCreateLobbyCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSLobbyCreateLobbyCallbackInfo(const EOS_Lobby_CreateLobbyCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
	{
		const FUTF8ToTCHAR LobbyIdChar(data.LobbyId);
		LobbyId = LobbyIdChar.Get();
	}
};

/**
 * Input parameters for the EOS_Lobby_DestroyLobby Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyDestroyLobbyOptions
{
	GENERATED_BODY()
public:
	/** Local user destroying the lobby, must own the lobby */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
	/** Lobby Id to destroy */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
public:
	FEOSLobbyDestroyLobbyOptions() = default;
};

/**
 * Output parameters for the EOS_Lobby_DestroyLobby function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyDestroyLobbyCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Lobby_DestroyLobby */
	void* ClientData;
	/** Destroyed lobby id */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
public:
public:
	FEOSLobbyDestroyLobbyCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSLobbyDestroyLobbyCallbackInfo(const EOS_Lobby_DestroyLobbyCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
	{
		const FUTF8ToTCHAR LobbyIdChar(data.LobbyId);
		LobbyId = LobbyIdChar.Get();
	}
};

/**
 * Input parameters for the EOS_Lobby_JoinLobby Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyJoinLobbyOptions
{
	GENERATED_BODY()
public:
	/** Lobby handle to join */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSHLobbyDetails LobbyDetailsHandle;
	/** Local user joining the lobby */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
	/** If true, this lobby will be associated with the user's presence information. A user can only associate one lobby at a time with their presence information.
	* This affects the ability of the Social Overlay to show game related actions to take in the user's social graph.
	*
	* @note The Social Overlay can handle only one of the following three options at a time:
	* * using the bPresenceEnabled flags within the Sessions interface
	* * using the bPresenceEnabled flags within the Lobby interface
	* * using EOS_PresenceModification_SetJoinInfo
	*
	* @see EOS_PresenceModification_SetJoinInfoOptions
	* @see EOS_Lobby_CreateLobbyOptions
	* @see EOS_Lobby_JoinLobbyOptions
	* @see EOS_Sessions_CreateSessionModificationOptions
	* @see EOS_Sessions_JoinSessionOptions
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	bool bPresenceEnabled;
	/**
	* (Optional) Set this value to override the default local options for the RTC Room, if it is enabled for this lobby. Set this to NULL if
	* your application does not use the Lobby RTC Rooms feature, or if you would like to use the default settings. This option is ignored if
	* the specified lobby does not have an RTC Room enabled and will not cause errors.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSLobbyLocalRTCOptions LocalRTCOptions;
	
	/** 
	* This value indicates whether or not the local user allows crossplay interactions. If it is false, the local user 
	* will be treated as allowing crossplay.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	bool bCrossplayOptOut;
public:
	FEOSLobbyJoinLobbyOptions()
		: bPresenceEnabled(false)
		, bCrossplayOptOut(false)
	{
	}
};

/**
 * Output parameters for the EOS_Lobby_JoinLobby function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyJoinLobbyCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Lobby_JoinLobby */
	void* ClientData;
	/** The id of the lobby affected */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
public:
public:
	FEOSLobbyJoinLobbyCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSLobbyJoinLobbyCallbackInfo(const EOS_Lobby_JoinLobbyCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
	{
		const FUTF8ToTCHAR LobbyIdChar(data.LobbyId);
		LobbyId = LobbyIdChar.Get();
	}
};

/**
 * Input parameters for the EOS_Lobby_JoinLobbyById function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyJoinLobbyByIdOptions
{
	GENERATED_BODY()
public:
	/** The ID of the lobby */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
	/** The Product User ID of the local user joining the lobby */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
	/** If true, this lobby will be associated with the user's presence information. A user can only associate one lobby at a time with their presence information.
	 * This affects the ability of the Social Overlay to show game related actions to take in the user's social graph.
	 *
	 * @note The Social Overlay can handle only one of the following three options at a time:
	 * * using the bPresenceEnabled flags within the Sessions interface
	 * * using the bPresenceEnabled flags within the Lobby interface
	 * * using EOS_PresenceModification_SetJoinInfo
	 *
	 * @see EOS_PresenceModification_SetJoinInfoOptions
	 * @see EOS_Lobby_CreateLobbyOptions
	 * @see EOS_Lobby_JoinLobbyOptions
	 * @see EOS_Lobby_JoinLobbyByIdOptions
	 * @see EOS_Sessions_CreateSessionModificationOptions
	 * @see EOS_Sessions_JoinSessionOptions
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	bool bPresenceEnabled;
	/**
	 * (Optional) Set this value to override the default local options for the RTC Room, if it is enabled for this lobby. Set this to NULL if
	 * your application does not use the Lobby RTC Rooms feature, or if you would like to use the default settings. This option is ignored if
	 * the specified lobby does not have an RTC Room enabled and will not cause errors.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSLobbyLocalRTCOptions LocalRTCOptions;
	/** 
	* This value indicates whether or not the local user allows crossplay interactions. If it is false, the local user
	* will be treated as allowing crossplay.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	bool bCrossplayOptOut;
public:
	FEOSLobbyJoinLobbyByIdOptions()
		: bPresenceEnabled(false)
		, bCrossplayOptOut(false)
	{
	} ;
};

/**
 * Output parameters for the EOS_Lobby_JoinLobbyById function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyJoinLobbyByIdCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Lobby_JoinLobbyById */
	void* ClientData;
	/** The id of the lobby affected */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
public:
	FEOSLobbyJoinLobbyByIdCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSLobbyJoinLobbyByIdCallbackInfo(const EOS_Lobby_JoinLobbyByIdCallbackInfo& Data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(Data.ResultCode))
		  , ClientData(Data.ClientData)
	{
		const FUTF8ToTCHAR LobbyIdChar(Data.LobbyId);
		LobbyId = LobbyIdChar.Get();
	}
};

/**
 * Input parameters for the EOS_Lobby_LeaveLobby Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyLeaveLobbyOptions
{
	GENERATED_BODY()
public:
	/** Local user leaving the lobby */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
	/** The id of the lobby affected */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
public:
	FEOSLobbyLeaveLobbyOptions() = default;
};

/**
 * Output parameters for the EOS_Lobby_LeaveLobby function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyLeaveLobbyCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Lobby_LeaveLobby */
	void* ClientData;
	/** The id of the lobby affected */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
public:
	FEOSLobbyLeaveLobbyCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSLobbyLeaveLobbyCallbackInfo(const EOS_Lobby_LeaveLobbyCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
	{
		const FUTF8ToTCHAR LobbyIdChar(data.LobbyId);
		LobbyId = LobbyIdChar.Get();
	}
};

/**
 * Input parameters for the EOS_Lobby_UpdateLobbyModification Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyUpdateLobbyModificationOptions
{
	GENERATED_BODY()
public:
	/** The id of the local user making modifications, must be the owner to modify lobby data, but may be a lobby member to modify their own attributes */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
	/** The id of the lobby affected */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
public:
	FEOSLobbyUpdateLobbyModificationOptions() = default;
};

/**
 * Input parameters for the EOS_Lobby_UpdateLobby Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyUpdateLobbyOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	/** Builder handle */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSHLobbyModification LobbyModificationHandle;
public:
public:
	FEOSLobbyUpdateLobbyOptions()
		: ApiVersion(EOS_LOBBY_UPDATELOBBY_API_LATEST)
	{
	}

	FEOSLobbyUpdateLobbyOptions(const EOS_Lobby_UpdateLobbyOptions& data)
		: ApiVersion(EOS_LOBBY_UPDATELOBBY_API_LATEST)
		  , LobbyModificationHandle(data.LobbyModificationHandle)
	{
	}
};

/**
 * Input parameters for the EOS_Lobby_GetConnectString function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyGetConnectStringOptions
{
	GENERATED_BODY()
public:
	/** Builder handle */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	/** The Product User ID of the local user requesting the connection string generated from the lobby; this user must currently own the lobby */
	FEOSProductUserId LocalUserId;
	/** The ID of the lobby to generate a connection string for */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
public:
	FEOSLobbyGetConnectStringOptions()
	{
	}
};

/**
 * Input parameters for the EOS_Lobby_GetConnectString function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyParseConnectStringOptions
{
	GENERATED_BODY()
public:
	/** The connection string to parse */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString ConnectString;
public:
	FEOSLobbyParseConnectStringOptions()
	{
	}
};

/**
 * Input parameters for the EOS_LobbyDetails_CopyMemberInfo function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyDetailsCopyMemberInfoOptions
{
	GENERATED_BODY()
public:
	/** The Product User ID of the lobby member to copy. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId TargetUserId;
public:
	FEOSLobbyDetailsCopyMemberInfoOptions()
	{
	}
};

/**
 * Input parameters for the EOS_LobbyModification_SetAllowedPlatformIdsOptions function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyModificationSetAllowedPlatformIdsOptions
{
	GENERATED_BODY()
public:
	/** 
	 * Array of platform IDs indicating the player platforms allowed to register with the session. Platform IDs are 
	 * found in the EOS header file, e.g. EOS_OPT_Epic. For some platforms, the value will be in the EOS Platform specific
	 * header file. If null, the lobby will be unrestricted.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	TArray<int32> AllowedPlatformIds;
public:
	FEOSLobbyModificationSetAllowedPlatformIdsOptions()
	{
	}
};

/**
 * Output parameters for the EOS_Lobby_UpdateLobby function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyUpdateLobbyCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Lobby_UpdateLobby */
	void* ClientData;
	/** The id of the lobby affected */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
public:
	FEOSLobbyUpdateLobbyCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSLobbyUpdateLobbyCallbackInfo(const EOS_Lobby_UpdateLobbyCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
	{
		const FUTF8ToTCHAR LobbyIdChar(data.LobbyId);
		LobbyId = LobbyIdChar.Get();
	}
};

/**
 * Input parameters for the EOS_Lobby_PromoteMember Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyPromoteMemberOptions
{
	GENERATED_BODY()
public:
	/** Lobby id of interest */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
	/** Local User making the request */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
	/** Member to promote to owner of the lobby */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId TargetUserId;
public:
	FEOSLobbyPromoteMemberOptions() = default;
};

/**
 * Output parameters for the EOS_Lobby_PromoteMember function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyPromoteMemberCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Lobby_PromoteMember */
	void* ClientData;
	/** Lobby id of interest */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
public:
	FEOSLobbyPromoteMemberCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSLobbyPromoteMemberCallbackInfo(const EOS_Lobby_PromoteMemberCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
	{
		const FUTF8ToTCHAR LobbyIdChar(data.LobbyId);
		LobbyId = LobbyIdChar.Get();
	}
};

/**
 * Output parameters for the EOS_Lobby_PromoteMember function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyDetailsMemberInfo
{
	GENERATED_BODY()
public:
	/** The Product User ID of the lobby member. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId UserId;
	/** The platform of the lobby member. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	int32 Platform;
	/** Does this member allow crossplay */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	bool bAllowsCrossplay;
public:
	FEOSLobbyDetailsMemberInfo()
		: Platform(0)
		, bAllowsCrossplay(false)
	{
	}

	FEOSLobbyDetailsMemberInfo(const EOS_LobbyDetails_MemberInfo& Data)
		: UserId(Data.UserId)
		, Platform(Data.Platform)
		, bAllowsCrossplay(Data.bAllowsCrossplay > 0)
	{
	}
};

/**
 * Input parameters for the EOS_Lobby_KickMember Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyKickMemberOptions
{
	GENERATED_BODY()
public:
	/** Lobby id of interest */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
	/** Local User making the request */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
	/** Member to kick from the lobby */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId TargetUserId;
public:
	FEOSLobbyKickMemberOptions() = default;
};

/**
 * Output parameters for the EOS_Lobby_KickMember function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyKickMemberCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Lobby_KickMember */
	void* ClientData;
	/** Lobby id of interest */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
public:
	FEOSLobbyKickMemberCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSLobbyKickMemberCallbackInfo(const EOS_Lobby_KickMemberCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
	{
		const FUTF8ToTCHAR LobbyIdChar(data.LobbyId);
		LobbyId = LobbyIdChar.Get();
	}
};

/**
 * Output parameters for the EOS_Lobby_OnLeaveLobbyRequestedCallback Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyLeaveLobbyRequestedCallbackInfo
{
	GENERATED_BODY()
public:
	/** The Product User ID of the local user who received the leave lobby notification. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
	/** Lobby ID associated with the leave lobby request. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
public:
	FEOSLobbyLeaveLobbyRequestedCallbackInfo()
	{
	}

	FEOSLobbyLeaveLobbyRequestedCallbackInfo(const EOS_Lobby_LeaveLobbyRequestedCallbackInfo& Data)
		: LocalUserId(Data.LocalUserId)
	{
		const FUTF8ToTCHAR LobbyIdChar(Data.LobbyId);
		LobbyId = LobbyIdChar.Get();
	}
};

USTRUCT(BlueprintType)
struct FEOSLobbyAddNotifyLobbyUpdateReceivedOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	FEOSNotificationId NotificationID;
public:
	FEOSLobbyAddNotifyLobbyUpdateReceivedOptions()
		: ApiVersion(EOS_LOBBY_ADDNOTIFYLOBBYUPDATERECEIVED_API_LATEST)
	{
	}

	FEOSLobbyAddNotifyLobbyUpdateReceivedOptions(const EOS_Lobby_AddNotifyLobbyUpdateReceivedOptions& data)
		: ApiVersion(EOS_LOBBY_ADDNOTIFYLOBBYUPDATERECEIVED_API_LATEST)
	{
	}
};

/**
 * Output parameters for the EOS_Lobby_OnLobbyUpdateReceivedCallback Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyUpdateReceivedCallbackInfo
{
	GENERATED_BODY()
public:
	/** Context that was passed into EOS_Lobby_KickMember */
	void* ClientData;
	/** Lobby id of interest */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
public:
	FEOSLobbyUpdateReceivedCallbackInfo()
		: ClientData(nullptr)
	{
	}

	FEOSLobbyUpdateReceivedCallbackInfo(const EOS_Lobby_LobbyUpdateReceivedCallbackInfo& data)
		: ClientData(data.ClientData)
	{
		const FUTF8ToTCHAR LobbyIdChar(data.LobbyId);
		LobbyId = LobbyIdChar.Get();
	}
};

USTRUCT(BlueprintType)
struct FEOSLobbyAddNotifyLobbyMemberUpdateReceivedOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	FEOSNotificationId NotificationID;
public:
	FEOSLobbyAddNotifyLobbyMemberUpdateReceivedOptions()
		: ApiVersion(EOS_LOBBY_ADDNOTIFYLOBBYMEMBERUPDATERECEIVED_API_LATEST)
	{
	}
};

/**
 * Output parameters for the EOS_Lobby_OnLobbyMemberUpdateReceivedCallback Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyMemberUpdateReceivedCallbackInfo
{
	GENERATED_BODY()
public:
	/** Context that was passed into EOS_Lobby_AddNotifyLobbyMemberUpdateReceived */
	void* ClientData;
	/** The id of the lobby affected */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
	/** Target user that was affected */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId TargetUserId;
public:
	FEOSLobbyMemberUpdateReceivedCallbackInfo()
		: ClientData(nullptr)
	{
	}

	FEOSLobbyMemberUpdateReceivedCallbackInfo(const EOS_Lobby_LobbyMemberUpdateReceivedCallbackInfo& data)
		: ClientData(data.ClientData)
		  , TargetUserId(data.TargetUserId)
	{
		const FUTF8ToTCHAR LobbyIdChar(data.LobbyId);
		LobbyId = LobbyIdChar.Get();
	}
};

/**
 * Input parameters for the EOS_Lobby_AddNotifyLobbyMemberStatusReceived Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyAddNotifyLobbyMemberStatusReceivedOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	FEOSNotificationId NotificationID;
public:
	FEOSLobbyAddNotifyLobbyMemberStatusReceivedOptions()
		: ApiVersion(EOS_LOBBY_ADDNOTIFYLOBBYMEMBERSTATUSRECEIVED_API_LATEST)
	{
	}

	FEOSLobbyAddNotifyLobbyMemberStatusReceivedOptions(const EOS_Lobby_AddNotifyLobbyMemberStatusReceivedOptions& data)
		: ApiVersion(EOS_LOBBY_ADDNOTIFYLOBBYMEMBERSTATUSRECEIVED_API_LATEST)
	{
	}
};

/**
 * Output parameters for the EOS_Lobby_AddNotifyLobbyMemberStatusReceived function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyMemberStatusReceivedCallbackInfo
{
	GENERATED_BODY()
public:
	/** Context that was passed into EOS_Lobby_AddNotifyLobbyMemberStatusReceived */
	void* ClientData;
	/** The id of the lobby affected */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
	/** Target user that was affected */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId TargetUserId;
	/** Latest status of the user */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EEOSELobbyMemberStatus CurrentStatus;
public:
	FEOSLobbyMemberStatusReceivedCallbackInfo()
		: ClientData(nullptr)
		  , CurrentStatus(EEOSELobbyMemberStatus::EOS_LMS_CLOSED)
	{
	}

	FEOSLobbyMemberStatusReceivedCallbackInfo(const EOS_Lobby_LobbyMemberStatusReceivedCallbackInfo& data)
		: ClientData(data.ClientData)
		  , TargetUserId(data.TargetUserId)
		  , CurrentStatus(static_cast<EEOSELobbyMemberStatus>(data.CurrentStatus))
	{
		const FUTF8ToTCHAR LobbyIdChar(data.LobbyId);
		LobbyId = LobbyIdChar.Get();
	}
};

USTRUCT(BlueprintType)
struct FEOSLobbyAddNotifyLobbyInviteReceivedOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	FEOSNotificationId NotificationID;
public:
	FEOSLobbyAddNotifyLobbyInviteReceivedOptions()
		: ApiVersion(EOS_LOBBY_ADDNOTIFYLOBBYINVITERECEIVED_API_LATEST)
	{
	}

	FEOSLobbyAddNotifyLobbyInviteReceivedOptions(const EOS_Lobby_AddNotifyLobbyInviteReceivedOptions& data)
		: ApiVersion(EOS_LOBBY_ADDNOTIFYLOBBYINVITERECEIVED_API_LATEST)
	{
	}
};

/**
 * Output parameters for the EOS_Lobby_OnLobbyInviteReceivedCallback Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyInviteReceivedCallbackInfo
{
	GENERATED_BODY()
public:
	/** Context that was passed into EOS_Lobby_AddNotifyLobbyInviteReceived */
	void* ClientData;
	/** The invite id */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString InviteId;
	/** User that received the invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
	/** Target user that sent the invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId TargetUserId;
public:
	FEOSLobbyInviteReceivedCallbackInfo()
		: ClientData(nullptr)
	{
	}

	FEOSLobbyInviteReceivedCallbackInfo(const EOS_Lobby_LobbyInviteReceivedCallbackInfo& data)
		: ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
		  , TargetUserId(data.TargetUserId)
	{
		const FUTF8ToTCHAR InviteIdChar(data.InviteId);
		InviteId = InviteIdChar.Get();
	}
};

/**
 * Input parameters for the EOS_Lobby_HardMuteMember function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyHardMuteMemberOptions
{
	GENERATED_BODY()
public:
	/** The ID of the lobby */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
	/** The Product User ID of the local user requesting the hard mute; this user must be the lobby owner */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
	/** The Product User ID of the lobby member to hard mute */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId TargetUserId;
	/** TargetUserId hard mute status (mute on or off) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	bool bHardMute;
public:
	FEOSLobbyHardMuteMemberOptions()
		: bHardMute(false)
	{
	} ;
};

/**
 * Output parameters for the EOS_Lobby_HardMuteMember function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyHardMuteMemberCallbackInfo
{
	GENERATED_BODY()
public:
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Lobby_HardMuteMember */
	void* ClientData;
	/** The ID of the lobby */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
	/** The Product User ID of the lobby member whose mute status has been updated */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId TargetUserId;
public:
	FEOSLobbyHardMuteMemberCallbackInfo()
		: ResultCode(EOSResult::MAX)
		  , ClientData(nullptr)
	{
	}

	FEOSLobbyHardMuteMemberCallbackInfo(const EOS_Lobby_HardMuteMemberCallbackInfo& Data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(Data.ResultCode))
		  , ClientData(Data.ClientData)
		  , TargetUserId(Data.TargetUserId)
	{
		const FUTF8ToTCHAR ConvertedLobbyId(Data.LobbyId);
		LobbyId = ConvertedLobbyId.Get();
	}
};

/** The most recent version of the EOS_Lobby_AddNotifyLobbyInviteAccepted API. */
USTRUCT(BlueprintType)
struct FEOSLobbyAddNotifyLobbyInviteAcceptedOptions
{
	GENERATED_BODY()
public:
	FEOSLobbyAddNotifyLobbyInviteAcceptedOptions()
	{
	}
};

/**
 * Output parameters for the EOS_Lobby_OnLobbyInviteAcceptedCallback Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyInviteAcceptedCallbackInfo
{
	GENERATED_BODY()
public:
	/** Context that was passed into EOS_Lobby_AddNotifyLobbyInviteReceived */
	void* ClientData;
	/** The invite id */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString InviteId;
	/** User that received the invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
	/** Target user that sent the invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId TargetUserId;
	/** Lobby ID that the user has been invited to */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
public:
	FEOSLobbyInviteAcceptedCallbackInfo()
		: ClientData(nullptr)
	{
	}

	FEOSLobbyInviteAcceptedCallbackInfo(const EOS_Lobby_LobbyInviteAcceptedCallbackInfo& data)
		: ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
		  , TargetUserId(data.TargetUserId)
	{
		const FUTF8ToTCHAR InviteIdChar(data.InviteId);
		InviteId = InviteIdChar.Get();

		const FUTF8ToTCHAR LobbyIdChar(data.LobbyId);
		LobbyId = LobbyIdChar.Get();
	}
};

USTRUCT(BlueprintType)
struct FEOSLobbyAddNotifyLobbyInviteRejectedOptions
{
	GENERATED_BODY()
public:
	FEOSLobbyAddNotifyLobbyInviteRejectedOptions()
	{
	}
};

/**
 * Output parameters for the EOS_Lobby_OnLobbyInviteRejectedCallback Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyLobbyInviteRejectedCallbackInfo
{
	GENERATED_BODY()
public:
	/** Context that was passed into EOS_Lobby_AddNotifyJoinLobbyAccepted */
	void* ClientData;
	/** The invite ID */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString InviteId;
	/** The Product User ID of the local user who received the invitation */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
	/** The Product User ID of the user who sent the invitation */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId TargetUserId;
	/** Lobby ID that the user has been invited to */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
public:
	FEOSLobbyLobbyInviteRejectedCallbackInfo()
		: ClientData(nullptr)
	{
	}

	FEOSLobbyLobbyInviteRejectedCallbackInfo(const EOS_Lobby_LobbyInviteRejectedCallbackInfo& Data)
		: ClientData(Data.ClientData)
		  , LocalUserId(Data.LocalUserId)
		  , TargetUserId(Data.TargetUserId)
	{
		const FUTF8ToTCHAR InviteIdChar(Data.InviteId);
		InviteId = InviteIdChar.Get();

		const FUTF8ToTCHAR LobbyIdChar(Data.LobbyId);
		LobbyId = LobbyIdChar.Get();
	}
};

/**
* Output parameters for the EOS_Lobby_OnJoinLobbyAcceptedCallback Function.
*/
USTRUCT(BlueprintType)
struct FEOSLobbyJoinLobbyAcceptedCallbackInfo
{
	GENERATED_BODY()
public:
	/** Context that was passed into EOS_Lobby_AddNotifyJoinLobbyAccepted */
	void* ClientData;
	/** The Product User ID of the local user who is joining */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
	/** 
	* The UI Event associated with this Join Game event.
	* This should be used with EOS_Lobby_CopyLobbyDetailsHandleByUiEventId to get a handle to be used
	* when calling EOS_Lobby_JoinLobby.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSUIEventId UiEventId;
public:
	FEOSLobbyJoinLobbyAcceptedCallbackInfo()
		: ClientData(nullptr)
	{
	}

	FEOSLobbyJoinLobbyAcceptedCallbackInfo(const EOS_Lobby_JoinLobbyAcceptedCallbackInfo& data)
		: ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
		  , UiEventId(data.UiEventId)
	{
	}
};

/** The most recent version of the EOS_Lobby_AddNotifyJoinGameAccepted API. */
USTRUCT(BlueprintType)
struct FEOSLobbyAddNotifyJoinLobbyAcceptedOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	FEOSLobbyAddNotifyJoinLobbyAcceptedOptions()
		: ApiVersion(EOS_LOBBY_ADDNOTIFYJOINLOBBYACCEPTED_API_LATEST)
	{
	}
};

/**
 * Input parameters for the EOS_Lobby_CopyLobbyDetailsHandleByInviteId Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyCopyLobbyDetailsHandleByInviteIdOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	/** Lobby invite id */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString InviteId;
public:
	FEOSLobbyCopyLobbyDetailsHandleByInviteIdOptions()
		: ApiVersion(EOS_LOBBY_COPYLOBBYDETAILSHANDLEBYINVITEID_API_LATEST)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSLobbyCopyLobbyDetailsHandleByUiEventIdOptions
{
	GENERATED_BODY()
public:
	/** UI Event associated with the lobby */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSUIEventId UiEventId;
public:
	FEOSLobbyCopyLobbyDetailsHandleByUiEventIdOptions() = default;
};

/**
* Input parameters for the EOS_Lobby_GetRTCRoomName function.
*/
USTRUCT(BlueprintType)
struct FEOSLobbyGetRTCRoomNameOptions
{
	GENERATED_BODY()
public:
	/** The ID of the lobby to get the RTC Room name for */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
	/** The Product User ID of the local user in the lobby */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
public:
	FEOSLobbyGetRTCRoomNameOptions() = default;
};

USTRUCT(BlueprintType)
struct FEOSLobbyIsRTCRoomConnectedOptions
{
	GENERATED_BODY()
public:
	/** The ID of the lobby to get the RTC Room name for */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
	/** The Product User ID of the local user in the lobby */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
public:
	FEOSLobbyIsRTCRoomConnectedOptions() = default;
};

USTRUCT(BlueprintType)
struct FEOSLobbyAddNotifySendLobbyNativeInviteRequestedOptions
{
	GENERATED_BODY()
public:
	FEOSLobbyAddNotifySendLobbyNativeInviteRequestedOptions() = default;
};

/**
 * Output parameters for the EOS_Lobby_OnSendLobbyNativeInviteRequestedCallback Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbySendLobbyNativeInviteRequestedCallbackInfo
{
	GENERATED_BODY()
public:
	/**
	 * Identifies this event which will need to be acknowledged with EOS_UI_AcknowledgeEventId().
	 * @see EOS_UI_AcknowledgeEventId
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSUIEventId UiEventId;
	/** The Product User ID of the local user who is inviting. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
	/**
	 * The Native Platform Account Type. If only a single integrated platform is configured then
	 * this will always reference that platform.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString TargetNativeAccountType;
	/** The Native Platform Account ID of the target user being invited. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString TargetUserNativeAccountId;
	/** Lobby ID that the user is being invited to */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
public:
	FEOSLobbySendLobbyNativeInviteRequestedCallbackInfo()
	{
	}

	FEOSLobbySendLobbyNativeInviteRequestedCallbackInfo(const EOS_Lobby_SendLobbyNativeInviteRequestedCallbackInfo& Data)
		: UiEventId(Data.UiEventId)
		  , LocalUserId(Data.LocalUserId)
	{
		const FUTF8ToTCHAR TargetNativeAccountTypeChar(Data.TargetNativeAccountType);
		TargetNativeAccountType = TargetNativeAccountTypeChar.Get();

		const FUTF8ToTCHAR TargetUserNativeAccountIdChar(Data.TargetUserNativeAccountId);
		TargetUserNativeAccountId = TargetUserNativeAccountIdChar.Get();

		const FUTF8ToTCHAR LobbyIdChar(Data.LobbyId);
		LobbyId = LobbyIdChar.Get();
	}
};

/**
* Input parameters for the EOS_Lobby_AddNotifyRTCRoomConnectionChanged function.
*/
USTRUCT(BlueprintType)
struct FEOSLobbyAddNotifyRTCRoomConnectionChangedOptions
{
	GENERATED_BODY()
public:
	/** The ID of the lobby to receive RTC Room connection change notifications for
	 * This is deprecated and no longer needed. The notification is raised for any LobbyId or LocalUserId. If any filtering is required, the callback struct (EOS_Lobby_RTCRoomConnectionChangedCallbackInfo) has both a LobbyId and LocalUserId field.
	 **/
	FString LobbyId_DEPRECATED;
	/** The Product User ID of the local user in the lobby
	 * This is deprecated and no longer needed. The notification is raised for any LobbyId or LocalUserId. If any filtering is required, the callback struct (EOS_Lobby_RTCRoomConnectionChangedCallbackInfo) has both a LobbyId and LocalUserId field.
	 **/
	FEOSProductUserId LocalUserId_DEPRECATED;
public:
	FEOSLobbyAddNotifyRTCRoomConnectionChangedOptions() = default;
};

USTRUCT(BlueprintType)
struct FEOSLobbyRTCRoomConnectionChangedCallbackInfo
{
	GENERATED_BODY()
public:
	/** Context that was passed into EOS_Lobby_AddNotifyRTCRoomConnectionChanged */
	void* ClientData;
	/** The ID of the lobby which had a RTC Room connection state change */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
	/** The Product User ID of the local user who is in the lobby and registered for notifications */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
	/** The new connection state of the room */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	bool bIsConnected;
	/**
	If bIsConnected is EOS_FALSE, this result will be the reason we were disconnected.
	* EOS_Success: The room was left locally. This may be because: the associated lobby was Left or Destroyed, the connection to the lobby was interrupted, or because the SDK is being shutdown. If the lobby connection returns (lobby did not permanently go away), we will reconnect.
	* EOS_NoConnection: There was a network issue connecting to the server. We will attempt to reconnect soon.
	* EOS_RTC_UserKicked: The user has been kicked by the server. We will not reconnect.
	* EOS_RTC_UserBanned: The user has been banned by the server. We will not reconnect.
	* EOS_ServiceFailure: A known error occurred during interaction with the server. We will attempt to reconnect soon.
	* EOS_UnexpectedError: Unexpected error. We will attempt to reconnect soon.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EOSResult DisconnectReason;
public:
	FEOSLobbyRTCRoomConnectionChangedCallbackInfo()
		: ClientData(nullptr)
		  , bIsConnected(false)
		  , DisconnectReason(EOSResult::MAX)
	{
	}

	FEOSLobbyRTCRoomConnectionChangedCallbackInfo(const EOS_Lobby_RTCRoomConnectionChangedCallbackInfo& data)
		: ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
		  , bIsConnected(data.bIsConnected > 0)
		  , DisconnectReason(EOSHelpers::ToEOSCoreResult(data.DisconnectReason))
	{
		const FUTF8ToTCHAR LobbyIdChar(data.LobbyId);
		LobbyId = LobbyIdChar.Get();
	}
};


/**
 * Input parameters for the EOS_Lobby_CreateLobbySearch Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyCreateLobbySearchOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	/** Maximum number of results allowed from the search */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	int32 MaxResults;
public:
	explicit FEOSLobbyCreateLobbySearchOptions()
		: ApiVersion(EOS_LOBBY_CREATELOBBYSEARCH_API_LATEST)
		  , MaxResults(50)
	{
	}

	FEOSLobbyCreateLobbySearchOptions(const EOS_Lobby_CreateLobbySearchOptions& data)
		: ApiVersion(EOS_LOBBY_CREATELOBBYSEARCH_API_LATEST)
		  , MaxResults(data.MaxResults)
	{
	}
};

/**
 * Input parameters for the EOS_Lobby_SendInvite Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbySendInviteOptions
{
	GENERATED_BODY()
public:
	/** The id of the lobby affected */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
	/** Local user sending the invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
	/** Target user receiving the invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId TargetUserId;
public:
	FEOSLobbySendInviteOptions() = default;
};

/**
 * Output parameters for the EOS_Lobby_SendInvite function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbySendInviteCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Lobby_SendInvite */
	void* ClientData;
	/** The id of the lobby affected */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
public:
	FEOSLobbySendInviteCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSLobbySendInviteCallbackInfo(const EOS_Lobby_SendInviteCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
	{
		const FUTF8ToTCHAR LobbyIdChar(data.LobbyId);
		LobbyId = LobbyIdChar.Get();
	}
};

/**
 * Input parameters for the EOS_Lobby_RejectInvite Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyRejectInviteOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	/** The invite id to reject */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString InviteId;
	/** Local user rejecting the invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
public:
	FEOSLobbyRejectInviteOptions()
		: ApiVersion(EOS_LOBBY_REJECTINVITE_API_LATEST)
	{
	}
};

/**
 * Output parameters for the EOS_Lobby_RejectInvite function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyRejectInviteCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Lobby_RejectInvite */
	void* ClientData;
	/** The invite id to reject */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString InviteId;
public:
	FEOSLobbyRejectInviteCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSLobbyRejectInviteCallbackInfo(const EOS_Lobby_RejectInviteCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
	{
		const FUTF8ToTCHAR InviteIdChar(data.InviteId);
		InviteId = InviteIdChar.Get();
	}
};

/**
 * Input parameters for the EOS_Lobby_QueryInvites Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyQueryInvitesOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	/** Local User Id to query invites */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
public:
	FEOSLobbyQueryInvitesOptions()
		: ApiVersion(EOS_LOBBY_QUERYINVITES_API_LATEST)
	{
	}

	FEOSLobbyQueryInvitesOptions(const EOS_Lobby_QueryInvitesOptions& data)
		: ApiVersion(EOS_LOBBY_QUERYINVITES_API_LATEST)
		  , LocalUserId(data.LocalUserId)
	{
	}
};

/**
 * Output parameters for the EOS_Lobby_QueryInvites function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyQueryInvitesCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Lobby_QueryInvites */
	void* ClientData;
	/** Local User Id that made the request */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
public:
	FEOSLobbyQueryInvitesCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSLobbyQueryInvitesCallbackInfo(const EOS_Lobby_QueryInvitesCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
	{
	}
};

/**
 * Input parameters for the EOS_Lobby_GetInviteCount Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyGetInviteCountOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	/** Local user that has invites */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
public:
	FEOSLobbyGetInviteCountOptions()
		: ApiVersion(EOS_LOBBY_GETINVITECOUNT_API_LATEST)
	{
	}

	FEOSLobbyGetInviteCountOptions(const EOS_Lobby_GetInviteCountOptions& data)
		: ApiVersion(EOS_LOBBY_GETINVITECOUNT_API_LATEST)
		  , LocalUserId(data.LocalUserId)
	{
	}
};

/**
 * Input parameters for the EOS_Lobby_GetInviteIdByIndex Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyGetInviteIdByIndexOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	/** Local user that has invites */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
	/** Index of the invite id to retrieve */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	int32 Index;
public:
	explicit FEOSLobbyGetInviteIdByIndexOptions()
		: ApiVersion(EOS_LOBBY_GETINVITEIDBYINDEX_API_LATEST)
		  , Index(0)
	{
	}

	FEOSLobbyGetInviteIdByIndexOptions(const EOS_Lobby_GetInviteIdByIndexOptions& data)
		: ApiVersion(EOS_LOBBY_GETINVITEIDBYINDEX_API_LATEST)
		  , LocalUserId(data.LocalUserId)
		  , Index(data.Index)
	{
	}
};

/**
 * Input parameters for the EOS_Lobby_CopyLobbyDetailsHandle Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyCopyLobbyDetailsHandleOptions
{
	GENERATED_BODY()
public:
	/** The id of the lobby affected */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
	/** Local user making the request */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
public:
	FEOSLobbyCopyLobbyDetailsHandleOptions() = default;
};

/**
* Input parameters for the EOS_LobbyModification_SetBucketIdOptions function.
*/
USTRUCT(BlueprintType)
struct FEOSLobbyModificationSetBucketIdOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	/** The new bucket id associated with the lobby */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString BucketId;
public:
	FEOSLobbyModificationSetBucketIdOptions()
		: ApiVersion(EOS_LOBBYMODIFICATION_SETBUCKETID_API_LATEST)
	{
	}

	FEOSLobbyModificationSetBucketIdOptions(const EOS_LobbyModification_SetBucketIdOptions& data)
		: ApiVersion(EOS_LOBBYMODIFICATION_SETBUCKETID_API_LATEST)
	{
		const FUTF8ToTCHAR BucketIdChar(data.BucketId);
		BucketId = BucketIdChar.Get();
	}
};

/**
 * Contains information about lobby and lobby member data
 */
USTRUCT(BlueprintType)
struct FEOSLobbyAttributeData
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
	/** Contains information about lobby and lobby member data */
	EOS_Lobby_AttributeData AttributeData;
public:
	FEOSLobbyAttributeData()
		: ApiVersion(EOS_LOBBY_ATTRIBUTEDATA_API_LATEST)
	{
	}

	FEOSLobbyAttributeData(const EOS_Lobby_AttributeData& data)
		: ApiVersion(EOS_LOBBY_ATTRIBUTEDATA_API_LATEST)
		  , AttributeData(data)
	{
	}
};

/**
 *  An attribute and its visibility setting stored with a lobby.
 *  Used to store both lobby and lobby member data
 */
USTRUCT(BlueprintType)
struct FEOSLobbyAttribute
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	/** Key/Value pair describing the attribute */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSLobbyAttributeData Data;
	/** Is this attribute public or private to the lobby and its members */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EEOSELobbyAttributeVisibility Visbility;
public:
	explicit FEOSLobbyAttribute()
		: ApiVersion(EOS_LOBBY_ATTRIBUTE_API_LATEST)
		  , Visbility(EEOSELobbyAttributeVisibility::EOS_LAT_PUBLIC)
	{
	}

	FEOSLobbyAttribute(const EOS_Lobby_Attribute& data)
		: ApiVersion(EOS_LOBBY_ATTRIBUTE_API_LATEST)
		  , Data(*data.Data)
		  , Visbility(static_cast<EEOSELobbyAttributeVisibility>(data.Visibility))
	{
	}
};

/**
 * Input parameters for the EOS_LobbyModification_SetPermissionLevel Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyModificationSetPermissionLevelOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	/** Permission level of the lobby */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EEOSELobbyPermissionLevel PermissionLevel;
public:
	explicit FEOSLobbyModificationSetPermissionLevelOptions()
		: ApiVersion(EOS_LOBBYMODIFICATION_SETPERMISSIONLEVEL_API_LATEST)
		  , PermissionLevel(EEOSELobbyPermissionLevel::EOS_LPL_PUBLICADVERTISED)
	{
	}

	FEOSLobbyModificationSetPermissionLevelOptions(const EOS_LobbyModification_SetPermissionLevelOptions& data)
		: ApiVersion(EOS_LOBBYMODIFICATION_SETPERMISSIONLEVEL_API_LATEST)
		  , PermissionLevel(static_cast<EEOSELobbyPermissionLevel>(data.PermissionLevel))
	{
	}
};

/**
 * Input parameters for the EOS_LobbyModification_SetMaxMembers Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyModificationSetMaxMembersOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	/** New maximum number of lobby members */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	int32 MaxMembers;
public:
	explicit FEOSLobbyModificationSetMaxMembersOptions()
		: ApiVersion(EOS_LOBBYMODIFICATION_SETMAXMEMBERS_API_LATEST)
		  , MaxMembers(10)
	{
	}

	FEOSLobbyModificationSetMaxMembersOptions(const EOS_LobbyModification_SetMaxMembersOptions& data)
		: ApiVersion(EOS_LOBBYMODIFICATION_SETMAXMEMBERS_API_LATEST)
		  , MaxMembers(data.MaxMembers)
	{
	}
};

/**
* Input parameters for the EOS_LobbyModification_SetInvitesAllowed Function.
*/
USTRUCT(BlueprintType)
struct FEOSLobbyModificationSetInvitesAllowedOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	/** If true then invites can currently be sent for the associated lobby */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	bool bInvitesAllowed;
public:
	explicit FEOSLobbyModificationSetInvitesAllowedOptions()
		: ApiVersion(EOS_LOBBYMODIFICATION_SETINVITESALLOWED_API_LATEST)
		  , bInvitesAllowed(true)
	{
	}

	FEOSLobbyModificationSetInvitesAllowedOptions(const EOS_LobbyModification_SetInvitesAllowedOptions& data)
		: ApiVersion(EOS_LOBBYMODIFICATION_SETINVITESALLOWED_API_LATEST)
		  , bInvitesAllowed(data.bInvitesAllowed > 0)
	{
	}
};

/**
 * Input parameters for the EOS_LobbyModification_AddAttribute Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyModificationAddAttributeOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	/** Key/Value pair describing the attribute to add to the lobby */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSLobbyAttributeData Attribute;
	/** Is this attribute public or private to the lobby and its members */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EEOSELobbyAttributeVisibility Visibility;
public:
	explicit FEOSLobbyModificationAddAttributeOptions()
		: ApiVersion(EOS_LOBBYMODIFICATION_ADDATTRIBUTE_API_LATEST)
		  , Visibility(EEOSELobbyAttributeVisibility::EOS_LAT_PUBLIC)
	{
	}

	FEOSLobbyModificationAddAttributeOptions(const EOS_LobbyModification_AddAttributeOptions& data)
		: ApiVersion(EOS_LOBBYMODIFICATION_ADDATTRIBUTE_API_LATEST)
		  , Attribute(*data.Attribute)
		  , Visibility(static_cast<EEOSELobbyAttributeVisibility>(data.Visibility))
	{
	}
};

/**
 * Input parameters for the EOS_LobbyModification_RemoveAttribute Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyModificationRemoveAttributeOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	/** Name of the key */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString Key;
public:
	FEOSLobbyModificationRemoveAttributeOptions()
		: ApiVersion(EOS_LOBBYMODIFICATION_REMOVEATTRIBUTE_API_LATEST)
	{
	}

	FEOSLobbyModificationRemoveAttributeOptions(const EOS_LobbyModification_RemoveAttributeOptions& data)
		: ApiVersion(EOS_LOBBYMODIFICATION_REMOVEATTRIBUTE_API_LATEST)
	{
		const FUTF8ToTCHAR KeyChar(data.Key);
		Key = KeyChar.Get();
	}
};

/**
 * Input parameters for the EOS_LobbyModification_AddMemberAttribute Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyModificationAddMemberAttributeOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	/** Key/Value pair describing the attribute to add to the lobby member */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSLobbyAttributeData Attribute;
	/** Is this attribute public or private to the rest of the lobby members */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EEOSELobbyAttributeVisibility Visibility;
public:
	explicit FEOSLobbyModificationAddMemberAttributeOptions()
		: ApiVersion(EOS_LOBBYMODIFICATION_ADDMEMBERATTRIBUTE_API_LATEST)
		  , Visibility(EEOSELobbyAttributeVisibility::EOS_LAT_PUBLIC)
	{
	}

	FEOSLobbyModificationAddMemberAttributeOptions(const EOS_LobbyModification_AddMemberAttributeOptions& data)
		: ApiVersion(EOS_LOBBYMODIFICATION_ADDMEMBERATTRIBUTE_API_LATEST)
		  , Attribute(*data.Attribute)
		  , Visibility(static_cast<EEOSELobbyAttributeVisibility>(data.Visibility))
	{
	}
};

/**
 * Input parameters for the EOS_LobbyModification_RemoveMemberAttribute Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyModificationRemoveMemberAttributeOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	/** Name of the key */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString Key;
public:
	FEOSLobbyModificationRemoveMemberAttributeOptions()
		: ApiVersion(EOS_LOBBYMODIFICATION_REMOVEMEMBERATTRIBUTE_API_LATEST)
	{
	}

	FEOSLobbyModificationRemoveMemberAttributeOptions(const EOS_LobbyModification_RemoveMemberAttributeOptions& data)
		: ApiVersion(EOS_LOBBYMODIFICATION_REMOVEMEMBERATTRIBUTE_API_LATEST)
	{
		const FUTF8ToTCHAR KeyChar(data.Key);
		Key = KeyChar.Get();
	}
};

/**
 * Input parameters for the EOS_LobbyDetails_GetLobbyOwner Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyDetailsGetLobbyOwnerOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	FEOSLobbyDetailsGetLobbyOwnerOptions()
		: ApiVersion(EOS_LOBBYDETAILS_GETLOBBYOWNER_API_LATEST)
	{
	}

	FEOSLobbyDetailsGetLobbyOwnerOptions(const EOS_LobbyDetails_GetLobbyOwnerOptions& data)
		: ApiVersion(EOS_LOBBYDETAILS_GETLOBBYOWNER_API_LATEST)
	{
	}
};

/**
 * Input parameters for the EOS_LobbyDetails_CopyInfo Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyDetailsCopyInfoOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	FEOSLobbyDetailsCopyInfoOptions()
		: ApiVersion(EOS_LOBBYDETAILS_COPYINFO_API_LATEST)
	{
	}

	FEOSLobbyDetailsCopyInfoOptions(const EOS_LobbyDetails_CopyInfoOptions& data)
		: ApiVersion(EOS_LOBBYDETAILS_COPYINFO_API_LATEST)
	{
	}
};

/**
 * Input parameters for the EOS_LobbyDetails_GetAttributeCount Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyDetailsGetAttributeCountOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	FEOSLobbyDetailsGetAttributeCountOptions()
		: ApiVersion(EOS_LOBBYDETAILS_GETATTRIBUTECOUNT_API_LATEST)
	{
	}

	FEOSLobbyDetailsGetAttributeCountOptions(const EOS_LobbyDetails_GetAttributeCountOptions& data)
		: ApiVersion(EOS_LOBBYDETAILS_GETATTRIBUTECOUNT_API_LATEST)
	{
	}
};

/**
 * Input parameters for the EOS_LobbyDetails_CopyAttributeByIndex Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyDetailsCopyAttributeByIndexOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	/**
	* The index of the attribute to retrieve
	* @see EOS_LobbyDetails_GetAttributeCount
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	int32 AttrIndex;
public:
	explicit FEOSLobbyDetailsCopyAttributeByIndexOptions()
		: ApiVersion(EOS_LOBBYDETAILS_COPYATTRIBUTEBYINDEX_API_LATEST)
		  , AttrIndex(0)
	{
	}

	FEOSLobbyDetailsCopyAttributeByIndexOptions(const EOS_LobbyDetails_CopyAttributeByIndexOptions& data)
		: ApiVersion(EOS_LOBBYDETAILS_COPYATTRIBUTEBYINDEX_API_LATEST)
		  , AttrIndex(data.AttrIndex)
	{
	}
};

/**
 * Input parameters for the EOS_LobbyDetails_CopyAttributeByKey Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyDetailsCopyAttributeByKeyOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	/** Name of the attribute */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString AttrKey;
public:
	FEOSLobbyDetailsCopyAttributeByKeyOptions()
		: ApiVersion(EOS_LOBBYDETAILS_COPYATTRIBUTEBYINDEX_API_LATEST)
	{
	}

	FEOSLobbyDetailsCopyAttributeByKeyOptions(const EOS_LobbyDetails_CopyAttributeByKeyOptions& data)
		: ApiVersion(EOS_LOBBYDETAILS_COPYATTRIBUTEBYINDEX_API_LATEST)
	{
		const FUTF8ToTCHAR AttrKeyChar(data.AttrKey);
		AttrKey = AttrKeyChar.Get();
	}
};

/**
 * Input parameters for the EOS_LobbyDetails_GetMemberAttributeCount Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyDetailsGetMemberAttributeCountOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	/** Lobby member of interest */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId TargetUserId;
public:
	FEOSLobbyDetailsGetMemberAttributeCountOptions()
		: ApiVersion(EOS_LOBBYDETAILS_GETMEMBERATTRIBUTECOUNT_API_LATEST)
	{
	}

	FEOSLobbyDetailsGetMemberAttributeCountOptions(const EOS_LobbyDetails_GetMemberAttributeCountOptions& data)
		: ApiVersion(EOS_LOBBYDETAILS_GETMEMBERATTRIBUTECOUNT_API_LATEST)
		  , TargetUserId(data.TargetUserId)
	{
	}
};

/**
 * Input parameters for the EOS_LobbyDetails_CopyMemberAttributeByIndex Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyDetailsCopyMemberAttributeByIndexOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	/** Lobby member of interest */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId TargetUserId;
	/** Attribute index */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	int32 AttrIndex;
public:
	explicit FEOSLobbyDetailsCopyMemberAttributeByIndexOptions()
		: ApiVersion(EOS_LOBBYDETAILS_COPYMEMBERATTRIBUTEBYINDEX_API_LATEST)
		  , AttrIndex(0)
	{
	}

	FEOSLobbyDetailsCopyMemberAttributeByIndexOptions(const EOS_LobbyDetails_CopyMemberAttributeByIndexOptions& data)
		: ApiVersion(EOS_LOBBYDETAILS_COPYMEMBERATTRIBUTEBYINDEX_API_LATEST)
		  , TargetUserId(data.TargetUserId)
		  , AttrIndex(data.AttrIndex)
	{
	}
};

/**
 * Input parameters for the EOS_LobbyDetails_CopyMemberAttributeByKey Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyDetailsCopyMemberAttributeByKeyOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	/** Lobby member of interest */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId TargetUserId;
	/** Name of the attribute */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString AttrKey;
public:
	FEOSLobbyDetailsCopyMemberAttributeByKeyOptions()
		: ApiVersion(EOS_LOBBYDETAILS_COPYMEMBERATTRIBUTEBYKEY_API_LATEST)
	{
	}

	FEOSLobbyDetailsCopyMemberAttributeByKeyOptions(const EOS_LobbyDetails_CopyMemberAttributeByKeyOptions& data)
		: ApiVersion(EOS_LOBBYDETAILS_COPYMEMBERATTRIBUTEBYKEY_API_LATEST)
		  , TargetUserId(data.TargetUserId)
	{
		const FUTF8ToTCHAR AttrKeyChar(data.AttrKey);
		AttrKey = AttrKeyChar.Get();
	}
};

/**
 * Input parameters for the EOS_LobbyDetails_GetMemberCount Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyDetailsGetMemberCountOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	FEOSLobbyDetailsGetMemberCountOptions()
		: ApiVersion(EOS_LOBBYDETAILS_GETMEMBERCOUNT_API_LATEST)
	{
	}

	FEOSLobbyDetailsGetMemberCountOptions(const EOS_LobbyDetails_GetMemberCountOptions& data)
		: ApiVersion(EOS_LOBBYDETAILS_GETMEMBERCOUNT_API_LATEST)
	{
	}
};

/**
 * Input parameters for the EOS_LobbyDetails_GetMemberByIndex Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbyDetailsGetMemberByIndexOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	/** Index of the member to retrieve */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	int32 MemberIndex;
public:
	explicit FEOSLobbyDetailsGetMemberByIndexOptions()
		: ApiVersion(EOS_LOBBYDETAILS_GETMEMBERBYINDEX_API_LATEST)
		  , MemberIndex(0)
	{
	}

	FEOSLobbyDetailsGetMemberByIndexOptions(const EOS_LobbyDetails_GetMemberByIndexOptions& data)
		: ApiVersion(EOS_LOBBYDETAILS_GETMEMBERBYINDEX_API_LATEST)
		  , MemberIndex(data.MemberIndex)
	{
	}
};

/**
 * Input parameters for the EOS_LobbySearch_Find Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbySearchFindOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	/** User making the search request */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
public:
	FEOSLobbySearchFindOptions()
		: ApiVersion(EOS_LOBBYSEARCH_FIND_API_LATEST)
	{
	}

	FEOSLobbySearchFindOptions(const EOS_LobbySearch_FindOptions& data)
		: ApiVersion(EOS_LOBBYSEARCH_FIND_API_LATEST)
		  , LocalUserId(data.LocalUserId)
	{
	}
};

/**
 * Output parameters for the EOS_LobbySearch_Find function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbySearchFindCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EOSResult ResultCode;
	/** Context that was passed into EOS_LobbySearch_Find */
	void* ClientData;
public:
	FEOSLobbySearchFindCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSLobbySearchFindCallbackInfo(const EOS_LobbySearch_FindCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
	{
	}
};

/**
 * Input parameters for the EOS_LobbySearch_SetLobbyId Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbySearchSetLobbyIdOptions
{
	GENERATED_BODY()
public:
	/** The id of the lobby to set */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString LobbyId;
public:
	FEOSLobbySearchSetLobbyIdOptions() = default;
};

/**
 * Input parameters for the EOS_LobbySearch_SetTargetUserId Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbySearchSetTargetUserIdOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	/** Search lobbies for given user, returning any lobbies where this user is currently registered */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId TargetUserId;
public:
	FEOSLobbySearchSetTargetUserIdOptions()
		: ApiVersion(EOS_LOBBYSEARCH_SETTARGETUSERID_API_LATEST)
	{
	}

	FEOSLobbySearchSetTargetUserIdOptions(const EOS_LobbySearch_SetTargetUserIdOptions& data)
		: ApiVersion(EOS_LOBBYSEARCH_SETTARGETUSERID_API_LATEST)
		  , TargetUserId(data.TargetUserId)
	{
	}
};

/**
 * Input parameters for the EOS_LobbySearch_SetParameter Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbySearchSetParameterOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	/** Search parameter describing a key and a value to compare */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSLobbyAttributeData Parameter;
	/** The type of comparison to make against the search parameter */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EEOSEComparisonOp ComparisonOp;
public:
	explicit FEOSLobbySearchSetParameterOptions()
		: ApiVersion(EOS_LOBBYSEARCH_SETPARAMETER_API_LATEST)
		  , ComparisonOp(EEOSEComparisonOp::EOS_CO_EQUAL)
	{
	}

	FEOSLobbySearchSetParameterOptions(const EOS_LobbySearch_SetParameterOptions& data)
		: ApiVersion(EOS_LOBBYSEARCH_SETPARAMETER_API_LATEST)
		  , Parameter(*data.Parameter)
		  , ComparisonOp(static_cast<EEOSEComparisonOp>(data.ComparisonOp))
	{
	}
};

/**
 * Input parameters for the EOS_LobbySearch_RemoveParameter Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbySearchRemoveParameterOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	/** Search parameter key to remove from the search */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString Key;
	/** Search comparison operation associated with the key to remove */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EEOSEComparisonOp ComparisonOp;
public:
	FEOSLobbySearchRemoveParameterOptions()
		: ApiVersion(EOS_LOBBYSEARCH_REMOVEPARAMETER_API_LATEST)
		  , ComparisonOp(EEOSEComparisonOp::EOS_CO_EQUAL)
	{
	}

	FEOSLobbySearchRemoveParameterOptions(const EOS_LobbySearch_RemoveParameterOptions& data)
		: ApiVersion(EOS_LOBBYSEARCH_REMOVEPARAMETER_API_LATEST)
		  , ComparisonOp(static_cast<EEOSEComparisonOp>(data.ComparisonOp))
	{
		const FUTF8ToTCHAR KeyChar(data.Key);
		Key = KeyChar.Get();
	}
};

/**
 * Input parameters for the EOS_LobbySearch_SetMaxResults Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbySearchSetMaxResultsOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	/** Maximum number of search results to return from the query */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	int32 MaxResults;
public:
	FEOSLobbySearchSetMaxResultsOptions()
		: ApiVersion(EOS_LOBBYSEARCH_SETMAXRESULTS_API_LATEST)
		  , MaxResults(50)
	{
	}

	FEOSLobbySearchSetMaxResultsOptions(const EOS_LobbySearch_SetMaxResultsOptions& data)
		: ApiVersion(EOS_LOBBYSEARCH_SETMAXRESULTS_API_LATEST)
		  , MaxResults(data.MaxResults)
	{
	}
};

/**
 * Input parameters for the EOS_LobbySearch_GetSearchResultCount Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbySearchGetSearchResultCountOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	FEOSLobbySearchGetSearchResultCountOptions()
		: ApiVersion(EOS_LOBBYSEARCH_GETSEARCHRESULTCOUNT_API_LATEST)
	{
	}

	FEOSLobbySearchGetSearchResultCountOptions(const EOS_LobbySearch_GetSearchResultCountOptions& data)
		: ApiVersion(EOS_LOBBYSEARCH_GETSEARCHRESULTCOUNT_API_LATEST)
	{
	}
};

/**
 * Input parameters for the EOS_LobbySearch_CopySearchResultByIndex Function.
 */
USTRUCT(BlueprintType)
struct FEOSLobbySearchCopySearchResultByIndexOptions
{
	GENERATED_BODY()
public:
	/** API Version */
	int32 ApiVersion;
public:
	/**
	* The index of the lobby to retrieve within the completed search query
	 * @see EOS_LobbySearch_GetSearchResultCount
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	int32 LobbyIndex;
public:
	explicit FEOSLobbySearchCopySearchResultByIndexOptions()
		: ApiVersion(EOS_LOBBYSEARCH_COPYSEARCHRESULTBYINDEX_API_LATEST)
		  , LobbyIndex(0)
	{
	}

	FEOSLobbySearchCopySearchResultByIndexOptions(const EOS_LobbySearch_CopySearchResultByIndexOptions& data)
		: ApiVersion(EOS_LOBBYSEARCH_COPYSEARCHRESULTBYINDEX_API_LATEST)
		  , LobbyIndex(data.LobbyIndex)
	{
	}
};

/** Handle to the calls responsible for creating a search object */
USTRUCT(BlueprintType)
struct FEOSHLobbySearch
{
	GENERATED_BODY()
public:
	EOS_HLobbySearch Handle;
public:
	FEOSHLobbySearch()
		: Handle(nullptr)
	{
	}

	FEOSHLobbySearch(const EOS_HLobbySearch& data)
		: Handle(data)
	{
	}

public:
	operator EOS_HLobbySearch() const { return Handle; }
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		DELEGATES
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLobbyCreateLobbyCallback, const FEOSLobbyCreateLobbyCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLobbyDestroyLobbyCallback, const FEOSLobbyDestroyLobbyCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLobbyJoinLobbyCallback, const FEOSLobbyJoinLobbyCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLobbyJoinLobbyByIdCallback, const FEOSLobbyJoinLobbyByIdCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLobbyLeaveLobbyCallback, const FEOSLobbyLeaveLobbyCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLobbyUpdateLobbyCallback, const FEOSLobbyUpdateLobbyCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLobbyPromoteMemberCallback, const FEOSLobbyPromoteMemberCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLobbyKickMemberCallback, const FEOSLobbyKickMemberCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLobbyUpdateReceivedCallback, const FEOSLobbyUpdateReceivedCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLobbyMemberUpdateReceivedCallback, const FEOSLobbyMemberUpdateReceivedCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLobbyMemberStatusReceivedCallback, const FEOSLobbyMemberStatusReceivedCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLobbyInviteReceivedCallback, const FEOSLobbyInviteReceivedCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLobbySendInviteCallback, const FEOSLobbySendInviteCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLobbyRejectInviteCallback, const FEOSLobbyRejectInviteCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLobbyQueryInvitesCallback, const FEOSLobbyQueryInvitesCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLobbyFindCallback, const FEOSLobbySearchFindCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLobbyInviteAcceptedCallback, const FEOSLobbyInviteAcceptedCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnJoinLobbyAcceptedCallback, const FEOSLobbyJoinLobbyAcceptedCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnRTCRoomConnectionChangedCallback, const FEOSLobbyRTCRoomConnectionChangedCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnJoinLobbyByIdCallback, const FEOSLobbyJoinLobbyByIdCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnHardMuteMemberCallback, const FEOSLobbyHardMuteMemberCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLobbyInviteRejectedCallback, const FEOSLobbyLobbyInviteRejectedCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSendLobbyNativeInviteRequestedCallback, const FEOSLobbySendLobbyNativeInviteRequestedCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLeaveLobbyRequestedCallback, const FEOSLobbyLeaveLobbyRequestedCallbackInfo&, Data);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyCreateLobbyCallbackDelegate, const FEOSLobbyCreateLobbyCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyDestroyLobbyCallbackDelegate, const FEOSLobbyDestroyLobbyCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyJoinLobbyCallbackDelegate, const FEOSLobbyJoinLobbyCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyJoinLobbyByIdCallbackDelegate, const FEOSLobbyJoinLobbyByIdCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyLeaveLobbyCallbackDelegate, const FEOSLobbyLeaveLobbyCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyUpdateLobbyCallbackDelegate, const FEOSLobbyUpdateLobbyCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyPromoteMemberCallbackDelegate, const FEOSLobbyPromoteMemberCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyKickMemberCallbackDelegate, const FEOSLobbyKickMemberCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyUpdateReceivedCallbackDelegate, const FEOSLobbyUpdateReceivedCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyMemberUpdateReceivedCallbackDelegate, const FEOSLobbyMemberUpdateReceivedCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyMemberStatusReceivedCallbackDelegate, const FEOSLobbyMemberStatusReceivedCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyInviteReceivedCallbackDelegate, const FEOSLobbyInviteReceivedCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbySendInviteCallbackDelegate, const FEOSLobbySendInviteCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyRejectInviteCallbacDelegatek, const FEOSLobbyRejectInviteCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyQueryInvitesCallbackDelegate, const FEOSLobbyQueryInvitesCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyFindCallbackDelegate, const FEOSLobbySearchFindCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyInviteAcceptedCallbackDelegate, const FEOSLobbyInviteAcceptedCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRTCRoomConnectionChangedCallbackDelegate, const FEOSLobbyRTCRoomConnectionChangedCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJoinLobbyByIdCallbackDelegate, const FEOSLobbyJoinLobbyByIdCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHardMuteMemberCallbackDelegate, const FEOSLobbyHardMuteMemberCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyInviteRejectedCallbackDelegate, const FEOSLobbyLobbyInviteRejectedCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSendLobbyNativeInviteRequestedCallbackDelegate, const FEOSLobbySendLobbyNativeInviteRequestedCallbackInfo&, Data);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		CALLBACK OBJECTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

struct FCreateLobbyCallback
{
public:
	FOnLobbyCreateLobbyCallback m_Callback;
public:
	FCreateLobbyCallback(const FOnLobbyCreateLobbyCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FCreateLobbyCallback()
	{
		m_Callback.Unbind();
	}
};

struct FDestroyLobbyCallback
{
public:
	FOnLobbyDestroyLobbyCallback m_Callback;
public:
	FDestroyLobbyCallback(const FOnLobbyDestroyLobbyCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FDestroyLobbyCallback()
	{
		m_Callback.Unbind();
	}
};

struct FJoinLobbyCallback
{
public:
	FOnLobbyJoinLobbyCallback m_Callback;
public:
	FJoinLobbyCallback(const FOnLobbyJoinLobbyCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FJoinLobbyCallback()
	{
		m_Callback.Unbind();
	}
};

struct FJoinLobbyByIdCallback
{
public:
	FOnJoinLobbyByIdCallback m_Callback;
public:
	FJoinLobbyByIdCallback(const FOnJoinLobbyByIdCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FJoinLobbyByIdCallback()
	{
		m_Callback.Unbind();
	}
};

struct FHardMuteMemberCallback
{
public:
	FOnHardMuteMemberCallback m_Callback;
public:
	FHardMuteMemberCallback(const FOnHardMuteMemberCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FHardMuteMemberCallback()
	{
		m_Callback.Unbind();
	}
};

struct FLeaveLobbyCallback
{
public:
	FOnLobbyLeaveLobbyCallback m_Callback;
public:
	FLeaveLobbyCallback(const FOnLobbyLeaveLobbyCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FLeaveLobbyCallback()
	{
		m_Callback.Unbind();
	}
};

struct FUpdateLobbyCallback
{
public:
	FOnLobbyUpdateLobbyCallback m_Callback;
public:
	FUpdateLobbyCallback(const FOnLobbyUpdateLobbyCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FUpdateLobbyCallback()
	{
		m_Callback.Unbind();
	}
};

struct FLobbyPromoteMemberCallback
{
public:
	FOnLobbyPromoteMemberCallback m_Callback;
public:
	FLobbyPromoteMemberCallback(const FOnLobbyPromoteMemberCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FLobbyPromoteMemberCallback()
	{
		m_Callback.Unbind();
	}
};

struct FLobbyKickMemberCallback
{
public:
	FOnLobbyKickMemberCallback m_Callback;
public:
	FLobbyKickMemberCallback(const FOnLobbyKickMemberCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FLobbyKickMemberCallback()
	{
		m_Callback.Unbind();
	}
};

struct FLobbySendInviteCallback
{
public:
	FOnLobbySendInviteCallback m_Callback;
public:
	FLobbySendInviteCallback(const FOnLobbySendInviteCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FLobbySendInviteCallback()
	{
		m_Callback.Unbind();
	}
};

struct FLobbyRejectInviteCallback
{
public:
	FOnLobbyRejectInviteCallback m_Callback;
public:
	FLobbyRejectInviteCallback(const FOnLobbyRejectInviteCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FLobbyRejectInviteCallback()
	{
		m_Callback.Unbind();
	}
};

struct FLobbyQueryInvitesCallback
{
public:
	FOnLobbyQueryInvitesCallback m_Callback;
public:
	FLobbyQueryInvitesCallback(const FOnLobbyQueryInvitesCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FLobbyQueryInvitesCallback()
	{
		m_Callback.Unbind();
	}
};

struct FLobbyFindCallback
{
public:
	FOnLobbyFindCallback m_Callback;
public:
	FLobbyFindCallback(const FOnLobbyFindCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FLobbyFindCallback()
	{
		m_Callback.Unbind();
	}
};

struct FLobbyUpdateReceivedCallback
{
public:
	FLobbyUpdateReceivedCallback(const FOnLobbyUpdateReceivedCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FLobbyUpdateReceivedCallback()
	{
		m_Callback.Unbind();
	}

public:
	FOnLobbyUpdateReceivedCallback m_Callback;
};

struct FLobbyInviteRejectedCallback
{
public:
	FLobbyInviteRejectedCallback(const FOnLobbyInviteRejectedCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FLobbyInviteRejectedCallback()
	{
		m_Callback.Unbind();
	}

public:
	FOnLobbyInviteRejectedCallback m_Callback;
};

struct FLobbyNativeInviteRequestedCallback
{
public:
	FLobbyNativeInviteRequestedCallback(const FOnSendLobbyNativeInviteRequestedCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FLobbyNativeInviteRequestedCallback()
	{
		m_Callback.Unbind();
	}

public:
	FOnSendLobbyNativeInviteRequestedCallback m_Callback;
};

struct FLobbyMemberUpdateReceivedCallback
{
public:
	FLobbyMemberUpdateReceivedCallback(const FOnLobbyMemberUpdateReceivedCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FLobbyMemberUpdateReceivedCallback()
	{
		m_Callback.Unbind();
	}

public:
	FOnLobbyMemberUpdateReceivedCallback m_Callback;
};

struct FLobbyMemberStatusReceivedCallback
{
public:
	FLobbyMemberStatusReceivedCallback(const FOnLobbyMemberStatusReceivedCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FLobbyMemberStatusReceivedCallback()
	{
		m_Callback.Unbind();
	}

public:
	FOnLobbyMemberStatusReceivedCallback m_Callback;
};

struct FLobbyInviteAcceptedCallback
{
public:
	FLobbyInviteAcceptedCallback(const FOnLobbyInviteAcceptedCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FLobbyInviteAcceptedCallback()
	{
		m_Callback.Unbind();
	}

public:
	FOnLobbyInviteAcceptedCallback m_Callback;
};

struct FJoinLobbyAcceptedCallback
{
public:
	FJoinLobbyAcceptedCallback(const FOnJoinLobbyAcceptedCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FJoinLobbyAcceptedCallback()
	{
		m_Callback.Unbind();
	}

public:
	FOnJoinLobbyAcceptedCallback m_Callback;
};

struct FLobbyRTCRoomConnectionChanged
{
public:
	FLobbyRTCRoomConnectionChanged(const FOnRTCRoomConnectionChangedCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FLobbyRTCRoomConnectionChanged()
	{
		m_Callback.Unbind();
	}

public:
	FOnRTCRoomConnectionChangedCallback m_Callback;
};

struct FLeaveLobbyRequestedCallback
{
public:
	FLeaveLobbyRequestedCallback(const FOnLeaveLobbyRequestedCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FLeaveLobbyRequestedCallback()
	{
		m_Callback.Unbind();
	}

public:
	FOnLeaveLobbyRequestedCallback m_Callback;
};


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		REQUEST OBJECTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

struct FCreateLobbyOptions : public EOS_Lobby_CreateLobbyOptions
{
	FCreateLobbyOptions(FString InBucketId, FString InLobbyId, int32 InAllowedPlatformIdsCount)
		: EOS_Lobby_CreateLobbyOptions()
	{
		ApiVersion = EOS_LOBBY_CREATELOBBY_API_LATEST;

		PointerArray.AddZeroed(InAllowedPlatformIdsCount);

		if (!InBucketId.IsEmpty())
		{
			BucketId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(BucketId), TCHAR_TO_UTF8(*InBucketId), 256);
		}

		if (!InLobbyId.IsEmpty())
		{
			LobbyId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(LobbyId), TCHAR_TO_UTF8(*InLobbyId), 256);
		}

		AllowedPlatformIds = PointerArray.GetData();
		AllowedPlatformIdsCount = InAllowedPlatformIdsCount;
	}

	~FCreateLobbyOptions()
	{
		delete[] BucketId;
		delete[] LobbyId;
	}
	
	TArray<uint32_t> PointerArray;
};

struct FDestroyLobbyOptions : public EOS_Lobby_DestroyLobbyOptions
{
	FDestroyLobbyOptions(FString lobbyId)
		: EOS_Lobby_DestroyLobbyOptions()
	{
		ApiVersion = EOS_LOBBY_DESTROYLOBBY_API_LATEST;

		if (!lobbyId.IsEmpty())
		{
			LobbyId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(LobbyId), TCHAR_TO_UTF8(*lobbyId), 256);
		}
	}

	~FDestroyLobbyOptions()
	{
		delete[] LobbyId;
	}
};

struct FLeaveLobbyOptions : public EOS_Lobby_LeaveLobbyOptions
{
	FLeaveLobbyOptions(FString lobbyId)
		: EOS_Lobby_LeaveLobbyOptions()
	{
		ApiVersion = EOS_LOBBY_LEAVELOBBY_API_LATEST;

		if (!lobbyId.IsEmpty())
		{
			LobbyId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(LobbyId), TCHAR_TO_UTF8(*lobbyId), 256);
		}
	}

	~FLeaveLobbyOptions()
	{
		delete[] LobbyId;
	}
};

struct FPromoteMemberOptions : public EOS_Lobby_PromoteMemberOptions
{
	FPromoteMemberOptions(FString lobbyId)
		: EOS_Lobby_PromoteMemberOptions()
	{
		ApiVersion = EOS_LOBBY_UPDATELOBBYMODIFICATION_API_LATEST;

		if (!lobbyId.IsEmpty())
		{
			LobbyId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(LobbyId), TCHAR_TO_UTF8(*lobbyId), 256);
		}
	}

	~FPromoteMemberOptions()
	{
		delete[] LobbyId;
	}
};


struct FKickMemberOptions : public EOS_Lobby_KickMemberOptions
{
	FKickMemberOptions(FString lobbyId)
		: EOS_Lobby_KickMemberOptions()
	{
		ApiVersion = EOS_LOBBY_UPDATELOBBYMODIFICATION_API_LATEST;

		if (!lobbyId.IsEmpty())
		{
			LobbyId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(LobbyId), TCHAR_TO_UTF8(*lobbyId), 256);
		}
	}

	~FKickMemberOptions()
	{
		delete[] LobbyId;
	}
};


struct FSendInviteOptions : public EOS_Lobby_SendInviteOptions
{
	FSendInviteOptions(FString lobbyId)
		: EOS_Lobby_SendInviteOptions()
	{
		ApiVersion = EOS_LOBBY_UPDATELOBBYMODIFICATION_API_LATEST;

		if (!lobbyId.IsEmpty())
		{
			LobbyId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(LobbyId), TCHAR_TO_UTF8(*lobbyId), 256);
		}
	}

	~FSendInviteOptions()
	{
		delete[] LobbyId;
	}
};


struct FLobbyInviteReceivedCallback
{
public:
	FLobbyInviteReceivedCallback(const FOnLobbyInviteReceivedCallback& callback)
		: m_Callback(callback)
	{
	}

	~FLobbyInviteReceivedCallback()
	{
		m_Callback.Unbind();
	}

public:
	FOnLobbyInviteReceivedCallback m_Callback;
};


struct FCopyLobbyDetailsHandleOptions : public EOS_Lobby_CopyLobbyDetailsHandleOptions
{
	FCopyLobbyDetailsHandleOptions(FString lobbyId)
		: EOS_Lobby_CopyLobbyDetailsHandleOptions()
	{
		ApiVersion = EOS_LOBBY_UPDATELOBBYMODIFICATION_API_LATEST;

		if (!lobbyId.IsEmpty())
		{
			LobbyId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(LobbyId), TCHAR_TO_UTF8(*lobbyId), 256);
		}
	}

	~FCopyLobbyDetailsHandleOptions()
	{
		delete[] LobbyId;
	}
};


struct FGetRTCRoomNameOptions : public EOS_Lobby_GetRTCRoomNameOptions
{
	FGetRTCRoomNameOptions(FString lobbyId)
		: EOS_Lobby_GetRTCRoomNameOptions()
	{
		ApiVersion = EOS_LOBBY_GETRTCROOMNAME_API_LATEST;

		if (!lobbyId.IsEmpty())
		{
			LobbyId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(LobbyId), TCHAR_TO_UTF8(*lobbyId), 256);
		}
	}

	~FGetRTCRoomNameOptions()
	{
		delete[] LobbyId;
	}
};


struct FIsRTCRoomConnectedOptions : public EOS_Lobby_IsRTCRoomConnectedOptions
{
	FIsRTCRoomConnectedOptions(FString lobbyId)
		: EOS_Lobby_IsRTCRoomConnectedOptions()
	{
		ApiVersion = EOS_LOBBY_ISRTCROOMCONNECTED_API_LATEST;

		if (!lobbyId.IsEmpty())
		{
			LobbyId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(LobbyId), TCHAR_TO_UTF8(*lobbyId), 256);
		}
	}

	~FIsRTCRoomConnectedOptions()
	{
		delete[] LobbyId;
	}
};

struct FAddNotifyRTCRoomConnectionChangedOptions : public EOS_Lobby_AddNotifyRTCRoomConnectionChangedOptions
{
	FAddNotifyRTCRoomConnectionChangedOptions(FString lobbyId)
		: EOS_Lobby_AddNotifyRTCRoomConnectionChangedOptions()
	{
		ApiVersion = EOS_LOBBY_ADDNOTIFYRTCROOMCONNECTIONCHANGED_API_LATEST;

		if (!lobbyId.IsEmpty())
		{
			LobbyId_DEPRECATED = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(LobbyId_DEPRECATED), TCHAR_TO_UTF8(*lobbyId), 256);
		}
	}

	~FAddNotifyRTCRoomConnectionChangedOptions()
	{
		delete[] LobbyId_DEPRECATED;
	}
};


struct FSetLobbyIdOptions : public EOS_LobbySearch_SetLobbyIdOptions
{
	FSetLobbyIdOptions(FString lobbyId)
		: EOS_LobbySearch_SetLobbyIdOptions()
	{
		ApiVersion = EOS_LOBBY_UPDATELOBBYMODIFICATION_API_LATEST;

		if (!lobbyId.IsEmpty())
		{
			LobbyId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(LobbyId), TCHAR_TO_UTF8(*lobbyId), 256);
		}
	}

	~FSetLobbyIdOptions()
	{
		delete[] LobbyId;
	}
};
