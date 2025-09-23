/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "Lobby/EOSLobby.h"
#include "Core/EOSCoreLibrary.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLogging.h"

void UCoreLobby::Deinitialize()
{
	LogVerbose("");

	for (auto Element: m_LobbyUpdateReceivedCallbacks)
	{
		EOS_Lobby_RemoveNotifyLobbyUpdateReceived(GetLobbyHandle(this), Element.Key);
	}

	for (auto Element: m_LobbyMemberUpdateReceivedCallbacks)
	{
		EOS_Lobby_RemoveNotifyLobbyMemberUpdateReceived(GetLobbyHandle(this), Element.Key);
	}

	for (auto Element: m_LobbyMemberStatusReceivedCallbacks)
	{
		EOS_Lobby_RemoveNotifyLobbyMemberStatusReceived(GetLobbyHandle(this), Element.Key);
	}

	for (auto Element: m_LobbyInviteReceivedCallbacks)
	{
		EOS_Lobby_RemoveNotifyLobbyInviteReceived(GetLobbyHandle(this), Element.Key);
	}

	for (auto Element: m_LobbyInviteAcceptedCallbacks)
	{
		EOS_Lobby_RemoveNotifyLobbyInviteAccepted(GetLobbyHandle(this), Element.Key);
	}

	for (auto Element: m_LobbyJoinLobbyAccepted)
	{
		EOS_Lobby_RemoveNotifyJoinLobbyAccepted(GetLobbyHandle(this), Element.Key);
	}

	for (auto Element: m_LobbyRTCRoomConnectionChanged)
	{
		EOS_Lobby_RemoveNotifyRTCRoomConnectionChanged(GetLobbyHandle(this), Element.Key);
	}

	for (auto Element: m_LobbyLeaveLobbyRequestedCallback)
	{
		EOS_Lobby_RemoveNotifyLeaveLobbyRequested(GetLobbyHandle(this), Element.Key);
	}

	m_LobbyUpdateReceivedCallbacks.Empty();
	m_LobbyMemberUpdateReceivedCallbacks.Empty();
	m_LobbyMemberStatusReceivedCallbacks.Empty();
	m_LobbyInviteReceivedCallbacks.Empty();
	m_LobbyInviteAcceptedCallbacks.Empty();
	m_LobbyJoinLobbyAccepted.Empty();
	m_LobbyRTCRoomConnectionChanged.Empty();
	m_LobbyLeaveLobbyRequestedCallback.Empty();
}

UCoreLobby* UCoreLobby::GetLobby(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (GetLobbyHandle(WorldContextObject))
		{
			if (UWorld* World = WorldContextObject->GetWorld())
			{
				if (UGameInstance* GameInstance = World->GetGameInstance())
				{
					return GameInstance->GetSubsystem<UCoreLobby>();
				}
			}	
		}
	}
	return nullptr;
}

void UCoreLobby::EOSLobbyCreateLobby(UObject* WorldContextObject, FEOSLobbyCreateLobbyOptions Options, const FOnLobbyCreateLobbyCallback& Callback)
{
	LogVerbose("");

	if (GetLobbyHandle(WorldContextObject))
	{
		FCreateLobbyOptions LobbyOptions(Options.BucketId, Options.LobbyId, Options.AllowedPlatformIds.Num());
		EOS_Lobby_LocalRTCOptions RtcOptions = Options.LocalRTCOptions;
		LobbyOptions.LocalUserId = Options.LocalUserId;
		LobbyOptions.MaxLobbyMembers = static_cast<uint32_t>(Options.MaxLobbyMembers);
		LobbyOptions.PermissionLevel = static_cast<EOS_ELobbyPermissionLevel>(Options.PermissionLevel);
		LobbyOptions.bPresenceEnabled = Options.bPresenceEnabled;
		LobbyOptions.bAllowInvites = Options.bAllowInvites;
		LobbyOptions.bDisableHostMigration = Options.bDisableHostMigration;
		LobbyOptions.bEnableRTCRoom = Options.bEnableRTCRoom;
		LobbyOptions.LocalRTCOptions = &RtcOptions;
		LobbyOptions.bEnableJoinById = Options.bEnableJoinById;
		LobbyOptions.bRejoinAfterKickRequiresInvite = Options.bRejoinAfterKickRequiresInvite;
		
		for (int32 i=0; i<Options.AllowedPlatformIds.Num(); i++)
		{
			LobbyOptions.PointerArray.Add(Options.AllowedPlatformIds[i]);
		}
		LobbyOptions.bCrossplayOptOut = Options.bCrossplayOptOut;

		FCreateLobbyCallback* CallbackObj = new FCreateLobbyCallback(Callback);
		EOS_Lobby_CreateLobby(GetLobbyHandle(WorldContextObject), &LobbyOptions, CallbackObj, Internal_OnCreateLobbyCallback);
	}
}

void UCoreLobby::EOSLobbyDestroyLobby(UObject* WorldContextObject, FEOSLobbyDestroyLobbyOptions Options, const FOnLobbyDestroyLobbyCallback& Callback)
{
	LogVerbose("");

	if (GetLobbyHandle(WorldContextObject))
	{
		FDestroyLobbyOptions LobbyOptions(Options.LobbyId);
		LobbyOptions.LocalUserId = Options.LocalUserId;

		FDestroyLobbyCallback* CallbackObj = new FDestroyLobbyCallback(Callback);
		EOS_Lobby_DestroyLobby(GetLobbyHandle(WorldContextObject), &LobbyOptions, CallbackObj, Internal_OnDestroyLobbyCallback);
	}
}

void UCoreLobby::EOSLobbyJoinLobby(UObject* WorldContextObject, FEOSLobbyJoinLobbyOptions Options, const FOnLobbyJoinLobbyCallback& Callback)
{
	LogVerbose("");

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_JoinLobbyOptions LobbyOptions = {};
		EOS_Lobby_LocalRTCOptions RtcOptions = Options.LocalRTCOptions;
		LobbyOptions.ApiVersion = EOS_LOBBY_JOINLOBBY_API_LATEST;
		LobbyOptions.LobbyDetailsHandle = Options.LobbyDetailsHandle;
		LobbyOptions.LocalUserId = Options.LocalUserId;
		LobbyOptions.bPresenceEnabled = Options.bPresenceEnabled;
		LobbyOptions.LocalRTCOptions = &RtcOptions;
		LobbyOptions.bCrossplayOptOut = Options.bCrossplayOptOut;

		FJoinLobbyCallback* CallbackObj = new FJoinLobbyCallback(Callback);
		EOS_Lobby_JoinLobby(GetLobbyHandle(WorldContextObject), &LobbyOptions, CallbackObj, Internal_OnJoinLobbyCallback);
	}
}

void UCoreLobby::EOSLobbyJoinLobbyById(UObject* WorldContextObject, FEOSLobbyJoinLobbyByIdOptions Options, const FOnJoinLobbyByIdCallback& Callback)
{
	LogVerbose("");

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_JoinLobbyByIdOptions LobbyOptions = {};

		const FTCHARToUTF8 ConvertedLobbyId(*Options.LobbyId);
		
		EOS_Lobby_LocalRTCOptions RtcOptions = Options.LocalRTCOptions;
		LobbyOptions.ApiVersion = EOS_LOBBY_JOINLOBBYBYID_API_LATEST;
		LobbyOptions.LobbyId = ConvertedLobbyId.Get();
		LobbyOptions.LocalUserId = Options.LocalUserId;
		LobbyOptions.bPresenceEnabled = Options.bPresenceEnabled;
		LobbyOptions.LocalRTCOptions = &RtcOptions;
		LobbyOptions.bCrossplayOptOut = Options.bCrossplayOptOut;

		FJoinLobbyByIdCallback* CallbackObj = new FJoinLobbyByIdCallback(Callback);
		EOS_Lobby_JoinLobbyById(GetLobbyHandle(WorldContextObject), &LobbyOptions, CallbackObj, Internal_OnJoinLobbyByIdCallback);
	}
}

void UCoreLobby::EOSLobbyHardMuteMember(UObject* WorldContextObject, FEOSLobbyHardMuteMemberOptions Options, const FOnHardMuteMemberCallback& Callback)
{
	LogVerbose("");

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_HardMuteMemberOptions LobbyOptions = {};
		
		const FTCHARToUTF8 ConvertedLobbyId(*Options.LobbyId);
		LobbyOptions.ApiVersion = EOS_LOBBY_JOINLOBBYBYID_API_LATEST;
		LobbyOptions.LobbyId = ConvertedLobbyId.Get();
		LobbyOptions.LocalUserId = Options.LocalUserId;
		LobbyOptions.TargetUserId = Options.TargetUserId;
		LobbyOptions.bHardMute = Options.bHardMute;

		FHardMuteMemberCallback* CallbackObj = new FHardMuteMemberCallback(Callback);
		EOS_Lobby_HardMuteMember(GetLobbyHandle(WorldContextObject), &LobbyOptions, CallbackObj, Internal_OnJoinHardMuteCallback);
	}
}

