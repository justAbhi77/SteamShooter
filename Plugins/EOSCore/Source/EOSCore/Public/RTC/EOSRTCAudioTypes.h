/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"

#include "eos_rtc_audio_types.h"
#include "Core/EOSHelpers.h"
#include "Core/EOSTypes.h"
#include "EOSRTCAudioTypes.generated.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		ENUMS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

/**
* An enumeration of the different audio channel statuses.
*/
UENUM(BlueprintType)
enum class EEOSERTCAudioStatus : uint8
{
	/** Audio unsupported by the source (no devices) */
	EOS_RTCAS_Unsupported = 0,
	/** Audio enabled */
	EOS_RTCAS_Enabled = 1,
	/** Audio disabled */
	EOS_RTCAS_Disabled = 2,
	/** Audio disabled by the administrator */
	EOS_RTCAS_AdminDisabled = 3,
	/** Audio channel is disabled temporarily for both sending and receiving */
	EOS_RTCAS_NotListeningDisabled = 4
};

/**
* An enumeration of the different audio input device statuses.
*/
UENUM(BlueprintType)
enum class EEOSERTCAudioInputStatus : uint8
{
	/** The device is not in used right now (e.g: you are alone in the room). In such cases, the hardware resources are not allocated. */
	EOS_RTCAIS_Idle = 0,
	/** The device is being used and capturing audio */
	EOS_RTCAIS_Recording = 1,
	/**
	* The SDK is in a recording state, but actually capturing silence because the device is exclusively being used by the platform at the moment.
	* This only applies to certain platforms.
	*/
	EOS_RTCAIS_RecordingSilent = 2,
	/**
	* The SDK is in a recording state, but actually capturing silence because the device is disconnected (e.g: the microphone is not plugged in).
	* This only applies to certain platforms.
	*/
	EOS_RTCAIS_RecordingDisconnected = 3,
	/** Something failed while trying to use the device */
	EOS_RTCAIS_Failed = 4
};

/**
* An enumeration of the different audio output device statuses.
*/
UENUM(BlueprintType)
enum class EEOSERTCAudioOutputStatus : uint8
{
	/** The device is not in used right now (e.g: you are alone in the room). In such cases, the hardware resources are not allocated. */
	EOS_RTCAOS_Idle = 0,
	/** Device is in use */
	EOS_RTCAOS_Playing = 1,
	/** Something failed while trying to use the device */
	EOS_RTCAOS_Failed = 2
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		STRUCTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

/**
* This struct is used to inform the audio system of a user.
*/
USTRUCT(BlueprintType)
struct FEOSRegisterPlatformAudioUserOptions
{
	GENERATED_BODY()
public:
	/** Platform dependent user id. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString UserId;
public:
	explicit FEOSRegisterPlatformAudioUserOptions() = default;
};


/**
* This struct is used to remove a user from the audio system.
*/
USTRUCT(BlueprintType)
struct FEOSUnregisterPlatformAudioUserOptions
{
	GENERATED_BODY()
public:
	/** Platform dependent user id. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString UserId;
public:
	explicit FEOSUnregisterPlatformAudioUserOptions() = default;
};

/**
* This struct is used to call EOS_RTCAudio_AddNotifyParticipantUpdated.
*/
USTRUCT(BlueprintType)
struct FEOSAddNotifyParticipantUpdatedOptions
{
	GENERATED_BODY()
public:
	/** The Product User ID of the user trying to request this operation. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId LocalUserId;
	/** The  room this event is registered on. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString RoomName;
public:
	explicit FEOSAddNotifyParticipantUpdatedOptions() = default;
};

/**
 * This struct is passed in with a call to EOS_RTCAudio_AddNotifyParticipantUpdated registered event.
 */
USTRUCT(BlueprintType)
struct FEOSJoinRoomCallbackInfo
{
	GENERATED_BODY()
public:
	/** Client-specified data passed into EOS_RTCAudio_AddNotifyParticipantUpdated. */
	void* ClientData;
	/** The Product User ID of the user who initiated this request. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
	/** The room associated with this event. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString RoomName;
	/** The participant updated. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId ParticipantId;
	/** The participant speaking / non-speaking status. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	bool bSpeaking;
	/** The participant audio status (enabled, disabled). */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EEOSERTCAudioStatus AudioStatus;
public:
	FEOSJoinRoomCallbackInfo()
		: ClientData(nullptr)
		  , bSpeaking(false)
		  , AudioStatus(EEOSERTCAudioStatus::EOS_RTCAS_Disabled)
	{
	}

	FEOSJoinRoomCallbackInfo(const EOS_RTCAudio_ParticipantUpdatedCallbackInfo& data)
		: ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
		  , RoomName(UTF8_TO_TCHAR(data.RoomName))
		  , ParticipantId(data.ParticipantId)
		  , bSpeaking(data.bSpeaking > 0)
		  , AudioStatus(static_cast<EEOSERTCAudioStatus>(data.AudioStatus))
	{
	}
};

/**
* This struct is used to call EOS_RTCAudio_AddNotifyAudioDevicesChanged.
*/
USTRUCT(BlueprintType)
struct FEOSAddNotifyAudioDevicesChangedOptions
{
	GENERATED_BODY()
public:
	explicit FEOSAddNotifyAudioDevicesChangedOptions() = default;
};

/**
* This struct is passed in with a call to EOS_RTCAudio_AddNotifyAudioDevicesChanged registered event.
*/
USTRUCT(BlueprintType)
struct FEOSAudioDevicesChangedCallbackInfo
{
	GENERATED_BODY()
public:
	/** Client-specified data passed into EOS_RTCAudio_AddNotifyAudioDevicesChanged. */
	void* ClientData;
public:
	FEOSAudioDevicesChangedCallbackInfo() = default;

