/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "CustomInvites/EOSCustomInvites.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLogging.h"

void UCoreCustomInvites::Deinitialize()
{
	LogVerbose("");

	for (auto& Element : m_CustomInviteReceivedCallback)
	{
		EOS_CustomInvites_RemoveNotifyCustomInviteReceived(GetCustomInvitesHandle(this), Element.Key);
	}

	for (auto& Element : m_CustomInvitesOnCustomInviteAcceptedCallback)
	{
		EOS_CustomInvites_RemoveNotifyCustomInviteAccepted(GetCustomInvitesHandle(this), Element.Key);
	}
	
	m_CustomInviteReceivedCallback.Empty();
	m_CustomInvitesOnCustomInviteAcceptedCallback.Empty();
}

UCoreCustomInvites* UCoreCustomInvites::GetCustomInvites(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (GetCustomInvitesHandle(WorldContextObject))
		{
			if (UWorld* World = WorldContextObject->GetWorld())
			{
				if (UGameInstance* GameInstance = World->GetGameInstance())
				{
					return GameInstance->GetSubsystem<UCoreCustomInvites>();
				}
			}	
		}
	}
	return nullptr;
}

FEOSNotificationId UCoreCustomInvites::EOSCustomInvitesAddNotifyCustomInviteRejected(UObject* WorldContextObject, FEOSCustomInvitesAddNotifyCustomInviteRejectedOptions Options, const FOnCustomInvitesOnCustomInviteRejectedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId NotificationId = {};

	if (GetCustomInvitesHandle(WorldContextObject))
	{
		EOS_CustomInvites_AddNotifyCustomInviteRejectedOptions CustomInvitesOptions = {  };
		CustomInvitesOptions.ApiVersion = EOS_CUSTOMINVITES_ADDNOTIFYCUSTOMINVITEREJECTED_API_LATEST;

		FCustomInvitesOnCustomInviteRejectedCallback* CallbackObj = new FCustomInvitesOnCustomInviteRejectedCallback(Callback);
		NotificationId = EOS_CustomInvites_AddNotifyCustomInviteRejected(GetCustomInvitesHandle(WorldContextObject), &CustomInvitesOptions, CallbackObj, [](const EOS_CustomInvites_CustomInviteRejectedCallbackInfo* Data)
		{
			const FCustomInvitesOnCustomInviteRejectedCallback* CallbackObj = static_cast<FCustomInvitesOnCustomInviteRejectedCallback*>(Data->ClientData);
			check(CallbackObj);
			if (CallbackObj)
			{
				CallbackObj->m_Callback.ExecuteIfBound(*Data);
			}
		});
		GetCustomInvites(WorldContextObject)->m_CustomInvitesOnCustomInviteRejectedCallback.Add(NotificationId, CallbackObj);
	}
	
	return NotificationId;
}

void UCoreCustomInvites::EOSCustomInvitesRemoveNotifyCustomInviteRejected(UObject* WorldContextObject, FEOSNotificationId Id)
{
	LogVerbose("");

	if (GetCustomInvites(WorldContextObject))
	{
		EOS_CustomInvites_RemoveNotifyCustomInviteReceived(GetCustomInvitesHandle(WorldContextObject), Id);
		GetCustomInvites(WorldContextObject)->m_CustomInvitesOnCustomInviteRejectedCallback.Remove(Id);
	}
}

EOSResult UCoreCustomInvites::EOSCustomInvitesSetCustomInvite(UObject* WorldContextObject, FEOSCustomInvitesSetCustomInviteOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;
	
	if (GetCustomInvitesHandle(WorldContextObject))
	{
		const FTCHARToUTF8 Payload(*Options.Payload);
		
		EOS_CustomInvites_SetCustomInviteOptions CustomInvitesOptions = { };
		CustomInvitesOptions.ApiVersion = EOS_CUSTOMINVITES_SETCUSTOMINVITE_API_LATEST;
		CustomInvitesOptions.LocalUserId = Options.LocalUserId;
		CustomInvitesOptions.Payload = Payload.Get();

		Result = EOSHelpers::ToEOSCoreResult(EOS_CustomInvites_SetCustomInvite(GetCustomInvitesHandle(WorldContextObject), &CustomInvitesOptions));
	}

	return Result;
}

