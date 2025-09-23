/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "eos_anticheatcommon_types.h"
#include "EOSCoreAntiCheatCommon.generated.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		ENUMS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

/** Flags describing the type of a remote client */
UENUM(BlueprintType)
enum class EEOSEAntiCheatCommonClientType : uint8
{
	/** An ordinary player that requires anti-cheat client protection to play */
	EOS_ACCCT_ProtectedClient = 0,
	/** The player does not need the anti-cheat client to play because of their platform or other factors */
	EOS_ACCCT_UnprotectedClient = 1,
	/** The client is an AI bot, not an actual human */
	EOS_ACCCT_AIBot = 2
};

/** Flags describing the platform of a remote client, if known */
UENUM(BlueprintType)
enum class EEOSEAntiCheatCommonClientPlatform : uint8
{
	/** Unknown platform */
	EOS_ACCCP_Unknown = 0,
	/** The client is playing on Windows */
	EOS_ACCCP_Windows = 1,
	/** The client is playing on Mac */
	EOS_ACCCP_Mac = 2,
	/** The client is playing on Linux */
	EOS_ACCCP_Linux = 3,
	/** The client is playing on an Xbox device */
	EOS_ACCCP_Xbox = 4,
	/** The client is playing on a PlayStation device */
	EOS_ACCCP_PlayStation = 5,
	/** The client is playing on a Nintendo device */
	EOS_ACCCP_Nintendo = 6,
	/** The client is playing on iOS */
	EOS_ACCCP_iOS = 7,
	/** The client is playing on Android */
	EOS_ACCCP_Android = 8
};

/** Anti-cheat action values. Applicable to both clients and remote peers. */
UENUM(BlueprintType)
enum class EEOSEAntiCheatCommonClientAction : uint8
{
	/** Not used */
	EOS_ACCCA_Invalid = 0,
	/** The client/peer must be removed from the current game session */
	EOS_ACCCA_RemovePlayer = 1
};

/** Anti-cheat action reasons. Applicable to both clients and remote peers. */
UENUM(BlueprintType)
enum class EEOSEAntiCheatCommonClientActionReason : uint8
{
	/** Not used */
	EOS_ACCCAR_Invalid = 0,
	/** An internal error occurred */
	EOS_ACCCAR_InternalError = 1,
	/** An anti-cheat message received from the client/peer was corrupt or invalid */
	EOS_ACCCAR_InvalidMessage = 2,
	/** The client/peer's anti-cheat authentication failed */
	EOS_ACCCAR_AuthenticationFailed = 3,
	/** The client/peer failed to load the anti-cheat module at startup */
	EOS_ACCCAR_NullClient = 4,
	/** The client/peer's anti-cheat heartbeat was not received */
	EOS_ACCCAR_HeartbeatTimeout = 5,
	/** The client/peer failed an anti-cheat client runtime check */
	EOS_ACCCAR_ClientViolation = 6,
	/** The client/peer failed an anti-cheat backend runtime check */
	EOS_ACCCAR_BackendViolation = 7,
	/** The client/peer is temporarily blocked from playing on this game server */
	EOS_ACCCAR_TemporaryCooldown = 8,
	/** The client/peer is temporarily banned */
	EOS_ACCCAR_TemporaryBanned = 9,
	/** The client/peer is permanently banned */
	EOS_ACCCAR_PermanentBanned = 10
};

/** The client/peer's anti-cheat authentication status */
UENUM(BlueprintType)
enum class EEOSEAntiCheatCommonClientAuthStatus : uint8
{
	/** Not used */
	EOS_ACCCAS_Invalid = 0,
	/** The client/peer's anti-cheat functionality has been validated by this game server */
	EOS_ACCCAS_LocalAuthComplete = 1,
	/** The client/peer's anti-cheat functionality has been validated by the anti-cheat backend service */
	EOS_ACCCAS_RemoteAuthComplete = 2
};

