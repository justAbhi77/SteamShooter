/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "Friends/EOSFriends.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLogging.h"

void UCoreFriends::Deinitialize()
{
	LogVerbose("");

	for (auto Element: m_FriendsUpdateCallbacks)
	{
		EOS_Friends_RemoveNotifyFriendsUpdate(GetFriendsHandle(this), Element.Key);
	}

	m_FriendsUpdateCallbacks.Empty();
}

UCoreFriends* UCoreFriends::GetFriends(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (GetFriendsHandle(WorldContextObject))
		{
			if (UWorld* World = WorldContextObject->GetWorld())
			{
				if (UGameInstance* GameInstance = World->GetGameInstance())
				{
					return GameInstance->GetSubsystem<UCoreFriends>();
				}
			}	
		}
	}
	return nullptr;
}

void UCoreFriends::EOSFriendsQueryFriends(UObject* WorldContextObject, FEOSFriendsQueryFriendsOptions Options, const FOnFriendsQueryFriendsCallback& Callback)
{
	LogVerbose("");

	if (GetFriendsHandle(WorldContextObject))
	{
		EOS_Friends_QueryFriendsOptions FriendsOptions = { };
		FriendsOptions.ApiVersion = Options.ApiVersion;
		FriendsOptions.LocalUserId = Options.LocalUserId;

		FFriendsQueryFriendsCallback* CallbackObj = new FFriendsQueryFriendsCallback(Callback);
		EOS_Friends_QueryFriends(GetFriendsHandle(WorldContextObject), &FriendsOptions, CallbackObj, Internal_OnQueryFriendsCallback);
	}
}

void UCoreFriends::EOSFriendsSendInvite(UObject* WorldContextObject, FEOSFriendsSendInviteOptions Options, const FOnFriendsSendInviteCallback& Callback)
{
	LogVerbose("");

	if (GetFriendsHandle(WorldContextObject))
	{
		EOS_Friends_SendInviteOptions FriendsOptions = { };
		FriendsOptions.ApiVersion = Options.ApiVersion;
		FriendsOptions.LocalUserId = Options.LocalUserId;
		FriendsOptions.TargetUserId = Options.TargetUserId;

		FFriendsSendInviteCallback* CallbackObj = new FFriendsSendInviteCallback(Callback);
		EOS_Friends_SendInvite(GetFriendsHandle(WorldContextObject), &FriendsOptions, CallbackObj, Internal_OnSendInviteCallback);
	}
}

void UCoreFriends::EOSFriendsAcceptInvite(UObject* WorldContextObject, FEOSFriendsAcceptInviteOptions Options, const FOnFriendsAcceptInviteCallback& Callback)
{
	LogVerbose("");

	if (GetFriendsHandle(WorldContextObject))
	{
		EOS_Friends_AcceptInviteOptions FriendsOptions = { };
		FriendsOptions.ApiVersion = Options.ApiVersion;
		FriendsOptions.LocalUserId = Options.LocalUserId;
		FriendsOptions.TargetUserId = Options.TargetUserId;

		FFriendsAcceptInviteCallback* CallbackObj = new FFriendsAcceptInviteCallback(Callback);
		EOS_Friends_AcceptInvite(GetFriendsHandle(WorldContextObject), &FriendsOptions, CallbackObj, Internal_OnAcceptInviteCallback);
	}
}

void UCoreFriends::EOSFriendsRejectInvite(UObject* WorldContextObject, FEOSFriendsRejectInviteOptions Options, const FOnFriendsRejectInviteCallback& Callback)
{
	LogVerbose("");

	if (GetFriendsHandle(WorldContextObject))
	{
		EOS_Friends_RejectInviteOptions FriendsOptions = { };
		FriendsOptions.ApiVersion = Options.ApiVersion;
		FriendsOptions.LocalUserId = Options.LocalUserId;
		FriendsOptions.TargetUserId = Options.TargetUserId;

		FFriendsRejectInviteCallback* CallbackObj = new FFriendsRejectInviteCallback(Callback);
		EOS_Friends_RejectInvite(GetFriendsHandle(WorldContextObject), &FriendsOptions, CallbackObj, Internal_OnRejectInviteCallback);
	}
}

