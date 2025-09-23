/**
* Copyright (C) 2017-2022 | eelDev AB
*
 official EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "EOSCoreModule.h"
#include "Presence/EOSPresenceTypes.h"
#include "EOSPresence.generated.h"

/**
 * The Presence methods allow you to query, read other player's presence information, as well as to modify your own.
 *
 * QueryPresence must be called once per login, per remote user, before data will be available. It is currently only possible to query presence for
 * users that are on your friends list, all other queries will return no results.
 *
 * @see EOS_Platform_GetPresenceInterface
 */

UCLASS()
class EOSCORE_API UCorePresence : public UEOSCoreSubsystem
{
	GENERATED_BODY()
public:
	virtual void Deinitialize() override;
public:
	/**
	 * The Presence methods allow you to query, read other player's presence information, as well as to modify your own.
	 *
	 * QueryPresence must be called once per login, per remote user, before data will be available. It is currently only possible to query presence for
	 * users that are on your friends list, all other queries will return no results.
	 *
	 * @see EOS_Platform_GetPresenceInterface
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Interfaces", meta = (WorldContext = "WorldContextObject"))
	static UCorePresence* GetPresence(UObject* WorldContextObject);

public:
	/**
	* Query a user's presence. This must complete successfully before CopyPresence will have valid results. If HasPresence returns true for a remote
	* user, this does not need to be called.
	*
	* @param Options Object containing properties related to who is querying presence and for what user
	* @param Callback Pointer to a function that handles receiving the completion information
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Presence", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Presence_QueryPresence"))
	void EOSPresenceQueryPresence(UObject* WorldContextObject, FEOSPresenceQueryPresenceOptions Options, const FOnPresenceQueryPresenceCompleteCallback& Callback);

	/**
	* Check if we already have presence for a user
	*
	* @param Options Object containing properties related to who is requesting presence and for what user
	* @return EOS_TRUE if we have presence for the requested user, or EOS_FALSE if the request was invalid or we do not have cached data
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Presence", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Presence_HasPresence"))
	static bool EOSPresenceHasPresence(UObject* WorldContextObject, FEOSPresenceHasPresenceOptions Options);

	/**
	* Get a user's cached presence object. If successful, this data must be released by calling EOS_Presence_Info_Release
	*
	* @param Options Object containing properties related to who is requesting presence and for what user
	* @param OutPresence A pointer to a pointer of Presence Info. If the returned result is success, this will be set to data that must be later released, otherwise this will be set to NULL
	* @return Success if we have cached data, or an error result if the request was invalid or we do not have cached data.
	*
	* @see EOS_Presence_Info_Release
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Presence", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Presence_CopyPresence"))
	static EOSResult EOSPresenceCopyPresence(UObject* WorldContextObject, FEOSPresenceCopyPresenceOptions Options, FEOSPresenceInfo& OutPresence);

	/**
	* Creates a presence modification handle. This handle can used to add multiple changes to your presence that can be applied with EOS_Presence_SetPresence.
	* The resulting handle must be released by calling EOS_PresenceModification_Release once it has been passed to EOS_Presence_SetPresence.
	*
	* @param Options Object containing properties related to the user modifying their presence
	* @param OutPresenceModificationHandle Pointer to a Presence Modification Handle to be set if successful
	* @return Success if we successfully created the Presence Modification Handle pointed at in OutPresenceModificationHandle, or an error result if the input data was invalid
	*
	* @see EOS_PresenceModification_Release
	* @see EOS_Presence_SetPresence
	* @see EOS_PresenceModification_SetStatus
	* @see EOS_PresenceModification_SetRawRichText
	* @see EOS_PresenceModification_SetData
	* @see EOS_PresenceModification_DeleteData
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Presence", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Presence_CreatePresenceModification"))
	static EOSResult EOSPresenceCreatePresenceModification(UObject* WorldContextObject, FEOSPresenceCreatePresenceModificationOptions Options, FEOSHPresenceModification& OutPresenceModificationHandle);

	/**
	* Sets your new presence with the data applied to a PresenceModificationHandle. The PresenceModificationHandle can be released safely after calling this function.
	*
	* @param Options Object containing a PresenceModificationHandle and associated user data
	* @param Callback Pointer to a function that handles receiving the completion information
	*
	* @see EOS_Presence_CreatePresenceModification
	* @see EOS_PresenceModification_Release
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Presence", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Presence_SetPresence"))
	void EOSPresenceSetPresence(UObject* WorldContextObject, FEOSPresenceSetPresenceOptions Options, const FOnPresenceSetPresenceCompleteCallback& Callback);

	/**
	* Register to receive notifications when presence changes. If the returned NotificationId is valid, you must call RemoveNotifyOnPresenceChanged when you no longer wish to
	* have your NotificationHandler called
	*
	* @param Callback The callback to be fired when a presence change occurs
	* @return Notification ID representing the registered callback if successful, an invalid NotificationId if not
	*
	* @see EOS_INVALID_NOTIFICATIONID
	* @see EOS_Presence_RemoveNotifyOnPresenceChanged
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Presence", meta = (AutoCreateRefTerm = "callback", WorldContext = "WorldContextObject", DisplayName = "EOS_Presence_AddNotifyOnPresenceChanged"))
	static FEOSNotificationId EOSPresenceAddNotifyOnPresenceChanged(UObject* WorldContextObject, const FOnPresenceChangedCallback& Callback);

	/**
	* Unregister a previously bound notification handler from receiving presence update notifications
	*
	* @param NotificationId The Notification ID representing the registered callback
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Presence", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Presence_RemoveNotifyOnPresenceChanged"))
	static void EOSPresenceRemoveNotifyOnPresenceChanged(UObject* WorldContextObject, FEOSNotificationId NotificationId);

	/**
	* Register to receive notifications when a user accepts a join game option via the social overlay.
	* @note must call RemoveNotifyJoinGameAccepted to remove the notification
	*
	* @param Callback A callback that is fired when a a notification is received.
	*
	* @return handle representing the registered callback
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Presence", meta = (AutoCreateRefTerm = "callback", WorldContext = "WorldContextObject", DisplayName = "EOS_Presence_AddNotifyJoinGameAccepted"))
	static FEOSNotificationId EOSPresenceAddNotifyJoinGameAccepted(UObject* WorldContextObject, const FOnPresenceOnJoinGameAcceptedCallback& Callback);

	/**
	* Unregister from receiving notifications when a user accepts a join game option via the social overlay.
	*
	* @param Id Handle representing the registered callback
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Presence", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Presence_RemoveNotifyJoinGameAccepted"))
	static void EOSPresenceRemoveNotifyJoinGameAccepted(UObject* WorldContextObject, FEOSNotificationId Id);

	/**
	* Gets a join info custom game-data string for a specific user. This is a helper function for reading the presence data related to how a user can be joined.
	* Its meaning is entirely application dependent.
	*
	* This value will be valid only after a QueryPresence call has successfully completed.
	*
	* @param Options Object containing an associated user
	* @param OutInfo The buffer into which the character data should be written.  The buffer must be long enough to hold a string of EOS_PRESENCEMODIFICATION_JOININFO_MAX_LENGTH.
	*
	* @return An EOS_EResult that indicates whether the location string was copied into the OutBuffer.
	*         EOS_Success if the information is available and passed out in OutBuffer
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_NotFound if there is user or the location string was not found.
	*         EOS_LimitExceeded - The OutBuffer is not large enough to receive the location string. InOutBufferLength contains the required minimum length to perform the operation successfully.
	*
	* @see EOS_PRESENCEMODIFICATION_JOININFO_MAX_LENGTH
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Presence", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Presence_GetJoinInfo"))
	static EOSResult EOSPresenceGetJoinInfo(UObject* WorldContextObject, FEOSPresenceGetJoinInfoOptions Options, FString& OutInfo);

	/**
	* To modify your own presence, you must call EOS_Presence_CreatePresenceModification to create a Presence Modification handle. To modify that handle, call
	* EOS_PresenceModification_* methods. Once you are finished, call EOS_Presence_SetPresence with your handle. You must then release your Presence Modification
	* handle by calling EOS_PresenceModification_Release.
	*/