	FEOSAudioDevicesChangedCallbackInfo(const EOS_RTCAudio_AudioDevicesChangedCallbackInfo& data)
		: ClientData(data.ClientData)
	{
	}
};

/**
* Input parameters for the EOS_RTCAudio_GetAudioInputDevicesCount function.
*/
USTRUCT(BlueprintType)
struct FEOSGetAudioInputDevicesCountOptions
{
	GENERATED_BODY()
public:
	explicit FEOSGetAudioInputDevicesCountOptions() = default;
};

/**
* Input parameters for the EOS_RTCAudio_GetAudioInputDeviceByIndex function.
*/
USTRUCT(BlueprintType)
struct FEOSGetAudioInputDeviceByIndexOptions
{
	GENERATED_BODY()
public:
	/** Index of the device info to retrieve. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	int32 DeviceInfoIndex;
public:
	FEOSGetAudioInputDeviceByIndexOptions()
		: DeviceInfoIndex(0)
	{
	}
};

/**
* This struct is used to get information about a specific input device.
*/
USTRUCT(BlueprintType)
struct FEOSAudioInputDeviceInfo
{
	GENERATED_BODY()
public:
	/** True if this is the default audio input device in the system. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	bool bDefaultDevice;
	/**
	 * The persistent unique id of the device.
	 * The value can be cached - invalidated only when the audio device pool is changed.
	 *
	 * @see EOS_RTCAudio_AddNotifyAudioDevicesChanged
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString DeviceId;
	/**
	* The name of the device
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString DeviceName;
public:
	FEOSAudioInputDeviceInfo()
		: bDefaultDevice(false)
	{
	}

	FEOSAudioInputDeviceInfo(const EOS_RTCAudio_AudioInputDeviceInfo* Data)
		: bDefaultDevice(Data->bDefaultDevice > 0)
		  , DeviceId(UTF8_TO_TCHAR(Data->DeviceId))
		  , DeviceName(UTF8_TO_TCHAR(Data->DeviceName))
	{
	}
};

/**
 * Output parameters for the EOS_RTCAudio_GetAudioOutputDevicesCount function.
 */
USTRUCT(BlueprintType)
struct FEOSGetAudioOutputDevicesCountOptions
{
	GENERATED_BODY()
public:
	explicit FEOSGetAudioOutputDevicesCountOptions() = default;
};

/**
* Input parameters for the EOS_RTCAudio_GetAudioOutputDeviceByIndex function.
*/
USTRUCT(BlueprintType)
struct FEOSGetAudioOutputDeviceByIndexOptions
{
	GENERATED_BODY()
public:
	/** Index of the device info to retrieve. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	int32 DeviceInfoIndex;
public:
	FEOSGetAudioOutputDeviceByIndexOptions()
		: DeviceInfoIndex(0)
	{
	}
};

/**
* This struct is used to get information about a specific input device.
*/
USTRUCT(BlueprintType)
struct FEOSAudioOutputDeviceInfo
{
	GENERATED_BODY()
public:
	/** True if this is the default audio input device in the system. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	bool bDefaultDevice;
	/** 
	*  The persistent unique id of the device.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString DeviceId;
	/**
	* The name of the device
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString DeviceName;
public:
	FEOSAudioOutputDeviceInfo()
		: bDefaultDevice(false)
	{
	}

	FEOSAudioOutputDeviceInfo(const EOS_RTCAudio_AudioOutputDeviceInfo& data)
		: bDefaultDevice(data.bDefaultDevice > 0)
		  , DeviceId(UTF8_TO_TCHAR(data.DeviceId))
		  , DeviceName(UTF8_TO_TCHAR(data.DeviceName))
	{
	}
};

/**
* This struct is used to call EOS_RTCAudio_SetAudioInputSettings.
*/
USTRUCT(BlueprintType)
struct FEOSSetAudioInputSettingsOptions
{
	GENERATED_BODY()
public:
	/** The Product User ID of the user trying to request this operation. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId LocalUserId;
	/** 
	 * The device Id to be used for this user. Pass NULL or empty string to use default input device.
	 * 
	 * If the device ID is invalid, the default device will be used instead.
	 * Despite this fact, that device ID will be stored and the library will try to move on it when an audio device pool is being changed.
	 * 
	 * The actual hardware audio device usage depends on the current payload and optimized not to use it
	 * when generated audio frames cannot be processed by someone else based on a scope of rules (For instance, when a client is alone in a room).
	 * 
	 * @see EOS_RTCAudio_AddNotifyAudioDevicesChanged
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString DeviceId;
	/** The volume to be configured for this device (range 0.0 to 100.0).
	* At the moment, the only value that produce any effect is 0.0 (silence). Any other value is ignored and causes no change to the volume.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	float Volume;
	/** Enable or disable Platform AEC (Acoustic Echo Cancellation) if available. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	bool bPlatformAEC;
public:
	FEOSSetAudioInputSettingsOptions()
		: Volume(0.0f)
		  , bPlatformAEC(false)
	{
	}
};

/**
* This struct is used to call EOS_RTCAudio_SetAudioOutputSettings.
*/
USTRUCT(BlueprintType)
struct FEOSSetAudioOutputSettingsOptions
{
	GENERATED_BODY()
public:
	/** The Product User ID of the user who initiated this request. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId LocalUserId;
	/** 
	 * The device Id to be used for this user. Pass NULL or empty string to use default output device.
	 * 
	 * If the device ID is invalid, the default device will be used instead.
	 * Despite of this fact, that device ID will be stored and the library will try to move on it when a device pool is being changed.
	 * 
	 * The actual hardware audio device usage depends on the current payload and optimized not to use it
	 * when generated audio frames cannot be processed by someone else based on a scope of rules (For instance, when a client is alone in a room).
	 * 
	 * @see EOS_RTCAudio_AddNotifyAudioDevicesChanged
	 * 
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString DeviceId;
	/** The volume to be configured for this device (range 0.0 to 100.0). Volume 50 means that the audio volume is not modified
	* and stays in its source value.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	float Volume;
public:
	FEOSSetAudioOutputSettingsOptions()
		: Volume(0.0f)
	{
	}
};

/**
* This struct is used to call EOS_RTCAudio_AddNotifyAudioInputState.
*/
USTRUCT(BlueprintType)
struct FEOSAddNotifyAudioInputStateOptions
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
	explicit FEOSAddNotifyAudioInputStateOptions() = default;
};

/**
* This struct is passed in with a call to EOS_RTCAudio_AddNotifyAudioInputState registered event.
*/
USTRUCT(BlueprintType)
struct FEOSAudioInputStateCallbackInfo
{
	GENERATED_BODY()
public:
	/** Client-specified data passed into EOS_RTCAudio_AddNotifyAudioInputState. */
	void* ClientData;
	/** The Product User ID of the user who initiated this request. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
	/** The room associated with this event. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString RoomName;
	/** The status of the audio input. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EEOSERTCAudioInputStatus Status;
public:
	FEOSAudioInputStateCallbackInfo()
		: ClientData(nullptr)
		  , Status(EEOSERTCAudioInputStatus::EOS_RTCAIS_Failed)
	{
	}

	FEOSAudioInputStateCallbackInfo(const EOS_RTCAudio_AudioInputStateCallbackInfo& data)
		: ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
		  , RoomName(UTF8_TO_TCHAR(data.RoomName))
		  , Status(static_cast<EEOSERTCAudioInputStatus>(data.Status))
	{
	}
};

/**
* This struct is used to call EOS_RTCAudio_AddNotifyAudioOutputState.
*/
USTRUCT(BlueprintType)
struct FEOSAddNotifyAudioOutputStateOptions
{
	GENERATED_BODY()
public:
	/** The Product User ID of the user trying to request this operation. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId LocalUserId;
	/** The  room this event is registered on. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString RoomName;
public:
	explicit FEOSAddNotifyAudioOutputStateOptions() = default;
};

/**
* This struct is passed in with a call to EOS_RTCAudio_AddNotifyAudioOutputState registered event.
*/
USTRUCT(BlueprintType)
struct FEOSOutputStateCallbackInfo
{
	GENERATED_BODY()
public:
	/** Client-specified data passed into EOS_RTCAudio_AddNotifyAudioOutputState. */
	void* ClientData;
	/** The Product User ID of the user who initiated this request. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
	/** The room associated with this event. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString RoomName;
	/** The status of the audio output. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EEOSERTCAudioOutputStatus Status;
public:
	FEOSOutputStateCallbackInfo()
		: ClientData(nullptr)
		  , Status(EEOSERTCAudioOutputStatus::EOS_RTCAOS_Idle)
	{
	}

	FEOSOutputStateCallbackInfo(const EOS_RTCAudio_AudioOutputStateCallbackInfo& data)
		: ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
		  , RoomName(UTF8_TO_TCHAR(data.RoomName))
		  , Status(static_cast<EEOSERTCAudioOutputStatus>(data.Status))
	{
	}
};

/**
* This struct is used to call EOS_RTCAudio_AddNotifyAudioBeforeSend.
*/
USTRUCT(BlueprintType)
struct FEOSAddNotifyAudioBeforeSendOptions
{
	GENERATED_BODY()
public:
	/** The Product User ID of the user trying to request this operation. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId LocalUserId;
	/** The  room this event is registered on. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString RoomName;
public:
	explicit FEOSAddNotifyAudioBeforeSendOptions() = default;
};

/**
* This struct is used to represent an audio buffer received in Callbacks from EOS_RTCAudio_AddNotifyAudioBeforeSend and EOS_RTCAudio_AddNotifyAudioBeforeRender.
*/
USTRUCT(BlueprintType)
struct FEOSRTCAudioAudioBuffer
{
	GENERATED_BODY()
public:
	/** Sample rate for the samples in the Frames buffer. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	int32 SampleRate;
	/** Number of channels for the samples in the Frames buffer. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	int32 Channels;

	const EOS_RTCAudio_AudioBuffer* Data;
public:
	FEOSRTCAudioAudioBuffer()
		: SampleRate(0)
		  , Channels(0)
	{
	}

	FEOSRTCAudioAudioBuffer(const EOS_RTCAudio_AudioBuffer* data)
		: SampleRate(data->SampleRate)
		  , Channels(data->Channels)
		  , Data(data)
	{
	}

public:
	operator const EOS_RTCAudio_AudioBuffer*() const
	{
		return Data;
	}
};

/**
* This struct is passed in with a call to EOS_RTCAudio_AddNotifyAudioBeforeSend registered event.
*/
USTRUCT(BlueprintType)
struct FEOSBeforeSendCallbackInfo
{
	GENERATED_BODY()
public:
	/** Client-specified data passed into EOS_RTCAudio_AddNotifyAudioOutputState. */
	void* ClientData;
	/** The Product User ID of the user who initiated this request. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
	/** The room associated with this event. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString RoomName;
	/** The status of the audio output. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSRTCAudioAudioBuffer Buffer;
public:
	FEOSBeforeSendCallbackInfo() = default;

	FEOSBeforeSendCallbackInfo(const EOS_RTCAudio_AudioBeforeSendCallbackInfo& data)
		: ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
		  , RoomName(UTF8_TO_TCHAR(data.RoomName))
		  , Buffer(data.Buffer)
	{
	}
};

/**
* This struct is used to call EOS_RTCAudio_AddNotifyAudioBeforeRender.
*/
USTRUCT(BlueprintType)
struct FEOSAddNotifyAudioBeforeRenderOptions
{
	GENERATED_BODY()
public:
	/** The Product User ID of the user trying to request this operation. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId LocalUserId;
	/** The  room this event is registered on. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString RoomName;
	/**
	* Mixed audio or unmixed audio.
	* For unmixed audio notifications it is not supported to modify the samples in the Callback.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	bool bUnmixedAudio;
public:
	FEOSAddNotifyAudioBeforeRenderOptions()
		: bUnmixedAudio(false)
	{
	}
};

/**
* This struct is passed in with a call to EOS_RTCAudio_AddNotifyAudioBeforeRender registered event.
*/
USTRUCT(BlueprintType)
struct FEOSBeforeRenderCallbackInfo
{
	GENERATED_BODY()
public:
	/** Client-specified data passed into EOS_RTCAudio_AddNotifyAudioOutputState. */
	void* ClientData;
	/** The Product User ID of the user who initiated this request. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId LocalUserId;
	/** The room associated with this event. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString RoomName;
	/**
	* Audio buffer.
	* If bUnmixedAudio was set to true when setting the notifications (aka: you get buffers per participant), then you should
	* not modify this buffer.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSRTCAudioAudioBuffer Buffer;
	/**
	* The Product User ID of the participant if bUnmixedAudio was set to true when setting the notifications, or empty if
	* bUnmixedAudio was set to false and thus the buffer is the mixed audio of all participants
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId ParticipantId;
public:
	FEOSBeforeRenderCallbackInfo() = default;

	FEOSBeforeRenderCallbackInfo(const EOS_RTCAudio_AudioBeforeRenderCallbackInfo& data)
		: ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
		  , RoomName(UTF8_TO_TCHAR(data.RoomName))
		  , Buffer(data.Buffer)
		  , ParticipantId(data.ParticipantId)
	{
	}
};

/**
* This struct is used to call EOS_RTCAudio_SendAudio.
*/
USTRUCT(BlueprintType)
struct FEOSSendAudioOptions
{
	GENERATED_BODY()
public:
	/** The Product User ID of the user trying to request this operation. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId LocalUserId;
	/** The  room this event is registered on. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString RoomName;
	/**
	* Audio buffer, which must have a duration of 10 ms.
	* @note The SDK makes a copy of buffer. There is no need to keep the buffer around after calling EOS_RTCAudio_SendAudio
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSRTCAudioAudioBuffer Buffer;
public:
	explicit FEOSSendAudioOptions() = default;
};

/**
* This struct is passed in with a call to EOS_RTCAudio_UpdateSending
*/
USTRUCT(BlueprintType)
struct FEOSUpdateSendingOptions
{
	GENERATED_BODY()
public:
	/** The Product User ID of the user trying to request this operation. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId LocalUserId;
	/** The  room this event is registered on. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString RoomName;
	/** Muted or unmuted audio track status */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	EEOSERTCAudioStatus AudioStatus;
public:
	FEOSUpdateSendingOptions()
		: AudioStatus(EEOSERTCAudioStatus::EOS_RTCAS_Disabled)
	{
	}
};

/**
* This struct is passed in with a call to EOS_RTCAudio_OnUpdateSendingCallback.
*/
USTRUCT(BlueprintType)
struct FEOSUpdateSendingCallbackInfo
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
	/** The Product User ID of the user who initiated this request. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
	/** The room this settings should be applied on. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString RoomName;
	/** Muted or unmuted audio track status */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EEOSERTCAudioStatus AudioStatus;
public:
	FEOSUpdateSendingCallbackInfo()
		: ResultCode(EOSResult::MAX)
		  , ClientData(nullptr)
		  , AudioStatus(EEOSERTCAudioStatus::EOS_RTCAS_Unsupported)
	{
	}

