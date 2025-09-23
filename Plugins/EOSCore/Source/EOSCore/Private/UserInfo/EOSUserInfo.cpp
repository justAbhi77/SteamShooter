/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "UserInfo/EOSUserInfo.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLogging.h"

UCoreUserInfo* UCoreUserInfo::GetUserInfo(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (GetUserInfoHandle(WorldContextObject))
		{
			if (UWorld* World = WorldContextObject->GetWorld())
			{
				if (UGameInstance* GameInstance = World->GetGameInstance())
				{
					return GameInstance->GetSubsystem<UCoreUserInfo>();
				}
			}	
		}
	}
	return nullptr;
}

void UCoreUserInfo::EOSUserInfoQueryUserInfo(UObject* WorldContextObject, FEOSUserInfoQueryUserInfoOptions Options, const FOnQueryUserInfoCallback& Callback)
{
	LogVerbose("");

	if (GetUserInfoHandle(WorldContextObject))
	{
		EOS_UserInfo_QueryUserInfoOptions UserInfoOptions = { };
		UserInfoOptions.ApiVersion = EOS_USERINFO_QUERYUSERINFO_API_LATEST;
		UserInfoOptions.LocalUserId = Options.LocalUserId;
		UserInfoOptions.TargetUserId = Options.TargetUserId;

		FQueryUserInfoCallback* CallbackObj = new FQueryUserInfoCallback(Callback);
		EOS_UserInfo_QueryUserInfo(GetUserInfoHandle(WorldContextObject), &UserInfoOptions, CallbackObj, Internal_OnQueryUserInfoCallback);
	}
}

void UCoreUserInfo::EOSUserInfoQueryUserInfoByDisplayName(UObject* WorldContextObject, FEOSUserInfoQueryUserInfoByDisplayNameOptions Options, const FOnQueryUserInfoByDisplayNameCallback& Callback)
{
	LogVerbose("");

	if (GetUserInfoHandle(WorldContextObject))
	{
		const FTCHARToUTF8 DisplayName(*Options.DisplayName);
		
		EOS_UserInfo_QueryUserInfoByDisplayNameOptions UserInfoOptions = { };
		UserInfoOptions.ApiVersion = Options.ApiVersion;
		UserInfoOptions.LocalUserId = Options.LocalUserId;
		UserInfoOptions.DisplayName = DisplayName.Get();
		
		FQueryUserInfoByDisplayNameCallback* CallbackObj = new FQueryUserInfoByDisplayNameCallback(Callback);
		EOS_UserInfo_QueryUserInfoByDisplayName(GetUserInfoHandle(WorldContextObject), &UserInfoOptions, CallbackObj, Internal_OnQueryUserInfoByDisplayNameCallback);
	}
}

void UCoreUserInfo::EOSUserInfoQueryUserInfoByExternalAccount(UObject* WorldContextObject, FEOSUserInfoQueryUserInfoByExternalAccountOptions Options, const FOnQueryUserInfoByExternalAccountCallback& Callback)
{
	LogVerbose("");

	if (GetUserInfoHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ExternalAccountId(*Options.ExternalAccountId);
		
		EOS_UserInfo_QueryUserInfoByExternalAccountOptions UserInfoOptions = { };
		UserInfoOptions.ApiVersion = Options.ApiVersion;
		UserInfoOptions.LocalUserId = Options.LocalUserId;
		UserInfoOptions.ExternalAccountId = ExternalAccountId.Get();
		UserInfoOptions.AccountType = static_cast<EOS_EExternalAccountType>(Options.AccountType);
		
		FQueryUserInfoByExternalAccountCallback* CallbackObj = new FQueryUserInfoByExternalAccountCallback(Callback);
		EOS_UserInfo_QueryUserInfoByExternalAccount(GetUserInfoHandle(WorldContextObject), &UserInfoOptions, CallbackObj, Internal_OnQueryUserInfoByExternalAccountCallback);
	}
}

EOSResult UCoreUserInfo::EOSUserInfoCopyUserInfo(UObject* WorldContextObject, FEOSUserInfoCopyUserInfoOptions Options, FEOSUserInfo& OutUserInfo)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetUserInfoHandle(WorldContextObject))
	{
		EOS_UserInfo_CopyUserInfoOptions UserInfoOptions = { };
		UserInfoOptions.ApiVersion = Options.ApiVersion;
		UserInfoOptions.LocalUserId = Options.LocalUserId;
		UserInfoOptions.TargetUserId = Options.TargetUserId;

		EOS_UserInfo* UserInfo = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_UserInfo_CopyUserInfo(GetUserInfoHandle(WorldContextObject), &UserInfoOptions, &UserInfo));

		if (Result == EOSResult::EOS_Success)
		{
			OutUserInfo = *UserInfo;
		}
		EOS_UserInfo_Release(UserInfo);
	}

	return Result;
}

