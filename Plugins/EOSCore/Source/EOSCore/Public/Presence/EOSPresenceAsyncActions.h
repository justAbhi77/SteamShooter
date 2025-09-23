/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "Presence/EOSPresenceTypes.h"
#include "Core/EOSCoreAsync.h"
#include "EOSPresenceAsyncActions.generated.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCorePresenceQueryPresence
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCorePresenceQueryPresence : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPresenceQueryPresenceCompleteCallbackDelegate OnCallback;
private:
	FOnPresenceQueryPresenceCompleteCallback m_Callback;
public:
	UEOSCorePresenceQueryPresence() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCorePresenceQueryPresence() { m_Callback.Unbind(); }
public:
	/**
	* Query a user's presence. This must complete successfully before CopyPresence will have valid results. If HasPresence returns true for a remote
	* user, this does not need to be called.
	*
	* @param Options Object containing properties related to who is querying presence and for what user
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Presence|Async", DisplayName = "EOS_Presence_QueryPresence"))
	static UEOSCorePresenceQueryPresence* EOSPresenceQueryPresenceAsync(UObject* WorldContextObject, FEOSPresenceQueryPresenceOptions Options);
protected:
	FEOSPresenceQueryPresenceOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSPresenceQueryPresenceCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCorePresenceSetPresence
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCorePresenceSetPresence : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPresenceSetPresenceCompleteCallbackDelegate OnCallback;
private:
	FOnPresenceSetPresenceCompleteCallback m_Callback;
public:
	UEOSCorePresenceSetPresence() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCorePresenceSetPresence() { m_Callback.Unbind(); }
public:
	/**
	* Sets your new presence with the data applied to a PresenceModificationHandle. The PresenceModificationHandle can be released safely after calling this function.
	*
	* @param Options Object containing a PresenceModificationHandle and associated user data
	*
	* @see EOS_Presence_CreatePresenceModification
	* @see EOS_PresenceModification_Release
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Presence|Async", DisplayName = "EOS_Presence_SetPresence"))
	static UEOSCorePresenceSetPresence* EOSPresenceSetPresenceAsync(UObject* WorldContextObject, FEOSPresenceSetPresenceOptions Options);
protected:
	FEOSPresenceSetPresenceOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSPresenceSetPresenceCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};