	/**
	* Modifies a user's online status to be the new state.
	*
	* @param Options Object containing properties related to setting a user's Status
	* @return Success if modification was added successfully, otherwise an error code related to the problem
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Presence", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_PresenceModification_SetStatus"))
	static EOSResult EOSPresenceModificationSetStatus(UObject* WorldContextObject, const FEOSHPresenceModification& Handle, FEOSPresenceModificationSetStatusOptions Options);

	/**
	* Modifies a user's Rich Presence string to a new state. This is the exact value other users will see
	* when they query the local user's presence.
	*
	* @param Options Object containing properties related to setting a user's RichText string
	* @return Success if modification was added successfully, otherwise an error code related to the problem
	*
	* @see EOS_PRESENCE_RICH_TEXT_MAX_VALUE_LENGTH
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Presence", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_PresenceModification_SetRawRichText"))
	static EOSResult EOSPresenceModificationSetRawRichText(UObject* WorldContextObject, const FEOSHPresenceModification& Handle, FEOSPresenceModificationSetRawRichTextOptions Options);

	/**
	* Modifies one or more rows of user-defined presence data for a local user. At least one InfoData object
	* must be specified.
	*
	* @param Options Object containing an array of new presence data.
	* @return Success if modification was added successfully, otherwise an error code related to the problem
	*
	* @see EOS_PRESENCE_DATA_MAX_KEYS
	* @see EOS_PRESENCE_DATA_MAX_KEY_LENGTH
	* @see EOS_PRESENCE_DATA_MAX_VALUE_LENGTH
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Presence", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_PresenceModification_SetData"))
	static EOSResult EOSPresenceModificationSetData(UObject* WorldContextObject, const FEOSHPresenceModification& Handle, FEOSPresenceModificationSetDataOptions Options);

	/**
	* Removes one or more rows of user-defined presence data for a local user. At least one DeleteDataInfo object
	* must be specified.
	*
	* @param Options Object containing an array of new presence data.
	* @return Success if modification was added successfully, otherwise an error code related to the problem
	*
	* @see EOS_PRESENCE_DATA_MAX_KEYS
	* @see EOS_PRESENCE_DATA_MAX_KEY_LENGTH
	* @see EOS_PRESENCE_DATA_MAX_VALUE_LENGTH
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Presence", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_PresenceModification_DeleteData"))
	static EOSResult EOSPresenceModificationDeleteData(UObject* WorldContextObject, const FEOSHPresenceModification& Handle, FEOSPresenceModificationDeleteDataOptions Options);

	/**
	* Sets your new join info custom game-data string. This is a helper function for reading the presence data related to how a user can be joined.
	* Its meaning is entirely application dependent.
	*
	* @param Options Object containing a join info string and associated user data
	* @return Success if modification was added successfully, otherwise an error code related to the problem
	*
	* @see EOS_PRESENCEMODIFICATION_JOININFO_MAX_LENGTH
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Presence", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_PresenceModification_SetJoinInfo"))
	static EOSResult EOSPresenceModificationSetJoinInfo(UObject* WorldContextObject, const FEOSHPresenceModification& Handle, FEOSPresenceModificationSetJoinInfoOptions Options);

	/**
	 * Release the memory associated with an EOS_HPresenceModification handle. This must be called on Handles retrieved from EOS_Presence_CreatePresenceModification.
	 * This can be safely called on a NULL presence modification handle. This also may be safely called while a call to SetPresence is still pending.
	 *
	 * @param PresenceModificationHandle The presence modification handle to release
	 *
	 * @see EOS_Presence_CreatePresenceModification
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Presence", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_PresenceModification_Release"))
	static void EOSPresenceModificationRelease(UObject* WorldContextObject, const FEOSHPresenceModification& PresenceModificationHandle);
private:
	static void Internal_OnQueryPresenceCallback(const EOS_Presence_QueryPresenceCallbackInfo* Data);
	static void Internal_OnSetPresenceCallback(const EOS_Presence_SetPresenceCallbackInfo* Data);

protected:
	TMap<FEOSNotificationId, FPresenceChangedCallback*> m_PresenceChangedCallbacks;
	TMap<FEOSNotificationId, FPresenceOnJoinGameAcceptedCallback*> m_PresenceOnJoinGameAcceptedCallbacks;
};
