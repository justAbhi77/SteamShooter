/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "Sessions/EOSSessionsTypes.h"
#include "Core/EOSCoreAsync.h"
#include "EOSSessionsAsyncActions.generated.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreSessionsUpdateSession
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreSessionsUpdateSession : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnSessionUpdateSessionCallbackDelegate OnCallback;
private:
	FOnSessionUpdateSessionCallback m_Callback;
public:
	UEOSCoreSessionsUpdateSession() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCoreSessionsUpdateSession() { m_Callback.Unbind(); }
public:
	/**
	* Update a session given a session modification handle created by EOS_Sessions_CreateSessionModification or EOS_Sessions_UpdateSessionModification
	*
	* @param Options Structure containing information about the session to be updated
	*
	* @return EOS_Success if the update completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_Sessions_OutOfSync if the session is out of sync and will be updated on the next connection with the backend
	*         EOS_NotFound if a session to be updated does not exist
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Sessions|Async", DisplayName = "EOS_Sessions_UpdateSession"))
	static UEOSCoreSessionsUpdateSession* EOSSessionsUpdateSessionAsync(UObject* WorldContextObject, FEOSSessionsUpdateSessionOptions Options);
protected:
	FEOSSessionsUpdateSessionOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSSessionsUpdateSessionCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreSessionsDestroySession
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreSessionsDestroySession : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnSessionDestroySessionCallbackDelegate OnCallback;
private:
	FOnSessionDestroySessionCallback m_Callback;
public:
	UEOSCoreSessionsDestroySession() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCoreSessionsDestroySession() { m_Callback.Unbind(); }
public:
	/**
	* Destroy a session given a session name
	*
	* @param Options Structure containing information about the session to be destroyed
	*
	* @return EOS_Success if the destroy completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_AlreadyPending if the session is already marked for destroy
	*         EOS_NotFound if a session to be destroyed does not exist
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Sessions|Async", DisplayName = "EOS_Sessions_DestroySession"))
	static UEOSCoreSessionsDestroySession* EOSSessionsDestroySessionAsync(UObject* WorldContextObject, FEOSSessionsDestroySessionOptions Options);
protected:
	FEOSSessionsDestroySessionOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSSessionsDestroySessionCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreSessionsJoinSession
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreSessionsJoinSession : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnSessionJoinSessionCallbackDelegate OnCallback;
private:
	FOnSessionJoinSessionCallback m_Callback;
public:
	UEOSCoreSessionsJoinSession() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCoreSessionsJoinSession() { m_Callback.Unbind(); }
public:
	/**
	* Join a session, creating a local session under a given session name.  Backend will validate various conditions to make sure it is possible to join the session.
	*
	* @param Options Structure containing information about the session to be joined
	*
	* @return EOS_Success if the join completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_Sessions_SessionAlreadyExists if the session is already exists or is in the process of being joined
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Sessions|Async", DisplayName = "EOS_Sessions_JoinSession"))
	static UEOSCoreSessionsJoinSession* EOSSessionsJoinSessionAsync(UObject* WorldContextObject, FEOSSessionsJoinSessionOptions Options);
protected:
	FEOSSessionsJoinSessionOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSSessionsJoinSessionCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreSessionsStartSession
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreSessionsStartSession : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnSessionStartSessionCallbackDelegate OnCallback;
private:
	FOnSessionStartSessionCallback m_Callback;
public:
	UEOSCoreSessionsStartSession() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCoreSessionsStartSession() { m_Callback.Unbind(); }
public:
	/**
	* Mark a session as started, making it unable to find if session properties indicate "join in progress" is not available
	*
	* @param Options Structure containing information about the session to be started
	*
	* @return EOS_Success if the start completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_Sessions_OutOfSync if the session is out of sync and will be updated on the next connection with the backend
	*         EOS_NotFound if a session to be started does not exist
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Sessions|Async", DisplayName = "EOS_Sessions_StartSession"))
	static UEOSCoreSessionsStartSession* EOSSessionsStartSessionAsync(UObject* WorldContextObject, FEOSSessionsStartSessionOptions Options);
protected:
	FEOSSessionsStartSessionOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSSessionsStartSessionCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreSessionsEndSession
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreSessionsEndSession : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnSessionEndSessionCallbackDelegate OnCallback;
private:
	FOnSessionEndSessionCallback m_Callback;
public:
	UEOSCoreSessionsEndSession() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCoreSessionsEndSession() { m_Callback.Unbind(); }
public:
	/**
	* Mark a session as ended, making it available to find if "join in progress" was disabled.  The session may be started again if desired
	*
	* @param Options Structure containing information about the session to be ended
	*
	* @return EOS_Success if the end completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_Sessions_OutOfSync if the session is out of sync and will be updated on the next connection with the backend
	*         EOS_NotFound if a session to be ended does not exist
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Sessions|Async", DisplayName = "EOS_Sessions_EndSession"))
	static UEOSCoreSessionsEndSession* EOSSessionsEndSessionAsync(UObject* WorldContextObject, FEOSSessionsEndSessionOptions Options);
protected:
	FEOSSessionsEndSessionOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSSessionsEndSessionCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreSessionsRegisterPlayers
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreSessionsRegisterPlayers : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnSessionRegisterPlayersCallbackDelegate OnCallback;
private:
	FOnSessionRegisterPlayersCallback m_Callback;
public:
	UEOSCoreSessionsRegisterPlayers() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCoreSessionsRegisterPlayers() { m_Callback.Unbind(); }
public:
	/**
	* Register a group of players with the session, allowing them to invite others or otherwise indicate they are part of the session for determining a full session
	*
	* @param Options Structure containing information about the session and players to be registered
	*
	* @return EOS_Success if the register completes successfully
	*         EOS_NoChange if the players to register registered previously
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_Sessions_OutOfSync if the session is out of sync and will be updated on the next connection with the backend
	*         EOS_NotFound if a session to register players does not exist
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Sessions|Async", DisplayName = "EOS_Sessions_RegisterPlayers"))
	static UEOSCoreSessionsRegisterPlayers* EOSSessionsRegisterPlayersAsync(UObject* WorldContextObject, FEOSSessionsRegisterPlayersOptions Options);
protected:
	FEOSSessionsRegisterPlayersOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSSessionsRegisterPlayersCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreSessionsUnregisterPlayers
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreSessionsUnregisterPlayers : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnSessionUnregisterPlayersCallbackDelegate OnCallback;
private:
	FOnSessionUnregisterPlayersCallback m_Callback;
public:
	UEOSCoreSessionsUnregisterPlayers() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCoreSessionsUnregisterPlayers() { m_Callback.Unbind(); }
public:
	/**
	* Unregister a group of players with the session, freeing up space for others to join
	*
	* @param Options Structure containing information about the session and players to be unregistered
	*
	* @return EOS_Success if the unregister completes successfully
	*         EOS_NoChange if the players to unregister were not found
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_Sessions_OutOfSync if the session is out of sync and will be updated on the next connection with the backend
	*         EOS_NotFound if a session to be unregister players does not exist
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Sessions|Async", DisplayName = "EOS_Sessions_UnregisterPlayers"))
	static UEOSCoreSessionsUnregisterPlayers* EOSSessionsUnregisterPlayersAsync(UObject* WorldContextObject, FEOSSessionsUnregisterPlayersOptions Options);
protected:
	FEOSSessionsUnregisterPlayersOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSSessionsUnregisterPlayersCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreSessionsSendInvite
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreSessionsSendInvite : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnSessionSessionSendInviteCallbackDelegate OnCallback;
private:
	FOnSessionSessionSendInviteCallback m_Callback;
public:
	UEOSCoreSessionsSendInvite() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCoreSessionsSendInvite() { m_Callback.Unbind(); }
public:
	/**
	* Send an invite to another player.  User must have created the session or be registered in the session or else the call will fail
	*
	* @param Options Structure containing information about the session and player to invite
	*
	* @return EOS_Success if the send invite completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_NotFound if the session to send the invite from does not exist
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Sessions|Async", DisplayName = "EOS_Sessions_SendInvite"))
	static UEOSCoreSessionsSendInvite* EOSSessionsSendInviteAsync(UObject* WorldContextObject, FEOSSessionsSendInviteOptions Options);
protected:
	FEOSSessionsSendInviteOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSSessionsSendInviteCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreSessionsRejectInvite
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreSessionsRejectInvite : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnSessionRejectInviteCallbackDelegate OnCallback;
private:
	FOnSessionRejectInviteCallback m_Callback;
public:
	UEOSCoreSessionsRejectInvite() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCoreSessionsRejectInvite() { m_Callback.Unbind(); }
public:
	/**
	* Reject an invite from another player.
	*
	* @param Options Structure containing information about the invite to reject
	*
	* @return EOS_Success if the invite rejection completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_NotFound if the invite does not exist
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Sessions|Async", DisplayName = "EOS_Sessions_RejectInvite"))
	static UEOSCoreSessionsRejectInvite* EOSSessionsRejectInviteAsync(UObject* WorldContextObject, FEOSSessionsRejectInviteOptions Options);
protected:
	FEOSSessionsRejectInviteOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSSessionsRejectInviteCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreSessionsQueryInvites
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreSessionsQueryInvites : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnSessionQueryInvitesCallbackDelegate OnCallback;
private:
	FOnSessionQueryInvitesCallback m_Callback;
public:
	UEOSCoreSessionsQueryInvites() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCoreSessionsQueryInvites() { m_Callback.Unbind(); }
public:
	/**
	* Retrieve all existing invites for a single user
	*
	* @param Options Structure containing information about the invites to query
	*
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Sessions|Async", DisplayName = "EOS_Sessions_QueryInvites"))
	static UEOSCoreSessionsQueryInvites* EOSSessionsQueryInvitesAsync(UObject* WorldContextObject, FEOSSessionsQueryInvitesOptions Options);
protected:
	FEOSSessionsQueryInvitesOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSSessionsQueryInvitesCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreSessionsSearchFind
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreSessionsSearchFind : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnSessionFindCallbackDelegate OnCallback;
private:
	FOnSessionFindCallback m_Callback;
public:
	UEOSCoreSessionsSearchFind() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCoreSessionsSearchFind() { m_Callback.Unbind(); }
public:
	/**
	* Find sessions matching the search criteria setup via this session search handle.
	* When the operation completes, this handle will have the search results that can be parsed
	*
	* @param Options Structure containing information about the search criteria to use
	*
	* @return EOS_Success if the find operation completes successfully
	*         EOS_NotFound if searching for an individual session by sessionid or targetuserid returns no results
	*         EOS_InvalidParameters if any of the options are incorrect
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Sessions|Async", DisplayName = "EOS_SessionSearch_Find"))
	static UEOSCoreSessionsSearchFind* EOSSessionSearchFindAsync(UObject* WorldContextObject, FEOSHSessionSearch SearchHandle, FEOSSessionSearchFindOptions Options);
protected:
	FEOSSessionSearchFindOptions m_Options;
	FEOSHSessionSearch m_SearchHandle;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSSessionSearchFindCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};
