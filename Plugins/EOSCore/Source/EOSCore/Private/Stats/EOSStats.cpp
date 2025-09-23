/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "Stats/EOSStats.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLogging.h"

UCoreStats* UCoreStats::GetStats(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (GetStatsHandle(WorldContextObject))
		{
			if (UWorld* World = WorldContextObject->GetWorld())
			{
				if (UGameInstance* GameInstance = World->GetGameInstance())
				{
					return GameInstance->GetSubsystem<UCoreStats>();
				}
			}	
		}
	}
	return nullptr;
}

void UCoreStats::EOSStatsIngestStat(UObject* WorldContextObject, FEOSStatsIngestStatOptions Options, const FOnStatsIngestStatCompleteCallback& Callback)
{
	LogVerbose("");

	if (GetStatsHandle(WorldContextObject))
	{
		FIngestStatOperation IngestStatsOperation(Options.Stats.Num(), Options.LocalUserId, Options.TargetUserId);

		for (int32 i = 0; i < Options.Stats.Num(); i++)
		{
			FCStringAnsi::Strncpy(const_cast<char*>(IngestStatsOperation.PointerArray[i].StatName), TCHAR_TO_UTF8(*Options.Stats[i].StatName), (Options.Stats[i].StatName.Len() + 1));
			IngestStatsOperation.PointerArray[i].IngestAmount = Options.Stats[i].IngestAmount;
		}

		FIngestStatCompleteCallback* CallbackObj = new FIngestStatCompleteCallback(Callback);
		EOS_Stats_IngestStat(GetStatsHandle(WorldContextObject), &IngestStatsOperation, CallbackObj, Internal_OnIngestStatCallback);
	}
}

void UCoreStats::EOSStatsQueryStats(UObject* WorldContextObject, FEOSStatsQueryStatsOptions Options, const FOnStatsQueryStatsCompleteCallback& Callback)
{
	LogVerbose("");

	if (GetStatsHandle(WorldContextObject))
	{
		FQueryStatsOperation QueryStatsOperation(Options.StatNames.Num(), Options.LocalUserId, Options.TargetUserId, (Options.StartTime == "-1" || Options.StartTime.IsEmpty()) ? -1 : FCString::Atoi64(*Options.StartTime), (Options.EndTime == "-1" || Options.EndTime.IsEmpty()) ? -1 : FCString::Atoi64(*Options.EndTime));

		for (int32 i = 0; i < Options.StatNames.Num(); i++)
		{
			FCStringAnsi::Strncpy(QueryStatsOperation.PointerArray[i], TCHAR_TO_UTF8(*Options.StatNames[i]), (Options.StatNames[i].Len() + 1));
		}

		FQueryStatsCompleteCallback* CallbackObj = new FQueryStatsCompleteCallback(Callback);
		EOS_Stats_QueryStats(GetStatsHandle(WorldContextObject), &QueryStatsOperation, CallbackObj, Internal_OnQueryStatsCallback);
	}
}

int32 UCoreStats::EOSStatsGetStatsCount(UObject* WorldContextObject, FEOSStatsGetStatCountOptions Options)
{
	LogVerbose("");

	int32 Result = 0;

	if (GetStatsHandle(WorldContextObject))
	{
		EOS_Stats_GetStatCountOptions StatsOptions = {};
		StatsOptions.ApiVersion = Options.ApiVersion;
		StatsOptions.TargetUserId = Options.TargetUserId;

		Result = EOS_Stats_GetStatsCount(GetStatsHandle(WorldContextObject), &StatsOptions);
	}

	return Result;
}

EOSResult UCoreStats::EOSStatsCopyStatByIndex(UObject* WorldContextObject, FEOSStatsCopyStatByIndexOptions Options, FEOSStatsStat& OutStat)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetStatsHandle(WorldContextObject))
	{
		EOS_Stats_CopyStatByIndexOptions StatsOptions = {};
		StatsOptions.ApiVersion = Options.ApiVersion;
		StatsOptions.TargetUserId = Options.TargetUserId;
		StatsOptions.StatIndex = static_cast<uint32_t>(Options.StatIndex);

		EOS_Stats_Stat* Stat = nullptr;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Stats_CopyStatByIndex(GetStatsHandle(WorldContextObject), &StatsOptions, &Stat));

		if (Result == EOSResult::EOS_Success)
		{
			OutStat = *Stat;
		}

		EOS_Stats_Stat_Release(Stat);
	}

	return Result;
}

EOSResult UCoreStats::EOSStatsCopyStatByName(UObject* WorldContextObject, FEOSStatsCopyStatByNameOptions Options, FEOSStatsStat& OutStat)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetStatsHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedStatName(*Options.Name);
		
		EOS_Stats_CopyStatByNameOptions StatsOptions = {};
		StatsOptions.ApiVersion = Options.ApiVersion;
		StatsOptions.TargetUserId = Options.TargetUserId;
		StatsOptions.Name = ConvertedStatName.Get();

		EOS_Stats_Stat* Stat = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Stats_CopyStatByName(GetStatsHandle(WorldContextObject), &StatsOptions, &Stat));

		if (Result == EOSResult::EOS_Success)
		{
			OutStat = *Stat;
		}

		EOS_Stats_Stat_Release(Stat);
	}

	return Result;
}

void UCoreStats::Internal_OnIngestStatCallback(const EOS_Stats_IngestStatCompleteCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FIngestStatCompleteCallback* CallbackObj = static_cast<FIngestStatCompleteCallback*>(Data->ClientData);
	check(CallbackObj);

	if (CallbackObj) 
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreStats::Internal_OnQueryStatsCallback(const EOS_Stats_OnQueryStatsCompleteCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FQueryStatsCompleteCallback* CallbackObj = static_cast<FQueryStatsCompleteCallback*>(Data->ClientData);
	check(CallbackObj);

	if (CallbackObj) 
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}
