/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "UserInfo/EOSUserInfoTypes.h"
#include "Core/EOSCoreAsync.h"
#include "EOSUserInfoAsyncActions.generated.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreUserInfoQueryUserInfo
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreUserInfoQueryUserInfo : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnQueryUserInfoCallbackDelegate OnCallback;
private:
	FOnQueryUserInfoCallback m_Callback;
public:
	UEOSCoreUserInfoQueryUserInfo() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCoreUserInfoQueryUserInfo() { m_Callback.Unbind(); }
public:
	/**
	* EOS_UserInfo_QueryUserInfo is used to start an asynchronous query to retrieve information, such as display name, about another account.
	* Once the callback has been fired with a successful ResultCode, it is possible to call EOS_UserInfo_CopyUserInfo to receive an EOS_UserInfo containing the available information.
	*
	* @param Options structure containing the input parameters
	*
	* @see EOS_UserInfo
	* @see EOS_UserInfo_CopyUserInfo
	* @see EOS_UserInfo_QueryUserInfoOptions
	* @see EOS_UserInfo_OnQueryUserInfoCallback
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|UserInfo|Async", DisplayName = "EOS_UserInfo_QueryUserInfo"))
	static UEOSCoreUserInfoQueryUserInfo* EOSUserInfoQueryUserInfoAsync(UObject* WorldContextObject, FEOSUserInfoQueryUserInfoOptions Options);
protected:
	FEOSUserInfoQueryUserInfoOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSUserInfoQueryUserInfoCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreUserInfoQueryUserInfoByDisplayName
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreUserInfoQueryUserInfoByDisplayName : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnQueryUserInfoByDisplayNameCallbackDelegate OnCallback;
private:
	FOnQueryUserInfoByDisplayNameCallback m_Callback;
public:
	UEOSCoreUserInfoQueryUserInfoByDisplayName() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCoreUserInfoQueryUserInfoByDisplayName() { m_Callback.Unbind(); }
public:
	/**
	* EOS_UserInfo_QueryUserInfoByDisplayName is used to start an asynchronous query to retrieve user information by display name. This can be useful for getting the EOS_EpicAccountId for a display name.
	* Once the callback has been fired with a successful ResultCode, it is possible to call EOS_UserInfo_CopyUserInfo to receive an EOS_UserInfo containing the available information.
	*
	* @param Options structure containing the input parameters
	*
	* @see EOS_UserInfo
	* @see EOS_UserInfo_CopyUserInfo
	* @see EOS_UserInfo_QueryUserInfoByDisplayNameOptions
	* @see EOS_UserInfo_OnQueryUserInfoByDisplayNameCallback
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|UserInfo|Async", DisplayName = "EOS_UserInfo_QueryUserInfoByDisplayName"))
	static UEOSCoreUserInfoQueryUserInfoByDisplayName* EOSUserInfoQueryUserInfoByDisplayNameAsync(UObject* WorldContextObject, FEOSUserInfoQueryUserInfoByDisplayNameOptions Options);
protected:
	FEOSUserInfoQueryUserInfoByDisplayNameOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSUserInfoQueryUserInfoByDisplayNameCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreUserInfoByExternalAccount
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreUserInfoByExternalAccount : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnQueryUserInfoByExternalAccountCallbackDelegate OnCallback;
private:
	FOnQueryUserInfoByExternalAccountCallback m_Callback;
public:
	UEOSCoreUserInfoByExternalAccount() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCoreUserInfoByExternalAccount() { m_Callback.Unbind(); }
public:
	/**
	* EOS_UserInfo_QueryUserInfoByExternalAccount is used to start an asynchronous query to retrieve user information by external accounts.
	* This can be useful for getting the EOS_EpicAccountId for external accounts.
	* Once the callback has been fired with a successful ResultCode, it is possible to call CopyUserInfo to receive an EOS_UserInfo containing the available information.
	*
	* @param Options structure containing the input parameters
	*
	* @see EOS_UserInfo
	* @see EOS_UserInfo_QueryUserInfoByExternalAccountOptions
	* @see EOS_UserInfo_OnQueryUserInfoByExternalAccountCallback
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|UserInfo|Async", DisplayName = "EOS_UserInfo_QueryUserInfoByExternalAccount"))
	static UEOSCoreUserInfoByExternalAccount* EOSUserInfoQueryUserInfoByExternalAccountAsync(UObject* WorldContextObject, FEOSUserInfoQueryUserInfoByExternalAccountOptions Options);
protected:
	FEOSUserInfoQueryUserInfoByExternalAccountOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSUserInfoQueryUserInfoByExternalAccountCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};
