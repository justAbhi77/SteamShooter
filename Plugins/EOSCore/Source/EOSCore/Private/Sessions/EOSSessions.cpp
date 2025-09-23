/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "Sessions/EOSSessions.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSHelpers.h"
#include "Core/EOSCoreLogging.h"

void UCoreSessions::Deinitialize()
{
	LogVerbose("");

	for (auto Element: m_SessionInviteReceivedCallbacks)
	{
		EOS_Sessions_RemoveNotifySessionInviteReceived(GetSessionsHandle(this), Element.Key);
	}

	for (auto Element: m_SessionInviteAcceptedCallbacks)
	{
		EOS_Sessions_RemoveNotifySessionInviteAccepted(GetSessionsHandle(this), Element.Key);
	}

	for (auto Element: m_SessionJoinSessionAcceptedCallbacks)
	{
		EOS_Sessions_RemoveNotifySessionInviteAccepted(GetSessionsHandle(this), Element.Key);
	}

	m_SessionInviteReceivedCallbacks.Empty();
	m_SessionInviteAcceptedCallbacks.Empty();
	m_SessionJoinSessionAcceptedCallbacks.Empty();
}

UCoreSessions* UCoreSessions::GetSessions(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (GetSessionsHandle(WorldContextObject))
		{
			if (UWorld* World = WorldContextObject->GetWorld())
			{
				if (UGameInstance* GameInstance = World->GetGameInstance())
				{
					return GameInstance->GetSubsystem<UCoreSessions>();
				}
			}	
		}
	}
	return nullptr;
}

EOSResult UCoreSessions::EOSSessionsCreateSessionModification(UObject* WorldContextObject, FEOSSessionsCreateSessionModificationOptions Options, FEOSHSessionModification& OutSessionModificationHandle)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetSessionsHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedSessionName(*Options.SessionName);
		const FTCHARToUTF8 BucketId(*Options.BucketId);
		const FTCHARToUTF8 SessionId(*Options.SessionId);

		EOS_Sessions_CreateSessionModificationOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONS_CREATESESSIONMODIFICATION_API_LATEST;
		SessionsOptions.SessionName = ConvertedSessionName.Get();
		SessionsOptions.BucketId = BucketId.Get();
		SessionsOptions.MaxPlayers = static_cast<uint32_t>(Options.MaxPlayers);
		SessionsOptions.LocalUserId = Options.LocalUserId;
		SessionsOptions.bPresenceEnabled = Options.bPresenceEnabled;
		if (Options.SessionId.Len() > 0)
		{
			SessionsOptions.SessionId = SessionId.Get();
		}
		SessionsOptions.bSanctionsEnabled = Options.bSanctionsEnabled;

		TArray<uint32_t> AllowedPlatformIds;
		if (Options.AllowedPlatformIds.Num() > 0)
		{
			for (int32 i=0; i<Options.AllowedPlatformIds.Num(); i++)
			{
				AllowedPlatformIds.Add(Options.AllowedPlatformIds[i]);
			}
			SessionsOptions.AllowedPlatformIds = AllowedPlatformIds.GetData();
			SessionsOptions.AllowedPlatformIdsCount = Options.AllowedPlatformIds.Num();
		} else
		{
			SessionsOptions.AllowedPlatformIds = nullptr;
			SessionsOptions.AllowedPlatformIdsCount = 0;
		}

		EOS_HSessionModification Handle = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Sessions_CreateSessionModification(GetSessionsHandle(WorldContextObject), &SessionsOptions, &Handle));

		if (Result == EOSResult::EOS_Success)
		{
			OutSessionModificationHandle = Handle;
		}
	}

	return Result;
}

EOSResult UCoreSessions::EOSSessionsUpdateSessionModification(UObject* WorldContextObject, FEOSSessionsUpdateSessionModificationOptions Options, FEOSHSessionModification& OutSessionModificationHandle)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetSessionsHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedSessionName(*Options.SessionName);

		EOS_Sessions_UpdateSessionModificationOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONS_UPDATESESSIONMODIFICATION_API_LATEST;
		SessionsOptions.SessionName = ConvertedSessionName.Get();

		EOS_HSessionModification Handle = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Sessions_UpdateSessionModification(GetSessionsHandle(WorldContextObject), &SessionsOptions, &Handle));

		if (Result == EOSResult::EOS_Success)
		{
			OutSessionModificationHandle = Handle;
		}
	}

	return Result;
}

void UCoreSessions::EOSSessionsUpdateSession(UObject* WorldContextObject, const FEOSSessionsUpdateSessionOptions& Options, const FOnSessionUpdateSessionCallback& Callback)
{
	LogVerbose("");

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_Sessions_UpdateSessionOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONS_UPDATESESSION_API_LATEST;
		SessionsOptions.SessionModificationHandle = Options.SessionModificationHandle;

		FUpdateSessionCallback* CallbackObj = new FUpdateSessionCallback(Callback);
		EOS_Sessions_UpdateSession(GetSessionsHandle(WorldContextObject), &SessionsOptions, CallbackObj, Internal_OnSessionsUpdateSessionCallback);
	}
}

void UCoreSessions::EOSSessionsDestroySession(UObject* WorldContextObject, FEOSSessionsDestroySessionOptions Options, const FOnSessionDestroySessionCallback& Callback)
{
	LogVerbose("");

	if (GetSessionsHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedSessionName(*Options.SessionName);

		EOS_Sessions_DestroySessionOptions SessionsOptions = { };
		SessionsOptions.ApiVersion = EOS_SESSIONS_DESTROYSESSION_API_LATEST;
		SessionsOptions.SessionName = ConvertedSessionName.Get();

		FDestroySessionCallback* CallbackObj = new FDestroySessionCallback(Callback);
		EOS_Sessions_DestroySession(GetSessionsHandle(WorldContextObject), &SessionsOptions, CallbackObj, Internal_OnSessionsDestroySessionCallback);
	}
}

void UCoreSessions::EOSSessionsJoinSession(UObject* WorldContextObject, FEOSSessionsJoinSessionOptions Options, const FOnSessionJoinSessionCallback& Callback)
{
	LogVerbose("");

	if (GetSessionsHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedSessionName(*Options.SessionName);

		EOS_Sessions_JoinSessionOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONS_JOINSESSION_API_LATEST;
		SessionsOptions.SessionName = ConvertedSessionName.Get();
		SessionsOptions.SessionHandle = Options.SessionHandle;
		SessionsOptions.LocalUserId = Options.LocalUserId;
		SessionsOptions.bPresenceEnabled = Options.bPresenceEnabled;

		FJoinSessionCallback* CallbackObj = new FJoinSessionCallback(Callback);
		EOS_Sessions_JoinSession(GetSessionsHandle(WorldContextObject), &SessionsOptions, CallbackObj, Internal_OnSessionsJoinSessionCallback);
	}
}