FEOSNotificationId UCoreLobby::EOSLobbyAddNotifyLobbyInviteRejected(UObject* WorldContextObject, FEOSLobbyAddNotifyLobbyInviteRejectedOptions Options, const FOnLobbyInviteRejectedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_AddNotifyLobbyInviteRejectedOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = EOS_LOBBY_ADDNOTIFYLOBBYINVITEREJECTED_API_LATEST;

		FLobbyInviteRejectedCallback* CallbackObj = new FLobbyInviteRejectedCallback(Callback);

		Id = EOS_Lobby_AddNotifyLobbyInviteRejected(GetLobbyHandle(WorldContextObject), &LobbyOptions, CallbackObj, ([](const EOS_Lobby_LobbyInviteRejectedCallbackInfo* Data)
		{
			FLobbyInviteRejectedCallback* CallbackObj = static_cast<FLobbyInviteRejectedCallback*>(Data->ClientData);
			CallbackObj->m_Callback.ExecuteIfBound(*Data);
		}));
		GetLobby(WorldContextObject)->m_LobbyInviteRejectedCallback.Add(Id, CallbackObj);
	}

	return Id;
}

void UCoreLobby::EOSLobbyRemoveNotifyLobbyInviteRejected(UObject* WorldContextObject, FEOSNotificationId Id)
{
	LogVerbose("");

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_RemoveNotifyLobbyInviteRejected(GetLobbyHandle(WorldContextObject), Id);
		GetLobby(WorldContextObject)->m_LobbyInviteRejectedCallback.Remove(Id);
	}
}

FEOSNotificationId UCoreLobby::EOSLobbyAddNotifySendLobbyNativeInviteRequested(UObject* WorldContextObject, FEOSLobbyAddNotifySendLobbyNativeInviteRequestedOptions Options, const FOnSendLobbyNativeInviteRequestedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_AddNotifySendLobbyNativeInviteRequestedOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = EOS_LOBBY_ADDNOTIFYSENDLOBBYNATIVEINVITEREQUESTED_API_LATEST;

		FLobbyNativeInviteRequestedCallback* CallbackObj = new FLobbyNativeInviteRequestedCallback(Callback);

		Id = EOS_Lobby_AddNotifySendLobbyNativeInviteRequested(GetLobbyHandle(WorldContextObject), &LobbyOptions, CallbackObj, ([](const EOS_Lobby_SendLobbyNativeInviteRequestedCallbackInfo* Data)
		{
			const FLobbyNativeInviteRequestedCallback* CallbackObj = static_cast<FLobbyNativeInviteRequestedCallback*>(Data->ClientData);
			CallbackObj->m_Callback.ExecuteIfBound(*Data);
		}));
		GetLobby(WorldContextObject)->m_LobbyNativeInviteRequestedCallback.Add(Id, CallbackObj);
	}

	return Id;
}

void UCoreLobby::EOSLobbyRemoveNotifySendLobbyNativeInviteRequested(UObject* WorldContextObject, FEOSNotificationId Id)
{
	LogVerbose("");

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_RemoveNotifySendLobbyNativeInviteRequested(GetLobbyHandle(WorldContextObject), Id);
		GetLobby(WorldContextObject)->m_LobbyNativeInviteRequestedCallback.Remove(Id);
	}
}

void UCoreLobby::EOSLobbyLeaveLobby(UObject* WorldContextObject, FEOSLobbyLeaveLobbyOptions Options, const FOnLobbyLeaveLobbyCallback& Callback)
{
	LogVerbose("");

	if (GetLobbyHandle(WorldContextObject))
	{
		FLeaveLobbyOptions LobbyOptions(Options.LobbyId);
		LobbyOptions.LocalUserId = Options.LocalUserId;

		FLeaveLobbyCallback* CallbackObj = new FLeaveLobbyCallback(Callback);
		EOS_Lobby_LeaveLobby(GetLobbyHandle(WorldContextObject), &LobbyOptions, CallbackObj, Internal_OnLeaveLobbyCallback);
	}
}

struct FUpdateLobbyModificationOptions : public EOS_Lobby_UpdateLobbyModificationOptions
{
	FUpdateLobbyModificationOptions(FString InLobbyId)
		: EOS_Lobby_UpdateLobbyModificationOptions()
	{
		ApiVersion = EOS_LOBBY_UPDATELOBBYMODIFICATION_API_LATEST;
		if (!InLobbyId.IsEmpty())
		{
			LobbyId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(LobbyId), TCHAR_TO_UTF8(*InLobbyId), 256);
		}
	}

	~FUpdateLobbyModificationOptions()
	{
		delete[] LobbyId;
	}
};

EOSResult UCoreLobby::EOSLobbyUpdateLobbyModification(UObject* WorldContextObject, FEOSLobbyUpdateLobbyModificationOptions Options, FEOSHLobbyModification& OutLobbyModificationHandle)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		FUpdateLobbyModificationOptions LobbyOptions(Options.LobbyId);
		LobbyOptions.LocalUserId = Options.LocalUserId;
		
		EOS_HLobbyModification LobbyHandle = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Lobby_UpdateLobbyModification(GetLobbyHandle(WorldContextObject), &LobbyOptions, &LobbyHandle));
		if (Result == EOSResult::EOS_Success)
		{
			OutLobbyModificationHandle = LobbyHandle;
		}
	}

	return Result;
}

void UCoreLobby::EOSLobbyUpdateLobby(UObject* WorldContextObject, FEOSLobbyUpdateLobbyOptions Options, const FOnLobbyUpdateLobbyCallback& Callback)
{
	LogVerbose("");

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_UpdateLobbyOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;
		LobbyOptions.LobbyModificationHandle = Options.LobbyModificationHandle;

		FUpdateLobbyCallback* CallbackObj = new FUpdateLobbyCallback(Callback);
		EOS_Lobby_UpdateLobby(GetLobbyHandle(WorldContextObject), &LobbyOptions, CallbackObj, Internal_OnUpdateLobbyCallback);
	}
}

void UCoreLobby::EOSLobbyPromoteMember(UObject* WorldContextObject, FEOSLobbyPromoteMemberOptions Options, const FOnLobbyPromoteMemberCallback& Callback)
{
	LogVerbose("");

	if (GetLobbyHandle(WorldContextObject))
	{
		FPromoteMemberOptions LobbyOptions(Options.LobbyId);
		LobbyOptions.LocalUserId = Options.LocalUserId;
		LobbyOptions.TargetUserId = Options.TargetUserId;

		FLobbyPromoteMemberCallback* CallbackObj = new FLobbyPromoteMemberCallback(Callback);
		EOS_Lobby_PromoteMember(GetLobbyHandle(WorldContextObject), &LobbyOptions, CallbackObj, Internal_OnPromoteMemberCallback);
	}
}

void UCoreLobby::EOSLobbyKickMember(UObject* WorldContextObject, FEOSLobbyKickMemberOptions Options, const FOnLobbyKickMemberCallback& Callback)
{
	LogVerbose("");

	if (GetLobbyHandle(WorldContextObject))
	{
		FKickMemberOptions LobbyOptions(Options.LobbyId);
		LobbyOptions.LocalUserId = Options.LocalUserId;
		LobbyOptions.TargetUserId = Options.TargetUserId;

		FLobbyKickMemberCallback* CallbackObj = new FLobbyKickMemberCallback(Callback);
		EOS_Lobby_KickMember(GetLobbyHandle(WorldContextObject), &LobbyOptions, CallbackObj, Internal_OnKickMemberCallback);
	}
}

FEOSNotificationId UCoreLobby::EOSLobbyAddNotifyLobbyUpdateReceived(UObject* WorldContextObject, const FOnLobbyUpdateReceivedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_AddNotifyLobbyUpdateReceivedOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = EOS_LOBBY_ADDNOTIFYLOBBYUPDATERECEIVED_API_LATEST;

		FLobbyUpdateReceivedCallback* CallbackObj = new FLobbyUpdateReceivedCallback(Callback);

		Id = EOS_Lobby_AddNotifyLobbyUpdateReceived(GetLobbyHandle(WorldContextObject), &LobbyOptions, CallbackObj, ([](const EOS_Lobby_LobbyUpdateReceivedCallbackInfo* data)
		{
			FLobbyUpdateReceivedCallback* CallbackObj = static_cast<FLobbyUpdateReceivedCallback*>(data->ClientData);
			CallbackObj->m_Callback.ExecuteIfBound(*data);
		}));
		GetLobby(WorldContextObject)->m_LobbyUpdateReceivedCallbacks.Add(Id, CallbackObj);
	}

	return Id;
}

