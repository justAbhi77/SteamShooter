/**
* Copyright (C) 2017-2022 | eelDev AB
*
 official EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "EOSCoreModule.h"
#include "Sessions/EOSSessionsTypes.h"
#include "EOSSessions.generated.h"

/**
 * The Session Interface is used to manage sessions that can be advertised with the backend service
 * All Session Interface calls take a handle of type EOS_HSessions as the first parameter.
 * This handle can be retrieved from a EOS_HPlatform handle by using the EOS_Platform_GetSessionsInterface function.
 *
 * NOTE: At this time, this feature is only available for products that are part of the Epic Games store.
 *
 * @see EOS_Platform_GetSessionsInterface
 */

UCLASS()
class EOSCORE_API UCoreSessions : public UEOSCoreSubsystem
{
	GENERATED_BODY()
public:
	virtual void Deinitialize() override;
public:
	/**
	 * The Session Interface is used to manage sessions that can be advertised with the backend service
	 * All Session Interface calls take a handle of type EOS_HSessions as the first parameter.
	 * This handle can be retrieved from a EOS_HPlatform handle by using the EOS_Platform_GetSessionsInterface function.
	 *
	 * NOTE: At this time, this feature is only available for products that are part of the Epic Games store.
	 *
	 * @see EOS_Platform_GetSessionsInterface
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Interfaces", meta = (WorldContext = "WorldContextObject"))
	static UCoreSessions* GetSessions(UObject* WorldContextObject);

public:
	/**
	* Creates a session modification handle (EOS_HSessionModification).  The session modification handle is used to build a new session and can be applied with EOS_Sessions_UpdateSession
	* The EOS_HSessionModification must be released by calling EOS_SessionModification_Release once it no longer needed.
	*
	* @param Options Required fields for the creation of a session such as a name, bucketid, and max players
	* @param OutSessionModificationHandle Pointer to a Session Modification Handle only set if successful
	* @return EOS_Success if we successfully created the Session Modification Handle pointed at in OutSessionModificationHandle, or an error result if the input data was invalid
	*
	* @see EOS_SessionModification_Release
	* @see EOS_Sessions_UpdateSession
	* @see EOS_HSessionModification
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sessions_CreateSessionModification"))
	static EOSResult EOSSessionsCreateSessionModification(UObject* WorldContextObject, FEOSSessionsCreateSessionModificationOptions Options, FEOSHSessionModification& OutSessionModificationHandle);

	/**
	* Creates a session modification handle (EOS_HSessionModification). The session modification handle is used to modify an existing session and can be applied with EOS_Sessions_UpdateSession.
	* The EOS_HSessionModification must be released by calling EOS_SessionModification_Release once it is no longer needed.
	*
	* @param Options Required fields such as session name
	* @param OutSessionModificationHandle Pointer to a Session Modification Handle only set if successful
	* @return EOS_Success if we successfully created the Session Modification Handle pointed at in OutSessionModificationHandle, or an error result if the input data was invalid
	*
	* @see EOS_SessionModification_Release
	* @see EOS_Sessions_UpdateSession
	* @see EOS_HSessionModification
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sessions_UpdateSessionModification"))
	static EOSResult EOSSessionsUpdateSessionModification(UObject* WorldContextObject, FEOSSessionsUpdateSessionModificationOptions Options, FEOSHSessionModification& OutSessionModificationHandle);

	/**
	* Update a session given a session modification handle created by EOS_Sessions_CreateSessionModification or EOS_Sessions_UpdateSessionModification
	*
	* @param Options Structure containing information about the session to be updated
	* @param Callback A callback that is fired when the update operation completes, either successfully or in error
	*
	* @return EOS_Success if the update completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_Sessions_OutOfSync if the session is out of sync and will be updated on the next connection with the backend
	*         EOS_NotFound if a session to be updated does not exist
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sessions_UpdateSession"))
	void EOSSessionsUpdateSession(UObject* WorldContextObject, const FEOSSessionsUpdateSessionOptions& Options, const FOnSessionUpdateSessionCallback& Callback);

	/**
	* Destroy a session given a session name
	*
	* @param Options Structure containing information about the session to be destroyed
	* @param Callback A callback that is fired when the destroy operation completes, either successfully or in error
	*
	* @return EOS_Success if the destroy completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_AlreadyPending if the session is already marked for destroy
	*         EOS_NotFound if a session to be destroyed does not exist
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sessions_DestroySession"))
	void EOSSessionsDestroySession(UObject* WorldContextObject, FEOSSessionsDestroySessionOptions Options, const FOnSessionDestroySessionCallback& Callback);

	/**
	* Join a session, creating a local session under a given session name.  Backend will validate various conditions to make sure it is possible to join the session.
	*
	* @param Options Structure containing information about the session to be joined
	* @param Callback A callback that is fired when the join operation completes, either successfully or in error
	*
	* @return EOS_Success if the join completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_Sessions_SessionAlreadyExists if the session is already exists or is in the process of being joined
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sessions_JoinSession"))
	void EOSSessionsJoinSession(UObject* WorldContextObject, FEOSSessionsJoinSessionOptions Options, const FOnSessionJoinSessionCallback& Callback);

	/**
	* Mark a session as started, making it unable to find if session properties indicate "join in progress" is not available
	*
	* @param Options Structure containing information about the session to be started
	* @param Callback A callback that is fired when the start operation completes, either successfully or in error
	*
	* @return EOS_Success if the start completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_Sessions_OutOfSync if the session is out of sync and will be updated on the next connection with the backend
	*         EOS_NotFound if a session to be started does not exist
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sessions_StartSession"))
	void EOSSessionsStartSession(UObject* WorldContextObject, FEOSSessionsStartSessionOptions Options, const FOnSessionStartSessionCallback& Callback);

	/**
	* Mark a session as ended, making it available to find if "join in progress" was disabled.  The session may be started again if desired
	*
	* @param Options Structure containing information about the session to be ended
	* @param Callback A callback that is fired when the end operation completes, either successfully or in error
	*
	* @return EOS_Success if the end completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_Sessions_OutOfSync if the session is out of sync and will be updated on the next connection with the backend
	*         EOS_NotFound if a session to be ended does not exist
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sessions_EndSession"))
	void EOSSessionsEndSession(UObject* WorldContextObject, FEOSSessionsEndSessionOptions Options, const FOnSessionEndSessionCallback& Callback);

	/**
	* Register a group of players with the session, allowing them to invite others or otherwise indicate they are part of the session for determining a full session
	*
	* @param Options Structure containing information about the session and players to be registered
	* @param Callback A callback that is fired when the registration operation completes, either successfully or in error
	*
	* @return EOS_Success if the register completes successfully
	*         EOS_NoChange if the players to register registered previously
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_Sessions_OutOfSync if the session is out of sync and will be updated on the next connection with the backend
	*         EOS_NotFound if a session to register players does not exist
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sessions_RegisterPlayers"))
	void EOSSessionsRegisterPlayers(UObject* WorldContextObject, FEOSSessionsRegisterPlayersOptions Options, const FOnSessionRegisterPlayersCallback& Callback);

	/**
	* Unregister a group of players with the session, freeing up space for others to join
	*
	* @param Options Structure containing information about the session and players to be unregistered
	* @param Callback A callback that is fired when the unregistration operation completes, either successfully or in error
	*
	* @return EOS_Success if the unregister completes successfully
	*         EOS_NoChange if the players to unregister were not found
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_Sessions_OutOfSync if the session is out of sync and will be updated on the next connection with the backend
	*         EOS_NotFound if a session to be unregister players does not exist
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sessions_UnregisterPlayers"))
	void EOSSessionsUnregisterPlayers(UObject* WorldContextObject, FEOSSessionsUnregisterPlayersOptions Options, const FOnSessionUnregisterPlayersCallback& Callback);

	/**
	* Send an invite to another player.  User must have created the session or be registered in the session or else the call will fail
	*
	* @param Options Structure containing information about the session and player to invite
	* @param Callback A callback that is fired when the send invite operation completes, either successfully or in error
	*
	* @return EOS_Success if the send invite completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_NotFound if the session to send the invite from does not exist
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sessions_SendInvite"))
	void EOSSessionsSendInvite(UObject* WorldContextObject, FEOSSessionsSendInviteOptions Options, const FOnSessionSessionSendInviteCallback& Callback);

	/**
	* Reject an invite from another player.
	*
	* @param Options Structure containing information about the invite to reject
	* @param Callback A callback that is fired when the reject invite operation completes, either successfully or in error
	*
	* @return EOS_Success if the invite rejection completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_NotFound if the invite does not exist
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sessions_RejectInvite"))
	void EOSSessionsRejectInvite(UObject* WorldContextObject, FEOSSessionsRejectInviteOptions Options, const FOnSessionRejectInviteCallback& Callback);

	/**
	* Retrieve all existing invites for a single user
	*
	* @param Options Structure containing information about the invites to query
	* @param Callback A callback that is fired when the query invites operation completes, either successfully or in error
	*
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sessions_QueryInvites"))
	void EOSSessionsQueryInvites(UObject* WorldContextObject, FEOSSessionsQueryInvitesOptions Options, const FOnSessionQueryInvitesCallback& Callback);

	/**
	* Get the number of known invites for a given user
	*
	* @param Options the Options associated with retrieving the current invite count
	*
	* @return number of known invites for a given user or 0 if there is an error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sessions_GetInviteCount"))
	static int32 EOSSessionsGetInviteCount(UObject* WorldContextObject, FEOSSessionsGetInviteCountOptions Options);

	/**
	* Retrieve an invite ID from a list of active invites for a given user
	*
	* @param Options Structure containing the input parameters
	*
	* @return EOS_Success if the input is valid and an invite ID was returned
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_NotFound if the invite doesn't exist
	*
	* @see EOS_Sessions_GetInviteCount
	* @see EOS_Sessions_CopySessionHandleByInviteId
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sessions_GetInviteIdByIndex"))
	static EOSResult EOSSessionsGetInviteIdByIndex(UObject* WorldContextObject, FEOSSessionsGetInviteIdByIndexOptions Options, FString& OutBuffer);

	/**
	* Create a session search handle.  This handle may be modified to include various search parameters.
	* Searching is possible in three methods, all mutually exclusive
	* - set the session ID to find a specific session
	* - set the target user ID to find a specific user
	* - set session parameters to find an array of sessions that match the search criteria
	*
	* @param Options Structure containing required parameters such as the maximum number of search results
	* @param OutSessionSearchHandle The new search handle or null if there was an error creating the search handle
	*
	* @return EOS_Success if the search creation completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sessions_CreateSessionSearch"))
	static EOSResult EOSSessionsCreateSessionSearch(UObject* WorldContextObject, FEOSSessionsCreateSessionSearchOptions Options, FEOSHSessionSearch& OutSessionSearchHandle);

	/**
	* Create a handle to an existing active session.
	*
	* @param Options Structure containing information about the active session to retrieve
	* @param OutSessionHandle The new active session handle or null if there was an error
	*
	* @return EOS_Success if the session handle was created successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*         EOS_NotFound if the active session doesn't exist
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sessions_CopyActiveSessionHandle"))
	static EOSResult EOSSessionsCopyActiveSessionHandle(UObject* WorldContextObject, FEOSSessionsCopyActiveSessionHandleOptions Options, FEOSHActiveSession& OutSessionHandle);

	/**
	* Register to receive session invites.
	* @note must call RemoveNotifySessionInviteReceived to remove the notification
	*
	* @param Callback A callback that is fired when a session invite for a user has been received
	*
	* @return handle representing the registered callback
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sessions_AddNotifySessionInviteReceived"))
	static FEOSNotificationId EOSSessionsAddNotifySessionInviteReceived(UObject* WorldContextObject, const FOnSessionInviteReceivedCallback& Callback);

	/**
	* Unregister from receiving session invites.
	*
	* @param Id Handle representing the registered callback
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sessions_RemoveNotifySessionInviteReceived"))
	void EOSSessionsRemoveNotifySessionInviteReceived(UObject* WorldContextObject, FEOSNotificationId Id);

	/**
	 * Register to receive notifications when a user accepts a session invite via the social overlay.
	 * @note must call RemoveNotifySessionInviteAccepted to remove the notification
	 *
	 * @param Callback A callback that is fired when a a notification is received.
	 *
	 * @return handle representing the registered callback
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sessions_AddNotifySessionInviteAccepted"))
	static FEOSNotificationId EOSSessionsAddNotifySessionInviteAccepted(UObject* WorldContextObject, const FOnSessionInviteAcceptedCallback& Callback);

	/**
	 * Unregister from receiving notifications when a user accepts a session invite via the social overlay.
	 *
	 * @param Id Handle representing the registered callback
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sessions_RemoveNotifySessionInviteAccepted"))
	static void EOSSessionsRemoveNotifySessionInviteAccepted(UObject* WorldContextObject, FEOSNotificationId Id);

	/**
	* Register to receive notifications when a user accepts a session invite via the social overlay.
	* @note must call RemoveNotifySessionInviteAccepted to remove the notification
	*
	* @param Callback A callback that is fired when a a notification is received.
	*
	* @return handle representing the registered callback
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sessions_AddNotifyJoinSessionAccepted"))
	static FEOSNotificationId EOSSessionsAddNotifyJoinSessionAccepted(UObject* WorldContextObject, const FOnSessionJoinSessionAcceptedCallback& Callback);

	/**
	* Unregister from receiving notifications when a user accepts a session invite via the social overlay.
	*
	* @param Id Handle representing the registered callback
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sessions_RemoveNotifyJoinSessionAccepted"))
	static void EOSSessionsRemoveNotifyJoinSessionAccepted(UObject* WorldContextObject, FEOSNotificationId Id);

	/**
	* EOS_Sessions_CopySessionHandleByInviteId is used to immediately retrieve a handle to the session information from after notification of an invite
	* If the call returns an EOS_Success result, the out parameter, OutSessionHandle, must be passed to EOS_SessionDetails_Release to release the memory associated with it.
	*
	* @param Options Structure containing the input parameters
	* @param OutSessionHandle out parameter used to receive the session handle
	*
	* @return EOS_Success if the information is available and passed out in OutSessionHandle
	*         EOS_InvalidParameters if you pass an invalid invite ID or a null pointer for the out parameter
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*         EOS_NotFound if the invite ID cannot be found
	*
	* @see EOS_Sessions_CopySessionHandleByInviteIdOptions
	* @see EOS_SessionDetails_Release
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sessions_CopySessionHandleByInviteId"))
	static EOSResult EOSSessionsCopySessionHandleByInviteId(UObject* WorldContextObject, FEOSSessionsCopySessionHandleByInviteIdOptions Options, FEOSHSessionDetails& OutSessionHandle);

	/**
	* EOS_Sessions_CopySessionHandleByUiEventId is used to immediately retrieve a handle to the session information from after notification of a join game event.
	* If the call returns an EOS_Success result, the out parameter, OutSessionHandle, must be passed to EOS_SessionDetails_Release to release the memory associated with it.
	*
	* @param Options Structure containing the input parameters
	* @param OutSessionHandle out parameter used to receive the session handle
	*
	* @return EOS_Success if the information is available and passed out in OutSessionHandle
	*         EOS_InvalidParameters if you pass an invalid invite ID or a null pointer for the out parameter
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*         EOS_NotFound if the invite ID cannot be found
	*
	* @see EOS_Sessions_CopySessionHandleByUiEventIdOptions
	* @see EOS_SessionDetails_Release
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sessions_CopySessionHandleByUiEventId"))
	static EOSResult EOSSessionsCopySessionHandleByUiEventId(UObject* WorldContextObject, FEOSSessionsCopySessionHandleByUiEventIdOptions Options, FEOSHSessionDetails& OutSessionHandle);

	/**
	* EOS_Sessions_CopySessionHandleForPresence is used to immediately retrieve a handle to the session information which was marked with bPresenceEnabled on create or join.
	* If the call returns an EOS_Success result, the out parameter, OutSessionHandle, must be passed to EOS_SessionDetails_Release to release the memory associated with it.
	*
	* @param Options Structure containing the input parameters
	* @param OutSessionHandle out parameter used to receive the session handle
	*
	* @return EOS_Success if the information is available and passed out in OutSessionHandle
	*         EOS_InvalidParameters if you pass an invalid invite ID or a null pointer for the out parameter
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*         EOS_NotFound if there is no session with bPresenceEnabled
	*
	* @see EOS_Sessions_CopySessionHandleForPresenceOptions
	* @see EOS_SessionDetails_Release
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sessions_CopySessionHandleForPresence"))
	static EOSResult EOSSessionsCopySessionHandleForPresence(UObject* WorldContextObject, FEOSSessionsCopySessionHandleForPresenceOptions Options, FEOSHSessionDetails& OutSessionHandle);

	/**
	* EOS_Sessions_IsUserInSession returns whether or not a given user can be found in a specified session
	*
	* @param Options Structure containing the input parameters
	*
	* @return EOS_Success if the user is found in the specified session
	*		   EOS_NotFound if the user is not found in the specified session
	*		   EOS_InvalidParameters if you pass an invalid invite ID or a null pointer for the out parameter
	*		   EOS_IncompatibleVersion if the API version passed in is incorrect
	*		   EOS_Invalid_ProductUserID if an invalid target user is specified
	*		   EOS_Sessions_InvalidSession if the session specified is invalid
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sessions_IsUserInSession"))
	static EOSResult EOSSessionsIsUserInSession(UObject* WorldContextObject, FEOSSessionsIsUserInSessionOptions Options);

	/**
	* Dump the contents of active sessions that exist locally to the log output, purely for debug purposes
	*
	* @param Options Options related to dumping session state such as the session name
	*
	* @return EOS_Success if the output operation completes successfully
	*         EOS_NotFound if the session specified does not exist
	*         EOS_InvalidParameters if any of the options are incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sessions_DumpSessionState"))
	static EOSResult EOSSessionsDumpSessionState(UObject* WorldContextObject, FEOSSessionsDumpSessionStateOptions Options);

	/**
	 * To modify sessions, you must call EOS_Sessions_CreateSessionModification to create a Session Modification handle. To modify that handle, call
	 * EOS_HSessionModification methods. Once you are finished, call EOS_Sessions_UpdateSession with your handle. You must then release your Session Modification
	 * handle by calling EOS_SessionModification_Release.
	 */