void UCoreSessions::EOSSessionsStartSession(UObject* WorldContextObject, FEOSSessionsStartSessionOptions Options, const FOnSessionStartSessionCallback& Callback)
{
	LogVerbose("");

	if (GetSessionsHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedSessionName(*Options.SessionName);

		EOS_Sessions_StartSessionOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONS_STARTSESSION_API_LATEST;
		SessionsOptions.SessionName = ConvertedSessionName.Get();

		FStartSessionCallback* CallbackObj = new FStartSessionCallback(Callback);
		EOS_Sessions_StartSession(GetSessionsHandle(WorldContextObject), &SessionsOptions, CallbackObj, Internal_OnSessionsStartSessionCallback);
	}
}

void UCoreSessions::EOSSessionsEndSession(UObject* WorldContextObject, FEOSSessionsEndSessionOptions Options, const FOnSessionEndSessionCallback& Callback)
{
	LogVerbose("");

	if (GetSessionsHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedSessionName(*Options.SessionName);

		EOS_Sessions_EndSessionOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONS_ENDSESSION_API_LATEST;
		SessionsOptions.SessionName = ConvertedSessionName.Get();

		FEndSessionCallback* CallbackObj = new FEndSessionCallback(Callback);
		EOS_Sessions_EndSession(GetSessionsHandle(WorldContextObject), &SessionsOptions, CallbackObj, Internal_OnSessionsEndSessionCallback);
	}
}

void UCoreSessions::EOSSessionsRegisterPlayers(UObject* WorldContextObject, FEOSSessionsRegisterPlayersOptions Options, const FOnSessionRegisterPlayersCallback& Callback)
{
	LogVerbose("");

	if (GetSessionsHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedSessionName(*Options.SessionName);

		EOS_Sessions_RegisterPlayersOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONS_REGISTERPLAYERS_API_LATEST;
		SessionsOptions.SessionName = ConvertedSessionName.Get();
		SessionsOptions.PlayersToRegister = new EOS_ProductUserId[Options.PlayersToRegister.Num()];
		SessionsOptions.PlayersToRegisterCount = static_cast<uint32_t>(Options.PlayersToRegister.Num());

		if (Options.PlayersToRegister.Num() > 0)
		{
			for (int32 i = 0; i < Options.PlayersToRegister.Num(); i++)
				SessionsOptions.PlayersToRegister[i] = Options.PlayersToRegister[i];
		}
		else
		{
			SessionsOptions.PlayersToRegister = NULL;
			SessionsOptions.PlayersToRegisterCount = 0;
		}

		FRegisterPlayersCallback* CallbackObj = new FRegisterPlayersCallback(Callback);
		EOS_Sessions_RegisterPlayers(GetSessionsHandle(WorldContextObject), &SessionsOptions, CallbackObj, Internal_OnSessionRegisterPlayersCallback);

		delete[] SessionsOptions.PlayersToRegister;
	}
}

void UCoreSessions::EOSSessionsUnregisterPlayers(UObject* WorldContextObject, FEOSSessionsUnregisterPlayersOptions Options, const FOnSessionUnregisterPlayersCallback& Callback)
{
	LogVerbose("");

	if (GetSessionsHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedSessionName(*Options.SessionName);

		EOS_Sessions_UnregisterPlayersOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONS_UNREGISTERPLAYERS_API_LATEST;
		SessionsOptions.SessionName = ConvertedSessionName.Get();
		SessionsOptions.PlayersToUnregister = new EOS_ProductUserId[Options.PlayersToUnregister.Num()];
		SessionsOptions.PlayersToUnregisterCount = static_cast<uint32_t>(Options.PlayersToUnregister.Num());

		if (Options.PlayersToUnregister.Num() > 0)
		{
			for (int32 i = 0; i < Options.PlayersToUnregister.Num(); i++)
				SessionsOptions.PlayersToUnregister[i] = Options.PlayersToUnregister[i];
		}
		else
		{
			SessionsOptions.PlayersToUnregister = NULL;
		}
		FUnregisterPlayersCallback* CallbackObj = new FUnregisterPlayersCallback(Callback);
		EOS_Sessions_UnregisterPlayers(GetSessionsHandle(WorldContextObject), &SessionsOptions, CallbackObj, Internal_OnSessionsUnregisterPlayersCallback);

		delete[] SessionsOptions.PlayersToUnregister;
	}
}

void UCoreSessions::EOSSessionsSendInvite(UObject* WorldContextObject, FEOSSessionsSendInviteOptions Options, const FOnSessionSessionSendInviteCallback& Callback)
{
	LogVerbose("");

	if (GetSessionsHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedSessionName(*Options.SessionName);

		EOS_Sessions_SendInviteOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONS_SENDINVITE_API_LATEST;
		SessionsOptions.SessionName = ConvertedSessionName.Get();
		SessionsOptions.LocalUserId = Options.LocalUserId;
		SessionsOptions.TargetUserId = Options.TargetUserId;

		FSendInviteCallback* CallbackObj = new FSendInviteCallback(Callback);
		EOS_Sessions_SendInvite(GetSessionsHandle(WorldContextObject), &SessionsOptions, CallbackObj, Internal_OnSessionsSendInviteCallback);
	}
}

void UCoreSessions::EOSSessionsRejectInvite(UObject* WorldContextObject, FEOSSessionsRejectInviteOptions Options, const FOnSessionRejectInviteCallback& Callback)
{
	LogVerbose("");

	if (GetSessionsHandle(WorldContextObject))
	{
		const FTCHARToUTF8 InviteId(*Options.InviteId);
		
		EOS_Sessions_RejectInviteOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONS_REJECTINVITE_API_LATEST;
		SessionsOptions.LocalUserId = Options.LocalUserId;
		SessionsOptions.InviteId = InviteId.Get();

		FRejectInviteCallback* CallbackObj = new FRejectInviteCallback(Callback);
		EOS_Sessions_RejectInvite(GetSessionsHandle(WorldContextObject), &SessionsOptions, CallbackObj, Internal_OnSessionsRejectInviteCallback);
	}
}

