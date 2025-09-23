/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "Connect/EOSConnect.h"

#include "EOSCoreSharedTypes.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLogging.h"

void UCoreConnect::Deinitialize()
{
	LogVerbose("");

	for (auto Element: m_AuthExpirationCallbacks)
	{
		EOS_Connect_RemoveNotifyAuthExpiration(GetConnectHandle(this), Element.Key);
	}

	for (auto Element: m_LoginStatusChangedCallbacks)
	{
		EOS_Connect_RemoveNotifyLoginStatusChanged(GetConnectHandle(this), Element.Key);
	}

	m_AuthExpirationCallbacks.Empty();
	m_LoginStatusChangedCallbacks.Empty();
}

UCoreConnect* UCoreConnect::GetConnect(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (GetConnectHandle(WorldContextObject))
		{
			if (UWorld* World = WorldContextObject->GetWorld())
			{
				if (UGameInstance* GameInstance = World->GetGameInstance())
				{
					return GameInstance->GetSubsystem<UCoreConnect>();
				}
			}	
		}
	}
	return nullptr;
}

void UCoreConnect::EOSConnectLogin(UObject* WorldContextObject, FEOSConnectLoginOptions Options, const FOnLoginCallback& Callback)
{
	LogVerbose("");

	if (GetConnectHandle(WorldContextObject))
	{
		FConnectCredentials Credentials(Options.Credentials.AccessToken);
		Credentials.Type = static_cast<EOS_EExternalCredentialType>(Options.Credentials.Type);

		FConnectUserLoginInfo UserLoginInfo(Options.UserLoginInfo.DisplayName);

		EOS_Connect_LoginOptions LoginOptions = { };
		LoginOptions.ApiVersion = Options.ApiVersion;
		LoginOptions.Credentials = &Credentials;

		EEOSEExternalCredentialType Type = Options.Credentials.Type;

		if (Type == EEOSEExternalCredentialType::EOS_ECT_APPLE_ID_TOKEN ||
			Type == EEOSEExternalCredentialType::EOS_ECT_NINTENDO_ID_TOKEN || 
			Type == EEOSEExternalCredentialType::EOS_ECT_NINTENDO_NSA_ID_TOKEN || 
			Type == EEOSEExternalCredentialType::EOS_ECT_DEVICEID_ACCESS_TOKEN || 
			Type == EEOSEExternalCredentialType::EOS_ECT_OCULUS_USERID_NONCE)
		{
			LoginOptions.UserLoginInfo = &UserLoginInfo;
		}
		else
		{
			LoginOptions.UserLoginInfo = nullptr;
		}

		FLoginCallback* CallbackObj = new FLoginCallback(Callback);
		EOS_Connect_Login(GetConnectHandle(WorldContextObject), &LoginOptions, CallbackObj, Internal_OnLoginCallback);
	}
}

void UCoreConnect::EOSConnectCreateUser(UObject* WorldContextObject, FEOSConnectCreateUserOptions Options, const FOnCreateUserCallback& Callback)
{
	LogVerbose("");

	if (GetConnectHandle(WorldContextObject))
	{
		EOS_Connect_CreateUserOptions ConnectOptions = { };
		ConnectOptions.ApiVersion = EOS_CONNECT_CREATEUSER_API_LATEST;
		ConnectOptions.ContinuanceToken = Options.ContinuanceToken;

		FCreateUserCallback* CallbackObj = new FCreateUserCallback(Callback);
		EOS_Connect_CreateUser(GetConnectHandle(WorldContextObject), &ConnectOptions, CallbackObj, Internal_OnCreateUserCallback);
	}
}

void UCoreConnect::EOSConnectLinkAccount(UObject* WorldContextObject, FEOSConnectLinkAccountOptions Options, const FOnLinkAccountCallback& Callback)
{
	LogVerbose("");

	if (GetConnectHandle(WorldContextObject))
	{
		EOS_Connect_LinkAccountOptions ConnectOptions = { };
		ConnectOptions.ApiVersion = Options.ApiVersion;
		ConnectOptions.LocalUserId = Options.LocalUserId;
		ConnectOptions.ContinuanceToken = Options.ContinuanceToken;

		FLinkAccountCallback* CallbackObj = new FLinkAccountCallback(Callback);
		EOS_Connect_LinkAccount(GetConnectHandle(WorldContextObject), &ConnectOptions, CallbackObj, Internal_OnLinkAccountCallback);
	}
}

