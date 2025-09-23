/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "EOSCoreModule.h"
#include "Auth/EOSAuthTypes.h"
#include "EOSAuthentication.generated.h"

/**
 * The Auth Interface is used to manage local user permissions and access to backend services through the verification of various forms of credentials.
 * All Auth Interface calls take a handle of type EOS_HAuth as the first parameter.
 * This handle can be retrieved from a EOS_HPlatform handle by using the EOS_Platform_GetAuthInterface function.
 *
 * @see EOS_Platform_GetAuthInterface
 */

UCLASS()
class EOSCORE_API UCoreAuthentication : public UEOSCoreSubsystem
{
	GENERATED_BODY()
public:
	virtual void Deinitialize() override;
public:
	/**
	 * The Auth Interface is used to manage local user permissions and access to backend services through the verification of various forms of credentials.
	 * All Auth Interface calls take a handle of type EOS_HAuth as the first parameter.
	 * This handle can be retrieved from a EOS_HPlatform handle by using the EOS_Platform_GetAuthInterface function.
	 *
	 * @see EOS_Platform_GetAuthInterface
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Interfaces", meta = (WorldContext = "WorldContextObject"))
	static UCoreAuthentication* GetAuthentication(UObject* WorldContextObject);

public:
	/**
	* Login/Authenticate with user credentials.
	*
	* @param Options structure containing the account credentials to use during the login operation
	* @param Callback a callback that is fired when the login operation completes, either successfully or in error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Authentication", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Auth_Login"))
	void EOSAuthLogin(UObject* WorldContextObject, FEOSAuthLoginOptions Options, const FOnAuthLoginCallback& Callback);

	/**
	* Signs the player out of the online service.
	*
	* @param Options structure containing information about which account to log out.
	* @param Callback a callback that is fired when the logout operation completes, either successfully or in error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Authentication", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Auth_Logout"))
	void EOSAuthLogout(UObject* WorldContextObject, FEOSAuthLogoutOptions Options, const FOnAuthLogoutCallback& Callback);

	/**
	* Link external account by continuing previous login attempt with a continuance token.
	*
	* On Desktop and Mobile platforms, the user will be presented the Epic Account Portal to resolve their identity.
	*
	* On Console, the user will login to their Epic Account using an external device, e.g. a mobile device or a desktop PC,
	* by browsing to the presented authentication URL and entering the device code presented by the game on the console.
	*
	* On success, the user will be logged in at the completion of this action.
	* This will commit this external account to the Epic Account and cannot be undone in the SDK.
	*
	* @param Options structure containing the account credentials to use during the link account operation
	* @param Callback a callback that is fired when the link account operation completes, either successfully or in error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Authentication", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Auth_LinkAccount"))
	void EOSAuthLinkAccount(UObject* WorldContextObject, FEOSAuthLinkAccountOptions Options, const FOnAuthLinkAccountCallback& Callback);

	/**
	* Deletes a previously received and locally stored persistent auth access token for the currently logged in user of the local device.
	*
	* On Desktop and Mobile platforms, the access token is deleted from the keychain of the local user and a backend request is made to revoke the token on the authentication server.
	* On Console platforms, even though the caller is responsible for storing and deleting the access token on the local device,
	* this function should still be called with the access token before its deletion to make the best effort in attempting to also revoke it on the authentication server.
	* If the function would fail on Console, the caller should still proceed as normal to delete the access token locally as intended.
	*
	* @param Options structure containing operation input parameters
	* @param Callback a callback that is fired when the deletion operation completes, either successfully or in error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Authentication", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Auth_DeletePersistentAuth"))
	void EOSAuthDeletePersistentAuth(UObject* WorldContextObject, const FEOSAuthDeletePersistentAuthOptions& Options, const FOnAuthDeletePersistentAuthCallback& Callback);

	/**
	* Contact the backend service to verify validity of an existing user auth token.
	* This function is intended for server-side use only.
	*
	* @param Options structure containing information about the auth token being verified
	* @param Callback a callback that is fired when the logout operation completes, either successfully or in error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Authentication", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Auth_VerifyUserAuth"))
	void EOSAuthVerifyUserAuth(UObject* WorldContextObject, const FEOSAuthVerifyUserAuthOptions& Options, const FOnAuthVerifyUserAuthCallback& Callback);

	/**
	* Fetch the number of accounts that are logged in.
	*
	* @return the number of accounts logged in.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Authentication", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Auth_GetLoggedInAccountsCount"))
	static int32 EOSAuthGetLoggedInAccountsCount(UObject* WorldContextObject);

	/**
	* Fetch an Epic Online Services Account ID that is logged in.
	*
	* @param Index An index into the list of logged in accounts. If the index is out of bounds, the returned Epic Online Services Account ID will be invalid.
	*
	* @return The Epic Online Services Account ID associated with the index passed
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Authentication", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Auth_GetLoggedInAccountByIndex"))
	static FEOSEpicAccountId EOSAuthGetLoggedInAccountByIndex(UObject* WorldContextObject, int32 Index);

	/**
	* Fetches the login status for an Epic Online Services Account ID.
	*
	* @param LocalUserId The Epic Online Services Account ID of the user being queried
	*
	* @return The enum value of a user's login status
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Authentication", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Auth_GetLoginStatus"))
	static EOSELoginStatus EOSAuthGetLoginStatus(UObject* WorldContextObject, FEOSEpicAccountId LocalUserId);

	/**
	* Fetches the login status for an account id.
	*
	* @param LocalUserId the account id of the user being queried
	*
	* @return the enum value of a user's login status
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Authentication", meta = (DisplayName = "EOS_Auth_GetLoginStatus (Pure)"))
	static EOSELoginStatus EOSAuthGetLoginStatusPure(UObject* WorldContextObject, const FEOSEpicAccountId LocalUserId) { return EOSAuthGetLoginStatus(WorldContextObject, LocalUserId); }

	/**
	* Fetches a user auth token for an Epic Online Services Account ID.
	*
	* @param Options Structure containing the api version of CopyUserAuthToken to use
	* @param LocalUserId The Epic Online Services Account ID of the user being queried
	* @param OutUserAuthToken The auth token for the given user, if it exists and is valid; use EOS_Auth_Token_Release when finished
	*
	* @see EOS_Auth_Token_Release
	*
	* @return EOS_Success if the information is available and passed out in OutUserAuthToken
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_NotFound if the auth token is not found or expired.
	*
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Authentication", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Auth_CopyUserAuthToken"))
	static EOSResult EOSAuthCopyUserAuthToken(UObject* WorldContextObject, FEOSAuthCopyUserAuthTokenOptions Options, FEOSEpicAccountId LocalUserId, FEOSAuthToken& OutUserAuthToken);

	/**
	 * Fetch an ID token for an Epic Account ID.
	 *
	 * ID tokens are used to securely verify user identities with online services.
	 * The most common use case is using an ID token to authenticate the local user by their selected account ID,
	 * which is the account ID that should be used to access any game-scoped data for the current application.
	 *
	 * An ID token for the selected account ID of a locally authenticated user will always be readily available.
	 * To retrieve it for the selected account ID, you can use EOS_Auth_CopyIdToken directly after a successful user login.
	 *
	 * @param Options Structure containing the account ID for which to copy an ID token.
	 * @param OutIdToken An ID token for the given user, if it exists and is valid; use EOS_Auth_IdToken_Release when finished.
	 *
	 * @see EOS_Auth_IdToken_Release
	 *
	 * @return EOS_Success if the information is available and passed out in OutUserIdToken
	 *         EOS_InvalidParameters if you pass a null pointer for the out parameter
	 *         EOS_NotFound if the Id token is not found or expired.
	 *
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Authentication", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Auth_CopyIdToken"))
	static EOSResult EOSAuthCopyIdToken(UObject* WorldContextObject, FEOSAuthCopyIdTokenOptions Options, FEOSAuthIdToken& OutIdToken);

	/**
	 * Query the backend for an ID token that describes one of the merged account IDs of a local authenticated user.
	 *
	 * The ID token can be used to impersonate a merged account ID when communicating with online services.
	 *
	 * An ID token for the selected account ID of a locally authenticated user will always be readily available and does not need to be queried explicitly.
	 *
	 * @param Options Structure containing the merged account ID for which to query an ID token.
	 * @param Callback A callback that is fired when the operation completes, either successfully or in error.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Authentication", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Auth_QueryIdToken"))
	static void EOSAuthQueryIdToken(UObject* WorldContextObject, FEOSAuthQueryIdTokenOptions Options, const FOnAuthQueryIdTokenCallback& Callback);

	/**
	 * Verify a given ID token for authenticity and validity.
	 * @note Can only be called by dedicated servers.
	 *
	 * @param Options Structure containing information about the ID token to verify.
	 * @param Callback A callback that is fired when the operation completes, either successfully or in error.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Authentication", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Auth_VerifyIdToken"))
	static void EOSAuthVerifyIdToken(UObject* WorldContextObject, FEOSAuthVerifyIdTokenOptions Options, const FOnAuthVerifyIdTokenCallback& Callback);

	/**
	 * Fetch the selected account ID to the current application for a local authenticated user.
	 *
	 * @param LocalUserId The account ID of a currently logged in account.
	 * @param OutSelectedAccountId The selected account ID corresponding to the given account ID.
	 *
	 * @return EOS_Success if the user is logged in and the information is available.
	 *         EOS_InvalidParameters if the output parameter is NULL.
	 *         EOS_InvalidUser if the input account ID is not locally known.
	 *         EOS_InvalidAuth if the input account ID is not locally logged in.
	 *         EOS_NotFound otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Authentication", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Auth_GetSelectedAccountId"))
	static EOSResult EOSAuthGetSelectedAccountId(UObject* WorldContextObject, FEOSEpicAccountId LocalUserId, FEOSEpicAccountId& OutSelectedAccountId);

	/**
	 * Fetch the number of merged accounts for a given logged in account.
	 *
	 * @param LocalUserId The account ID of a currently logged in account.
	 *
	 * @return the number of merged accounts for the logged in account.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Authentication", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Auth_GetMergedAccountsCount"))
	static int32 EOSAuthGetMergedAccountsCount(UObject* WorldContextObject, FEOSEpicAccountId LocalUserId);

	/**
	 * Fetch one of the merged account IDs for a given logged in account.
	 *
	 * @param LocalUserId The account ID of a currently logged in account.
	 * @param Index An index into the list of merged accounts. If the index is out of bounds, the returned Epic Account ID will be invalid.
	 *
	 * @return The Epic Account ID associated with the index passed.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Authentication", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Auth_GetMergedAccountByIndex"))
	static FEOSEpicAccountId EOSAuthGetMergedAccountByIndex(UObject* WorldContextObject, FEOSEpicAccountId LocalUserId, int32 Index);

	/**
	* Register to receive login status updates.
	* @note must call RemoveNotifyLoginStatusChanged to remove the notification
	*
	* @param Callback a callback that is fired when the login status for a user changes
	*
	* @return handle representing the registered callback
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Authentication", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Auth_AddNotifyLoginStatusChanged"))
	static FEOSNotificationId EOSAuthAddNotifyLoginStatusChanged(UObject* WorldContextObject, const FOnAuthLoginStatusChangedCallback& Callback);

	/**
	 * Unregister from receiving login status updates.
	 *
	 * @param ID handle representing the registered callback
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Authentication", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Auth_RemoveNotifyLoginStatusChanged"))
	static void EOSAuthRemoveNotifyLoginStatusChanged(UObject* WorldContextObject, FEOSNotificationId ID);
private:
	static void Internal_OnAuthLoginCallback(const EOS_Auth_LoginCallbackInfo* Data);
	static void Internal_OnAuthLogoutCallback(const EOS_Auth_LogoutCallbackInfo* Data);
	static void Internal_OnAuthOnLinkAccountCallback(const EOS_Auth_LinkAccountCallbackInfo* Data);
	static void Internal_OnAuthDeletePersistentAuthCallback(const EOS_Auth_DeletePersistentAuthCallbackInfo* Data);
	static void Internal_OnAuthVerifyUserAuthCallback(const EOS_Auth_VerifyUserAuthCallbackInfo* Data);
	static void Internal_OnAuthQueryIdTokenCallback(const EOS_Auth_QueryIdTokenCallbackInfo* Data);
	static void Internal_OnAuthVerifyIdTokenCallback(const EOS_Auth_VerifyIdTokenCallbackInfo* Data);
protected:
	TMap<FEOSNotificationId, FAuthLoginStatusChangedCallback*> m_AuthLoginStatusChangedCallbacks;
};