void UCoreSessions::EOSSessionsQueryInvites(UObject* WorldContextObject, FEOSSessionsQueryInvitesOptions Options, const FOnSessionQueryInvitesCallback& Callback)
{
	LogVerbose("");

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_Sessions_QueryInvitesOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONS_QUERYINVITES_API_LATEST;
		SessionsOptions.LocalUserId = Options.LocalUserId;

		FQueryInvitesCallback* CallbackObj = new FQueryInvitesCallback(Callback);
		EOS_Sessions_QueryInvites(GetSessionsHandle(WorldContextObject), &SessionsOptions, CallbackObj, Internal_OnSessionsQueryInvitesCallback);
	}
}

int32 UCoreSessions::EOSSessionsGetInviteCount(UObject* WorldContextObject, FEOSSessionsGetInviteCountOptions Options)
{
	LogVerbose("");

	int32 Result = 0;

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_Sessions_GetInviteCountOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONS_GETINVITECOUNT_API_LATEST;
		SessionsOptions.LocalUserId = Options.LocalUserId;

		Result = EOS_Sessions_GetInviteCount(GetSessionsHandle(WorldContextObject), &SessionsOptions);
	}

	return Result;
}

EOSResult UCoreSessions::EOSSessionsGetInviteIdByIndex(UObject* WorldContextObject, FEOSSessionsGetInviteIdByIndexOptions Options, FString& OutBuffer)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_Sessions_GetInviteIdByIndexOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONS_GETINVITEIDBYINDEX_API_LATEST;
		SessionsOptions.LocalUserId = Options.LocalUserId;
		SessionsOptions.Index = static_cast<uint32_t>(Options.Index);

		char* Buffer = new char[1024];
		int32_t* Size = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Sessions_GetInviteIdByIndex(GetSessionsHandle(WorldContextObject), &SessionsOptions, Buffer, Size));

		if (Result == EOSResult::EOS_Success)
		{
			OutBuffer = Buffer;
		}

		delete[] Buffer;
	}

	return Result;
}

EOSResult UCoreSessions::EOSSessionsCreateSessionSearch(UObject* WorldContextObject, FEOSSessionsCreateSessionSearchOptions Options, FEOSHSessionSearch& OutSessionSearchHandle)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_Sessions_CreateSessionSearchOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONS_CREATESESSIONSEARCH_API_LATEST;
		SessionsOptions.MaxSearchResults = static_cast<uint32_t>(Options.MaxSearchResults);

		EOS_HSessionSearch Handle = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Sessions_CreateSessionSearch(GetSessionsHandle(WorldContextObject), &SessionsOptions, &Handle));

		if (Result == EOSResult::EOS_Success)
		{
			OutSessionSearchHandle = Handle;
		}
	}

	return Result;
}

EOSResult UCoreSessions::EOSSessionsCopyActiveSessionHandle(UObject* WorldContextObject, FEOSSessionsCopyActiveSessionHandleOptions Options, FEOSHActiveSession& OutSessionHandle)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetSessionsHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedSessionName(*Options.SessionName);

		EOS_Sessions_CopyActiveSessionHandleOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONS_COPYACTIVESESSIONHANDLE_API_LATEST;
		SessionsOptions.SessionName = ConvertedSessionName.Get();

		EOS_HActiveSession Handle = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Sessions_CopyActiveSessionHandle(GetSessionsHandle(WorldContextObject), &SessionsOptions, &Handle));

		if (Result == EOSResult::EOS_Success)
		{
			OutSessionHandle = Handle;
		}
	}

	return Result;
}

FEOSNotificationId UCoreSessions::EOSSessionsAddNotifySessionInviteReceived(UObject* WorldContextObject, const FOnSessionInviteReceivedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId NotificationId;

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_Sessions_AddNotifySessionInviteReceivedOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONS_ADDNOTIFYSESSIONINVITERECEIVED_API_LATEST;

		FSessionInviteReceivedCallback* CallbackObj = new FSessionInviteReceivedCallback(Callback);

		NotificationId = EOS_Sessions_AddNotifySessionInviteReceived(GetSessionsHandle(WorldContextObject), &SessionsOptions, CallbackObj, ([](const EOS_Sessions_SessionInviteReceivedCallbackInfo* data)
		{
			FSessionInviteReceivedCallback* CallbackData = static_cast<FSessionInviteReceivedCallback*>(data->ClientData);
			CallbackData->m_Callback.ExecuteIfBound(*data);
		}));
		GetSessions(WorldContextObject)->m_SessionInviteReceivedCallbacks.Add(NotificationId, CallbackObj);
	}

	return NotificationId;
}

void UCoreSessions::EOSSessionsRemoveNotifySessionInviteReceived(UObject* WorldContextObject, FEOSNotificationId Id)
{
	LogVerbose("");

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_Sessions_RemoveNotifySessionInviteReceived(GetSessionsHandle(WorldContextObject), Id);
		GetSessions(WorldContextObject)->m_SessionInviteReceivedCallbacks.Remove(Id);
	}
}

FEOSNotificationId UCoreSessions::EOSSessionsAddNotifySessionInviteAccepted(UObject* WorldContextObject, const FOnSessionInviteAcceptedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId NotificationId;

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_Sessions_AddNotifySessionInviteAcceptedOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONS_ADDNOTIFYSESSIONINVITEACCEPTED_API_LATEST;

		FSessionInviteAcceptedCallback* CallbackObj = new FSessionInviteAcceptedCallback(Callback);

		NotificationId = EOS_Sessions_AddNotifySessionInviteAccepted(GetSessionsHandle(WorldContextObject), &SessionsOptions, CallbackObj, ([](const EOS_Sessions_SessionInviteAcceptedCallbackInfo* data)
		{
			FSessionInviteAcceptedCallback* CallbackData = static_cast<FSessionInviteAcceptedCallback*>(data->ClientData);
			CallbackData->m_Callback.ExecuteIfBound(*data);
		}));
		GetSessions(WorldContextObject)->m_SessionInviteAcceptedCallbacks.Add(NotificationId, CallbackObj);
	}

	return NotificationId;
}

void UCoreSessions::EOSSessionsRemoveNotifySessionInviteAccepted(UObject* WorldContextObject, FEOSNotificationId Id)
{
	LogVerbose("");

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_Sessions_RemoveNotifySessionInviteAccepted(GetSessionsHandle(WorldContextObject), Id);
		GetSessions(WorldContextObject)->m_SessionInviteAcceptedCallbacks.Remove(Id);
	}
}

