/**
* Copyright (C) 2017-2022 | eelDev AB
*
 official EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "EOSCoreModule.h"
#include "eos_lobby_types.h"
#include "Lobby/EOSLobbyTypes.h"
#include "EOSLobby.generated.h"

/**
 * The Lobby Interface is used to manage lobbies that provide a persistent connection between users and
 * notifications of data sharing/updates.  Lobbies may also be found by advertising and searching with the backend service.
 * All Lobby Interface calls take a handle of type EOS_HLobby as the first parameter.
 * This handle can be retrieved from a EOS_HPlatform handle by using the EOS_Platform_GetLobbyInterface function.
 *
 * NOTE: At this time, this feature is only available for products that are part of the Epic Games store.
 *
 * @see EOS_Platform_GetLobbyInterface
 */

UCLASS()
class EOSCORE_API UCoreLobby : public UEOSCoreSubsystem
{
	GENERATED_BODY()
public:
	virtual void Deinitialize() override;
public:
	/**
	 * The Lobby Interface is used to manage lobbies that provide a persistent connection between users and
	 * notifications of data sharing/updates.  Lobbies may also be found by advertising and searching with the backend service.
	 * All Lobby Interface calls take a handle of type EOS_HLobby as the first parameter.
	 * This handle can be retrieved from a EOS_HPlatform handle by using the EOS_Platform_GetLobbyInterface function.
	 *
	 * NOTE: At this time, this feature is only available for products that are part of the Epic Games store.
	 *
	 * @see EOS_Platform_GetLobbyInterface
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Interfaces", meta = (WorldContext = "WorldContextObject"))
	static UCoreLobby* GetLobby(UObject* WorldContextObject);

public:
	/**
	* Creates a lobby and adds the user to the lobby membership.  There is no data associated with the lobby at the start and can be added vis EOS_Lobby_UpdateLobbyModification
	*
	* If the lobby is successfully created with an RTC Room enabled, the lobby system will automatically join and maintain the connection to the RTC room as long as the
	* local user remains in the lobby. Applications can use the EOS_Lobby_GetRTCRoomName to get the name of the RTC Room associated with a lobby, which may be used with
	* many of the EOS_RTC_* suite of functions. This can be useful to: register for notifications for talking status; to mute or unmute the local user's audio output;
	* to block or unblock room participants; to set local audio device settings; and more.
	*
	* @param Options Required fields for the creation of a lobby such as a user count and its starting advertised state
	* @param Callback A callback that is fired when the create operation completes, either successfully or in error
	*
	* @return EOS_Success if the creation completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_LimitExceeded if the number of allowed lobbies is exceeded
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_CreateLobby"))
	void EOSLobbyCreateLobby(UObject* WorldContextObject, FEOSLobbyCreateLobbyOptions Options, const FOnLobbyCreateLobbyCallback& Callback);

	/**
	* Destroy a lobby given a lobby ID
	*
	* @param Options Structure containing information about the lobby to be destroyed
	* @param Callback A callback that is fired when the destroy operation completes, either successfully or in error
	*
	* @return EOS_Success if the destroy completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_AlreadyPending if the lobby is already marked for destroy
	*         EOS_NotFound if the lobby to be destroyed does not exist
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_DestroyLobby"))
	void EOSLobbyDestroyLobby(UObject* WorldContextObject, FEOSLobbyDestroyLobbyOptions Options, const FOnLobbyDestroyLobbyCallback& Callback);

	/**
	* Join a lobby, creating a local instance under a given lobby ID.  Backend will validate various conditions to make sure it is possible to join the lobby.
	*
	* If the lobby is successfully join has an RTC Room enabled, the lobby system will automatically join and maintain the connection to the RTC room as long as the
	* local user remains in the lobby. Applications can use the EOS_Lobby_GetRTCRoomName to get the name of the RTC Room associated with a lobby, which may be used with
	* many of the EOS_RTC_* suite of functions. This can be useful to: register for notifications for talking status; to mute or unmute the local user's audio output;
	* to block or unblock room participants; to set local audio device settings; and more.
	*
	* @param Options Structure containing information about the lobby to be joined
	* @param Callback A callback that is fired when the join operation completes, either successfully or in error
	*
	* @return EOS_Success if the destroy completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_JoinLobby"))
	void EOSLobbyJoinLobby(UObject* WorldContextObject, FEOSLobbyJoinLobbyOptions Options, const FOnLobbyJoinLobbyCallback& Callback);

	/**
	 * This is a special case of EOS_Lobby_JoinLobby.  It should only be used if the lobby has had Join-by-ID enabled.
	 * Additionally, Join-by-ID should only be enabled to support native invites on an integrated platform.
	 *
	 * @param Options Structure containing information about the lobby to be joined
	 * @param Callback A callback that is fired when the join operation completes, either successfully or in error
	 *
	 * @return EOS_Success if the destroy completes successfully
	 *         EOS_InvalidParameters if any of the options are incorrect
	 *
	 * @see EOS_Lobby_JoinLobby
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_JoinLobbyById"))
	void EOSLobbyJoinLobbyById(UObject* WorldContextObject, FEOSLobbyJoinLobbyByIdOptions Options, const FOnJoinLobbyByIdCallback& Callback);

	/**
	* Leave a lobby given a lobby ID
	*
	* If the lobby you are leaving had an RTC Room enabled, leaving the lobby will also automatically leave the RTC room.
	*
	* @param Options Structure containing information about the lobby to be left
	* @param Callback A callback that is fired when the leave operation completes, either successfully or in error
	*
	* @return EOS_Success if the leave completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_AlreadyPending if the lobby is already marked for leave
	*         EOS_NotFound if a lobby to be left does not exist
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_LeaveLobby"))
	void EOSLobbyLeaveLobby(UObject* WorldContextObject, FEOSLobbyLeaveLobbyOptions Options, const FOnLobbyLeaveLobbyCallback& Callback);

	/**
	* Creates a lobby modification handle (EOS_HLobbyModification). The lobby modification handle is used to modify an existing lobby and can be applied with EOS_Lobby_UpdateLobby.
	* The EOS_HLobbyModification must be released by calling EOS_LobbyModification_Release once it is no longer needed.
	*
	* @param Options Required fields such as lobby ID
	* @param OutLobbyModificationHandle Pointer to a Lobby Modification Handle only set if successful
	* @return EOS_Success if we successfully created the Lobby Modification Handle pointed at in OutLobbyModificationHandle, or an error result if the input data was invalid
	*		   EOS_InvalidParameters if any of the options are incorrect
	*
	* @see EOS_LobbyModification_Release
	* @see EOS_Lobby_UpdateLobby
	* @see EOS_HLobbyModification
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_UpdateLobbyModification"))
	static EOSResult EOSLobbyUpdateLobbyModification(UObject* WorldContextObject, FEOSLobbyUpdateLobbyModificationOptions Options, FEOSHLobbyModification& OutLobbyModificationHandle);

	/**
	* Update a lobby given a lobby modification handle created by EOS_Lobby_UpdateLobbyModification
	*
	* @param Options Structure containing information about the lobby to be updated
	* @param Callback A callback that is fired when the update operation completes, either successfully or in error
	*
	* @return EOS_Success if the update completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_Lobby_NotOwner if the lobby modification contains modifications that are only allowed by the owner
	*         EOS_NotFound if the lobby to update does not exist
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_UpdateLobby"))
	void EOSLobbyUpdateLobby(UObject* WorldContextObject, FEOSLobbyUpdateLobbyOptions Options, const FOnLobbyUpdateLobbyCallback& Callback);

	/**
	* Promote an existing member of the lobby to owner, allowing them to make lobby data modifications
	*
	* @param Options Structure containing information about the lobby and member to be promoted
	* @param Callback A callback that is fired when the promotion operation completes, either successfully or in error
	*
	* @return EOS_Success if the promote completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_Lobby_NotOwner if the calling user is not the owner of the lobby
	*         EOS_NotFound if the lobby of interest does not exist
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_PromoteMember"))
	void EOSLobbyPromoteMember(UObject* WorldContextObject, FEOSLobbyPromoteMemberOptions Options, const FOnLobbyPromoteMemberCallback& Callback);

	/**
	* Kick an existing member from the lobby
	*
	* @param Options Structure containing information about the lobby and member to be kicked
	* @param Callback A callback that is fired when the kick operation completes, either successfully or in error
	*
	* @return EOS_Success if the kick completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_Lobby_NotOwner if the calling user is not the owner of the lobby
	*         EOS_NotFound if a lobby of interest does not exist
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_KickMember"))
	void EOSLobbyKickMember(UObject* WorldContextObject, FEOSLobbyKickMemberOptions Options, const FOnLobbyKickMemberCallback& Callback);

	/**
	 * Hard mute an existing member in the lobby, can't speak but can hear other members of the lobby
	 *
	 * @param Options Structure containing information about the lobby and member to be hard muted
	 * @param Callback A callback that is fired when the hard mute operation completes, either successfully or in error
	 *
	 * @return EOS_Success if the hard mute completes successfully
	 *         EOS_IncompatibleVersion if the API version passed in is incorrect
	 *         EOS_InvalidParameters if any of the options are incorrect
	 *         EOS_Invalid_ProductUserID if a target user is incorrect
	 *         EOS_NotFound if lobby or target user cannot be found
	 *         EOS_Lobby_VoiceNotEnabled if lobby has no voice enabled
	 *         EOS_Lobby_NotOwner if the calling user is not the owner of the lobby
	 *         EOS_NotFound if a lobby of interest does not exist
	 *         EOS_AlreadyPending if the user is already marked for hard mute
	 *         EOS_TooManyRequests if there are too many requests
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_HardMuteMember"))
	void EOSLobbyHardMuteMember(UObject* WorldContextObject, FEOSLobbyHardMuteMemberOptions Options, const FOnHardMuteMemberCallback& Callback);

	/**
	* Register to receive notifications when a lobby owner updates the attributes associated with the lobby.
	* @note must call RemoveNotifyLobbyUpdateReceived to remove the notification
	*
	* @param Callback A callback that is fired when a a notification is received.
	*
	* @return handle representing the registered callback
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_AddNotifyLobbyUpdateReceived"))
	static FEOSNotificationId EOSLobbyAddNotifyLobbyUpdateReceived(UObject* WorldContextObject, const FOnLobbyUpdateReceivedCallback& Callback);

	/**
	* Unregister from receiving notifications when a lobby changes its data.
	*
	* @param Id Handle representing the registered callback
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_RemoveNotifyLobbyUpdateReceived"))
	static void EOSLobbyRemoveNotifyLobbyUpdateReceived(UObject* WorldContextObject, FEOSNotificationId Id);

	/**
	* Register to receive notifications when a lobby member updates the attributes associated with themselves inside the lobby.
	* @note must call RemoveNotifyLobbyMemberUpdateReceived to remove the notification
	*
	* @param Callback A callback that is fired when a a notification is received.
	*
	* @return handle representing the registered callback
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_AddNotifyLobbyMemberUpdateReceived"))
	static FEOSNotificationId EOSLobbyAddNotifyLobbyMemberUpdateReceived(UObject* WorldContextObject, const FOnLobbyMemberUpdateReceivedCallback& Callback);

	/**
	* Unregister from receiving notifications when lobby members change their data.
	*
	* @param Id Handle representing the registered callback
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_RemoveNotifyLobbyMemberUpdateReceived"))
	static void EOSLobbyRemoveNotifyLobbyMemberUpdateReceived(UObject* WorldContextObject, FEOSNotificationId Id);

	/**
	* Register to receive notifications about the changing status of lobby members.
	* @note must call RemoveNotifyLobbyMemberStatusReceived to remove the notification
	*
	* @param Callback A callback that is fired when a a notification is received.
	*
	* @return handle representing the registered callback
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_AddNotifyLobbyMemberStatusReceived"))
	static FEOSNotificationId EOSLobbyAddNotifyLobbyMemberStatusReceived(UObject* WorldContextObject, const FOnLobbyMemberStatusReceivedCallback& Callback);

	/**
	* Unregister from receiving notifications when lobby members status change.
	*
	* @param Id Handle representing the registered callback
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_RemoveNotifyLobbyMemberStatusReceived"))
	static void EOSLobbyRemoveNotifyLobbyMemberStatusReceived(UObject* WorldContextObject, FEOSNotificationId Id);

	/**
	* Send an invite to another user.  User must be a member of the lobby or else the call will fail
	*
	* @param Options Structure containing information about the lobby and user to invite
	* @param Callback A callback that is fired when the send invite operation completes, either successfully or in error
	*
	* @return EOS_Success if the send invite completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_NotFound if the lobby to send the invite from does not exist
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_SendInvite"))
	void EOSLobbySendInvite(UObject* WorldContextObject, FEOSLobbySendInviteOptions Options, const FOnLobbySendInviteCallback& Callback);

	/**
	* Reject an invite from another user.
	*
	* @param Options Structure containing information about the invite to reject
	* @param Callback A callback that is fired when the reject invite operation completes, either successfully or in error
	*
	* @return EOS_Success if the invite rejection completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_NotFound if the invite does not exist
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_RejectInvite"))
	void EOSLobbyRejectInvite(UObject* WorldContextObject, FEOSLobbyRejectInviteOptions Options, const FOnLobbyRejectInviteCallback& Callback);

	/**
	* Retrieve all existing invites for a single user
	*
	* @param Options Structure containing information about the invites to query
	* @param Callback A callback that is fired when the query invites operation completes, either successfully or in error
	*
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_QueryInvites"))
	void EOSLobbyQueryInvites(UObject* WorldContextObject, FEOSLobbyQueryInvitesOptions Options, const FOnLobbyQueryInvitesCallback& Callback);

	/**
	* Get the number of known invites for a given user
	*
	* @param Options the Options associated with retrieving the current invite count
	*
	* @return number of known invites for a given user or 0 if there is an error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_GetInviteCount"))
	static int32 EOSLobbyGetInviteCount(UObject* WorldContextObject, FEOSLobbyGetInviteCountOptions Options);

	/**
	* Retrieve an invite ID from a list of active invites for a given user
	*
	* @param Options Structure containing the input parameters
	*
	* @return EOS_Success if the input is valid and an invite ID was returned
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_NotFound if the invite doesn't exist
	*
	* @see EOS_Lobby_GetInviteCount
	* @see EOS_Lobby_CopyLobbyDetailsHandleByInviteId
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_GetInviteIdByIndex"))
	static EOSResult EOSLobbyGetInviteIdByIndex(UObject* WorldContextObject, FEOSLobbyGetInviteIdByIndexOptions Options, FString& OutInviteId);

	/**
	* Create a lobby search handle.  This handle may be modified to include various search parameters.
	* Searching is possible in three methods, all mutually exclusive
	* - set the lobby ID to find a specific lobby
	* - set the target user ID to find a specific user
	* - set lobby parameters to find an array of lobbies that match the search criteria (not available yet)
	*
	* @param Options Structure containing required parameters such as the maximum number of search results
	* @param OutLobbySearchHandle The new search handle or null if there was an error creating the search handle
	*
	* @return EOS_Success if the search creation completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_CreateLobbySearch"))
	static EOSResult EOSLobbyCreateLobbySearch(UObject* WorldContextObject, FEOSLobbyCreateLobbySearchOptions Options, FEOSHLobbySearch& OutLobbySearchHandle);

	/**
	* Register to receive notifications about lobby invites sent to local users.
	* @note must call RemoveNotifyLobbyInviteReceived to remove the notification
	*
	* @param Callback A callback that is fired when a a notification is received.
	*
	* @return handle representing the registered callback
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_AddNotifyLobbyInviteReceived"))
	static FEOSNotificationId EOSLobbyAddNotifyLobbyInviteReceived(UObject* WorldContextObject, const FOnLobbyInviteReceivedCallback& Callback);

	/**
	* Unregister from receiving notifications when a user receives a lobby invitation.
	*
	* @param Id Handle representing the registered callback
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_RemoveNotifyLobbyInviteReceived"))
	static void EOSLobbyRemoveNotifyLobbyInviteReceived(UObject* WorldContextObject, FEOSNotificationId Id);

	/**
	 * Register to receive notifications about lobby invites accepted by local user via the overlay.
	 * @note must call RemoveNotifyLobbyInviteAccepted to remove the notification
	 *
	 * @param Options Structure containing information about the request.
	 * @param Callback A callback that is fired when a a notification is received.
	 *
	 * @return handle representing the registered callback
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_AddNotifyLobbyInviteAccepted"))
	static FEOSNotificationId EOSLobbyAddNotifyLobbyInviteAccepted(UObject* WorldContextObject, FEOSLobbyAddNotifyLobbyInviteAcceptedOptions Options, const FOnLobbyInviteAcceptedCallback& Callback);

	/**
	* Unregister from receiving notifications when a user accepts a lobby invitation via the overlay.
	*
	* @param Id Handle representing the registered callback
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_RemoveNotifyLobbyInviteAccepted"))
	static void EOSLobbyRemoveNotifyLobbyInviteAccepted(UObject* WorldContextObject, FEOSNotificationId Id);

	/**
	* Register to receive notifications about lobby invites accepted by local user via the overlay.
	* @note must call RemoveNotifyLobbyInviteAccepted to remove the notification
	*
	* @param Options Structure containing information about the request.
	* @param Callback A callback that is fired when a a notification is received.
	*
	* @return handle representing the registered callback
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_AddNotifyJoinLobbyAccepted"))
	static FEOSNotificationId EOSLobbyAddNotifyJoinLobbyAccepted(UObject* WorldContextObject, FEOSLobbyAddNotifyJoinLobbyAcceptedOptions Options, const FOnJoinLobbyAcceptedCallback& Callback);

	/**
	 * Register to receive notifications about lobby join game accepted by local user via the overlay.
	 * Register to receive notifications about lobby invites rejected by local user via the overlay.
	 * @note must call RemoveNotifyLobbyInviteRejected to remove the notification
	 *
	 * @param Options Structure containing information about the request.
	 * @param Callback A callback that is fired when a a notification is received.
	 *
	 * @return handle representing the registered callback
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_AddNotifyLobbyInviteRejected"))
	static FEOSNotificationId EOSLobbyAddNotifyLobbyInviteRejected(UObject* WorldContextObject, FEOSLobbyAddNotifyLobbyInviteRejectedOptions Options, const FOnLobbyInviteRejectedCallback& Callback);

	/**
	 * Unregister from receiving notifications when a user rejects a lobby invitation via the overlay.
	 *
	 * @param Id Handle representing the registered callback
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_RemoveNotifyLobbyInviteRejected"))
	static void EOSLobbyRemoveNotifyLobbyInviteRejected(UObject* WorldContextObject, FEOSNotificationId Id);

	/**
	* Unregister from receiving notifications when a user accepts a lobby invitation via the overlay.
	*
	* @param Id Handle representing the registered callback
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_RemoveNotifyJoinLobbyAccepted"))
	static void EOSLobbyRemoveNotifyJoinLobbyAccepted(UObject* WorldContextObject, FEOSNotificationId Id);

	/**
	 * Register to receive notifications about a lobby "INVITE" performed by a local user via the overlay.
	 * This is only needed when a configured integrated platform has EOS_IPMF_DisableSDKManagedSessions set.  The EOS SDK will
	 * then use the state of EOS_IPMF_PreferEOSIdentity and EOS_IPMF_PreferIntegratedIdentity to determine when the Callback is
	 * called.
	 *
	 * @note must call EOS_Lobby_RemoveNotifySendLobbyNativeInviteRequested to remove the notification.
	 *
	 * @param Options Structure containing information about the request.
	 * @param Callback A callback that is fired when a notification is received.
	 *
	 * @return handle representing the registered callback
	 *
	 * @see EOS_IPMF_DisableSDKManagedSessions
	 * @see EOS_IPMF_PreferEOSIdentity
	 * @see EOS_IPMF_PreferIntegratedIdentity
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_AddNotifySendLobbyNativeInviteRequested"))
	static FEOSNotificationId EOSLobbyAddNotifySendLobbyNativeInviteRequested(UObject* WorldContextObject, FEOSLobbyAddNotifySendLobbyNativeInviteRequestedOptions Options, const FOnSendLobbyNativeInviteRequestedCallback& Callback);

	/**
	 * Unregister from receiving notifications when a user requests a send invite via the overlay.
	 *
	 * @param Id Handle representing the registered callback
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_RemoveNotifySendLobbyNativeInviteRequested"))
	static void EOSLobbyRemoveNotifySendLobbyNativeInviteRequested(UObject* WorldContextObject, FEOSNotificationId Id);
	
	/**
	* EOS_Lobby_CopyLobbyDetailsHandleByInviteId is used to immediately retrieve a handle to the lobby information from after notification of an invite
	* If the call returns an EOS_Success result, the out parameter, OutLobbyDetailsHandle, must be passed to EOS_LobbyDetails_Release to release the memory associated with it.
	*
	* @param Options Structure containing the input parameters
	* @param OutLobbyDetailsHandle out parameter used to receive the lobby handle
	*
	* @return EOS_Success if the information is available and passed out in OutLobbyDetailsHandle
	*         EOS_InvalidParameters if you pass an invalid invite ID or a null pointer for the out parameter
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*         EOS_NotFound If the invite ID cannot be found
	*
	* @see EOS_Lobby_CopyLobbyDetailsHandleByInviteIdOptions
	* @see EOS_LobbyDetails_Release
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_CopyLobbyDetailsHandleByInviteId"))
	static EOSResult EOSLobbyCopyLobbyDetailsHandleByInviteId(UObject* WorldContextObject, FEOSLobbyCopyLobbyDetailsHandleByInviteIdOptions Options, FEOSHLobbyDetails& OutLobbyDetailsHandle);

	/**
	* EOS_Lobby_CopyLobbyDetailsHandleByUiEventId is used to immediately retrieve a handle to the lobby information from after notification of an join game
	* If the call returns an EOS_Success result, the out parameter, OutLobbyDetailsHandle, must be passed to EOS_LobbyDetails_Release to release the memory associated with it.
	*
	* @param Options Structure containing the input parameters
	* @param OutLobbyDetailsHandle out parameter used to receive the lobby handle
	*
	* @return EOS_Success if the information is available and passed out in OutLobbyDetailsHandle
	*         EOS_InvalidParameters if you pass an invalid ui event ID
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*         EOS_NotFound If the invite ID cannot be found
	*
	* @see EOS_Lobby_CopyLobbyDetailsHandleByUiEventIdOptions
	* @see EOS_LobbyDetails_Release
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_CopyLobbyDetailsHandleByUiEventId"))
	static EOSResult EOSLobbyCopyLobbyDetailsHandleByUiEventId(UObject* WorldContextObject, FEOSLobbyCopyLobbyDetailsHandleByUiEventIdOptions Options, FEOSHLobbyDetails& OutLobbyDetailsHandle);

	
	/**
	* Create a handle to an existing lobby.
	* If the call returns an EOS_Success result, the out parameter, OutLobbyDetailsHandle, must be passed to EOS_LobbyDetails_Release to release the memory associated with it.
	*
	* @param Options Structure containing information about the lobby to retrieve
	* @param OutLobbyDetailsHandle The new active lobby handle or null if there was an error
	*
	* @return EOS_Success if the lobby handle was created successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*         EOS_NotFound if the lobby doesn't exist
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_CopyLobbyDetailsHandle"))
	static EOSResult EOSLobbyCopyLobbyDetailsHandle(UObject* WorldContextObject, FEOSLobbyCopyLobbyDetailsHandleOptions Options, FEOSHLobbyDetails& OutLobbyDetailsHandle);

	/**
	 * Get the name of the RTC room associated with a specific lobby a local user belongs to.
	 *
	 * This value can be used whenever you need a RoomName value in the RTC_* suite of functions. RTC Room Names must not be used with
	 * EOS_RTC_JoinRoom, EOS_RTC_LeaveRoom, or EOS_RTC_AddNotifyDisconnected. Doing so will return EOS_AccessDenied or
	 * EOS_INVALID_NOTIFICATIONID if used with those functions.
	 *
	 * This function will only succeed when called on a lobby the local user is currently a member of.
	 *
	 * @param Options Structure containing information about the RTC room name to retrieve
	 * @param OutBuffer The buffer to store the null-terminated room name string within
	 * @param InOutBufferLength In: The maximum amount of writable chars in OutBuffer, Out: The minimum amount of chars needed in OutBuffer to store the RTC room name (including the null-terminator)
	 *
	 * @return EOS_Success if a room exists for the specified lobby, there was enough space in OutBuffer, and the name was written successfully
	 *         EOS_NotFound if the lobby does not exist
	 *         EOS_Disabled if the lobby exists, but did not have the RTC Room feature enabled when created
	 *         EOS_InvalidParameters if you pass a null pointer on invalid length for any of the parameters
	 *         EOS_LimitExceeded The OutBuffer is not large enough to receive the room name. InOutBufferLength contains the required minimum length to perform the operation successfully.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_GetRTCRoomName"))
	static EOSResult EOSLobbyGetRTCRoomName(UObject* WorldContextObject, FEOSLobbyGetRTCRoomNameOptions Options, FString& OutBuffer, int32 InOutBufferLength = 256);

	/**
	 * Get the current connection status of the RTC Room for a lobby.
	 *
	 * The RTC Room connection status is independent of the lobby connection status, however the lobby system will attempt to keep
	 * them consistent, automatically connecting to the RTC room after joining a lobby which has an associated RTC room and disconnecting
	 * from the RTC room when a lobby is left or disconnected.
	 *
	 * This function will only succeed when called on a lobby the local user is currently a member of.
	 *
	 * @param Options Structure containing information about the lobby to query the RTC Room connection status for
	 * @param bOutIsConnected If the result is EOS_Success, this will be set to EOS_TRUE if we are connected, or EOS_FALSE if we are not yet connected.
	 *
	 * @return EOS_Success if we are connected to the specified lobby, the input options and parameters were valid and we were able to write to bOutIsConnected successfully.
	 *         EOS_NotFound if the lobby doesn't exist
	 *         EOS_Disabled if the lobby exists, but did not have the RTC Room feature enabled when created
	 *         EOS_InvalidParameters if bOutIsConnected is NULL, or any other parameters are NULL or invalid
	 *
	 * @see EOS_Lobby_AddNotifyRTCRoomConnectionChanged
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_IsRTCRoomConnected"))
	static EOSResult EOSLobbyIsRTCRoomConnected(UObject* WorldContextObject, FEOSLobbyIsRTCRoomConnectedOptions Options, bool& bOutIsConnected);

	/**
	 * Register to receive notifications of when the RTC Room for a particular lobby has a connection status change.
	 *
	 * The RTC Room connection status is independent of the lobby connection status, however the lobby system will attempt to keep
	 * them consistent, automatically connecting to the RTC room after joining a lobby which has an associated RTC room and disconnecting
	 * from the RTC room when a lobby is left or disconnected.
	 *
	 * This notification is entirely informational and requires no action in response by the application. If the connected status is offline
	 * (bIsConnected is EOS_FALSE), the connection will automatically attempt to reconnect. The purpose of this notification is to allow
	 * applications to show the current connection status of the RTC room when the connection is not established.
	 *
	 * Unlike EOS_RTC_AddNotifyDisconnected, EOS_RTC_LeaveRoom should not be called when the RTC room is disconnected.
	 *
	 * This function will only succeed when called on a lobby the local user is currently a member of.
	 *
	 * @param Options Structure containing information about the lobby to receive updates about
	 * @param Callback The function to call if the RTC Room's connection status changes
	 *
	 * @return A valid notification ID if the Callback was successfully registered, or EOS_INVALID_NOTIFICATIONID if the input was invalid, the lobby did not exist, or the lobby did not have an RTC room.
	 *
	 * @see EOS_Lobby_RemoveNotifyRTCRoomConnectionChanged
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_AddNotifyRTCRoomConnectionChanged"))
	static FEOSNotificationId EOSLobbyAddNotifyRTCRoomConnectionChanged(UObject* WorldContextObject, FEOSLobbyAddNotifyRTCRoomConnectionChangedOptions Options, const FOnRTCRoomConnectionChangedCallback& Callback);

	/**
	 * Unregister from receiving notifications when an RTC Room's connection status changes.
	 *
	 * This should be called when the local user is leaving a lobby.
	 *
	 * @param Id Handle representing the registered callback
	 *
	 * @see EOS_Lobby_AddNotifyRTCRoomConnectionChanged
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_RemoveNotifyRTCRoomConnectionChanged"))
	static void EOSLobbyRemoveNotifyRTCRoomConnectionChanged(UObject* WorldContextObject, FEOSNotificationId Id);

	/**
	 * Get the Connection string for an EOS lobby. The connection string describes the presence of a player in terms of game state.
	 * Xbox platforms expect titles to embed this into their MultiplayerActivity at creation.
	 * When present, the SDK will use this value to populate session presence in the social overlay and facilitate platform invitations.
	 *
	 * @param Options Structure containing the input parameters. API version, the LobbyID of the lobby to generate the string from and the PUID of the requesting user.
	 * @param OutBuffer The buffer to store the null-terminated ConnectString within
	 *
	 * @return EOS_Success if retrieving the string was successful.
	 *         EOS_InvalidParameters if the OutBuffer or InOutBufferLength are null.
	 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
	 *         EOS_NotFound if no lobby is found matching the LobbyID and PUID provided.
	 *         EOS_LimitExceeded if the provided InOutBufferLength is too small to contain the resulting string.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_GetConnectString"))
	static EOSResult EOSLobbyGetConnectString(UObject* WorldContextObject, FEOSLobbyGetConnectStringOptions Options, FString& OutBuffer);
	
	/**
	 * Parse the ConnectString for an EOS lobby invitation to extract just the lobby ID.
	 * Used for joining a lobby from a connection string (as generated by GetConnectString) found in a platform invitation or presence.
	 *
	 * @param Options Structure containing the input parameters. API version and ConnectString.
	 * @param OutBuffer The buffer to store the null-terminated lobby ID within
	 *
	 * @return EOS_Success if retrieving the string was successful.
	 *         EOS_InvalidParameters if the OutBuffer or InOutBufferLength are null.
	 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
	 *         EOS_LimitExceeded if the provided InOutBufferLength is too small to contain the resulting string.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_ParseConnectString"))
	static EOSResult EOSLobbyParseConnectString(UObject* WorldContextObject, FEOSLobbyParseConnectStringOptions Options, FString& OutBuffer);
	
	/**
	 * Register to receive notifications about leave lobby requests performed by the local user via the overlay.
	 * When user requests to leave the lobby in the social overlay, the SDK does not automatically leave the lobby, it is up to the game to perform any necessary cleanup and call the EOS_Lobby_LeaveLobby method using the lobbyId sent in the notification function.
	 * @note must call EOS_Lobby_RemoveNotifyLeaveLobbyRequested to remove the notification.
	 *
	 * @param Options Structure containing information about the request.
	 * @param Callback A callback that is fired when a notification is received.
	 *
	 * @return handle representing the registered callback
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_AddNotifyLeaveLobbyRequested"))
	static FEOSNotificationId EOSLobbyAddNotifyLeaveLobbyRequested(UObject* WorldContextObject, const FOnLeaveLobbyRequestedCallback& Callback);
	
	/**
	 * Unregister from receiving notifications when a user performs a leave lobby action via the overlay.
	 *
	 * @param InId Handle representing the registered callback
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Lobby_RemoveNotifyLeaveLobbyRequested"))
	static void EOSLobbyRemoveNotifyLeaveLobbyRequested(UObject* WorldContextObject, FEOSNotificationId InId);

	/**
	 * To modify lobbies or the lobby member data, you must call EOS_Lobby_UpdateLobbyModification to create a Lobby Modification handle. To modify that handle, call
	 * FEOSHLobbyModification methods. Once you are finished, call EOS_Lobby_UpdateLobby with your handle. You must then release your Lobby Modification
	 * handle by calling EOSLobbyModificationRelease.
	 */

