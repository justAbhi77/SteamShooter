/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "eos_friends_types.h"
#include "Core/EOSHelpers.h"
#include "EOSFriendsTypes.generated.h"

class UCoreFriends;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		ENUMS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

UENUM(BlueprintType)
enum class EEOSEFriendsStatus : uint8
{
	/** The two accounts have no friendship status */
	EOS_FS_NotFriends = 0,
	/** The local account has sent a friend invite to the other account */
	EOS_FS_InviteSent = 1,
	/** The other account has sent a friend invite to the local account */
	EOS_FS_InviteReceived = 2,
	/** The accounts have accepted friendship */
	EOS_FS_Friends = 3
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		STRUCTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
/**
 * Input parameters for the EOS_Friends_QueryFriends Function.
 */
USTRUCT(BlueprintType)
struct FEOSFriendsQueryFriendsOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Account ID of the user whose friends should be asynchronously retrieved */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	FEOSEpicAccountId LocalUserId;
public:
	explicit FEOSFriendsQueryFriendsOptions()
		: ApiVersion(EOS_FRIENDS_QUERYFRIENDS_API_LATEST)
	{
	}
};

/**
 * Output parameters for the EOS_Friends_QueryFriends Function. These parameters are received through the callback provided to EOS_Friends_QueryFriends
 */
USTRUCT(BlueprintType)
struct FEOSFriendsQueryFriendsCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful query, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Friends_QueryFriends */
	void* ClientData;
	/** Account ID of the user whose friends were queried */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	FEOSEpicAccountId LocalUserId;
public:
	explicit FEOSFriendsQueryFriendsCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		, ClientData(nullptr)
	{
	}

	FEOSFriendsQueryFriendsCallbackInfo(const EOS_Friends_QueryFriendsCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		, ClientData(data.ClientData)
		, LocalUserId(data.LocalUserId)
	{
	}
};

/**
 * Input parameters for the EOS_Friends_SendInvite Function.
 */
USTRUCT(BlueprintType)
struct FEOSFriendsSendInviteOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Account ID of the user who is sending the friendship invitation */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	FEOSEpicAccountId LocalUserId;
	/** Account ID of the user who is receiving the friendship invitation */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	FEOSEpicAccountId TargetUserId;
public:
	explicit FEOSFriendsSendInviteOptions()
		: ApiVersion(EOS_FRIENDS_SENDINVITE_API_LATEST)
	{
	}
};

/**
 * Output parameters for the EOS_Friends_SendInvite API.
 */
USTRUCT(BlueprintType)
struct FEOSFriendsSendInviteCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned if the invitation was sent, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Friends_SendInvite */
	void* ClientData;
	/** Account ID of the user who sent the friendship invitation */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	FEOSEpicAccountId LocalUserId;
	/** Account ID of the user to whom the friendship invitation was sent */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	FEOSEpicAccountId TargetUserId;
public:
	explicit FEOSFriendsSendInviteCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		, ClientData(nullptr)
	{
	}

	FEOSFriendsSendInviteCallbackInfo(const EOS_Friends_SendInviteCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		, ClientData(data.ClientData)
		, LocalUserId(data.LocalUserId)
		, TargetUserId(data.TargetUserId)
	{
	}
};

/**
 * Input parameters for the EOS_Friends_AcceptInvite Function.
 */
USTRUCT(BlueprintType)
struct FEOSFriendsAcceptInviteOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Account ID of the user who is accepting the friendship invitation */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	FEOSEpicAccountId LocalUserId;
	/** Account ID of the user who sent the local user a friendship invitation */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	FEOSEpicAccountId TargetUserId;
public:
	explicit FEOSFriendsAcceptInviteOptions()
		: ApiVersion(EOS_FRIENDS_ACCEPTINVITE_API_LATEST)
	{
	}
};

/**
 * Output parameters for the EOS_Friends_AcceptInvite Function.
 */
USTRUCT(BlueprintType)
struct FEOSFriendsAcceptInviteCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned if the invitation was sent, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	EOSResult ResultCode;
	/** Context that was passed into to EOS_Friends_AcceptInvite */
	void* ClientData;
	/** Account ID of the user who is accepting the friendship invitation */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	FEOSEpicAccountId LocalUserId;
	/** Account ID of the user who sent the local user a friendship invitation */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	FEOSEpicAccountId TargetUserId;
public:
	explicit FEOSFriendsAcceptInviteCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		, ClientData(nullptr)
	{
	}

	FEOSFriendsAcceptInviteCallbackInfo(const EOS_Friends_AcceptInviteCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		, ClientData(data.ClientData)
		, LocalUserId(data.LocalUserId)
		, TargetUserId(data.TargetUserId)
	{
	}
};

/**
 * Input parameters for the EOS_Friends_RejectInvite Function.
 */
USTRUCT(BlueprintType)
struct FEOSFriendsRejectInviteOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Account ID of the user who is rejecting a friendship invitation */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	FEOSEpicAccountId LocalUserId;
	/** Account ID of the user who sent the friendship invitation */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	FEOSEpicAccountId TargetUserId;
