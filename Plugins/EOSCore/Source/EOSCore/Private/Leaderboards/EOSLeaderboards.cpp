/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "Leaderboards/EOSLeaderboards.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLogging.h"

void UCoreLeaderboards::Deinitialize()
{
	LogVerbose("");
}

UCoreLeaderboards* UCoreLeaderboards::GetLeaderboards(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (GetLeaderboardsHandle(WorldContextObject))
		{
			if (UWorld* World = WorldContextObject->GetWorld())
			{
				if (UGameInstance* GameInstance = World->GetGameInstance())
				{
					return GameInstance->GetSubsystem<UCoreLeaderboards>();
				}
			}	
		}
	}
	return nullptr;
}

void UCoreLeaderboards::EOSLeaderboardsQueryLeaderboardDefinitions(UObject* WorldContextObject, FEOSLeaderboardsQueryLeaderboardDefinitionsOptions Options, const FOnQueryLeaderboardDefinitionsCompleteCallback& Callback)
{
	LogVerbose("");

	if (GetLeaderboardsHandle(WorldContextObject))
	{
		EOS_Leaderboards_QueryLeaderboardDefinitionsOptions LeaderboardsOptions = { };
		LeaderboardsOptions.ApiVersion = Options.ApiVersion;
		LeaderboardsOptions.StartTime = FCString::Atoi64(*Options.StartTime);
		LeaderboardsOptions.EndTime = FCString::Atoi64(*Options.EndTime);
		LeaderboardsOptions.LocalUserId = Options.LocalUserId;

		FLeaderboardDefinitionsCompleteCallback* CallbackObj = new FLeaderboardDefinitionsCompleteCallback(Callback);
		EOS_Leaderboards_QueryLeaderboardDefinitions(GetLeaderboardsHandle(WorldContextObject), &LeaderboardsOptions, CallbackObj, Internal_OnQueryLeaderboardDefinitionsCallback);
	}
}

int32 UCoreLeaderboards::EOSLeaderboardsGetLeaderboardDefinitionCount(UObject* WorldContextObject, FEOSLeaderboardsGetLeaderboardDefinitionCountOptions Options)
{
	LogVerbose("");

	int32 Result = 0;

	if (GetLeaderboardsHandle(WorldContextObject))
	{
		EOS_Leaderboards_GetLeaderboardDefinitionCountOptions LeaderboardsOptions = { };
		LeaderboardsOptions.ApiVersion = Options.ApiVersion;

		Result = EOS_Leaderboards_GetLeaderboardDefinitionCount(GetLeaderboardsHandle(WorldContextObject), &LeaderboardsOptions);
	}

	return Result;
}

EOSResult UCoreLeaderboards::EOSLeaderboardsCopyLeaderboardDefinitionByIndex(UObject* WorldContextObject, FEOSLeaderboardsCopyLeaderboardDefinitionByIndexOptions Options, FEOSLeaderboardsDefinition& OutLeaderboardDefinition)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLeaderboardsHandle(WorldContextObject))
	{
		EOS_Leaderboards_CopyLeaderboardDefinitionByIndexOptions LeaderboardsOptions = { };
		LeaderboardsOptions.ApiVersion = Options.ApiVersion;
		LeaderboardsOptions.LeaderboardIndex = static_cast<uint32_t>(Options.LeaderboardIndex);

		EOS_Leaderboards_Definition* Definition = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Leaderboards_CopyLeaderboardDefinitionByIndex(GetLeaderboardsHandle(WorldContextObject), &LeaderboardsOptions, &Definition));

		if (Result == EOSResult::EOS_Success)
		{
			OutLeaderboardDefinition = *Definition;
		}

		EOS_Leaderboards_Definition_Release(Definition);
	}

	return Result;
}

EOSResult UCoreLeaderboards::EOSLeaderboardsCopyLeaderboardDefinitionByLeaderboardId(UObject* WorldContextObject, FEOSLeaderboardsCopyLeaderboardDefinitionByLeaderboardIdOptions Options, FEOSLeaderboardsDefinition& OutLeaderboardDefinition)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLeaderboardsHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedLeaderboardId(*Options.LeaderboardId);

		EOS_Leaderboards_CopyLeaderboardDefinitionByLeaderboardIdOptions LeaderboardsOptions = { };
		LeaderboardsOptions.ApiVersion = Options.ApiVersion;
		LeaderboardsOptions.LeaderboardId = ConvertedLeaderboardId.Get();

		EOS_Leaderboards_Definition* Definition = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Leaderboards_CopyLeaderboardDefinitionByLeaderboardId(GetLeaderboardsHandle(WorldContextObject), &LeaderboardsOptions, &Definition));

		if (Result == EOSResult::EOS_Success)
		{
			OutLeaderboardDefinition = *Definition;
		}

		EOS_Leaderboards_Definition_Release(Definition);
	}

	return Result;
}