/** Flags describing a remote client. These can be updated during a play session */
UENUM(BlueprintType)
enum class EEOSEAntiCheatCommonClientFlags : uint8
{
	/** No particular flags relevant for this client */
	EOS_ACCCF_None = 0,
	/** The client has admin privileges on the game server */
	EOS_ACCCF_Admin = (1 << 0)
};

ENUM_CLASS_FLAGS(EEOSEAntiCheatCommonClientFlags);

/** Flags describing the input device used by a remote client, if known. These can be updated during a play session. */
UENUM(BlueprintType)
enum class EEOSEAntiCheatCommonClientInput : uint8
{
	/** Unknown input device */
	EOS_ACCCI_Unknown = 0,
	/** The client is using mouse and keyboard */
	EOS_ACCCI_MouseKeyboard = 1,
	/** The client is using a gamepad or game controller */
	EOS_ACCCI_Gamepad = 2,
	/** The client is using a touch input device (e.g. phone/tablet screen) */
	EOS_ACCCI_TouchInput = 3
};

/**
* Types supported for custom gameplay behavior events.
* These have a considerable impact on performance
*/
UENUM(BlueprintType)
enum class EEOSEAntiCheatCommonEventType : uint8
{
	/** Not used */
	EOS_ACCET_Invalid = 0,
	/**
	* A general game event that is not specific to any individual player.
	* Low memory use which is constant with respect to the number of players.
	*/
	EOS_ACCET_GameEvent = 1,
	/**
	* An event that is logically associated with a specific player. Events logged in
	* this category require a specific ClientHandle to which they will be attached.
	* Higher memory use which scales according to the number of players.
	*/
	EOS_ACCET_PlayerEvent = 2
};

/** Types supported for custom gameplay behavior event parameters */
UENUM(BlueprintType)
enum class EEOSEAntiCheatCommonEventParamType : uint8
{
	/** Not used */
	EOS_ACCEPT_Invalid = 0,
	/** EOS_AntiCheatCommon_ClientHandle */
	EOS_ACCEPT_ClientHandle = 1,
	/** const char* */
	EOS_ACCEPT_String = 2,
	/** uint32_t */
	EOS_ACCEPT_UInt32 = 3,
	/** int32_t */
	EOS_ACCEPT_Int32 = 4,
	/** uint64_t */
	EOS_ACCEPT_UInt64 = 5,
	/** int64_t */
	EOS_ACCEPT_Int64 = 6,
	/** EOS_AntiCheatCommon_Vec3f */
	EOS_ACCEPT_Vector3f = 7,
	/** EOS_AntiCheatCommon_Quat */
	EOS_ACCEPT_Quat = 8
};

/** Details of a player's movement state */
UENUM(BlueprintType)
enum class EEOSEAntiCheatCommonPlayerMovementState : uint8
{
	/** No particular state applies */
	EOS_ACCPMS_None = 0,
	/** Player is crouching */
	EOS_ACCPMS_Crouching = 1,
	/** Player is prone */
	EOS_ACCPMS_Prone = 2,
	/** Player is mounted in a vehicle or similar */
	EOS_ACCPMS_Mounted = 3,
	/** Player is swimming in a fluid volume */
	EOS_ACCPMS_Swimming = 4,
	/** Player is falling under the effects of gravity, such as when jumping or walking off the edge of a surface */
	EOS_ACCPMS_Falling = 5,
	/** Player is flying, ignoring the effects of gravity */
	EOS_ACCPMS_Flying = 6,
	/** Player is on a ladder */
	EOS_ACCPMS_OnLadder = 7
};

/** The source of a damage event */
UENUM(BlueprintType)
enum class EEOSEAntiCheatCommonPlayerTakeDamageSource : uint8
{
	/** No particular source relevant */
	EOS_ACCPTDS_None = 0,
	/** Damage caused by a player controlled character */
	EOS_ACCPTDS_Player = 1,
	/** Damage caused by a non-player character such as an AI enemy */
	EOS_ACCPTDS_NonPlayerCharacter = 2,
	/** Damage caused by the world (falling off level, into lava, etc) */
	EOS_ACCPTDS_World = 3
};