FEOSNotificationId UCoreSessions::EOSSessionsAddNotifyJoinSessionAccepted(UObject* WorldContextObject, const FOnSessionJoinSessionAcceptedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId NotificationId;

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_Sessions_AddNotifyJoinSessionAcceptedOptions SessionsOptions = {EOS_SESSIONS_ADDNOTIFYJOINSESSIONACCEPTED_API_LATEST};
		SessionsOptions.ApiVersion = EOS_SESSIONS_ADDNOTIFYJOINSESSIONACCEPTED_API_LATEST;

		FSessionJoinSessionAcceptedCallback* CallbackObj = new FSessionJoinSessionAcceptedCallback(Callback);

		NotificationId = EOS_Sessions_AddNotifyJoinSessionAccepted(GetSessionsHandle(WorldContextObject), &SessionsOptions, CallbackObj, ([](const EOS_Sessions_JoinSessionAcceptedCallbackInfo* data)
		{
			FSessionJoinSessionAcceptedCallback* CallbackData = static_cast<FSessionJoinSessionAcceptedCallback*>(data->ClientData);
			CallbackData->m_Callback.ExecuteIfBound(*data);
		}));
		GetSessions(WorldContextObject)->m_SessionJoinSessionAcceptedCallbacks.Add(NotificationId, CallbackObj);
	}

	return NotificationId;
}

void UCoreSessions::EOSSessionsRemoveNotifyJoinSessionAccepted(UObject* WorldContextObject, FEOSNotificationId Id)
{
	LogVerbose("");

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_Sessions_RemoveNotifySessionInviteAccepted(GetSessionsHandle(WorldContextObject), Id);
		GetSessions(WorldContextObject)->m_SessionJoinSessionAcceptedCallbacks.Remove(Id);
	}
}

EOSResult UCoreSessions::EOSSessionsCopySessionHandleByInviteId(UObject* WorldContextObject, FEOSSessionsCopySessionHandleByInviteIdOptions Options, FEOSHSessionDetails& OutSessionHandle)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetSessionsHandle(WorldContextObject))
	{
		const FTCHARToUTF8 InviteId(*Options.InviteId);
		
		EOS_Sessions_CopySessionHandleByInviteIdOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONS_COPYSESSIONHANDLEBYINVITEID_API_LATEST;
		SessionsOptions.InviteId = InviteId.Get();

		EOS_HSessionDetails Handle = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Sessions_CopySessionHandleByInviteId(GetSessionsHandle(WorldContextObject), &SessionsOptions, &Handle));

		if (Result == EOSResult::EOS_Success)
		{
			OutSessionHandle = Handle;
		}
	}

	return Result;
}


EOSResult UCoreSessions::EOSSessionsCopySessionHandleByUiEventId(UObject* WorldContextObject, FEOSSessionsCopySessionHandleByUiEventIdOptions Options, FEOSHSessionDetails& OutSessionHandle)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_Sessions_CopySessionHandleByUiEventIdOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONS_COPYSESSIONHANDLEBYUIEVENTID_API_LATEST;
		SessionsOptions.UiEventId = Options.UiEventId;

		EOS_HSessionDetails Handle = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Sessions_CopySessionHandleByUiEventId(GetSessionsHandle(WorldContextObject), &SessionsOptions, &Handle));

		if (Result == EOSResult::EOS_Success)
		{
			OutSessionHandle = Handle;
		}
	}

	return Result;
}

EOSResult UCoreSessions::EOSSessionsCopySessionHandleForPresence(UObject* WorldContextObject, FEOSSessionsCopySessionHandleForPresenceOptions Options, FEOSHSessionDetails& OutSessionHandle)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_Sessions_CopySessionHandleForPresenceOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONS_COPYSESSIONHANDLEFORPRESENCE_API_LATEST;
		SessionsOptions.LocalUserId = Options.LocalUserId;

		EOS_HSessionDetails Handle = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Sessions_CopySessionHandleForPresence(GetSessionsHandle(WorldContextObject), &SessionsOptions, &Handle));

		if (Result == EOSResult::EOS_Success)
		{
			OutSessionHandle = Handle;
		}
	}

	return Result;
}

EOSResult UCoreSessions::EOSSessionsIsUserInSession(UObject* WorldContextObject, FEOSSessionsIsUserInSessionOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetSessionsHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedSessionName(*Options.SessionName);

		EOS_Sessions_IsUserInSessionOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONS_ISUSERINSESSION_API_LATEST;
		SessionsOptions.SessionName = ConvertedSessionName.Get();
		SessionsOptions.TargetUserId = Options.TargetUserId;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Sessions_IsUserInSession(GetSessionsHandle(WorldContextObject), &SessionsOptions));
	}

	return Result;
}

EOSResult UCoreSessions::EOSSessionsDumpSessionState(UObject* WorldContextObject, FEOSSessionsDumpSessionStateOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetSessionsHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedSessionName(*Options.SessionName);

		EOS_Sessions_DumpSessionStateOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONS_DUMPSESSIONSTATE_API_LATEST;
		SessionsOptions.SessionName = ConvertedSessionName.Get();

		Result = EOSHelpers::ToEOSCoreResult(EOS_Sessions_DumpSessionState(GetSessionsHandle(WorldContextObject), &SessionsOptions));
	}

	return Result;
}

EOSResult UCoreSessions::EOSSessionModificationSetBucketId(UObject* WorldContextObject, const FEOSHSessionModification& Handle, FEOSSessionModificationSetBucketIdOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetSessionsHandle(WorldContextObject))
	{
		const FTCHARToUTF8 BucketId(*Options.BucketId);
		
		EOS_SessionModification_SetBucketIdOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONMODIFICATION_SETBUCKETID_API_LATEST;
		SessionsOptions.BucketId = BucketId.Get();

		Result = EOSHelpers::ToEOSCoreResult(EOS_SessionModification_SetBucketId(Handle, &SessionsOptions));
	}

	return Result;
}

EOSResult UCoreSessions::EOSSessionModificationSetHostAddress(UObject* WorldContextObject, const FEOSHSessionModification& Handle, FEOSSessionModificationSetHostAddressOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetSessionsHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedHostAddress(*Options.HostAddress);

		EOS_SessionModification_SetHostAddressOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONMODIFICATION_SETHOSTADDRESS_API_LATEST;
		SessionsOptions.HostAddress = ConvertedHostAddress.Get();

		Result = EOSHelpers::ToEOSCoreResult(EOS_SessionModification_SetHostAddress(Handle, &SessionsOptions));
	}

	return Result;
}

