/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Core/EOSHelpers.h"
#include "Core/EOSTypes.h"
#include "eos_rtc_types.h"
#include "EOSRTCTypes.generated.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		ENUMS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

/** Participant RTC's status change */
UENUM(BlueprintType)
enum class EEOSERTCParticipantStatus : uint8
{
	/** Participant joined the room */
	EOS_RTCPS_Joined = 0,
	/** Participant left the room */
	EOS_RTCPS_Left = 1
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		STRUCTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //


/**
 * This struct is used to call EOS_RTC_SetSetting
 *
 * Available values of SettingName:
 * - DisableEchoCancelation: Disables the use of echo cancellation for the audio channel. Default "False".
 * - DisableNoiseSupression: Disables the use of noise suppression for the audio channel. Default "False".
 * - DisableAutoGainControl: Disables the use of auto gain control for the audio channel. Default "False".
 * - DisableDtx: Allows to disable the use of DTX.  Default "False".
 */
USTRUCT(BlueprintType)
struct FEOSRTCSetSettingOptions
{
	GENERATED_BODY()
public:
	/** Setting that should be set. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString SettingName;
	/** Value to set the setting to. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString SettingValue;
public:
	FEOSRTCSetSettingOptions()
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSRTCSetRoomSettingOptions
{
	GENERATED_BODY()
public:
	/** The Product User ID of the user trying to request this operation. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId LocalUserId;
	/** The room the setting will be applied to. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString RoomName;
	/** Setting that should be set. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString SettingName;
	/** Value to set the setting to. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString SettingValue;
public:
	FEOSRTCSetRoomSettingOptions()
	{
	}
};

/**
* This struct is used to call EOS_RTC_JoinRoom.
*/
USTRUCT(BlueprintType)
struct FJoinRoomOptions
{
	GENERATED_BODY()
public:
	/** The product user id of the user trying to request this operation. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId LocalUserId;
	/** The room the user would like to join. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString RoomName;
	/** The room the user would like to join. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString ClientBaseUrl;
	/** Authorization credential token to join the room. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString ParticipantToken;
	/** The participant id used to join the room. If set to NULL the LocalUserId will be used instead. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId ParticipantId;

	/** Join room flags, e.g. EOS_RTC_JOINROOMFLAGS_ENABLE_ECHO. This is a bitwise-or union of the defined flags. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	int32 Flags;
	/** Enable or disable Manual Audio Input. If manual audio input is enabled audio recording is not started and the audio
	* buffers must be passed manually using EOS_RTCAudio_SendAudio.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	bool bManualAudioInputEnabled;
	/** Enable or disable Manual Audio Output. If manual audio output is enabled audio rendering is not started and the audio
	* buffers must be received with EOS_RTCAudio_AddNotifyAudioBeforeRender and rendered manually.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	bool bManualAudioOutputEnabled;
public:
	FJoinRoomOptions()
		: Flags(0)
		  , bManualAudioInputEnabled(false)
		  , bManualAudioOutputEnabled(false)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSRTCOption
{
	GENERATED_BODY()
public:
	/** The unique key of the option. The max size of the string is EOS_RTC_OPTION_KEY_MAXCHARCOUNT. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString Key;
	/** The value of the option. The max size of the string is EOS_RTC_OPTION_VALUE_MAXCHARCOUNT. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString Value;

public:
	FEOSRTCOption() = default;
	FEOSRTCOption(const EOS_RTC_Option& Data)
	{
		const FUTF8ToTCHAR KeyChar(Data.Key);
		Key = KeyChar.Get();

		const FUTF8ToTCHAR ValueChar(Data.Value);
		Value = ValueChar.Get();
	}
};

/**
* This struct is passed in with a call to EOS_RTC_OnJoinRoomCallback.
*/
USTRUCT(BlueprintType)
struct FEOSRTCJoinRoomCallbackInfo
{
	GENERATED_BODY()
public:
	/** This returns:
	* EOS_Success if the channel was successfully joined.
	* EOS_NoConnection: unable to connect to RTC servers (retryable).
	* EOS_InvalidAuth: if the token is invalid (not retryable).
	* EOS_RTC_TooManyParticipants: if the room cannot accept more participants (not retryable).
	* EOS_AccessDenied: if the room name belongs to the RTC voice system (not retryable).
	* EOS_UnexpectedError otherwise (retryable).
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	EOSResult ResultCode;
	/** Client-specified data passed into EOS_RTC_JoinRoom. */
	void* ClientData;
	/** The Product User ID of the user who initiated this request. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId LocalUserId;
	/** The room the user was trying to join. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString RoomName;
	/**
	 * The room option items.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	TArray<FEOSRTCOption> RoomOptions;
public:
	FEOSRTCJoinRoomCallbackInfo()
		: ResultCode(EOSResult::MAX)
		  , ClientData(nullptr)
	{
	}

	FEOSRTCJoinRoomCallbackInfo(const EOS_RTC_JoinRoomCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
		  , RoomName(UTF8_TO_TCHAR(data.RoomName))
	{
		for (uint32_t i=0; i<data.RoomOptionsCount; i++)
		{
			RoomOptions.Add(data.RoomOptions[i]);
		}
	}
};

/**
* This struct is used to call EOS_RTC_LeaveRoom.
*/
USTRUCT(BlueprintType)
struct FLeaveRoomOptions
{
	GENERATED_BODY()
public:
	/** Product User ID of the user requesting to leave the room */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId LocalUserId;
	/** The room to leave. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString RoomName;
public:
	explicit FLeaveRoomOptions() = default;
};

/**
* This struct is passed in with a call to EOS_RTC_OnLeaveRoomCallback.
*/
USTRUCT(BlueprintType)
struct FEOSRTCLeaveRoomCallbackInfo
{
	GENERATED_BODY()
public:
	/** This returns:
	* EOS_Success if the channel was successfully left.
	* EOS_AccessDenied if the room name belongs to the RTC voice system.
	* EOS_UnexpectedError otherwise.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	EOSResult ResultCode;
	/** Client-specified data passed into EOS_RTC_LeaveRoomOptions. */
	void* ClientData;
	/** The Product User ID of the user who initiated this request. */
	FEOSProductUserId LocalUserId;
	/** The room the user was trying to leave. */
	FString RoomName;
public:
	FEOSRTCLeaveRoomCallbackInfo()
		: ResultCode(EOSResult::MAX)
		  , ClientData(nullptr)
	{
	}

