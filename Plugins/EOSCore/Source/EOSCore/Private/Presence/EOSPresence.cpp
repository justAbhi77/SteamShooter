/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "Presence/EOSPresence.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLogging.h"

void UCorePresence::Deinitialize()
{
	LogVerbose("");

	for (auto Element: m_PresenceChangedCallbacks)
	{
		EOS_Presence_RemoveNotifyOnPresenceChanged(GetPresenceHandle(this), Element.Key);
	}

	for (auto Element: m_PresenceOnJoinGameAcceptedCallbacks)
	{
		EOS_Presence_RemoveNotifyJoinGameAccepted(GetPresenceHandle(this), Element.Key);
	}

	m_PresenceChangedCallbacks.Empty();
	m_PresenceOnJoinGameAcceptedCallbacks.Empty();
}

UCorePresence* UCorePresence::GetPresence(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (GetPresenceHandle(WorldContextObject))
		{
			if (UWorld* World = WorldContextObject->GetWorld())
			{
				if (UGameInstance* GameInstance = World->GetGameInstance())
				{
					return GameInstance->GetSubsystem<UCorePresence>();
				}
			}	
		}
	}
	return nullptr;
}

void UCorePresence::EOSPresenceQueryPresence(UObject* WorldContextObject, FEOSPresenceQueryPresenceOptions Options, const FOnPresenceQueryPresenceCompleteCallback& Callback)
{
	LogVerbose("");

	if (GetPresenceHandle(WorldContextObject))
	{
		EOS_Presence_QueryPresenceOptions PresenceOptions = {Options.ApiVersion, Options.LocalUserId, Options.TargetUserId};
		FQueryPresenceCompleteCallback* CallbackObj = new FQueryPresenceCompleteCallback(Callback);
		EOS_Presence_QueryPresence(GetPresenceHandle(WorldContextObject), &PresenceOptions, CallbackObj, Internal_OnQueryPresenceCallback);
	}
}

bool UCorePresence::EOSPresenceHasPresence(UObject* WorldContextObject, FEOSPresenceHasPresenceOptions Options)
{
	LogVerbose("");

	if (GetPresenceHandle(WorldContextObject))
	{
		EOS_Presence_HasPresenceOptions PresenceOptions = {Options.ApiVersion, Options.LocalUserId, Options.TargetUserId};

		return (EOS_Presence_HasPresence(GetPresenceHandle(WorldContextObject), &PresenceOptions) > 0);
	}

	return false;
}

EOSResult UCorePresence::EOSPresenceCopyPresence(UObject* WorldContextObject, FEOSPresenceCopyPresenceOptions Options, FEOSPresenceInfo& OutPresence)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetPresenceHandle(WorldContextObject))
	{
		EOS_Presence_CopyPresenceOptions PresenceOptions = {};
		PresenceOptions.ApiVersion = Options.ApiVersion;
		PresenceOptions.LocalUserId = Options.LocalUserId;
		PresenceOptions.TargetUserId = Options.TargetUserId;

		EOS_Presence_Info* PresenceInfo = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Presence_CopyPresence(GetPresenceHandle(WorldContextObject), &PresenceOptions, &PresenceInfo));

		if (Result == EOSResult::EOS_Success)
		{
			OutPresence = *PresenceInfo;
			EOS_Presence_Info_Release(PresenceInfo);
		}
	}

	return Result;
}

EOSResult UCorePresence::EOSPresenceCreatePresenceModification(UObject* WorldContextObject, FEOSPresenceCreatePresenceModificationOptions Options, FEOSHPresenceModification& OutPresenceModificationHandle)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetPresenceHandle(WorldContextObject))
	{
		EOS_Presence_CreatePresenceModificationOptions PresenceOptions = {};
		PresenceOptions.ApiVersion = Options.ApiVersion;
		PresenceOptions.LocalUserId = Options.LocalUserId;

		EOS_HPresenceModification PresenceModification;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Presence_CreatePresenceModification(GetPresenceHandle(WorldContextObject), &PresenceOptions, &PresenceModification));

		if (Result == EOSResult::EOS_Success)
		{
			OutPresenceModificationHandle = PresenceModification;
		}
	}

	return Result;
}