void UCoreLobby::EOSLobbyRemoveNotifyLobbyUpdateReceived(UObject* WorldContextObject, FEOSNotificationId Id)
{
	LogVerbose("");

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_RemoveNotifyLobbyUpdateReceived(GetLobbyHandle(WorldContextObject), Id);
		GetLobby(WorldContextObject)->m_LobbyUpdateReceivedCallbacks.Remove(Id);
	}
}

FEOSNotificationId UCoreLobby::EOSLobbyAddNotifyLobbyMemberUpdateReceived(UObject* WorldContextObject, const FOnLobbyMemberUpdateReceivedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_AddNotifyLobbyMemberUpdateReceivedOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = EOS_LOBBY_ADDNOTIFYLOBBYMEMBERUPDATERECEIVED_API_LATEST;

		FLobbyMemberUpdateReceivedCallback* CallbackObj = new FLobbyMemberUpdateReceivedCallback(Callback);

		Id = EOS_Lobby_AddNotifyLobbyMemberUpdateReceived(GetLobbyHandle(WorldContextObject), &LobbyOptions, CallbackObj, ([](const EOS_Lobby_LobbyMemberUpdateReceivedCallbackInfo* data)
		{
			FLobbyMemberUpdateReceivedCallback* CallbackObj = static_cast<FLobbyMemberUpdateReceivedCallback*>(data->ClientData);
			CallbackObj->m_Callback.ExecuteIfBound(*data);
		}));

		GetLobby(WorldContextObject)->m_LobbyMemberUpdateReceivedCallbacks.Add(Id, CallbackObj);
	}

	return Id;
}

void UCoreLobby::EOSLobbyRemoveNotifyLobbyMemberUpdateReceived(UObject* WorldContextObject, FEOSNotificationId Id)
{
	LogVerbose("");

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_RemoveNotifyLobbyMemberUpdateReceived(GetLobbyHandle(WorldContextObject), Id);
		GetLobby(WorldContextObject)->m_LobbyMemberUpdateReceivedCallbacks.Remove(Id);
	}
}

FEOSNotificationId UCoreLobby::EOSLobbyAddNotifyLobbyMemberStatusReceived(UObject* WorldContextObject, const FOnLobbyMemberStatusReceivedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_AddNotifyLobbyMemberStatusReceivedOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = EOS_LOBBY_ADDNOTIFYLOBBYMEMBERSTATUSRECEIVED_API_LATEST;

		FLobbyMemberStatusReceivedCallback* CallbackObj = new FLobbyMemberStatusReceivedCallback(Callback);

		Id = EOS_Lobby_AddNotifyLobbyMemberStatusReceived(GetLobbyHandle(WorldContextObject), &LobbyOptions, CallbackObj, ([](const EOS_Lobby_LobbyMemberStatusReceivedCallbackInfo* data)
		{
			FLobbyMemberStatusReceivedCallback* CallbackObj = static_cast<FLobbyMemberStatusReceivedCallback*>(data->ClientData);
			CallbackObj->m_Callback.ExecuteIfBound(*data);
		}));

		GetLobby(WorldContextObject)->m_LobbyMemberStatusReceivedCallbacks.Add(Id, CallbackObj);
	}

	return Id;
}

void UCoreLobby::EOSLobbyRemoveNotifyLobbyMemberStatusReceived(UObject* WorldContextObject, FEOSNotificationId Id)
{
	LogVerbose("");

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_RemoveNotifyLobbyMemberStatusReceived(GetLobbyHandle(WorldContextObject), Id);
		GetLobby(WorldContextObject)->m_LobbyMemberStatusReceivedCallbacks.Remove(Id);
	}
}

void UCoreLobby::EOSLobbySendInvite(UObject* WorldContextObject, FEOSLobbySendInviteOptions Options, const FOnLobbySendInviteCallback& Callback)
{
	LogVerbose("");

	if (GetLobbyHandle(WorldContextObject))
	{
		FSendInviteOptions LobbyOptions(Options.LobbyId);
		LobbyOptions.LocalUserId = Options.LocalUserId;
		LobbyOptions.TargetUserId = Options.TargetUserId;
		
		FLobbySendInviteCallback* CallbackObj = new FLobbySendInviteCallback(Callback);
		EOS_Lobby_SendInvite(GetLobbyHandle(WorldContextObject), &LobbyOptions, CallbackObj, Internal_OnSendInviteCallback);
	}
}

void UCoreLobby::EOSLobbyRejectInvite(UObject* WorldContextObject, FEOSLobbyRejectInviteOptions Options, const FOnLobbyRejectInviteCallback& Callback)
{
	LogVerbose("");

	if (GetLobbyHandle(WorldContextObject))
	{
		const FTCHARToUTF8 InviteId(*Options.InviteId);
		
		EOS_Lobby_RejectInviteOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;
		LobbyOptions.InviteId = InviteId.Get();
		LobbyOptions.LocalUserId = Options.LocalUserId;

		FLobbyRejectInviteCallback* CallbackObj = new FLobbyRejectInviteCallback(Callback);
		EOS_Lobby_RejectInvite(GetLobbyHandle(WorldContextObject), &LobbyOptions, CallbackObj, Internal_OnRejectInviteCallback);
	}
}

void UCoreLobby::EOSLobbyQueryInvites(UObject* WorldContextObject, FEOSLobbyQueryInvitesOptions Options, const FOnLobbyQueryInvitesCallback& Callback)
{
	LogVerbose("");

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_QueryInvitesOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;
		LobbyOptions.LocalUserId = Options.LocalUserId;

		FLobbyQueryInvitesCallback* CallbackObj = new FLobbyQueryInvitesCallback(Callback);
		EOS_Lobby_QueryInvites(GetLobbyHandle(WorldContextObject), &LobbyOptions, CallbackObj, Internal_OnQueryInvitesCallback);
	}
}

int32 UCoreLobby::EOSLobbyGetInviteCount(UObject* WorldContextObject, FEOSLobbyGetInviteCountOptions Options)
{
	LogVerbose("");

	int32 Result = 0;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_GetInviteCountOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;
		LobbyOptions.LocalUserId = Options.LocalUserId;

		Result = EOS_Lobby_GetInviteCount(GetLobbyHandle(WorldContextObject), &LobbyOptions);
	}

	return Result;
}

EOSResult UCoreLobby::EOSLobbyGetInviteIdByIndex(UObject* WorldContextObject, FEOSLobbyGetInviteIdByIndexOptions Options, FString& OutInviteId)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_GetInviteIdByIndexOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;
		LobbyOptions.LocalUserId = Options.LocalUserId;
		LobbyOptions.Index = static_cast<uint32_t>(Options.Index);

		char Buffer[512];
		int32 Size = 512;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Lobby_GetInviteIdByIndex(GetLobbyHandle(WorldContextObject), &LobbyOptions, Buffer, &Size));

		if (Result == EOSResult::EOS_Success)
		{
			OutInviteId = FString(UTF8_TO_TCHAR(Buffer));
		}
	}

	return Result;
}

EOSResult UCoreLobby::EOSLobbyCreateLobbySearch(UObject* WorldContextObject, FEOSLobbyCreateLobbySearchOptions Options, FEOSHLobbySearch& OutLobbySearchHandle)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_CreateLobbySearchOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;
		LobbyOptions.MaxResults = static_cast<uint32_t>(Options.MaxResults);

		EOS_HLobbySearch LobbyHandle = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Lobby_CreateLobbySearch(GetLobbyHandle(WorldContextObject), &LobbyOptions, &LobbyHandle));

		if (Result == EOSResult::EOS_Success)
		{
			OutLobbySearchHandle = LobbyHandle;
		}
	}

	return Result;
}

FEOSNotificationId UCoreLobby::EOSLobbyAddNotifyLobbyInviteReceived(UObject* WorldContextObject, const FOnLobbyInviteReceivedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_AddNotifyLobbyInviteReceivedOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = EOS_LOBBY_ADDNOTIFYLOBBYINVITERECEIVED_API_LATEST;

		FLobbyInviteReceivedCallback* CallbackObj = new FLobbyInviteReceivedCallback(Callback);
		CallbackObj->m_Callback = Callback;

		Id = EOS_Lobby_AddNotifyLobbyInviteReceived(GetLobbyHandle(WorldContextObject), &LobbyOptions, CallbackObj, ([](const EOS_Lobby_LobbyInviteReceivedCallbackInfo* Data)
		{
			FLobbyInviteReceivedCallback* CallbackObj = static_cast<FLobbyInviteReceivedCallback*>(Data->ClientData);
			CallbackObj->m_Callback.ExecuteIfBound(*Data);
		}));

		GetLobby(WorldContextObject)->m_LobbyInviteReceivedCallbacks.Add(Id, CallbackObj);
	}

	return Id;
}