void UCoreLeaderboards::EOSLeaderboardsQueryLeaderboardRanks(UObject* WorldContextObject, FEOSLeaderboardsQueryLeaderboardRanksOptions Options, const FOnQueryLeaderboardRanksCompleteCallback& Callback)
{
	LogVerbose("");

	if (GetLeaderboardsHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedLeaderboardId(*Options.LeaderboardId);

		EOS_Leaderboards_QueryLeaderboardRanksOptions LeaderboardsOptions = { };
		LeaderboardsOptions.ApiVersion = Options.ApiVersion;
		LeaderboardsOptions.LeaderboardId = ConvertedLeaderboardId.Get();
		LeaderboardsOptions.LocalUserId = Options.LocalUserId;

		FLeaderboardRanksCompleteCallback* CallbackObj = new FLeaderboardRanksCompleteCallback(Callback);
		EOS_Leaderboards_QueryLeaderboardRanks(GetLeaderboardsHandle(WorldContextObject), &LeaderboardsOptions, CallbackObj, Internal_OnQueryLeaderboardRanksCallback);
	}
}

int32 UCoreLeaderboards::EOSLeaderboardsGetLeaderboardRecordCount(UObject* WorldContextObject, FEOSLeaderboardsGetLeaderboardRecordCountOptions Options)
{
	LogVerbose("");

	int32 Result = 0;

	if (GetLeaderboardsHandle(WorldContextObject))
	{
		EOS_Leaderboards_GetLeaderboardRecordCountOptions LeaderboardsOptions = { };
		LeaderboardsOptions.ApiVersion = Options.ApiVersion;

		Result = EOS_Leaderboards_GetLeaderboardRecordCount(GetLeaderboardsHandle(WorldContextObject), &LeaderboardsOptions);
	}
	return Result;
}

EOSResult UCoreLeaderboards::EOSLeaderboardsCopyLeaderboardRecordByIndex(UObject* WorldContextObject, FEOSLeaderboardsCopyLeaderboardRecordByIndexOptions Options, FEOSLeaderboardsLeaderboardRecord& OutLeaderboardRecord)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLeaderboardsHandle(WorldContextObject))
	{
		EOS_Leaderboards_CopyLeaderboardRecordByIndexOptions LeaderboardsOptions = { };
		LeaderboardsOptions.ApiVersion = Options.ApiVersion;
		LeaderboardsOptions.LeaderboardRecordIndex = static_cast<uint32_t>(Options.LeaderboardRecordIndex);

		EOS_Leaderboards_LeaderboardRecord* LeaderboardRecord = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Leaderboards_CopyLeaderboardRecordByIndex(GetLeaderboardsHandle(WorldContextObject), &LeaderboardsOptions, &LeaderboardRecord));

		if (Result == EOSResult::EOS_Success)
		{
			OutLeaderboardRecord = *LeaderboardRecord;
		}

		EOS_Leaderboards_LeaderboardRecord_Release(LeaderboardRecord);
	}

	return Result;
}

EOSResult UCoreLeaderboards::EOSLeaderboardsCopyLeaderboardRecordByUserId(UObject* WorldContextObject, FEOSLeaderboardsCopyLeaderboardRecordByUserIdOptions Options, FEOSLeaderboardsLeaderboardRecord& OutLeaderboardRecord)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLeaderboardsHandle(WorldContextObject))
	{
		EOS_Leaderboards_CopyLeaderboardRecordByUserIdOptions LeaderboardsOptions = { };
		LeaderboardsOptions.ApiVersion = Options.ApiVersion;
		LeaderboardsOptions.UserId = Options.UserId;

		EOS_Leaderboards_LeaderboardRecord* LeaderboardRecord = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Leaderboards_CopyLeaderboardRecordByUserId(GetLeaderboardsHandle(WorldContextObject), &LeaderboardsOptions, &LeaderboardRecord));

		if (Result == EOSResult::EOS_Success)
		{
			OutLeaderboardRecord = *LeaderboardRecord;
		}

		EOS_Leaderboards_LeaderboardRecord_Release(LeaderboardRecord);
	}
	return Result;
}

void UCoreLeaderboards::EOSLeaderboardsQueryLeaderboardUserScores(UObject* WorldContextObject, FEOSLeaderboardsQueryLeaderboardUserScoresOptions Options, const FOnQueryLeaderboardUserScoresCompleteCallback& Callback)
{
	LogVerbose("");

	if (GetLeaderboardsHandle(WorldContextObject))
	{
		FQueryLeaderboardUserScoresOperation QueryLeaderboardUserScoresOperation(Options.UserIds.Num(), Options.UserIds.Num(), FCString::Atoi(*Options.StartTime), FCString::Atoi(*Options.EndTime));
		QueryLeaderboardUserScoresOperation.LocalUserId = Options.LocalUserId;

		for (int32 i = 0; i < Options.UserIds.Num(); i++)
		{
			QueryLeaderboardUserScoresOperation.UserIdsArray[i] = Options.UserIds[i];

			for (int32 ix = 0; ix < Options.StatInfo.Num(); ix++)
			{
				FCStringAnsi::Strncpy(const_cast<char*>(QueryLeaderboardUserScoresOperation.UserScoresArray[i].StatName), TCHAR_TO_UTF8(*Options.StatInfo[ix].StatName), 256);
				QueryLeaderboardUserScoresOperation.UserScoresArray[i].Aggregation = static_cast<EOS_ELeaderboardAggregation>(Options.StatInfo[ix].Aggregation);
			}
		}

		FLeaderboardUserScoresCompleteCallback* CallbackObj = new FLeaderboardUserScoresCompleteCallback(Callback);
		EOS_Leaderboards_QueryLeaderboardUserScores(GetLeaderboardsHandle(WorldContextObject), &QueryLeaderboardUserScoresOperation, CallbackObj, Internal_OnQueryLeaderboardUserScoresCallback);
	}
}