	/**
	* Set the bucket ID associated with this session.
	* Values such as region, game mode, etc can be combined here depending on game need.
	* Setting this is strongly recommended to improve search performance.
	*
	* @param Options Options associated with the bucket ID of the session
	*
	* @return EOS_Success if setting this parameter was successful
	*         EOS_InvalidParameters if the bucket ID is invalid or null
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_SessionModification_SetBucketId"))
	static EOSResult EOSSessionModificationSetBucketId(UObject* WorldContextObject, const FEOSHSessionModification& Handle, FEOSSessionModificationSetBucketIdOptions Options);

	/**
	* Set the host address associated with this session
	* Setting this is optional, if the value is not set the SDK will fill the value in from the service.
	* It is useful to set if other addressing mechanisms are desired or if LAN addresses are preferred during development
	*
	* @note No validation of this value occurs to allow for flexibility in addressing methods
	*
	* @param Options Options associated with the host address of the session
	*
	* @return EOS_Success if setting this parameter was successful
	*         EOS_InvalidParameters if the host ID is an empty string
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_SessionModification_SetHostAddress"))
	static EOSResult EOSSessionModificationSetHostAddress(UObject* WorldContextObject, const FEOSHSessionModification& Handle, FEOSSessionModificationSetHostAddressOptions Options);

	/**
	* Set the session permissions associated with this session.
	* The permissions range from "public" to "invite only" and are described by EOS_EOnlineSessionPermissionLevel
	*
	* @param Options Options associated with the permission level of the session
	*
	* @return EOS_Success if setting this parameter was successful
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_SessionModification_SetPermissionLevel"))
	static EOSResult EOSSessionModificationSetPermissionLevel(UObject* WorldContextObject, const FEOSHSessionModification& Handle, FEOSSessionModificationSetPermissionLevelOptions Options);

	/**
	* Set whether or not join in progress is allowed
	* Once a session is started, it will no longer be visible to search queries unless this flag is set or the session returns to the pending or ended state
	*
	* @param Options Options associated with setting the join in progress state the session
	*
	* @return EOS_Success if setting this parameter was successful
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_SessionModification_SetJoinInProgressAllowed"))
	static EOSResult EOSSessionModificationSetJoinInProgressAllowed(UObject* WorldContextObject, const FEOSHSessionModification& Handle, FEOSSessionModificationSetJoinInProgressAllowedOptions Options);

	/**
	* Set the maximum number of players allowed in this session.
	* When updating the session, it is not possible to reduce this number below the current number of existing players
	*
	* @param Options Options associated with max number of players in this session
	*
	* @return EOS_Success if setting this parameter was successful
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_SessionModification_SetMaxPlayers"))
	static EOSResult EOSSessionModificationSetMaxPlayers(UObject* WorldContextObject, const FEOSHSessionModification& Handle, FEOSSessionModificationSetMaxPlayersOptions Options);

	/**
	* Allows enabling or disabling invites for this session.
	* The session will also need to have `bPresenceEnabled` true.
	*
	* @param Options Options associated with invites allowed flag for this session.
	*
	* @return EOS_Success if setting this parameter was successful
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_SessionModification_SetInvitesAllowed"))
	static EOSResult EOSSessionModificationSetInvitesAllowed(UObject* WorldContextObject, const FEOSHSessionModification& Handle, FEOSSessionModificationSetInvitesAllowedOptions Options);

	/**
	* Associate an attribute with this session
	* An attribute is something that may or may not be advertised with the session.
	* If advertised, it can be queried for in a search, otherwise the data remains local to the client
	*
	* @param Options Options to set the attribute and its advertised state
	*
	* @return EOS_Success if setting this parameter was successful
	*		   EOS_InvalidParameters if the attribution is missing information or otherwise invalid
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*/
	static EOSResult EOSSessionModificationAddAttribute(UObject* WorldContextObject, const FEOSHSessionModification& Handle, const FEOSSessionModificationAddAttributeOptions& Options);