int32 UCoreFriends::EOSFriendsGetFriendsCount(UObject* WorldContextObject, FEOSFriendsGetFriendsCountOptions Options)
{
	LogVerbose("");

	int32 Result = 0;

	if (GetFriendsHandle(WorldContextObject))
	{
		EOS_Friends_GetFriendsCountOptions FriendsOptions = { };
		FriendsOptions.ApiVersion = Options.ApiVersion;
		FriendsOptions.LocalUserId = Options.LocalUserId;

		Result = EOS_Friends_GetFriendsCount(GetFriendsHandle(WorldContextObject), &FriendsOptions);
	}

	return Result;
}

FEOSEpicAccountId UCoreFriends::EOSFriendsGetFriendAtIndex(UObject* WorldContextObject, FEOSFriendsGetFriendAtIndexOptions Options)
{
	LogVerbose("");

	FEOSEpicAccountId Id;

	if (GetFriendsHandle(WorldContextObject))
	{
		EOS_Friends_GetFriendAtIndexOptions FriendsOptions = { };
		FriendsOptions.ApiVersion = Options.ApiVersion;
		FriendsOptions.LocalUserId = Options.LocalUserId;
		FriendsOptions.Index = Options.Index;

		Id = EOS_Friends_GetFriendAtIndex(GetFriendsHandle(WorldContextObject), &FriendsOptions);
	}

	return Id;
}

EEOSEFriendsStatus UCoreFriends::EOSFriendsGetStatus(UObject* WorldContextObject, FEOSFriendsGetStatusOptions Options)
{
	LogVerbose("");

	EEOSEFriendsStatus Status = EEOSEFriendsStatus::EOS_FS_NotFriends;

	if (GetFriendsHandle(WorldContextObject))
	{
		EOS_Friends_GetStatusOptions FriendsOptions = { };
		FriendsOptions.ApiVersion = Options.ApiVersion;
		FriendsOptions.LocalUserId = Options.LocalUserId;
		FriendsOptions.TargetUserId = Options.TargetUserId;

		Status = static_cast<EEOSEFriendsStatus>(EOS_Friends_GetStatus(GetFriendsHandle(WorldContextObject), &FriendsOptions));
	}

	return Status;
}

FEOSNotificationId UCoreFriends::EOSFriendsAddNotifyFriendsUpdate(UObject* WorldContextObject, const FOnFriendsUpdateCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetFriendsHandle(WorldContextObject))
	{
		const EOS_Friends_AddNotifyFriendsUpdateOptions FriendsOptions = { EOS_FRIENDS_ADDNOTIFYFRIENDSUPDATE_API_LATEST };

		FFriendsUpdateCallback* CallbackObj = new FFriendsUpdateCallback(WorldContextObject, Callback);

		Id = EOS_Friends_AddNotifyFriendsUpdate(GetFriendsHandle(WorldContextObject), &FriendsOptions, CallbackObj, ([](const EOS_Friends_OnFriendsUpdateInfo* Data)
		{
			const FFriendsUpdateCallback* CallbackObj = static_cast<FFriendsUpdateCallback*>(Data->ClientData);
			CallbackObj->Callback.ExecuteIfBound(*Data);
		}));

		GetFriends(WorldContextObject)->m_FriendsUpdateCallbacks.Add(Id, CallbackObj);
	}

	return Id;
}

void UCoreFriends::EOSFriendsRemoveNotifyFriendsUpdate(UObject* WorldContextObject, FEOSNotificationId Id)
{
	LogVerbose("");

	if (GetFriendsHandle(WorldContextObject))
	{
		EOS_Friends_RemoveNotifyFriendsUpdate(GetFriendsHandle(WorldContextObject), Id);
		GetFriends(WorldContextObject)->m_FriendsUpdateCallbacks.Remove(Id);
	}
}