public:
	explicit FEOSFriendsRejectInviteOptions()
		: ApiVersion(EOS_FRIENDS_REJECTINVITE_API_LATEST)
	{
	}
};

/**
 * Output parameters for the EOS_Friends_RejectInvite Function.
 */
USTRUCT(BlueprintType)
struct FEOSFriendsRejectInviteCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned if the invitation was sent, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	EOSResult ResultCode;
	/** Context that was passed into to EOS_Friends_RejectInvite */
	void* ClientData;
	/** Account ID of the user who is rejecting a friendship invitation */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	FEOSEpicAccountId LocalUserId;
	/** Account ID of the user who sent the friendship invitation */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	FEOSEpicAccountId TargetUserId;
public:
	explicit FEOSFriendsRejectInviteCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		, ClientData(nullptr)
	{
	}

	FEOSFriendsRejectInviteCallbackInfo(const EOS_Friends_RejectInviteCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		, ClientData(data.ClientData)
		, LocalUserId(data.LocalUserId)
		, TargetUserId(data.TargetUserId)
	{
	}
};

/**
 * Input parameters for the EOS_Friends_GetFriendsCount Function.
 */
USTRUCT(BlueprintType)
struct FEOSFriendsGetFriendsCountOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Account ID of the user whose friendships should be counted */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	FEOSEpicAccountId LocalUserId;
public:
	explicit FEOSFriendsGetFriendsCountOptions()
		: ApiVersion(EOS_FRIENDS_GETFRIENDSCOUNT_API_LATEST)
	{
	}
};

/**
 * Input parameters for the EOS_Friends_GetFriendAtIndex Function.
 */
USTRUCT(BlueprintType)
struct FEOSFriendsGetFriendAtIndexOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Account ID of the user whose friend list is being queried */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	FEOSEpicAccountId LocalUserId;
	/** Index into the friend list. This value must be between 0 and EOS_Friends_GetFriendsCount-1 inclusively. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	int32 Index;
public:
	explicit FEOSFriendsGetFriendAtIndexOptions()
		: ApiVersion(EOS_FRIENDS_GETFRIENDATINDEX_API_LATEST)
		, Index(0)
	{
	}
};

/**
 * Input parameters for the EOS_Friends_GetBlockedUsersCount function.
 */
USTRUCT(BlueprintType)
struct FEOSFriendsGetBlockedUsersCountOptions
{
	GENERATED_BODY()
public:
	/** The Epic Account ID of the user whose blocked users should be counted. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	FEOSEpicAccountId LocalUserId;
};

/**
 * Input parameters for the EOS_Friends_GetStatus Function.
 */
USTRUCT(BlueprintType)
struct FEOSFriendsGetBlockedUserAtIndexOptions
{
	GENERATED_BODY()
public:
	/** The Epic Account ID of the user whose blocked users list is being queried. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	FEOSEpicAccountId LocalUserId;
	/** Index into the blocked users list. This value must be between 0 and EOS_Friends_GetBlockedUsersCount-1 inclusively. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	int32 Index;
public:
	FEOSFriendsGetBlockedUserAtIndexOptions()
		: Index(0)
	{
	}
};

/**
 * Input parameters for the EOS_Friends_GetStatus Function.
 */
USTRUCT(BlueprintType)
struct FEOSFriendsGetStatusOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Account ID of the local, logged in user */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	FEOSEpicAccountId LocalUserId;
	/** Account ID of the user whose friendship status with the local user is being queried */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	FEOSEpicAccountId TargetUserId;
public:
	explicit FEOSFriendsGetStatusOptions()
		: ApiVersion(EOS_FRIENDS_GETSTATUS_API_LATEST)
	{
	}
};

/** Input parameters for the EOS_Friends_AddNotifyFriendsUpdate Function.  */
USTRUCT(BlueprintType)
struct FEOSFriendsAddNotifyFriendsUpdateOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	explicit FEOSFriendsAddNotifyFriendsUpdateOptions()
		: ApiVersion(EOS_FRIENDS_ADDNOTIFYFRIENDSUPDATE_API_LATEST)
	{
	}
};

/**
 * Structure containing information about a blocklist update.
 */
USTRUCT(BlueprintType)
struct FEOSFriendsOnBlockedUsersUpdateInfo
{
	GENERATED_BODY()
public:
	/** Client-specified data passed into EOS_Friends_AddNotifyFriendsUpdate */
	void* ClientData;
	/** The Epic Account ID of the local user who is receiving the update */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	FEOSEpicAccountId LocalUserId;
	/** The Epic Account ID of the user whose blocked status is being updated. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	FEOSEpicAccountId TargetUserId;
	/** TargetUserId block status (blocked or not). */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	bool bBlocked;
public:
	FEOSFriendsOnBlockedUsersUpdateInfo()
		: ClientData(nullptr)
		, bBlocked(false)
	{
	}

	FEOSFriendsOnBlockedUsersUpdateInfo(const EOS_Friends_OnBlockedUsersUpdateInfo& Data)
		: ClientData(Data.ClientData)
		, LocalUserId(Data.LocalUserId)
		, TargetUserId(Data.TargetUserId)
		, bBlocked(Data.bBlocked > 0)
	{
	}
};