	/**
	* Set the bucket ID associated with this lobby.
	* Values such as region, game mode, etc can be combined here depending on game need.
	* Setting this is strongly recommended to improve search performance.
	*
	* @param Options Options associated with the bucket ID of the lobby
	*
	* @return EOS_Success if setting this parameter was successful
	*         EOS_InvalidParameters if the bucket ID is invalid or null
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbyModification_SetBucketId"))
	static EOSResult EOSLobbyModificationSetBucketId(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FEOSLobbyModificationSetBucketIdOptions Options);

	/**
	* Set the permissions associated with this lobby.
	* The permissions range from "public" to "invite only" and are described by EOS_ELobbyPermissionLevel
	*
	* @param Options Options associated with the permission level of the lobby
	*
	* @return EOS_Success if setting this parameter was successful
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbyModification_SetPermissionLevel"))
	static EOSResult EOSLobbyModificationSetPermissionLevel(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FEOSLobbyModificationSetPermissionLevelOptions Options);

	/**
	* Set the maximum number of members allowed in this lobby.
	* When updating the lobby, it is not possible to reduce this number below the current number of existing members
	*
	* @param Options Options associated with max number of members in this lobby
	*
	* @return EOS_Success if setting this parameter was successful
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbyModification_SetMaxMembers"))
	static EOSResult EOSLobbyModificationSetMaxMembers(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FEOSLobbyModificationSetMaxMembersOptions Options);

	/**
	* Allows enabling or disabling invites for this lobby.
	* The lobby will also need to have `bPresenceEnabled` true.
	*
	* @param Options Options associated with invites allowed flag for this lobby.
	*
	* @return EOS_Success if setting this parameter was successful
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbyModification_SetInvitesAllowed"))
	static EOSResult EOSLobbyModificationSetInvitesAllowed(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FEOSLobbyModificationSetInvitesAllowedOptions Options);

	/**
	* Associate an attribute with this lobby
	* An attribute is something may be public or private with the lobby.
	* If public, it can be queried for in a search, otherwise the data remains known only to lobby members
	*
	* @param Options Options to set the attribute and its visibility state
	*
	* @return EOS_Success if setting this parameter was successful
	*		   EOS_InvalidParameters if the attribute is missing information or otherwise invalid
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbyModification_AddAttribute"))
	static EOSResult EOSLobbyModificationAddAttribute(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FEOSLobbyModificationAddAttributeOptions Options);

	/**
	 * Associate an attribute with this lobby
	 * An attribute is something may be public or private with the lobby.
	 * If public, it can be queried for in a search, otherwise the data remains known only to lobby members
	 *
	 * @return EOS_Success if setting this parameter was successful
	 *		   EOS_InvalidParameters if the attribute is missing information or otherwise invalid
	 *         EOS_IncompatibleVersion if the API version passed in is incorrect
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbyModification_AddAttributeBool"))
	static EOSResult EOSLobbyModificationAddAttributeBool(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FString Key, bool bValue, EEOSELobbyAttributeVisibility Visibility);
	/**
	 * Associate an attribute with this lobby
	 * An attribute is something may be public or private with the lobby.
	 * If public, it can be queried for in a search, otherwise the data remains known only to lobby members
	 *
	 * @return EOS_Success if setting this parameter was successful
	 *		   EOS_InvalidParameters if the attribute is missing information or otherwise invalid
	 *         EOS_IncompatibleVersion if the API version passed in is incorrect
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbyModification_AddMemberAttribute"))
	static EOSResult EOSLobbyModificationAddAttributeDouble(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FString Key, FString Value, EEOSELobbyAttributeVisibility Visibility);
	/**
	* Associate an attribute with this lobby
	* An attribute is something may be public or private with the lobby.
	* If public, it can be queried for in a search, otherwise the data remains known only to lobby members
	*
	* @return EOS_Success if setting this parameter was successful
	*		   EOS_InvalidParameters if the attribute is missing information or otherwise invalid
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbyModification_AddAttributeInt64"))
	static EOSResult EOSLobbyModificationAddAttributeInt64(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FString Key, FString Value, EEOSELobbyAttributeVisibility Visibility);

	/**
	  * Associate an attribute with this lobby
	  * An attribute is something may be public or private with the lobby.
	  * If public, it can be queried for in a search, otherwise the data remains known only to lobby members
	  *
	  * @return EOS_Success if setting this parameter was successful
	  *		   EOS_InvalidParameters if the attribute is missing information or otherwise invalid
	  *         EOS_IncompatibleVersion if the API version passed in is incorrect
	  */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbyModification_AddAttributeString"))
	static EOSResult EOSLobbyModificationAddAttributeString(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FString Key, FString Value, EEOSELobbyAttributeVisibility Visibility);

