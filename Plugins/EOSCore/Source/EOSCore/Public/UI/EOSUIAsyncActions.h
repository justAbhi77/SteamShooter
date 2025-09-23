/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "UI/EOSUITypes.h"
#include "Core/EOSCoreAsync.h"
#include "EOSUIAsyncActions.generated.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreUIShowFriends
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreUIShowFriends : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnShowFriendsCallbackDelegate OnCallback;
private:
	FOnShowFriendsCallback m_Callback;
public:
	UEOSCoreUIShowFriends() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCoreUIShowFriends() { m_Callback.Unbind(); }
public:
	/**
	* Opens the Social Overlay with a request to show the friends list.
	*
	* @param Options Structure containing the Epic Online Services Account ID of the friends list to show.
	*
	* @return EOS_Success If the Social Overlay has been notified about the request.
	*         EOS_InvalidParameters If any of the options are incorrect.
	*         EOS_NotConfigured If the Social Overlay is not properly configured.
	*         EOS_NoChange If the Social Overlay is already visible.
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|UI|Async", DisplayName = "EOS_UI_ShowFriends"))
	static UEOSCoreUIShowFriends* EOSUIShowFriendsAsync(UObject* WorldContextObject, FEOSUIShowFriendsOptions Options);
protected:
	FEOSUIShowFriendsOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSUIShowFriendsCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreUIHideFriends
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreUIHideFriends : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnHideFriendsCallbackDelegate OnCallback;
private:
	FOnHideFriendsCallback m_Callback;
public:
	UEOSCoreUIHideFriends() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCoreUIHideFriends() { m_Callback.Unbind(); }
public:
	/**
	* Hides the active Social Overlay.
	*
	* @param Options Structure containing the Epic Online Services Account ID of the browser to close.
	*
	* @return EOS_Success If the Social Overlay has been notified about the request.
	*         EOS_InvalidParameters If any of the options are incorrect.
	*         EOS_NotConfigured If the Social Overlay is not properly configured.
	*         EOS_NoChange If the Social Overlay is already hidden.
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|UI|Async", DisplayName = "EOS_UI_HideFriends"))
	static UEOSCoreUIHideFriends* EOSUIHideFriendsAsync(UObject* WorldContextObject, FEOSUIHideFriendsOptions Options);
protected:
	FEOSUIHideFriendsOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSUIHideFriendsCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};