	FEOSUpdateSendingCallbackInfo(const EOS_RTCAudio_UpdateSendingCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
		  , RoomName(UTF8_TO_TCHAR(data.RoomName))
		  , AudioStatus(static_cast<EEOSERTCAudioStatus>(data.AudioStatus))
	{
	}
};

/**
* This struct is passed in with a call to EOS_RTCAudio_UpdateReceiving.
*/
USTRUCT(BlueprintType)
struct FEOSUpdateReceivingOptions
{
	GENERATED_BODY()
public:
	/** The Product User ID of the user trying to request this operation. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId LocalUserId;
	/** The room this settings should be applied on. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString RoomName;
	/** The participant to modify or null to update the global configuration */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId ParticipantId;
	/** Mute or unmute audio track */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	bool bAudioEnabled;
public:
	FEOSUpdateReceivingOptions()
		: bAudioEnabled(false)
	{
	}
};

/**
* This struct is passed in with a call to EOS_RTCAudio_OnUpdateReceivingCallback.
*/
USTRUCT(BlueprintType)
struct FEOSUpdateReceivingCallbackInfo
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
	/** The Product User ID of the user who initiated this request. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
	/** The room this settings should be applied on. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString RoomName;
	/** The participant to modify or null to update the global configuration */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId ParticipantId;
	/** Muted or unmuted audio track */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	bool bAudioEnabled;
public:
	FEOSUpdateReceivingCallbackInfo()
		: ResultCode(EOSResult::MAX)
		  , ClientData(nullptr)
		  , bAudioEnabled(false)
	{
	}