	/**
	* Associate an attribute with this session
	* An attribute is something that may or may not be advertised with the session.
	* If advertised, it can be queried for in a search, otherwise the data remains local to the client
	*
	* @param Options Options to set the attribute and its advertised state
	*
	* @return EOS_Success if setting this parameter was successful
	*		   EOS_InvalidParameters if the attribution is missing information or otherwise invalid
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_SessionModification_AddAttributeBool"))
	static EOSResult EOSSessionModificationAddAttributeBool(UObject* WorldContextObject, const FEOSHSessionModification& Handle, EEOSESessionAttributeAdvertisementType AdvertisementType, const FString& Key, bool bValue);

	/**
	* Associate an attribute with this session
	* An attribute is something that may or may not be advertised with the session.
	* If advertised, it can be queried for in a search, otherwise the data remains local to the client
	*
	* @param Options Options to set the attribute and its advertised state
	*
	* @return EOS_Success if setting this parameter was successful
	*		   EOS_InvalidParameters if the attribution is missing information or otherwise invalid
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_SessionModification_AddAttributeDouble"))
	static EOSResult EOSSessionModificationAddAttributeDouble(UObject* WorldContextObject, const FEOSHSessionModification& Handle, EEOSESessionAttributeAdvertisementType AdvertisementType, const FString& Key, const FString& Value);

	/**
	* Associate an attribute with this session
	* An attribute is something that may or may not be advertised with the session.
	* If advertised, it can be queried for in a search, otherwise the data remains local to the client
	*
	* @param Options Options to set the attribute and its advertised state
	*
	* @return EOS_Success if setting this parameter was successful
	*		   EOS_InvalidParameters if the attribution is missing information or otherwise invalid
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_SessionModification_AddAttributeInt64"))
	static EOSResult EOSSessionModificationAddAttributeInt64(UObject* WorldContextObject, const FEOSHSessionModification& Handle, EEOSESessionAttributeAdvertisementType AdvertisementType, const FString& Key, const FString& Value);

	/**
	 * Associate an attribute with this session
	 * An attribute is something that may or may not be advertised with the session.
	 * If advertised, it can be queried for in a search, otherwise the data remains local to the client
	 *
	 * @param Options Options to set the attribute and its advertised state
	 *
	 * @return EOS_Success if setting this parameter was successful
	 *		   EOS_InvalidParameters if the attribution is missing information or otherwise invalid
	 *         EOS_IncompatibleVersion if the API version passed in is incorrect
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_SessionModification_AddAttributeString"))
	static EOSResult EOSSessionModificationAddAttributeString(UObject* WorldContextObject, const FEOSHSessionModification& Handle, EEOSESessionAttributeAdvertisementType AdvertisementType, const FString& Key, const FString& Value);

	/**
	* Remove an attribute from this session
	*
	* @param Options Specify the key of the attribute to remove
	*
	* @return EOS_Success if removing this parameter was successful
	*		   EOS_InvalidParameters if the key is null or empty
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_SessionModification_RemoveAttribute"))
	static EOSResult EOSSessionModificationRemoveAttribute(UObject* WorldContextObject, const FEOSHSessionModification& Handle, const FEOSSessionModificationRemoveAttributeOptions& Options);

	/**
	 * Representation of an existing session some local players are actively involved in (via Create/Join)
	 */

