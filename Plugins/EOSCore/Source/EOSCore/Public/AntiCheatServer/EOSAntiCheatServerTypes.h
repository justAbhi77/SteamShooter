/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "eos_anticheatserver_types.h"
#include "Core/EOSCoreAntiCheatCommon.h"
#include "Core/EOSTypes.h"
#include "EOSAntiCheatServerTypes.generated.h"

class UCoreAntiCheatServer;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		STRUCTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

USTRUCT(BlueprintType)
struct FEOSAntiCheatServerAddNotifyMessageToClientOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatServerAddNotifyMessageToClientOptions()
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatServerAddNotifyClientActionRequiredOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatServerAddNotifyClientActionRequiredOptions()
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatServerAddNotifyClientAuthStatusChangedOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatServerAddNotifyClientAuthStatusChangedOptions()
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatServerBeginSessionOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatServerBeginSessionOptions()
		: RegisterTimeoutSeconds(60)
		  , bEnableGameplayData(false)
	{
	}

public:
	/** 
	* Time in seconds to allow newly registered clients to complete anti-cheat authentication.
	* Recommended value: 60
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	int32 RegisterTimeoutSeconds;
	/** Optional name of this game server */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	FString ServerName;
	/** 
	* Gameplay data collection APIs such as LogPlayerTick will be enabled if set to true.
	* If you do not use these APIs, it is more efficient to set this value to false.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	bool bEnableGameplayData;
	/** The Product User ID of the local user who is associated with this session. Dedicated servers should set this to null. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	FEOSProductUserId LocalUserId;
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatServerEndSessionOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatServerEndSessionOptions()
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatServerRegisterClientOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatServerRegisterClientOptions()
		: ClientHandle(nullptr)
		  , ClientType(EEOSEAntiCheatCommonClientType::EOS_ACCCT_UnprotectedClient)
		  , ClientPlatform(EEOSEAntiCheatCommonClientPlatform::EOS_ACCCP_Unknown)
	{
	}

public:
	/** Locally unique value describing the remote user (e.g. a player object pointer) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	FEOSAntiCheatCommonClientHandle ClientHandle;
	/** Type of remote user being registered */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	EEOSEAntiCheatCommonClientType ClientType;
	/** Remote user's platform, if known */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	EEOSEAntiCheatCommonClientPlatform ClientPlatform;
	/** 
	* Identifier for the remote user. This is typically a string representation of an
	* account ID, but it can be any string which is both unique (two different users will never
	* have the same string) and consistent (if the same user connects to this game session
	* twice, the same string will be used) in the scope of a single protected game session.
	*/
	FString AccountId_DEPRECATED;
	/** 
	* Optional IP address for the remote user. May be null if not available.
	* IPv4 format: "0.0.0.0"
	* IPv6 format: "0:0:0:0:0:0:0:0"
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	FString IpAddress;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	FEOSProductUserId UserId;
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatServerUnregisterClientOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatServerUnregisterClientOptions()
		: ClientHandle(nullptr)
	{
	}

public:
	/** Locally unique value describing the remote user, as previously passed to RegisterClient */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	FEOSAntiCheatCommonClientHandle ClientHandle;
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatServerReceiveMessageFromClientOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatServerReceiveMessageFromClientOptions()
		: ClientHandle(nullptr)
	{
	}

public:
	/** Optional value, if non-null then only messages addressed to this specific client will be returned */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	FEOSAntiCheatCommonClientHandle ClientHandle;
	/** The data received */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	TArray<uint8> Data;
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatServerSetClientNetworkStateOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatServerSetClientNetworkStateOptions()
		: ClientHandle(nullptr)
		  , bIsNetworkActive(false)
	{
	}

public:
	/** Locally unique value describing the remote user (e.g. a player object pointer) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	FEOSAntiCheatCommonClientHandle ClientHandle;
	/** True if the network is functioning normally, false if temporarily interrupted */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	bool bIsNetworkActive;
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatServerGetProtectMessageOutputLengthOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatServerGetProtectMessageOutputLengthOptions()
		: DataLengthBytes(0)
	{
	}

public:
	/** Length in bytes of input */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	int32 DataLengthBytes;
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatServerProtectMessageOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatServerProtectMessageOptions()
		: ClientHandle(nullptr)
		  , DataLengthBytes(0)
		  , OutBufferSizeBytes(0)
	{
	}

