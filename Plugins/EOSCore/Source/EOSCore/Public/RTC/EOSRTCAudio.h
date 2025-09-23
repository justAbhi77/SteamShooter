/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "EOSCoreModule.h"
#include "Core/EOSTypes.h"
#include "EOSRTCAudioTypes.h"
#include "EOSRTCAudio.generated.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UCoreRTCAudio
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

/**
* The RTC Audio Interface. This is used to manage Audio specific RTC features
*
* @see EOS_RTC_GetVoiceInterface
*/

UCLASS()
class EOSCORE_API UCoreRTCAudio : public UEOSCoreSubsystem
{
	GENERATED_BODY()
public:
	virtual void Deinitialize() override;
public:
	/**
	* The RTC Audio Interface. This is used to manage Audio specific RTC features
	*
	* @see EOS_RTC_GetVoiceInterface
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Interfaces", meta = (WorldContext = "WorldContextObject"))
	static UCoreRTCAudio* GetRTCAudio(UObject* WorldContextObject);

public:
	/**
	 * Use this function to inform the audio system of a user.
	 *
	 * This function is only necessary for some platforms.
	 *
	 * @param Options structure containing the parameters for the operation.
	 * @return EOS_Success if the user was successfully registered, EOS_UnexpectedError otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAudio", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAudio_RegisterPlatformAudioUser"))
	static EOSResult EOSRTCAudioRegisterPlatformAudioUser(FEOSHRTCAudio Handle, FEOSRegisterPlatformAudioUserOptions Options);

	/**
	 * Use this function to remove a user that was added with EOS_RTCAudio_RegisterPlatformAudioUser.
	 *
	 * @param Options structure containing the parameters for the operation.
	 * @return EOS_Success if the user was successfully unregistered, EOS_UnexpectedError otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAudio", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAudio_UnregisterPlatformAudioUser"))
	static EOSResult EOSRTCAudioUnregisterPlatformAudioUser(FEOSHRTCAudio Handle, FEOSUnregisterPlatformAudioUserOptions Options);

	/**
	 * Returns the number of audio input devices available in the system.
	 *
	 * The returned value should not be cached and should instead be used immediately with the EOS_RTCAudio_GetAudioInputDeviceByIndex
	 * function.
	 *
	 * @param Options structure containing the parameters for the operation
	 * @return The number of audio input devices
	 * @see EOS_RTCAudio_GetAudioInputDeviceByIndex
	 * @see EOS_RTCAudio_AddNotifyAudioDevicesChanged
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAudio", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAudio_GetAudioInputDevicesCount"))
	static int32 EOSRTCAudioGetAudioInputDevicesCount(FEOSHRTCAudio Handle, const FEOSGetAudioInputDevicesCountOptions Options);

	/**
	 * Fetches an audio input device's info from then given index. The returned value should not be cached and important
	 * information should be copied off of the result object immediately.
	 *
	 * @param Options structure containing the index being accessed
	 * @return A pointer to the device information, or NULL on error. You should NOT keep hold of this pointer.
	 * @see EOS_RTCAudio_GetAudioInputDevicesCount
	 * @see EOS_RTCAudio_AddNotifyAudioDevicesChanged
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAudio", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAudio_GetAudioInputDeviceByIndex"))
	static FEOSAudioInputDeviceInfo EOSRTCAudioGetAudioInputDeviceByIndex(FEOSHRTCAudio Handle, FEOSGetAudioOutputDeviceByIndexOptions Options);

	/**
	 * Returns the number of audio output devices available in the system.
	 *
	 * The returned value should not be cached and should instead be used immediately with the EOS_RTCAudio_GetAudioOutputDeviceByIndex
	 * function.
	 *
	 * @param Options structure containing the parameters for the operation
	 * @return The number of audio output devices
	 * @see EOS_RTCAudio_GetAudioOutputDeviceByIndex
	 * @see EOS_RTCAudio_AddNotifyAudioDevicesChanged
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAudio", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAudio_GetAudioOutputDevicesCount"))
	static int32 EOSRTCAudioGetAudioOutputDevicesCount(FEOSHRTCAudio Handle, FEOSGetAudioOutputDevicesCountOptions Options);

	/**
	 * Fetches an audio output device's info from then given index.
	 *
	 * The returned value should not be cached and important information should be copied off of the result object immediately.
	 *
	 * @param Options structure containing the index being accessed
	 * @return A pointer to the device information, or NULL on error. You should NOT keep hold of this pointer.
	 * @see EOS_RTCAudio_GetAudioOutputDevicesCount
	 * @see EOS_RTCAudio_AddNotifyAudioDevicesChanged
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAudio", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAudio_GetAudioOutputDeviceByIndex"))
	static FEOSAudioOutputDeviceInfo EOSRTCAudioGetAudioOutputDeviceByIndex(FEOSHRTCAudio Handle, FEOSGetAudioOutputDeviceByIndexOptions Options);

	/**
	 * Use this function to set audio input settings, such as the active input device, volume, or platform AEC.
	 *
	 * @param Options structure containing the parameters for the operation.
	 * @return EOS_Success if the setting was successful
	 *         EOS_InvalidParameters if any of the parameters are incorrect
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAudio", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAudio_SetAudioInputSettings"))
	static EOSResult EOSRTCAudioSetAudioInputSettings(FEOSHRTCAudio Handle, FEOSSetAudioInputSettingsOptions Options);

	/**
	 * Use this function to set audio output settings, such as the active output device or volume.
	 *
	 * @param Options structure containing the parameters for the operation.
	 * @return EOS_Success if the setting was successful
	 *         EOS_InvalidParameters if any of the parameters are incorrect
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAudio", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAudio_SetAudioOutputSettings"))
	static EOSResult EOSRTCAudioSetAudioOutputSettings(FEOSHRTCAudio Handle, FEOSSetAudioOutputSettingsOptions Options);

	/**
	 * Use this function to push a new audio buffer to be sent to the participants of a room.
	 *
	 * This should only be used if Manual Audio Input was enabled locally for the specified room.
	 *
	 * @param Options structure containing the parameters for the operation.
	 * @return EOS_Success if the buffer was successfully queued for sending
	 *         EOS_InvalidParameters if any of the parameters are incorrect
	 *         EOS_NotFound if the specified room was not found
	 *         EOS_InvalidState if manual recording was not enabled when joining the room.
	 * @see EOS_RTC_JoinRoomOptions
	 * @see EOS_Lobby_LocalRTCOptions
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAudio", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAudio_SendAudio"))
	static EOSResult EOSRTCAudioSendAudio(FEOSHRTCAudio Handle, FEOSSendAudioOptions Options);

	/**
	 * Use this function to tweak outgoing audio options per room.
	 *
	 * @note Due to internal implementation details, this function requires that you first register to any notification for room
	 *
	 * @param Options structure containing the parameters for the operation.
	 * @param Callback The callback to be fired when the operation completes, either successfully or in error
	 * @return EOS_Success if the operation succeeded
	 *         EOS_InvalidParameters if any of the parameters are incorrect
	 *         EOS_NotFound if the local user is not in the room
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAudio", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAudio_UpdateSending"))
	static void EOSRTCAudioUpdateSending(FEOSHRTCAudio Handle, FEOSUpdateSendingOptions Options, const FOnUpdateSendingCallback& Callback);

	/**
	 * Use this function to tweak incoming audio options per room.
	 *
	 * @note Due to internal implementation details, this function requires that you first register to any notification for room
	 *
	 * @param Options structure containing the parameters for the operation.
	 * @param ClientData Arbitrary data that is passed back in the Callback
	 * @param Callback The callback to be fired when the operation completes, either successfully or in error
	 * @return EOS_Success if the operation succeeded
	 *         EOS_InvalidParameters if any of the parameters are incorrect
	 *         EOS_NotFound if either the local user or specified participant are not in the room
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAudio", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAudio_UpdateReceiving"))
	static void EOSRTCAudioUpdateReceiving(FEOSHRTCAudio Handle, FEOSUpdateReceivingOptions Options, const FOnUpdateReceivingCallback& Callback);

	/**
	 * Use this function to change outgoing audio volume for a room.
	 *
	 * @note Due to internal implementation details, this function requires that you first register to any notification for room
	 *
	 * @param Options structure containing the parameters for the operation.
	 * @param Callback The callback to be fired when the operation completes, either successfully or in error
	 * @return EOS_Success if the operation succeeded
	 *         EOS_InvalidParameters if any of the parameters are incorrect
	 *         EOS_NotFound if the local user is not in the room
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAudio", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAudio_UpdateSendingVolume"))
	static void EOSRTCAudioUpdateSendingVolume(FEOSHRTCAudio Handle, FEOSRTCAudioUpdateSendingVolumeOptions Options, const FOnUpdateSendingVolumeCallback& Callback);

	/**
	 * Use this function to change incoming audio volume for a room.
	 *
	 * @note Due to internal implementation details, this function requires that you first register to any notification for room
	 *
	 * @param Options structure containing the parameters for the operation.
	 * @param Callback The callback to be fired when the operation completes, either successfully or on error
	 * @return EOS_Success if the operation succeeded
	 *         EOS_InvalidParameters if any of the parameters are incorrect
	 *         EOS_NotFound if the local user is not in the room
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAudio", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAudio_UpdateReceivingVolume"))
	static void EOSRTCAudioUpdateReceivingVolume(FEOSHRTCAudio Handle, FEOSRTCAudioUpdateReceivingVolumeOptions Options, const FOnUpdateReceivingVolumeCallback& Callback);

	/**
	 * Use this function to change participant audio volume for a room.
	 *
	 * @note Due to internal implementation details, this function requires that you first register to any notification for room
	 *
	 * @param Options structure containing the parameters for the operation.
	 * @param Callback The callback to be fired when the operation completes, either successfully or in error
	 * @return EOS_Success if the operation succeeded
	 *         EOS_InvalidParameters if any of the parameters are incorrect
	 *         EOS_NotFound if either the local user or specified participant are not in the room
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAudio", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAudio_UpdateParticipantVolume"))
	static void EOSRTCAudioUpdateParticipantVolume(FEOSHRTCAudio Handle, FEOSRTCAudioUpdateParticipantVolumeOptions Options, const FOnUpdateParticipantVolumeCallback& Callback);

	/**
	 * Register to receive notifications when a room participant audio status is updated (f.e when speaking flag changes).
	 *
	 * If the returned NotificationId is valid, you must call EOS_RTCAudio_RemoveNotifyParticipantUpdated when you no longer wish
	 * to have your Callback called.
	 *
	 * @param ClientData Arbitrary data that is passed back in the Callback
	 * @param Callback The callback to be fired when a presence change occurs
	 * @return Notification ID representing the registered callback if successful, an invalid NotificationId if not
	 *
	 * @see EOS_INVALID_NOTIFICATIONID
	 * @see EOS_RTCAudio_RemoveNotifyParticipantUpdated
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAudio", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAudio_AddNotifyParticipantUpdated"))
	static FEOSNotificationId EOSRTCAudioAddNotifyParticipantUpdated(UObject* WorldContextObject, FEOSHRTCAudio Handle, FEOSAddNotifyParticipantUpdatedOptions Options, const FOnParticipantUpdatedCallback& Callback);

	/**
	 * Unregister a previously bound notification handler from receiving participant updated notifications
	 *
	 * @param NotificationId The Notification ID representing the registered callback
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAudio", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAudio_RemoveNotifyParticipantUpdated"))
	static void EOSRTCAudioRemoveNotifyParticipantUpdated(UObject* WorldContextObject, FEOSHRTCAudio Handle, FEOSNotificationId NotificationId);

	/**
	 * Register to receive notifications when an audio device is added or removed to the system.
	 *
	 * If the returned NotificationId is valid, you must call EOS_RTCAudio_RemoveNotifyAudioDevicesChanged when you no longer wish
	 * to have your Callback called.
	 *
	 * @param ClientData Arbitrary data that is passed back in the Callback
	 * @param Callback The callback to be fired when an audio device change occurs
	 * @return Notification ID representing the registered callback if successful, an invalid NotificationId if not
	 *
	 * @see EOS_INVALID_NOTIFICATIONID
	 * @see EOS_RTCAudio_RemoveNotifyAudioDevicesChanged
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAudio", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAudio_AddNotifyAudioDevicesChanged"))
	static FEOSNotificationId EOSRTCAudioAddNotifyAudioDevicesChanged(UObject* WorldContextObject, FEOSHRTCAudio Handle, FEOSAddNotifyAudioDevicesChangedOptions Options, const FOnAudioDevicesChangedCallback& Callback);

	/**
	 * Unregister a previously bound notification handler from receiving audio devices notifications
	 *
	 * @param NotificationId The Notification ID representing the registered callback
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAudio", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAudio_RemoveNotifyAudioDevicesChanged"))
	static void EOSRTCAudioRemoveNotifyAudioDevicesChanged(UObject* WorldContextObject, FEOSHRTCAudio Handle, FEOSNotificationId NotificationId);

	/**
	 * Register to receive notifications when audio input state changed.
	 *
	 * If the returned NotificationId is valid, you must call EOS_RTCAudio_RemoveNotifyAudioInputState when you no longer wish to
	 * have your Callback called.
	 *
	 * @param ClientData Arbitrary data that is passed back in the Callback
	 * @param Callback The callback to be fired when audio input state changes
	 * @return Notification ID representing the registered callback if successful, an invalid NotificationId if not
	 *
	 * @see EOS_INVALID_NOTIFICATIONID
	 * @see EOS_RTCAudio_RemoveNotifyAudioInputState
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAudio", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAudio_AddNotifyAudioInputState"))
	static FEOSNotificationId EOSRTCAudioAddNotifyAudioInputState(UObject* WorldContextObject, FEOSHRTCAudio Handle, FEOSAddNotifyAudioInputStateOptions Options, const FOnAudioInputStateCallback& Callback);

	/**
	 * Unregister a previously bound notification handler from receiving notifications on audio input state changed.
	 *
	 * @param NotificationId The Notification ID representing the registered callback
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAudio", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAudio_RemoveNotifyAudioInputState"))
	static void EOSRTCAudioRemoveNotifyAudioInputState(UObject* WorldContextObject, FEOSHRTCAudio Handle, FEOSNotificationId NotificationId);

	/**
	 * Register to receive notifications when audio output state changed.
	 *
	 * If the returned NotificationId is valid, you must call EOS_RTCAudio_RemoveNotifyAudioOutputState when you no longer wish to
	 * have your Callback called.
	 *
	 * @param ClientData Arbitrary data that is passed back in the Callback
	 * @param Callback The callback to be fired when audio output state changes
	 * @return Notification ID representing the registered callback if successful, an invalid NotificationId if not
	 *
	 * @see EOS_INVALID_NOTIFICATIONID
	 * @see EOS_RTCAudio_RemoveNotifyAudioOutputState
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAudio", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAudio_AddNotifyAudioOutputState"))
	static FEOSNotificationId EOSRTCAudioAddNotifyAudioOutputState(UObject* WorldContextObject, FEOSHRTCAudio Handle, FEOSAddNotifyAudioOutputStateOptions Options, const FOnAudioOutputStateCallback& Callback);

	/**
	 * Unregister a previously bound notification handler from receiving notifications on audio output state changed.
	 *
	 * @param NotificationId The Notification ID representing the registered callback
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAudio", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAudio_RemoveNotifyAudioOutputState"))
	static void EOSRTCAudioRemoveNotifyAudioOutputState(UObject* WorldContextObject, FEOSHRTCAudio Handle, FEOSNotificationId NotificationId);

	/**
	 * Register to receive notifications when local audio buffers are about to be encoded and sent.
	 *
	 * This gives you access to the audio data about to be sent, allowing for example the implementation of custom filters/effects.
	 *
	 * If the returned NotificationId is valid, you must call EOS_RTCAudio_RemoveNotifyAudioBeforeSend when you no longer wish to
	 * have your Callback called.
	 *
	 * @param ClientData Arbitrary data that is passed back in the Callback
	 * @param Callback The callback to be fired when a presence change occurs
	 * @return Notification ID representing the registered callback if successful, an invalid NotificationId if not
	 *
	 * @see EOS_INVALID_NOTIFICATIONID
	 * @see EOS_RTCAudio_RemoveNotifyAudioBeforeSend
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAudio", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAudio_AddNotifyAudioBeforeSend"))
	static FEOSNotificationId EOSRTCAudioAddNotifyAudioBeforeSend(UObject* WorldContextObject, FEOSHRTCAudio Handle, FEOSAddNotifyAudioBeforeSendOptions Options, const FOnAudioBeforeSendCallback& Callback);

	/**
	 * Unregister a previously bound notification handler from receiving local audio buffers before they are encoded and sent.
	 *
	 * @param NotificationId The Notification ID representing the registered callback
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAudio", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAudio_RemoveNotifyAudioBeforeSend"))
	static void EOSRTCAudioRemoveNotifyAudioBeforeSend(UObject* WorldContextObject, FEOSHRTCAudio Handle, FEOSNotificationId NotificationId);

	/**
	 * Register to receive notifications with remote audio buffers before they are rendered.
	 *
	 * This gives you access to the audio data received, allowing for example the implementation of custom filters/effects.
	 *
	 * If the returned NotificationId is valid, you must call EOS_RTCAudio_RemoveNotifyAudioBeforeRender when you no longer wish to
	 * have your Callback called.
	 *
	 * @param ClientData Arbitrary data that is passed back in the Callback
	 * @param Callback The callback to be fired when a presence change occurs
	 * @return Notification ID representing the registered callback if successful, an invalid NotificationId if not
	 *
	 * @see EOS_INVALID_NOTIFICATIONID
	 * @see EOS_RTCAudio_RemoveNotifyAudioBeforeRender
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAudio", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAudio_AddNotifyAudioBeforeRender"))
	static FEOSNotificationId EOSRTCAudioAddNotifyAudioBeforeRender(UObject* WorldContextObject, FEOSHRTCAudio Handle, FEOSAddNotifyAudioBeforeRenderOptions Options, const FOnAudioBeforeRenderCallback& Callback);

	/**
	 * Unregister a previously bound notification handler from receiving remote audio buffers before they are rendered.
	 *
	 * @param NotificationId The Notification ID representing the registered callback
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAudio", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAudio_RemoveNotifyAudioBeforeRender"))
	static void EOSRTCAudioRemoveNotifyAudioBeforeRender(UObject* WorldContextObject, FEOSHRTCAudio Handle, FEOSNotificationId NotificationId);
private:
	static void Internal_OnUpdateSendingCallback(const EOS_RTCAudio_UpdateSendingCallbackInfo* Data);
	static void Internal_OnUpdateReceivingCallback(const EOS_RTCAudio_UpdateReceivingCallbackInfo* Data);
	static void Internal_UpdateSendingVolumeCallback(const EOS_RTCAudio_UpdateSendingVolumeCallbackInfo* Data);
	static void Internal_UpdateReceivingVolumeCallback(const EOS_RTCAudio_UpdateReceivingVolumeCallbackInfo* Data);
	static void Internal_UpdateParticipantVolumeCallback(const EOS_RTCAudio_UpdateParticipantVolumeCallbackInfo* Data);
protected:
	TMap<FEOSNotificationId, FAudioDevicesChangedCallback*> m_OnAudioDevicesChangedCallbacks;
	TMap<FEOSNotificationId, FAudioInputStateCallback*> m_OnAudioInputStateCallbacks;
	TMap<FEOSNotificationId, FAudioOutputStateCallback*> m_OnAudioOutputStateCallbacks;
	TMap<FEOSNotificationId, FAudioBeforeSendCallback*> m_OnAudioBeforeSendCallbacks;
	TMap<FEOSNotificationId, FAudioBeforeRenderCallback*> m_OnAudioBeforeRenderCallbacks;
	TMap<FEOSNotificationId, FParticipantUpdatedCallback*> m_OnParticipantUpdatedCallbacks;
};