void UCoreCustomInvites::EOSCustomInvitesSendCustomInvite(UObject* WorldContextObject, FEOSCustomInvitesSendCustomInviteOptions Options, const FOnCustomInvitesOnSendCustomInviteCallback& Callback)
{
	LogVerbose("");

	if (GetCustomInvitesHandle(WorldContextObject))
	{
		FCustomInvitesSendCustomInvite CustomInvitesOptions = { Options.LocalUserId, Options.TargetUserIds };
		CustomInvitesOptions.ApiVersion = EOS_CUSTOMINVITES_SETCUSTOMINVITE_API_LATEST;

		FCustomInvitesSendCustomInviteCallback* CallbackObj = new FCustomInvitesSendCustomInviteCallback(Callback);
		EOS_CustomInvites_SendCustomInvite(GetCustomInvitesHandle(WorldContextObject), &CustomInvitesOptions, CallbackObj, Internal_OnSendCustomInviteCallback);
	}
}

FEOSNotificationId UCoreCustomInvites::EOSCustomInvitesAddNotifyCustomInviteReceived(UObject* WorldContextObject, FEOSCustomInvitesAddNotifyCustomInviteReceivedOptions Options, const FOnCustomInvitesOnCustomInviteReceivedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId NotificationId = {};

	if (GetCustomInvitesHandle(WorldContextObject))
	{
		EOS_CustomInvites_AddNotifyCustomInviteReceivedOptions CustomInvitesOptions = {  };
		CustomInvitesOptions.ApiVersion = EOS_CUSTOMINVITES_ADDNOTIFYCUSTOMINVITERECEIVED_API_LATEST;

		FCustomInvitesCustomInviteReceivedCallback* CallbackObj = new FCustomInvitesCustomInviteReceivedCallback(Callback);
		NotificationId = EOS_CustomInvites_AddNotifyCustomInviteReceived(GetCustomInvitesHandle(WorldContextObject), &CustomInvitesOptions, CallbackObj, [](const EOS_CustomInvites_OnCustomInviteReceivedCallbackInfo* Data)
		{
			const FCustomInvitesCustomInviteReceivedCallback* CallbackObj = static_cast<FCustomInvitesCustomInviteReceivedCallback*>(Data->ClientData);
			check(CallbackObj);
			if (CallbackObj)
			{
				CallbackObj->m_Callback.ExecuteIfBound(*Data);
			}
		});
		GetCustomInvites(WorldContextObject)->m_CustomInviteReceivedCallback.Add(NotificationId, CallbackObj);
	}
	
	return NotificationId;
}

void UCoreCustomInvites::EOSCustomInvitesRemoveNotifyCustomInviteReceived(UObject* WorldContextObject, FEOSNotificationId Id)
{
	LogVerbose("");

	if (GetCustomInvites(WorldContextObject))
	{
		EOS_CustomInvites_RemoveNotifyCustomInviteReceived(GetCustomInvitesHandle(WorldContextObject), Id);
		GetCustomInvites(WorldContextObject)->m_CustomInviteReceivedCallback.Remove(Id);
	}
}

FEOSNotificationId UCoreCustomInvites::EOSCustomInvitesAddNotifyCustomInviteAccepted(UObject* WorldContextObject, const FOnCustomInvitesOnCustomInviteAcceptedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId NotificationId = {};

	if (GetCustomInvitesHandle(WorldContextObject))
	{
		EOS_CustomInvites_AddNotifyCustomInviteAcceptedOptions CustomInvitesOptions = {  };
		CustomInvitesOptions.ApiVersion = EOS_CUSTOMINVITES_ADDNOTIFYCUSTOMINVITEACCEPTED_API_LATEST;

		FCustomInvitesCustomInviteAcceptedCallback* CallbackObj = new FCustomInvitesCustomInviteAcceptedCallback(Callback);
		NotificationId = EOS_CustomInvites_AddNotifyCustomInviteAccepted(GetCustomInvitesHandle(WorldContextObject), &CustomInvitesOptions, CallbackObj, [](const EOS_CustomInvites_OnCustomInviteAcceptedCallbackInfo* Data)
		{
			const FCustomInvitesCustomInviteAcceptedCallback* CallbackObj = static_cast<FCustomInvitesCustomInviteAcceptedCallback*>(Data->ClientData);
			check(CallbackObj);
			if (CallbackObj)
			{
				CallbackObj->m_Callback.ExecuteIfBound(*Data);
			}
		});
		GetCustomInvites(WorldContextObject)->m_CustomInvitesOnCustomInviteAcceptedCallback.Add(NotificationId, CallbackObj);
	}
	
	return NotificationId;
}