int32 UCoreUserInfo::EOSUserInfoGetExternalUserInfoCount(UObject* WorldContextObject, FEOSUserInfoGetExternalUserInfoCountOptions Options)
{
	LogVerbose("");

	int32 Result = 0;

	if (GetUserInfoHandle(WorldContextObject))
	{
		EOS_UserInfo_GetExternalUserInfoCountOptions UserInfoOptions = { };
		UserInfoOptions.ApiVersion = Options.ApiVersion;
		UserInfoOptions.LocalUserId = Options.LocalUserId;
		UserInfoOptions.TargetUserId = Options.TargetUserId;

		Result = EOS_UserInfo_GetExternalUserInfoCount(GetUserInfoHandle(WorldContextObject), &UserInfoOptions);
	}

	return Result;
}

EOSResult UCoreUserInfo::EOSUserInfoCopyExternalUserInfoByIndex(UObject* WorldContextObject, FEOSUserInfoCopyExternalUserInfoByIndexOptions Options, FEOSUserInfoExternalUserInfo& OutExternalUserInfo)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetUserInfoHandle(WorldContextObject))
	{
		EOS_UserInfo_CopyExternalUserInfoByIndexOptions UserInfoOptions = { };
		UserInfoOptions.ApiVersion = Options.ApiVersion;
		UserInfoOptions.LocalUserId = Options.LocalUserId;
		UserInfoOptions.TargetUserId = Options.TargetUserId;
		UserInfoOptions.Index = static_cast<uint32_t>(Options.Index);
		
		EOS_UserInfo_ExternalUserInfo* ExternalUserInfo = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_UserInfo_CopyExternalUserInfoByIndex(GetUserInfoHandle(WorldContextObject), &UserInfoOptions, &ExternalUserInfo));

		if (Result == EOSResult::EOS_Success)
		{
			OutExternalUserInfo = *ExternalUserInfo;
			EOS_UserInfo_ExternalUserInfo_Release(ExternalUserInfo);
		}
	}

	return Result;
}

EOSResult UCoreUserInfo::EOSUserInfoCopyExternalUserInfoByAccountType(UObject* WorldContextObject, FEOSUserInfoCopyExternalUserInfoByAccountTypeOptions Options, FEOSUserInfoExternalUserInfo& OutExternalUserInfo)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetUserInfoHandle(WorldContextObject))
	{
		EOS_UserInfo_CopyExternalUserInfoByAccountTypeOptions UserInfoOptions = { };
		UserInfoOptions.ApiVersion = Options.ApiVersion;
		UserInfoOptions.LocalUserId = Options.LocalUserId;
		UserInfoOptions.TargetUserId = Options.TargetUserId;
		UserInfoOptions.AccountType = static_cast<EOS_EExternalAccountType>(Options.AccountType);
		
		EOS_UserInfo_ExternalUserInfo* ExternalUserInfo = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_UserInfo_CopyExternalUserInfoByAccountType(GetUserInfoHandle(WorldContextObject), &UserInfoOptions, &ExternalUserInfo));

		if (Result == EOSResult::EOS_Success)
		{
			OutExternalUserInfo = *ExternalUserInfo;
			EOS_UserInfo_ExternalUserInfo_Release(ExternalUserInfo);
		}
	}

	return Result;
}

EOSResult UCoreUserInfo::EOSUserInfoCopyExternalUserInfoByAccountId(UObject* WorldContextObject, FEOSUserInfoCopyExternalUserInfoByAccountIdOptions Options, FEOSUserInfoExternalUserInfo& OutExternalUserInfo)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetUserInfoHandle(WorldContextObject))
	{
		const FTCHARToUTF8 AccountId(*Options.AccountId);
		
		EOS_UserInfo_CopyExternalUserInfoByAccountIdOptions UserInfoOptions = { };
		UserInfoOptions.ApiVersion = Options.ApiVersion;
		UserInfoOptions.LocalUserId = Options.LocalUserId;
		UserInfoOptions.TargetUserId = Options.TargetUserId;
		UserInfoOptions.AccountId = AccountId.Get();
		
		EOS_UserInfo_ExternalUserInfo* ExternalUserInfo = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_UserInfo_CopyExternalUserInfoByAccountId(GetUserInfoHandle(WorldContextObject), &UserInfoOptions, &ExternalUserInfo));

		if (Result == EOSResult::EOS_Success)
		{
			OutExternalUserInfo = *ExternalUserInfo;
			EOS_UserInfo_ExternalUserInfo_Release(ExternalUserInfo);
		}

	}

	return Result;
}