	/**
	* Remove an attribute associated with the lobby
	*
	* @param Options Specify the key of the attribute to remove
	*
	* @return EOS_Success if removing this parameter was successful
	*		   EOS_InvalidParameters if the key is null or empty
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbyModification_RemoveAttribute"))
	static EOSResult EOSLobbyModificationRemoveAttribute(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FEOSLobbyModificationRemoveAttributeOptions Options);

	/**
	* Associate an attribute with a member of the lobby
	* Lobby member data is always private to the lobby
	*
	* @param Options Options to set the attribute and its visibility state
	*
	* @return EOS_Success if setting this parameter was successful
	*		   EOS_InvalidParameters if the attribute is missing information or otherwise invalid
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*/
	static EOSResult EOSLobbyModificationAddMemberAttribute(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FEOSLobbyModificationAddMemberAttributeOptions Options);

	/**
	 * Associate an attribute with a member of the lobby
	 * Lobby member data is always private to the lobby
	 *
	 * @return EOS_Success if setting this parameter was successful
	 *		   EOS_InvalidParameters if the attribute is missing information or otherwise invalid
	 *         EOS_IncompatibleVersion if the API version passed in is incorrect
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbyModification_AddMemberAttributeBool"))
	static EOSResult EOSLobbyModificationAddMemberAttributeBool(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FString Key, bool bValue, EEOSELobbyAttributeVisibility Visibility);

	/**
	  * Associate an attribute with a member of the lobby
	  * Lobby member data is always private to the lobby
	  *
	  * @return EOS_Success if setting this parameter was successful
	  *		   EOS_InvalidParameters if the attribute is missing information or otherwise invalid
	  *         EOS_IncompatibleVersion if the API version passed in is incorrect
	  */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbyModification_AddMemberAttributeDouble"))
	static EOSResult EOSLobbyModificationAddMemberAttributeDouble(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FString Key, FString Value, EEOSELobbyAttributeVisibility Visibility);
	/**
	* Associate an attribute with a member of the lobby
	* Lobby member data is always private to the lobby
	*
	* @return EOS_Success if setting this parameter was successful
	*		   EOS_InvalidParameters if the attribute is missing information or otherwise invalid
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbyModification_AddMemberAttributeInt64"))
	static EOSResult EOSLobbyModificationAddMemberAttributeInt64(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FString Key, FString Value, EEOSELobbyAttributeVisibility Visibility);

	/**
	 * Associate an attribute with a member of the lobby
	 * Lobby member data is always private to the lobby
	 *
	 * @return EOS_Success if setting this parameter was successful
	 *		   EOS_InvalidParameters if the attribute is missing information or otherwise invalid
	 *         EOS_IncompatibleVersion if the API version passed in is incorrect
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbyModification_AddMemberAttributeString"))
	static EOSResult EOSLobbyModificationAddMemberAttributeString(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FString Key, FString Value, EEOSELobbyAttributeVisibility Visibility);

	/**
	* Remove an attribute associated with of member of the lobby
	*
	* @param Options Specify the key of the member attribute to remove
	*
	* @return EOS_Success if removing this parameter was successful
	*		   EOS_InvalidParameters if the key is null or empty
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbyModification_RemoveMemberAttribute"))
	static EOSResult EOSLobbyModificationRemoveMemberAttribute(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FEOSLobbyModificationRemoveMemberAttributeOptions Options);

	/**
	 * Set the Allowed Platform IDs for the lobby
	 *
	 * @param Options Options associated with allowed Platform IDs for this lobby
	 *
	 * @return EOS_Success if setting this parameter was successful
	 *         EOS_IncompatibleVersion if the API version passed in is incorrect
	 *         EOS_InvalidParameters if the AllowedPlatformIds array is null but the count is 0 or if the count is greater than 0 and the array is null.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbyModification_SetAllowedPlatformIds"))
	static EOSResult EOSLobbyModificationSetAllowedPlatformIds(UObject* WorldContextObject, FEOSHLobbyModification Handle, FEOSLobbyModificationSetAllowedPlatformIdsOptions Options);

	
	/**
	 * A "read only" representation of an existing lobby that games interact with externally.
	 * Both the lobby and lobby search interfaces interface use this common class for lobby management and search results
	 */