void UCoreCustomInvites::EOSCustomInvitesRemoveNotifyCustomInviteAccepted(UObject* WorldContextObject, FEOSNotificationId Id)
{
	LogVerbose("");

	if (GetCustomInvites(WorldContextObject))
	{
		EOS_CustomInvites_RemoveNotifyCustomInviteAccepted(GetCustomInvitesHandle(WorldContextObject), Id);
		GetCustomInvites(WorldContextObject)->m_CustomInvitesOnCustomInviteAcceptedCallback.Remove(Id);
	}
}

EOSResult UCoreCustomInvites::EOSCustomInvitesFinalizeInvite(UObject* WorldContextObject, FEOSCustomInvitesFinalizeInviteOptions Options)
{
	LogVerbose("");
	
	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetCustomInvitesHandle(WorldContextObject))
	{
		EOS_CustomInvites_FinalizeInviteOptions CustomInvitesOptions = { };
		CustomInvitesOptions.ApiVersion = EOS_CUSTOMINVITES_FINALIZEINVITE_API_LATEST;
		CustomInvitesOptions.TargetUserId = Options.TargetUserId;
		CustomInvitesOptions.LocalUserId = Options.LocalUserId;
		CustomInvitesOptions.CustomInviteId = new char[512];
		FCStringAnsi::Strncpy(const_cast<char*>(CustomInvitesOptions.CustomInviteId), TCHAR_TO_UTF8(*Options.CustomInviteId), 512);

		CustomInvitesOptions.ProcessingResult = static_cast<EOS_EResult>(Options.ProcessingResult);

		Result = EOSHelpers::ToEOSCoreResult(EOS_CustomInvites_FinalizeInvite(GetCustomInvitesHandle(WorldContextObject), &CustomInvitesOptions));
	}
	
	return Result;
}

void UCoreCustomInvites::EOSCustomInvitesSendRequestToJoin(UObject* WorldContextObject, FEOSCustomInvitesSendRequestToJoinOptions Options, const FOnCustomInvitesOnSendRequestToJoinCallback& Callback)
{
	LogVerbose("");

	if (GetCustomInvitesHandle(WorldContextObject))
	{
		EOS_CustomInvites_SendRequestToJoinOptions CustomInvitesOptions = { };
		CustomInvitesOptions.ApiVersion = EOS_CUSTOMINVITES_SENDREQUESTTOJOIN_API_LATEST;
		CustomInvitesOptions.LocalUserId = Options.LocalUserId;
		CustomInvitesOptions.TargetUserId = Options.TargetUserId;

		FCustomInvitesOnSendRequestToJoinCallback* CallbackObj = new FCustomInvitesOnSendRequestToJoinCallback(Callback);
		EOS_CustomInvites_SendRequestToJoin(GetCustomInvitesHandle(WorldContextObject), &CustomInvitesOptions, CallbackObj, Internal_OnSendRequestToJoinCallback);
	}
}

FEOSNotificationId UCoreCustomInvites::EOSCustomInvitesAddNotifyRequestToJoinResponseReceived(UObject* WorldContextObject, const FOnCustomInvitesOnRequestToJoinResponseReceivedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId NotificationId = {};

	if (GetCustomInvitesHandle(WorldContextObject))
	{
		EOS_CustomInvites_AddNotifyRequestToJoinResponseReceivedOptions CustomInvitesOptions = {  };
		CustomInvitesOptions.ApiVersion = EOS_CUSTOMINVITES_ADDNOTIFYREQUESTTOJOINRESPONSERECEIVED_API_LATEST;

		FCustomInvitesOnRequestToJoinResponseReceivedCallback* CallbackObj = new FCustomInvitesOnRequestToJoinResponseReceivedCallback(Callback);
		NotificationId = EOS_CustomInvites_AddNotifyRequestToJoinResponseReceived(GetCustomInvitesHandle(WorldContextObject), &CustomInvitesOptions, CallbackObj, [](const EOS_CustomInvites_RequestToJoinResponseReceivedCallbackInfo* Data)
		{
			const FCustomInvitesOnRequestToJoinResponseReceivedCallback* CallbackObj = static_cast<FCustomInvitesOnRequestToJoinResponseReceivedCallback*>(Data->ClientData);
			check(CallbackObj);
			if (CallbackObj)
			{
				CallbackObj->m_Callback.ExecuteIfBound(*Data);
			}
		});
		GetCustomInvites(WorldContextObject)->m_CustomInvitesOnRequestToJoinResponseReceivedCallback.Add(NotificationId, CallbackObj);
	}
	
	return NotificationId;
}