void UCoreConnect::EOSConnectUnlinkAccount(UObject* WorldContextObject, FEOSConnectUnlinkAccountOptions Options, const FOnUnlinkAccountCallback& Callback)
{
	LogVerbose("");

	if (GetConnectHandle(WorldContextObject))
	{
		EOS_Connect_UnlinkAccountOptions ConnectOptions = { };
		ConnectOptions.ApiVersion = Options.ApiVersion;
		ConnectOptions.LocalUserId = Options.LocalUserId;

		FUnlinkAccountCallback* CallbackObj = new FUnlinkAccountCallback(Callback);
		EOS_Connect_UnlinkAccount(GetConnectHandle(WorldContextObject), &ConnectOptions, CallbackObj, Internal_OnUnlinkAccountCallback);
	}
}

void UCoreConnect::EOSConnectCreateDeviceId(UObject* WorldContextObject, FEOSConnectCreateDeviceIdOptions Options, const FOnCreateDeviceIdCallback& Callback)
{
	LogVerbose("");

	if (GetConnectHandle(WorldContextObject))
	{
		const FConnectCreateDeviceIdOptions ConnectOptions(Options.DeviceModel);
		
		FCreateDeviceIdCallback* CallbackObj = new FCreateDeviceIdCallback(Callback);
		EOS_Connect_CreateDeviceId(GetConnectHandle(WorldContextObject), &ConnectOptions, CallbackObj, Internal_OnCreateDeviceIdCallback);
	}
}

void UCoreConnect::EOSConnectDeleteDeviceId(UObject* WorldContextObject, FEOSConnectDeleteDeviceIdOptions Options, const FOnDeleteDeviceIdCallback& Callback)
{
	LogVerbose("");

	if (GetConnectHandle(WorldContextObject))
	{
		EOS_Connect_DeleteDeviceIdOptions ConnectOptions = { };
		ConnectOptions.ApiVersion = Options.ApiVersion;

		FDeleteDeviceIdCallback* CallbackObj = new FDeleteDeviceIdCallback(Callback);
		EOS_Connect_DeleteDeviceId(GetConnectHandle(WorldContextObject), &ConnectOptions, CallbackObj, Internal_OnDeleteDeviceIdCallback);
	}
}

void UCoreConnect::EOSConnectTransferDeviceIdAccount(UObject* WorldContextObject, FEOSConnectTransferDeviceIdAccountOptions Options, const FOnTransferDeviceIdAccountCallback& Callback)
{
	LogVerbose("");

	if (GetConnectHandle(WorldContextObject))
	{
		EOS_Connect_TransferDeviceIdAccountOptions ConnectOptions = { };
		ConnectOptions.ApiVersion = EOS_CONNECT_TRANSFERDEVICEIDACCOUNT_API_LATEST;

		FTransferDeviceIdAccountCallback* CallbackObj = new FTransferDeviceIdAccountCallback(Callback);
		EOS_Connect_TransferDeviceIdAccount(GetConnectHandle(WorldContextObject), &ConnectOptions, CallbackObj, Internal_OnTransferDeviceIdAccount);
	}
}

void UCoreConnect::EOSConnectQueryExternalAccountMappings(UObject* WorldContextObject, FEOSConnectQueryExternalAccountMappingsOptions Options, const FOnQueryExternalAccountMappingsCallback& Callback)
{
	LogVerbose("");

	if (GetConnectHandle(WorldContextObject))
	{
		FQueryExternalAccountMappingsOperation QueryExternalAccountMappingsOperation(Options.ExternalAccountIds.Num(), Options.LocalUserId, static_cast<EOS_EExternalAccountType>(Options.AccountIdType));

		for (int32 i = 0; i < Options.ExternalAccountIds.Num(); i++)
		{
			FCStringAnsi::Strncpy(QueryExternalAccountMappingsOperation.PointerArray[i], TCHAR_TO_UTF8(*Options.ExternalAccountIds[i]), EOS_CONNECT_EXTERNAL_ACCOUNT_ID_MAX_LENGTH);
		}

		FQueryExternalAccountMappingsCallback* CallbackObj = new FQueryExternalAccountMappingsCallback(Callback);
		EOS_Connect_QueryExternalAccountMappings(GetConnectHandle(WorldContextObject), &QueryExternalAccountMappingsOperation, CallbackObj, Internal_OnQueryExternalAccountMappingsCallback);
	}
}