void UCoreLobby::EOSLobbyRemoveNotifyLobbyInviteReceived(UObject* WorldContextObject, FEOSNotificationId Id)
{
	LogVerbose("");

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_RemoveNotifyLobbyInviteReceived(GetLobbyHandle(WorldContextObject), Id);
		GetLobby(WorldContextObject)->m_LobbyInviteReceivedCallbacks.Remove(Id);
	}
}

FEOSNotificationId UCoreLobby::EOSLobbyAddNotifyLobbyInviteAccepted(UObject* WorldContextObject, FEOSLobbyAddNotifyLobbyInviteAcceptedOptions Options, const FOnLobbyInviteAcceptedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_AddNotifyLobbyInviteAcceptedOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = EOS_LOBBY_ADDNOTIFYLOBBYINVITEACCEPTED_API_LATEST;

		FLobbyInviteAcceptedCallback* CallbackObj = new FLobbyInviteAcceptedCallback(Callback);

		Id = EOS_Lobby_AddNotifyLobbyInviteAccepted(GetLobbyHandle(WorldContextObject), &LobbyOptions, CallbackObj, ([](const EOS_Lobby_LobbyInviteAcceptedCallbackInfo* Data)
		{
			FLobbyInviteAcceptedCallback* CallbackObj = static_cast<FLobbyInviteAcceptedCallback*>(Data->ClientData);
			CallbackObj->m_Callback.ExecuteIfBound(*Data);
		}));

		GetLobby(WorldContextObject)->m_LobbyInviteAcceptedCallbacks.Add(Id, CallbackObj);
	}

	return Id;
}

void UCoreLobby::EOSLobbyRemoveNotifyLobbyInviteAccepted(UObject* WorldContextObject, FEOSNotificationId Id)
{
	LogVerbose("");

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_RemoveNotifyLobbyInviteAccepted(GetLobbyHandle(WorldContextObject), Id);
		GetLobby(WorldContextObject)->m_LobbyInviteAcceptedCallbacks.Remove(Id);
	}
}

FEOSNotificationId UCoreLobby::EOSLobbyAddNotifyJoinLobbyAccepted(UObject* WorldContextObject, FEOSLobbyAddNotifyJoinLobbyAcceptedOptions Options, const FOnJoinLobbyAcceptedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_AddNotifyJoinLobbyAcceptedOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;

		FJoinLobbyAcceptedCallback* CallbackObj = new FJoinLobbyAcceptedCallback(Callback);

		Id = EOS_Lobby_AddNotifyJoinLobbyAccepted(GetLobbyHandle(WorldContextObject), &LobbyOptions, CallbackObj, ([](const EOS_Lobby_JoinLobbyAcceptedCallbackInfo* Data)
		{
			FJoinLobbyAcceptedCallback* CallbackObj = static_cast<FJoinLobbyAcceptedCallback*>(Data->ClientData);
			CallbackObj->m_Callback.ExecuteIfBound(*Data);
		}));

		GetLobby(WorldContextObject)->m_LobbyJoinLobbyAccepted.Add(Id, CallbackObj);
	}

	return Id;
}

void UCoreLobby::EOSLobbyRemoveNotifyJoinLobbyAccepted(UObject* WorldContextObject, FEOSNotificationId Id)
{
	LogVerbose("");

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_RemoveNotifyJoinLobbyAccepted(GetLobbyHandle(WorldContextObject), Id);
		GetLobby(WorldContextObject)->m_LobbyJoinLobbyAccepted.Remove(Id);
	}
}

EOSResult UCoreLobby::EOSLobbyCopyLobbyDetailsHandleByInviteId(UObject* WorldContextObject, FEOSLobbyCopyLobbyDetailsHandleByInviteIdOptions Options, FEOSHLobbyDetails& OutLobbyDetailsHandle)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		const FTCHARToUTF8 InviteId(*Options.InviteId);
		
		EOS_Lobby_CopyLobbyDetailsHandleByInviteIdOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;
		LobbyOptions.InviteId = InviteId.Get();

		EOS_HLobbyDetails LobbyHandle = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Lobby_CopyLobbyDetailsHandleByInviteId(GetLobbyHandle(WorldContextObject), &LobbyOptions, &LobbyHandle));

		if (Result == EOSResult::EOS_Success)
		{
			OutLobbyDetailsHandle = LobbyHandle;
		}
	}

	return Result;
}

EOSResult UCoreLobby::EOSLobbyCopyLobbyDetailsHandleByUiEventId(UObject* WorldContextObject, FEOSLobbyCopyLobbyDetailsHandleByUiEventIdOptions Options, FEOSHLobbyDetails& OutLobbyDetailsHandle)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_CopyLobbyDetailsHandleByUiEventIdOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = EOS_LOBBY_COPYLOBBYDETAILSHANDLEBYUIEVENTID_API_LATEST;
		LobbyOptions.UiEventId = Options.UiEventId;

		EOS_HLobbyDetails LobbyHandle = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Lobby_CopyLobbyDetailsHandleByUiEventId(GetLobbyHandle(WorldContextObject), &LobbyOptions, &LobbyHandle));

		if (Result == EOSResult::EOS_Success)
		{
			OutLobbyDetailsHandle = LobbyHandle;
		}
	}

	return Result;
}

EOSResult UCoreLobby::EOSLobbyCopyLobbyDetailsHandle(UObject* WorldContextObject, FEOSLobbyCopyLobbyDetailsHandleOptions Options, FEOSHLobbyDetails& OutLobbyDetailsHandle)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		FCopyLobbyDetailsHandleOptions LobbyOptions(Options.LobbyId);
		LobbyOptions.LocalUserId = Options.LocalUserId;

		EOS_HLobbyDetails LobbyHandle = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Lobby_CopyLobbyDetailsHandle(GetLobbyHandle(WorldContextObject), &LobbyOptions, &LobbyHandle));

		if (Result == EOSResult::EOS_Success)
		{
			OutLobbyDetailsHandle = LobbyHandle;
		}
	}

	return Result;
}

EOSResult UCoreLobby::EOSLobbyGetRTCRoomName(UObject* WorldContextObject, FEOSLobbyGetRTCRoomNameOptions Options, FString& OutBuffer, int32 InOutBufferLength)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_UnexpectedError;

	OutBuffer.Empty();

	if (GetLobbyHandle(WorldContextObject))
	{
		FGetRTCRoomNameOptions LobbyOptions(Options.LobbyId);
		LobbyOptions.LocalUserId = Options.LocalUserId;

		TArray<char> Buffer;
		Buffer.SetNum(InOutBufferLength);

		uint32_t BufferLength = InOutBufferLength;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Lobby_GetRTCRoomName(GetLobbyHandle(WorldContextObject), &LobbyOptions, Buffer.GetData(), &BufferLength));

		if (Result == EOSResult::EOS_Success)
		{
			OutBuffer = UTF8_TO_TCHAR(Buffer.GetData());
		}
	}

	return Result;
}

EOSResult UCoreLobby::EOSLobbyIsRTCRoomConnected(UObject* WorldContextObject, FEOSLobbyIsRTCRoomConnectedOptions Options, bool& bOutIsConnected)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_UnexpectedError;

	bOutIsConnected = false;

	if (GetLobbyHandle(WorldContextObject))
	{
		FIsRTCRoomConnectedOptions LobbyOptions(Options.LobbyId);
		LobbyOptions.LocalUserId = Options.LocalUserId;

		EOS_Bool bIsConnected = false;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Lobby_IsRTCRoomConnected(GetLobbyHandle(WorldContextObject), &LobbyOptions, &bIsConnected));

		if (Result == EOSResult::EOS_Success)
		{
			bOutIsConnected = (bIsConnected > 0);
		}
	}

	return Result;
}

FEOSNotificationId UCoreLobby::EOSLobbyAddNotifyRTCRoomConnectionChanged(UObject* WorldContextObject, FEOSLobbyAddNotifyRTCRoomConnectionChangedOptions Options, const FOnRTCRoomConnectionChangedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetLobbyHandle(WorldContextObject))
	{
		FAddNotifyRTCRoomConnectionChangedOptions LobbyOptions(Options.LobbyId_DEPRECATED);
		LobbyOptions.LocalUserId_DEPRECATED = Options.LocalUserId_DEPRECATED;

		FLobbyRTCRoomConnectionChanged* CallbackObj = new FLobbyRTCRoomConnectionChanged(Callback);

		Id = EOS_Lobby_AddNotifyRTCRoomConnectionChanged(GetLobbyHandle(WorldContextObject), &LobbyOptions, CallbackObj, ([](const EOS_Lobby_RTCRoomConnectionChangedCallbackInfo* data)
		{
			FLobbyRTCRoomConnectionChanged* CallbackObj = static_cast<FLobbyRTCRoomConnectionChanged*>(data->ClientData);
			CallbackObj->m_Callback.ExecuteIfBound(*data);
		}));
		GetLobby(WorldContextObject)->m_LobbyRTCRoomConnectionChanged.Add(Id, CallbackObj);
	}

	return Id;
}

