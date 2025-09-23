/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "OnlineLeaderboardsEOSCore.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemEOSCore.h"
#include "OnlineSubsystemTypesEOSCore.h"
#include "OnlineIdentityEOSCore.h"
#include "OnlineStatsEOSCore.h"
#include "OnlineSubsystemEOSCorePrivatePCH.h"

struct FQueryLeaderboardForUserOptions :
	public EOS_Leaderboards_QueryLeaderboardUserScoresOptions
{
	TArray<char*> PointerArray;
	TArray<EOS_Leaderboards_UserScoresQueryStatInfo> StatInfoArray;
	TArray<EOS_ProductUserId> ProductUserIds;

	FQueryLeaderboardForUserOptions(const uint32 InNumColumns, TArray<EOS_ProductUserId>& InProductUserIds)
		: EOS_Leaderboards_QueryLeaderboardUserScoresOptions()
		  , ProductUserIds(MoveTemp(InProductUserIds))
	{
		ApiVersion = EOS_LEADERBOARDS_QUERYLEADERBOARDUSERSCORES_API_LATEST;

		StartTime = EOS_LEADERBOARDS_TIME_UNDEFINED;
		EndTime = EOS_LEADERBOARDS_TIME_UNDEFINED;

		PointerArray.AddZeroed(InNumColumns);
		for (int32 Index = 0; Index < PointerArray.Num(); Index++)
		{
			PointerArray[Index] = new char[EOSCORE_OSS_STRING_BUFFER_LENGTH + 1];
		}

		StatInfoArray.AddZeroed(InNumColumns);
		StatInfo = StatInfoArray.GetData();
		StatInfoCount = InNumColumns;

		UserIds = ProductUserIds.GetData();
		UserIdsCount = ProductUserIds.Num();
	}

	~FQueryLeaderboardForUserOptions()
	{
		for (int32 Index = 0; Index < PointerArray.Num(); Index++)
		{
			delete[] PointerArray[Index];
		}
	}
};

struct FQueryLeaderboardForUsersContext
{
	TArray<TSharedRef<const FUniqueNetId>> Players;
	FOnlineLeaderboardReadRef ReadObject;

	FQueryLeaderboardForUsersContext(const TArray<TSharedRef<const FUniqueNetId>>& InPlayers, FOnlineLeaderboardReadRef& InReadObject)
		: Players(InPlayers)
		  , ReadObject(InReadObject)
	{
	}

	void AddEmptyRowForPlayer(TSharedRef<const FUniqueNetId> NetId, const FString& Nickname)
	{
		FOnlineStatsRow Row(Nickname, NetId);
		for (const FColumnMetaData& Column : ReadObject->ColumnMetadata)
		{
			Row.Columns.Add(Column.ColumnName, FVariantData());
		}
		Row.Rank = -1;
		ReadObject->Rows.Add(Row);
	}
};

typedef TEOSCallback<EOS_Leaderboards_OnQueryLeaderboardUserScoresCompleteCallback, EOS_Leaderboards_OnQueryLeaderboardUserScoresCompleteCallbackInfo> FQueryLeaderboardForUsersCallback;