	/**
	* EOS_ActiveSession_CopyInfo is used to immediately retrieve a copy of active session information
	* If the call returns an EOS_Success result, the out parameter, OutActiveSessionInfo, must be passed to EOS_ActiveSession_Info_Release to release the memory associated with it.
	*
	* @param Options Structure containing the input parameters
	* @param OutActiveSessionInfo Out parameter used to receive the EOS_ActiveSession_Info structure.
	*
	* @return EOS_Success if the information is available and passed out in OutActiveSessionInfo
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*
	* @see EOS_ActiveSession_Info
	* @see EOS_ActiveSession_CopyInfoOptions
	* @see EOS_ActiveSession_Info_Release
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_ActiveSession_CopyInfo"))
	static EOSResult EOSActiveSessionCopyInfo(UObject* WorldContextObject, const FEOSHActiveSession& Handle, FEOSActiveSessionCopyInfoOptions Options, FEOSActiveSessionInfo& OutActiveSessionInfo);

	/**
	* Get the number of registered players associated with this active session
	*
	* @param Options the Options associated with retrieving the registered player count
	*
	* @return number of registered players in the active session or 0 if there is an error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_ActiveSession_GetRegisteredPlayerCount"))
	static int32 EOSActiveSessionGetRegisteredPlayerCount(UObject* WorldContextObject, const FEOSHActiveSession& Handle, FEOSActiveSessionGetRegisteredPlayerCountOptions Options);

	/**
	* EOS_ActiveSession_GetRegisteredPlayerByIndex is used to immediately retrieve individual players registered with the active session.
	*
	* @param Options Structure containing the input parameters
	*
	* @return the product user ID for the registered player at a given index or null if that index is invalid
	*
	* @see EOS_ActiveSession_GetRegisteredPlayerCount
	* @see EOS_ActiveSession_GetRegisteredPlayerByIndexOptions
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_ActiveSession_GetRegisteredPlayerByIndex"))
	static FEOSProductUserId EOSActiveSessionGetRegisteredPlayerByIndex(UObject* WorldContextObject, const FEOSHActiveSession& Handle, FEOSActiveSessionGetRegisteredPlayerByIndexOptions Options);

	/**
	 * This class represents the details of a session, including its session properties and the attribution associated with it
	 * Locally created or joined active sessions will contain this information as will search results.
	 * A handle to a session is required to join a session via search or invite
	 */