public:
	/** Locally unique value describing the remote user to whom the message will be sent */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	FEOSAntiCheatCommonClientHandle ClientHandle;
	/** Length in bytes of input */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	int32 DataLengthBytes;
	/** The data to encrypt */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	TArray<uint8> Data;
	/** The size in bytes of OutBuffer */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	int32 OutBufferSizeBytes;
};

USTRUCT(BlueprintType)
struct FEOSAntiCheatServerUnprotectMessageOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAntiCheatServerUnprotectMessageOptions()
		: ClientHandle(nullptr)
		  , DataLengthBytes(0)
		  , OutBufferSizeBytes(0)
	{
	}

public:
	/** Locally unique value describing the remote user to whom the message will be sent */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	FEOSAntiCheatCommonClientHandle ClientHandle;
	/** Length in bytes of input */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	int32 DataLengthBytes;
	/** The data to encrypt */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	TArray<uint8> Data;
	/** The size in bytes of OutBuffer */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AntiCheatClient")
	int32 OutBufferSizeBytes;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		DELEGATES
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
/**
* Callback issued when a new message must be dispatched to a connected client.
*
* Messages contain opaque binary data of up to 256 bytes and must be transmitted
* to the correct client using the game's own networking layer, then delivered
* to the client anti-cheat instance using the EOS_AntiCheatClient_ReceiveMessageFromServer function.
*
* This callback is always issued from within EOS_Platform_Tick on its calling thread.
*/
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAntiCheatServerOnMessageToClientCallback, const FEOSAntiCheatCommonOnMessageToClientCallbackInfo&, data);

/**
* Callback issued when an action must be applied to a connected client.
* This callback is always issued from within EOS_Platform_Tick on its calling thread.
*/
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAntiCheatServerOnClientActionRequiredCallback, const FEOSAntiCheatCommonOnClientActionRequiredCallbackInfo&, data);

/**
* Optional callback issued when a connected client's authentication status has changed.
* This callback is always issued from within EOS_Platform_Tick on its calling thread.
*/
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAntiCheatServerOnClientAuthStatusChangedCallback, const FEOSAntiCheatCommonOnClientAuthStatusChangedCallbackInfo&, data);

/**
* Callback issued when a new message must be dispatched to a connected client.
*
* Messages contain opaque binary data of up to 256 bytes and must be transmitted
* to the correct client using the game's own networking layer, then delivered
* to the client anti-cheat instance using the EOS_AntiCheatClient_ReceiveMessageFromServer function.
*
* This callback is always issued from within EOS_Platform_Tick on its calling thread.
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAntiCheatServerOnMessageToClientCallbackDelegate, const FEOSAntiCheatCommonOnMessageToClientCallbackInfo&, data);

/**
* Callback issued when an action must be applied to a connected client.
* This callback is always issued from within EOS_Platform_Tick on its calling thread.
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAntiCheatServerOnClientActionRequiredCallbackDelegate, const FEOSAntiCheatCommonOnClientActionRequiredCallbackInfo&, data);

/**
* Optional callback issued when a connected client's authentication status has changed.
* This callback is always issued from within EOS_Platform_Tick on its calling thread.
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAntiCheatServerOnClientAuthStatusChangedCallbackDelegate, const FEOSAntiCheatCommonOnClientAuthStatusChangedCallbackInfo&, data);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		CALLBACK OBJECTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
struct FAntiCheatServerMessageToClient
{
public:
	FAntiCheatServerMessageToClient(const FOnAntiCheatServerOnMessageToClientCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FAntiCheatServerMessageToClient()
	{
		m_Callback.Unbind();
	}

public:
	FOnAntiCheatServerOnMessageToClientCallback m_Callback;
};

struct FAntiCheatServerActionRequired
{
public:
	FAntiCheatServerActionRequired(const FOnAntiCheatServerOnClientActionRequiredCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FAntiCheatServerActionRequired()
	{
		m_Callback.Unbind();
	}

public:
	FOnAntiCheatServerOnClientActionRequiredCallback m_Callback;
};

struct FAntiCheatServerClientAuthStatusChanged
{
public:
	FAntiCheatServerClientAuthStatusChanged(const FOnAntiCheatServerOnClientAuthStatusChangedCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FAntiCheatServerClientAuthStatusChanged()
	{
		m_Callback.Unbind();
	}

public:
	FOnAntiCheatServerOnClientAuthStatusChangedCallback m_Callback;
};
