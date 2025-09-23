/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Core/EOSHelpers.h"
#include "Core/EOSTypes.h"
#include "EOSRTCAdminTypes.generated.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		STRUCTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

/**
* Input parameters for the EOS_RTCAdmin_QueryJoinRoomToken function.
*/
USTRUCT(BlueprintType)
struct FEOSQueryJoinRoomTokenOptions
{
	GENERATED_BODY()
public:
	/** Product User ID for local user who is querying join room tokens. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId LocalUserId;
	/** Room name to request a token for. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString RoomName;
	/** An array of Product User IDs indicating the users to retrieve a token for. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	TArray<FEOSProductUserId> TargetUserIds;
	/**
	* Array of IP Addresses, one for each of the users we're querying tokens for.
	* There should be TargetUserIdsCount Ip Addresses, you can set an entry to NULL if not known.
	* If TargetUserIpAddresses is set to NULL IP Addresses will be ignored.
	* IPv4 format: "0.0.0.0"
	* IPv6 format: "0:0:0:0:0:0:0:0"
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	TArray<FString> TargetUserIpAddresses;
public:
	explicit FEOSQueryJoinRoomTokenOptions() = default;
};

/**
* Data containing the result information for a query join room token request.
*/
USTRUCT(BlueprintType)
struct FEOSQueryJoinRoomTokenCompleteCallbackInfo
{
	GENERATED_BODY()
public:
	/** This returns:
	* EOS_Success if the channel was successfully blocked.
	* EOS_UnexpectedError otherwise.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EOSResult ResultCode;
	/** Client-specified data passed into EOS_RTCAudio_UpdateSending. */
	void* ClientData;
	/** The room this settings should be applied on. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString RoomName;
	/** URL passed to backend to join room. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString ClientBaseUrl;
	/** If the query completed successfully, this contains an identifier that should be used to retrieve the tokens.
	* This identifier is only valid for the duration of the callback. 
	*
	* @see EOS_RTCAdmin_CopyUserTokenByIndex
	* @see EOS_RTCAdmin_CopyUserTokenByUserId
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	int32 QueryId;
	/** How many token received as result of the query */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	int32 TokenCount;
public:
	FEOSQueryJoinRoomTokenCompleteCallbackInfo()
	: ResultCode(EOSResult::EOS_UnexpectedError)
	, ClientData(nullptr)
	, QueryId(0)
	, TokenCount(0)
	{}

	FEOSQueryJoinRoomTokenCompleteCallbackInfo(const EOS_RTCAdmin_QueryJoinRoomTokenCompleteCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		, ClientData(data.ClientData)
		, RoomName(UTF8_TO_TCHAR(data.RoomName))
		, ClientBaseUrl(UTF8_TO_TCHAR(data.ClientBaseUrl))
		, QueryId(data.QueryId)
		, TokenCount(data.TokenCount)
	{
	}
};

/**
* Contains information about a collection of user tokens for joining a room.
*/
USTRUCT(BlueprintType)
struct FEOSUserToken
{
	GENERATED_BODY()
public:
	/** The Product User ID for the user who owns this user token. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId ProductUserId;
	/** Access token to enable a user to join a room */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString Token;
public:
	explicit FEOSUserToken() = default;

	FEOSUserToken(const EOS_RTCAdmin_UserToken& data)
		: ProductUserId(data.ProductUserId)
		, Token(UTF8_TO_TCHAR(data.Token))
	{
	}
};

/**
* Input parameters for the EOS_RTCAdmin_CopyUserTokenByIndex function.
*/
USTRUCT(BlueprintType)
struct FEOSCopyUserTokenByIndexOptions
{
	GENERATED_BODY()
public:
	/** Index of the user token to retrieve from the cache. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	int32 UserTokenIndex;
	/** Query identifier received as part of a previous query.
	* @see EOS_RTCAdmin_QueryJoinRoomTokenCompleteCallbackInfo
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	int32 QueryId;
public:
	FEOSCopyUserTokenByIndexOptions()
		: UserTokenIndex(0)
		, QueryId(0)
	{}
};

/**
* Input parameters for the EOS_RTCAdmin_CopyUserTokenByUserId function.
*/
USTRUCT(BlueprintType)
struct FEOSCopyUserTokenByUserIdOptions
{
	GENERATED_BODY()
public:
	/** The Product User ID for the user whose user token we're copying. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId TargetUserId;
	/** Query identifier received as part of a previous query.
	* @see EOS_RTCAdmin_QueryJoinRoomTokenCompleteCallbackInfo
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	int32 QueryId;
public:
	FEOSCopyUserTokenByUserIdOptions()
		: QueryId(0)
	{}
};

/**
* Input parameters for the EOS_RTCAdmin_Kick function.
*/
USTRUCT(BlueprintType)
struct FEOSKickOptions
{
	GENERATED_BODY()
public:
	/** Room name to kick the participant from */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString RoomName;
	/** Product User ID of the participant to kick from the room */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId TargetUserId;
public:
	explicit FEOSKickOptions() = default;
};

/**
* Data containing the result information for a kick participant request.
*/
USTRUCT(BlueprintType)
struct FEOSKickCompleteCallbackInfo
{
	GENERATED_BODY()
public:
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EOSResult ResultCode;
	/** Client-specified data passed into the kick request */
	void* ClientData;
public:
	FEOSKickCompleteCallbackInfo()
		: ResultCode(EOSResult::MAX)
		, ClientData(nullptr) {}