	FEOSRTCLeaveRoomCallbackInfo(const EOS_RTC_LeaveRoomCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
		  , RoomName(UTF8_TO_TCHAR(data.RoomName))
	{
	}
};

/**
* This struct is passed in with a call to EOS_RTC_BlockParticipant.
*/
USTRUCT(BlueprintType)
struct FBlockParticipantOptions
{
	GENERATED_BODY()
public:
	/** Product User ID of the user trying to request this operation. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId LocalUserId;
	/** The room the users should be blocked on. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString RoomName;
	/** Product User ID of the participant to block */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId ParticipantId;
	/** Block or unblock the participant */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	bool bBlocked;
public:
	FBlockParticipantOptions()
		: bBlocked(false)
	{
	}
};

/**
* This struct is passed in with a call to EOS_RTC_OnBlockParticipantCallback.
*/
USTRUCT(BlueprintType)
struct FEOSRTCBlockParticipantCallbackInfo
{
	GENERATED_BODY()
public:
	/** This returns:
	* EOS_Success if the channel was successfully blocked.
	* EOS_UnexpectedError otherwise.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	EOSResult ResultCode;
	/** Client-specified data passed into EOS_RTC_BlockParticipant. */
	void* ClientData;
	/** The Product User ID of the user who initiated this request. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId LocalUserId;
	/** The room the users should be blocked on. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString RoomName;
	/** The Product User ID of the participant being blocked */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId ParticipantId;
	/** The block state that should have been set */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	bool bBlocked;
public:
	FEOSRTCBlockParticipantCallbackInfo()
		: ResultCode(EOSResult::EOS_UnexpectedError)
		  , bBlocked(false)
	{
	}