void UCoreLobby::EOSLobbyRemoveNotifyRTCRoomConnectionChanged(UObject* WorldContextObject, FEOSNotificationId Id)
{
	LogVerbose("");

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_RemoveNotifyRTCRoomConnectionChanged(GetLobbyHandle(WorldContextObject), Id);
		GetLobby(WorldContextObject)->m_LobbyRTCRoomConnectionChanged.Remove(Id);
	}
}

EOSResult UCoreLobby::EOSLobbyGetConnectString(UObject* WorldContextObject, FEOSLobbyGetConnectStringOptions Options, FString& OutBuffer)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_GetConnectStringOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = EOS_LOBBY_GETCONNECTSTRING_API_LATEST;
		LobbyOptions.LocalUserId = Options.LocalUserId;
		const FTCHARToUTF8 LobbyId(*Options.LobbyId);
		LobbyOptions.LobbyId = LobbyId.Get();

		char Buffer[EOS_LOBBY_GETCONNECTSTRING_BUFFER_SIZE];
		uint32_t Size = EOS_LOBBY_GETCONNECTSTRING_BUFFER_SIZE;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Lobby_GetConnectString(GetLobbyHandle(WorldContextObject), &LobbyOptions, Buffer, &Size));

		if (Result == EOSResult::EOS_Success)
		{
			OutBuffer = FString(UTF8_TO_TCHAR(Buffer));
		}
	}

	return Result;
}

EOSResult UCoreLobby::EOSLobbyParseConnectString(UObject* WorldContextObject, FEOSLobbyParseConnectStringOptions Options, FString& OutBuffer)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_ParseConnectStringOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = EOS_LOBBY_PARSECONNECTSTRING_API_LATEST;
		const FTCHARToUTF8 ConnectString(*Options.ConnectString);
		LobbyOptions.ConnectString = ConnectString.Get();

		char Buffer[EOS_LOBBY_PARSECONNECTSTRING_BUFFER_SIZE];
		uint32_t Size = EOS_LOBBY_PARSECONNECTSTRING_BUFFER_SIZE;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Lobby_ParseConnectString(GetLobbyHandle(WorldContextObject), &LobbyOptions, Buffer, &Size));

		if (Result == EOSResult::EOS_Success)
		{
			OutBuffer = FString(UTF8_TO_TCHAR(Buffer));
		}
	}

	return Result;
}

FEOSNotificationId UCoreLobby::EOSLobbyAddNotifyLeaveLobbyRequested(UObject* WorldContextObject, const FOnLeaveLobbyRequestedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_AddNotifyLeaveLobbyRequestedOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = EOS_LOBBY_ADDNOTIFYLEAVELOBBYREQUESTED_API_LATEST;

		FLeaveLobbyRequestedCallback* CallbackObj = new FLeaveLobbyRequestedCallback(Callback);

		Id = EOS_Lobby_AddNotifyLeaveLobbyRequested(GetLobbyHandle(WorldContextObject), &LobbyOptions, CallbackObj, ([](const EOS_Lobby_LeaveLobbyRequestedCallbackInfo* Data)
		{
			FLeaveLobbyRequestedCallback* CallbackObj = static_cast<FLeaveLobbyRequestedCallback*>(Data->ClientData);
			CallbackObj->m_Callback.ExecuteIfBound(*Data);
		}));
		GetLobby(WorldContextObject)->m_LobbyLeaveLobbyRequestedCallback.Add(Id, CallbackObj);
	}

	return Id;
}

void UCoreLobby::EOSLobbyRemoveNotifyLeaveLobbyRequested(UObject* WorldContextObject, FEOSNotificationId InId)
{
	LogVerbose("");

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_Lobby_RemoveNotifyLeaveLobbyRequested(GetLobbyHandle(WorldContextObject), InId);
		GetLobby(WorldContextObject)->m_LobbyLeaveLobbyRequestedCallback.Remove(InId);
	}
}

EOSResult UCoreLobby::EOSLobbyModificationSetBucketId(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FEOSLobbyModificationSetBucketIdOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		const FTCHARToUTF8 BucketId(*Options.BucketId);
		
		EOS_LobbyModification_SetBucketIdOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;
		LobbyOptions.BucketId = BucketId.Get();

		Result = EOSHelpers::ToEOSCoreResult(EOS_LobbyModification_SetBucketId(Handle, &LobbyOptions));
	}

	return Result;
}


EOSResult UCoreLobby::EOSLobbyModificationSetPermissionLevel(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FEOSLobbyModificationSetPermissionLevelOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_LobbyModification_SetPermissionLevelOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;
		LobbyOptions.PermissionLevel = static_cast<EOS_ELobbyPermissionLevel>(Options.PermissionLevel);

		Result = EOSHelpers::ToEOSCoreResult(EOS_LobbyModification_SetPermissionLevel(Handle, &LobbyOptions));
	}

	return Result;
}

EOSResult UCoreLobby::EOSLobbyModificationSetMaxMembers(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FEOSLobbyModificationSetMaxMembersOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_LobbyModification_SetMaxMembersOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;
		LobbyOptions.MaxMembers = static_cast<uint32_t>(Options.MaxMembers);

		Result = EOSHelpers::ToEOSCoreResult(EOS_LobbyModification_SetMaxMembers(Handle, &LobbyOptions));
	}

	return Result;
}

EOSResult UCoreLobby::EOSLobbyModificationSetInvitesAllowed(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FEOSLobbyModificationSetInvitesAllowedOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_LobbyModification_SetInvitesAllowedOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;
		LobbyOptions.bInvitesAllowed = Options.bInvitesAllowed;

		Result = EOSHelpers::ToEOSCoreResult(EOS_LobbyModification_SetInvitesAllowed(Handle, &LobbyOptions));
	}

	return Result;
}

EOSResult UCoreLobby::EOSLobbyModificationAddAttribute(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FEOSLobbyModificationAddAttributeOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_LobbyModification_AddAttributeOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;
		LobbyOptions.Attribute = &Options.Attribute.AttributeData;
		LobbyOptions.Visibility = static_cast<EOS_ELobbyAttributeVisibility>(Options.Visibility);

		Result = EOSHelpers::ToEOSCoreResult(EOS_LobbyModification_AddAttribute(Handle, &LobbyOptions));
	}

	return Result;
}

EOSResult UCoreLobby::EOSLobbyModificationAddAttributeBool(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FString Key, bool bValue, EEOSELobbyAttributeVisibility Visibility)
{
	LogVerbose("");

	const FTCHARToUTF8 ConvertedKey(*Key);

	FEOSLobbyModificationAddAttributeOptions Options;
	EOS_Lobby_AttributeData LobbyAttributeData = {};
	LobbyAttributeData.ApiVersion = EOS_LOBBYSEARCH_SETPARAMETER_API_LATEST;
	LobbyAttributeData.Key = ConvertedKey.Get();
	LobbyAttributeData.Value.AsBool = bValue;
	LobbyAttributeData.ValueType = EOS_ESessionAttributeType::EOS_AT_BOOLEAN;

	Options.Attribute = LobbyAttributeData;
	Options.Visibility = Visibility;

	return EOSLobbyModificationAddAttribute(WorldContextObject, Handle, Options);
}

EOSResult UCoreLobby::EOSLobbyModificationAddAttributeDouble(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FString Key, FString Value, EEOSELobbyAttributeVisibility Visibility)
{
	LogVerbose("");

	const FTCHARToUTF8 ConvertedKey(*Key);

	FEOSLobbyModificationAddAttributeOptions Options;

	EOS_Lobby_AttributeData LobbyAttributeData = {};
	LobbyAttributeData.ApiVersion = EOS_LOBBYSEARCH_SETPARAMETER_API_LATEST;
	LobbyAttributeData.Value.AsDouble = FCString::Atod(*Value);
	LobbyAttributeData.ValueType = EOS_ESessionAttributeType::EOS_AT_DOUBLE;

	Options.Attribute = LobbyAttributeData;
	Options.Visibility = Visibility;

	return EOSLobbyModificationAddAttribute(WorldContextObject, Handle, Options);
}

EOSResult UCoreLobby::EOSLobbyModificationAddAttributeInt64(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FString Key, FString Value, EEOSELobbyAttributeVisibility Visibility)
{
	LogVerbose("");

	const FTCHARToUTF8 ConvertedKey(*Key);

	FEOSLobbyModificationAddAttributeOptions Options;
	EOS_Lobby_AttributeData LobbyAttributeData = {};
	LobbyAttributeData.ApiVersion = EOS_LOBBYSEARCH_SETPARAMETER_API_LATEST;
	LobbyAttributeData.Value.AsInt64 = FCString::Atoi64(*Value);
	LobbyAttributeData.ValueType = EOS_ESessionAttributeType::EOS_AT_INT64;

	Options.Attribute = LobbyAttributeData;
	Options.Visibility = Visibility;

	return EOSLobbyModificationAddAttribute(WorldContextObject, Handle, Options);
}

