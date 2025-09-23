/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "ProgressionSnapshot/EOSProgressionSnapshot.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLibrary.h"
#include "Core/EOSCoreLogging.h"

void UCoreProgressionSnapshot::Deinitialize()
{
	LogVerbose("");
}

UCoreProgressionSnapshot* UCoreProgressionSnapshot::GetProgressionSnapshot(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (GetProgressionSnapshotHandle(WorldContextObject))
		{
			if (UWorld* World = WorldContextObject->GetWorld())
			{
				if (UGameInstance* GameInstance = World->GetGameInstance())
				{
					return GameInstance->GetSubsystem<UCoreProgressionSnapshot>();
				}
			}	
		}
	}
	return nullptr;
}

EOSResult UCoreProgressionSnapshot::EOSProgressionSnapshotBeginSnapshot(UObject* WorldContextObject, FEOSProgressionSnapshotBeginSnapshotOptions Options, int32& OutSnapshotId)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_UnexpectedError;
	OutSnapshotId = 0;

	if (GetProgressionSnapshotHandle(WorldContextObject))
	{
		EOS_ProgressionSnapshot_BeginSnapshotOptions ProgressionSnapshotOptions = {};
		ProgressionSnapshotOptions.ApiVersion = EOS_PROGRESSIONSNAPSHOT_BEGINSNAPSHOT_API_LATEST;
		ProgressionSnapshotOptions.LocalUserId = Options.LocalUserId;

		uint32_t* SnapshotId = nullptr;

		Result = EOSHelpers::ToEOSCoreResult(EOS_ProgressionSnapshot_BeginSnapshot(GetProgressionSnapshotHandle(WorldContextObject), &ProgressionSnapshotOptions, SnapshotId));

		if (Result == EOSResult::EOS_Success)
		{
			OutSnapshotId = *SnapshotId;	
		}
	}

	return Result;
}

EOSResult UCoreProgressionSnapshot::EOSProgressionSnapshotAddProgression(UObject* WorldContextObject, FEOSProgressionSnapshotAddProgressionOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_UnexpectedError;

	if (GetProgressionSnapshotHandle(WorldContextObject))
	{
		const FTCHARToUTF8 Key(*Options.Key);
		const FTCHARToUTF8 Value(*Options.Value);
		
		EOS_ProgressionSnapshot_AddProgressionOptions ProgressionSnapshotOptions = {};
		ProgressionSnapshotOptions.ApiVersion = EOS_PROGRESSIONSNAPSHOT_ADDPROGRESSION_API_LATEST;
		ProgressionSnapshotOptions.SnapshotId = Options.SnapshotId;
		ProgressionSnapshotOptions.Key = Key.Get();
		ProgressionSnapshotOptions.Value = Value.Get();

		uint32_t* SnapshotId = nullptr;

		Result = EOSHelpers::ToEOSCoreResult(EOS_ProgressionSnapshot_AddProgression(GetProgressionSnapshotHandle(WorldContextObject), &ProgressionSnapshotOptions));

		delete[] ProgressionSnapshotOptions.Key;
		delete[] ProgressionSnapshotOptions.Value;
	}

	return Result;
}

void UCoreProgressionSnapshot::EOSProgressionSnapshotSubmitSnapshot(UObject* WorldContextObject, FEOSProgressionSnapshotSubmitSnapshotOptions Options, const FOnSubmitSnapshotCallback& Callback)
{
	LogVerbose("");

	if (GetProgressionSnapshotHandle(WorldContextObject))
	{
		EOS_ProgressionSnapshot_SubmitSnapshotOptions ProgressionSnapshotOptions = {};
		ProgressionSnapshotOptions.ApiVersion = EOS_PROGRESSIONSNAPSHOT_SUBMITSNAPSHOT_API_LATEST;
		ProgressionSnapshotOptions.SnapshotId = Options.SnapshotId;
		
		FSubmitSnapshotCallback* CallbackObj = new FSubmitSnapshotCallback(Callback);
		EOS_ProgressionSnapshot_SubmitSnapshot(GetProgressionSnapshotHandle(WorldContextObject), &ProgressionSnapshotOptions, CallbackObj, Internal_OnSubmitSnapshotCallback);
	}
}

EOSResult UCoreProgressionSnapshot::EOSProgressionSnapshotEndSnapshot(UObject* WorldContextObject, FEOSProgressionSnapshotEndSnapshotOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_UnexpectedError;

	if (GetProgressionSnapshotHandle(WorldContextObject))
	{
		EOS_ProgressionSnapshot_EndSnapshotOptions ProgressionSnapshotOptions = {};
		ProgressionSnapshotOptions.ApiVersion = EOS_PROGRESSIONSNAPSHOT_ENDSNAPSHOT_API_LATEST;
		ProgressionSnapshotOptions.SnapshotId = Options.SnapshotId;

		Result = EOSHelpers::ToEOSCoreResult(EOS_ProgressionSnapshot_EndSnapshot(GetProgressionSnapshotHandle(WorldContextObject), &ProgressionSnapshotOptions));
	}

	return Result;
}

void UCoreProgressionSnapshot::EOSProgressionSnapshotDeleteSnapshot(UObject* WorldContextObject, FEOSProgressionSnapshotDeleteSnapshotOptions Options, const FOnDeleteSnapshotCallback& Callback)
{
	LogVerbose("");

	if (GetProgressionSnapshotHandle(WorldContextObject))
	{
		EOS_ProgressionSnapshot_DeleteSnapshotOptions ProgressionSnapshotOptions = {};
		ProgressionSnapshotOptions.ApiVersion = EOS_PROGRESSIONSNAPSHOT_DELETESNAPSHOT_API_LATEST;
		ProgressionSnapshotOptions.LocalUserId = Options.LocalUserId;
		
		FDeleteSnapshotCallback* CallbackObj = new FDeleteSnapshotCallback(Callback);
		EOS_ProgressionSnapshot_DeleteSnapshot(GetProgressionSnapshotHandle(WorldContextObject), &ProgressionSnapshotOptions, CallbackObj, Internal_OnDeleteSnapshotCallback);
	}
}

void UCoreProgressionSnapshot::Internal_OnSubmitSnapshotCallback(const EOS_ProgressionSnapshot_SubmitSnapshotCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FSubmitSnapshotCallback* CallbackObj = static_cast<FSubmitSnapshotCallback*>(Data->ClientData);
	check(CallbackObj);

	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreProgressionSnapshot::Internal_OnDeleteSnapshotCallback(const EOS_ProgressionSnapshot_DeleteSnapshotCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FDeleteSnapshotCallback* CallbackObj = static_cast<FDeleteSnapshotCallback*>(Data->ClientData);
	check(CallbackObj);

	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}