	FEOSUpdateReceivingCallbackInfo(const EOS_RTCAudio_UpdateReceivingCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
		  , RoomName(UTF8_TO_TCHAR(data.RoomName))
		  , ParticipantId(data.ParticipantId)
		  , bAudioEnabled(data.bAudioEnabled > 0)
	{
	}
};

/**
 * This struct is passed in with a call to EOS_RTCAudio_UpdateSendingVolume
 */
USTRUCT(BlueprintType)
struct FEOSRTCAudioUpdateSendingVolumeOptions
{
	GENERATED_BODY()
public:
	/** The Product User ID of the user trying to request this operation. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId LocalUserId;
	/** The room this settings should be applied on. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString RoomName;
	/** The volume to be set for sent audio (range 0.0 to 100.0). Volume 50 means that the audio volume is not modified
	 * and stays in its source value. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	float Volume;
public:
	FEOSRTCAudioUpdateSendingVolumeOptions()
		: Volume(100)
	{
	}
};

/**
 * This struct is passed in with a call to EOS_RTCAudio_OnUpdateSendingVolumeCallback.
 */
USTRUCT(BlueprintType)
struct FEOSRTCAudioUpdateSendingVolumeCallbackInfo
{
	GENERATED_BODY()
public:
	/** This returns:
	 * EOS_Success if sending volume of channels of the local user was successfully changed.
	 * EOS_UnexpectedError otherwise.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EOSResult ResultCode;
	/** Client-specified data passed into EOS_RTCAudio_UpdateSendingVolume. */
	void* ClientData;
	/** The Product User ID of the user who initiated this request. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
	/** The room this settings should be applied on. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString RoomName;
	/** The volume that was set for sent audio (range 0.0 to 100.0). */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	float Volume;
public:
	FEOSRTCAudioUpdateSendingVolumeCallbackInfo()
		: ResultCode(EOSResult::MAX)
		  , ClientData(nullptr)
		  , Volume(100)
	{
	}