	/**
	* EOS_SessionDetails_CopyInfo is used to immediately retrieve a copy of session information from a given source such as a active session or a search result.
	* If the call returns an EOS_Success result, the out parameter, OutSessionInfo, must be passed to EOS_SessionDetails_Info_Release to release the memory associated with it.
	*
	* @param Options Structure containing the input parameters
	* @param OutSessionInfo Out parameter used to receive the EOS_SessionDetails_Info structure.
	*
	* @return EOS_Success if the information is available and passed out in OutSessionInfo
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*
	* @see EOS_SessionDetails_Info
	* @see EOS_SessionDetails_CopyInfoOptions
	* @see EOS_SessionDetails_Info_Release
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_SessionDetails_CopyInfo"))
	static EOSResult EOSSessionDetailsCopyInfo(UObject* WorldContextObject, const FEOSHSessionDetails& Handle, FEOSSessionDetailsCopyInfoOptions options, FEOSSessionDetailsInfo& outSessionInfo);


	/**
	* Get the number of attributes associated with this session
	*
	* @param Options the Options associated with retrieving the attribute count
	*
	* @return number of attributes on the session or 0 if there is an error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_SessionDetails_GetSessionAttributeCount"))
	static int32 EOSSessionDetailsGetSessionAttributeCount(UObject* WorldContextObject, const FEOSHSessionDetails& Handle, FEOSSessionDetailsGetSessionAttributeCountOptions Options);

	/**
	* EOS_SessionDetails_CopySessionAttributeByIndex is used to immediately retrieve a copy of session attribution from a given source such as a active session or a search result.
	* If the call returns an EOS_Success result, the out parameter, OutSessionAttribute, must be passed to EOS_SessionDetails_Attribute_Release to release the memory associated with it.
	*
	* @param Options Structure containing the input parameters
	* @param OutSessionAttribute Out parameter used to receive the EOS_SessionDetails_Attribute structure.
	*
	* @return EOS_Success if the information is available and passed out in OutSessionAttribute
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*
	* @see EOS_SessionDetails_Attribute
	* @see EOS_SessionDetails_CopySessionAttributeByIndexOptions
	* @see EOS_SessionDetails_Attribute_Release
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_SessionDetails_CopySessionAttributeByIndex"))
	static EOSResult EOSSessionDetailsCopySessionAttributeByIndex(UObject* WorldContextObject, const FEOSHSessionDetails& Handle, FEOSSessionDetailsCopySessionAttributeByIndexOptions Options, FEOSSessionDetailsAttribute& OutSessionAttribute);

	/**
	* EOS_SessionDetails_CopySessionAttributeByKey is used to immediately retrieve a copy of session attribution from a given source such as a active session or a search result.
	* If the call returns an EOS_Success result, the out parameter, OutSessionAttribute, must be passed to EOS_SessionDetails_Attribute_Release to release the memory associated with it.
	*
	* @param Options Structure containing the input parameters
	* @param OutSessionAttribute Out parameter used to receive the EOS_SessionDetails_Attribute structure.
	*
	* @return EOS_Success if the information is available and passed out in OutSessionAttribute
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*
	* @see EOS_SessionDetails_Attribute
	* @see EOS_SessionDetails_CopySessionAttributeByKeyOptions
	* @see EOS_SessionDetails_Attribute_Release
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_SessionDetails_CopySessionAttributeByKey"))
	static EOSResult EOSSessionDetailsCopySessionAttributeByKey(UObject* WorldContextObject, const FEOSHSessionDetails& Handle, FEOSSessionDetailsCopySessionAttributeByKeyOptions Options, FEOSSessionDetailsAttribute& OutSessionAttribute);

	/**
	 * Class responsible for the creation, setup, and execution of a search query.
	 * Search parameters are defined, the query is executed and the search results are returned within this object
	 */

