/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "eos_custominvites_types.h"
#include "Core/EOSTypes.h"
#include "Core/EOSHelpers.h"
#include "EOSCustomInvitesTypes.generated.h"


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		ENUMS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

/** Response to an invite request. */
UENUM(BlueprintType)
enum class EEOSERequestToJoinResponse : uint8
{
	/** The target of the invite request has accepted. */
	EOS_RTJR_ACCEPTED = 0,
	/** The target of the invite request has rejected. */
	EOS_RTJR_REJECTED = 1
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		STRUCTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

USTRUCT(BlueprintType)
struct FEOSCustomInvitesSetCustomInviteOptions
{
	GENERATED_BODY()
public:
	/** Local user creating / sending a Custom Invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId LocalUserId;
	/** String payload for the Custom Invite (must be less than EOS_CUSTOMINVITES_MAX_PAYLOAD_LENGTH) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FString Payload;
public:
	explicit FEOSCustomInvitesSetCustomInviteOptions() = default;
};

USTRUCT(BlueprintType)
struct FEOSCustomInvitesSendCustomInviteOptions
{
	GENERATED_BODY()
public:
	/** Local user sending a CustomInvite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId LocalUserId;
	/** Users to whom the invites should be sent */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	TArray<FEOSProductUserId> TargetUserIds;
public:
	explicit FEOSCustomInvitesSendCustomInviteOptions() = default;
};

USTRUCT(BlueprintType)
struct FEOSCustomInvitesAddNotifyCustomInviteReceivedOptions
{
	GENERATED_BODY()
public:
	explicit FEOSCustomInvitesAddNotifyCustomInviteReceivedOptions() = default;
};

/**
 * Output parameters for the EOS_CustomInvites_OnCustomInviteReceivedCallback Function.
 */
USTRUCT(BlueprintType)
struct FEOSCustomInvitesOnCustomInviteReceivedCallbackInfo
{
	GENERATED_BODY()
public:
	/** Context that was passed into EOS_CustomInvites_AddNotifyCustomInviteReceived */
	void* ClientData;
	/** User that sent this custom invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId TargetUserId;
	/** Recipient Local user id */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId LocalUserId;
	/** Id of the received Custom Invite*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FString CustomInviteId;
	/** Payload of the received Custom Invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FString Payload;
public:
	explicit FEOSCustomInvitesOnCustomInviteReceivedCallbackInfo()
		: ClientData(nullptr)
	{
	}

	FEOSCustomInvitesOnCustomInviteReceivedCallbackInfo(const EOS_CustomInvites_OnCustomInviteReceivedCallbackInfo& data)
		: ClientData(data.ClientData)
		, TargetUserId(data.TargetUserId)
		, LocalUserId(data.LocalUserId)
	{
		const FUTF8ToTCHAR CustomInviteIdChar(data.CustomInviteId);
		CustomInviteId = CustomInviteIdChar.Get();

		const FUTF8ToTCHAR PayloadChar(data.Payload);
		Payload = PayloadChar.Get();
	}
};

USTRUCT(BlueprintType)
struct FEOSCustomInvitesAddNotifyCustomInviteAcceptedOptions
{
	GENERATED_BODY()
public:
	explicit FEOSCustomInvitesAddNotifyCustomInviteAcceptedOptions() = default;
};

/**
 * Output parameters for the EOS_CustomInvites_OnCustomInviteReceivedCallback Function.
 */
USTRUCT(BlueprintType)
struct FEOSCustomInvitesOnCustomInviteAcceptedCallbackInfo
{
	GENERATED_BODY()
public:
	/** Context that was passed into EOS_CustomInvites_AddNotifyCustomInviteAccepted */
	void* ClientData;
	/** User that sent this custom invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId TargetUserId;
	/** Recipient Local user id */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId LocalUserId;
	/** Id of the received Custom Invite*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FString CustomInviteId;
	/** Payload of the received Custom Invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FString Payload;
public:
	explicit FEOSCustomInvitesOnCustomInviteAcceptedCallbackInfo()
		: ClientData(nullptr)
	{
	}

	FEOSCustomInvitesOnCustomInviteAcceptedCallbackInfo(const EOS_CustomInvites_OnCustomInviteAcceptedCallbackInfo& data)
		: ClientData(data.ClientData)
		, TargetUserId(data.TargetUserId)
		, LocalUserId(data.LocalUserId)
	{
		const FUTF8ToTCHAR CustomInviteIdChar(data.CustomInviteId);
		CustomInviteId = CustomInviteIdChar.Get();

		const FUTF8ToTCHAR PayloadChar(data.Payload);
		Payload = PayloadChar.Get();
	}
};

USTRUCT(BlueprintType)
struct FEOSCustomInvitesAddNotifyCustomInviteRejectedOptions
{
	GENERATED_BODY()
public:
	explicit FEOSCustomInvitesAddNotifyCustomInviteRejectedOptions() = default;
};

/**
 * Output parameters for the EOS_CustomInvites_CustomInviteRejectedCallback Function.
 */
USTRUCT(BlueprintType)
struct FEOSCustomInvitesCustomInviteRejectedCallbackInfo
{
	GENERATED_BODY()
public:
	/** Context that was passed into EOS_CustomInvites_AddNotifyCustomInviteAccepted */
	void* ClientData;
	/** User that sent the custom invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId TargetUserId;
	/** Recipient Local user id */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId LocalUserId;
	/** Id of the rejected Custom Invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FString CustomInviteId;
	/** Payload of the rejected Custom Invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FString Payload;
public:
	explicit FEOSCustomInvitesCustomInviteRejectedCallbackInfo()
		: ClientData(nullptr)
	{
	}

	FEOSCustomInvitesCustomInviteRejectedCallbackInfo(const EOS_CustomInvites_CustomInviteRejectedCallbackInfo& data)
		: ClientData(data.ClientData)
		, TargetUserId(data.TargetUserId)
		, LocalUserId(data.LocalUserId)
	{
		const FUTF8ToTCHAR CustomInviteIdChar(data.CustomInviteId);
		CustomInviteId = CustomInviteIdChar.Get();

		const FUTF8ToTCHAR PayloadChar(data.Payload);
		Payload = PayloadChar.Get();
	}
};

/**
 * Output parameters for the EOS_CustomInvites_SendCustomInvite Function. These parameters are received through the callback provided to EOS_CustomInvites_SendCustomInvite
 */
USTRUCT(BlueprintType)
struct FEOSCustomInvitesOnSendCustomInviteCallbackInfo
{
	GENERATED_BODY()
public:
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	EOSResult ResultCode;
	/** Context that was passed into EOS_CustomInvites_SendCustomInvite */
	void* ClientData;
	/** Local user sending a CustomInvite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId LocalUserId;
	/** Users to whom the invites were successfully sent (can be different than original call if an invite for same Payload was previously sent) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	TArray<FEOSProductUserId> TargetUserIds;
public:
	explicit FEOSCustomInvitesOnSendCustomInviteCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		, ClientData(nullptr)
	{
	}

	FEOSCustomInvitesOnSendCustomInviteCallbackInfo(const EOS_CustomInvites_SendCustomInviteCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		, ClientData(data.ClientData)
		, LocalUserId(data.LocalUserId)
	{
		for (uint32_t i=0; i<data.TargetUserIdsCount; i++)
		{
			TargetUserIds.Add(data.TargetUserIds[i]);
		}
	}
};

USTRUCT(BlueprintType)
struct FEOSCustomInvitesRequestToJoinReceivedCallbackInfo
{
	GENERATED_BODY()
public:
	/** Context that was passed into EOS_CustomInvites_AddNotifyRequestToJoinReceived */
	void* ClientData;
	/** User that sent this response */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId FromUserId;
	/** Recipient Local user id */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId ToUserId;
public:
	FEOSCustomInvitesRequestToJoinReceivedCallbackInfo()
		: ClientData(nullptr)
	{
	}