	/**
	* Get the product user ID of the current owner for a given lobby
	*
	* @param Options Structure containing the input parameters
	*
	* @return the product user ID for the lobby owner or null if the input parameters are invalid
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbyDetails_GetLobbyOwner"))
	static FEOSProductUserId EOSLobbyDetailsGetLobbyOwner(UObject* WorldContextObject, const FEOSHLobbyDetails& Handle, FEOSLobbyDetailsGetLobbyOwnerOptions Options);

	/**
	* EOS_LobbyDetails_CopyInfo is used to immediately retrieve a copy of lobby information from a given source such as a existing lobby or a search result.
	* If the call returns an EOS_Success result, the out parameter, OutLobbyDetailsInfo, must be passed to EOS_LobbyDetails_Info_Release to release the memory associated with it.
	*
	* @param Options Structure containing the input parameters
	* @param OutLobbyDetailsInfo Out parameter used to receive the EOS_LobbyDetails_Info structure.
	*
	* @return EOS_Success if the information is available and passed out in OutLobbyDetailsInfo
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*
	* @see EOS_LobbyDetails_Info
	* @see EOS_LobbyDetails_CopyInfoOptions
	* @see EOS_LobbyDetails_Info_Release
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbyDetails_CopyInfo"))
	static EOSResult EOSLobbyDetailsCopyInfo(UObject* WorldContextObject, const FEOSHLobbyDetails& Handle, FEOSLobbyDetailsCopyInfoOptions Options, FEOSLobbyDetailsInfo& OutLobbyDetailsInfo);

	/**
	 * EOS_LobbyDetails_CopyMemberInfo is used to immediately retrieve a copy of lobby member information from an existing lobby.
	 * If the call returns an EOS_Success result, the out parameter, OutLobbyDetailsMemberInfo, must be passed to EOS_LobbyDetails_MemberInfo_Release to release the memory associated with it.
	 * Note: this information is only available if you are actively in the lobby.  It is not available for search results.
	 *
	 * @param Options Structure containing the input parameters
	 * @param OutLobbyDetailsMemberInfo Out parameter used to receive the EOS_LobbyDetails_Info structure.
	 *
	 * @return EOS_Success if the information is available and passed out in OutLobbyMemberDetailsInfo
	 *         EOS_InvalidParameters if you pass a null pointer for the out parameter
	 *         EOS_IncompatibleVersion if the API version passed in is incorrect
	 *         EOS_NotFound if searching for a target user ID returns no results
	 *
	 * @see EOS_LobbyDetails_MemberInfo
	 * @see EOS_LobbyDetails_CopyMemberInfoOptions
	 * @see EOS_LobbyDetails_MemberInfo_Release
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbyDetails_CopyMemberInfo"))
	static EOSResult EOSLobbyDetailsCopyMemberInfo(UObject* WorldContextObject, FEOSHLobbyDetails Handle, FEOSLobbyDetailsCopyMemberInfoOptions Options, FEOSLobbyDetailsMemberInfo& OutLobbyDetailsMemberInfo);
	
	/**
	* Get the number of attributes associated with this lobby
	*
	* @param Options the Options associated with retrieving the attribute count
	*
	* @return number of attributes on the lobby or 0 if there is an error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbyDetails_GetAttributeCount"))
	static int32 EOSLobbyDetailsGetAttributeCount(UObject* WorldContextObject, const FEOSHLobbyDetails& Handle, FEOSLobbyDetailsGetAttributeCountOptions Options);

	/**
	* EOS_LobbyDetails_CopyAttributeByIndex is used to immediately retrieve a copy of a lobby attribute from a given source such as a existing lobby or a search result.
	* If the call returns an EOS_Success result, the out parameter, OutAttribute, must be passed to EOS_Lobby_Attribute_Release to release the memory associated with it.
	*
	* @param Options Structure containing the input parameters
	* @param OutAttribute Out parameter used to receive the EOS_Lobby_Attribute structure.
	*
	* @return EOS_Success if the information is available and passed out in OutAttribute
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*
	* @see EOS_Lobby_Attribute
	* @see EOS_LobbyDetails_CopyAttributeByIndexOptions
	* @see EOS_Lobby_Attribute_Release
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbyDetails_CopyAttributeByIndex"))
	static EOSResult EOSLobbyDetailsCopyAttributeByIndex(UObject* WorldContextObject, const FEOSHLobbyDetails& Handle, FEOSLobbyDetailsCopyAttributeByIndexOptions Options, FEOSLobbyAttribute& OutAttribute);

	/**
	* EOS_LobbyDetails_CopyAttributeByKey is used to immediately retrieve a copy of a lobby attribute from a given source such as a existing lobby or a search result.
	* If the call returns an EOS_Success result, the out parameter, OutAttribute, must be passed to EOS_Lobby_Attribute_Release to release the memory associated with it.
	*
	* @param Options Structure containing the input parameters
	* @param OutAttribute Out parameter used to receive the EOS_Lobby_Attribute structure.
	*
	* @return EOS_Success if the information is available and passed out in OutAttribute
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*
	* @see EOS_Lobby_Attribute
	* @see EOS_LobbyDetails_CopyAttributeByKeyOptions
	* @see EOS_Lobby_Attribute_Release
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbyDetails_CopyAttributeByKey"))
	static EOSResult EOSLobbyDetailsCopyAttributeByKey(UObject* WorldContextObject, const FEOSHLobbyDetails& Handle, FEOSLobbyDetailsCopyAttributeByKeyOptions Options, FEOSLobbyAttribute& OutAttribute);

	/**
	* Get the number of members associated with this lobby
	*
	* @param Options the Options associated with retrieving the member count
	*
	* @return number of members in the existing lobby or 0 if there is an error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbyDetails_GetMemberCount"))
	static int32 EOSLobbyDetailsGetMemberCount(UObject* WorldContextObject, const FEOSHLobbyDetails& Handle, FEOSLobbyDetailsGetMemberCountOptions Options);

	/**
	* EOS_LobbyDetails_GetMemberByIndex is used to immediately retrieve individual members registered with a lobby.
	*
	* @param Options Structure containing the input parameters
	*
	* @return the product user ID for the registered member at a given index or null if that index is invalid
	*
	* @see EOS_LobbyDetails_GetMemberCount
	* @see EOS_LobbyDetails_GetMemberByIndexOptions
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbyDetails_GetMemberByIndex"))
	static FEOSProductUserId EOSLobbyDetailsGetMemberByIndex(UObject* WorldContextObject, const FEOSHLobbyDetails& Handle, FEOSLobbyDetailsGetMemberByIndexOptions Options);

	/**
	* EOS_LobbyDetails_GetMemberAttributeCount is used to immediately retrieve the attribute count for members in a lobby.
	*
	* @param Options Structure containing the input parameters
	*
	* @return the number of attributes associated with a given lobby member or 0 if that member is invalid
	*
	* @see EOS_LobbyDetails_GetMemberCount
	* @see EOS_LobbyDetails_GetMemberAttributeCountOptions
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbyDetails_GetMemberAttributeCount"))
	static int32 EOSLobbyDetailsGetMemberAttributeCount(UObject* WorldContextObject, const FEOSHLobbyDetails& Handle, FEOSLobbyDetailsGetMemberAttributeCountOptions Options);

	/**
	* EOS_LobbyDetails_CopyMemberAttributeByIndex is used to immediately retrieve a copy of a lobby member attribute from an existing lobby.
	* If the call returns an EOS_Success result, the out parameter, OutAttribute, must be passed to EOS_Lobby_Attribute_Release to release the memory associated with it.
	*
	* @param Options Structure containing the input parameters
	* @param OutAttribute Out parameter used to receive the EOS_Lobby_Attribute structure.
	*
	* @return EOS_Success if the information is available and passed out in OutAttribute
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*
	* @see EOS_Lobby_Attribute
	* @see EOS_LobbyDetails_CopyMemberAttributeByIndexOptions
	* @see EOS_Lobby_Attribute_Release
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbyDetails_CopyMemberAttributeByIndex"))
	static EOSResult EOSLobbyDetailsCopyMemberAttributeByIndex(UObject* WorldContextObject, const FEOSHLobbyDetails& Handle, FEOSLobbyDetailsCopyMemberAttributeByIndexOptions Options, FEOSLobbyAttribute& OutAttribute);

	/**
	* EOS_LobbyDetails_CopyMemberAttributeByKey is used to immediately retrieve a copy of a lobby member attribute from an existing lobby.
	* If the call returns an EOS_Success result, the out parameter, OutAttribute, must be passed to EOS_Lobby_Attribute_Release to release the memory associated with it.
	*
	* @param Options Structure containing the input parameters
	* @param OutAttribute Out parameter used to receive the EOS_Lobby_Attribute structure.
	*
	* @return EOS_Success if the information is available and passed out in OutAttribute
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*
	* @see EOS_Lobby_Attribute
	* @see EOS_LobbyDetails_CopyMemberAttributeByKeyOptions
	* @see EOS_Lobby_Attribute_Release
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbyDetails_CopyMemberAttributeByKey"))
	static EOSResult EOSLobbyDetailsCopyMemberAttributeByKey(UObject* WorldContextObject, const FEOSHLobbyDetails& Handle, FEOSLobbyDetailsCopyMemberAttributeByKeyOptions Options, FEOSLobbyAttribute& OutAttribute);

	/**
	 * Class responsible for the creation, setup, and execution of a search query.
	 * Search parameters are defined, the query is executed and the search results are returned within this object
	 */