	FEOSRTCBlockParticipantCallbackInfo(const EOS_RTC_BlockParticipantCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
		  , RoomName(UTF8_TO_TCHAR(data.RoomName))
		  , ParticipantId(data.ParticipantId)
		  , bBlocked(data.bBlocked > 0)
	{
	}
};

/**
* This struct is used to call EOS_RTC_AddNotifyDisconnected.
*/
USTRUCT(BlueprintType)
struct FAddNotifyDisconnectedOptions
{
	GENERATED_BODY()
public:
	/** The Product User ID of the user trying to request this operation. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId LocalUserId;
	/** The room this event is registered on. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString RoomName;
public:
	explicit FAddNotifyDisconnectedOptions() = default;
};

USTRUCT(BlueprintType)
struct FEOSRTCDisconnectedCallbackInfo
{
	GENERATED_BODY()
public:
	/** This returns:
	* EOS_Success The room was left cleanly.
	* EOS_NoConnection: There was a network issue connecting to the server (retryable).
	* EOS_RTC_UserKicked: The user has been kicked by the server (retryable).
	* EOS_ServiceFailure: A known error occurred during interaction with the server (retryable).
	* EOS_UnexpectedError Unexpected error (retryable).
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	EOSResult ResultCode;
	/** Client-specified data passed into EOS_RTC_AddNotifyDisconnected. */
	void* ClientData;
	/** The Product User ID of the user who initiated this request. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId LocalUserId;
	/** The room associated with this event. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString RoomName;
public:
	FEOSRTCDisconnectedCallbackInfo()
		: ResultCode(EOSResult::MAX)
		  , ClientData(nullptr)
	{
	}

	FEOSRTCDisconnectedCallbackInfo(const EOS_RTC_DisconnectedCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
		  , RoomName(UTF8_TO_TCHAR(data.RoomName))
	{
	}
};

/**
* This struct is used to get information about a specific participant metadata item.
*/
USTRUCT(BlueprintType)
struct FEOSRTCParticipantMetadata
{
	GENERATED_BODY()
public:
	/** The unique key of this metadata item. The max size of the string is EOS_RTC_PARTICIPANTMETADATA_KEY_MAXCHARCOUNT. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString Key;
	/** The value of this metadata item. The max size of the string is EOS_RTC_PARTICIPANTMETADATA_VALUE_MAXCHARCOUNT. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString Value;
public:
	explicit FEOSRTCParticipantMetadata() = default;

	FEOSRTCParticipantMetadata(const EOS_RTC_ParticipantMetadata& data)
		: Key(UTF8_TO_TCHAR(data.Key))
		  , Value(UTF8_TO_TCHAR(data.Value))
	{
	}
};

USTRUCT(BlueprintType)
struct FAddNotifyParticipantStatusChangedOptions
{
	GENERATED_BODY()
public:
	/** The Product User ID of the user trying to request this operation. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId LocalUserId;
	/** The room this event is registered on. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString RoomName;
public:
	explicit FAddNotifyParticipantStatusChangedOptions() = default;
};

/**
* This struct is passed in with a call to EOS_RTC_AddNotifyParticipantStatusChanged registered event.
*/
USTRUCT(BlueprintType)
struct FEOSRTCParticipantStatusChangedCallbackInfo
{
	GENERATED_BODY()
public:
	/** Client-specified data passed into EOS_RTC_AddNotifyParticipantStatusChanged. */
	void* ClientData;
	/** The Product User ID of the user who initiated this request. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId LocalUserId;
	/** The room associated with this event. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString RoomName;
	/** The participant whose status changed. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId ParticipantId;
	/** What status change occurred */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	EEOSERTCParticipantStatus ParticipantStatus;
	/** The participant metadata items.
	* This is only set if ParticipantStatus is EOS_RTCPS_Joined
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	TArray<FEOSRTCParticipantMetadata> ParticipantMetadata;
	/** The participant's block list status, if ParticipantStatus is EOS_RTCPS_Joined.
	* This is set to true if the participant is in any of the local user's applicable block lists,
	* such Epic block list or any of the current platform's block lists.
	* It can be used to detect when an internal automatic RTC block is applied because of trust and safety restrictions.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	bool bParticipantInBlocklist;
public:
	FEOSRTCParticipantStatusChangedCallbackInfo()
		: ClientData(nullptr)
		  , ParticipantStatus(EEOSERTCParticipantStatus::EOS_RTCPS_Joined)
		  , bParticipantInBlocklist(false)
	{
	}

	FEOSRTCParticipantStatusChangedCallbackInfo(const EOS_RTC_ParticipantStatusChangedCallbackInfo& Data)
		: ClientData(Data.ClientData)
		  , LocalUserId(Data.LocalUserId)
		  , RoomName(UTF8_TO_TCHAR(Data.RoomName))
		  , ParticipantId(Data.ParticipantId)
		  , ParticipantStatus(static_cast<EEOSERTCParticipantStatus>(Data.ParticipantStatus))
		  , bParticipantInBlocklist(Data.bParticipantInBlocklist > 0)
	{
		for (uint32_t i = 0; i < Data.ParticipantMetadataCount; i++)
		{
			ParticipantMetadata.Add(Data.ParticipantMetadata[i]);
		}
	}
};

/**
* This struct is passed in with a call to EOS_RTC_AddNotifyParticipantStatusChanged registered event.
*/
USTRUCT(BlueprintType)
struct FEOSRTCRoomStatisticsUpdatedInfo
{
	GENERATED_BODY()
public:
	/** Client-specified data passed into EOS_RTC_AddNotifyRoomStatisticsUpdated. */
	void* ClientData;
	/** The Product User ID of the user who initiated this request. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId LocalUserId;
	/** The room associated with this event. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString RoomName;
	/** Statistics in JSON format */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString Statistic;
public:
	FEOSRTCRoomStatisticsUpdatedInfo()
		: ClientData(nullptr)
	{
	}