void UCoreCustomInvites::EOSCustomInvitesRemoveNotifyRequestToJoinResponseReceived(UObject* WorldContextObject, FEOSNotificationId InId)
{
	LogVerbose("");

	if (GetCustomInvites(WorldContextObject))
	{
		EOS_CustomInvites_RemoveNotifyRequestToJoinResponseReceived(GetCustomInvitesHandle(WorldContextObject), InId);
		GetCustomInvites(WorldContextObject)->m_CustomInvitesOnRequestToJoinResponseReceivedCallback.Remove(InId);
	}
}

FEOSNotificationId UCoreCustomInvites::EOSCustomInvitesAddNotifyRequestToJoinReceived(UObject* WorldContextObject, const FOnCustomInvitesOnRequestToJoinReceivedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId NotificationId = {};

	if (GetCustomInvitesHandle(WorldContextObject))
	{
		EOS_CustomInvites_AddNotifyRequestToJoinReceivedOptions CustomInvitesOptions = {  };
		CustomInvitesOptions.ApiVersion = EOS_CUSTOMINVITES_ADDNOTIFYREQUESTTOJOINRECEIVED_API_LATEST;

		FCustomInvitesOnRequestToJoinReceivedCallback* CallbackObj = new FCustomInvitesOnRequestToJoinReceivedCallback(Callback);
		NotificationId = EOS_CustomInvites_AddNotifyRequestToJoinReceived(GetCustomInvitesHandle(WorldContextObject), &CustomInvitesOptions, CallbackObj, [](const EOS_CustomInvites_RequestToJoinReceivedCallbackInfo* Data)
		{
			const FCustomInvitesOnRequestToJoinReceivedCallback* CallbackObj = static_cast<FCustomInvitesOnRequestToJoinReceivedCallback*>(Data->ClientData);
			check(CallbackObj);
			if (CallbackObj)
			{
				CallbackObj->m_Callback.ExecuteIfBound(*Data);
			}
		});
		GetCustomInvites(WorldContextObject)->m_CustomInvitesOnRequestToJoinReceivedCallback.Add(NotificationId, CallbackObj);
	}
	
	return NotificationId;
}

void UCoreCustomInvites::EOSCustomInvitesRemoveNotifyRequestToJoinReceived(UObject* WorldContextObject, FEOSNotificationId InId)
{
	if (GetCustomInvites(WorldContextObject))
	{
		EOS_CustomInvites_RemoveNotifyRequestToJoinReceived(GetCustomInvitesHandle(WorldContextObject), InId);
		GetCustomInvites(WorldContextObject)->m_CustomInvitesOnRequestToJoinReceivedCallback.Remove(InId);
	}
}

FEOSNotificationId UCoreCustomInvites::EOSCustomInvitesAddNotifySendCustomNativeInviteRequested(UObject* WorldContextObject, const FOnCustomInvitesOnSendCustomNativeInviteRequestedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId NotificationId = {};

	if (GetCustomInvitesHandle(WorldContextObject))
	{
		EOS_CustomInvites_AddNotifySendCustomNativeInviteRequestedOptions CustomInvitesOptions = {  };
		CustomInvitesOptions.ApiVersion = EOS_CUSTOMINVITES_ADDNOTIFYSENDCUSTOMNATIVEINVITEREQUESTED_API_LATEST;

		FCustomInvitesOnSendCustomNativeInviteRequestedCallback* CallbackObj = new FCustomInvitesOnSendCustomNativeInviteRequestedCallback(Callback);
		NotificationId = EOS_CustomInvites_AddNotifySendCustomNativeInviteRequested(GetCustomInvitesHandle(WorldContextObject), &CustomInvitesOptions, CallbackObj, [](const EOS_CustomInvites_SendCustomNativeInviteRequestedCallbackInfo* Data)
		{
			const FCustomInvitesOnSendCustomNativeInviteRequestedCallback* CallbackObj = static_cast<FCustomInvitesOnSendCustomNativeInviteRequestedCallback*>(Data->ClientData);
			check(CallbackObj);
			if (CallbackObj)
			{
				CallbackObj->m_Callback.ExecuteIfBound(*Data);
			}
		});
		GetCustomInvites(WorldContextObject)->m_CustomInvitesOnSendCustomNativeInviteRequestedCallback.Add(NotificationId, CallbackObj);
	}
	
	return NotificationId;
}

