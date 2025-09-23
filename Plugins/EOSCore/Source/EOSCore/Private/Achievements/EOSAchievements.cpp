/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "Achievements/EOSAchievements.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLogging.h"

void UCoreAchievements::Deinitialize()
{
	LogVerbose("");

	for (auto& Element : m_AchievementsUnlockedV2Callbacks)
	{
		EOS_Achievements_RemoveNotifyAchievementsUnlocked(GetAchievementsHandle(this), Element.Key);
	}

	m_AchievementsUnlockedV2Callbacks.Empty();
}

UCoreAchievements* UCoreAchievements::GetAchievements(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (GetAchievementsHandle(WorldContextObject))
		{
			if (UWorld* World = WorldContextObject->GetWorld())
			{
				if (UGameInstance* GameInstance = World->GetGameInstance())
				{
					return GameInstance->GetSubsystem<UCoreAchievements>();
				}
			}
		}
	}
	return nullptr;
}

void UCoreAchievements::EOSAchievementsQueryDefinitions(UObject* WorldContextObject, FEOSAchievementsQueryDefinitionsOptions Options, const FOnAchievementsQueryDefinitionsCallback& Callback)
{
	LogVerbose("");

	if (GetAchievementsHandle(WorldContextObject))
	{
		EOS_Achievements_QueryDefinitionsOptions AchievementsOptions = {};
		AchievementsOptions.ApiVersion = EOS_ACHIEVEMENTS_QUERYDEFINITIONS_API_LATEST;
		AchievementsOptions.LocalUserId = Options.LocalUserId;

		FQueryDefinitionsCallback* CallbackObj = new FQueryDefinitionsCallback(Callback);
		EOS_Achievements_QueryDefinitions(GetAchievementsHandle(WorldContextObject), &AchievementsOptions, CallbackObj, Internal_OnQueryDefinitionsCallback);
	}
}

int32 UCoreAchievements::EOSAchievementsGetAchievementDefinitionCount(UObject* WorldContextObject, FEOSAchievementsGetAchievementDefinitionCountOptions Options)
{
	LogVerbose("");

	int32 Result = 0;

	if (GetAchievementsHandle(WorldContextObject))
	{
		EOS_Achievements_GetAchievementDefinitionCountOptions AchievementsOptions = {};
		AchievementsOptions.ApiVersion = Options.ApiVersion;

		Result = EOS_Achievements_GetAchievementDefinitionCount(GetAchievementsHandle(WorldContextObject), &AchievementsOptions);
	}

	return Result;
}

EOSResult UCoreAchievements::EOSAchievementsCopyAchievementDefinitionV2ByIndex(UObject* WorldContextObject, FEOSAchievementsCopyAchievementDefinitionV2ByIndexOptions Options, FEOSAchievementsDefinitionV2& OutDefinition)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetAchievementsHandle(WorldContextObject))
	{
		EOS_Achievements_CopyAchievementDefinitionV2ByIndexOptions AchievementsOptions = {};
		AchievementsOptions.ApiVersion = EOS_ACHIEVEMENTS_COPYDEFINITIONV2BYINDEX_API_LATEST;
		AchievementsOptions.AchievementIndex = static_cast<uint32_t>(Options.AchievementIndex);

		EOS_Achievements_DefinitionV2* Definition = nullptr;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Achievements_CopyAchievementDefinitionV2ByIndex(GetAchievementsHandle(WorldContextObject), &AchievementsOptions, &Definition));

		if (Result == EOSResult::EOS_Success)
		{
			OutDefinition = *Definition;
		}

		EOS_Achievements_DefinitionV2_Release(Definition);
	}

	return Result;
}

EOSResult UCoreAchievements::EOSAchievementsCopyAchievementDefinitionV2ByAchievementId(UObject* WorldContextObject, FEOSAchievementsCopyAchievementDefinitionV2ByAchievementIdOptions Options, FEOSAchievementsDefinitionV2& OutDefinition)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_AccessDenied;

	if (GetAchievementsHandle(WorldContextObject))
	{
		const FCopyAchievementDefinitionV2ByAchievementIdOptions AchievementsOptions(Options.AchievementId);

		EOS_Achievements_DefinitionV2* Definition = nullptr;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Achievements_CopyAchievementDefinitionV2ByAchievementId(GetAchievementsHandle(WorldContextObject), &AchievementsOptions, &Definition));

		if (Result == EOSResult::EOS_Success)
		{
			OutDefinition = *Definition;
		}

		EOS_Achievements_DefinitionV2_Release(Definition);
	}

	return Result;
}

void UCoreAchievements::EOSAchievementsQueryPlayerAchievements(UObject* WorldContextObject, FEOSAchievementsQueryPlayerAchievementsOptions Options, const FOnAchievementsQueryPlayerAchievementsCallback& Callback)
{
	LogVerbose("");

	if (GetAchievementsHandle(WorldContextObject))
	{
		EOS_Achievements_QueryPlayerAchievementsOptions AchievementsOptions = {};
		AchievementsOptions.ApiVersion = Options.ApiVersion;
		AchievementsOptions.LocalUserId = Options.LocalUserId;
		AchievementsOptions.TargetUserId = Options.TargetUserId;

		FQueryPlayerAchievementsCallback* CallbackObj = new FQueryPlayerAchievementsCallback({this, Callback});
		EOS_Achievements_QueryPlayerAchievements(GetAchievementsHandle(WorldContextObject), &AchievementsOptions, CallbackObj, Internal_OnQueryPlayerAchievementsCallback);
	}
}