/** Type of damage applied in a damage event */
UENUM(BlueprintType)
enum class EEOSEAntiCheatCommonPlayerTakeDamageType : uint8
{
	/** No particular type relevant */
	EOS_ACCPTDT_None = 0,
	/** Damage caused by a point source such as a bullet or melee attack */
	EOS_ACCPTDT_PointDamage = 1,
	/** Damage caused by a radial source such as an explosion */
	EOS_ACCPTDT_RadialDamage = 2,
	/** Damage over time such as bleeding, poison, etc */
	EOS_ACCPTDT_DamageOverTime = 3
};

/** The result of a damage event, if any */
UENUM(BlueprintType)
enum class EEOSEAntiCheatCommonPlayerTakeDamageResult : uint8
{
	/** No direct state change consequence for the victim */
	EOS_ACCPTDR_None = 0,
	/** Player character is temporarily incapacitated and requires assistance to recover */
	EOS_ACCPTDR_Downed = 1,
	/** Player character is permanently incapacitated and cannot recover (e.g. dead) */
	EOS_ACCPTDR_Eliminated = 2
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		STRUCTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

USTRUCT(BlueprintType)
struct FEOSAntiCheatCommonClientHandle
{
	GENERATED_BODY()

public:
	FEOSAntiCheatCommonClientHandle()
		: Handle(nullptr)
	{
	}

public:
	FEOSAntiCheatCommonClientHandle(EOS_AntiCheatCommon_ClientHandle InHandle)
		: Handle(static_cast<UObject*>(InHandle))
	{
	}

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	UObject* Handle;

public:
	operator EOS_AntiCheatCommon_ClientHandle()
	{
		return Handle;
	}
};

/** Vector using left-handed coordinate system (as in Unreal Engine) */
USTRUCT(BlueprintType)
struct FEOSAntiCheatCommonVec3f
{
	GENERATED_BODY()

public:
	explicit FEOSAntiCheatCommonVec3f()
		: X(0.0f)
		, Y(0.0f)
		, Z(0.0f)
	{
	}

public:
	/** X axis coordinate - forward direction */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	float X;
	/** Y axis coordinate - right direction */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	float Y;
	/** Z axis coordinate - up direction */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	float Z;

public:
	operator EOS_AntiCheatCommon_Vec3f() const
	{
		EOS_AntiCheatCommon_Vec3f Data;
		Data.x = X;
		Data.y = Y;
		Data.z = Z;
		return Data;
	}
};

/** Quaternion using left-handed coordinate system (as in Unreal Engine) */
USTRUCT(BlueprintType)
struct FEOSAntiCheatCommonQuat
{
	GENERATED_BODY()

public:
	explicit FEOSAntiCheatCommonQuat()
		: W(0.0f)
		, X(0.0f)
		, Y(0.0f)
		, Z(0.0f)
	{
	}

public:
	/** W component - scalar part */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	float W;
	/** X component - forward direction */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	float X;
	/** Y component - right direction */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	float Y;
	/** Z component - up direction */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	float Z;

public:
	operator EOS_AntiCheatCommon_Quat()
	{
		EOS_AntiCheatCommon_Quat Data;
		Data.w = W;
		Data.x = X;
		Data.y = Y;
		Data.z = Z;
		return Data;
	}
};

/**
* Structure containing details about a new message that must be dispatched to a connected client/peer.
*/
USTRUCT(BlueprintType)
struct FEOSAntiCheatCommonOnMessageToClientCallbackInfo
{
	GENERATED_BODY()

public:
	/** Caller-specified context data */
	void* ClientData;
	/** The identifier of the client/peer that this message must be delivered to. See the RegisterClient and RegisterPeer functions. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FEOSAntiCheatCommonClientHandle ClientHandle;
	/** The message data that must be sent to the server */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FString MessageData;
	/** The size in bytes of MessageData */
	int32 MessageDataSizeBytes;

public:
	explicit FEOSAntiCheatCommonOnMessageToClientCallbackInfo()
		: ClientData(nullptr)
		, ClientHandle(nullptr)
		, MessageDataSizeBytes(0)
	{
	}