void UCorePresence::EOSPresenceSetPresence(UObject* WorldContextObject, FEOSPresenceSetPresenceOptions Options, const FOnPresenceSetPresenceCompleteCallback& Callback)
{
	LogVerbose("");

	if (GetPresenceHandle(WorldContextObject))
	{
		EOS_Presence_SetPresenceOptions PresenceOptions = {};
		PresenceOptions.ApiVersion = Options.ApiVersion;
		PresenceOptions.LocalUserId = Options.LocalUserId;
		PresenceOptions.PresenceModificationHandle = Options.PresenceModificationHandle;

		FSetPresenceCompleteCallback* CallbackObj = new FSetPresenceCompleteCallback(Callback);
		EOS_Presence_SetPresence(GetPresenceHandle(WorldContextObject), &PresenceOptions, CallbackObj, Internal_OnSetPresenceCallback);
	}
}

FEOSNotificationId UCorePresence::EOSPresenceAddNotifyOnPresenceChanged(UObject* WorldContextObject, const FOnPresenceChangedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId NotificationId;

	if (GetPresenceHandle(WorldContextObject))
	{
		EOS_Presence_AddNotifyOnPresenceChangedOptions PresenceOptions = {};
		PresenceOptions.ApiVersion = EOS_PRESENCE_ADDNOTIFYONPRESENCECHANGED_API_LATEST;

		FPresenceChangedCallback* CallbackObj = new FPresenceChangedCallback(Callback);

		NotificationId = EOS_Presence_AddNotifyOnPresenceChanged(GetPresenceHandle(WorldContextObject), &PresenceOptions, CallbackObj, ([](const EOS_Presence_PresenceChangedCallbackInfo* Data)
		{
			const FPresenceChangedCallback* CallbackData = static_cast<FPresenceChangedCallback*>(Data->ClientData);
			CallbackData->m_Callback.ExecuteIfBound(*Data);
		}));

		GetPresence(WorldContextObject)->m_PresenceChangedCallbacks.Add(NotificationId, CallbackObj);
	}

	return NotificationId;
}

void UCorePresence::EOSPresenceRemoveNotifyOnPresenceChanged(UObject* WorldContextObject, FEOSNotificationId NotificationId)
{
	LogVerbose("");

	if (GetPresenceHandle(WorldContextObject))
	{
		EOS_Presence_RemoveNotifyOnPresenceChanged(GetPresenceHandle(WorldContextObject), NotificationId);
		GetPresence(WorldContextObject)->m_PresenceChangedCallbacks.Remove(NotificationId);
	}
}

FEOSNotificationId UCorePresence::EOSPresenceAddNotifyJoinGameAccepted(UObject* WorldContextObject, const FOnPresenceOnJoinGameAcceptedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId NotificationId;

	if (GetPresenceHandle(WorldContextObject))
	{
		const EOS_Presence_AddNotifyJoinGameAcceptedOptions PresenceOptions = {EOS_PRESENCE_ADDNOTIFYJOINGAMEACCEPTED_API_LATEST};

		FPresenceOnJoinGameAcceptedCallback* CallbackObj = new FPresenceOnJoinGameAcceptedCallback(Callback);

		NotificationId = EOS_Presence_AddNotifyJoinGameAccepted(GetPresenceHandle(WorldContextObject), &PresenceOptions, CallbackObj, ([](const EOS_Presence_JoinGameAcceptedCallbackInfo* Data)
		{
			const FPresenceOnJoinGameAcceptedCallback* CallbackData = static_cast<FPresenceOnJoinGameAcceptedCallback*>(Data->ClientData);
			CallbackData->m_Callback.ExecuteIfBound(*Data);
		}));

		GetPresence(WorldContextObject)->m_PresenceOnJoinGameAcceptedCallbacks.Add(NotificationId, CallbackObj);
	}

	return NotificationId;
}

