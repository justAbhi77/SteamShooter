/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "Lobby/EOSLobbyTypes.h"
#include "Core/EOSCoreAsync.h"
#include "EOSLobbyAsyncActions.generated.h" 

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreLobbyCreateLobby
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreLobbyCreateLobby : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
        FOnLobbyCreateLobbyCallbackDelegate OnCallback;
private:
    FOnLobbyCreateLobbyCallback m_Callback;
public:
    UEOSCoreLobbyCreateLobby() { m_Callback.BindUFunction(this, "HandleCallback"); }
    ~UEOSCoreLobbyCreateLobby() { m_Callback.Unbind(); }
public:
    /**
    * Creates a lobby and adds the user to the lobby membership.  There is no data associated with the lobby at the start and can be added vis EOS_Lobby_UpdateLobbyModification
    *
    * @param Options Required fields for the creation of a lobby such as a user count and its starting advertised state
    *
    * @return EOS_Success if the creation completes successfully
    *         EOS_InvalidParameters if any of the options are incorrect
    *         EOS_LimitExceeded if the number of allowed lobbies is exceeded
    */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Lobby|Async", DisplayName = "EOS_Lobby_CreateLobby"))
		static UEOSCoreLobbyCreateLobby* EOSLobbyCreateLobbyAsync(UObject* WorldContextObject, FEOSLobbyCreateLobbyOptions Options);
protected:
    FEOSLobbyCreateLobbyOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
        void HandleCallback(const FEOSLobbyCreateLobbyCallbackInfo& Data, bool bWasSuccessful)
    {
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
    }
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreLobbyDestroyLobby
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreLobbyDestroyLobby : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
        FOnLobbyDestroyLobbyCallbackDelegate OnCallback;
private:
    FOnLobbyDestroyLobbyCallback m_Callback;
public:
    UEOSCoreLobbyDestroyLobby() { m_Callback.BindUFunction(this, "HandleCallback"); }
    ~UEOSCoreLobbyDestroyLobby() { m_Callback.Unbind(); }
public:
    /**
    * Destroy a lobby given a lobby ID
    *
    * @param Options Structure containing information about the lobby to be destroyed
    *
    * @return EOS_Success if the destroy completes successfully
    *         EOS_InvalidParameters if any of the options are incorrect
    *         EOS_AlreadyPending if the lobby is already marked for destroy
    *         EOS_NotFound if the lobby to be destroyed does not exist
    */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Lobby|Async", DisplayName = "EOS_Lobby_DestroyLobby"))
		static UEOSCoreLobbyDestroyLobby* EOSLobbyDestroyLobbyAsync(UObject* WorldContextObject, FEOSLobbyDestroyLobbyOptions Options);
protected:
    FEOSLobbyDestroyLobbyOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
        void HandleCallback(const FEOSLobbyDestroyLobbyCallbackInfo& Data, bool bWasSuccessful)
    {
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
    }
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreLobbyJoinLobby
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreLobbyJoinLobby : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
        FOnLobbyJoinLobbyCallbackDelegate OnCallback;
private:
    FOnLobbyJoinLobbyCallback m_Callback;
public:
    UEOSCoreLobbyJoinLobby() { m_Callback.BindUFunction(this, "HandleCallback"); }
    ~UEOSCoreLobbyJoinLobby() { m_Callback.Unbind(); }
public:
    /**
    * Join a lobby, creating a local instance under a given lobby ID.  Backend will validate various conditions to make sure it is possible to join the lobby.
    *
    * @param Options Structure containing information about the lobby to be joined
    *
    * @return EOS_Success if the destroy completes successfully
    *         EOS_InvalidParameters if any of the options are incorrect
    */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Lobby|Async", DisplayName = "EOS_Lobby_JoinLobby"))
		static UEOSCoreLobbyJoinLobby* EOSLobbyJoinLobbyAsync(UObject* WorldContextObject, FEOSLobbyJoinLobbyOptions Options);
protected:
    FEOSLobbyJoinLobbyOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
        void HandleCallback(const FEOSLobbyJoinLobbyCallbackInfo& Data, bool bWasSuccessful)
    {
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
    }
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreLobbyLeaveLobby
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreLobbyLeaveLobby : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
        FOnLobbyLeaveLobbyCallbackDelegate OnCallback;
private:
    FOnLobbyLeaveLobbyCallback m_Callback;
public:
    UEOSCoreLobbyLeaveLobby() { m_Callback.BindUFunction(this, "HandleCallback"); }
    ~UEOSCoreLobbyLeaveLobby() { m_Callback.Unbind(); }
public:
    /**
    * Leave a lobby given a lobby ID
    *
    * @param Options Structure containing information about the lobby to be left
    *
    * @return EOS_Success if the leave completes successfully
    *         EOS_InvalidParameters if any of the options are incorrect
    *         EOS_AlreadyPending if the lobby is already marked for leave
    *         EOS_NotFound if a lobby to be left does not exist
    */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Lobby|Async", DisplayName = "EOS_Lobby_LeaveLobby"))
		static UEOSCoreLobbyLeaveLobby* EOSLobbyLeaveLobbyAsync(UObject* WorldContextObject, FEOSLobbyLeaveLobbyOptions Options);