int32 UCoreLeaderboards::EOSLeaderboardsGetLeaderboardUserScoreCount(UObject* WorldContextObject, FEOSLeaderboardsGetLeaderboardUserScoreCountOptions Options)
{
	LogVerbose("");

	int32 Result = 0;

	if (GetLeaderboardsHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedStatName(*Options.StatName);

		EOS_Leaderboards_GetLeaderboardUserScoreCountOptions LeaderboardsOptions = { };
		LeaderboardsOptions.ApiVersion = Options.ApiVersion;
		LeaderboardsOptions.StatName = ConvertedStatName.Get();

		Result = EOS_Leaderboards_GetLeaderboardUserScoreCount(GetLeaderboardsHandle(WorldContextObject), &LeaderboardsOptions);
	}

	return Result;
}

EOSResult UCoreLeaderboards::EOSLeaderboardsCopyLeaderboardUserScoreByIndex(UObject* WorldContextObject, FEOSLeaderboardsCopyLeaderboardUserScoreByIndexOptions Options, FEOSLeaderboardsLeaderboardUserScore& OutLeaderboardUserScore)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLeaderboardsHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedStatName(*Options.StatName);

		EOS_Leaderboards_CopyLeaderboardUserScoreByIndexOptions LeaderboardsOptions = { };
		LeaderboardsOptions.ApiVersion = Options.ApiVersion;
		LeaderboardsOptions.LeaderboardUserScoreIndex = static_cast<uint32_t>(Options.LeaderboardUserScoreIndex);
		LeaderboardsOptions.StatName = ConvertedStatName.Get();

		EOS_Leaderboards_LeaderboardUserScore* LeaderboardUserScores = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Leaderboards_CopyLeaderboardUserScoreByIndex(GetLeaderboardsHandle(WorldContextObject), &LeaderboardsOptions, &LeaderboardUserScores));

		if (Result == EOSResult::EOS_Success)
		{
			OutLeaderboardUserScore = *LeaderboardUserScores;
		}

		EOS_Leaderboards_LeaderboardUserScore_Release(LeaderboardUserScores);
	}
	return Result;
}

EOSResult UCoreLeaderboards::EOSLeaderboardsCopyLeaderboardUserScoreByUserId(UObject* WorldContextObject, FEOSLeaderboardsCopyLeaderboardUserScoreByUserIdOptions Options, FEOSLeaderboardsLeaderboardUserScore& OutLeaderboardUserScore)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetLeaderboardsHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedStatName(*Options.StatName);

		EOS_Leaderboards_CopyLeaderboardUserScoreByUserIdOptions LeaderboardsOptions = { };
		LeaderboardsOptions.ApiVersion = Options.ApiVersion;
		LeaderboardsOptions.UserId = Options.UserId;
		LeaderboardsOptions.StatName = ConvertedStatName.Get();

		EOS_Leaderboards_LeaderboardUserScore* LeaderboardUserScores = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Leaderboards_CopyLeaderboardUserScoreByUserId(GetLeaderboardsHandle(WorldContextObject), &LeaderboardsOptions, &LeaderboardUserScores));

		if (Result == EOSResult::EOS_Success)
		{
			OutLeaderboardUserScore = *LeaderboardUserScores;
		}

		EOS_Leaderboards_LeaderboardUserScore_Release(LeaderboardUserScores);
	}

	return Result;
}

void UCoreLeaderboards::Internal_OnQueryLeaderboardDefinitionsCallback(const EOS_Leaderboards_OnQueryLeaderboardDefinitionsCompleteCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FLeaderboardDefinitionsCompleteCallback* CallbackObj = static_cast<FLeaderboardDefinitionsCompleteCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreLeaderboards::Internal_OnQueryLeaderboardRanksCallback(const EOS_Leaderboards_OnQueryLeaderboardRanksCompleteCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FLeaderboardRanksCompleteCallback* CallbackObj = static_cast<FLeaderboardRanksCompleteCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreLeaderboards::Internal_OnQueryLeaderboardUserScoresCallback(const EOS_Leaderboards_OnQueryLeaderboardUserScoresCompleteCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FLeaderboardUserScoresCompleteCallback* CallbackObj = static_cast<FLeaderboardUserScoresCompleteCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}