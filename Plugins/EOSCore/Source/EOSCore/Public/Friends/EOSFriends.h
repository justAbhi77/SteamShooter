/**
* Copyright (C) 2017-2022 | eelDev AB
*
 official EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "EOSCoreModule.h"
#include "eos_friends_types.h"
#include "Friends/EOSFriendsTypes.h"
#include "EOSFriends.generated.h"

/**
 * The Friends Interface is used to manage a user's friends list, by interacting with the backend services, and to retrieve the cached list of friends and pending invitations.
 * All Friends Interface calls take a handle of type EOS_HFriends as the first parameter.
 * This handle can be retrieved from a EOS_HPlatform handle by using the EOS_Platform_GetFriendsInterface function.
 *
 * @see EOS_Platform_GetFriendsInterface
 */

UCLASS()
class EOSCORE_API UCoreFriends : public UEOSCoreSubsystem
{
	GENERATED_BODY()
public:
	virtual void Deinitialize() override;
public:
	/**
	 * The Friends Interface is used to manage a user's friends list, by interacting with the backend services, and to retrieve the cached list of friends and pending invitations.
	 * All Friends Interface calls take a handle of type EOS_HFriends as the first parameter.
	 * This handle can be retrieved from a EOS_HPlatform handle by using the EOS_Platform_GetFriendsInterface function.
	 *
	 * @see EOS_Platform_GetFriendsInterface
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Interfaces", meta = (WorldContext = "WorldContextObject"))
	static UCoreFriends* GetFriends(UObject* WorldContextObject);

public:
	/**
	* Starts an asynchronous task that reads the user's friends list from the backend service, caching it for future use.
	*
	* @note When the Social Overlay is enabled then this will be called automatically.  The Social Overlay is enabled by default (see EOS_PF_DISABLE_SOCIAL_OVERLAY).
	*
	* @param Options structure containing the account for which to retrieve the friends list
	* @param Callback a callback that is fired when the async operation completes, either successfully or in error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Friends", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Friends_QueryFriends"))
	void EOSFriendsQueryFriends(UObject* WorldContextObject, FEOSFriendsQueryFriendsOptions Options, const FOnFriendsQueryFriendsCallback& Callback);

	/**
	* Starts an asynchronous task that sends a friend invitation to another user. The completion delegate is executed after the backend response has been received.
	* It does not indicate that the target user has responded to the friend invitation.
	*
	* @param Options structure containing the account to send the invite from and the account to send the invite to
	* @param Callback a callback that is fired when the async operation completes, either successfully or in error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Friends", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Friends_SendInvite"))
	void EOSFriendsSendInvite(UObject* WorldContextObject, FEOSFriendsSendInviteOptions Options, const FOnFriendsSendInviteCallback& Callback);

	/**
	* Starts an asynchronous task that accepts a friend invitation from another user. The completion delegate is executed after the backend response has been received.
	*
	* @param Options structure containing the logged in account and the inviting account
	* @param Callback a callback that is fired when the async operation completes, either successfully or in error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Friends", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Friends_AcceptInvite"))
	void EOSFriendsAcceptInvite(UObject* WorldContextObject, FEOSFriendsAcceptInviteOptions Options, const FOnFriendsAcceptInviteCallback& Callback);

	/**
	* Starts an asynchronous task that rejects a friend invitation from another user. The completion delegate is executed after the backend response has been received.
	*
	* @param Options structure containing the logged in account and the inviting account
	* @param Callback a callback that is fired when the async operation completes, either successfully or in error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Friends", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Friends_RejectInvite"))
	void EOSFriendsRejectInvite(UObject* WorldContextObject, FEOSFriendsRejectInviteOptions Options, const FOnFriendsRejectInviteCallback& Callback);

	/**
	* Retrieves the number of friends on the friends list that has already been retrieved by the EOS_Friends_QueryFriends API.
	*
	* @param Options structure containing the Epic Online Services Account ID of user who owns the friends list
	* @return the number of friends on the list
	*
	* @see EOS_Friends_GetFriendAtIndex
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Friends", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Friends_GetFriendsCount"))
	static int32 EOSFriendsGetFriendsCount(UObject* WorldContextObject, FEOSFriendsGetFriendsCountOptions Options);

	/**
	* Retrieves the Epic Online Services Account ID of an entry from the friends list that has already been retrieved by the EOS_Friends_QueryFriends API.
	* The Epic Online Services Account ID returned by this function may belong to an account that has been invited to be a friend or that has invited the local user to be a friend.
	* To determine if the Epic Online Services Account ID returned by this function is a friend or a pending friend invitation, use the EOS_Friends_GetStatus function.
	*
	* @param Options structure containing the Epic Online Services Account ID of the owner of the friends list and the index into the list
	* @return the Epic Online Services Account ID of the friend. Note that if the index provided is out of bounds, the returned Epic Online Services Account ID will be a "null" account ID.
	*
	* @see EOS_Friends_GetFriendsCount
	* @see EOS_Friends_GetStatus
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Friends", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Friends_GetFriendAtIndex"))
	static FEOSEpicAccountId EOSFriendsGetFriendAtIndex(UObject* WorldContextObject, FEOSFriendsGetFriendAtIndexOptions Options);

	/**
	* Retrieve the friendship status between the local user and another user.
	*
	* @param Options structure containing the Epic Online Services Account ID of the friend list to check and the account of the user to test friendship status
	* @return A value indicating whether the two accounts have a friendship, pending invites in either direction, or no relationship
	*         EOS_FS_Friends is returned for two users that have confirmed friendship
	*         EOS_FS_InviteSent is returned when the local user has sent a friend invitation but the other user has not accepted or rejected it
	*         EOS_FS_InviteReceived is returned when the other user has sent a friend invitation to the local user
	*         EOS_FS_NotFriends is returned when there is no known relationship
	*
	* @see EOS_EFriendsStatus
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Friends", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Friends_GetStatus"))
	static EEOSEFriendsStatus EOSFriendsGetStatus(UObject* WorldContextObject, FEOSFriendsGetStatusOptions Options);

	/**
	* Listen for changes to friends for a particular account.
	*
	* @param Callback The callback to be invoked when a change to any friend status changes.
	* @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Friends", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Friends_AddNotifyFriendsUpdate"))
	FEOSNotificationId EOSFriendsAddNotifyFriendsUpdate(UObject* WorldContextObject, const FOnFriendsUpdateCallback& Callback);

	/**
	* Stop listening for friends changes on a previously bound handler.
	*
	* @param Id The previously bound notification ID.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Friends", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Friends_RemoveNotifyFriendsUpdate"))
	static void EOSFriendsRemoveNotifyFriendsUpdate(UObject* WorldContextObject, FEOSNotificationId Id);

	/**
	 * Retrieves the number of blocked users on the blocklist that has already been retrieved by the EOS_Friends_QueryFriends API.
	 *
	 * @param Options structure containing the Epic Account ID of user who owns the blocklist.
	 * @return the number of users on the blocklist.
	 *
	 * @see EOS_Friends_QueryFriends
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Friends", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Friends_GetBlockedUsersCount"))
	int32 EOSFriendsGetBlockedUsersCount(UObject* WorldContextObject, FEOSFriendsGetBlockedUsersCountOptions Options);

	/**
	 * Retrieves the Epic Account ID of an entry from the blocklist that has already been retrieved by the EOS_Friends_QueryFriends API.
	 *
	 * @param Options structure containing the Epic Account ID of the owner of the blocklist and the index into the list.
	 * @return the Epic Account ID of the blocked user. Note that if the index provided is out of bounds, the returned Epic Account ID will be a "null" account ID.
	 *
	 * @see EOS_Friends_QueryFriends
	 * @see EOS_Friends_GetBlockedUsersCount
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Friends", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Friends_GetBlockedUserAtIndex"))
	FEOSEpicAccountId EOSFriendsGetBlockedUserAtIndex(UObject* WorldContextObject, FEOSFriendsGetBlockedUserAtIndexOptions Options);

	/**
	 * Listen for changes to blocklist for a particular account.
	 *
	 * @param Callback The callback to be invoked when a blocklist changes.
	 * @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Friends", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Friends_AddNotifyBlockedUsersUpdate"))
	FEOSNotificationId EOSFriendsAddNotifyBlockedUsersUpdate(UObject* WorldContextObject, const FOnFriendsBlockedUsersUpdateCallback& Callback);
private:
	static void Internal_OnQueryFriendsCallback(const EOS_Friends_QueryFriendsCallbackInfo* Data);
	static void Internal_OnSendInviteCallback(const EOS_Friends_SendInviteCallbackInfo* Data);
	static void Internal_OnAcceptInviteCallback(const EOS_Friends_AcceptInviteCallbackInfo* Data);
	static void Internal_OnRejectInviteCallback(const EOS_Friends_RejectInviteCallbackInfo* Data);
protected:
	TMap<FEOSNotificationId, FFriendsUpdateCallback*> m_FriendsUpdateCallbacks;
	TMap<FEOSNotificationId, FFriendsOnBlockedUsersUpdateCallback*> m_FriendsBlockedUsersUpdateCallbacks;
};