	/**
	* Set a session ID to find and will return at most one search result.  Setting TargetUserId or SearchParameters will result in EOS_SessionSearch_Find failing
	*
	* @param Options A specific session ID for which to search
	*
	* @return EOS_Success if setting this session ID was successful
	*         EOS_InvalidParameters if the session ID is invalid or null
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_SessionSearch_SetSessionId"))
	static EOSResult EOSSessionSearchSetSessionId(UObject* WorldContextObject, const FEOSHSessionSearch& Handle, FEOSSessionSearchSetSessionIdOptions Options);

	/**
	* Set a target user ID to find and will return at most one search result.  Setting SessionId or SearchParameters will result in EOS_SessionSearch_Find failing
	* @note a search result will only be found if this user is in a public session
	*
	* @param Options a specific target user ID to find
	*
	* @return EOS_Success if setting this target user ID was successful
	*         EOS_InvalidParameters if the target user ID is invalid or null
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_SessionSearch_SetTargetUserId"))
	static EOSResult EOSSessionSearchSetTargetUserId(UObject* WorldContextObject, const FEOSHSessionSearch& Handle, FEOSSessionSearchSetTargetUserIdOptions Options);

	/**
	* Add a parameter to an array of search criteria combined via an implicit AND operator.  Setting SessionId or TargetUserId will result in EOS_SessionSearch_Find failing
	*
	* @param Options a search parameter and its comparison op
	*
	* @return EOS_Success if setting this search parameter was successful
	*         EOS_InvalidParameters if the search criteria is invalid or null
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*
	* @see EOS_Sessions_AttributeData
	* @see EOS_EComparisonOp
	*/
	static EOSResult EOSSessionSearchSetParameter(UObject* WorldContextObject, const FEOSHSessionSearch& Handle, const FEOSSessionSearchSetParameterOptions& Options);

