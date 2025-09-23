/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "Friends/EOSFriendsTypes.h"
#include "Core/EOSCoreAsync.h"
#include "EOSFriendsAsyncActions.generated.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreFriendsQueryFriends
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreFriendsQueryFriends : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnFriendsQueryFriendsCallbackDelegate OnCallback;
private:
	FOnFriendsQueryFriendsCallback m_Callback;
public:
	UEOSCoreFriendsQueryFriends() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCoreFriendsQueryFriends() { m_Callback.Unbind(); }
public:
	/**
	* Starts an asynchronous task that reads the user's friends list from the backend service, caching it for future use.
	*
	* @note When the Social Overlay is enabled then this will be called automatically.  The Social Overlay is enabled by default (see EOS_PF_DISABLE_SOCIAL_OVERLAY).
	*
	* @param Options structure containing the account for which to retrieve the friends list
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Friends|Async", DisplayName = "EOS_Friends_Query_Friends"))
	static UEOSCoreFriendsQueryFriends* EOSFriendsQueryFriendsAsync(UObject* WorldContextObject, FEOSFriendsQueryFriendsOptions Options);
protected:
	FEOSFriendsQueryFriendsOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSFriendsQueryFriendsCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreFriendsSendInvite
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreFriendsSendInvite : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnFriendsSendInviteCallbackDelegate OnCallback;
private:
	FOnFriendsSendInviteCallback m_Callback;
public:
	UEOSCoreFriendsSendInvite() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCoreFriendsSendInvite() { m_Callback.Unbind(); }
public:
	/**
	* Starts an asynchronous task that sends a friend invitation to another user. The completion delegate is executed after the backend response has been received.
	* It does not indicate that the target user has responded to the friend invitation.
	*
	* @param Options structure containing the account to send the invite from and the account to send the invite to
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Friends|Async", DisplayName = "EOS_Friends_SendInvite"))
	static UEOSCoreFriendsSendInvite* EOSFriendsSendInviteAsync(UObject* WorldContextObject, FEOSFriendsSendInviteOptions Options);
protected:
	FEOSFriendsSendInviteOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSFriendsSendInviteCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreFriendsAcceptInvite
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreFriendsAcceptInvite : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnFriendsAcceptInviteCallbackDelegate OnCallback;
private:
	FOnFriendsAcceptInviteCallback m_Callback;
public:
	UEOSCoreFriendsAcceptInvite() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCoreFriendsAcceptInvite() { m_Callback.Unbind(); }
public:
	/**
	* Starts an asynchronous task that accepts a friend invitation from another user. The completion delegate is executed after the backend response has been received.
	*
	* @param Options structure containing the logged in account and the inviting account
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Friends|Async", DisplayName = "EOS_Friends_AcceptInvite"))
	static UEOSCoreFriendsAcceptInvite* EOSFriendsAcceptInviteAsync(UObject* WorldContextObject, FEOSFriendsAcceptInviteOptions Options);
protected:
	FEOSFriendsAcceptInviteOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSFriendsAcceptInviteCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreFriendsRejectInvite
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreFriendsRejectInvite : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnFriendsRejectInviteCallbackDelegate OnCallback;
private:
	FOnFriendsRejectInviteCallback m_Callback;
public:
	UEOSCoreFriendsRejectInvite() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCoreFriendsRejectInvite() { m_Callback.Unbind(); }
public:
	/**
	* Starts an asynchronous task that rejects a friend invitation from another user. The completion delegate is executed after the backend response has been received.
	*
	* @param Options structure containing the logged in account and the inviting account
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Friends|Async", DisplayName = "EOS_Friends_RejectInvite"))
	static UEOSCoreFriendsRejectInvite* EOSFriendsRejectInviteAsync(UObject* WorldContextObject, FEOSFriendsRejectInviteOptions Options);
protected:
	FEOSFriendsRejectInviteOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSFriendsRejectInviteCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};