void UCorePresence::EOSPresenceRemoveNotifyJoinGameAccepted(UObject* WorldContextObject, FEOSNotificationId Id)
{
	LogVerbose("");

	if (GetPresenceHandle(WorldContextObject))
	{
		EOS_Presence_RemoveNotifyJoinGameAccepted(GetPresenceHandle(WorldContextObject), Id);
		GetPresence(WorldContextObject)->m_PresenceOnJoinGameAcceptedCallbacks.Remove(Id);
	}
}

EOSResult UCorePresence::EOSPresenceGetJoinInfo(UObject* WorldContextObject, FEOSPresenceGetJoinInfoOptions Options, FString& OutInfo)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetPresenceHandle(WorldContextObject))
	{
		EOS_Presence_GetJoinInfoOptions PresenceOptions = {};
		PresenceOptions.ApiVersion = Options.ApiVersion;
		PresenceOptions.LocalUserId = Options.LocalUserId;

		char Buffer[EOS_PRESENCEMODIFICATION_JOININFO_MAX_LENGTH];
		int32_t Size = EOS_PRESENCEMODIFICATION_JOININFO_MAX_LENGTH;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Presence_GetJoinInfo(GetPresenceHandle(WorldContextObject), &PresenceOptions, Buffer, &Size));

		if (Result == EOSResult::EOS_Success)
		{
			OutInfo = Buffer;
		}
	}
	return Result;
}

EOSResult UCorePresence::EOSPresenceModificationSetStatus(UObject* WorldContextObject, const FEOSHPresenceModification& Handle, FEOSPresenceModificationSetStatusOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetPresenceHandle(WorldContextObject))
	{
		EOS_PresenceModification_SetStatusOptions PresenceOptions = {};
		PresenceOptions.ApiVersion = Options.ApiVersion;
		PresenceOptions.Status = static_cast<EOS_Presence_EStatus>(Options.Status);

		Result = EOSHelpers::ToEOSCoreResult(EOS_PresenceModification_SetStatus(Handle, &PresenceOptions));
	}

	return Result;
}

EOSResult UCorePresence::EOSPresenceModificationSetRawRichText(UObject* WorldContextObject, const FEOSHPresenceModification& Handle, FEOSPresenceModificationSetRawRichTextOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetPresenceHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedRichText(*Options.RichText);
		
		EOS_PresenceModification_SetRawRichTextOptions PresenceOptions = {};
		PresenceOptions.ApiVersion = Options.ApiVersion;
		PresenceOptions.RichText = ConvertedRichText.Get();

		Result = EOSHelpers::ToEOSCoreResult(EOS_PresenceModification_SetRawRichText(Handle, &PresenceOptions));
	}

	return Result;
}

struct FSetDataOperation : public EOS_PresenceModification_SetDataOptions
{
public:
	FSetDataOperation(int32 numRecords)
		: EOS_PresenceModification_SetDataOptions()
	{
		PointerArray.AddZeroed(numRecords);

		for (int32 i = 0; i < numRecords; i++)
		{
			PointerArray[i].ApiVersion = EOS_PRESENCEMODIFICATION_SETDATA_API_LATEST;
			PointerArray[i].Key = new char[256];
			PointerArray[i].Value = new char[256];
		}

		ApiVersion = EOS_PRESENCEMODIFICATION_SETDATA_API_LATEST;
		RecordsCount = numRecords;
		Records = PointerArray.GetData();
	}

	~FSetDataOperation()
	{
		for (int32 i = 0; i < PointerArray.Num(); i++)
		{
			delete[] PointerArray[i].Key;
			delete[] PointerArray[i].Value;
		}
	}

public:
	TArray<EOS_Presence_DataRecord> PointerArray;
};