EOSResult UCoreLobby::EOSLobbyModificationAddAttributeString(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FString Key, FString Value, EEOSELobbyAttributeVisibility Visibility)
{
	LogVerbose("");

	const FTCHARToUTF8 ConvertedKey(*Key);
	const FTCHARToUTF8 ConvertedValue(*Value);

	FEOSLobbyModificationAddAttributeOptions Options;

	EOS_Lobby_AttributeData LobbyAttributeData = {};
	LobbyAttributeData.ApiVersion = EOS_LOBBYSEARCH_SETPARAMETER_API_LATEST;
	LobbyAttributeData.Key = ConvertedKey.Get();

	LobbyAttributeData.Value.AsUtf8 = ConvertedValue.Get();
	LobbyAttributeData.ValueType = EOS_ESessionAttributeType::EOS_AT_STRING;

	Options.Attribute = LobbyAttributeData;
	Options.Visibility = Visibility;

	return EOSLobbyModificationAddAttribute(WorldContextObject, Handle, Options);
}

EOSResult UCoreLobby::EOSLobbyModificationRemoveAttribute(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FEOSLobbyModificationRemoveAttributeOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedKey(*Options.Key);

		EOS_LobbyModification_RemoveAttributeOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;

		Result = EOSHelpers::ToEOSCoreResult(EOS_LobbyModification_RemoveAttribute(Handle, &LobbyOptions));
	}

	return Result;
}

EOSResult UCoreLobby::EOSLobbyModificationAddMemberAttribute(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FEOSLobbyModificationAddMemberAttributeOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_LobbyModification_AddMemberAttributeOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;
		LobbyOptions.Attribute = &Options.Attribute.AttributeData;
		LobbyOptions.Visibility = static_cast<EOS_ELobbyAttributeVisibility>(Options.Visibility);

		Result = EOSHelpers::ToEOSCoreResult(EOS_LobbyModification_AddMemberAttribute(Handle, &LobbyOptions));
	}

	return Result;
}

EOSResult UCoreLobby::EOSLobbyModificationAddMemberAttributeBool(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FString Key, bool bValue, EEOSELobbyAttributeVisibility Visibility)
{
	LogVerbose("");

	const FTCHARToUTF8 ConvertedKey(*Key);

	FEOSLobbyModificationAddMemberAttributeOptions Options;

	EOS_Lobby_AttributeData LobbyAttributeData = {};
	LobbyAttributeData.ApiVersion = EOS_LOBBYSEARCH_SETPARAMETER_API_LATEST;
	LobbyAttributeData.Key = ConvertedKey.Get();
	LobbyAttributeData.Value.AsBool = bValue;
	LobbyAttributeData.ValueType = EOS_ESessionAttributeType::EOS_AT_BOOLEAN;

	Options.Attribute = LobbyAttributeData;
	Options.Visibility = Visibility;

	return EOSLobbyModificationAddMemberAttribute(WorldContextObject, Handle, Options);
}

EOSResult UCoreLobby::EOSLobbyModificationAddMemberAttributeDouble(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FString Key, FString Value, EEOSELobbyAttributeVisibility Visibility)
{
	LogVerbose("");

	const FTCHARToUTF8 ConvertedKey(*Key);

	FEOSLobbyModificationAddMemberAttributeOptions Options;

	EOS_Lobby_AttributeData LobbyAttributeData = {};
	LobbyAttributeData.ApiVersion = EOS_LOBBYSEARCH_SETPARAMETER_API_LATEST;
	LobbyAttributeData.Key = ConvertedKey.Get();
	LobbyAttributeData.Value.AsDouble = FCString::Atod(*Value);
	LobbyAttributeData.ValueType = EOS_ESessionAttributeType::EOS_AT_DOUBLE;

	Options.Attribute = LobbyAttributeData;
	Options.Visibility = Visibility;

	return EOSLobbyModificationAddMemberAttribute(WorldContextObject, Handle, Options);
}

EOSResult UCoreLobby::EOSLobbyModificationAddMemberAttributeInt64(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FString Key, FString Value, EEOSELobbyAttributeVisibility Visibility)
{
	LogVerbose("");

	const FTCHARToUTF8 ConvertedKey(*Key);

	FEOSLobbyModificationAddMemberAttributeOptions Options;

	EOS_Lobby_AttributeData LobbyAttributeData = {};
	LobbyAttributeData.ApiVersion = EOS_LOBBYSEARCH_SETPARAMETER_API_LATEST;
	LobbyAttributeData.Key = ConvertedKey.Get();
	LobbyAttributeData.Value.AsInt64 = FCString::Atoi64(*Value);
	LobbyAttributeData.ValueType = EOS_ESessionAttributeType::EOS_AT_INT64;

	Options.Attribute = LobbyAttributeData;
	Options.Visibility = Visibility;

	return EOSLobbyModificationAddMemberAttribute(WorldContextObject, Handle, Options);
}

EOSResult UCoreLobby::EOSLobbyModificationAddMemberAttributeString(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FString Key, FString Value, EEOSELobbyAttributeVisibility Visibility)
{
	LogVerbose("");

	const FTCHARToUTF8 ConvertedKey(*Key);
	const FTCHARToUTF8 ConvertedValue(*Value);

	FEOSLobbyModificationAddMemberAttributeOptions Options;

	EOS_Lobby_AttributeData LobbyAttributeData = {};
	LobbyAttributeData.ApiVersion = EOS_LOBBYSEARCH_SETPARAMETER_API_LATEST;
	LobbyAttributeData.Key = ConvertedKey.Get();
	LobbyAttributeData.Value.AsUtf8 = ConvertedValue.Get();
	LobbyAttributeData.ValueType = EOS_ESessionAttributeType::EOS_AT_STRING;

	Options.Attribute = LobbyAttributeData;
	Options.Visibility = Visibility;

	return EOSLobbyModificationAddMemberAttribute(WorldContextObject, Handle, Options);
}

EOSResult UCoreLobby::EOSLobbyModificationRemoveMemberAttribute(UObject* WorldContextObject, const FEOSHLobbyModification& Handle, FEOSLobbyModificationRemoveMemberAttributeOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedKey(*Options.Key);

		EOS_LobbyModification_RemoveMemberAttributeOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;

		Result = EOSHelpers::ToEOSCoreResult(EOS_LobbyModification_RemoveMemberAttribute(Handle, &LobbyOptions));
	}

	return Result;
}

EOSResult UCoreLobby::EOSLobbyModificationSetAllowedPlatformIds(UObject* WorldContextObject, FEOSHLobbyModification Handle, FEOSLobbyModificationSetAllowedPlatformIdsOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_LobbyModification_SetAllowedPlatformIdsOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = EOS_LOBBYMODIFICATION_SETALLOWEDPLATFORMIDS_API_LATEST;
		TArray<uint32_t> PointerArray;
		for (int32 i=0; i<Options.AllowedPlatformIds.Num(); i++)
		{
			PointerArray.Add(Options.AllowedPlatformIds[i]);
		}
		LobbyOptions.AllowedPlatformIds = PointerArray.GetData();
		LobbyOptions.AllowedPlatformIdsCount = Options.AllowedPlatformIds.Num();

		Result = EOSHelpers::ToEOSCoreResult(EOS_LobbyModification_SetAllowedPlatformIds(Handle, &LobbyOptions));
	}

	return Result;
}

FEOSProductUserId UCoreLobby::EOSLobbyDetailsGetLobbyOwner(UObject* WorldContextObject, const FEOSHLobbyDetails& Handle, FEOSLobbyDetailsGetLobbyOwnerOptions Options)
{
	LogVerbose("");

	FEOSProductUserId ProductUserId;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_LobbyDetails_GetLobbyOwnerOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;

		ProductUserId = EOS_LobbyDetails_GetLobbyOwner(Handle, &LobbyOptions);
	}

	return ProductUserId;
}

EOSResult UCoreLobby::EOSLobbyDetailsCopyInfo(UObject* WorldContextObject, const FEOSHLobbyDetails& Handle, FEOSLobbyDetailsCopyInfoOptions Options, FEOSLobbyDetailsInfo& OutLobbyDetailsInfo)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_LobbyDetails_CopyInfoOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;
		EOS_LobbyDetails_Info* LobbyDetails = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_LobbyDetails_CopyInfo(Handle, &LobbyOptions, &LobbyDetails));

		if (Result == EOSResult::EOS_Success)
		{
			OutLobbyDetailsInfo = *LobbyDetails;
		}
	}

	return Result;
}

