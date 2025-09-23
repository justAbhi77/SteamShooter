/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "eos_anticheatclient_types.h"
#include "Core/EOSCoreAntiCheatCommon.h"
#include "Core/EOSTypes.h"
#include "Core/EOSHelpers.h"
#include "EOSAntiCheatClientTypes.generated.h"

class UCoreAntiCheatClient;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		ENUMS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

/** Operating modes */
UENUM(BlueprintType)
enum class EEOSEAntiCheatClientMode : uint8
{
	/** Not used */
	EOS_ACCM_Invalid = 0,
	/** Dedicated or listen server mode */
	EOS_ACCM_ClientServer = 1,
	/** Full mesh peer-to-peer mode */
	EOS_ACCM_PeerToPeer = 2
};

/** Anti-cheat integrity violation types */
UENUM(BlueprintType)
enum class EEOSEAntiCheatClientViolationType : uint8
{
	/** Not used */
	EOS_ACCVT_Invalid = 0,
	/** An anti-cheat asset integrity catalog file could not be found */
	EOS_ACCVT_IntegrityCatalogNotFound = 1,
	/** An anti-cheat asset integrity catalog file is corrupt or invalid */
	EOS_ACCVT_IntegrityCatalogError = 2,
	/** An anti-cheat asset integrity catalog file's certificate has been revoked. */
	EOS_ACCVT_IntegrityCatalogCertificateRevoked = 3,
	/**
	* The primary anti-cheat asset integrity catalog does not include an entry for the game's
	* main executable, which is required.
	*/
	EOS_ACCVT_IntegrityCatalogMissingMainExecutable = 4,
	/** A disallowed game file modification was detected */
	EOS_ACCVT_GameFileMismatch = 5,
	/** A disallowed game file removal was detected */
	EOS_ACCVT_RequiredGameFileNotFound = 6,
	/** A disallowed game file addition was detected */
	EOS_ACCVT_UnknownGameFileForbidden = 7,
	/** A system file failed an integrity check */
	EOS_ACCVT_SystemFileUntrusted = 8,
	/** A disallowed code module was loaded into the game process */
	EOS_ACCVT_ForbiddenModuleLoaded = 9,
	/** A disallowed game process memory modification was detected */
	EOS_ACCVT_CorruptedMemory = 10,
	/** A disallowed tool was detected running in the system */
	EOS_ACCVT_ForbiddenToolDetected = 11,
	/** An internal anti-cheat integrity check failed */
	EOS_ACCVT_InternalAntiCheatViolation = 12,
	/** Integrity checks on messages between the game client and game server failed */
	EOS_ACCVT_CorruptedNetworkMessageFlow = 13,
	/** The game is running inside a disallowed virtual machine */
	EOS_ACCVT_VirtualMachineNotAllowed = 14,
	/** A forbidden operating system configuration was detected */
	EOS_ACCVT_ForbiddenSystemConfiguration = 15
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		STRUCTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

/** Structure containing details about integrity violation related to the local client */
USTRUCT(BlueprintType)
struct FEOSAntiCheatClientOnClientIntegrityViolatedCallbackInfo
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatClientOnClientIntegrityViolatedCallbackInfo()
		: ClientData(nullptr)
		  , ViolationType(EEOSEAntiCheatClientViolationType::EOS_ACCVT_Invalid)
	{
	}

	FEOSAntiCheatClientOnClientIntegrityViolatedCallbackInfo(const EOS_AntiCheatClient_OnClientIntegrityViolatedCallbackInfo& data)
		: ClientData(data.ClientData)
		, ViolationType(static_cast<EEOSEAntiCheatClientViolationType>(data.ViolationType))
		  , ViolationMessage(UTF8_TO_TCHAR(data.ViolationMessage))
	{
	
	}
public:
	/** Caller-specified context data */
	void* ClientData;
	/** Code describing the violation that occurred */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	EEOSEAntiCheatClientViolationType ViolationType;
	/** String describing the violation which should be displayed to the user */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FString ViolationMessage;
};

/**
* Structure containing details about a new message that must be dispatched to the game server.
*/
USTRUCT(BlueprintType)
struct FEOSAntiCheatClientOnMessageToServerCallbackInfo
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatClientOnMessageToServerCallbackInfo()
		: ClientData(nullptr)
		  , MessageDataSizeBytes(0)
	{
	}

	FEOSAntiCheatClientOnMessageToServerCallbackInfo(const EOS_AntiCheatClient_OnMessageToServerCallbackInfo& data)
		: ClientData(data.ClientData)
		  , MessageData(UTF8_TO_TCHAR(data.MessageData))
		  , MessageDataSizeBytes(data.MessageDataSizeBytes)
	{
		
	}