void UCoreConnect::EOSConnectQueryProductUserIdMappings(UObject* WorldContextObject, FEOSConnectQueryProductUserIdMappingsOptions Options, const FOnQueryProductUserIdMappingsCallback& Callback)
{
	LogVerbose("");

	if (GetConnectHandle(WorldContextObject))
	{
		FConnectQueryProductUserIdMappingsOptions ConnectOptions(Options.ProductUserIds.Num());
		ConnectOptions.LocalUserId = Options.LocalUserId;
		ConnectOptions.AccountIdType_DEPRECATED = Options.AccountIdType_DEPRECATED;

		for (int32 i = 0; i < Options.ProductUserIds.Num(); i++)
		{
			ConnectOptions.PointerArray[i] = Options.ProductUserIds[i];
		}

		FQueryProductUserIdMappingsCallback* CallbackObj = new FQueryProductUserIdMappingsCallback(Callback);
		EOS_Connect_QueryProductUserIdMappings(GetConnectHandle(WorldContextObject), &ConnectOptions, CallbackObj, Internal_OnQueryProductUserIdMappingsCallback);
	}
}

FEOSProductUserId UCoreConnect::EOSConnectGetExternalAccountMapping(UObject* WorldContextObject, FEOSConnectGetExternalAccountMappingsOptions Options)
{
	LogVerbose("");

	FEOSProductUserId ProductUserId;

	if (GetConnectHandle(WorldContextObject))
	{
		FGetExternalAccountMappingsOptions ConnectOptions(Options.TargetExternalUserId);
		ConnectOptions.LocalUserId = Options.LocalUserId;
		ConnectOptions.AccountIdType = static_cast<EOS_EExternalAccountType>(Options.AccountIdType);

		ProductUserId = EOS_Connect_GetExternalAccountMapping(GetConnectHandle(WorldContextObject), &ConnectOptions);
	}

	return ProductUserId;
}

EOSResult UCoreConnect::EOSConnectGetProductUserIdMapping(UObject* WorldContextObject, FEOSConnectGetProductUserIdMappingOptions Options, FString& OutString)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_UnrecognizedResponse;

	if (GetConnectHandle(WorldContextObject))
	{
		EOS_Connect_GetProductUserIdMappingOptions ConnectOptions = { };
		ConnectOptions.ApiVersion = Options.ApiVersion;
		ConnectOptions.LocalUserId = Options.LocalUserId;
		ConnectOptions.AccountIdType = static_cast<EOS_EExternalAccountType>(Options.AccountIdType);
		ConnectOptions.TargetProductUserId = Options.TargetProductUserId;

		char Buffer[EOS_CONNECT_EXTERNAL_ACCOUNT_ID_MAX_LENGTH];
		int32_t BufferSize = EOS_CONNECT_EXTERNAL_ACCOUNT_ID_MAX_LENGTH;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Connect_GetProductUserIdMapping(GetConnectHandle(WorldContextObject), &ConnectOptions, Buffer, &BufferSize));

		if (Result == EOSResult::EOS_Success)
		{
			OutString = Buffer;
		}
	}

	return Result;
}

int32 UCoreConnect::EOSConnectGetProductUserExternalAccountCount(UObject* WorldContextObject, FEOSConnectGetProductUserExternalAccountCountOptions Options)
{
	LogVerbose("");

	int32 Result = 0;

	if (GetConnectHandle(WorldContextObject))
	{
		EOS_Connect_GetProductUserExternalAccountCountOptions ConnectOptions = { };
		ConnectOptions.ApiVersion = Options.ApiVersion;
		ConnectOptions.TargetUserId = Options.TargetUserId;

		Result = EOS_Connect_GetProductUserExternalAccountCount(GetConnectHandle(WorldContextObject), &ConnectOptions);
	}

	return Result;
}

