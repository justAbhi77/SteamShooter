/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "EOSCoreModule.h"
#include "Core/EOSTypes.h"
#include "EOSRTCTypes.h"
#include "EOSRTC.generated.h"

/**
* The RTC Interface is used to manage joining and leaving rooms.
*
* @see EOS_Platform_GetRTCInterface
*/

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UCoreRTC
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

UCLASS()
class EOSCORE_API UCoreRTC : public UEOSCoreSubsystem
{
	GENERATED_BODY()
public:
	virtual void Deinitialize() override;
public:
	/**
	* The RTC Interface is used to manage joining and leaving rooms.
	*
	* @see EOS_Platform_GetRTCInterface
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Interfaces", meta = (WorldContext = "WorldContextObject"))
	static UCoreRTC* GetRTC(UObject* WorldContextObject);

public:
	/**
	 * Get a handle to the Audio interface
	 * @return EOS_HRTCAudio handle
	 *
	 * @see eos_rtc_audio.h
	 * @see eos_rtc_audio_types.h
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTC", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTC_GetAudioInterface"))
	static FEOSHRTCAudio EOSRTCGetAudioInterface(UObject* WorldContextObject);

	/**
	 * Use this function to join a room.
	 *
	 * This function does not need to called for the Lobby RTC Room system; doing so will return EOS_AccessDenied. The lobby system will
	 * automatically join and leave RTC Rooms for all lobbies that have RTC rooms enabled.
	 *
	 * @param Options structure containing the parameters for the operation.
	 * @param Callback a callback that is fired when the async operation completes, either successfully or in error
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTC", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTC_JoinRoom"))
	void EOSRTCJoinRoom(UObject* WorldContextObject, FJoinRoomOptions Options, const FOnJoinRoomCallback& Callback);

	/**
	 * Use this function to leave a room and clean up all the resources associated with it. This function has to always be called when the
	 * room is abandoned even if the user is already disconnected for other reasons.
	 *
	 * This function does not need to called for the Lobby RTC Room system; doing so will return EOS_AccessDenied. The lobby system will
	 * automatically join and leave RTC Rooms for all lobbies that have RTC rooms enabled.
	 *
	 * @param Options structure containing the parameters for the operation.
	 * @param Callback a callback that is fired when the async operation completes, either successfully or in error
	 * @return EOS_Success if the operation succeeded
	 *         EOS_InvalidParameters if any of the parameters are incorrect
	 *         EOS_NotFound if not in the specified room
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTC", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTC_LeaveRoom"))
	void EOSRTCLeaveRoom(UObject* WorldContextObject, FLeaveRoomOptions Options, const FOnLeaveRoomCallback& Callback);

	/**
	 * Use this function to block a participant already connected to the room. After blocking them no media will be sent or received between
	 * that user and the local user. This method can be used after receiving the OnParticipantStatusChanged notification.
	 *
	 * @param Options structure containing the parameters for the operation.
	 * @param Callback a callback that is fired when the async operation completes, either successfully or in error
	 * @return EOS_Success if the operation succeeded
	 *         EOS_InvalidParameters if any of the parameters are incorrect
	 *         EOS_NotFound if either the local user or specified participant are not in the specified room
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTC", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTC_BlockParticipant"))
	void EOSRTCBlockParticipant(UObject* WorldContextObject, FBlockParticipantOptions Options, const FOnBlockParticipantCallback& Callback);

	/**
	 * Register to receive notifications when disconnected from the room. If the returned NotificationId is valid, you must call
	 * EOS_RTC_RemoveNotifyDisconnected when you no longer wish to have your Callback called.
	 *
	 * This function will always return EOS_INVALID_NOTIFICATIONID when used with lobby RTC room. To be notified of the connection
	 * status of a Lobby-managed RTC room, use the EOS_Lobby_AddNotifyRTCRoomConnectionChanged function instead.
	 *
	 * @param Callback The callback to be fired when a presence change occurs
	 * @return Notification ID representing the registered callback if successful, an invalid NotificationId if not
	 *
	 * @see EOS_INVALID_NOTIFICATIONID
	 * @see EOS_RTC_RemoveNotifyDisconnected
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTC", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTC_AddNotifyDisconnected"))
	static FEOSNotificationId EOSRTCAddNotifyDisconnected(UObject* WorldContextObject, FAddNotifyDisconnectedOptions Options, const FOnDisconnectedCallback& Callback);

	/**
	 * Unregister a previously bound notification handler from receiving room disconnection notifications
	 *
	 * @param NotificationId The Notification ID representing the registered callback
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTC", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTC_RemoveNotifyDisconnected"))
	static void EOSRTCRemoveNotifyDisconnected(UObject* WorldContextObject, FEOSNotificationId NotificationId);

	/**
	 * Register to receive notifications when a participant's status changes (e.g: join or leave the room). If the returned NotificationId is valid, you must call
	 * EOS_RTC_RemoveNotifyParticipantStatusChanged when you no longer wish to have your Callback called.
	 *
	 * If you register to this notification before joining a room, you will receive a notification for every member already in the room when you join said room.
	 * This allows you to know who is already in the room when you join.
	 *
	 * To be used effectively with a Lobby-managed RTC room, this should be registered during the EOS_Lobby_CreateLobby or EOS_Lobby_JoinLobby completion
	 * callbacks when the ResultCode is EOS_Success. If this notification is registered after that point, it is possible to miss notifications for
	 * already-existing room participants.
	 *
	 * @param Callback The callback to be fired when a presence change occurs
	 * @return Notification ID representing the registered callback if successful, an invalid NotificationId if not
	 *
	 * @note This notification is also raised when the local user joins the room, but NOT when the local user leaves the room.
	 *
	 * @see EOS_INVALID_NOTIFICATIONID
	 * @see EOS_RTC_RemoveNotifyParticipantStatusChanged
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTC", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTC_AddNotifyParticipantStatusChanged"))
	static FEOSNotificationId EOSRTCAddNotifyParticipantStatusChanged(UObject* WorldContextObject, FAddNotifyParticipantStatusChangedOptions Options, const FOnParticipantStatusChangedCallback& Callback);

	/**
	 * Unregister a previously bound notification handler from receiving participant status change notifications
	 *
	 * @param NotificationId The Notification ID representing the registered callback
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTC", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTC_RemoveNotifyParticipantStatusChanged"))
	static void EOSRTCRemoveNotifyParticipantStatusChanged(UObject* WorldContextObject, FEOSNotificationId NotificationId);

	/**
	 * Use this function to control settings.
	 *
	 * The available settings are documented as part of EOS_RTC_SetSettingOptions.
	 *
	 * @param Options structure containing the parameters for the operation
	 * @return EOS_Success when the setting is successfully set, EOS_NotFound when the setting is unknown, EOS_InvalidParameters when the value is invalid.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTC", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTC_SetSetting"))
	EOSResult EOSRTCSetSetting(UObject* WorldContextObject, FEOSRTCSetSettingOptions Options);
	
	/**
	 * Use this function to control settings for the specific room.
	 *
	 * The available settings are documented as part of EOS_RTC_SetRoomSettingOptions.
	 *
	 * @param Options structure containing the parameters for the operation
	 * @return EOS_Success when the setting is successfully set, EOS_NotFound when the setting is unknown, EOS_InvalidParameters when the value is invalid.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTC", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTC_SetRoomSetting"))
	EOSResult EOSRTCSetRoomSetting(UObject* WorldContextObject, FEOSRTCSetRoomSettingOptions Options);
	
	/**
	 * Register to receive notifications to receiving periodical statistics update. If the returned NotificationId is valid, you must call
	 * EOS_RTC_RemoveNotifyRoomStatisticsUpdated when you no longer wish to have your Callback called.
	 *
	 * @param Callback The callback to be fired when a statistics updated.
	 * @return Notification ID representing the registered callback if successful, an invalid NotificationId if not
	 *
	 * @see EOS_INVALID_NOTIFICATIONID
	 * @see EOS_RTC_RemoveNotifyRoomStatisticsUpdated
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTC", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTC_AddNotifyRoomStatisticsUpdated"))
	FEOSNotificationId EOSRTCAddNotifyRoomStatisticsUpdated(UObject* WorldContextObject, FEOSProductUserId LocalUserId, FString RoomName, const FOnRoomStatisticsUpdatedCallback& Callback);
	
	/**
	 * Unregister a previously bound notification handler from receiving periodical statistics update notifications
	 *
	 * @param NotificationId The Notification ID representing the registered callback
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTC", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTC_RemoveNotifyRoomStatisticsUpdated"))
	void EOSRTCRemoveNotifyRoomStatisticsUpdated(UObject* WorldContextObject, FEOSNotificationId NotificationId);
private:
	static void Internal_OnJoinRoomCallback(const EOS_RTC_JoinRoomCallbackInfo* Data);
	static void Internal_OnLeaveRoomCallback(const EOS_RTC_LeaveRoomCallbackInfo* Data);
	static void Internal_OnBlockParticipantCallback(const EOS_RTC_BlockParticipantCallbackInfo* Data);
protected:
	TMap<FEOSNotificationId, FDisconnectedCallback*> m_DisconnectedCallbacks;
	TMap<FEOSNotificationId, FRoomStatisticsUpdatedCallback*> m_RoomStatisticsUpdatedCallback;
	TMap<FEOSNotificationId, FParticipantStatusChangedCallback*> m_ParticipantStatusChangedCallbacks;
};