public:
	/** Caller-specified context data */
	void* ClientData;
	/** The message data that must be sent to the server */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FString MessageData;
	/** The size in bytes of MessageData */
	int32 MessageDataSizeBytes;
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatClientAddNotifyClientIntegrityViolatedOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatClientAddNotifyClientIntegrityViolatedOptions()
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatClientAddNotifyMessageToServerOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatClientAddNotifyMessageToServerOptions()
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatClientAddNotifyMessageToPeerOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatClientAddNotifyMessageToPeerOptions()
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatClientAddNotifyPeerActionRequiredOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatClientAddNotifyPeerActionRequiredOptions()
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatClientAddNotifyPeerAuthStatusChangedOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatClientAddNotifyPeerAuthStatusChangedOptions()
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatClientBeginSessionOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatClientBeginSessionOptions()
		: Mode(EEOSEAntiCheatClientMode::EOS_ACCM_Invalid)
	{
	}
public:
	/** Logged in user identifier from earlier call to EOS_Connect_Login family of functions */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	FEOSProductUserId LocalUserId;
	/** Operating mode */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	EEOSEAntiCheatClientMode Mode;
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatClientEndSessionOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatClientEndSessionOptions()
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatClientPollStatusOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatClientPollStatusOptions()
		: OutMessageLength(1024)
	{
	}

public:
	/** The size of OutMessage in bytes. Recommended size is 256 bytes. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	int32 OutMessageLength;
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatClientAddExternalIntegrityCatalogOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatClientAddExternalIntegrityCatalogOptions()
	{
	}
public:
	/** UTF-8 path to the .bin catalog file to add */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	FString PathToBinFile;
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatClientReceiveMessageFromServerOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatClientReceiveMessageFromServerOptions()
	{
	}
public:
	/** The data received */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	TArray<uint8> Data;
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatClientGetProtectMessageOutputLengthOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatClientGetProtectMessageOutputLengthOptions()
		: DataLengthBytes(0)
	{
	}
public:
	/** Length in bytes of input */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	int32 DataLengthBytes;
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatClientProtectMessageOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatClientProtectMessageOptions()
		: OutBufferSizeBytes(0)
	{
	}
public:
	/** The data to encrypt */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	TArray<uint8> Data;
	/** The size in bytes of OutBuffer */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	int32 OutBufferSizeBytes;
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatClientUnprotectMessageOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatClientUnprotectMessageOptions()
		: OutBufferSizeBytes(0)
	{
	}
public:
	/** The data to decrypt */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	TArray<uint8> Data;
	/** The size in bytes of OutBuffer */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	int32 OutBufferSizeBytes;
};

/**
* A special peer handle that represents the client itself.
* It does not need to be registered or unregistered and is
* used in OnPeerActionRequiredCallback to quickly signal to the user
* that they will not be able to join online play.
*/
USTRUCT(BlueprintType)
struct FEOSAntiCheatClientRegisterPeerOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatClientRegisterPeerOptions()
		: PeerHandle(nullptr)
		  , ClientType(EEOSEAntiCheatCommonClientType::EOS_ACCCT_UnprotectedClient)
		  , ClientPlatform(EEOSEAntiCheatCommonClientPlatform::EOS_ACCCP_Unknown)
		, AuthenticationTimeout(60)
	{
	}
public:
	/** Locally unique value describing the remote user (e.g. a player object pointer) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	FEOSAntiCheatCommonClientHandle PeerHandle;
	/** Type of remote user being registered */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	EEOSEAntiCheatCommonClientType ClientType;
	/** Remote user's platform, if known */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	EEOSEAntiCheatCommonClientPlatform ClientPlatform;
	/**
	 * Time in seconds to allow newly registered peers to send the initial message containing their token.
	 * Recommended value: 60
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	int32 AuthenticationTimeout;
	/** Deprecated - use PeerProductUserId instead */
	FString AccountId_DEPRECATED;
	/** 
	* Optional IP address for the remote user. May be null if not available.
	* IPv4 format: "0.0.0.0"
	* IPv6 format: "0:0:0:0:0:0:0:0"
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	FString IpAddress;
	/** EOS_ProductUserId Identifier for the remote user */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	FEOSProductUserId PeerProductUserId;
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatClientUnregisterPeerOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatClientUnregisterPeerOptions()
		: PeerHandle(nullptr)
	{
	}