	FEOSCustomInvitesRequestToJoinReceivedCallbackInfo(const EOS_CustomInvites_RequestToJoinReceivedCallbackInfo& data)
		: ClientData(data.ClientData)
		, FromUserId(data.FromUserId)
		, ToUserId(data.ToUserId)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSCustomInvitesRequestToJoinResponseReceivedCallbackInfo
{
	GENERATED_BODY()
public:
	/** Context that was passed into EOS_CustomInvites_AddNotifyRequestToJoinResponseReceived */
	void* ClientData;
	/** User that sent this response */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId FromUserId;
	/** Recipient Local user id */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId ToUserId;
	/** The Intent associated with this response */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	EEOSERequestToJoinResponse Response;
public:
	FEOSCustomInvitesRequestToJoinResponseReceivedCallbackInfo()
		: ClientData(nullptr)
		, Response(EEOSERequestToJoinResponse::EOS_RTJR_REJECTED)
	{
	}

	FEOSCustomInvitesRequestToJoinResponseReceivedCallbackInfo(const EOS_CustomInvites_RequestToJoinResponseReceivedCallbackInfo& data)
		: ClientData(data.ClientData)
		, FromUserId(data.FromUserId)
		, ToUserId(data.ToUserId)
		, Response(static_cast<EEOSERequestToJoinResponse>(data.Response))
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSCustomInvitesAcceptRequestToJoinCallbackInfo
{
	GENERATED_BODY()
public:
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	EOSResult ResultCode;
	/** Context that was passed into EOS_CustomInvites_AcceptRequestToJoin */
	void* ClientData;
	/** Local user accepting an invite request */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId LocalUserId;
	/** Target user that sent original invite request */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId TargetUserId;
public:
	explicit FEOSCustomInvitesAcceptRequestToJoinCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		, ClientData(nullptr)
	{
	}

	FEOSCustomInvitesAcceptRequestToJoinCallbackInfo(const EOS_CustomInvites_AcceptRequestToJoinCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		, ClientData(data.ClientData)
		, LocalUserId(data.LocalUserId)
		, TargetUserId(data.TargetUserId)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSCustomInvitesRejectRequestToJoinCallbackInfo
{
	GENERATED_BODY()
public:
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	EOSResult ResultCode;
	/** Context that was passed into EOS_CustomInvites_RejectRequestToJoin */
	void* ClientData;
	/** Local user declining a request to join */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId LocalUserId;
	/** Target user that sent original request to join */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId TargetUserId;
public:
	explicit FEOSCustomInvitesRejectRequestToJoinCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		, ClientData(nullptr)
	{
	}

	FEOSCustomInvitesRejectRequestToJoinCallbackInfo(const EOS_CustomInvites_RejectRequestToJoinCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		, ClientData(data.ClientData)
		, LocalUserId(data.LocalUserId)
		, TargetUserId(data.TargetUserId)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSCustomInvitesSendCustomNativeInviteRequestedCallbackInfo
{
	GENERATED_BODY()
public:
	/** Context that was passed into EOS_CustomInvites_AddNotifySendCustomNativeInviteRequested */
	void* ClientData;
	/**
	 * Identifies this event which will need to be acknowledged with EOS_UI_AcknowledgeEventId().
	 * @see EOS_UI_AcknowledgeEventId
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSUIEventId UiEventId;
	/** The Product User ID of the local user who is inviting. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId LocalUserId;
	/**
	 * The Native Platform Account Type. If only a single integrated platform is configured then
	 * this will always reference that platform.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FString TargetNativeAccountType;
	/** The Native Platform Account ID of the target user being invited. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FString TargetUserNativeAccountId;
	/** Invite ID that the user is being invited to */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FString InviteId;
public:
	explicit FEOSCustomInvitesSendCustomNativeInviteRequestedCallbackInfo()
		: ClientData(nullptr)
	{
	}

	FEOSCustomInvitesSendCustomNativeInviteRequestedCallbackInfo(const EOS_CustomInvites_SendCustomNativeInviteRequestedCallbackInfo& data)
		: ClientData(data.ClientData)
		, UiEventId(data.UiEventId)
		, LocalUserId(data.LocalUserId)
	{
		const FUTF8ToTCHAR TargetNativeAccountTypeChar(data.TargetNativeAccountType);
		TargetNativeAccountType = TargetNativeAccountTypeChar.Get();
		
		const FUTF8ToTCHAR TargetUserNativeAccountIdChar(data.TargetUserNativeAccountId);
		TargetUserNativeAccountId = TargetUserNativeAccountIdChar.Get();

		const FUTF8ToTCHAR InviteIdChar(data.InviteId);
		InviteId = InviteIdChar.Get();
	}
};

USTRUCT(BlueprintType)
struct FEOSCustomInvitesOnRequestToJoinRejectedCallbackInfo
{
	GENERATED_BODY()
public:
	/** Context that was passed into EOS_CustomInvites_AddNotifyCustomInviteRejected */
	void* ClientData;
	/** User that sent the custom invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId TargetUserId;
	/** Recipient Local user id */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId LocalUserId;
public:
	explicit FEOSCustomInvitesOnRequestToJoinRejectedCallbackInfo()
		: ClientData(nullptr)
	{
	}

	FEOSCustomInvitesOnRequestToJoinRejectedCallbackInfo(const EOS_CustomInvites_OnRequestToJoinRejectedCallbackInfo& data)
		: ClientData(data.ClientData)
		, TargetUserId(data.TargetUserId)
		, LocalUserId(data.LocalUserId)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSCustomInvitesOnRequestToJoinAcceptedCallbackInfo
{
	GENERATED_BODY()
public:
	/** Context that was passed into EOS_CustomInvites_AddNotifyRequestToJoinAccepted */
	void* ClientData;
	/** User that sent the request to join */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId TargetUserId;
	/** Local user ID of the Request to Join recipient */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId LocalUserId;
public:
	explicit FEOSCustomInvitesOnRequestToJoinAcceptedCallbackInfo()
		: ClientData(nullptr)
	{
	}

	FEOSCustomInvitesOnRequestToJoinAcceptedCallbackInfo(const EOS_CustomInvites_OnRequestToJoinAcceptedCallbackInfo& data)
		: ClientData(data.ClientData)
		, TargetUserId(data.TargetUserId)
		, LocalUserId(data.LocalUserId)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSCustomInvitesSendRequestToJoinCallbackInfo
{
	GENERATED_BODY()
public:
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	EOSResult ResultCode;
	/** Context that was passed into EOS_CustomInvites_SendRequestToJoin */
	void* ClientData;
	/** Local user requesting an invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId LocalUserId;
	/** Recipient of Request Invite*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId TargetUserId;
public:
	explicit FEOSCustomInvitesSendRequestToJoinCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		, ClientData(nullptr)
	{
	}

	FEOSCustomInvitesSendRequestToJoinCallbackInfo(const EOS_CustomInvites_SendRequestToJoinCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		, ClientData(data.ClientData)
		, LocalUserId(data.LocalUserId)
		, TargetUserId(data.TargetUserId)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSCustomInvitesFinalizeInviteOptions
{
	GENERATED_BODY()
public:
	/** User that sent the custom invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId TargetUserId;
	/** Recipient Local user id */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId LocalUserId;
	/** Id of the Custom Invite accepted */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FString CustomInviteId;
	/** Result of the Processing operation, transmitted to Social Overlay if applicable */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	EOSResult ProcessingResult;
public:
	FEOSCustomInvitesFinalizeInviteOptions()
		: ProcessingResult(EOSResult::MAX)
	{}
};

USTRUCT(BlueprintType)
struct FEOSCustomInvitesSendRequestToJoinOptions
{
	GENERATED_BODY()
public:
	/** Local user Requesting an Invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId LocalUserId;
	/** Recipient of Request Invite*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId TargetUserId;
};

USTRUCT(BlueprintType)
struct FEOSCustomInvitesAcceptRequestToJoinOptions
{
	GENERATED_BODY()
public:
	/** Local user accepting a request to join */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId LocalUserId;
	/** Target user that sent original request to join */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId TargetUserId;
};

USTRUCT(BlueprintType)
struct FEOSCustomInvitesRejectRequestToJoinOptions
{
	GENERATED_BODY()
public:
	/** Local user declining an invite request */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId LocalUserId;
	/** Target user that sent original invite request */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId TargetUserId;
};


DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCustomInvitesOnSendCustomInviteCallback, const FEOSCustomInvitesOnSendCustomInviteCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCustomInvitesOnCustomInviteReceivedCallback, const FEOSCustomInvitesOnCustomInviteReceivedCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCustomInvitesOnCustomInviteAcceptedCallback, const FEOSCustomInvitesOnCustomInviteAcceptedCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCustomInvitesOnCustomInviteRejectedCallback, const FEOSCustomInvitesCustomInviteRejectedCallbackInfo&, Data);

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCustomInvitesOnRequestToJoinRejectedCallback, const FEOSCustomInvitesOnRequestToJoinRejectedCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCustomInvitesOnRequestToJoinAcceptedCallback, const FEOSCustomInvitesOnRequestToJoinAcceptedCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCustomInvitesOnSendCustomNativeInviteRequestedCallback, const FEOSCustomInvitesSendCustomNativeInviteRequestedCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCustomInvitesOnRejectRequestToJoinCallback, const FEOSCustomInvitesRejectRequestToJoinCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCustomInvitesOnAcceptRequestToJoinCallback, const FEOSCustomInvitesAcceptRequestToJoinCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCustomInvitesOnRequestToJoinReceivedCallback, const FEOSCustomInvitesRequestToJoinReceivedCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCustomInvitesOnRequestToJoinResponseReceivedCallback, const FEOSCustomInvitesRequestToJoinResponseReceivedCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCustomInvitesOnSendRequestToJoinCallback, const FEOSCustomInvitesSendRequestToJoinCallbackInfo&, Data);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		CALLBACK OBJECTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
struct FCustomInvitesSendCustomInviteCallback
{
public:
	FOnCustomInvitesOnSendCustomInviteCallback m_Callback;
public:
	FCustomInvitesSendCustomInviteCallback(const FOnCustomInvitesOnSendCustomInviteCallback& callback)
		: m_Callback(callback)
	{
	}
	virtual ~FCustomInvitesSendCustomInviteCallback()
	{
		m_Callback.Unbind();
	}
};

struct FCustomInvitesCustomInviteReceivedCallback
{
public:
	FOnCustomInvitesOnCustomInviteReceivedCallback m_Callback;
public:
	FCustomInvitesCustomInviteReceivedCallback(const FOnCustomInvitesOnCustomInviteReceivedCallback& callback)
		: m_Callback(callback)
	{
	}
	virtual ~FCustomInvitesCustomInviteReceivedCallback()
	{
		m_Callback.Unbind();
	}
};

struct FCustomInvitesCustomInviteAcceptedCallback
{
public:
	FOnCustomInvitesOnCustomInviteAcceptedCallback m_Callback;
public:
	FCustomInvitesCustomInviteAcceptedCallback(const FOnCustomInvitesOnCustomInviteAcceptedCallback& callback)
		: m_Callback(callback)
	{
	}
	virtual ~FCustomInvitesCustomInviteAcceptedCallback()
	{
		m_Callback.Unbind();
	}
};

struct FCustomInvitesOnCustomInviteRejectedCallback
{
public:
	FOnCustomInvitesOnCustomInviteRejectedCallback m_Callback;
public:
	FCustomInvitesOnCustomInviteRejectedCallback(const FOnCustomInvitesOnCustomInviteRejectedCallback& callback)
		: m_Callback(callback)
	{
	}
	virtual ~FCustomInvitesOnCustomInviteRejectedCallback()
	{
		m_Callback.Unbind();
	}
};

struct FCustomInvitesOnSendRequestToJoinCallback
{
public:
	FOnCustomInvitesOnSendRequestToJoinCallback m_Callback;
public:
	FCustomInvitesOnSendRequestToJoinCallback(const FOnCustomInvitesOnSendRequestToJoinCallback& callback)
		: m_Callback(callback)
	{
	}
	virtual ~FCustomInvitesOnSendRequestToJoinCallback()
	{
		m_Callback.Unbind();
	}
};

struct FCustomInvitesOnRequestToJoinResponseReceivedCallback
{
public:
	FOnCustomInvitesOnRequestToJoinResponseReceivedCallback m_Callback;
public:
	FCustomInvitesOnRequestToJoinResponseReceivedCallback(const FOnCustomInvitesOnRequestToJoinResponseReceivedCallback& callback)
		: m_Callback(callback)
	{
	}
	virtual ~FCustomInvitesOnRequestToJoinResponseReceivedCallback()
	{
		m_Callback.Unbind();
	}
};

struct FCustomInvitesOnRequestToJoinReceivedCallback
{
public:
	FOnCustomInvitesOnRequestToJoinReceivedCallback m_Callback;
public:
	FCustomInvitesOnRequestToJoinReceivedCallback(const FOnCustomInvitesOnRequestToJoinReceivedCallback& callback)
		: m_Callback(callback)
	{
	}
	virtual ~FCustomInvitesOnRequestToJoinReceivedCallback()
	{
		m_Callback.Unbind();
	}
};

struct FCustomInvitesOnSendCustomNativeInviteRequestedCallback
{
public:
	FOnCustomInvitesOnSendCustomNativeInviteRequestedCallback m_Callback;
public:
	FCustomInvitesOnSendCustomNativeInviteRequestedCallback(const FOnCustomInvitesOnSendCustomNativeInviteRequestedCallback& callback)
		: m_Callback(callback)
	{
	}
	virtual ~FCustomInvitesOnSendCustomNativeInviteRequestedCallback()
	{
		m_Callback.Unbind();
	}
};

struct FCustomInvitesOnRequestToJoinAcceptedCallback
{
public:
	FOnCustomInvitesOnRequestToJoinAcceptedCallback m_Callback;
public:
	FCustomInvitesOnRequestToJoinAcceptedCallback(const FOnCustomInvitesOnRequestToJoinAcceptedCallback& callback)
		: m_Callback(callback)
	{
	}
	virtual ~FCustomInvitesOnRequestToJoinAcceptedCallback()
	{
		m_Callback.Unbind();
	}
};

struct FCustomInvitesOnRequestToJoinRejectedCallback
{
public:
	FOnCustomInvitesOnRequestToJoinRejectedCallback m_Callback;
public:
	FCustomInvitesOnRequestToJoinRejectedCallback(const FOnCustomInvitesOnRequestToJoinRejectedCallback& callback)
		: m_Callback(callback)
	{
	}
	virtual ~FCustomInvitesOnRequestToJoinRejectedCallback()
	{
		m_Callback.Unbind();
	}
};

struct FCustomInvitesOnAcceptRequestToJoinCallback
{
public:
	FOnCustomInvitesOnAcceptRequestToJoinCallback m_Callback;
public:
	FCustomInvitesOnAcceptRequestToJoinCallback(const FOnCustomInvitesOnAcceptRequestToJoinCallback& callback)
		: m_Callback(callback)
	{
	}
	virtual ~FCustomInvitesOnAcceptRequestToJoinCallback()
	{
		m_Callback.Unbind();
	}
};

struct FCustomInvitesOnRejectRequestToJoinCallback
{
public:
	FOnCustomInvitesOnRejectRequestToJoinCallback m_Callback;
public:
	FCustomInvitesOnRejectRequestToJoinCallback(const FOnCustomInvitesOnRejectRequestToJoinCallback& callback)
		: m_Callback(callback)
	{
	}
	virtual ~FCustomInvitesOnRejectRequestToJoinCallback()
	{
		m_Callback.Unbind();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		Operations
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
struct FCustomInvitesSendCustomInvite : public EOS_CustomInvites_SendCustomInviteOptions
{
	TArray<EOS_ProductUserId> PointerArray;
	
	FCustomInvitesSendCustomInvite(EOS_ProductUserId InLocalUserId, TArray<FEOSProductUserId>& InTargetUserIds)
		: EOS_CustomInvites_SendCustomInviteOptions()
		, PointerArray(MoveTemp(InTargetUserIds))
	{
		ApiVersion = EOS_CUSTOMINVITES_SETCUSTOMINVITE_API_LATEST;
		LocalUserId = InLocalUserId;

		TargetUserIds = PointerArray.GetData();
		TargetUserIdsCount = PointerArray.Num();
	}
};