	FEOSRTCRoomStatisticsUpdatedInfo(const EOS_RTC_RoomStatisticsUpdatedInfo& Data)
		: ClientData(Data.ClientData)
		  , LocalUserId(Data.LocalUserId)
		  , RoomName(UTF8_TO_TCHAR(Data.RoomName))
		  , Statistic(UTF8_TO_TCHAR(Data.Statistic))
	{
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		DELEGATES
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnJoinRoomCallback, const FEOSRTCJoinRoomCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLeaveRoomCallback, const FEOSRTCLeaveRoomCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnBlockParticipantCallback, const FEOSRTCBlockParticipantCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnDisconnectedCallback, const FEOSRTCDisconnectedCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnParticipantStatusChangedCallback, const FEOSRTCParticipantStatusChangedCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnRoomStatisticsUpdatedCallback, const FEOSRTCRoomStatisticsUpdatedInfo&, Data);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		CALLBACK OBJECTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

struct FParticipantStatusChangedCallback
{
public:
	FParticipantStatusChangedCallback(const FOnParticipantStatusChangedCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FParticipantStatusChangedCallback()
	{
		m_Callback.Unbind();
	}

public:
	FOnParticipantStatusChangedCallback m_Callback;
};

struct FDisconnectedCallback
{
public:
	FDisconnectedCallback(const FOnDisconnectedCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FDisconnectedCallback()
	{
		m_Callback.Unbind();
	}

public:
	FOnDisconnectedCallback m_Callback;
};

struct FRoomStatisticsUpdatedCallback
{
public:
	FRoomStatisticsUpdatedCallback(const FOnRoomStatisticsUpdatedCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FRoomStatisticsUpdatedCallback()
	{
		m_Callback.Unbind();
	}

public:
	FOnRoomStatisticsUpdatedCallback m_Callback;
};

struct FBlockParticipantCallback
{
public:
	FOnBlockParticipantCallback m_Callback;
public:
	FBlockParticipantCallback(const FOnBlockParticipantCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FBlockParticipantCallback()
	{
		m_Callback.Unbind();
	}
};

struct FLeaveRoomCallback
{
public:
	FOnLeaveRoomCallback m_Callback;
public:
	FLeaveRoomCallback(const FOnLeaveRoomCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FLeaveRoomCallback()
	{
		m_Callback.Unbind();
	}
};

struct FJoinRoomCallback
{
public:
	FOnJoinRoomCallback m_Callback;
public:
	FJoinRoomCallback(const FOnJoinRoomCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FJoinRoomCallback()
	{
		m_Callback.Unbind();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		REQUEST OBJECTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

struct FEOSRTCJoinRoomOptions : public EOS_RTC_JoinRoomOptions
{
	FEOSRTCJoinRoomOptions(FString roomName, FString clientBaseUrl, FString participantToken)
		: EOS_RTC_JoinRoomOptions()
	{
		ApiVersion = EOS_RTC_JOINROOM_API_LATEST;

		if (!roomName.IsEmpty())
		{
			RoomName = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(RoomName), TCHAR_TO_UTF8(*roomName), 256);
		}

		if (!clientBaseUrl.IsEmpty())
		{
			ClientBaseUrl = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(ClientBaseUrl), TCHAR_TO_UTF8(*clientBaseUrl), 256);
		}

		if (!participantToken.IsEmpty())
		{
			ParticipantToken = new char[1024];
			FCStringAnsi::Strncpy(const_cast<char*>(ParticipantToken), TCHAR_TO_UTF8(*participantToken), 1024);
		}
	}

	~FEOSRTCJoinRoomOptions()
	{
		delete[] RoomName;
		delete[] ClientBaseUrl;
		delete[] ParticipantToken;
	}
};

struct FEOSRTCLeaveRoomOptions : public EOS_RTC_LeaveRoomOptions
{
	FEOSRTCLeaveRoomOptions(FString roomName)
		: EOS_RTC_LeaveRoomOptions()
	{
		ApiVersion = EOS_RTC_LEAVEROOM_API_LATEST;

		if (!roomName.IsEmpty())
		{
			RoomName = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(RoomName), TCHAR_TO_UTF8(*roomName), 256);
		}
	}

