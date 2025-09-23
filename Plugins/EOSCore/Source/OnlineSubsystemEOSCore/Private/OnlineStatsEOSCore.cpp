/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "OnlineStatsEOSCore.h"
#include "OnlineSubsystemEOSCore.h"
#include "OnlineSubsystemTypesEOSCore.h"
#include "OnlineIdentityEOSCore.h"
#include "OnlineSubsystemEOSCorePrivatePCH.h"

#define FLOAT_STAT_SCALER 1000.f

inline int32 FloatStatToIntStat(float Value)
{
	return FMath::TruncToInt(Value * FLOAT_STAT_SCALER);
}

inline float IntStatToFloatStat(int32 Value)
{
	return (float)Value / FLOAT_STAT_SCALER;
}

struct FStatNameBuffer
{
	char StatName[EOSCORE_OSS_STRING_BUFFER_LENGTH + 1];
};


void FOnlineStatsEOSCore::QueryStats(const TSharedRef<const FUniqueNetId> LocalUserId, const TSharedRef<const FUniqueNetId> StatsUser, const FOnlineStatsQueryUserStatsComplete& Delegate)
{
	UE_LOG_ONLINE_STATS(Warning, TEXT("QueryStats() without a list of stats names to query is not supported"));
	Delegate.ExecuteIfBound(FOnlineError(EOnlineErrorResult::NotImplemented), TSharedPtr<const FOnlineStatsUserStats>());
}

struct FQueryStatsOptions :
	public EOS_Stats_QueryStatsOptions
{
	TArray<char*> PointerArray;

	FQueryStatsOptions(const uint32 InNumStatIds) :
		EOS_Stats_QueryStatsOptions()
	{
		ApiVersion = EOS_STATS_QUERYSTATS_API_LATEST;

		StartTime = EOS_STATS_TIME_UNDEFINED;
		EndTime = EOS_STATS_TIME_UNDEFINED;

		PointerArray.AddZeroed(InNumStatIds);
		for (int32 Index = 0; Index < PointerArray.Num(); Index++)
		{
			PointerArray[Index] = new char[EOSCORE_OSS_STRING_BUFFER_LENGTH + 1];
		}
		StatNames = (const char**)PointerArray.GetData();
		StatNamesCount = InNumStatIds;
	}

	~FQueryStatsOptions()
	{
		for (int32 Index = 0; Index < PointerArray.Num(); Index++)
		{
			delete [] PointerArray[Index];
		}
	}
};

typedef TEOSCallback<EOS_Stats_OnQueryStatsCompleteCallback, EOS_Stats_OnQueryStatsCompleteCallbackInfo> FReadStatsCallback;

struct FStatsQueryContext
{
	int32 NumPlayerReads;
	TArray<FString> StatNames;
	FOnlineStatsQueryUsersStatsComplete Delegate;
	/** Stats are added here as they come in from the service. They are added to the global cache at the end */
	TUniqueNetIdMap<TSharedRef<FOnlineStatsUserStats>> m_StatsCache;

	FStatsQueryContext(int32 InNumPlayerReads, const TArray<FString>& InStatNames, const FOnlineStatsQueryUsersStatsComplete& InDelegate)
		: NumPlayerReads(InNumPlayerReads)
		  , StatNames(InStatNames)
		  , Delegate(InDelegate)
	{
	}
};

typedef TSharedPtr<FStatsQueryContext> FStatsQueryContextPtr;

static void AppendStats(TUniqueNetIdMap<TSharedRef<FOnlineStatsUserStats>>& m_StatsCache, const TUniqueNetIdMap<TSharedRef<FOnlineStatsUserStats>>& m_StatsCacheToAppend)
{
	for (const TPair<TSharedRef<const FUniqueNetId>, TSharedRef<FOnlineStatsUserStats>>& StatsUser : m_StatsCacheToAppend)
	{
		TSharedRef<FOnlineStatsUserStats>* UserCachedStats = m_StatsCache.Find(StatsUser.Key);
		if (!UserCachedStats)
		{
			UserCachedStats = &m_StatsCache.Emplace(StatsUser.Key, MakeShared<FOnlineStatsUserStats>(StatsUser.Key));
		}

		for (TPair<FString, FOnlineStatValue>& NewStat : StatsUser.Value->Stats)
		{
			FOnlineStatValue& StatToUpdate = (*UserCachedStats)->Stats.FindOrAdd(NewStat.Key);
			StatToUpdate = NewStat.Value;
		}
	}
}