	/**
	* Add a parameter to an array of search criteria combined via an implicit AND operator.  Setting SessionId or TargetUserId will result in EOS_SessionSearch_Find failing
	*
	* @param Options a search parameter and its comparison op
	*
	* @return EOS_Success if setting this search parameter was successful
	*         EOS_InvalidParameters if the search criteria is invalid or null
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*
	* @see EOS_Sessions_AttributeData
	* @see EOS_EComparisonOp
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_SessionSearch_SetParameterBool"))
	static EOSResult EOSSessionSearchSetParameterBool(UObject* WorldContextObject, const FEOSHSessionSearch& Handle, FString Key, bool bValue, EEOSEComparisonOp ComparisonOp);

	/**
	* Add a parameter to an array of search criteria combined via an AND operator.  Setting SessionId or TargetUserId will result in EOS_SessionSearch_Find failing
	*
	* @param Options a search parameter and its comparison op
	*
	* @return EOS_Success if setting this search parameter was successful
	*         EOS_InvalidParameters if the search criteria is invalid or null
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*
	* @see EOS_Sessions_AttributeData
	* @see EOS_EComparisonOp
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_SessionSearch_SetParameterDouble"))
	static EOSResult EOSSessionSearchSetParameterDouble(UObject* WorldContextObject, const FEOSHSessionSearch& Handle, FString Key, FString Value, EEOSEComparisonOp ComparisonOp);

	/**
	 * Add a parameter to an array of search criteria combined via an AND operator.  Setting SessionId or TargetUserId will result in EOS_SessionSearch_Find failing
	 *
	 * @param Options a search parameter and its comparison op
	 *
	 * @return EOS_Success if setting this search parameter was successful
	 *         EOS_InvalidParameters if the search criteria is invalid or null
	 *         EOS_IncompatibleVersion if the API version passed in is incorrect
	 *
	 * @see EOS_Sessions_AttributeData
	 * @see EOS_EComparisonOp
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_SessionSearch_SetParameterInt64"))
	static EOSResult EOSSessionSearchSetParameterInt64(UObject* WorldContextObject, const FEOSHSessionSearch& Handle, FString Key, FString Value, EEOSEComparisonOp ComparisonOp);

	/**
	 * Add a parameter to an array of search criteria combined via an AND operator.  Setting SessionId or TargetUserId will result in EOS_SessionSearch_Find failing
	 *
	 * @param Options a search parameter and its comparison op
	 *
	 * @return EOS_Success if setting this search parameter was successful
	 *         EOS_InvalidParameters if the search criteria is invalid or null
	 *         EOS_IncompatibleVersion if the API version passed in is incorrect
	 *
	 * @see EOS_Sessions_AttributeData
	 * @see EOS_EComparisonOp
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_SessionSearch_SetParameterString"))
	static EOSResult EOSSessionSearchSetParameterString(UObject* WorldContextObject, const FEOSHSessionSearch& Handle, FString Key, FString Value, EEOSEComparisonOp ComparisonOp);

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
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_SessionSearch_RemoveParameter"))
	static EOSResult EOSSessionSearchRemoveParameter(UObject* WorldContextObject, const FEOSHSessionSearch& Handle, FEOSSessionSearchRemoveParameterOptions Options);

	/**
	* Set the maximum number of search results to return in the query, can't be more than EOS_SESSIONS_MAX_SEARCH_RESULTS
	*
	* @param Options maximum number of search results to return in the query
	*
	* @return EOS_Success if setting the max results was successful
	*         EOS_InvalidParameters if the number of results requested is invalid
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_SessionSearch_SetMaxResults"))
	static EOSResult EOSSessionSearchSetMaxResults(UObject* WorldContextObject, const FEOSHSessionSearch& Handle, FEOSSessionSearchSetMaxResultsOptions Options);

	/**
	* Find sessions matching the search criteria setup via this session search handle.
	* When the operation completes, this handle will have the search results that can be parsed
	*
	* @param Options Structure containing information about the search criteria to use
	* @param Callback A callback that is fired when the search operation completes, either successfully or in error
	*
	* @return EOS_Success if the find operation completes successfully
	*         EOS_NotFound if searching for an individual session by sessionid or targetuserid returns no results
	*         EOS_InvalidParameters if any of the options are incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_SessionSearch_Find"))
	void EOSSessionSearchFind(UObject* WorldContextObject, const FEOSHSessionSearch& Handle, FEOSSessionSearchFindOptions Options, const FOnSessionFindCallback& Callback);

	/**
	* Get the number of search results found by the search parameters in this search
	*
	* @param Options Options associated with the search count
	*
	* @return return the number of search results found by the query or 0 if search is not complete
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_SessionSearch_GetSearchResultCount"))
	static int32 EOSSessionSearchGetSearchResultCount(UObject* WorldContextObject, const FEOSHSessionSearch& Handle, FEOSSessionSearchGetSearchResultCountOptions Options);

	/**
	* EOS_SessionSearch_CopySearchResultByIndex is used to immediately retrieve a handle to the session information from a given search result.
	* If the call returns an EOS_Success result, the out parameter, OutSessionHandle, must be passed to EOS_SessionDetails_Release to release the memory associated with it.
	*
	* @param Options Structure containing the input parameters
	* @param OutSessionHandle out parameter used to receive the session handle
	*
	* @return EOS_Success if the information is available and passed out in OutSessionHandle
	*         EOS_InvalidParameters if you pass an invalid index or a null pointer for the out parameter
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*
	* @see EOS_SessionSearch_CopySearchResultByIndexOptions
	* @see EOS_SessionDetails_Release
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_SessionSearch_CopySearchResultByIndex"))
	static EOSResult EOSSessionSearchCopySearchResultByIndex(UObject* WorldContextObject, const FEOSHSessionSearch& Handle, const FEOSSessionSearchCopySearchResultByIndexOptions& Options, FEOSHSessionDetails& OutSessionHandle);

	/**
	* Release the memory associated with session modification.
	* This must be called on data retrieved from EOS_Sessions_CreateSessionModification or EOS_Sessions_UpdateSessionModification
	*
	* @param SessionModificationHandle - The session modification handle to release
	*
	* @see EOS_Sessions_CreateSessionModification
	* @see EOS_Sessions_UpdateSessionModification
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions|Memory")
	static void EOSSessionModificationRelease(UObject* WorldContextObject, const FEOSHSessionModification& SessionModificationHandle);

	/**
	 * Release the memory associated with an active session.
	 * This must be called on data retrieved from EOS_Sessions_CopyActiveSessionHandle
	 *
	 * @param ActiveSessionHandle - The active session handle to release
	 *
	 * @see EOS_Sessions_CopyActiveSessionHandle
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions|Memory")
	static void EOSActiveSessionRelease(UObject* WorldContextObject, const FEOSHActiveSession& ActiveSessionHandle);

	/**
	 * Release the memory associated with a single session. This must be called on data retrieved from EOS_SessionSearch_CopySearchResultByIndex.
	 *
	 * @param SessionHandle - The session handle to release
	 *
	 * @see EOS_SessionSearch_CopySearchResultByIndex
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions|Memory")
	static void EOSSessionDetailsRelease(UObject* WorldContextObject, const FEOSHSessionDetails& SessionHandle);

	/**
	 * Release the memory associated with a session search. This must be called on data retrieved from EOS_Sessions_CreateSessionSearch.
	 *
	 * @param SessionSearchHandle - The session search handle to release
	 *
	 * @see EOS_Sessions_CreateSessionSearch
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sessions|Memory")
	static void EOSSessionSearchRelease(UObject* WorldContextObject, const FEOSHSessionSearch& SessionSearchHandle);
private:
	static void Internal_OnSessionsUpdateSessionCallback(const EOS_Sessions_UpdateSessionCallbackInfo* Data);
	static void Internal_OnSessionsDestroySessionCallback(const EOS_Sessions_DestroySessionCallbackInfo* Data);
	static void Internal_OnSessionsJoinSessionCallback(const EOS_Sessions_JoinSessionCallbackInfo* Data);
	static void Internal_OnSessionsStartSessionCallback(const EOS_Sessions_StartSessionCallbackInfo* Data);
	static void Internal_OnSessionsEndSessionCallback(const EOS_Sessions_EndSessionCallbackInfo* Data);
	static void Internal_OnSessionRegisterPlayersCallback(const EOS_Sessions_RegisterPlayersCallbackInfo* Data);
	static void Internal_OnSessionsUnregisterPlayersCallback(const EOS_Sessions_UnregisterPlayersCallbackInfo* Data);
	static void Internal_OnSessionsSendInviteCallback(const EOS_Sessions_SendInviteCallbackInfo* Data);
	static void Internal_OnSessionsRejectInviteCallback(const EOS_Sessions_RejectInviteCallbackInfo* Data);
	static void Internal_OnSessionsQueryInvitesCallback(const EOS_Sessions_QueryInvitesCallbackInfo* Data);
	static void Internal_OnSessionsFindCallback(const EOS_SessionSearch_FindCallbackInfo* Data);
protected:
	TMap<FEOSNotificationId, FSessionInviteReceivedCallback*> m_SessionInviteReceivedCallbacks;
	TMap<FEOSNotificationId, FSessionInviteAcceptedCallback*> m_SessionInviteAcceptedCallbacks;
	TMap<FEOSNotificationId, FSessionJoinSessionAcceptedCallback*> m_SessionJoinSessionAcceptedCallbacks;
	
	
	
	
	
};