	FEOSRTCAudioUpdateSendingVolumeCallbackInfo(const EOS_RTCAudio_UpdateSendingVolumeCallbackInfo& Data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(Data.ResultCode))
		  , ClientData(Data.ClientData)
		  , LocalUserId(Data.LocalUserId)
		  , Volume(Data.Volume)
	{
		const FUTF8ToTCHAR ConvertedRoomName(Data.RoomName);
		RoomName = ConvertedRoomName.Get();
	}
};

/**
 * This struct is passed in with a call to EOS_RTCAudio_UpdateReceivingVolume
 */
USTRUCT(BlueprintType)
struct FEOSRTCAudioUpdateReceivingVolumeOptions
{
	GENERATED_BODY()
public:
	/** The Product User ID of the user trying to request this operation. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId LocalUserId;
	/** The room this settings should be applied on. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString RoomName;
	/** The volume to be set for sent audio (range 0.0 to 100.0). Volume 50 means that the audio volume is not modified
	 * and stays in its source value. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	float Volume;
public:
	FEOSRTCAudioUpdateReceivingVolumeOptions()
		: Volume(100)
	{
	}
};

/**
 * This struct is passed in with a call to EOS_RTCAudio_OnUpdateReceivingVolumeCallback.
 */
USTRUCT(BlueprintType)
struct FEOSRTCAudioUpdateReceivingVolumeCallbackInfo
{
	GENERATED_BODY()
public:
	/** This returns:
	 * EOS_Success if sending volume of channels of the local user was successfully changed.
	 * EOS_UnexpectedError otherwise.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EOSResult ResultCode;
	/** Client-specified data passed into EOS_RTCAudio_UpdateSendingVolume. */
	void* ClientData;
	/** The Product User ID of the user who initiated this request. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
	/** The room this settings should be applied on. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString RoomName;
	/** The volume that was set for sent audio (range 0.0 to 100.0). */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	float Volume;
public:
	FEOSRTCAudioUpdateReceivingVolumeCallbackInfo()
		: ResultCode(EOSResult::MAX)
		  , ClientData(nullptr)
		  , Volume(100)
	{
	}