void UCoreCustomInvites::EOSCustomInvitesRemoveNotifySendCustomNativeInviteRequested(UObject* WorldContextObject, FEOSNotificationId InId)
{
	if (GetCustomInvites(WorldContextObject))
	{
		EOS_CustomInvites_RemoveNotifySendCustomNativeInviteRequested(GetCustomInvitesHandle(WorldContextObject), InId);
		GetCustomInvites(WorldContextObject)->m_CustomInvitesOnSendCustomNativeInviteRequestedCallback.Remove(InId);
	}
}

FEOSNotificationId UCoreCustomInvites::EOSCustomInvitesAddNotifyRequestToJoinAccepted(UObject* WorldContextObject, const FOnCustomInvitesOnRequestToJoinAcceptedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId NotificationId = {};

	if (GetCustomInvitesHandle(WorldContextObject))
	{
		EOS_CustomInvites_AddNotifyRequestToJoinAcceptedOptions CustomInvitesOptions = {  };
		CustomInvitesOptions.ApiVersion = EOS_CUSTOMINVITES_ADDNOTIFYREQUESTTOJOINACCEPTED_API_LATEST;

		FCustomInvitesOnRequestToJoinAcceptedCallback* CallbackObj = new FCustomInvitesOnRequestToJoinAcceptedCallback(Callback);
		NotificationId = EOS_CustomInvites_AddNotifyRequestToJoinAccepted(GetCustomInvitesHandle(WorldContextObject), &CustomInvitesOptions, CallbackObj, [](const EOS_CustomInvites_OnRequestToJoinAcceptedCallbackInfo* Data)
		{
			const FCustomInvitesOnRequestToJoinAcceptedCallback* CallbackObj = static_cast<FCustomInvitesOnRequestToJoinAcceptedCallback*>(Data->ClientData);
			check(CallbackObj);
			if (CallbackObj)
			{
				CallbackObj->m_Callback.ExecuteIfBound(*Data);
			}
		});
		GetCustomInvites(WorldContextObject)->m_CustomInvitesOnRequestToJoinAcceptedCallback.Add(NotificationId, CallbackObj);
	}
	
	return NotificationId;
}

void UCoreCustomInvites::EOSCustomInvitesRemoveNotifyRequestToJoinAccepted(UObject* WorldContextObject, FEOSNotificationId InId)
{
	if (GetCustomInvites(WorldContextObject))
	{
		EOS_CustomInvites_RemoveNotifyRequestToJoinAccepted(GetCustomInvitesHandle(WorldContextObject), InId);
		GetCustomInvites(WorldContextObject)->m_CustomInvitesOnRequestToJoinAcceptedCallback.Remove(InId);
	}
}

FEOSNotificationId UCoreCustomInvites::EOSCustomInvitesAddNotifyRequestToJoinRejected(UObject* WorldContextObject, const FOnCustomInvitesOnRequestToJoinRejectedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId NotificationId = {};

	if (GetCustomInvitesHandle(WorldContextObject))
	{
		EOS_CustomInvites_AddNotifyRequestToJoinRejectedOptions CustomInvitesOptions = {  };
		CustomInvitesOptions.ApiVersion = EOS_CUSTOMINVITES_ADDNOTIFYREQUESTTOJOINREJECTED_API_LATEST;

		FCustomInvitesOnRequestToJoinRejectedCallback* CallbackObj = new FCustomInvitesOnRequestToJoinRejectedCallback(Callback);
		NotificationId = EOS_CustomInvites_AddNotifyRequestToJoinRejected(GetCustomInvitesHandle(WorldContextObject), &CustomInvitesOptions, CallbackObj, [](const EOS_CustomInvites_OnRequestToJoinRejectedCallbackInfo* Data)
		{
			const FCustomInvitesOnRequestToJoinRejectedCallback* CallbackObj = static_cast<FCustomInvitesOnRequestToJoinRejectedCallback*>(Data->ClientData);
			check(CallbackObj);
			if (CallbackObj)
			{
				CallbackObj->m_Callback.ExecuteIfBound(*Data);
			}
		});
		GetCustomInvites(WorldContextObject)->m_CustomInvitesOnRequestToJoinRejectedCallback.Add(NotificationId, CallbackObj);
	}
	
	return NotificationId;
}