protected:
    FEOSLobbyLeaveLobbyOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
        void HandleCallback(const FEOSLobbyLeaveLobbyCallbackInfo& Data, bool bWasSuccessful)
    {
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
    }
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreLobbyUpdateLobby
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreLobbyUpdateLobby : public UEOSCoreAsyncAction
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintAssignable)
        FOnLobbyUpdateLobbyCallbackDelegate OnCallback;
private:
    FOnLobbyUpdateLobbyCallback m_Callback;
public:
    UEOSCoreLobbyUpdateLobby() { m_Callback.BindUFunction(this, "HandleCallback"); }
    ~UEOSCoreLobbyUpdateLobby() { m_Callback.Unbind(); }
public:
    /**
    * Update a lobby given a lobby modification handle created by EOS_Lobby_UpdateLobbyModification
    *
    * @param Options Structure containing information about the lobby to be updated
    *
    * @return EOS_Success if the update completes successfully
    *         EOS_InvalidParameters if any of the options are incorrect
    *         EOS_Lobby_NotOwner if the lobby modification contains modifications that are only allowed by the owner
    *         EOS_NotFound if the lobby to update does not exist
    */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Lobby|Async", DisplayName = "EOS_Lobby_UpdateLobby"))
        static UEOSCoreLobbyUpdateLobby* EOSLobbyUpdateLobbyAsync(UObject* WorldContextObject, FEOSLobbyUpdateLobbyOptions Options);
protected:
    FEOSLobbyUpdateLobbyOptions m_Options;
public:
	virtual void Activate() override;
private:
    UFUNCTION()
        void HandleCallback(const FEOSLobbyUpdateLobbyCallbackInfo& Data, bool bWasSuccessful)
    {
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
    }
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreLobbyPromoteMember
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreLobbyPromoteMember : public UEOSCoreAsyncAction
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintAssignable)
        FOnLobbyPromoteMemberCallbackDelegate OnCallback;
private:
    FOnLobbyPromoteMemberCallback m_Callback;
public:
    UEOSCoreLobbyPromoteMember() { m_Callback.BindUFunction(this, "HandleCallback"); }
    ~UEOSCoreLobbyPromoteMember() { m_Callback.Unbind(); }
public:
    /**
    * Promote an existing member of the lobby to owner, allowing them to make lobby data modifications
    *
    * @param Options Structure containing information about the lobby and member to be promoted
    *
    * @return EOS_Success if the promote completes successfully
    *         EOS_InvalidParameters if any of the options are incorrect
    *         EOS_Lobby_NotOwner if the calling user is not the owner of the lobby
    *         EOS_NotFound if the lobby of interest does not exist
    */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Lobby|Async", DisplayName = "EOS_Lobby_PromoteMember"))
        static UEOSCoreLobbyPromoteMember* EOSLobbyPromoteMemberAsync(UObject* WorldContextObject, FEOSLobbyPromoteMemberOptions Options);
protected:
    FEOSLobbyPromoteMemberOptions m_Options;
public:
	virtual void Activate() override;
private:
    UFUNCTION()
        void HandleCallback(const FEOSLobbyPromoteMemberCallbackInfo& Data, bool bWasSuccessful)
    {
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
    }
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreLobbyKickMember
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreLobbyKickMember : public UEOSCoreAsyncAction
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintAssignable)
        FOnLobbyKickMemberCallbackDelegate OnCallback;
private:
    FOnLobbyKickMemberCallback m_Callback;
public:
    UEOSCoreLobbyKickMember() { m_Callback.BindUFunction(this, "HandleCallback"); }
    ~UEOSCoreLobbyKickMember() { m_Callback.Unbind(); }
public:
    /**
    * Kick an existing member from the lobby
    *
    * @param Options Structure containing information about the lobby and member to be kicked
    *
    * @return EOS_Success if the kick completes successfully
    *         EOS_InvalidParameters if any of the options are incorrect
    *         EOS_Lobby_NotOwner if the calling user is not the owner of the lobby
    *         EOS_NotFound if a lobby of interest does not exist
    */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Lobby|Async", DisplayName = "EOS_Lobby_KickMember"))
        static UEOSCoreLobbyKickMember* EOSLobbyKickMemberAsync(UObject* WorldContextObject, FEOSLobbyKickMemberOptions Options);
protected:
    FEOSLobbyKickMemberOptions m_Options;
public:
	virtual void Activate() override;
private:
    UFUNCTION()
        void HandleCallback(const FEOSLobbyKickMemberCallbackInfo& Data, bool bWasSuccessful)
    {
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
    }
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreLobbySendInvite
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreLobbySendInvite : public UEOSCoreAsyncAction
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintAssignable)
        FOnLobbySendInviteCallbackDelegate OnCallback;
private:
    FOnLobbySendInviteCallback m_Callback;
