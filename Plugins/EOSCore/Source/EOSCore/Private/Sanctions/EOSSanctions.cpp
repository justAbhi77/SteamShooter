/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "Sanctions/EOSSanctions.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLogging.h"

UCoreSanctions* UCoreSanctions::GetSanctions(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (GetSanctionssHandle(WorldContextObject))
		{
			if (UWorld* World = WorldContextObject->GetWorld())
			{
				if (UGameInstance* GameInstance = World->GetGameInstance())
				{
					return GameInstance->GetSubsystem<UCoreSanctions>();
				}
			}	
		}
	}
	return nullptr;
}

void UCoreSanctions::EOSSanctionsQueryActivePlayerSanctions(UObject* WorldContextObject, FEOSSanctionsQueryActivePlayerSanctionsOptions Options, const FOnQueryActivePlayerSanctionsCallback& Callback)
{
	LogVerbose("");

	if (GetSanctionssHandle(WorldContextObject))
	{
		EOS_Sanctions_QueryActivePlayerSanctionsOptions SanctionsOptions = {};
		SanctionsOptions.ApiVersion = Options.ApiVersion;
		SanctionsOptions.LocalUserId = Options.LocalUserId;
		SanctionsOptions.TargetUserId = Options.TargetUserId;

		FQueryActivePlayerSanctionsCallback* CallbackObj = new FQueryActivePlayerSanctionsCallback(Callback);
		EOS_Sanctions_QueryActivePlayerSanctions(GetSanctionssHandle(WorldContextObject), &SanctionsOptions, CallbackObj, Internal_OnQueryActivePlayerSanctionsCallback);
	}
}

int32 UCoreSanctions::EOSSanctionsGetPlayerSanctionCount(UObject* WorldContextObject, FEOSSanctionsGetPlayerSanctionCountOptions Options)
{
	LogVerbose("");

	int32 Result = 0;

	if (GetSanctionssHandle(WorldContextObject))
	{
		EOS_Sanctions_GetPlayerSanctionCountOptions SanctionsOptions = {};
		SanctionsOptions.ApiVersion = Options.ApiVersion;
		SanctionsOptions.TargetUserId = Options.TargetUserId;

		Result = EOS_Sanctions_GetPlayerSanctionCount(GetSanctionssHandle(WorldContextObject), &SanctionsOptions);
	}

	return Result;
}

EOSResult UCoreSanctions::EOSSanctionsCopyPlayerSanctionByIndex(UObject* WorldContextObject, FEOSSanctionsCopyPlayerSanctionByIndexOptions Options, FEOSSanctionsPlayerSanction& OutSanction)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_UnexpectedError;

	if (GetSanctionssHandle(WorldContextObject))
	{
		EOS_Sanctions_CopyPlayerSanctionByIndexOptions SanctionsOptions = {};
		SanctionsOptions.ApiVersion = Options.ApiVersion;
		SanctionsOptions.SanctionIndex = Options.SanctionIndex;
		SanctionsOptions.TargetUserId = Options.TargetUserId;

		EOS_Sanctions_PlayerSanction* SanctionData = nullptr;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Sanctions_CopyPlayerSanctionByIndex(GetSanctionssHandle(WorldContextObject), &SanctionsOptions, &SanctionData));

		if (Result == EOSResult::EOS_Success)
		{
			OutSanction = *SanctionData;
			EOS_Sanctions_PlayerSanction_Release(SanctionData);
		}
	}

	return Result;
}

void UCoreSanctions::Internal_OnQueryActivePlayerSanctionsCallback(const EOS_Sanctions_QueryActivePlayerSanctionsCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FQueryActivePlayerSanctionsCallback* CallbackObj = static_cast<FQueryActivePlayerSanctionsCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}