EOSResult UCoreSessions::EOSSessionModificationSetPermissionLevel(UObject* WorldContextObject, const FEOSHSessionModification& Handle, FEOSSessionModificationSetPermissionLevelOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_SessionModification_SetPermissionLevelOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONMODIFICATION_SETPERMISSIONLEVEL_API_LATEST;
		SessionsOptions.PermissionLevel = static_cast<EOS_EOnlineSessionPermissionLevel>(Options.PermissionLevel);

		Result = EOSHelpers::ToEOSCoreResult(EOS_SessionModification_SetPermissionLevel(Handle, &SessionsOptions));
	}

	return Result;
}

EOSResult UCoreSessions::EOSSessionModificationSetJoinInProgressAllowed(UObject* WorldContextObject, const FEOSHSessionModification& Handle, FEOSSessionModificationSetJoinInProgressAllowedOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_SessionModification_SetJoinInProgressAllowedOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONMODIFICATION_SETJOININPROGRESSALLOWED_API_LATEST;
		SessionsOptions.bAllowJoinInProgress = Options.bAllowJoinInProgress;

		Result = EOSHelpers::ToEOSCoreResult(EOS_SessionModification_SetJoinInProgressAllowed(Handle, &SessionsOptions));
	}

	return Result;
}

EOSResult UCoreSessions::EOSSessionModificationSetMaxPlayers(UObject* WorldContextObject, const FEOSHSessionModification& Handle, FEOSSessionModificationSetMaxPlayersOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_SessionModification_SetMaxPlayersOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONMODIFICATION_SETMAXPLAYERS_API_LATEST;
		SessionsOptions.MaxPlayers = static_cast<uint32_t>(Options.MaxPlayers);

		Result = EOSHelpers::ToEOSCoreResult(EOS_SessionModification_SetMaxPlayers(Handle, &SessionsOptions));
	}

	return Result;
}

EOSResult UCoreSessions::EOSSessionModificationSetInvitesAllowed(UObject* WorldContextObject, const FEOSHSessionModification& Handle, FEOSSessionModificationSetInvitesAllowedOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_SessionModification_SetInvitesAllowedOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONMODIFICATION_SETINVITESALLOWED_API_LATEST;
		SessionsOptions.bInvitesAllowed = Options.bInvitesAllowed;

		Result = EOSHelpers::ToEOSCoreResult(EOS_SessionModification_SetInvitesAllowed(Handle, &SessionsOptions));
	}

	return Result;
}

EOSResult UCoreSessions::EOSSessionModificationAddAttribute(UObject* WorldContextObject, const FEOSHSessionModification& Handle, const FEOSSessionModificationAddAttributeOptions& Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_SessionModification_AddAttributeOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONMODIFICATION_ADDATTRIBUTE_API_LATEST;
		SessionsOptions.SessionAttribute = &Options.Data.AttributeData;
		SessionsOptions.AdvertisementType = static_cast<EOS_ESessionAttributeAdvertisementType>(Options.AdvertisementType);

		Result = EOSHelpers::ToEOSCoreResult(EOS_SessionModification_AddAttribute(Handle, &SessionsOptions));
	}

	return Result;
}

EOSResult UCoreSessions::EOSSessionModificationAddAttributeBool(UObject* WorldContextObject, const FEOSHSessionModification& Handle, EEOSESessionAttributeAdvertisementType AdvertisementType, const FString& Key, bool bValue)
{
	LogVerbose("");

	FEOSSessionModificationAddAttributeOptions SessionsOptions;

	SessionsOptions.AdvertisementType = AdvertisementType;

	const FTCHARToUTF8 ConvertedKey(*Key);
	EOS_Sessions_AttributeData AttributeData = {};
	AttributeData.ApiVersion = EOS_SESSIONS_SESSIONATTRIBUTEDATA_API_LATEST;
	AttributeData.Key = ConvertedKey.Get();
	AttributeData.Value.AsBool = bValue;
	AttributeData.ValueType = EOS_ESessionAttributeType::EOS_AT_BOOLEAN;

	SessionsOptions.Data.AttributeData = AttributeData;

	return EOSSessionModificationAddAttribute(WorldContextObject, Handle, SessionsOptions);
}

EOSResult UCoreSessions::EOSSessionModificationAddAttributeDouble(UObject* WorldContextObject, const FEOSHSessionModification& Handle, EEOSESessionAttributeAdvertisementType AdvertisementType, const FString& Key, const FString& Value)
{
	LogVerbose("");

	FEOSSessionModificationAddAttributeOptions SessionsOptions;

	SessionsOptions.AdvertisementType = AdvertisementType;

	const FTCHARToUTF8 ConvertedKey(*Key);
	EOS_Sessions_AttributeData AttributeData = {};
	AttributeData.ApiVersion = EOS_SESSIONS_SESSIONATTRIBUTEDATA_API_LATEST;
	AttributeData.Key = ConvertedKey.Get();
	AttributeData.Value.AsDouble = FCString::Atod(*Value);
	AttributeData.ValueType = EOS_ESessionAttributeType::EOS_AT_DOUBLE;

	SessionsOptions.Data.AttributeData = AttributeData;

	return EOSSessionModificationAddAttribute(WorldContextObject, Handle, SessionsOptions);
}

EOSResult UCoreSessions::EOSSessionModificationAddAttributeInt64(UObject* WorldContextObject, const FEOSHSessionModification& Handle, EEOSESessionAttributeAdvertisementType AdvertisementType, const FString& Key, const FString& Value)
{
	LogVerbose("");

	FEOSSessionModificationAddAttributeOptions SessionsOptions;

	SessionsOptions.AdvertisementType = AdvertisementType;

	const FTCHARToUTF8 ConvertedKey(*Key);
	EOS_Sessions_AttributeData AttributeData = {};
	AttributeData.ApiVersion = EOS_SESSIONS_SESSIONATTRIBUTEDATA_API_LATEST;
	AttributeData.Key = ConvertedKey.Get();
	AttributeData.Value.AsInt64 = FCString::Atoi64(*Value);
	AttributeData.ValueType = EOS_ESessionAttributeType::EOS_AT_INT64;

	SessionsOptions.Data.AttributeData = AttributeData;

	return EOSSessionModificationAddAttribute(WorldContextObject, Handle, SessionsOptions);
}