	FEOSRTCAudioUpdateReceivingVolumeCallbackInfo(const EOS_RTCAudio_UpdateReceivingVolumeCallbackInfo& Data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(Data.ResultCode))
		  , ClientData(Data.ClientData)
		  , LocalUserId(Data.LocalUserId)
		  , Volume(Data.Volume)
	{
		const FUTF8ToTCHAR ConvertedRoomName(Data.RoomName);
		RoomName = ConvertedRoomName.Get();
	}
};

/**
 * This struct is passed in with a call to EOS_RTCAudio_UpdateParticipantVolume
 */
USTRUCT(BlueprintType)
struct FEOSRTCAudioUpdateParticipantVolumeOptions
{
	GENERATED_BODY()
public:
	/** The Product User ID of the user trying to request this operation. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId LocalUserId;
	/** The room this settings should be applied on. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FString RoomName;
	/** The participant to modify or null to update the global configuration */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	FEOSProductUserId ParticipantId;
	/** The volume to be set for received audio (range 0.0 to 100.0). Volume 50 means that the audio volume is not modified
	 * and stays in its source value. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTC")
	float Volume;
public:
	FEOSRTCAudioUpdateParticipantVolumeOptions()
		: Volume(100)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSRTCAudioUpdateParticipantVolumeCallbackInfo
{
	GENERATED_BODY()
public:
	/** This returns:
	 * EOS_Success if sending volume of channels of the local user was successfully changed.
	 * EOS_UnexpectedError otherwise.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EOSResult ResultCode;
	/** Client-specified data passed into EOS_RTCAudio_UpdateSendingVolume. */
	void* ClientData;
	/** The Product User ID of the user who initiated this request. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
	/** The room this settings should be applied on. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString RoomName;
	/** The participant to modify or null to update the global configuration */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId ParticipantId;
	/** The volume that was set for sent audio (range 0.0 to 100.0). */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	float Volume;
public:
	FEOSRTCAudioUpdateParticipantVolumeCallbackInfo()
		: ResultCode(EOSResult::MAX)
		  , ClientData(nullptr)
		  , Volume(100)
	{
	}

	FEOSRTCAudioUpdateParticipantVolumeCallbackInfo(const EOS_RTCAudio_UpdateParticipantVolumeCallbackInfo& Data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(Data.ResultCode))
		  , ClientData(Data.ClientData)
		  , LocalUserId(Data.LocalUserId)
		  , ParticipantId(Data.ParticipantId)
		  , Volume(Data.Volume)
	{
		const FUTF8ToTCHAR ConvertedRoomName(Data.RoomName);
		RoomName = ConvertedRoomName.Get();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		DELEGATES
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnParticipantUpdatedCallback, const FEOSJoinRoomCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAudioInputStateCallback, const FEOSAudioInputStateCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAudioOutputStateCallback, const FEOSOutputStateCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAudioBeforeSendCallback, const FEOSBeforeSendCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAudioBeforeRenderCallback, const FEOSBeforeRenderCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnUpdateSendingCallback, const FEOSUpdateSendingCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnUpdateReceivingCallback, const FEOSUpdateReceivingCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAudioDevicesChangedCallback, const FEOSAudioDevicesChangedCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnUpdateSendingVolumeCallback, const FEOSRTCAudioUpdateSendingVolumeCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnUpdateReceivingVolumeCallback, const FEOSRTCAudioUpdateReceivingVolumeCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnUpdateParticipantVolumeCallback, const FEOSRTCAudioUpdateParticipantVolumeCallbackInfo&, Data);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		CALLBACK OBJECTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

struct FAudioBeforeRenderCallback
{
public:
	FAudioBeforeRenderCallback(const FEOSHRTCAudio AudioHandle, const FOnAudioBeforeRenderCallback& Callback)
		: m_AudioHandle(AudioHandle)
		  , m_Callback(Callback)
	{
	}

	virtual ~FAudioBeforeRenderCallback()
	{
		m_Callback.Unbind();
	}

public:
	FEOSHRTCAudio m_AudioHandle;
	FOnAudioBeforeRenderCallback m_Callback;
};

struct FAudioBeforeSendCallback
{
public:
	FAudioBeforeSendCallback(const FEOSHRTCAudio AudioHandle, const FOnAudioBeforeSendCallback& Callback)
		: m_AudioHandle(AudioHandle)
		  , m_Callback(Callback)
	{
	}

	virtual ~FAudioBeforeSendCallback()
	{
		m_Callback.Unbind();
	}

public:
	FEOSHRTCAudio m_AudioHandle;
	FOnAudioBeforeSendCallback m_Callback;
};

struct FAudioOutputStateCallback
{
public:
	FAudioOutputStateCallback(const FEOSHRTCAudio AudioHandle, const FOnAudioOutputStateCallback& Callback)
		: m_AudioHandle(AudioHandle)
		  , m_Callback(Callback)
	{
	}

	virtual ~FAudioOutputStateCallback()
	{
		m_Callback.Unbind();
	}

public:
	FEOSHRTCAudio m_AudioHandle;
	FOnAudioOutputStateCallback m_Callback;
};

struct FAudioInputStateCallback
{
public:
	FAudioInputStateCallback(const FEOSHRTCAudio AudioHandle, const FOnAudioInputStateCallback& Callback)
		: m_AudioHandle(AudioHandle)
		  , m_Callback(Callback)
	{
	}