EOSResult UCoreLobby::EOSLobbyDetailsCopyMemberInfo(UObject* WorldContextObject, FEOSHLobbyDetails Handle, FEOSLobbyDetailsCopyMemberInfoOptions Options, FEOSLobbyDetailsMemberInfo& OutLobbyDetailsMemberInfo)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_LobbyDetails_CopyMemberInfoOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = EOS_LOBBYDETAILS_COPYMEMBERINFO_API_LATEST;
		LobbyOptions.TargetUserId = Options.TargetUserId;
		
		EOS_LobbyDetails_MemberInfo* MemberInfo = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_LobbyDetails_CopyMemberInfo(Handle, &LobbyOptions, &MemberInfo));

		if (Result == EOSResult::EOS_Success)
		{
			OutLobbyDetailsMemberInfo = *MemberInfo;
		}
	}

	return Result;
}

int32 UCoreLobby::EOSLobbyDetailsGetAttributeCount(UObject* WorldContextObject, const FEOSHLobbyDetails& Handle, FEOSLobbyDetailsGetAttributeCountOptions Options)
{
	LogVerbose("");

	int32 Result = 0;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_LobbyDetails_GetAttributeCountOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;

		Result = EOS_LobbyDetails_GetAttributeCount(Handle, &LobbyOptions);
	}
	return Result;
}

EOSResult UCoreLobby::EOSLobbyDetailsCopyAttributeByIndex(UObject* WorldContextObject, const FEOSHLobbyDetails& Handle, FEOSLobbyDetailsCopyAttributeByIndexOptions Options, FEOSLobbyAttribute& OutAttribute)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_LobbyDetails_CopyAttributeByIndexOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;
		LobbyOptions.AttrIndex = static_cast<uint32_t>(Options.AttrIndex);

		EOS_Lobby_Attribute* LobbyDetails = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_LobbyDetails_CopyAttributeByIndex(Handle, &LobbyOptions, &LobbyDetails));

		if (Result == EOSResult::EOS_Success)
		{
			OutAttribute = *LobbyDetails;
		}
	}

	return Result;
}

EOSResult UCoreLobby::EOSLobbyDetailsCopyAttributeByKey(UObject* WorldContextObject, const FEOSHLobbyDetails& Handle, FEOSLobbyDetailsCopyAttributeByKeyOptions Options, FEOSLobbyAttribute& OutAttribute)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedKey(*Options.AttrKey);

		EOS_LobbyDetails_CopyAttributeByKeyOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;
		LobbyOptions.AttrKey = ConvertedKey.Get();

		EOS_Lobby_Attribute* LobbyDetails = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_LobbyDetails_CopyAttributeByKey(Handle, &LobbyOptions, &LobbyDetails));

		if (Result == EOSResult::EOS_Success)
		{
			OutAttribute = *LobbyDetails;
		}
	}

	return Result;
}

int32 UCoreLobby::EOSLobbyDetailsGetMemberCount(UObject* WorldContextObject, const FEOSHLobbyDetails& Handle, FEOSLobbyDetailsGetMemberCountOptions Options)
{
	LogVerbose("");

	int32 Result = 0;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_LobbyDetails_GetMemberCountOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;

		Result = EOS_LobbyDetails_GetMemberCount(Handle, &LobbyOptions);
	}

	return Result;
}

FEOSProductUserId UCoreLobby::EOSLobbyDetailsGetMemberByIndex(UObject* WorldContextObject, const FEOSHLobbyDetails& Handle, FEOSLobbyDetailsGetMemberByIndexOptions Options)
{
	LogVerbose("");

	FEOSProductUserId ProductUserId;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_LobbyDetails_GetMemberByIndexOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;
		LobbyOptions.MemberIndex = static_cast<uint32_t>(Options.MemberIndex);

		ProductUserId = EOS_LobbyDetails_GetMemberByIndex(Handle, &LobbyOptions);
	}

	return ProductUserId;
}

int32 UCoreLobby::EOSLobbyDetailsGetMemberAttributeCount(UObject* WorldContextObject, const FEOSHLobbyDetails& Handle, FEOSLobbyDetailsGetMemberAttributeCountOptions Options)
{
	LogVerbose("");

	int32 Result = 0;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_LobbyDetails_GetMemberAttributeCountOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;
		LobbyOptions.TargetUserId = Options.TargetUserId;

		Result = EOS_LobbyDetails_GetMemberAttributeCount(Handle, &LobbyOptions);
	}

	return Result;
}

EOSResult UCoreLobby::EOSLobbyDetailsCopyMemberAttributeByIndex(UObject* WorldContextObject, const FEOSHLobbyDetails& Handle, FEOSLobbyDetailsCopyMemberAttributeByIndexOptions Options, FEOSLobbyAttribute& OutAttribute)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_LobbyDetails_CopyMemberAttributeByIndexOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;
		LobbyOptions.TargetUserId = Options.TargetUserId;
		LobbyOptions.AttrIndex = static_cast<uint32_t>(Options.AttrIndex);

		EOS_Lobby_Attribute* LobbyDetails = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_LobbyDetails_CopyMemberAttributeByIndex(Handle, &LobbyOptions, &LobbyDetails));

		if (Result == EOSResult::EOS_Success)
		{
			OutAttribute = *LobbyDetails;
		}
	}

	return Result;
}

EOSResult UCoreLobby::EOSLobbyDetailsCopyMemberAttributeByKey(UObject* WorldContextObject, const FEOSHLobbyDetails& Handle, FEOSLobbyDetailsCopyMemberAttributeByKeyOptions Options, FEOSLobbyAttribute& OutAttribute)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedKey(*Options.AttrKey);

		EOS_LobbyDetails_CopyMemberAttributeByKeyOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;
		LobbyOptions.TargetUserId = Options.TargetUserId;
		LobbyOptions.AttrKey = ConvertedKey.Get();

		EOS_Lobby_Attribute* LobbyDetails = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_LobbyDetails_CopyMemberAttributeByKey(Handle, &LobbyOptions, &LobbyDetails));

		if (Result == EOSResult::EOS_Success)
		{
			OutAttribute = *LobbyDetails;
		}
	}

	return Result;
}

void UCoreLobby::EOSLobbySearchFind(UObject* WorldContextObject, const FEOSHLobbySearch& Handle, FEOSLobbySearchFindOptions Options, const FOnLobbyFindCallback& Callback)
{
	LogVerbose("");

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_LobbySearch_FindOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;
		LobbyOptions.LocalUserId = Options.LocalUserId;

		FLobbyFindCallback* CallbackObj = new FLobbyFindCallback(Callback);
		EOS_LobbySearch_Find(Handle, &LobbyOptions, CallbackObj, Internal_OnFindCallback);
	}
}

EOSResult UCoreLobby::EOSLobbySearchSetLobbyId(UObject* WorldContextObject, const FEOSHLobbySearch& Handle, FEOSLobbySearchSetLobbyIdOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		const FSetLobbyIdOptions LobbyOptions(Options.LobbyId);
		
		Result = EOSHelpers::ToEOSCoreResult(EOS_LobbySearch_SetLobbyId(Handle, &LobbyOptions));
	}

	return Result;
}

EOSResult UCoreLobby::EOSLobbySearchSetTargetUserId(UObject* WorldContextObject, const FEOSHLobbySearch& Handle, FEOSLobbySearchSetTargetUserIdOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_LobbySearch_SetTargetUserIdOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;
		LobbyOptions.TargetUserId = Options.TargetUserId;

		Result = EOSHelpers::ToEOSCoreResult(EOS_LobbySearch_SetTargetUserId(Handle, &LobbyOptions));
	}

	return Result;
}

EOSResult UCoreLobby::EOSLobbySearchSetParameter(UObject* WorldContextObject, const FEOSHLobbySearch& Handle, FEOSLobbySearchSetParameterOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_LobbySearch_SetParameterOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;
		LobbyOptions.Parameter = &Options.Parameter.AttributeData;
		LobbyOptions.ComparisonOp = static_cast<EOS_EComparisonOp>(Options.ComparisonOp);

		Result = EOSHelpers::ToEOSCoreResult(EOS_LobbySearch_SetParameter(Handle, &LobbyOptions));
	}

	return Result;
}

EOSResult UCoreLobby::EOSLobbySearchSetParameterBool(UObject* WorldContextObject, const FEOSHLobbySearch& Handle, FString Key, bool bValue, EEOSEComparisonOp ComparisonOp)
{
	LogVerbose("");

	const FTCHARToUTF8 ConvertedKey(*Key);

	FEOSLobbySearchSetParameterOptions Options;

	EOS_Lobby_AttributeData LobbyAttributeData = {};
	LobbyAttributeData.ApiVersion = EOS_LOBBYSEARCH_SETPARAMETER_API_LATEST;
	LobbyAttributeData.Key = ConvertedKey.Get();
	LobbyAttributeData.Value.AsBool = bValue;
	LobbyAttributeData.ValueType = EOS_ESessionAttributeType::EOS_AT_BOOLEAN;

	Options.Parameter = LobbyAttributeData;
	Options.ComparisonOp = ComparisonOp;

	return EOSLobbySearchSetParameter(WorldContextObject, Handle, Options);
}