	/**
	* Find lobbies matching the search criteria setup via this lobby search handle.
	* When the operation completes, this handle will have the search results that can be parsed
	*
	* @param Options Structure containing information about the search criteria to use
	* @param Callback A callback that is fired when the search operation completes, either successfully or in error
	*
	* @return EOS_Success if the find operation completes successfully
	*         EOS_NotFound if searching for an individual lobby by lobby ID or target user ID returns no results
	*         EOS_InvalidParameters if any of the options are incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbySearch_Find"))
	void EOSLobbySearchFind(UObject* WorldContextObject, const FEOSHLobbySearch& Handle, FEOSLobbySearchFindOptions Options, const FOnLobbyFindCallback& Callback);

	/**
	* Set a lobby ID to find and will return at most one search result.  Setting TargetUserId or SearchParameters will result in EOS_LobbySearch_Find failing
	*
	* @param Options A specific lobby ID for which to search
	*
	* @return EOS_Success if setting this lobby ID was successful
	*         EOS_InvalidParameters if the lobby ID is invalid or null
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbySearch_SetLobbyId"))
	static EOSResult EOSLobbySearchSetLobbyId(UObject* WorldContextObject, const FEOSHLobbySearch& Handle, FEOSLobbySearchSetLobbyIdOptions Options);

	/**
	* Set a target user ID to find.  Setting LobbyId or SearchParameters will result in EOS_LobbySearch_Find failing
	* @note a search result will only be found if this user is in a public lobby
	*
	* @param Options a specific target user ID to find
	*
	* @return EOS_Success if setting this target user ID was successful
	*         EOS_InvalidParameters if the target user ID is invalid or null
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbySearch_SetTargetUserId"))
	static EOSResult EOSLobbySearchSetTargetUserId(UObject* WorldContextObject, const FEOSHLobbySearch& Handle, FEOSLobbySearchSetTargetUserIdOptions Options);

	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbySearch_SetParameterBool"))
	static EOSResult EOSLobbySearchSetParameterBool(UObject* WorldContextObject, const FEOSHLobbySearch& Handle, FString Key, bool bValue, EEOSEComparisonOp ComparisonOp);
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbySearch_SetParameterDouble"))
	static EOSResult EOSLobbySearchSetParameterDouble(UObject* WorldContextObject, const FEOSHLobbySearch& Handle, FString Key, FString Value, EEOSEComparisonOp ComparisonOp);
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbySearch_SetParameterInt64"))
	static EOSResult EOSLobbySearchSetParameterInt64(UObject* WorldContextObject, const FEOSHLobbySearch& Handle, FString Key, FString Value, EEOSEComparisonOp ComparisonOp);
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbySearch_SetParameterString"))
	static EOSResult EOSLobbySearchSetParameterString(UObject* WorldContextObject, const FEOSHLobbySearch& Handle, FString Key, FString Value, EEOSEComparisonOp ComparisonOp);

	/**
	* Remove a parameter from the array of search criteria.
	*
	* @params Options a search parameter key name to remove
	*
	* @return EOS_Success if removing this search parameter was successful
	*         EOS_InvalidParameters if the search key is invalid or null
	*		   EOS_NotFound if the parameter was not a part of the search criteria
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbySearch_RemoveParameter"))
	static EOSResult EOSLobbySearchRemoveParameter(UObject* WorldContextObject, const FEOSHLobbySearch& Handle, FEOSLobbySearchRemoveParameterOptions Options);

	/**
	* Set the maximum number of search results to return in the query, can't be more than EOS_LOBBY_MAX_SEARCH_RESULTS
	*
	* @param Options maximum number of search results to return in the query
	*
	* @return EOS_Success if setting the max results was successful
	*         EOS_InvalidParameters if the number of results requested is invalid
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbySearch_SetMaxResults"))
	static EOSResult EOSLobbySearchSetMaxResults(UObject* WorldContextObject, const FEOSHLobbySearch& Handle, FEOSLobbySearchSetMaxResultsOptions Options);

	/**
	* Get the number of search results found by the search parameters in this search
	*
	* @param Options Options associated with the search count
	*
	* @return return the number of search results found by the query or 0 if search is not complete
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbySearch_GetSearchResultCount"))
	static int32 EOSLobbySearchGetSearchResultCount(UObject* WorldContextObject, const FEOSHLobbySearch& Handle, FEOSLobbySearchGetSearchResultCountOptions Options);

	/**
	* EOS_LobbySearch_CopySearchResultByIndex is used to immediately retrieve a handle to the lobby information from a given search result.
	* If the call returns an EOS_Success result, the out parameter, OutLobbyDetailsHandle, must be passed to EOS_LobbyDetails_Release to release the memory associated with it.
	*
	* @param Options Structure containing the input parameters
	* @param OutLobbyDetailsHandle out parameter used to receive the lobby details handle
	*
	* @return EOS_Success if the information is available and passed out in OutLobbyDetailsHandle
	*         EOS_InvalidParameters if you pass an invalid index or a null pointer for the out parameter
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*
	* @see EOS_LobbySearch_CopySearchResultByIndexOptions
	* @see EOS_LobbyDetails_Release
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbySearch_CopySearchResultByIndex"))
	static EOSResult EOSLobbySearchCopySearchResultByIndex(UObject* WorldContextObject, const FEOSHLobbySearch& Handle, FEOSLobbySearchCopySearchResultByIndexOptions Options, FEOSHLobbyDetails& OutLobbyDetailsHandle);
	
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (DisplayName = "EOSLobbyModificationRelease"))
	static void EOSLobbyModificationRelease(FEOSHLobbyModification LobbyModificationHandle);

	/**
	* Release the memory associated with a single lobby. This must be called on data retrieved from EOS_LobbySearch_CopySearchResultByIndex.
	*
	* @param LobbyHandle - The lobby handle to release
	*
	* @see EOS_LobbySearch_CopySearchResultByIndex
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbyDetails_Release"))
	static void EOSLobbyDetailsRelease(FEOSHLobbyDetails LobbyHandle);

	/**
	* Release the memory associated with a lobby search. This must be called on data retrieved from EOS_Lobby_CreateLobbySearch.
	*
	* @param LobbySearchHandle - The lobby search handle to release
	*
	* @see EOS_Lobby_CreateLobbySearch
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Lobby", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_LobbySearch_Release"))
	static void EOSLobbySearchRelease(FEOSHLobbySearch LobbySearchHandle);
private:
	static EOSResult EOSLobbySearchSetParameter(UObject* WorldContextObject, const FEOSHLobbySearch& Handle, FEOSLobbySearchSetParameterOptions Options);
private:
	static void Internal_OnCreateLobbyCallback(const EOS_Lobby_CreateLobbyCallbackInfo* Data);
	static void Internal_OnDestroyLobbyCallback(const EOS_Lobby_DestroyLobbyCallbackInfo* Data);
	static void Internal_OnJoinLobbyCallback(const EOS_Lobby_JoinLobbyCallbackInfo* Data);
	static void Internal_OnJoinLobbyByIdCallback(const EOS_Lobby_JoinLobbyByIdCallbackInfo* Data);
	static void Internal_OnJoinHardMuteCallback(const EOS_Lobby_HardMuteMemberCallbackInfo* Data);
	static void Internal_OnLeaveLobbyCallback(const EOS_Lobby_LeaveLobbyCallbackInfo* Data);
	static void Internal_OnUpdateLobbyCallback(const EOS_Lobby_UpdateLobbyCallbackInfo* Data);
	static void Internal_OnPromoteMemberCallback(const EOS_Lobby_PromoteMemberCallbackInfo* Data);
	static void Internal_OnKickMemberCallback(const EOS_Lobby_KickMemberCallbackInfo* Data);
	static void Internal_OnSendInviteCallback(const EOS_Lobby_SendInviteCallbackInfo* Data);
	static void Internal_OnRejectInviteCallback(const EOS_Lobby_RejectInviteCallbackInfo* Data);
	static void Internal_OnQueryInvitesCallback(const EOS_Lobby_QueryInvitesCallbackInfo* Data);
	static void Internal_OnFindCallback(const EOS_LobbySearch_FindCallbackInfo* Data);
protected:
	TMap<FEOSNotificationId, FLobbyUpdateReceivedCallback*> m_LobbyUpdateReceivedCallbacks;
	TMap<FEOSNotificationId, FLobbyInviteRejectedCallback*> m_LobbyInviteRejectedCallback;
	TMap<FEOSNotificationId, FLobbyNativeInviteRequestedCallback*> m_LobbyNativeInviteRequestedCallback;
	TMap<FEOSNotificationId, FLobbyMemberUpdateReceivedCallback*> m_LobbyMemberUpdateReceivedCallbacks;
	TMap<FEOSNotificationId, FLobbyMemberStatusReceivedCallback*> m_LobbyMemberStatusReceivedCallbacks;
	TMap<FEOSNotificationId, FLobbyInviteReceivedCallback*> m_LobbyInviteReceivedCallbacks;
	TMap<FEOSNotificationId, FLobbyInviteAcceptedCallback*> m_LobbyInviteAcceptedCallbacks;
	TMap<FEOSNotificationId, FJoinLobbyAcceptedCallback*> m_LobbyJoinLobbyAccepted;
	TMap<FEOSNotificationId, FLobbyRTCRoomConnectionChanged*> m_LobbyRTCRoomConnectionChanged;
	TMap<FEOSNotificationId, FLeaveLobbyRequestedCallback*> m_LobbyLeaveLobbyRequestedCallback;
};