	virtual ~FAudioInputStateCallback()
	{
		m_Callback.Unbind();
	}

public:
	FEOSHRTCAudio m_AudioHandle;
	FOnAudioInputStateCallback m_Callback;
};

struct FAudioDevicesChangedCallback
{
public:
	FAudioDevicesChangedCallback(const FEOSHRTCAudio AudioHandle, const FOnAudioDevicesChangedCallback& Callback)
		: m_AudioHandle(AudioHandle)
		  , m_Callback(Callback)
	{
	}

	virtual ~FAudioDevicesChangedCallback()
	{
		m_Callback.Unbind();
	}

public:
	FEOSHRTCAudio m_AudioHandle;
	FOnAudioDevicesChangedCallback m_Callback;
};

struct FParticipantUpdatedCallback
{
public:
	FParticipantUpdatedCallback(const FEOSHRTCAudio AudioHandle, const FOnParticipantUpdatedCallback& Callback)
		: m_AudioHandle(AudioHandle)
		  , m_Callback(Callback)
	{
	}

	virtual ~FParticipantUpdatedCallback()
	{
		m_Callback.Unbind();
	}

public:
	FEOSHRTCAudio m_AudioHandle;
	FOnParticipantUpdatedCallback m_Callback;
};

struct FUpdateReceivingOptionsCallback
{
public:
	FOnUpdateReceivingCallback m_Callback;
public:
	FUpdateReceivingOptionsCallback(const FOnUpdateReceivingCallback& Callback)
		: m_Callback(Callback)
	{
	}

	virtual ~FUpdateReceivingOptionsCallback()
	{
		m_Callback.Unbind();
	}
};

struct FUpdateSendingOptionsCallback
{
public:
	FOnUpdateSendingCallback m_Callback;
public:
	FUpdateSendingOptionsCallback(const FOnUpdateSendingCallback& Callback)
		: m_Callback(Callback)
	{
	}

	virtual ~FUpdateSendingOptionsCallback()
	{
		m_Callback.Unbind();
	}
};

struct FUpdateSendingVolumeCallback
{
public:
	FOnUpdateSendingVolumeCallback m_Callback;
public:
	FUpdateSendingVolumeCallback(const FOnUpdateSendingVolumeCallback& Callback)
		: m_Callback(Callback)
	{
	}

	virtual ~FUpdateSendingVolumeCallback()
	{
		m_Callback.Unbind();
	}
};

struct FUpdateReceivingVolumeCallback
{
public:
	FOnUpdateReceivingVolumeCallback m_Callback;
public:
	FUpdateReceivingVolumeCallback(const FOnUpdateReceivingVolumeCallback& Callback)
		: m_Callback(Callback)
	{
	}

	virtual ~FUpdateReceivingVolumeCallback()
	{
		m_Callback.Unbind();
	}
};

struct FUpdateParticipantVolumeCallback
{
public:
	FOnUpdateParticipantVolumeCallback m_Callback;
public:
	FUpdateParticipantVolumeCallback(const FOnUpdateParticipantVolumeCallback& Callback)
		: m_Callback(Callback)
	{
	}

	virtual ~FUpdateParticipantVolumeCallback()
	{
		m_Callback.Unbind();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		REQUEST OBJECTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //


struct FRegisterPlatformAudioUserOptions : public EOS_RTCAudio_RegisterPlatformAudioUserOptions
{
	FRegisterPlatformAudioUserOptions(FString userId)
		: EOS_RTCAudio_RegisterPlatformAudioUserOptions()
	{
		ApiVersion = EOS_RTCAUDIO_REGISTERPLATFORMAUDIOUSER_API_LATEST;

		if (!userId.IsEmpty())
		{
			UserId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(UserId), TCHAR_TO_UTF8(*userId), 256);
		}
	}

	~FRegisterPlatformAudioUserOptions()
	{
		delete[] UserId;
	}
};


struct FUnregisterPlatformAudioUserOptions : public EOS_RTCAudio_UnregisterPlatformAudioUserOptions
{
	FUnregisterPlatformAudioUserOptions(FString userId)
		: EOS_RTCAudio_UnregisterPlatformAudioUserOptions()
	{
		ApiVersion = EOS_RTCAUDIO_UNREGISTERPLATFORMAUDIOUSER_API_LATEST;

		if (!userId.IsEmpty())
		{
			UserId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(UserId), TCHAR_TO_UTF8(*userId), 256);
		}
	}

	~FUnregisterPlatformAudioUserOptions()
	{
		delete[] UserId;
	}
};


struct FSetAudioInputSettingsOptions : public EOS_RTCAudio_SetAudioInputSettingsOptions
{
	FSetAudioInputSettingsOptions(FString deviceId)
		: EOS_RTCAudio_SetAudioInputSettingsOptions()
	{
		ApiVersion = EOS_RTCAUDIO_SETAUDIOINPUTSETTINGS_API_LATEST;

		if (!deviceId.IsEmpty())
		{
			DeviceId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(DeviceId), TCHAR_TO_UTF8(*deviceId), 256);
		}
	}

	~FSetAudioInputSettingsOptions()
	{
		delete[] DeviceId;
	}
};


struct FSetAudioOutputSettingsOptions : public EOS_RTCAudio_SetAudioOutputSettingsOptions
{
	FSetAudioOutputSettingsOptions(FString deviceId)
		: EOS_RTCAudio_SetAudioOutputSettingsOptions()
	{
		ApiVersion = EOS_RTCAUDIO_SETAUDIOOUTPUTSETTINGS_API_LATEST;

		if (!deviceId.IsEmpty())
		{
			DeviceId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(DeviceId), TCHAR_TO_UTF8(*deviceId), 256);
		}
	}