	~FEOSRTCLeaveRoomOptions()
	{
		delete[] RoomName;
	}
};


struct FEOSRTCBlockParticipantOptions : public EOS_RTC_BlockParticipantOptions
{
	FEOSRTCBlockParticipantOptions(FString roomName)
		: EOS_RTC_BlockParticipantOptions()
	{
		ApiVersion = EOS_RTC_BLOCKPARTICIPANT_API_LATEST;

		if (!roomName.IsEmpty())
		{
			RoomName = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(RoomName), TCHAR_TO_UTF8(*roomName), 256);
		}
	}

	~FEOSRTCBlockParticipantOptions()
	{
		delete[] RoomName;
	}
};


struct FEOSRTCAddNotifyDisconnectedOptions : public EOS_RTC_AddNotifyDisconnectedOptions
{
	FEOSRTCAddNotifyDisconnectedOptions(FString roomName)
		: EOS_RTC_AddNotifyDisconnectedOptions()
	{
		ApiVersion = EOS_RTC_ADDNOTIFYDISCONNECTED_API_LATEST;

		if (!roomName.IsEmpty())
		{
			RoomName = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(RoomName), TCHAR_TO_UTF8(*roomName), 256);
		}
	}

	~FEOSRTCAddNotifyDisconnectedOptions()
	{
		delete[] RoomName;
	}
};

struct FEOSRTCAddNotifyRoomStatisticsUpdatedOptions : public EOS_RTC_AddNotifyRoomStatisticsUpdatedOptions
{
	FEOSRTCAddNotifyRoomStatisticsUpdatedOptions(FString InRoomName)
		: EOS_RTC_AddNotifyRoomStatisticsUpdatedOptions()
	{
		ApiVersion = EOS_RTC_ADDNOTIFYROOMSTATISTICSUPDATED_API_LATEST;

		if (!InRoomName.IsEmpty())
		{
			RoomName = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(RoomName), TCHAR_TO_UTF8(*InRoomName), 256);
		}
	}

	~FEOSRTCAddNotifyRoomStatisticsUpdatedOptions()
	{
		delete[] RoomName;
	}
};

struct FEOSRTCAddNotifyParticipantStatusChangedOptions : public EOS_RTC_AddNotifyParticipantStatusChangedOptions
{
	FEOSRTCAddNotifyParticipantStatusChangedOptions(FString roomName)
		: EOS_RTC_AddNotifyParticipantStatusChangedOptions()
	{
		ApiVersion = EOS_RTC_ADDNOTIFYDISCONNECTED_API_LATEST;

		if (!roomName.IsEmpty())
		{
			RoomName = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(RoomName), TCHAR_TO_UTF8(*roomName), 256);
		}
	}

	~FEOSRTCAddNotifyParticipantStatusChangedOptions()
	{
		delete[] RoomName;
	}
};