bool FOnlineLeaderboardsEOSCore::ReadLeaderboards(const TArray<TSharedRef<const FUniqueNetId>>& Players, FOnlineLeaderboardReadRef& ReadObject)
{
	if (Players.Num() == 0)
	{
		return true;
	}

	TArray<EOS_ProductUserId> ProductUserIds;
	ProductUserIds.Empty(Players.Num());
	for (TSharedRef<const FUniqueNetId> NetId : Players)
	{
		FUniqueNetIdEOSCore EOSId(*NetId);
		EOS_ProductUserId UserId = EOS_ProductUserId_FromString(TCHAR_TO_UTF8(*EOSId.m_ProductUserIdStr));
		if (UserId != nullptr)
		{
			ProductUserIds.Add(UserId);
		}
	}

	FQueryLeaderboardForUserOptions Options(ReadObject->ColumnMetadata.Num(), ProductUserIds);
	int32 Index = 0;
	for (const FColumnMetaData& Column : ReadObject->ColumnMetadata)
	{
		FCStringAnsi::Strncpy(Options.PointerArray[Index], TCHAR_TO_UTF8(*Column.ColumnName.ToString()), EOSCORE_OSS_STRING_BUFFER_LENGTH + 1);
		EOS_Leaderboards_UserScoresQueryStatInfo& StatInfo = Options.StatInfoArray[Index];
		StatInfo.ApiVersion = EOS_LEADERBOARDS_USERSCORESQUERYSTATINFO_API_LATEST;
		StatInfo.StatName = Options.PointerArray[Index];
		StatInfo.Aggregation = EOS_ELeaderboardAggregation::EOS_LA_Latest;
		Index++;
	}

	TSharedPtr<FQueryLeaderboardForUsersContext> QueryContext = MakeShared<FQueryLeaderboardForUsersContext>(Players, ReadObject);

	FQueryLeaderboardForUsersCallback* CallbackObj = new FQueryLeaderboardForUsersCallback();
	CallbackObj->m_CallbackLambda = [this, QueryContext](const EOS_Leaderboards_OnQueryLeaderboardUserScoresCompleteCallbackInfo* Data)
	{
		bool bWasSuccessful = Data->ResultCode == EOS_EResult::EOS_Success;
		if (!bWasSuccessful)
		{
			UE_LOG_ONLINE_LEADERBOARD(Error, TEXT("EOS_Leaderboards_QueryLeaderboardUserScores() failed with EOS result code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
			QueryContext->ReadObject->ReadState = EOnlineAsyncTaskState::Failed;
			TriggerOnLeaderboardReadCompleteDelegates(false);
			return;
		}

		char StatName[EOSCORE_OSS_STRING_BUFFER_LENGTH + 1];

		for (TSharedRef<const FUniqueNetId> NetId : QueryContext->Players)
		{
			FString Nickname = m_Subsystem->m_IdentityInterfacePtr->GetPlayerNickname(*NetId);
			EOS_ProductUserId UserId = m_Subsystem->m_IdentityInterfacePtr->GetProductUserId(*NetId);
			if (UserId == nullptr)
			{
				QueryContext->AddEmptyRowForPlayer(NetId, Nickname);
				continue;
			}

			FOnlineStatsRow Row(Nickname, NetId);

			EOS_Leaderboards_CopyLeaderboardUserScoreByUserIdOptions UserCopyOptions = {};
			UserCopyOptions.ApiVersion = EOS_LEADERBOARDS_COPYLEADERBOARDUSERSCOREBYUSERID_API_LATEST;
			UserCopyOptions.UserId = UserId;
			UserCopyOptions.StatName = StatName;

			for (const FColumnMetaData& Column : QueryContext->ReadObject->ColumnMetadata)
			{
				FCStringAnsi::Strncpy(StatName, TCHAR_TO_UTF8(*Column.ColumnName.ToString()), EOSCORE_OSS_STRING_BUFFER_LENGTH + 1);

				EOS_Leaderboards_LeaderboardUserScore* LeaderboardUserScore = nullptr;
				EOS_EResult UserCopyResult = EOS_Leaderboards_CopyLeaderboardUserScoreByUserId(m_Subsystem->m_LeaderboardsHandle, &UserCopyOptions, &LeaderboardUserScore);
				if (UserCopyResult != EOS_EResult::EOS_Success)
				{
					Row.Columns.Add(Column.ColumnName, FVariantData());
					continue;
				}

				Row.Columns.Add(Column.ColumnName, FVariantData(LeaderboardUserScore->Score));

				EOS_Leaderboards_LeaderboardUserScore_Release(LeaderboardUserScore);
			}
			QueryContext->ReadObject->Rows.Add(Row);
		}

		FName SortedColumn = QueryContext->ReadObject->SortedColumn;
		QueryContext->ReadObject->Rows.Sort([SortedColumn](const FOnlineStatsRow& RowA, const FOnlineStatsRow& RowB)
		{
			const FVariantData& ValueA = RowA.Columns[SortedColumn];
			const FVariantData& ValueB = RowB.Columns[SortedColumn];
			if (ValueA.GetType() == ValueB.GetType())
			{
				int32 ScoreA = 0;
				int32 ScoreB = 0;
				ValueA.GetValue(ScoreA);
				ValueB.GetValue(ScoreB);
				return ScoreA >= ScoreB;
			}
			return true;
		});
		int32 Rank = 1;
		for (FOnlineStatsRow& Row : QueryContext->ReadObject->Rows)
		{
			Row.Rank = Rank++;
		}

		QueryContext->ReadObject->ReadState = EOnlineAsyncTaskState::Done;

		TriggerOnLeaderboardReadCompleteDelegates(true);
	};

	ReadObject->ReadState = EOnlineAsyncTaskState::InProgress;

	EOS_Leaderboards_QueryLeaderboardUserScores(m_Subsystem->m_LeaderboardsHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());

	return true;
}

bool FOnlineLeaderboardsEOSCore::ReadLeaderboardsForFriends(int32 LocalUserNum, FOnlineLeaderboardReadRef& ReadObject)
{
	return false;
}

typedef TEOSCallback<EOS_Leaderboards_OnQueryLeaderboardRanksCompleteCallback, EOS_Leaderboards_OnQueryLeaderboardRanksCompleteCallbackInfo> FQueryLeaderboardCallback;

bool FOnlineLeaderboardsEOSCore::ReadLeaderboardsAroundRank(int32 Rank, uint32 Range, FOnlineLeaderboardReadRef& ReadObject)
{
	if (Rank > EOSCORE_MAX_NUM_RANKINGS)
	{
		UE_LOG_ONLINE_LEADERBOARD(Warning, TEXT("ReadLeaderboardsAroundRank() - Rank (%d) must be <= 1000"), Rank);
		return false;
	}
	uint32 StartIndex = (uint32)FMath::Clamp<int32>(Rank - (int32)Range, 0, EOSCORE_MAX_NUM_RANKINGS);
	uint32 EndIndex = FMath::Clamp<uint32>(Rank + (int32)Range, 0, EOSCORE_MAX_NUM_RANKINGS - 1);

	char LeaderboardId[EOSCORE_OSS_STRING_BUFFER_LENGTH + 1];
	EOS_Leaderboards_QueryLeaderboardRanksOptions Options = {};
	Options.ApiVersion = EOS_LEADERBOARDS_QUERYLEADERBOARDRANKS_API_LATEST;
	Options.LeaderboardId = LeaderboardId;
	FCStringAnsi::Strncpy(LeaderboardId, TCHAR_TO_UTF8(*ReadObject->LeaderboardName.ToString()), EOSCORE_OSS_STRING_BUFFER_LENGTH + 1);

	FQueryLeaderboardCallback* CallbackObj = new FQueryLeaderboardCallback();
	FOnlineLeaderboardReadRef LambdaReadObject = ReadObject;
	CallbackObj->m_CallbackLambda = [this, LambdaReadObject, StartIndex, EndIndex](const EOS_Leaderboards_OnQueryLeaderboardRanksCompleteCallbackInfo* Data)
	{
		bool bWasSuccessful = Data->ResultCode == EOS_EResult::EOS_Success;
		if (!bWasSuccessful)
		{
			UE_LOG_ONLINE_LEADERBOARD(Error, TEXT("EOS_Leaderboards_QueryLeaderboardRanks() failed with EOS result code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
			LambdaReadObject->ReadState = EOnlineAsyncTaskState::Failed;
			TriggerOnLeaderboardReadCompleteDelegates(false);
			return;
		}
		EOS_Leaderboards_GetLeaderboardRecordCountOptions CountOptions = {};
		CountOptions.ApiVersion = EOS_LEADERBOARDS_GETLEADERBOARDRECORDCOUNT_API_LATEST;

		uint32 LeaderboardCount = EOS_Leaderboards_GetLeaderboardRecordCount(m_Subsystem->m_LeaderboardsHandle, &CountOptions);
		if (LeaderboardCount <= StartIndex)
		{
			LambdaReadObject->ReadState = EOnlineAsyncTaskState::Done;
			TriggerOnLeaderboardReadCompleteDelegates(false);
			return;
		}
		uint32 NewEndIndex = FMath::Clamp<uint32>(EndIndex, 0, LeaderboardCount - 1);

		EOS_Leaderboards_CopyLeaderboardRecordByIndexOptions CopyOptions = {};
		CopyOptions.ApiVersion = EOS_LEADERBOARDS_COPYLEADERBOARDRECORDBYINDEX_API_LATEST;

		char EpicIdStr[EOS_CONNECT_EXTERNAL_ACCOUNT_ID_MAX_LENGTH + 1];
		int32 EpicIdStrSize = EOS_CONNECT_EXTERNAL_ACCOUNT_ID_MAX_LENGTH + 1;

		EOS_Connect_GetProductUserIdMappingOptions Options = {};
		Options.ApiVersion = EOS_CONNECT_GETPRODUCTUSERIDMAPPING_API_LATEST;
		Options.AccountIdType = EOS_EExternalAccountType::EOS_EAT_EPIC;
		Options.LocalUserId = m_Subsystem->m_IdentityInterfacePtr->GetLocalProductUserId(0);

		for (uint32 Index = StartIndex; Index <= NewEndIndex; Index++)
		{
			CopyOptions.LeaderboardRecordIndex = Index;

			EOS_Leaderboards_LeaderboardRecord* Record = nullptr;
			EOS_EResult Result = EOS_Leaderboards_CopyLeaderboardRecordByIndex(m_Subsystem->m_LeaderboardsHandle, &CopyOptions, &Record);
			if (Result == EOS_EResult::EOS_Success)
			{
				Options.TargetProductUserId = Record->UserId;
				FString Nickname = Record->UserDisplayName;

				Result = EOS_Connect_GetProductUserIdMapping(m_Subsystem->m_ConnectHandle, &Options, EpicIdStr, &EpicIdStrSize);
				if (Result == EOS_EResult::EOS_Success)
				{
					EOS_EpicAccountId AccountId = EOS_EpicAccountId_FromString(EpicIdStr);

					if (Nickname.IsEmpty())
					{
						Nickname = TEXT("Unknown Player");
					}

					FUniqueNetIdEOSCorePtr NetId = MakeShared<FUniqueNetIdEOSCore>(MakeNetIdStringFromIds(AccountId, Record->UserId));
					FOnlineStatsRow* Row = new(LambdaReadObject->Rows) FOnlineStatsRow(Nickname, NetId.ToSharedRef());
					Row->Rank = Record->Rank;
					Row->Columns.Add(LambdaReadObject->SortedColumn, FVariantData(Record->Score));
				}
			}
		}

		LambdaReadObject->ReadState = EOnlineAsyncTaskState::Done;
		TriggerOnLeaderboardReadCompleteDelegates(true);
	};

	ReadObject->ReadState = EOnlineAsyncTaskState::InProgress;

	EOS_Leaderboards_QueryLeaderboardRanks(m_Subsystem->m_LeaderboardsHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());

	return false;
}

bool FOnlineLeaderboardsEOSCore::ReadLeaderboardsAroundUser(TSharedRef<const FUniqueNetId> Player, uint32 Range, FOnlineLeaderboardReadRef& ReadObject)
{
	TArray<TSharedRef<const FUniqueNetId>> Players;
	Players.Add(Player);

	return ReadLeaderboards(Players, ReadObject);
}

void FOnlineLeaderboardsEOSCore::FreeStats(FOnlineLeaderboardRead& ReadObject)
{
}

bool FOnlineLeaderboardsEOSCore::WriteLeaderboards(const FName& SessionName, const FUniqueNetId& Player, FOnlineLeaderboardWrite& WriteObject)
{
	FUniqueNetIdEOSCorePtr NetId = MakeShared<FUniqueNetIdEOSCore>(Player);
	TArray<FOnlineStatsUserUpdatedStats> StatsToWrite;

	FOnlineStatsUserUpdatedStats& UpdatedStats = StatsToWrite.Emplace_GetRef(NetId.ToSharedRef());
	for (const TPair<FName, FVariantData>& Stat : WriteObject.Properties)
	{
		UpdatedStats.Stats.Add(Stat.Key.ToString(), FOnlineStatUpdate(Stat.Value, FOnlineStatUpdate::EOnlineStatModificationType::Unknown));
	}

	m_Subsystem->m_StatsInterfacePtr->UpdateStats(NetId.ToSharedRef(), StatsToWrite, FOnlineStatsUpdateStatsComplete());
	return true;
}

bool FOnlineLeaderboardsEOSCore::FlushLeaderboards(const FName& SessionName)
{
	TriggerOnLeaderboardFlushCompleteDelegates(SessionName, true);
	return true;
}

bool FOnlineLeaderboardsEOSCore::WriteOnlinePlayerRatings(const FName& SessionName, int32 LeaderboardId, const TArray<FOnlinePlayerScore>& PlayerScores)
{
	return false;
}