EOSResult UCoreSessions::EOSSessionModificationAddAttributeString(UObject* WorldContextObject, const FEOSHSessionModification& Handle, EEOSESessionAttributeAdvertisementType AdvertisementType, const FString& Key, const FString& Value)
{
	LogVerbose("");

	FEOSSessionModificationAddAttributeOptions SessionsOptions;

	SessionsOptions.AdvertisementType = AdvertisementType;

	const FTCHARToUTF8 ConvertedKey(*Key);
	const FTCHARToUTF8 ConvertedValue(*Value);
	
	EOS_Sessions_AttributeData AttributeData = {};
	AttributeData.ApiVersion = EOS_SESSIONS_SESSIONATTRIBUTEDATA_API_LATEST;
	AttributeData.Key = ConvertedKey.Get();
	AttributeData.Value.AsUtf8 = ConvertedValue.Get();
	AttributeData.ValueType = EOS_ESessionAttributeType::EOS_AT_STRING;

	SessionsOptions.Data.AttributeData = AttributeData;

	return EOSSessionModificationAddAttribute(WorldContextObject, Handle, SessionsOptions);
}

EOSResult UCoreSessions::EOSSessionModificationRemoveAttribute(UObject* WorldContextObject, const FEOSHSessionModification& Handle, const FEOSSessionModificationRemoveAttributeOptions& Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetSessionsHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedKey(*Options.Key);

		EOS_SessionModification_RemoveAttributeOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONMODIFICATION_REMOVEATTRIBUTE_API_LATEST;
		SessionsOptions.Key = ConvertedKey.Get();

		Result = EOSHelpers::ToEOSCoreResult(EOS_SessionModification_RemoveAttribute(Handle, &SessionsOptions));
	}

	return Result;
}

EOSResult UCoreSessions::EOSActiveSessionCopyInfo(UObject* WorldContextObject, const FEOSHActiveSession& Handle, FEOSActiveSessionCopyInfoOptions Options, FEOSActiveSessionInfo& OutActiveSessionInfo)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_ActiveSession_CopyInfoOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_ACTIVESESSION_COPYINFO_API_LATEST;

		EOS_ActiveSession_Info* ActiveSessionInfoHandle = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_ActiveSession_CopyInfo(Handle, &SessionsOptions, &ActiveSessionInfoHandle));

		if (Result == EOSResult::EOS_Success)
		{
			OutActiveSessionInfo = *ActiveSessionInfoHandle;
		}
	}

	return Result;
}

int32 UCoreSessions::EOSActiveSessionGetRegisteredPlayerCount(UObject* WorldContextObject, const FEOSHActiveSession& Handle, FEOSActiveSessionGetRegisteredPlayerCountOptions Options)
{
	LogVerbose("");

	int32 Result = 0;

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_ActiveSession_GetRegisteredPlayerCountOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_ACTIVESESSION_GETREGISTEREDPLAYERCOUNT_API_LATEST;

		Result = EOS_ActiveSession_GetRegisteredPlayerCount(Handle, &SessionsOptions);
	}

	return Result;
}

FEOSProductUserId UCoreSessions::EOSActiveSessionGetRegisteredPlayerByIndex(UObject* WorldContextObject, const FEOSHActiveSession& Handle, FEOSActiveSessionGetRegisteredPlayerByIndexOptions Options)
{
	LogVerbose("");

	FEOSProductUserId NotificationId;

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_ActiveSession_GetRegisteredPlayerByIndexOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_ACTIVESESSION_GETREGISTEREDPLAYERBYINDEX_API_LATEST;
		SessionsOptions.PlayerIndex = static_cast<uint32_t>(Options.PlayerIndex);

		NotificationId = EOS_ActiveSession_GetRegisteredPlayerByIndex(Handle, &SessionsOptions);
	}

	return NotificationId;
}

EOSResult UCoreSessions::EOSSessionDetailsCopyInfo(UObject* WorldContextObject, const FEOSHSessionDetails& Handle, FEOSSessionDetailsCopyInfoOptions Options, FEOSSessionDetailsInfo& outSessionInfo)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_SessionDetails_CopyInfoOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONDETAILS_COPYINFO_API_LATEST;

		EOS_SessionDetails_Info* SessionInfoHandle = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_SessionDetails_CopyInfo(Handle, &SessionsOptions, &SessionInfoHandle));

		if (Result == EOSResult::EOS_Success)
		{
			outSessionInfo = *SessionInfoHandle;
		}
	}

	return Result;
}

int32 UCoreSessions::EOSSessionDetailsGetSessionAttributeCount(UObject* WorldContextObject, const FEOSHSessionDetails& Handle, FEOSSessionDetailsGetSessionAttributeCountOptions Options)
{
	LogVerbose("");

	int32 Result = 0;

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_SessionDetails_GetSessionAttributeCountOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONDETAILS_GETSESSIONATTRIBUTECOUNT_API_LATEST;

		Result = EOS_SessionDetails_GetSessionAttributeCount(Handle, &SessionsOptions);
	}

	return Result;
}

EOSResult UCoreSessions::EOSSessionDetailsCopySessionAttributeByIndex(UObject* WorldContextObject, const FEOSHSessionDetails& Handle, FEOSSessionDetailsCopySessionAttributeByIndexOptions Options, FEOSSessionDetailsAttribute& OutSessionAttribute)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_SessionDetails_CopySessionAttributeByIndexOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONDETAILS_COPYSESSIONATTRIBUTEBYINDEX_API_LATEST;
		SessionsOptions.AttrIndex = static_cast<uint32_t>(Options.AttrIndex);

		EOS_SessionDetails_Attribute* SessionAttributeHandle = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_SessionDetails_CopySessionAttributeByIndex(Handle, &SessionsOptions, &SessionAttributeHandle));

		if (Result == EOSResult::EOS_Success)
		{
			OutSessionAttribute = *SessionAttributeHandle;
		}
	}

	return Result;
}

EOSResult UCoreSessions::EOSSessionDetailsCopySessionAttributeByKey(UObject* WorldContextObject, const FEOSHSessionDetails& Handle, FEOSSessionDetailsCopySessionAttributeByKeyOptions Options, FEOSSessionDetailsAttribute& OutSessionAttribute)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetSessionsHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedKey(*Options.AttrKey);

		EOS_SessionDetails_CopySessionAttributeByKeyOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONDETAILS_COPYSESSIONATTRIBUTEBYKEY_API_LATEST;
		SessionsOptions.AttrKey = ConvertedKey.Get();

		EOS_SessionDetails_Attribute* SessionAttributeHandle = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_SessionDetails_CopySessionAttributeByKey(Handle, &SessionsOptions, &SessionAttributeHandle));

		if (Result == EOSResult::EOS_Success)
		{
			OutSessionAttribute = *SessionAttributeHandle;
		}
	}

	return Result;
}