public:
	/** Locally unique value describing the remote user, as previously passed to EOS_AntiCheatClient_RegisterPeer */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	FEOSAntiCheatCommonClientHandle PeerHandle;
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatClientReceiveMessageFromPeerOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatClientReceiveMessageFromPeerOptions()
		: PeerHandle(nullptr)
	{
	}
public:
	/** The handle describing the sender of this message */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	FEOSAntiCheatCommonClientHandle PeerHandle;
	/** The data received */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	TArray<uint8> Data;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		DELEGATES
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAntiCheatClientOnMessageToServerCallback, const FEOSAntiCheatClientOnMessageToServerCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAntiCheatClientOnMessageToPeerCallback, const FEOSAntiCheatCommonOnMessageToClientCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAntiCheatClientOnPeerActionRequiredCallback, const FEOSAntiCheatCommonOnClientActionRequiredCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAntiCheatClientOnPeerAuthStatusChangedCallback, const FEOSAntiCheatCommonOnClientAuthStatusChangedCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAntiCheatClientOnMessageToServerCallbackDelegate, const FEOSAntiCheatClientOnMessageToServerCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAntiCheatClientOnMessageToPeerCallbackDelegate, const FEOSAntiCheatCommonOnMessageToClientCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAntiCheatClientOnPeerActionRequiredCallbackDelegate, const FEOSAntiCheatCommonOnClientActionRequiredCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAntiCheatClientOnPeerAuthStatusChangedCallbackDelegate, const FEOSAntiCheatCommonOnClientAuthStatusChangedCallbackInfo&, Data);

/**
 * Callback issued when the local client triggers an integrity violation.
 *
 * The message contains descriptive string of up to 256 characters and must be displayed to the player.
 *
 * This Callback is always issued from within EOS_Platform_Tick on its calling thread.
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAntiCheatClientOnClientIntegrityViolatedCallback, const FEOSAntiCheatClientOnClientIntegrityViolatedCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAntiCheatClientOnClientIntegrityViolatedDelegate, const FEOSAntiCheatClientOnClientIntegrityViolatedCallbackInfo&, Data);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		CALLBACK OBJECTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
struct FAntiCheatClientMessageToServer
{
public:
	FAntiCheatClientMessageToServer(const FOnAntiCheatClientOnMessageToServerCallback& Callback)
		: m_Callback(Callback)
	{
	}

	virtual ~FAntiCheatClientMessageToServer()
	{
		m_Callback.Unbind();
	}

public:
	FOnAntiCheatClientOnMessageToServerCallback m_Callback;
};

struct FAntiCheatClientMessageToPeer
{
public:
	FAntiCheatClientMessageToPeer(const FOnAntiCheatClientOnMessageToPeerCallback& Callback)
		: m_Callback(Callback)
	{
	}

	virtual ~FAntiCheatClientMessageToPeer()
	{
		m_Callback.Unbind();
	}

public:
	FOnAntiCheatClientOnMessageToPeerCallback m_Callback;
};

struct FAntiCheatClientPeerActionRequired
{
public:
	FAntiCheatClientPeerActionRequired(const FOnAntiCheatClientOnPeerActionRequiredCallback& Callback)
		: m_Callback(Callback)
	{
	}

	virtual ~FAntiCheatClientPeerActionRequired()
	{
		m_Callback.Unbind();
	}

public:
	FOnAntiCheatClientOnPeerActionRequiredCallback m_Callback;
};

struct FAntiCheatClientPeerAuthStatusChanged
{
public:
	FAntiCheatClientPeerAuthStatusChanged(const FOnAntiCheatClientOnPeerAuthStatusChangedCallback& Callback)
		: m_Callback(Callback)
	{
	}

	virtual ~FAntiCheatClientPeerAuthStatusChanged()
	{
		m_Callback.Unbind();
	}

public:
	FOnAntiCheatClientOnPeerAuthStatusChangedCallback m_Callback;
};

struct FAntiCheatClientOnClientIntegrityViolated
{
public:
	FAntiCheatClientOnClientIntegrityViolated(const FOnAntiCheatClientOnClientIntegrityViolatedCallback& Callback)
		: m_Callback(Callback)
	{
	}

	virtual ~FAntiCheatClientOnClientIntegrityViolated()
	{
		m_Callback.Unbind();
	}

public:
	FOnAntiCheatClientOnClientIntegrityViolatedCallback m_Callback;
};