/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "OnlineAchievementsEOSCore.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemEOSCore.h"
#include "OnlineSubsystemTypesEOSCore.h"
#include "OnlineStatsEOSCore.h"
#include "OnlineIdentityEOSCore.h"
#include "OnlineSubsystemModuleEOSCore.h"
#include "OnlineSubsystemEOSCorePrivatePCH.h"

struct FUnlockAchievements : public EOS_Achievements_UnlockAchievementsOptions
{
	TArray<char*> PointerArray;

	FUnlockAchievements(int32 numAchievements, EOS_ProductUserId userId)
		: EOS_Achievements_UnlockAchievementsOptions()
	{
		PointerArray.AddZeroed(numAchievements);

		for (int32 i = 0; i < PointerArray.Num(); i++)
		{
			PointerArray[i] = new char[256];
		}

		ApiVersion = EOS_ACHIEVEMENTS_UNLOCKACHIEVEMENTS_API_LATEST;
		UserId = userId;
		AchievementsCount = PointerArray.Num();
		AchievementIds = const_cast<const char**>(PointerArray.GetData());
	}
};

struct FSubsystemUnlockAchievementsCallback
{
public:
	FOnAchievementsWrittenDelegate m_Callback;
	FUniqueNetIdEOSCorePtr m_PlayerNetId;
public:
	FSubsystemUnlockAchievementsCallback(const FOnAchievementsWrittenDelegate& Callback, FUniqueNetIdEOSCorePtr PlayerNetId)
		: m_Callback(Callback)
		  , m_PlayerNetId(PlayerNetId)
	{
	}
};

void FOnlineAchievementsEOSCore::WriteAchievements(const FUniqueNetId& PlayerId, FOnlineAchievementsWriteRef& WriteObject, const FOnAchievementsWrittenDelegate& Delegate)
{
	bool bUseStats = true;

	TArray<FOnlineStatsUserUpdatedStats> StatsToWrite;
	FUniqueNetIdEOSCorePtr NetId = MakeShared<FUniqueNetIdEOSCore>(PlayerId);
	FOnlineStatsUserUpdatedStats& UpdatedStats = StatsToWrite.Emplace_GetRef(NetId.ToSharedRef());

	for (const TPair<FName, FVariantData>& Stat : WriteObject->Properties)
	{
		UpdatedStats.Stats.Add(Stat.Key.ToString(), FOnlineStatUpdate(Stat.Value, FOnlineStatUpdate::EOnlineStatModificationType::Unknown));
	}

	if (bUseStats)
	{
		EOSSubsystem->m_StatsInterfacePtr->UpdateStats(NetId.ToSharedRef(), StatsToWrite, FOnlineStatsUpdateStatsComplete());

		WriteObject->WriteState = EOnlineAsyncTaskState::Done;
		Delegate.ExecuteIfBound(PlayerId, true);
	}
	else
	{
		const EOS_ProductUserId UserId = EOS_ProductUserId_FromString(TCHAR_TO_UTF8(*NetId->m_ProductUserIdStr));
		FUnlockAchievements UnlockAchievementsOperation(UpdatedStats.Stats.Num(), UserId);

		for (const TPair<FName, FVariantData>& Stat : WriteObject->Properties)
		{
			int32 i = 0;
			FCStringAnsi::Strncpy(UnlockAchievementsOperation.PointerArray[i], TCHAR_TO_UTF8(*Stat.Key.ToString()), 256);
			i++;
		}

		FSubsystemUnlockAchievementsCallback* CallbackObj = new FSubsystemUnlockAchievementsCallback({Delegate, NetId});
		EOS_Achievements_UnlockAchievements(EOSSubsystem->m_AchievementsHandle, &UnlockAchievementsOperation, CallbackObj, [](const EOS_Achievements_OnUnlockAchievementsCompleteCallbackInfo* Data)
		{
			LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

			FSubsystemUnlockAchievementsCallback* CallbackObj = static_cast<FSubsystemUnlockAchievementsCallback*>(Data->ClientData);
			check(CallbackObj);

			if (CallbackObj)
			{
				CallbackObj->m_Callback.ExecuteIfBound(*CallbackObj->m_PlayerNetId, true);
			}
		});

		WriteObject->WriteState = EOnlineAsyncTaskState::Done;
	}
}

typedef TEOSCallback<EOS_Achievements_OnQueryPlayerAchievementsCompleteCallback, EOS_Achievements_OnQueryPlayerAchievementsCompleteCallbackInfo> FQueryProgressCallback;