EOSResult UCoreConnect::EOSConnectCopyProductUserExternalAccountByIndex(UObject* WorldContextObject, FEOSConnectCopyProductUserExternalAccountByIndexOptions Options, FEOSConnectExternalAccountInfo& OutExternalAccountInfo)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_UnrecognizedResponse;

	if (GetConnectHandle(WorldContextObject))
	{
		EOS_Connect_CopyProductUserExternalAccountByIndexOptions ConnectOptions = { };
		ConnectOptions.ApiVersion = Options.ApiVersion;
		ConnectOptions.TargetUserId = Options.TargetUserId;
		ConnectOptions.ExternalAccountInfoIndex = static_cast<uint32_t>(Options.ExternalAccountInfoIndex);

		EOS_Connect_ExternalAccountInfo* AccountInfo = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Connect_CopyProductUserExternalAccountByIndex(GetConnectHandle(WorldContextObject), &ConnectOptions, &AccountInfo));

		if (Result == EOSResult::EOS_Success)
		{
			OutExternalAccountInfo = *AccountInfo;
		}

		EOS_Connect_ExternalAccountInfo_Release(AccountInfo);
	}

	return Result;
}

EOSResult UCoreConnect::EOSConnectCopyProductUserExternalAccountByAccountType(UObject* WorldContextObject, FEOSConnectCopyProductUserExternalAccountByAccountTypeOptions Options, FEOSConnectExternalAccountInfo& OutExternalAccountInfo)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_UnrecognizedResponse;

	if (GetConnectHandle(WorldContextObject))
	{
		EOS_Connect_CopyProductUserExternalAccountByAccountTypeOptions ConnectOptions = { };
		ConnectOptions.ApiVersion = Options.ApiVersion;
		ConnectOptions.TargetUserId = Options.TargetUserId;
		ConnectOptions.AccountIdType = static_cast<EOS_EExternalAccountType>(Options.AccountIdType);

		EOS_Connect_ExternalAccountInfo* AccountInfo = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Connect_CopyProductUserExternalAccountByAccountType(GetConnectHandle(WorldContextObject), &ConnectOptions, &AccountInfo));

		if (Result == EOSResult::EOS_Success)
		{
			OutExternalAccountInfo = *AccountInfo;
		}

		EOS_Connect_ExternalAccountInfo_Release(AccountInfo);
	}

	return Result;
}

EOSResult UCoreConnect::EOSConnectCopyProductUserExternalAccountByAccountId(UObject* WorldContextObject, FEOSConnectCopyProductUserExternalAccountByAccountIdOptions Options, FEOSConnectExternalAccountInfo& OutExternalAccountInfo)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_UnrecognizedResponse;

	if (GetConnectHandle(WorldContextObject))
	{
		FCopyProductUserExternalAccountByAccountIdOptions ConnectOptions(Options.AccountId);
		ConnectOptions.TargetUserId = Options.TargetUserId;

		EOS_Connect_ExternalAccountInfo* AccountInfo = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Connect_CopyProductUserExternalAccountByAccountId(GetConnectHandle(WorldContextObject), &ConnectOptions, &AccountInfo));

		if (Result == EOSResult::EOS_Success)
		{
			OutExternalAccountInfo = *AccountInfo;
		}

		EOS_Connect_ExternalAccountInfo_Release(AccountInfo);
	}

	return Result;
}

EOSResult UCoreConnect::EOSConnectCopyProductUserInfo(UObject* WorldContextObject, FEOSConnectCopyProductUserInfoOptions Options, FEOSConnectExternalAccountInfo& OutExternalAccountInfo)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_UnrecognizedResponse;

	if (GetConnectHandle(WorldContextObject))
	{
		EOS_Connect_CopyProductUserInfoOptions ConnectOptions = { };
		ConnectOptions.ApiVersion = Options.ApiVersion;
		ConnectOptions.TargetUserId = Options.TargetUserId;

		EOS_Connect_ExternalAccountInfo* AccountInfo = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Connect_CopyProductUserInfo(GetConnectHandle(WorldContextObject), &ConnectOptions, &AccountInfo));

		if (Result == EOSResult::EOS_Success)
		{
			OutExternalAccountInfo = *AccountInfo;
		}

		EOS_Connect_ExternalAccountInfo_Release(AccountInfo);
	}

	return Result;
}

int32 UCoreConnect::EOSConnectGetLoggedInUsersCount(UObject* WorldContextObject)
{
	LogVerbose("");

	int32 Result = 0;

	if (GetConnectHandle(WorldContextObject))
	{
		Result = EOS_Connect_GetLoggedInUsersCount(GetConnectHandle(WorldContextObject));
	}

	return Result;
}