EOSResult UCorePresence::EOSPresenceModificationSetData(UObject* WorldContextObject, const FEOSHPresenceModification& Handle, FEOSPresenceModificationSetDataOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetPresenceHandle(WorldContextObject))
	{
		FSetDataOperation SetDataOperation(Options.Records.Num());

		for (int32 i = 0; i < Options.Records.Num(); i++)
		{
			FCStringAnsi::Strncpy(const_cast<char*>(SetDataOperation.PointerArray[i].Key), TCHAR_TO_UTF8(*Options.Records[i].Key), (Options.Records[i].Key.Len() + 1));
			FCStringAnsi::Strncpy(const_cast<char*>(SetDataOperation.PointerArray[i].Value), TCHAR_TO_UTF8(*Options.Records[i].Value), (Options.Records[i].Value.Len() + 1));
		}

		Result = EOSHelpers::ToEOSCoreResult(EOS_PresenceModification_SetData(Handle, &SetDataOperation));
	}

	return Result;
}

EOSResult UCorePresence::EOSPresenceModificationDeleteData(UObject* WorldContextObject, const FEOSHPresenceModification& Handle, FEOSPresenceModificationDeleteDataOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetPresenceHandle(WorldContextObject))
	{
		TArray<EOS_PresenceModification_DataRecordId> Records;
		Records.AddZeroed(Options.Records.Num());

		for (int32 i = 0; i < Options.Records.Num(); i++)
		{
			Records[i].ApiVersion = EOS_PRESENCEMODIFICATION_DATARECORDID_API_LATEST;
			Records[i].Key = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(Records[i].Key), TCHAR_TO_UTF8(*Options.Records[i].Key), (Options.Records[i].Key.Len() + 1));
		}

		EOS_PresenceModification_DeleteDataOptions PresenceOptions = {};
		PresenceOptions.ApiVersion = Options.ApiVersion;
		PresenceOptions.RecordsCount = Options.Records.Num();
		PresenceOptions.Records = Records.GetData();

		Result = EOSHelpers::ToEOSCoreResult(EOS_PresenceModification_DeleteData(Handle, &PresenceOptions));

		for (int32 i = 0; i < Records.Num(); i++)
		{
			delete[] Records[i].Key;
		}
	}

	return Result;
}

EOSResult UCorePresence::EOSPresenceModificationSetJoinInfo(UObject* WorldContextObject, const FEOSHPresenceModification& Handle, FEOSPresenceModificationSetJoinInfoOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetPresenceHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedJoinInfo(*Options.JoinInfo);
		
		EOS_PresenceModification_SetJoinInfoOptions PresenceOptions = {};
		PresenceOptions.ApiVersion = Options.ApiVersion;
		PresenceOptions.JoinInfo = ConvertedJoinInfo.Get();

		Result = EOSHelpers::ToEOSCoreResult(EOS_PresenceModification_SetJoinInfo(Handle, &PresenceOptions));
	}

	return Result;
}

void UCorePresence::EOSPresenceModificationRelease(UObject* WorldContextObject, const FEOSHPresenceModification& PresenceModificationHandle)
{
	LogVerbose("");

	if (GetPresenceHandle(WorldContextObject))
	{
		EOS_PresenceModification_Release(PresenceModificationHandle);
	}
}

void UCorePresence::Internal_OnQueryPresenceCallback(const EOS_Presence_QueryPresenceCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FQueryPresenceCompleteCallback* CallbackObj = static_cast<FQueryPresenceCompleteCallback*>(Data->ClientData);
	check(CallbackObj);

	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCorePresence::Internal_OnSetPresenceCallback(const EOS_Presence_SetPresenceCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FSetPresenceCompleteCallback* CallbackObj = static_cast<FSetPresenceCompleteCallback*>(Data->ClientData);
	check(CallbackObj);

	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}

}