void FOnlineAchievementsEOSCore::QueryAchievements(const FUniqueNetId& PlayerId, const FOnQueryAchievementsCompleteDelegate& Delegate)
{
	const int32 LocalUserId = EOSSubsystem->m_IdentityInterfacePtr->GetLocalUserNumFromUniqueNetId(PlayerId);

	if (LocalUserId < 0)
	{
		Delegate.ExecuteIfBound(PlayerId, false);
		return;
	}

	EOS_Achievements_QueryPlayerAchievementsOptions Options = {};
	Options.ApiVersion = EOS_ACHIEVEMENTS_QUERYPLAYERACHIEVEMENTS_API_LATEST;
	Options.LocalUserId = EOSSubsystem->m_IdentityInterfacePtr->GetLocalProductUserId(LocalUserId);
	Options.TargetUserId = EOSSubsystem->m_IdentityInterfacePtr->GetLocalProductUserId(LocalUserId);

	FQueryProgressCallback* CallbackObj = new FQueryProgressCallback();
	CallbackObj->m_CallbackLambda = [this, LambaPlayerId = FUniqueNetIdEOSCore(PlayerId), OnComplete = FOnQueryAchievementsCompleteDelegate(Delegate)](
		const EOS_Achievements_OnQueryPlayerAchievementsCompleteCallbackInfo* Data)
		{
			const bool bWasSuccessful = Data->ResultCode == EOS_EResult::EOS_Success;

			if (bWasSuccessful)
			{
				TSharedPtr<TArray<FOnlineAchievement>> Cheevos = MakeShareable(new TArray<FOnlineAchievement>());
				CachedAchievementsMap.Add(LambaPlayerId.UniqueNetIdStr, Cheevos);

				const int32 LocalUserNum = EOSSubsystem->m_IdentityInterfacePtr->GetLocalUserNumFromUniqueNetId(LambaPlayerId);
				const EOS_ProductUserId UserId = EOSSubsystem->m_IdentityInterfacePtr->GetLocalProductUserId(LocalUserNum);

				EOS_Achievements_GetPlayerAchievementCountOptions CountOptions = {};
				CountOptions.ApiVersion = EOS_ACHIEVEMENTS_GETPLAYERACHIEVEMENTCOUNT_API_LATEST;
				CountOptions.UserId = UserId;
				const uint32 Count = EOS_Achievements_GetPlayerAchievementCount(EOSSubsystem->m_AchievementsHandle, &CountOptions);

				EOS_Achievements_CopyPlayerAchievementByIndexOptions CopyOptions = {};
				CopyOptions.ApiVersion = EOS_ACHIEVEMENTS_COPYPLAYERACHIEVEMENTBYINDEX_API_LATEST;
				CopyOptions.LocalUserId = UserId;
				CopyOptions.TargetUserId = UserId;

				for (uint32 Index = 0; Index < Count; Index++)
				{
					CopyOptions.AchievementIndex = Index;

					EOS_Achievements_PlayerAchievement* AchievementEOS = nullptr;
					EOS_EResult Result = EOS_Achievements_CopyPlayerAchievementByIndex(EOSSubsystem->m_AchievementsHandle, &CopyOptions, &AchievementEOS);
					if (Result == EOS_EResult::EOS_Success)
					{
						FOnlineAchievement* Achievement = new(*Cheevos) FOnlineAchievement();

						Achievement->Id = AchievementEOS->AchievementId;
						Achievement->Progress = AchievementEOS->Progress;

						EOS_Achievements_PlayerAchievement_Release(AchievementEOS);
					}
					else
					{
						UE_LOG_ONLINE_ACHIEVEMENTS(Error, TEXT("EOS_Achievements_CopyPlayerAchievementByIndex() failed with error code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(Result)));
					}
				}
			}
			else
			{
				UE_LOG_ONLINE_ACHIEVEMENTS(Error, TEXT("EOS_Achievements_QueryPlayerAchievements() failed with error code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
			}
			OnComplete.ExecuteIfBound(LambaPlayerId, bWasSuccessful);
		};

	EOS_Achievements_QueryPlayerAchievements(EOSSubsystem->m_AchievementsHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());
}

typedef TEOSCallback<EOS_Achievements_OnQueryDefinitionsCompleteCallback, EOS_Achievements_OnQueryDefinitionsCompleteCallbackInfo> FQueryDefinitionsCallback;

void FOnlineAchievementsEOSCore::QueryAchievementDescriptions(const FUniqueNetId& PlayerId, const FOnQueryAchievementsCompleteDelegate& Delegate)
{
	if (CachedAchievementDefinitions.Num())
	{
		Delegate.ExecuteIfBound(PlayerId, true);
		return;
	}

	const int32 LocalUserId = EOSSubsystem->m_IdentityInterfacePtr->GetLocalUserNumFromUniqueNetId(PlayerId);
	if (LocalUserId < 0)
	{
		Delegate.ExecuteIfBound(PlayerId, false);
		return;
	}

	EOS_Achievements_QueryDefinitionsOptions Options = {};
	Options.ApiVersion = EOS_ACHIEVEMENTS_QUERYDEFINITIONS_API_LATEST;
	Options.LocalUserId = EOSSubsystem->m_IdentityInterfacePtr->GetLocalProductUserId(LocalUserId);

	FQueryDefinitionsCallback* CallbackObj = new FQueryDefinitionsCallback();
	CallbackObj->m_CallbackLambda = [this, LambaPlayerId = FUniqueNetIdEOSCore(PlayerId), OnComplete = FOnQueryAchievementsCompleteDelegate(Delegate)](const EOS_Achievements_OnQueryDefinitionsCompleteCallbackInfo* Data)
	{
		const bool bWasSuccessful = Data->ResultCode == EOS_EResult::EOS_Success;
		if (bWasSuccessful)
		{
			EOS_Achievements_GetAchievementDefinitionCountOptions CountOptions = {};
			CountOptions.ApiVersion = EOS_ACHIEVEMENTS_GETACHIEVEMENTDEFINITIONCOUNT_API_LATEST;
			const uint32 Count = EOS_Achievements_GetAchievementDefinitionCount(EOSSubsystem->m_AchievementsHandle, &CountOptions);

			EOS_Achievements_CopyAchievementDefinitionByIndexOptions CopyOptions = {};
			CopyOptions.ApiVersion = EOS_ACHIEVEMENTS_COPYDEFINITIONBYINDEX_API_LATEST;
			CachedAchievementDefinitions.Empty(Count);
			CachedAchievementDefinitionsMap.Empty();

			for (uint32 Index = 0; Index < Count; Index++)
			{
				CopyOptions.AchievementIndex = Index;
				EOS_Achievements_Definition* Definition = nullptr;

				EOS_EResult Result = EOS_Achievements_CopyAchievementDefinitionByIndex(EOSSubsystem->m_AchievementsHandle, &CopyOptions, &Definition);
				if (Result == EOS_EResult::EOS_Success)
				{
					FOnlineAchievementDesc* Desc = new(CachedAchievementDefinitions) FOnlineAchievementDesc();
					CachedAchievementDefinitionsMap.Add(Definition->AchievementId, Desc);

					Desc->Title = FText::FromString(Definition->DisplayName);
					Desc->LockedDesc = FText::FromString(Definition->LockedDescription);
					Desc->UnlockedDesc = FText::FromString(Definition->CompletionDescription);
					Desc->bIsHidden = Definition->bIsHidden == EOS_TRUE;

					EOS_Achievements_Definition_Release(Definition);
				}
				else
				{
					UE_LOG_ONLINE_ACHIEVEMENTS(Error, TEXT("EOS_Achievements_CopyAchievementDefinitionByIndex() failed with error code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(Result)));
				}
			}
		}
		else
		{
			UE_LOG_ONLINE_ACHIEVEMENTS(Error, TEXT("EOS_Achievements_QueryDefinitions() failed with error code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
		}

		OnComplete.ExecuteIfBound(LambaPlayerId, bWasSuccessful);
	};
	
	EOS_Achievements_QueryDefinitions(EOSSubsystem->m_AchievementsHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());
}

EOnlineCachedResult::Type FOnlineAchievementsEOSCore::GetCachedAchievement(const FUniqueNetId& PlayerId, const FString& AchievementId, FOnlineAchievement& OutAchievement)
{
	const FUniqueNetIdEOSCore EosId(PlayerId);
	
	if (CachedAchievementsMap.Contains(EosId.UniqueNetIdStr))
	{
		const TArray<FOnlineAchievement>& Achievements = *CachedAchievementsMap[EosId.UniqueNetIdStr];
		for (const FOnlineAchievement& Achievement : Achievements)
		{
			if (Achievement.Id == AchievementId)
			{
				OutAchievement = Achievement;
				return EOnlineCachedResult::Success;
			}
		}
	}
	return EOnlineCachedResult::NotFound;
}

EOnlineCachedResult::Type FOnlineAchievementsEOSCore::GetCachedAchievements(const FUniqueNetId& PlayerId, TArray<FOnlineAchievement>& OutAchievements)
{
	const FUniqueNetIdEOSCore EosId(PlayerId);
	
	if (CachedAchievementsMap.Contains(EosId.UniqueNetIdStr))
	{
		OutAchievements = *CachedAchievementsMap[EosId.UniqueNetIdStr];
		return EOnlineCachedResult::Success;
	}
	
	return EOnlineCachedResult::NotFound;
}

EOnlineCachedResult::Type FOnlineAchievementsEOSCore::GetCachedAchievementDescription(const FString& AchievementId, FOnlineAchievementDesc& OutAchievementDesc)
{
	if (CachedAchievementDefinitionsMap.Contains(AchievementId))
	{
		OutAchievementDesc = *CachedAchievementDefinitionsMap[AchievementId];
		return EOnlineCachedResult::Success;
	}
	return EOnlineCachedResult::NotFound;
}

#if !UE_BUILD_SHIPPING
bool FOnlineAchievementsEOSCore::ResetAchievements(const FUniqueNetId&)
{
	return false;
}
#endif