	FEOSAntiCheatCommonOnMessageToClientCallbackInfo(const EOS_AntiCheatCommon_OnMessageToClientCallbackInfo& data)
		: ClientData(data.ClientData)
		, ClientHandle(data.ClientHandle)
		, MessageData(UTF8_TO_TCHAR(data.MessageData))
		, MessageDataSizeBytes(data.MessageDataSizeBytes)
	{
	}
};

/** Structure containing details about a required client/peer action */
USTRUCT(BlueprintType)
struct FEOSAntiCheatCommonOnClientActionRequiredCallbackInfo
{
	GENERATED_BODY()

public:
	/** Caller-specified context data */
	void* ClientData;
	/** The identifier of the client/peer that this action applies to. See the RegisterClient and RegisterPeer functions. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FEOSAntiCheatCommonClientHandle ClientHandle;
	/** The action that must be applied to the specified client/peer */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	EEOSEAntiCheatCommonClientAction ClientAction;
	/** Code indicating the reason for the action */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	EEOSEAntiCheatCommonClientActionReason ActionReasonCode;
	/** String containing details about the action reason */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FString ActionReasonDetailsString;

public:
	explicit FEOSAntiCheatCommonOnClientActionRequiredCallbackInfo()
		: ClientData(nullptr)
		, ClientHandle(nullptr)
		, ClientAction(EEOSEAntiCheatCommonClientAction::EOS_ACCCA_Invalid)
		, ActionReasonCode(EEOSEAntiCheatCommonClientActionReason::EOS_ACCCAR_Invalid)
	{
	}

	FEOSAntiCheatCommonOnClientActionRequiredCallbackInfo(const EOS_AntiCheatCommon_OnClientActionRequiredCallbackInfo& data)
		: ClientData(data.ClientData)
		, ClientHandle(data.ClientHandle)
		, ClientAction(static_cast<EEOSEAntiCheatCommonClientAction>(data.ClientAction))
		, ActionReasonCode(static_cast<EEOSEAntiCheatCommonClientActionReason>(data.ActionReasonCode))
		, ActionReasonDetailsString(UTF8_TO_TCHAR(data.ActionReasonDetailsString))
	{
	}
};

/** Structure containing details about a client/peer authentication status change */
USTRUCT(BlueprintType)
struct FEOSAntiCheatCommonOnClientAuthStatusChangedCallbackInfo
{
	GENERATED_BODY()

public:
	/** Caller-specified context data */
	void* ClientData;
	/** The identifier of the client/peer that this status change applies to. See the RegisterClient and RegisterPeer functions. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FEOSAntiCheatCommonClientHandle ClientHandle;
	/** The client/peer's new authentication status */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	EEOSEAntiCheatCommonClientAuthStatus ClientAuthStatus;

public:
	explicit FEOSAntiCheatCommonOnClientAuthStatusChangedCallbackInfo()
		: ClientData(nullptr)
		, ClientHandle(nullptr)
		, ClientAuthStatus(EEOSEAntiCheatCommonClientAuthStatus::EOS_ACCCAS_Invalid)
	{
	}