	~FSetAudioOutputSettingsOptions()
	{
		delete[] DeviceId;
	}
};


struct FSendAudioOptions : public EOS_RTCAudio_SendAudioOptions
{
	FSendAudioOptions(FString roomName)
		: EOS_RTCAudio_SendAudioOptions()
	{
		ApiVersion = EOS_RTCAUDIO_SENDAUDIO_API_LATEST;

		if (!roomName.IsEmpty())
		{
			RoomName = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(RoomName), TCHAR_TO_UTF8(*roomName), 256);
		}
	}

	~FSendAudioOptions()
	{
		delete[] RoomName;
	}
};


struct FUpdateSendingOptions : public EOS_RTCAudio_UpdateSendingOptions
{
	FUpdateSendingOptions(FString roomName)
		: EOS_RTCAudio_UpdateSendingOptions()
	{
		ApiVersion = EOS_RTCAUDIO_UPDATESENDING_API_LATEST;

		if (!roomName.IsEmpty())
		{
			RoomName = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(RoomName), TCHAR_TO_UTF8(*roomName), 256);
		}
	}

	~FUpdateSendingOptions()
	{
		delete[] RoomName;
	}
};


struct FUpdateReceivingOptions : public EOS_RTCAudio_UpdateReceivingOptions
{
	FUpdateReceivingOptions(FString roomName)
		: EOS_RTCAudio_UpdateReceivingOptions()
	{
		ApiVersion = EOS_RTCAUDIO_UPDATERECEIVING_API_LATEST;

		if (!roomName.IsEmpty())
		{
			RoomName = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(RoomName), TCHAR_TO_UTF8(*roomName), 256);
		}
	}

	~FUpdateReceivingOptions()
	{
		delete[] RoomName;
	}
};


struct FAddNotifyParticipantUpdatedOptions : public EOS_RTCAudio_AddNotifyParticipantUpdatedOptions
{
	FAddNotifyParticipantUpdatedOptions(FString roomName)
		: EOS_RTCAudio_AddNotifyParticipantUpdatedOptions()
	{
		ApiVersion = EOS_RTCAUDIO_ADDNOTIFYPARTICIPANTUPDATED_API_LATEST;

		if (!roomName.IsEmpty())
		{
			RoomName = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(RoomName), TCHAR_TO_UTF8(*roomName), 256);
		}
	}

	~FAddNotifyParticipantUpdatedOptions()
	{
		delete[] RoomName;
	}
};


struct FAddNotifyAudioOutputStateOptions : public EOS_RTCAudio_AddNotifyAudioOutputStateOptions
{
	FAddNotifyAudioOutputStateOptions(FString roomName)
		: EOS_RTCAudio_AddNotifyAudioOutputStateOptions()
	{
		ApiVersion = EOS_RTCAUDIO_ADDNOTIFYAUDIOOUTPUTSTATE_API_LATEST;

		if (!roomName.IsEmpty())
		{
			RoomName = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(RoomName), TCHAR_TO_UTF8(*roomName), 256);
		}
	}

	~FAddNotifyAudioOutputStateOptions()
	{
		delete[] RoomName;
	}
};


struct FAddNotifyAudioBeforeSendOptions : public EOS_RTCAudio_AddNotifyAudioBeforeSendOptions
{
	FAddNotifyAudioBeforeSendOptions(FString roomName)
		: EOS_RTCAudio_AddNotifyAudioBeforeSendOptions()
	{
		ApiVersion = EOS_RTCAUDIO_ADDNOTIFYAUDIOBEFORESEND_API_LATEST;

		if (!roomName.IsEmpty())
		{
			RoomName = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(RoomName), TCHAR_TO_UTF8(*roomName), 256);
		}
	}

	~FAddNotifyAudioBeforeSendOptions()
	{
		delete[] RoomName;
	}
};


struct FAddNotifyAudioBeforeRenderOptions : public EOS_RTCAudio_AddNotifyAudioBeforeRenderOptions
{
	FAddNotifyAudioBeforeRenderOptions(FString roomName)
		: EOS_RTCAudio_AddNotifyAudioBeforeRenderOptions()
	{
		ApiVersion = EOS_RTCAUDIO_ADDNOTIFYAUDIOBEFORERENDER_API_LATEST;

		if (!roomName.IsEmpty())
		{
			RoomName = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(RoomName), TCHAR_TO_UTF8(*roomName), 256);
		}
	}

	~FAddNotifyAudioBeforeRenderOptions()
	{
		delete[] RoomName;
	}
};

struct FAddNotifyAudioInputStateOptions : public EOS_RTCAudio_AddNotifyAudioInputStateOptions
{
	FAddNotifyAudioInputStateOptions(FString roomName)
		: EOS_RTCAudio_AddNotifyAudioInputStateOptions()
	{
		ApiVersion = EOS_RTCAUDIO_ADDNOTIFYAUDIOINPUTSTATE_API_LATEST;

		if (!roomName.IsEmpty())
		{
			RoomName = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(RoomName), TCHAR_TO_UTF8(*roomName), 256);
		}
	}

	~FAddNotifyAudioInputStateOptions()
	{
		delete[] RoomName;
	}
};