EOSResult UCoreSessions::EOSSessionSearchSetSessionId(UObject* WorldContextObject, const FEOSHSessionSearch& Handle, FEOSSessionSearchSetSessionIdOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetSessionsHandle(WorldContextObject))
	{
		const FTCHARToUTF8 SessionId(*Options.SessionId);
		
		EOS_SessionSearch_SetSessionIdOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONSEARCH_SETSESSIONID_API_LATEST;
		SessionsOptions.SessionId = SessionId.Get();

		Result = EOSHelpers::ToEOSCoreResult(EOS_SessionSearch_SetSessionId(Handle, &SessionsOptions));
	}

	return Result;
}

EOSResult UCoreSessions::EOSSessionSearchSetTargetUserId(UObject* WorldContextObject, const FEOSHSessionSearch& Handle, FEOSSessionSearchSetTargetUserIdOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_SessionSearch_SetTargetUserIdOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONSEARCH_SETTARGETUSERID_API_LATEST;
		SessionsOptions.TargetUserId = Options.TargetUserId;

		Result = EOSHelpers::ToEOSCoreResult(EOS_SessionSearch_SetTargetUserId(Handle, &SessionsOptions));
	}

	return Result;
}

EOSResult UCoreSessions::EOSSessionSearchSetParameter(UObject* WorldContextObject, const FEOSHSessionSearch& Handle, const FEOSSessionSearchSetParameterOptions& Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_SessionSearch_SetParameterOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONSEARCH_SETPARAMETER_API_LATEST;
		SessionsOptions.Parameter = &Options.Parameter.AttributeData;
		SessionsOptions.ComparisonOp = static_cast<EOS_EOnlineComparisonOp>(Options.ComparisonOp);

		Result = EOSHelpers::ToEOSCoreResult(EOS_SessionSearch_SetParameter(Handle, &SessionsOptions));
	}

	return Result;
}

EOSResult UCoreSessions::EOSSessionSearchSetParameterBool(UObject* WorldContextObject, const FEOSHSessionSearch& Handle, FString Key, bool bValue, EEOSEComparisonOp ComparisonOp)
{
	LogVerbose("");

	FEOSSessionSearchSetParameterOptions SessionsOptions;

	const FTCHARToUTF8 ConvertedKey(*Key);

	EOS_Sessions_AttributeData AttributeData = {};
	AttributeData.ApiVersion = EOS_SESSIONS_SESSIONATTRIBUTEDATA_API_LATEST;
	AttributeData.Key = ConvertedKey.Get();
	AttributeData.Value.AsBool = bValue;
	AttributeData.ValueType = EOS_ESessionAttributeType::EOS_AT_BOOLEAN;

	SessionsOptions.ComparisonOp = ComparisonOp;
	SessionsOptions.Parameter.AttributeData = AttributeData;

	return EOSSessionSearchSetParameter(WorldContextObject, Handle, SessionsOptions);
}

EOSResult UCoreSessions::EOSSessionSearchSetParameterDouble(UObject* WorldContextObject, const FEOSHSessionSearch& Handle, FString Key, FString Value, EEOSEComparisonOp ComparisonOp)
{
	LogVerbose("");

	FEOSSessionSearchSetParameterOptions SessionsOptions;

	const FTCHARToUTF8 ConvertedKey(*Key);

	EOS_Sessions_AttributeData AttributeData = {};
	AttributeData.ApiVersion = EOS_SESSIONS_SESSIONATTRIBUTEDATA_API_LATEST;
	AttributeData.Key = ConvertedKey.Get();
	AttributeData.Value.AsDouble = FCString::Atod(*Value);
	AttributeData.ValueType = EOS_ESessionAttributeType::EOS_AT_DOUBLE;

	SessionsOptions.ComparisonOp = ComparisonOp;
	SessionsOptions.Parameter.AttributeData = AttributeData;

	return EOSSessionSearchSetParameter(WorldContextObject, Handle, SessionsOptions);
}

EOSResult UCoreSessions::EOSSessionSearchSetParameterInt64(UObject* WorldContextObject, const FEOSHSessionSearch& Handle, FString Key, FString Value, EEOSEComparisonOp ComparisonOp)
{
	LogVerbose("");

	FEOSSessionSearchSetParameterOptions SessionsOptions;

	const FTCHARToUTF8 ConvertedKey(*Key);

	EOS_Sessions_AttributeData AttributeData = {};
	AttributeData.ApiVersion = EOS_SESSIONS_SESSIONATTRIBUTEDATA_API_LATEST;
	AttributeData.Key = ConvertedKey.Get();
	AttributeData.Value.AsInt64 = FCString::Atoi64(*Value);
	AttributeData.ValueType = EOS_ESessionAttributeType::EOS_AT_INT64;

	SessionsOptions.ComparisonOp = ComparisonOp;
	SessionsOptions.Parameter.AttributeData = AttributeData;

	return EOSSessionSearchSetParameter(WorldContextObject, Handle, SessionsOptions);
}

EOSResult UCoreSessions::EOSSessionSearchSetParameterString(UObject* WorldContextObject, const FEOSHSessionSearch& Handle, FString Key, FString Value, EEOSEComparisonOp ComparisonOp)
{
	LogVerbose("");

	FEOSSessionSearchSetParameterOptions SessionsOptions;

	const FTCHARToUTF8 ConvertedKey(*Key);
	const FTCHARToUTF8 ConvertedValue(*Value);

	EOS_Sessions_AttributeData AttributeData = {};
	AttributeData.ApiVersion = EOS_SESSIONS_SESSIONATTRIBUTEDATA_API_LATEST;
	AttributeData.Key = ConvertedKey.Get();
	AttributeData.Value.AsUtf8 = ConvertedValue.Get();
	AttributeData.ValueType = EOS_ESessionAttributeType::EOS_AT_STRING;

	SessionsOptions.ComparisonOp = ComparisonOp;
	SessionsOptions.Parameter.AttributeData = AttributeData;

	return EOSSessionSearchSetParameter(WorldContextObject, Handle, SessionsOptions);
}