int32 UCoreAchievements::EOSAchievementsGetPlayerAchievementCount(UObject* WorldContextObject, FEOSAchievementsGetPlayerAchievementCountOptions Options)
{
	LogVerbose("");

	int32 Result = 0;

	if (GetAchievementsHandle(WorldContextObject))
	{
		EOS_Achievements_GetPlayerAchievementCountOptions AchievementsOptions = {};
		AchievementsOptions.ApiVersion = Options.ApiVersion;
		AchievementsOptions.UserId = Options.UserId;

		Result = EOS_Achievements_GetPlayerAchievementCount(GetAchievementsHandle(WorldContextObject), &AchievementsOptions);
	}

	return Result;
}

EOSResult UCoreAchievements::EOSAchievementsCopyPlayerAchievementByIndex(UObject* WorldContextObject, FEOSAchievementsCopyPlayerAchievementByIndexOptions Options, FEOSAchievementsPlayerAchievement& OutAchievement)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_AccessDenied;

	if (GetAchievementsHandle(WorldContextObject))
	{
		EOS_Achievements_CopyPlayerAchievementByIndexOptions AchievementsOptions = {};
		AchievementsOptions.ApiVersion = Options.ApiVersion;
		AchievementsOptions.LocalUserId = Options.LocalUserId;
		AchievementsOptions.TargetUserId = Options.TargetUserId;
		AchievementsOptions.AchievementIndex = static_cast<uint32_t>(Options.AchievementIndex);

		EOS_Achievements_PlayerAchievement* CallbackObj = nullptr;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Achievements_CopyPlayerAchievementByIndex(GetAchievementsHandle(WorldContextObject), &AchievementsOptions, &CallbackObj));

		if (Result == EOSResult::EOS_Success)
		{
			OutAchievement = *CallbackObj;
		}

		EOS_Achievements_PlayerAchievement_Release(CallbackObj);
	}

	return Result;
}

void UCoreAchievements::EOSAchievementsUnlockAchievements(UObject* WorldContextObject, FEOSAchievementsUnlockAchievementsOptions Options, const FOnAchievementsUnlockAchievementsCallback& Callback)
{
	LogVerbose("");

	if (GetAchievementsHandle(WorldContextObject))
	{
		FUnlockAchievementsOperation UnlockAchievementsOperation(Options.AchievementIds.Num(), Options.UserId);

		for (int32 i = 0; i < Options.AchievementIds.Num(); i++)
		{
			FCStringAnsi::Strncpy(UnlockAchievementsOperation.PointerArray[i], TCHAR_TO_UTF8(*Options.AchievementIds[i]), (Options.AchievementIds[i].Len() + 1));
		}

		FUnlockAchievementsCallback* CallbackObj = new FUnlockAchievementsCallback({this, Callback});
		EOS_Achievements_UnlockAchievements(GetAchievementsHandle(WorldContextObject), &UnlockAchievementsOperation, CallbackObj, Internal_OnUnlockAchievementsCallback);
	}
}

FEOSNotificationId UCoreAchievements::EOSAchievementsAddNotifyAchievementsUnlockedV2(UObject* WorldContextObject, const FOnAchievementsUnlockedV2Callback& Callback)
{
	LogVerbose("");

	FEOSNotificationId NotificationId;

	if (GetAchievementsHandle(WorldContextObject))
	{
		EOS_Achievements_AddNotifyAchievementsUnlockedV2Options AchievementsOptions = {};
		AchievementsOptions.ApiVersion = EOS_ACHIEVEMENTS_ADDNOTIFYACHIEVEMENTSUNLOCKEDV2_API_LATEST;

		FAchievementsUnlockedV2Callback* CallbackObj = new FAchievementsUnlockedV2Callback(Callback);

		NotificationId = EOS_Achievements_AddNotifyAchievementsUnlockedV2(GetAchievementsHandle(WorldContextObject), &AchievementsOptions, CallbackObj, ([](const EOS_Achievements_OnAchievementsUnlockedCallbackV2Info* Data)
		{
			const FAchievementsUnlockedV2Callback* CallbackData = static_cast<FAchievementsUnlockedV2Callback*>(Data->ClientData);
			CallbackData->m_Callback.ExecuteIfBound(*Data);
		}));
		GetAchievements(WorldContextObject)->m_AchievementsUnlockedV2Callbacks.Add(NotificationId, CallbackObj);
	}

	return NotificationId;
}

void UCoreAchievements::EOSAchievementsRemoveNotifyAchievementsUnlocked(UObject* WorldContextObject, FEOSNotificationId Id)
{
	LogVerbose("");

	if (GetAchievementsHandle(WorldContextObject))
	{
		EOS_Achievements_RemoveNotifyAchievementsUnlocked(GetAchievementsHandle(WorldContextObject), Id);
		GetAchievements(WorldContextObject)->m_AchievementsUnlockedV2Callbacks.Remove(Id);
	}
}

void UCoreAchievements::Internal_OnQueryDefinitionsCallback(const EOS_Achievements_OnQueryDefinitionsCompleteCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FQueryDefinitionsCallback* CallbackObj = static_cast<FQueryDefinitionsCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreAchievements::Internal_OnQueryPlayerAchievementsCallback(const EOS_Achievements_OnQueryPlayerAchievementsCompleteCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FQueryPlayerAchievementsCallback* CallbackObj = static_cast<FQueryPlayerAchievementsCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreAchievements::Internal_OnUnlockAchievementsCallback(const EOS_Achievements_OnUnlockAchievementsCompleteCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FUnlockAchievementsCallback* CallbackObj = static_cast<FUnlockAchievementsCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}