EOSResult UCoreLobby::EOSLobbySearchSetParameterDouble(UObject* WorldContextObject, const FEOSHLobbySearch& Handle, FString Key, FString Value, EEOSEComparisonOp ComparisonOp)
{
	LogVerbose("");

	const FTCHARToUTF8 ConvertedKey(*Key);

	FEOSLobbySearchSetParameterOptions Options;

	EOS_Lobby_AttributeData LobbyAttributeData = {};
	LobbyAttributeData.ApiVersion = EOS_LOBBYSEARCH_SETPARAMETER_API_LATEST;
	LobbyAttributeData.Key = ConvertedKey.Get();
	LobbyAttributeData.Value.AsDouble = FCString::Atod(*Value);
	LobbyAttributeData.ValueType = EOS_ESessionAttributeType::EOS_AT_DOUBLE;

	Options.Parameter = LobbyAttributeData;
	Options.ComparisonOp = ComparisonOp;

	return EOSLobbySearchSetParameter(WorldContextObject, Handle, Options);
}

EOSResult UCoreLobby::EOSLobbySearchSetParameterInt64(UObject* WorldContextObject, const FEOSHLobbySearch& Handle, FString Key, FString Value, EEOSEComparisonOp ComparisonOp)
{
	LogVerbose("");

	const FTCHARToUTF8 ConvertedKey(*Key);

	FEOSLobbySearchSetParameterOptions Options;

	EOS_Lobby_AttributeData LobbyAttributeData = {};
	LobbyAttributeData.ApiVersion = EOS_LOBBYSEARCH_SETPARAMETER_API_LATEST;
	LobbyAttributeData.Key = ConvertedKey.Get();
	LobbyAttributeData.Value.AsInt64 = FCString::Atoi64(*Value);
	LobbyAttributeData.ValueType = EOS_ESessionAttributeType::EOS_AT_INT64;

	Options.Parameter = LobbyAttributeData;
	Options.ComparisonOp = ComparisonOp;

	return EOSLobbySearchSetParameter(WorldContextObject, Handle, Options);
}

EOSResult UCoreLobby::EOSLobbySearchSetParameterString(UObject* WorldContextObject, const FEOSHLobbySearch& Handle, FString Key, FString Value, EEOSEComparisonOp ComparisonOp)
{
	LogVerbose("");

	const FTCHARToUTF8 ConvertedKey(*Key);
	const FTCHARToUTF8 ConvertedValue(*Value);

	FEOSLobbySearchSetParameterOptions Options;

	EOS_Lobby_AttributeData LobbyAttributeData = {};
	LobbyAttributeData.ApiVersion = EOS_LOBBYSEARCH_SETPARAMETER_API_LATEST;
	LobbyAttributeData.Key = ConvertedKey.Get();
	LobbyAttributeData.Value.AsUtf8 = ConvertedValue.Get();
	LobbyAttributeData.ValueType = EOS_ESessionAttributeType::EOS_AT_STRING;

	Options.Parameter = LobbyAttributeData;
	Options.ComparisonOp = ComparisonOp;

	return EOSLobbySearchSetParameter(WorldContextObject, Handle, Options);
}

EOSResult UCoreLobby::EOSLobbySearchRemoveParameter(UObject* WorldContextObject, const FEOSHLobbySearch& Handle, FEOSLobbySearchRemoveParameterOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedKey(*Options.Key);

		EOS_LobbySearch_RemoveParameterOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;
		LobbyOptions.Key = ConvertedKey.Get();
		LobbyOptions.ComparisonOp = static_cast<EOS_EComparisonOp>(Options.ComparisonOp);

		Result = EOSHelpers::ToEOSCoreResult(EOS_LobbySearch_RemoveParameter(Handle, &LobbyOptions));
	}

	return Result;
}

EOSResult UCoreLobby::EOSLobbySearchSetMaxResults(UObject* WorldContextObject, const FEOSHLobbySearch& Handle, FEOSLobbySearchSetMaxResultsOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_LobbySearch_SetMaxResultsOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;
		LobbyOptions.MaxResults = static_cast<uint32_t>(Options.MaxResults);

		Result = EOSHelpers::ToEOSCoreResult(EOS_LobbySearch_SetMaxResults(Handle, &LobbyOptions));
	}

	return Result;
}

int32 UCoreLobby::EOSLobbySearchGetSearchResultCount(UObject* WorldContextObject, const FEOSHLobbySearch& Handle, FEOSLobbySearchGetSearchResultCountOptions Options)
{
	LogVerbose("");

	int32 Result = 0;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_LobbySearch_GetSearchResultCountOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;

		Result = EOS_LobbySearch_GetSearchResultCount(Handle, &LobbyOptions);
	}

	return Result;
}

EOSResult UCoreLobby::EOSLobbySearchCopySearchResultByIndex(UObject* WorldContextObject, const FEOSHLobbySearch& Handle, FEOSLobbySearchCopySearchResultByIndexOptions Options, FEOSHLobbyDetails& OutLobbyDetailsHandle)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLobbyHandle(WorldContextObject))
	{
		EOS_LobbySearch_CopySearchResultByIndexOptions LobbyOptions = {};
		LobbyOptions.ApiVersion = Options.ApiVersion;
		LobbyOptions.LobbyIndex = static_cast<uint32_t>(Options.LobbyIndex);

		EOS_HLobbyDetails LobbyHandle = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_LobbySearch_CopySearchResultByIndex(Handle, &LobbyOptions, &LobbyHandle));

		if (Result == EOSResult::EOS_Success)
		{
			OutLobbyDetailsHandle = LobbyHandle;
		}
	}

	return Result;
}

void UCoreLobby::EOSLobbyModificationRelease(FEOSHLobbyModification LobbyModificationHandle)
{
	LogVerbose("");

	EOS_LobbyModification_Release(LobbyModificationHandle);
}

void UCoreLobby::EOSLobbyDetailsRelease(FEOSHLobbyDetails LobbyHandle)
{
	LogVerbose("");

	EOS_LobbyDetails_Release(LobbyHandle);
}

void UCoreLobby::EOSLobbySearchRelease(FEOSHLobbySearch LobbySearchHandle)
{
	LogVerbose("");

	EOS_LobbySearch_Release(LobbySearchHandle);
}

void UCoreLobby::Internal_OnCreateLobbyCallback(const EOS_Lobby_CreateLobbyCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FCreateLobbyCallback* CallbackObj = static_cast<FCreateLobbyCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreLobby::Internal_OnDestroyLobbyCallback(const EOS_Lobby_DestroyLobbyCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FDestroyLobbyCallback* CallbackObj = static_cast<FDestroyLobbyCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreLobby::Internal_OnJoinLobbyCallback(const EOS_Lobby_JoinLobbyCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FJoinLobbyCallback* CallbackObj = static_cast<FJoinLobbyCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreLobby::Internal_OnLeaveLobbyCallback(const EOS_Lobby_LeaveLobbyCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FLeaveLobbyCallback* CallbackObj = static_cast<FLeaveLobbyCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreLobby::Internal_OnUpdateLobbyCallback(const EOS_Lobby_UpdateLobbyCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FUpdateLobbyCallback* CallbackObj = static_cast<FUpdateLobbyCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreLobby::Internal_OnPromoteMemberCallback(const EOS_Lobby_PromoteMemberCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FLobbyPromoteMemberCallback* CallbackObj = static_cast<FLobbyPromoteMemberCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreLobby::Internal_OnKickMemberCallback(const EOS_Lobby_KickMemberCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FLobbyKickMemberCallback* CallbackObj = static_cast<FLobbyKickMemberCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreLobby::Internal_OnSendInviteCallback(const EOS_Lobby_SendInviteCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FLobbySendInviteCallback* CallbackObj = static_cast<FLobbySendInviteCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreLobby::Internal_OnRejectInviteCallback(const EOS_Lobby_RejectInviteCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FLobbyRejectInviteCallback* CallbackObj = static_cast<FLobbyRejectInviteCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreLobby::Internal_OnQueryInvitesCallback(const EOS_Lobby_QueryInvitesCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FLobbyQueryInvitesCallback* CallbackObj = static_cast<FLobbyQueryInvitesCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreLobby::Internal_OnFindCallback(const EOS_LobbySearch_FindCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FLobbyFindCallback* CallbackObj = static_cast<FLobbyFindCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreLobby::Internal_OnJoinLobbyByIdCallback(const EOS_Lobby_JoinLobbyByIdCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FJoinLobbyByIdCallback* CallbackObj = static_cast<FJoinLobbyByIdCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreLobby::Internal_OnJoinHardMuteCallback(const EOS_Lobby_HardMuteMemberCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FHardMuteMemberCallback* CallbackObj = static_cast<FHardMuteMemberCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}