/**
 * Structure containing information about a friend status update.
 */
USTRUCT(BlueprintType)
struct FEOSFriendsOnFriendsUpdateInfo
{
	GENERATED_BODY()
public:
	/** Client-specified data passed into EOS_Friends_AddNotifyFriendsUpdate */
	void* ClientData;
	/** The local user who is being notified of the friends update. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	FEOSEpicAccountId LocalUserId;
	/** The target user whose status is being updated. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	FEOSEpicAccountId TargetUserId;
	/** The previous status of the friend. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	EEOSEFriendsStatus PreviousStatus;
	/** The current status of the friend. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Friends")
	EEOSEFriendsStatus CurrentStatus;
public:
	explicit FEOSFriendsOnFriendsUpdateInfo()
		: ClientData(nullptr)
		, PreviousStatus(EEOSEFriendsStatus::EOS_FS_NotFriends)
		, CurrentStatus(EEOSEFriendsStatus::EOS_FS_NotFriends)
	{
	}

	FEOSFriendsOnFriendsUpdateInfo(const EOS_Friends_OnFriendsUpdateInfo& data)
		: ClientData(data.ClientData)
		, LocalUserId(data.LocalUserId)
		, TargetUserId(data.TargetUserId)
		, PreviousStatus(static_cast<EEOSEFriendsStatus>(data.PreviousStatus))
		, CurrentStatus(static_cast<EEOSEFriendsStatus>(data.CurrentStatus))
	{
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		DELEGATES
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnFriendsQueryFriendsCallback, const FEOSFriendsQueryFriendsCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnFriendsSendInviteCallback, const FEOSFriendsSendInviteCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnFriendsAcceptInviteCallback, const FEOSFriendsAcceptInviteCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnFriendsRejectInviteCallback, const FEOSFriendsRejectInviteCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnFriendsUpdateCallback, const FEOSFriendsOnFriendsUpdateInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnFriendsBlockedUsersUpdateCallback, const FEOSFriendsOnBlockedUsersUpdateInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFriendsQueryFriendsCallbackDelegate, const FEOSFriendsQueryFriendsCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFriendsSendInviteCallbackDelegate, const FEOSFriendsSendInviteCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFriendsAcceptInviteCallbackDelegate, const FEOSFriendsAcceptInviteCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFriendsRejectInviteCallbackDelegate, const FEOSFriendsRejectInviteCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFriendsUpdateCallbackDelegate, const FEOSFriendsOnFriendsUpdateInfo&, Data);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		CALLBACK OBJECTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

struct FFriendsQueryFriendsCallback
{
public:
	FOnFriendsQueryFriendsCallback m_Callback;
public:
	FFriendsQueryFriendsCallback(const FOnFriendsQueryFriendsCallback& callback)
		: m_Callback(callback)
	{
	}
	virtual ~FFriendsQueryFriendsCallback()
	{
		m_Callback.Unbind();
	}
};

struct FFriendsSendInviteCallback
{
public:
	FOnFriendsSendInviteCallback m_Callback;
public:
	FFriendsSendInviteCallback(const FOnFriendsSendInviteCallback& callback)
		: m_Callback(callback)
	{
	}
	virtual ~FFriendsSendInviteCallback()
	{
		m_Callback.Unbind();
	}
};

struct FFriendsAcceptInviteCallback
{
public:
	FOnFriendsAcceptInviteCallback m_Callback;
public:
	FFriendsAcceptInviteCallback(const FOnFriendsAcceptInviteCallback& callback)
		: m_Callback(callback)
	{
	}
	virtual ~FFriendsAcceptInviteCallback()
	{
		m_Callback.Unbind();
	}
};

struct FFriendsRejectInviteCallback
{
public:
	FOnFriendsRejectInviteCallback m_Callback;
public:
	FFriendsRejectInviteCallback(const FOnFriendsRejectInviteCallback& callback)
		: m_Callback(callback)
	{
	}
	virtual ~FFriendsRejectInviteCallback()
	{
		m_Callback.Unbind();
	}
};

struct FFriendsUpdateCallback
{
public:
	FFriendsUpdateCallback(UObject* WorldContextObject, const FOnFriendsUpdateCallback& callback)
		: WorldContextObject(WorldContextObject), Callback(callback)
	{
	}
	virtual ~FFriendsUpdateCallback()
	{
		Callback.Unbind();
	}
public:
	UObject* WorldContextObject;
	FOnFriendsUpdateCallback Callback;
};

struct FFriendsOnBlockedUsersUpdateCallback
{
public:
	FFriendsOnBlockedUsersUpdateCallback(UObject* WorldContextObject, const FOnFriendsBlockedUsersUpdateCallback& callback)
		: WorldContextObject(WorldContextObject), Callback(callback)
	{
	}
	virtual ~FFriendsOnBlockedUsersUpdateCallback()
	{
		Callback.Unbind();
	}
public:
	UObject* WorldContextObject;
	FOnFriendsBlockedUsersUpdateCallback Callback;
};