void FOnlineStatsEOSCore::QueryStats(const TSharedRef<const FUniqueNetId> LocalUserId, const TArray<TSharedRef<const FUniqueNetId>>& StatUsers, const TArray<FString>& StatNames,
                                 const FOnlineStatsQueryUsersStatsComplete& Delegate)
{
	if (StatNames.Num() == 0)
	{
		UE_LOG_ONLINE_STATS(Warning, TEXT("QueryStats() without a list of stats names to query is not supported"));
		Delegate.ExecuteIfBound(FOnlineError(EOnlineErrorResult::NotImplemented), TArray<TSharedRef<const FOnlineStatsUserStats>>());
		return;
	}
	if (StatUsers.Num() == 0)
	{
		UE_LOG_ONLINE_STATS(Warning, TEXT("QueryStats() without a list of users to query is not supported"));
		Delegate.ExecuteIfBound(FOnlineError(EOnlineErrorResult::NotImplemented), TArray<TSharedRef<const FOnlineStatsUserStats>>());
		return;
	}

	FQueryStatsOptions Options(StatNames.Num());
	for (int32 Index = 0; Index < StatNames.Num(); Index++)
	{
		FCStringAnsi::Strncpy(Options.PointerArray[Index], TCHAR_TO_UTF8(*StatNames[Index]), EOSCORE_OSS_STRING_BUFFER_LENGTH + 1);
	}

	int32 NumKnownUsers = 0;
	for (TSharedRef<const FUniqueNetId> StatUserId : StatUsers)
	{
		FUniqueNetIdEOSCore EOSId(*StatUserId);
		EOS_ProductUserId UserId = EOS_ProductUserId_FromString(TCHAR_TO_UTF8(*EOSId.m_ProductUserIdStr));
		if (UserId == nullptr)
		{
			continue;
		}
		NumKnownUsers++;
	}

	FStatsQueryContextPtr StatsQueryContext = MakeShareable(new FStatsQueryContext(NumKnownUsers, StatNames, Delegate));
	for (TSharedRef<const FUniqueNetId> StatUserId : StatUsers)
	{
		FUniqueNetIdEOSCore EOSId(*StatUserId);
		EOS_ProductUserId UserId = EOS_ProductUserId_FromString(TCHAR_TO_UTF8(*EOSId.m_ProductUserIdStr));
		if (UserId == nullptr)
		{
			continue;
		}

		Options.LocalUserId = UserId;

		FReadStatsCallback* CallbackObj = new FReadStatsCallback();
		CallbackObj->m_CallbackLambda = [this, StatsQueryContext](const EOS_Stats_OnQueryStatsCompleteCallbackInfo* Data)
		{
			StatsQueryContext->NumPlayerReads--;
			bool bWasSuccessful = Data->ResultCode == EOS_EResult::EOS_Success;
			if (bWasSuccessful)
			{
				FUniqueNetIdEOSCorePtr StatUserId = m_Subsystem->m_IdentityInterfacePtr->GetLocalUniqueNetIdEOS(Data->TargetUserId);
				if (StatUserId.IsValid())
				{
					char StatNameANSI[EOSCORE_OSS_STRING_BUFFER_LENGTH + 1];
					EOS_Stats_CopyStatByNameOptions Options = {};
					Options.ApiVersion = EOS_STATS_COPYSTATBYNAME_API_LATEST;
					Options.TargetUserId = Data->TargetUserId;
					Options.Name = StatNameANSI;

					TSharedPtr<FOnlineStatsUserStats> UserStats = m_StatsCache.Emplace(StatUserId.ToSharedRef(), MakeShared<FOnlineStatsUserStats>(StatUserId.ToSharedRef()));
					for (const FString& StatName : StatsQueryContext->StatNames)
					{
						FCStringAnsi::Strncpy(StatNameANSI, TCHAR_TO_UTF8(*StatName), EOSCORE_OSS_STRING_BUFFER_LENGTH + 1);

						EOS_Stats_Stat* ReadStat = nullptr;
						if (EOS_Stats_CopyStatByName(m_Subsystem->m_StatsHandle, &Options, &ReadStat) == EOS_EResult::EOS_Success)
						{
							UserStats->Stats.Add(StatName, FOnlineStatValue(ReadStat->Value));

							EOS_Stats_Stat_Release(ReadStat);
						}
						else
						{
							UserStats->Stats.Add(StatName, FOnlineStatValue());
						}
					}
				}
			}
			else
			{
				char ProductIdString[EOS_PRODUCTUSERID_MAX_LENGTH + 1];
				ProductIdString[0] = '\0';
				int32_t BufferSize = EOS_PRODUCTUSERID_MAX_LENGTH + 1;
				EOS_ProductUserId_ToString(Data->TargetUserId, ProductIdString, &BufferSize);
				UE_LOG_ONLINE_STATS(Error, TEXT("EOS_Stats_QueryStats() for user (%s) failed with EOS result code (%s)"), ANSI_TO_TCHAR(ProductIdString),
				                    ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
			}
			if (StatsQueryContext->NumPlayerReads <= 0)
			{
				AppendStats(m_StatsCache, StatsQueryContext->m_StatsCache);

				TArray<TSharedRef<const FOnlineStatsUserStats>> OutArray;
				for (const TPair<TSharedRef<const FUniqueNetId>, TSharedRef<FOnlineStatsUserStats>>& StatsUser : StatsQueryContext->m_StatsCache)
				{
					OutArray.Add(StatsUser.Value);
				}
				StatsQueryContext->Delegate.ExecuteIfBound(FOnlineError(StatsQueryContext->m_StatsCache.Num() > 0), OutArray);
			}
		};
		EOS_Stats_QueryStats(m_Subsystem->m_StatsHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());
	}
}

TSharedPtr<const FOnlineStatsUserStats> FOnlineStatsEOSCore::GetStats(const TSharedRef<const FUniqueNetId> StatsUserId) const
{
	if (const TSharedRef<FOnlineStatsUserStats>* const FoundStats = m_StatsCache.Find(StatsUserId))
	{
		return *FoundStats;
	}
	return nullptr;
}

inline int32 GetVariantValue(const FOnlineStatValue& Data)
{
	int32 Value = 0;
	switch (Data.GetType())
	{
	case EOnlineKeyValuePairDataType::Int32:
		{
			Data.GetValue(Value);
			break;
		}
	case EOnlineKeyValuePairDataType::UInt32:
		{
			uint32 Value1 = 0;
			Data.GetValue(Value);
			Value = (int32)Value1;
			break;
		}
	case EOnlineKeyValuePairDataType::Int64:
		{
			int64 Value1 = 0;
			Data.GetValue(Value1);
			Value = (int32)Value1;
			break;
		}
	case EOnlineKeyValuePairDataType::UInt64:
		{
			uint64 Value1 = 0;
			Data.GetValue(Value1);
			Value = (int32)Value1;
			break;
		}
	case EOnlineKeyValuePairDataType::Bool:
		{
			bool Value1 = false;
			Data.GetValue(Value1);
			Value = Value1 ? 1 : 0;
			break;
		}
	case EOnlineKeyValuePairDataType::Float:
		{
			float Value1 = 0.f;
			Data.GetValue(Value1);
			Value = FloatStatToIntStat(Value1);
			break;
		}
	case EOnlineKeyValuePairDataType::Double:
		{
			double Value1 = 0.f;
			Data.GetValue(Value1);
			Value = FloatStatToIntStat(Value1);
			break;
		}
	}
	return Value;
}

typedef TEOSCallback<EOS_Stats_OnIngestStatCompleteCallback, EOS_Stats_IngestStatCompleteCallbackInfo> FWriteStatsCallback;

void FOnlineStatsEOSCore::WriteStats(EOS_ProductUserId LocalUserId, EOS_ProductUserId UserId, const FOnlineStatsUserUpdatedStats& PlayerStats)
{
	TArray<EOS_Stats_IngestData> EOSData;
	TArray<FStatNameBuffer> EOSStatNames;
	EOSData.AddZeroed(PlayerStats.Stats.Num());
	EOSStatNames.AddZeroed(PlayerStats.Stats.Num());
	uint32 Index = 0;

	for (const TPair<FString, FOnlineStatUpdate>& Stat : PlayerStats.Stats)
	{
		EOS_Stats_IngestData& EOSStat = EOSData[Index];
		EOSStat.ApiVersion = EOS_STATS_INGESTDATA_API_LATEST;

		EOSStat.IngestAmount = GetVariantValue(Stat.Value.GetValue());
		FCStringAnsi::Strncpy(EOSStatNames[Index].StatName, TCHAR_TO_UTF8(*Stat.Key), EOSCORE_OSS_STRING_BUFFER_LENGTH + 1);
		EOSStat.StatName = EOSStatNames[Index].StatName;

		Index++;
	}

	EOS_Stats_IngestStatOptions Options = { };
	Options.ApiVersion = EOS_STATS_INGESTSTAT_API_LATEST;
	Options.LocalUserId = LocalUserId;
	Options.TargetUserId = UserId;
	Options.Stats = EOSData.GetData();
	Options.StatsCount = EOSData.Num();

	FWriteStatsCallback* CallbackObj = new FWriteStatsCallback();
	CallbackObj->m_CallbackLambda = [this](const EOS_Stats_IngestStatCompleteCallbackInfo* Data)
	{
		bool bWasSuccessful = Data->ResultCode == EOS_EResult::EOS_Success;
		if (!bWasSuccessful)
		{
			UE_LOG_ONLINE_STATS(Error, TEXT("EOS_Stats_IngestStat() failed with EOS result code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
		}
	};
	EOS_Stats_IngestStat(m_Subsystem->m_StatsHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());
}

void FOnlineStatsEOSCore::UpdateStats(const TSharedRef<const FUniqueNetId> LocalUserId, const TArray<FOnlineStatsUserUpdatedStats>& UpdatedUserStats, const FOnlineStatsUpdateStatsComplete& Delegate)
{
	FUniqueNetIdEOSCore EOSId(*LocalUserId);
	EOS_ProductUserId UserId = EOS_ProductUserId_FromString(TCHAR_TO_UTF8(*EOSId.m_ProductUserIdStr));
	if (UserId == NULL)
	{
		UE_LOG_ONLINE_STATS(Error, TEXT("UpdateStats() failed for unknown player (%s)"), *EOSId.UniqueNetIdStr);
		Delegate.ExecuteIfBound(FOnlineError(EOnlineErrorResult::InvalidCreds));
		return;
	}
	if (UpdatedUserStats.Num() == 0)
	{
		UE_LOG_ONLINE_STATS(Error, TEXT("UpdateStats() failed for player due to no user stats being specified"));
		Delegate.ExecuteIfBound(FOnlineError(EOnlineErrorResult::InvalidParams));
		return;
	}

	for (const FOnlineStatsUserUpdatedStats& StatsUpdate : UpdatedUserStats)
	{
		EOS_ProductUserId StatsUser = m_Subsystem->m_IdentityInterfacePtr->GetProductUserId(*StatsUpdate.Account);
		if (StatsUser != nullptr)
		{
			WriteStats(UserId, StatsUser, StatsUpdate);
		}
		else
		{
			UE_LOG_ONLINE_STATS(Error, TEXT("UpdateStats() failed for unknown player (%s)"), *StatsUpdate.Account->ToDebugString());
		}
	}
	Delegate.ExecuteIfBound(FOnlineError(EOnlineErrorResult::Success));
}

#if !UE_BUILD_SHIPPING
void FOnlineStatsEOSCore::ResetStats(const TSharedRef<const FUniqueNetId> StatsUserId)
{
	UE_LOG_ONLINE_STATS(Warning, TEXT("ResetStats() is not supported"));
}
#endif