public:
    UEOSCoreLobbySendInvite() { m_Callback.BindUFunction(this, "HandleCallback"); }
    ~UEOSCoreLobbySendInvite() { m_Callback.Unbind(); }
public:
    /**
    * Send an invite to another user.  User must be a member of the lobby or else the call will fail
    *
    * @param Options Structure containing information about the lobby and user to invite
    *
    * @return EOS_Success if the send invite completes successfully
    *         EOS_InvalidParameters if any of the options are incorrect
    *         EOS_NotFound if the lobby to send the invite from does not exist
    */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Lobby|Async", DisplayName = "EOS_Lobby_SendInvite"))
        static UEOSCoreLobbySendInvite* EOSLobbySendInviteAsync(UObject* WorldContextObject, FEOSLobbySendInviteOptions Options);
protected:
    FEOSLobbySendInviteOptions m_Options;
public:
	virtual void Activate() override;
private:
    UFUNCTION()
        void HandleCallback(const FEOSLobbySendInviteCallbackInfo& Data, bool bWasSuccessful)
    {
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
    }
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreLobbyRejectInvite
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreLobbyRejectInvite : public UEOSCoreAsyncAction
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintAssignable)
        FOnLobbyRejectInviteCallbacDelegatek OnCallback;
private:
    FOnLobbyRejectInviteCallback m_Callback;
public:
    UEOSCoreLobbyRejectInvite() { m_Callback.BindUFunction(this, "HandleCallback"); }
    ~UEOSCoreLobbyRejectInvite() { m_Callback.Unbind(); }
public:
    /**
    * Reject an invite from another user.
    *
    * @param Options Structure containing information about the invite to reject
    *
    * @return EOS_Success if the invite rejection completes successfully
    *         EOS_InvalidParameters if any of the options are incorrect
    *         EOS_NotFound if the invite does not exist
    */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Lobby|Async", DisplayName = "EOS_Lobby_RejectInvite"))
        static UEOSCoreLobbyRejectInvite* EOSLobbyRejectInviteAsync(UObject* WorldContextObject, FEOSLobbyRejectInviteOptions Options);
protected:
    FEOSLobbyRejectInviteOptions m_Options;
public:
	virtual void Activate() override;
private:
    UFUNCTION()
        void HandleCallback(const FEOSLobbyRejectInviteCallbackInfo& Data, bool bWasSuccessful)
    {
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
    }
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreLobbyQueryInvites
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreLobbyQueryInvites : public UEOSCoreAsyncAction
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintAssignable)
        FOnLobbyQueryInvitesCallbackDelegate OnCallback;
private:
    FOnLobbyQueryInvitesCallback m_Callback;
public:
    UEOSCoreLobbyQueryInvites() { m_Callback.BindUFunction(this, "HandleCallback"); }
    ~UEOSCoreLobbyQueryInvites() { m_Callback.Unbind(); }
public:
    /**
    * Retrieve all existing invites for a single user
    *
    * @param Options Structure containing information about the invites to query
    *
    */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Lobby|Async", DisplayName = "EOS_Lobby_QueryInvites"))
        static UEOSCoreLobbyQueryInvites* EOSLobbyQueryInvitesAsync(UObject* WorldContextObject, FEOSLobbyQueryInvitesOptions Options);
protected:
    FEOSLobbyQueryInvitesOptions m_Options;
public:
	virtual void Activate() override;
private:
    UFUNCTION()
        void HandleCallback(const FEOSLobbyQueryInvitesCallbackInfo& Data, bool bWasSuccessful)
    {
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
    }
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreLobbySearchFind
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreLobbySearchFind : public UEOSCoreAsyncAction
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintAssignable)
        FOnLobbyFindCallbackDelegate OnCallback;
private:
    FOnLobbyFindCallback m_Callback;
public:
    UEOSCoreLobbySearchFind() { m_Callback.BindUFunction(this, "HandleCallback"); }
    ~UEOSCoreLobbySearchFind() { m_Callback.Unbind(); }
public:
    /**
    * Find lobbies matching the search criteria setup via this lobby search handle.
    * When the operation completes, this handle will have the search results that can be parsed
    *
    * @param Options Structure containing information about the search criteria to use
    *
    * @return EOS_Success if the find operation completes successfully
    *         EOS_NotFound if searching for an individual lobby by lobby ID or target user ID returns no results
    *         EOS_InvalidParameters if any of the options are incorrect
    */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Lobby|Async", DisplayName = "EOS_LobbySearch_Find"))
        static UEOSCoreLobbySearchFind* EOSLobbySearchFindAsync(UObject* WorldContextObject, const FEOSHLobbySearch& Handle, FEOSLobbySearchFindOptions Options);
protected:
    FEOSLobbySearchFindOptions m_Options;
    FEOSHLobbySearch m_Handle;
public:
	virtual void Activate() override;
private:
    UFUNCTION()
        void HandleCallback(const FEOSLobbySearchFindCallbackInfo& Data, bool bWasSuccessful)
    {
        OnCallback.Broadcast(Data);
        m_Callback.Unbind();
        SetReadyToDestroy();
    }
};