int32 UCoreFriends::EOSFriendsGetBlockedUsersCount(UObject* WorldContextObject, FEOSFriendsGetBlockedUsersCountOptions Options)
{
	LogVerbose("");

	int32 Result = 0;

	if (GetFriendsHandle(WorldContextObject))
	{
		EOS_Friends_GetBlockedUsersCountOptions FriendsOptions = { };
		FriendsOptions.ApiVersion = EOS_FRIENDS_GETBLOCKEDUSERSCOUNT_API_LATEST;
		FriendsOptions.LocalUserId = Options.LocalUserId;

		Result = EOS_Friends_GetBlockedUsersCount(GetFriendsHandle(WorldContextObject), &FriendsOptions);
	}

	return Result;
}

FEOSEpicAccountId UCoreFriends::EOSFriendsGetBlockedUserAtIndex(UObject* WorldContextObject, FEOSFriendsGetBlockedUserAtIndexOptions Options)
{
	LogVerbose("");

	FEOSEpicAccountId Id;

	if (GetFriendsHandle(WorldContextObject))
	{
		EOS_Friends_GetBlockedUserAtIndexOptions FriendsOptions = { };
		FriendsOptions.ApiVersion = EOS_FRIENDS_GETBLOCKEDUSERATINDEX_API_LATEST;
		FriendsOptions.LocalUserId = Options.LocalUserId;
		FriendsOptions.Index = Options.Index;

		Id = EOS_Friends_GetBlockedUserAtIndex(GetFriendsHandle(WorldContextObject), &FriendsOptions);
	}

	return Id;
}

FEOSNotificationId UCoreFriends::EOSFriendsAddNotifyBlockedUsersUpdate(UObject* WorldContextObject, const FOnFriendsBlockedUsersUpdateCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetFriendsHandle(WorldContextObject))
	{
		const EOS_Friends_AddNotifyBlockedUsersUpdateOptions FriendsOptions = { EOS_FRIENDS_ADDNOTIFYBLOCKEDUSERSUPDATE_API_LATEST };

		FFriendsOnBlockedUsersUpdateCallback* CallbackObj = new FFriendsOnBlockedUsersUpdateCallback(WorldContextObject, Callback);

		Id = EOS_Friends_AddNotifyBlockedUsersUpdate(GetFriendsHandle(WorldContextObject), &FriendsOptions, CallbackObj, ([](const EOS_Friends_OnBlockedUsersUpdateInfo* Data)
		{
			const FFriendsOnBlockedUsersUpdateCallback* CallbackObj = static_cast<FFriendsOnBlockedUsersUpdateCallback*>(Data->ClientData);
			CallbackObj->Callback.ExecuteIfBound(*Data);
		}));

		GetFriends(WorldContextObject)->m_FriendsBlockedUsersUpdateCallbacks.Add(Id, CallbackObj);
	}

	return Id;
}

void UCoreFriends::Internal_OnQueryFriendsCallback(const EOS_Friends_QueryFriendsCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FFriendsQueryFriendsCallback* CallbackObj = static_cast<FFriendsQueryFriendsCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreFriends::Internal_OnSendInviteCallback(const EOS_Friends_SendInviteCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FFriendsSendInviteCallback* CallbackObj = static_cast<FFriendsSendInviteCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreFriends::Internal_OnAcceptInviteCallback(const EOS_Friends_AcceptInviteCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FFriendsAcceptInviteCallback* CallbackObj = static_cast<FFriendsAcceptInviteCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreFriends::Internal_OnRejectInviteCallback(const EOS_Friends_RejectInviteCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FFriendsRejectInviteCallback* CallbackObj = static_cast<FFriendsRejectInviteCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}