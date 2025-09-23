/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "Auth/EOSAuthTypes.h"
#include "Core/EOSCoreAsync.h"
#include "EOSAuthAsyncActions.generated.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreAuthLogin
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreAuthLogin : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnAuthLoginCallbackDelegate OnCallback;
private:
	FOnAuthLoginCallback m_Callback;
public:
	UEOSCoreAuthLogin() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCoreAuthLogin() { m_Callback.Unbind(); }
public:
	/**
	* Login/Authenticate with user credentials.
	*
	* @param Options structure containing the account credentials to use during the login operation
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Auth|Async", DisplayName = "EOS_Auth_Login"))
	static UEOSCoreAuthLogin* EOSAuthLoginAsync(UObject* WorldContextObject, FEOSAuthLoginOptions Options);
protected:
	FEOSAuthLoginOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSAuthLoginCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreAuthLogout
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreAuthLogout : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnAuthLogoutCallbackDelegate OnCallback;
private:
	FOnAuthLogoutCallback m_Callback;
public:
	UEOSCoreAuthLogout() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCoreAuthLogout() { m_Callback.Unbind(); }
public:
	/**
	* Signs the player out of the online service.
	*
	* @param Options structure containing information about which account to log out.
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Auth|Async", DisplayName = "EOS_Auth_Logout"))
	static UEOSCoreAuthLogout* EOSAuthLogoutAsync(UObject* WorldContextObject, FEOSAuthLogoutOptions Options);
protected:
	FEOSAuthLogoutOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSAuthLogoutCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreAuthLinkAccount
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreAuthLinkAccount : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnAuthLinkAccountCallbackDelegate OnCallback;
private:
	FOnAuthLinkAccountCallback m_Callback;
public:
	UEOSCoreAuthLinkAccount() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCoreAuthLinkAccount() { m_Callback.Unbind(); }
public:
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
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Auth|Async", DisplayName = "EOS_Auth_LinkAccount"))
	static UEOSCoreAuthLinkAccount* EOSAuthLinkAccount(UObject* WorldContextObject, FEOSAuthLinkAccountOptions Options);
protected:
	FEOSAuthLinkAccountOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSAuthLinkAccountCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreAuthDeletePersistentAuth
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreAuthDeletePersistentAuth : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnAuthDeletePersistentAuthCallbackDelegate OnCallback;
private:
	FOnAuthDeletePersistentAuthCallback m_Callback;
public:
	UEOSCoreAuthDeletePersistentAuth() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCoreAuthDeletePersistentAuth() { m_Callback.Unbind(); }
public:
	/**
	* Deletes a previously received and locally stored persistent auth access token for the currently logged in user of the local device.
	*
	* On Desktop and Mobile platforms, the access token is deleted from the keychain of the local user and a backend request is made to revoke the token on the authentication server.
	* On Console platforms, even though the caller is responsible for storing and deleting the access token on the local device,
	* this function should still be called with the access token before its deletion to make the best effort in attempting to also revoke it on the authentication server.
	* If the function would fail on Console, the caller should still proceed as normal to delete the access token locally as intended.
	*
	* @param Options structure containing operation input parameters
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Auth|Async", DisplayName = "EOS_Auth_DeletePersistentAuth"))
	static UEOSCoreAuthDeletePersistentAuth* EOSAuthDeletePersistentAuthAsync(UObject* WorldContextObject, const FEOSAuthDeletePersistentAuthOptions& Options);
protected:
	FEOSAuthDeletePersistentAuthOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSAuthDeletePersistentAuthCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreAuthVerifyUserAuth
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreAuthVerifyUserAuth : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnAuthVerifyUserAuthCallbackDelegate OnCallback;
private:
	FOnAuthVerifyUserAuthCallback m_Callback;
public:
	UEOSCoreAuthVerifyUserAuth() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCoreAuthVerifyUserAuth() { m_Callback.Unbind(); }
public:
	/**
	* Contact the backend service to verify validity of an existing user auth token.
	* This function is intended for server-side use only.
	*
	* @param Options structure containing information about the auth token being verified
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Auth|Async", DisplayName = "EOS_Auth_VerifyUserAuth"))
	static UEOSCoreAuthVerifyUserAuth* EOSAuthVerifyUserAuthAsync(UObject* WorldContextObject, FEOSAuthVerifyUserAuthOptions Options);
protected:
	FEOSAuthVerifyUserAuthOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSAuthVerifyUserAuthCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};