FEOSProductUserId UCoreConnect::EOSConnectGetLoggedInUserByIndex(UObject* WorldContextObject, int32 Index)
{
	LogVerbose("");

	FEOSProductUserId Result;

	if (GetConnectHandle(WorldContextObject))
	{
		Result = EOS_Connect_GetLoggedInUserByIndex(GetConnectHandle(WorldContextObject), Index);
	}

	return Result;
}

EEOSELoginStatus UCoreConnect::EOSConnectGetLoginStatus(UObject* WorldContextObject, FEOSProductUserId LocalUserId)
{
	LogVerbose("");

	EEOSELoginStatus Result = EEOSELoginStatus::EOS_LS_NotLoggedIn;

	if (GetConnectHandle(WorldContextObject))
	{
		Result = static_cast<EEOSELoginStatus>(EOS_Connect_GetLoginStatus(GetConnectHandle(WorldContextObject), LocalUserId));
	}

	return Result;
}

FEOSNotificationId UCoreConnect::EOSConnectAddNotifyAuthExpiration(UObject* WorldContextObject, const FOnAuthExpirationCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetConnectHandle(WorldContextObject))
	{
		constexpr EOS_Connect_AddNotifyAuthExpirationOptions ConnectOptions = { EOS_CONNECT_ADDNOTIFYAUTHEXPIRATION_API_LATEST };

		FAuthExpirationCallback* CallbackObj = new FAuthExpirationCallback(Callback);

		Id = EOS_Connect_AddNotifyAuthExpiration(GetConnectHandle(WorldContextObject), &ConnectOptions, CallbackObj, ([](const EOS_Connect_AuthExpirationCallbackInfo* Data)
		{
			FAuthExpirationCallback* CallbackObj = static_cast<FAuthExpirationCallback*>(Data->ClientData);
			CallbackObj->Callback.ExecuteIfBound(*Data);
		}));
		
		GetConnect(WorldContextObject)->m_AuthExpirationCallbacks.Add(Id, CallbackObj);
	}

	return Id;
}

void UCoreConnect::EOSConnectRemoveNotifyAuthExpiration(UObject* WorldContextObject, FEOSNotificationId Id)
{
	LogVerbose("");

	if (GetConnectHandle(WorldContextObject))
	{
		EOS_Connect_RemoveNotifyAuthExpiration(GetConnectHandle(WorldContextObject), Id);
		GetConnect(WorldContextObject)->m_AuthExpirationCallbacks.Remove(Id);
	}
}

FEOSNotificationId UCoreConnect::EOSConnectAddNotifyLoginStatusChanged(UObject* WorldContextObject, const FOnLoginStatusChangedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetConnectHandle(WorldContextObject))
	{
		constexpr EOS_Connect_AddNotifyLoginStatusChangedOptions ConnectOptions = { EOS_CONNECT_ADDNOTIFYLOGINSTATUSCHANGED_API_LATEST };

		FLoginStatusChangedCallback* CallbackObj = new FLoginStatusChangedCallback(Callback);

		Id = EOS_Connect_AddNotifyLoginStatusChanged(GetConnectHandle(WorldContextObject), &ConnectOptions, CallbackObj, ([](const EOS_Connect_LoginStatusChangedCallbackInfo* Data)
		{
			const FLoginStatusChangedCallback* CallbackObj = static_cast<FLoginStatusChangedCallback*>(Data->ClientData);
			CallbackObj->Callback.ExecuteIfBound(*Data);
		}));

		GetConnect(WorldContextObject)->m_LoginStatusChangedCallbacks.Add(Id, CallbackObj);
	}

	return Id;
}

void UCoreConnect::EOSConnectRemoveNotifyLoginStatusChanged(UObject* WorldContextObject, FEOSNotificationId Id)
{
	LogVerbose("");

	if (GetConnectHandle(WorldContextObject))
	{
		EOS_Connect_RemoveNotifyLoginStatusChanged(GetConnectHandle(WorldContextObject), Id);
		GetConnect(WorldContextObject)->m_LoginStatusChangedCallbacks.Remove(Id);
	}
}