	FEOSAntiCheatCommonOnClientAuthStatusChangedCallbackInfo(const EOS_AntiCheatCommon_OnClientAuthStatusChangedCallbackInfo& data)
		: ClientData(data.ClientData)
		, ClientHandle(data.ClientHandle)
		, ClientAuthStatus(static_cast<EEOSEAntiCheatCommonClientAuthStatus>(data.ClientAuthStatus))
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatCommonSetClientDetailsOptions
{
	GENERATED_BODY()

public:
	/** Version of the API */
	int32 ApiVersion;

public:
	/** Locally unique value used in RegisterClient/RegisterPeer */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FEOSAntiCheatCommonClientHandle ClientHandle;
	/** General flags associated with this client, if any */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	EEOSEAntiCheatCommonClientFlags ClientFlags;
	/** Input device being used by this client, if known */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	EEOSEAntiCheatCommonClientInput ClientInputMethod;

public:
	explicit FEOSAntiCheatCommonSetClientDetailsOptions()
		: ApiVersion(EOS_ANTICHEATCOMMON_SETCLIENTDETAILS_API_LATEST)
		, ClientHandle(nullptr)
		, ClientFlags(EEOSEAntiCheatCommonClientFlags::EOS_ACCCF_None)
		, ClientInputMethod(EEOSEAntiCheatCommonClientInput::EOS_ACCCI_Unknown)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatCommonSetGameSessionIdOptions
{
	GENERATED_BODY()

public:
	/** Version of the API */
	int32 ApiVersion;

public:
	/** Game session identifier */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FString GameSessionId;

public:
	explicit FEOSAntiCheatCommonSetGameSessionIdOptions()
		: ApiVersion(EOS_ANTICHEATCOMMON_SETGAMESESSIONID_API_LATEST)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatCommonRegisterEventParamDef
{
	GENERATED_BODY()

public:
	/** Parameter name. Allowed characters are 0-9, A-Z, a-z, '_', '-', '.' */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FString ParamName;
	/** Parameter type */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	EEOSEAntiCheatCommonEventParamType ParamType;

public:
	explicit FEOSAntiCheatCommonRegisterEventParamDef()
		: ParamType(EEOSEAntiCheatCommonEventParamType::EOS_ACCEPT_Invalid)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatCommonRegisterEventOptions
{
	GENERATED_BODY()

public:
	/** Version of the API */
	int32 ApiVersion;

public:
	/** Unique event identifier. Must be >= EOS_ANTICHEATCOMMON_REGISTEREVENT_CUSTOMEVENTBASE. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	int32 EventId;
	/** Name of the custom event. Allowed characters are 0-9, A-Z, a-z, '_', '-', '.' */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FString EventName;
	/** Type of the custom event */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	EEOSEAntiCheatCommonEventType EventType;
	/** Number of parameters described in ParamDefs. Must be <= EOS_ANTICHEATCOMMON_REGISTEREVENT_MAX_PARAMDEFSCOUNT. */
	uint32_t ParamDefsCount;
	/** Pointer to an array of EOS_AntiCheatCommon_RegisterEventParamDef with ParamDefsCount elements */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	TArray<FEOSAntiCheatCommonRegisterEventParamDef> ParamDefs;

public:
	explicit FEOSAntiCheatCommonRegisterEventOptions()
		: ApiVersion(EOS_ANTICHEATCOMMON_REGISTEREVENT_API_LATEST)
		, EventId(0)
		, EventType(EEOSEAntiCheatCommonEventType::EOS_ACCET_Invalid)
		, ParamDefsCount(0)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatCommonLogEventParamPair
{
	GENERATED_BODY()

public:
	/** Version of the API */
	int32 ApiVersion;

public:
	/** Parameter type */
	EEOSEAntiCheatCommonEventParamType ParamValueType;

	union
	{
		EOS_AntiCheatCommon_ClientHandle ClientHandle;
		const char* String; // Will be truncated if longer than EOS_ANTICHEATCOMMON_LOGEVENT_STRING_MAX_LENGTH bytes.
		uint32_t UInt32;
		int32_t Int32;
		uint64_t UInt64;
		int64_t Int64;
		EOS_AntiCheatCommon_Vec3f Vec3f;
		EOS_AntiCheatCommon_Quat Quat;
	} ParamValue;

public:
	explicit FEOSAntiCheatCommonLogEventParamPair()
		: ApiVersion(EOS_ANTICHEATCOMMON_LOGEVENT_API_LATEST)
		, ParamValueType(EEOSEAntiCheatCommonEventParamType::EOS_ACCEPT_Invalid)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatCommonLogEventOptions
{
	GENERATED_BODY()

public:
	/** Version of the API */
	int32 ApiVersion;

public:
	/** Optional client who this event is primarily associated with. If not applicable, use 0. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FEOSAntiCheatCommonClientHandle ClientHandle;
	/** Unique event identifier previously configured in RegisterEvent */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	int32 EventId;
	/** Set of parameter types previously configured in RegisterEvent, and their values */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	TArray<FEOSAntiCheatCommonLogEventParamPair> Params;

public:
	explicit FEOSAntiCheatCommonLogEventOptions()
		: ApiVersion(EOS_ANTICHEATCOMMON_LOGEVENT_API_LATEST)
		, ClientHandle(nullptr)
		, EventId(0)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatCommonLogGameRoundStartOptions
{
	GENERATED_BODY()

public:
	/** Version of the API */
	int32 ApiVersion;

public:
	/** Optional game session or match identifier useful for some backend API integrations */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FString SessionIdentifier;
	/** Optional name of the map being played */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FString LevelName;
	/** Optional name of the game mode being played */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FString ModeName;
	/** Optional length of the game round to be played, in seconds. If none, use 0. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	int32 RoundTimeSeconds;

public:
	explicit FEOSAntiCheatCommonLogGameRoundStartOptions()
		: ApiVersion(EOS_ANTICHEATCOMMON_LOGGAMEROUNDSTART_API_LATEST)
		, RoundTimeSeconds(0)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatCommonLogGameRoundEndOptions
{
	GENERATED_BODY()

public:
	/** Version of the API */
	int32 ApiVersion;

public:
	/** Optional identifier for the winning team */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	int32 WinningTeamId;

public:
	explicit FEOSAntiCheatCommonLogGameRoundEndOptions()
		: ApiVersion(EOS_ANTICHEATCOMMON_LOGGAMEROUNDEND_API_LATEST)
		, WinningTeamId(0)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatCommonLogPlayerSpawnOptions
{
	GENERATED_BODY()

public:
	/** Version of the API */
	int32 ApiVersion;

public:
	/** Locally unique value used in RegisterClient/RegisterPeer */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FEOSAntiCheatCommonClientHandle SpawnedPlayerHandle;
	/** Optional identifier for the player's team. If none, use 0. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	int32 TeamId;
	/** Optional identifier for the player's character. If none, use 0. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	int32 CharacterId;

public:
	explicit FEOSAntiCheatCommonLogPlayerSpawnOptions()
		: ApiVersion(EOS_ANTICHEATCOMMON_LOGPLAYERSPAWN_API_LATEST)
		, TeamId(0)
		, CharacterId(0)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatCommonLogPlayerDespawnOptions
{
	GENERATED_BODY()

public:
	/** Version of the API */
	int32 ApiVersion;

public:
	/** Locally unique value used in RegisterClient/RegisterPeer */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FEOSAntiCheatCommonClientHandle DespawnedPlayerHandle;

public:
	explicit FEOSAntiCheatCommonLogPlayerDespawnOptions()
		: ApiVersion(EOS_ANTICHEATCOMMON_LOGPLAYERDESPAWN_API_LATEST)
		, DespawnedPlayerHandle(nullptr)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatCommonLogPlayerReviveOptions
{
	GENERATED_BODY()

public:
	/** Version of the API */
	int32 ApiVersion;

public:
	/** Locally unique value used in RegisterClient/RegisterPeer */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FEOSAntiCheatCommonClientHandle RevivedPlayerHandle;
	/** Locally unique value used in RegisterClient/RegisterPeer */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FEOSAntiCheatCommonClientHandle ReviverPlayerHandle;

public:
	explicit FEOSAntiCheatCommonLogPlayerReviveOptions()
		: ApiVersion(EOS_ANTICHEATCOMMON_LOGPLAYERREVIVE_API_LATEST)
		, RevivedPlayerHandle(nullptr)
		, ReviverPlayerHandle(nullptr)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatCommonLogPlayerTickOptions
{
	GENERATED_BODY()

public:
	/** Version of the API */
	int32 ApiVersion;

public:
	/** Locally unique value used in RegisterClient/RegisterPeer */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FEOSAntiCheatCommonClientHandle PlayerHandle;
	/** Player's current world position as a 3D vector */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FEOSAntiCheatCommonVec3f PlayerPosition;
	/** Player's view rotation as a quaternion */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FEOSAntiCheatCommonQuat PlayerViewRotation;
	/** True if the player's view is zoomed (e.g. using a sniper rifle), otherwise false */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	bool bIsPlayerViewZoomed;
	/** Player's current health value */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	float PlayerHealth;
	/** Any movement state applicable */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	EEOSEAntiCheatCommonPlayerMovementState PlayerMovementState;

public:
	explicit FEOSAntiCheatCommonLogPlayerTickOptions()
		: ApiVersion(EOS_ANTICHEATCOMMON_LOGPLAYERTICK_API_LATEST)
		, PlayerHandle(nullptr)
		, bIsPlayerViewZoomed(false)
		, PlayerHealth(0.0f)
		, PlayerMovementState(EEOSEAntiCheatCommonPlayerMovementState::EOS_ACCPMS_None)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatCommonLogPlayerUseWeaponData
{
	GENERATED_BODY()

public:
	/** Version of the API */
	int32 ApiVersion;

public:
	/** Locally unique value used in RegisterClient/RegisterPeer */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FEOSAntiCheatCommonClientHandle PlayerHandle;
	/** Player's current world position as a 3D vector */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FEOSAntiCheatCommonVec3f PlayerPosition;
	/** Player's view rotation as a quaternion */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FEOSAntiCheatCommonQuat PlayerViewRotation;
	/** True if the player's view is zoomed (e.g. using a sniper rifle), otherwise false */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	bool bIsPlayerViewZoomed;
	/** Set to true if the player is using a melee attack, otherwise false */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	bool bIsMeleeAttack;
	/** Name of the weapon used. Will be truncated to EOS_ANTICHEATCOMMON_LOGPLAYERUSEWEAPON_WEAPONNAME_MAX_LENGTH bytes if longer. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FString WeaponName;

public:
	explicit FEOSAntiCheatCommonLogPlayerUseWeaponData()
		: ApiVersion(EOS_ANTICHEATCOMMON_LOGPLAYERUSEWEAPON_API_LATEST)
		, PlayerHandle(nullptr)
		, bIsPlayerViewZoomed(false)
		, bIsMeleeAttack(false)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatCommonLogPlayerUseWeaponOptions
{
	GENERATED_BODY()

public:
	/** Version of the API */
	int32 ApiVersion;

public:
	/** Struct containing detailed information about a weapon use event */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FEOSAntiCheatCommonLogPlayerUseWeaponData UseWeaponData;

public:
	explicit FEOSAntiCheatCommonLogPlayerUseWeaponOptions()
		: ApiVersion(EOS_ANTICHEATCOMMON_LOGPLAYERUSEWEAPON_API_LATEST)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatCommonLogPlayerUseAbilityOptions
{
	GENERATED_BODY()

public:
	/** Version of the API */
	int32 ApiVersion;

public:
	/** Locally unique value used in RegisterClient/RegisterPeer */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FEOSAntiCheatCommonClientHandle PlayerHandle;
	/** Game defined unique identifier for the ability being used */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	int32 AbilityId;
	/** Duration of the ability effect in milliseconds. If not applicable, use 0. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	int32 AbilityDurationMs;
	/** Cooldown until the ability can be used again in milliseconds. If not applicable, use 0. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	int32 AbilityCooldownMs;

public:
	explicit FEOSAntiCheatCommonLogPlayerUseAbilityOptions()
		: ApiVersion(EOS_ANTICHEATCOMMON_LOGPLAYERUSEABILITY_API_LATEST)
		, PlayerHandle(nullptr)
		, AbilityId(0)
		, AbilityDurationMs(0)
		, AbilityCooldownMs(0)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatCommonLogPlayerTakeDamageOptions
{
	GENERATED_BODY()

public:
	/** Version of the API */
	int32 ApiVersion;

public:
	/** Locally unique value used in RegisterClient/RegisterPeer */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FEOSAntiCheatCommonClientHandle VictimPlayerHandle;
	/** Victim player's current world position as a 3D vector */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FEOSAntiCheatCommonVec3f VictimPlayerPosition;
	/** Victim player's view rotation as a quaternion */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FEOSAntiCheatCommonQuat VictimPlayerViewRotation;
	/** Locally unique value used in RegisterClient/RegisterPeer */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FEOSAntiCheatCommonClientHandle AttackerPlayerHandle;
	/** Attacker player's current world position as a 3D vector */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FEOSAntiCheatCommonVec3f AttackerPlayerPosition;
	/** Attacker player's view rotation as a quaternion */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FEOSAntiCheatCommonQuat AttackerPlayerViewRotation;
	/**
	 * True if the damage was applied instantly at the time of attack from the game
	 * simulation's perspective, otherwise false (simulated ballistics, arrow, etc).
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	bool bIsHitscanAttack;
	/**
	 * True if there is a visible line of sight between the attacker and the victim at the time
	 * that damage is being applied, false if there is an obstacle like a wall or terrain in
	 * the way. For some situations like melee or hitscan weapons this is trivially
	 * true, for others like projectiles with simulated physics it may not be e.g. a player
	 * could fire a slow moving projectile and then move behind cover before it strikes.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	bool bHasLineOfSight;
	/** True if this was a critical hit that causes extra damage (e.g. headshot) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	bool bIsCriticalHit;
	/** Number of health points that the victim lost due to this damage event */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	float DamageTaken;
	/** Number of health points that the victim has remaining after this damage event */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	float HealthRemaining;
	/** Source of the damage event */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	EEOSEAntiCheatCommonPlayerTakeDamageSource DamageSource;
	/** Type of the damage being applied */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	EEOSEAntiCheatCommonPlayerTakeDamageType DamageType;
	/** Result of the damage for the victim, if any */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	EEOSEAntiCheatCommonPlayerTakeDamageResult DamageResult;
	/** PlayerUseWeaponData associated with this damage event if available, otherwise NULL */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	FEOSAntiCheatCommonLogPlayerUseWeaponData PlayerUseWeaponData;
	/** Time in milliseconds since the PlayerUseWeaponData event occurred if available, otherwise 0 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheat")
	int32 TimeSincePlayerUseWeaponMs;

public:
	explicit FEOSAntiCheatCommonLogPlayerTakeDamageOptions()
		: ApiVersion(EOS_ANTICHEATCOMMON_LOGPLAYERTAKEDAMAGE_API_LATEST)
		, VictimPlayerHandle(nullptr)
		, AttackerPlayerHandle(nullptr)
		, bIsHitscanAttack(false)
		, bHasLineOfSight(false)
		, bIsCriticalHit(false)
		, DamageTaken(0.0f)
		, HealthRemaining(0.0f)
		, DamageSource(EEOSEAntiCheatCommonPlayerTakeDamageSource::EOS_ACCPTDS_None)
		, DamageType(EEOSEAntiCheatCommonPlayerTakeDamageType::EOS_ACCPTDT_None)
		, DamageResult(EEOSEAntiCheatCommonPlayerTakeDamageResult::EOS_ACCPTDR_None)
		, TimeSincePlayerUseWeaponMs(0)
	{
	}
};