EOSResult UCoreUserInfo::EOSUserInfoCopyBestDisplayName(UObject* WorldContextObject, FEOSUserInfoCopyBestDisplayNameOptions Options, FEOSUserInfoBestDisplayName& OutBestDisplayName)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetUserInfoHandle(WorldContextObject))
	{
		EOS_UserInfo_CopyBestDisplayNameOptions UserInfoOptions = { };
		UserInfoOptions.ApiVersion = EOS_USERINFO_COPYBESTDISPLAYNAME_API_LATEST;
		UserInfoOptions.LocalUserId = Options.LocalUserId;
		UserInfoOptions.TargetUserId = Options.TargetUserId;
		
		EOS_UserInfo_BestDisplayName* BestDisplayName = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_UserInfo_CopyBestDisplayName(GetUserInfoHandle(WorldContextObject), &UserInfoOptions, &BestDisplayName));

		if (Result == EOSResult::EOS_Success)
		{
			OutBestDisplayName = *BestDisplayName;
			EOS_UserInfo_BestDisplayName_Release(BestDisplayName);
		}

	}

	return Result;
}

EOSResult UCoreUserInfo::EOSUserInfoCopyBestDisplayNameWithPlatform(UObject* WorldContextObject, FEOSUserInfoCopyBestDisplayNameWithPlatformOptions Options, FEOSUserInfoBestDisplayName& OutBestDisplayName)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetUserInfoHandle(WorldContextObject))
	{
		EOS_UserInfo_CopyBestDisplayNameWithPlatformOptions UserInfoOptions = { };
		UserInfoOptions.ApiVersion = EOS_USERINFO_COPYBESTDISPLAYNAME_API_LATEST;
		UserInfoOptions.LocalUserId = Options.LocalUserId;
		UserInfoOptions.TargetUserId = Options.TargetUserId;
		UserInfoOptions.TargetPlatformType = Options.TargetPlatformType;
		
		EOS_UserInfo_BestDisplayName* BestDisplayName = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_UserInfo_CopyBestDisplayNameWithPlatform(GetUserInfoHandle(WorldContextObject), &UserInfoOptions, &BestDisplayName));

		if (Result == EOSResult::EOS_Success)
		{
			OutBestDisplayName = *BestDisplayName;
			EOS_UserInfo_BestDisplayName_Release(BestDisplayName);
		}

	}

	return Result;
}

int32 UCoreUserInfo::EOSUserInfoGetLocalPlatformType(UObject* WorldContextObject)
{
	LogVerbose("");

	int32 Result = 0;

	if (GetUserInfoHandle(WorldContextObject))
	{
		EOS_UserInfo_GetLocalPlatformTypeOptions UserInfoOptions = { };
		UserInfoOptions.ApiVersion = EOS_USERINFO_GETLOCALPLATFORMTYPE_API_LATEST;

		Result = EOS_UserInfo_GetLocalPlatformType(GetUserInfoHandle(WorldContextObject), &UserInfoOptions);
	}

	return Result;
}

void UCoreUserInfo::Internal_OnQueryUserInfoCallback(const EOS_UserInfo_QueryUserInfoCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FQueryUserInfoCallback* CallbackObj = static_cast<FQueryUserInfoCallback*>(Data->ClientData);
	check(CallbackObj);

	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}

}

void UCoreUserInfo::Internal_OnQueryUserInfoByDisplayNameCallback(const EOS_UserInfo_QueryUserInfoByDisplayNameCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FQueryUserInfoByDisplayNameCallback* CallbackObj = static_cast<FQueryUserInfoByDisplayNameCallback*>(Data->ClientData);
	check(CallbackObj);

	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreUserInfo::Internal_OnQueryUserInfoByExternalAccountCallback(const EOS_UserInfo_QueryUserInfoByExternalAccountCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FQueryUserInfoByExternalAccountCallback* CallbackObj = static_cast<FQueryUserInfoByExternalAccountCallback*>(Data->ClientData);
	check(CallbackObj);

	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}