void UCoreCustomInvites::EOSCustomInvitesRemoveNotifyRequestToJoinRejected(UObject* WorldContextObject, FEOSNotificationId InId)
{
	if (GetCustomInvites(WorldContextObject))
	{
		EOS_CustomInvites_RemoveNotifyRequestToJoinRejected(GetCustomInvitesHandle(WorldContextObject), InId);
		GetCustomInvites(WorldContextObject)->m_CustomInvitesOnRequestToJoinRejectedCallback.Remove(InId);
	}
}

void UCoreCustomInvites::EOSCustomInvitesAcceptRequestToJoin(UObject* WorldContextObject, FEOSCustomInvitesAcceptRequestToJoinOptions Options, const FOnCustomInvitesOnAcceptRequestToJoinCallback Callback)
{
	LogVerbose("");

	if (GetCustomInvitesHandle(WorldContextObject))
	{
		EOS_CustomInvites_AcceptRequestToJoinOptions CustomInvitesOptions = { };
		CustomInvitesOptions.ApiVersion = EOS_CUSTOMINVITES_ACCEPTREQUESTTOJOIN_API_LATEST;
		CustomInvitesOptions.LocalUserId = Options.LocalUserId;
		CustomInvitesOptions.TargetUserId = Options.TargetUserId;

		FCustomInvitesOnAcceptRequestToJoinCallback* CallbackObj = new FCustomInvitesOnAcceptRequestToJoinCallback(Callback);
		EOS_CustomInvites_AcceptRequestToJoin(GetCustomInvitesHandle(WorldContextObject), &CustomInvitesOptions, CallbackObj, Internal_OnAcceptRequestToJoinCallback);
	}
}

void UCoreCustomInvites::EOSCustomInvitesRejectRequestToJoin(UObject* WorldContextObject, FEOSCustomInvitesRejectRequestToJoinOptions Options, const FOnCustomInvitesOnRejectRequestToJoinCallback& Callback)
{
	LogVerbose("");

	if (GetCustomInvitesHandle(WorldContextObject))
	{
		EOS_CustomInvites_RejectRequestToJoinOptions CustomInvitesOptions = { };
		CustomInvitesOptions.ApiVersion = EOS_CUSTOMINVITES_REJECTREQUESTTOJOIN_API_LATEST;
		CustomInvitesOptions.LocalUserId = Options.LocalUserId;
		CustomInvitesOptions.TargetUserId = Options.TargetUserId;

		FCustomInvitesOnRejectRequestToJoinCallback* CallbackObj = new FCustomInvitesOnRejectRequestToJoinCallback(Callback);
		EOS_CustomInvites_RejectRequestToJoin(GetCustomInvitesHandle(WorldContextObject), &CustomInvitesOptions, CallbackObj, Internal_OnRejectRequestToJoinCallback);
	}
}

void UCoreCustomInvites::Internal_OnSendCustomInviteCallback(const EOS_CustomInvites_SendCustomInviteCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FCustomInvitesSendCustomInviteCallback* CallbackObj = static_cast<FCustomInvitesSendCustomInviteCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreCustomInvites::Internal_OnSendRequestToJoinCallback(const EOS_CustomInvites_SendRequestToJoinCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FCustomInvitesOnSendRequestToJoinCallback* CallbackObj = static_cast<FCustomInvitesOnSendRequestToJoinCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreCustomInvites::Internal_OnAcceptRequestToJoinCallback(const EOS_CustomInvites_AcceptRequestToJoinCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FCustomInvitesOnAcceptRequestToJoinCallback* CallbackObj = static_cast<FCustomInvitesOnAcceptRequestToJoinCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreCustomInvites::Internal_OnRejectRequestToJoinCallback(const EOS_CustomInvites_RejectRequestToJoinCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FCustomInvitesOnRejectRequestToJoinCallback* CallbackObj = static_cast<FCustomInvitesOnRejectRequestToJoinCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}