	FEOSKickCompleteCallbackInfo(const EOS_RTCAdmin_KickCompleteCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		, ClientData(data.ClientData)
	{
	}
};

/**
* Input parameters for the EOS_RTCAdmin_SetParticipantHardMute function.
*/
USTRUCT(BlueprintType)
struct FEOSSetParticipantHardMuteOptions
{
	GENERATED_BODY()
public:
	/** Room to kick the participant from */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString RoomName;
	/** Product User ID of the participant to hard mute for every participant in the room. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId TargetUserId;
	/** Hard mute status (Mute on or off) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	bool bMute;
public:
	FEOSSetParticipantHardMuteOptions()
		: bMute(false)
	{}
};

/**
* Data containing the result information for a hard mute request.
*/
USTRUCT(BlueprintType)
struct FEOSSetParticipantHardMuteCompleteCallbackInfo
{
	GENERATED_BODY()
public:
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EOSResult ResultCode;
	/** Client-specified data passed into the kick request */
	void* ClientData;
public:
	FEOSSetParticipantHardMuteCompleteCallbackInfo()
		: ResultCode(EOSResult::MAX)
		, ClientData(nullptr) {}

	FEOSSetParticipantHardMuteCompleteCallbackInfo(const EOS_RTCAdmin_SetParticipantHardMuteCompleteCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		, ClientData(data.ClientData)
	{
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		DELEGATES
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnQueryJoinRoomTokenCompleteCallback, const FEOSQueryJoinRoomTokenCompleteCallbackInfo&, data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnKickCompleteCallback, const FEOSKickCompleteCallbackInfo&, data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSetParticipantHardMuteCompleteCallback, const FEOSSetParticipantHardMuteCompleteCallbackInfo&, data);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		CALLBACK OBJECTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

struct FQueryJoinRoomTokenCallback
{
public:
	FOnQueryJoinRoomTokenCompleteCallback m_Callback;
public:
	FQueryJoinRoomTokenCallback(const FOnQueryJoinRoomTokenCompleteCallback& callback)
		: m_Callback(callback)
	{
	}
	virtual ~FQueryJoinRoomTokenCallback()
	{
		m_Callback.Unbind();
	}
};

struct FKickCallback
{
public:
	FOnKickCompleteCallback m_Callback;
public:
	FKickCallback(const FOnKickCompleteCallback& callback)
		: m_Callback(callback)
	{
	}
	virtual ~FKickCallback()
	{
		m_Callback.Unbind();
	}
};

struct FSetParticipantHardMuteCallback
{
public:
	FOnSetParticipantHardMuteCompleteCallback m_Callback;
public:
	FSetParticipantHardMuteCallback(const FOnSetParticipantHardMuteCompleteCallback& callback)
		: m_Callback(callback)
	{
	}
	virtual ~FSetParticipantHardMuteCallback()
	{
		m_Callback.Unbind();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		REQUEST OBJECTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

struct FKickOptions : public EOS_RTCAdmin_KickOptions
{
	FKickOptions(FString roomName)
		: EOS_RTCAdmin_KickOptions()
	{
		ApiVersion = EOS_RTCADMIN_KICK_API_LATEST;

		if (!roomName.IsEmpty())
		{
			RoomName = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(RoomName), TCHAR_TO_UTF8(*roomName), 256);
		}
	}

	~FKickOptions()
	{
		delete[] RoomName;
	}
};

struct FSetParticipantHardMuteOptions : public EOS_RTCAdmin_SetParticipantHardMuteOptions
{
	FSetParticipantHardMuteOptions(FString roomName)
		: EOS_RTCAdmin_SetParticipantHardMuteOptions()
	{
		ApiVersion = EOS_RTCADMIN_SETPARTICIPANTHARDMUTE_API_LATEST;

		if (!roomName.IsEmpty())
		{
			RoomName = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(RoomName), TCHAR_TO_UTF8(*roomName), 256);
		}
	}

	~FSetParticipantHardMuteOptions()
	{
		delete[] RoomName;
	}
};