EOSResult UCoreSessions::EOSSessionSearchRemoveParameter(UObject* WorldContextObject, const FEOSHSessionSearch& Handle, FEOSSessionSearchRemoveParameterOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetSessionsHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedKey(*Options.Key);

		EOS_SessionSearch_RemoveParameterOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONSEARCH_REMOVEPARAMETER_API_LATEST;
		SessionsOptions.Key = ConvertedKey.Get();
		SessionsOptions.ComparisonOp = static_cast<EOS_EOnlineComparisonOp>(Options.ComparisonOp);

		Result = EOSHelpers::ToEOSCoreResult(EOS_SessionSearch_RemoveParameter(Handle, &SessionsOptions));
	}

	return Result;
}

EOSResult UCoreSessions::EOSSessionSearchSetMaxResults(UObject* WorldContextObject, const FEOSHSessionSearch& Handle, FEOSSessionSearchSetMaxResultsOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_SessionSearch_SetMaxResultsOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONSEARCH_SETMAXSEARCHRESULTS_API_LATEST;
		SessionsOptions.MaxSearchResults = static_cast<uint32_t>(Options.MaxSearchResults);

		Result = EOSHelpers::ToEOSCoreResult(EOS_SessionSearch_SetMaxResults(Handle, &SessionsOptions));
	}

	return Result;
}

void UCoreSessions::EOSSessionSearchFind(UObject* WorldContextObject, const FEOSHSessionSearch& Handle, FEOSSessionSearchFindOptions Options, const FOnSessionFindCallback& Callback)
{
	LogVerbose("");

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_SessionSearch_FindOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONSEARCH_FIND_API_LATEST;
		SessionsOptions.LocalUserId = Options.LocalUserId;

		FSessionFindCallback* CallbackObj = new FSessionFindCallback(Callback);
		EOS_SessionSearch_Find(Handle, &SessionsOptions, CallbackObj, Internal_OnSessionsFindCallback);
	}
}

int32 UCoreSessions::EOSSessionSearchGetSearchResultCount(UObject* WorldContextObject, const FEOSHSessionSearch& Handle, FEOSSessionSearchGetSearchResultCountOptions Options)
{
	LogVerbose("");

	int32 Result = 0;

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_SessionSearch_GetSearchResultCountOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONSEARCH_GETSEARCHRESULTCOUNT_API_LATEST;

		Result = EOS_SessionSearch_GetSearchResultCount(Handle, &SessionsOptions);
	}

	return Result;
}

EOSResult UCoreSessions::EOSSessionSearchCopySearchResultByIndex(UObject* WorldContextObject, const FEOSHSessionSearch& Handle, const FEOSSessionSearchCopySearchResultByIndexOptions& Options, FEOSHSessionDetails& OutSessionHandle)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetSessionsHandle(WorldContextObject))
	{
		EOS_SessionSearch_CopySearchResultByIndexOptions SessionsOptions = {};
		SessionsOptions.ApiVersion = EOS_SESSIONSEARCH_COPYSEARCHRESULTBYINDEX_API_LATEST;
		SessionsOptions.SessionIndex = static_cast<uint32_t>(Options.SessionIndex);

		EOS_HSessionDetails SessionHandle = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_SessionSearch_CopySearchResultByIndex(Handle, &SessionsOptions, &SessionHandle));

		if (Result == EOSResult::EOS_Success)
		{
			OutSessionHandle = SessionHandle;
		}
	}

	return Result;
}

void UCoreSessions::EOSSessionModificationRelease(UObject* WorldContextObject, const FEOSHSessionModification& SessionModificationHandle)
{
	LogVerbose("");

	EOS_SessionModification_Release(SessionModificationHandle);
}

void UCoreSessions::EOSActiveSessionRelease(UObject* WorldContextObject, const FEOSHActiveSession& ActiveSessionHandle)
{
	LogVerbose("");

	EOS_ActiveSession_Release(ActiveSessionHandle);
}

void UCoreSessions::EOSSessionDetailsRelease(UObject* WorldContextObject, const FEOSHSessionDetails& SessionHandle)
{
	LogVerbose("");

	EOS_SessionDetails_Release(SessionHandle);
}

void UCoreSessions::EOSSessionSearchRelease(UObject* WorldContextObject, const FEOSHSessionSearch& SessionSearchHandle)
{
	LogVerbose("");

	EOS_SessionSearch_Release(SessionSearchHandle);
}

void UCoreSessions::Internal_OnSessionsUpdateSessionCallback(const EOS_Sessions_UpdateSessionCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FUpdateSessionCallback* CallbackObj = static_cast<FUpdateSessionCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreSessions::Internal_OnSessionsDestroySessionCallback(const EOS_Sessions_DestroySessionCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FDestroySessionCallback* CallbackObj = static_cast<FDestroySessionCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreSessions::Internal_OnSessionsJoinSessionCallback(const EOS_Sessions_JoinSessionCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FJoinSessionCallback* CallbackObj = static_cast<FJoinSessionCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreSessions::Internal_OnSessionsStartSessionCallback(const EOS_Sessions_StartSessionCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FStartSessionCallback* CallbackObj = static_cast<FStartSessionCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreSessions::Internal_OnSessionsEndSessionCallback(const EOS_Sessions_EndSessionCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FEndSessionCallback* CallbackObj = static_cast<FEndSessionCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreSessions::Internal_OnSessionRegisterPlayersCallback(const EOS_Sessions_RegisterPlayersCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FRegisterPlayersCallback* CallbackObj = static_cast<FRegisterPlayersCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreSessions::Internal_OnSessionsUnregisterPlayersCallback(const EOS_Sessions_UnregisterPlayersCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FUnregisterPlayersCallback* CallbackObj = static_cast<FUnregisterPlayersCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreSessions::Internal_OnSessionsSendInviteCallback(const EOS_Sessions_SendInviteCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FSendInviteCallback* CallbackObj = static_cast<FSendInviteCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreSessions::Internal_OnSessionsRejectInviteCallback(const EOS_Sessions_RejectInviteCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FRejectInviteCallback* CallbackObj = static_cast<FRejectInviteCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreSessions::Internal_OnSessionsQueryInvitesCallback(const EOS_Sessions_QueryInvitesCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FQueryInvitesCallback* CallbackObj = static_cast<FQueryInvitesCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreSessions::Internal_OnSessionsFindCallback(const EOS_SessionSearch_FindCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FSessionFindCallback* CallbackObj = static_cast<FSessionFindCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}