EOSResult UCoreConnect::EOSConnectCopyIdToken(UObject* WorldContextObject, FEOSConnectCopyIdTokenOptions Options, FEOSConnectIdToken& OutIdToken)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_UnexpectedError;

	if (GetConnectHandle(WorldContextObject))
	{
		EOS_Connect_CopyIdTokenOptions ConnectOptions = {};
		ConnectOptions.ApiVersion = EOS_CONNECT_COPYIDTOKEN_API_LATEST;
		ConnectOptions.LocalUserId = Options.LocalUserId;
		
		EOS_Connect_IdToken* IdToken = nullptr;
		
		Result = EOSHelpers::ToEOSCoreResult(EOS_Connect_CopyIdToken(GetConnectHandle(WorldContextObject), &ConnectOptions, &IdToken));

		if (Result == EOSResult::EOS_Success)
		{
			OutIdToken = *IdToken;
		}
	}

	return Result;
}

void UCoreConnect::EOSConnectVerifyIdToken(UObject* WorldContextObject, FEOSConnectVerifyIdTokenOptions Options, const FOnConnectVerifyIdTokenCallback& Callback)
{
	LogVerbose("");

	if (GetConnectHandle(WorldContextObject))
	{
		const FTCHARToUTF8 JsonWebToken(*Options.IdToken.JsonWebToken);
		
		EOS_Connect_VerifyIdTokenOptions ConnectOptions = { 0 };
		ConnectOptions.ApiVersion = EOS_CONNECT_VERIFYIDTOKEN_API_LATEST;

		EOS_Connect_IdToken IdToken;
		IdToken.ApiVersion = EOS_CONNECT_IDTOKEN_API_LATEST;
		IdToken.ProductUserId = Options.IdToken.ProductUserId;
		IdToken.JsonWebToken = JsonWebToken.Get();
		
		ConnectOptions.IdToken = &IdToken;

		FConnectAuthExpirationCallback* CallbackObj = new FConnectAuthExpirationCallback(Callback);

		EOS_Connect_VerifyIdToken(GetConnectHandle(WorldContextObject), &ConnectOptions, CallbackObj, ([](const EOS_Connect_VerifyIdTokenCallbackInfo* Data)
		{
			const FConnectAuthExpirationCallback* CallbackObj = static_cast<FConnectAuthExpirationCallback*>(Data->ClientData);
			CallbackObj->Callback.ExecuteIfBound(*Data);
		}));
		
		delete[] IdToken.JsonWebToken;
	}
}

void UCoreConnect::Internal_OnLoginCallback(const EOS_Connect_LoginCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FLoginCallback* CallbackObj = static_cast<FLoginCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreConnect::Internal_OnCreateUserCallback(const EOS_Connect_CreateUserCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FCreateUserCallback* CallbackObj = static_cast<FCreateUserCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreConnect::Internal_OnLinkAccountCallback(const EOS_Connect_LinkAccountCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FLinkAccountCallback* CallbackObj = static_cast<FLinkAccountCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreConnect::Internal_OnUnlinkAccountCallback(const EOS_Connect_UnlinkAccountCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FUnlinkAccountCallback* CallbackObj = static_cast<FUnlinkAccountCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreConnect::Internal_OnCreateDeviceIdCallback(const EOS_Connect_CreateDeviceIdCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FCreateDeviceIdCallback* CallbackObj = static_cast<FCreateDeviceIdCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreConnect::Internal_OnDeleteDeviceIdCallback(const EOS_Connect_DeleteDeviceIdCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FDeleteDeviceIdCallback* CallbackObj = static_cast<FDeleteDeviceIdCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreConnect::Internal_OnTransferDeviceIdAccount(const EOS_Connect_TransferDeviceIdAccountCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FTransferDeviceIdAccountCallback* CallbackObj = static_cast<FTransferDeviceIdAccountCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreConnect::Internal_OnQueryExternalAccountMappingsCallback(const EOS_Connect_QueryExternalAccountMappingsCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FQueryExternalAccountMappingsCallback* CallbackObj = static_cast<FQueryExternalAccountMappingsCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreConnect::Internal_OnQueryProductUserIdMappingsCallback(const EOS_Connect_QueryProductUserIdMappingsCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FQueryProductUserIdMappingsCallback* CallbackObj = static_cast<FQueryProductUserIdMappingsCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}