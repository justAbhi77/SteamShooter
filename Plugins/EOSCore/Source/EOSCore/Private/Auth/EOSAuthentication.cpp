/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "Auth/EOSAuthentication.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLogging.h"
#include "EOSCoreSharedTypes.h"

void UCoreAuthentication::Deinitialize()
{
	LogVerbose("");

	for (auto Element: m_AuthLoginStatusChangedCallbacks)
	{
		EOS_Auth_RemoveNotifyLoginStatusChanged(GetAuthHandle(this), Element.Key);
	}
}

UCoreAuthentication* UCoreAuthentication::GetAuthentication(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (GetAuthHandle(WorldContextObject))
		{
			if (UWorld* World = WorldContextObject->GetWorld())
			{
				if (UGameInstance* GameInstance = World->GetGameInstance())
				{
					return GameInstance->GetSubsystem<UCoreAuthentication>();
				}
			}	
		}
	}
	return nullptr;
}

void UCoreAuthentication::EOSAuthLogin(UObject* WorldContextObject, FEOSAuthLoginOptions Options, const FOnAuthLoginCallback& Callback)
{
	LogVerbose("");

	if (GetAuthHandle(WorldContextObject))
	{
		FAuthCredentials Credentials(Options.Credentials.ID, Options.Credentials.Token);

		Credentials.ApiVersion = Options.Credentials.ApiVersion;
		Credentials.Type = static_cast<EOS_ELoginCredentialType>(Options.Credentials.Type);
		Credentials.ExternalType = static_cast<EOS_EExternalCredentialType>(Options.Credentials.ExternalType);

		if (Options.Credentials.ID.IsEmpty() || Options.Credentials.Type == EEOSLoginCredentialType::EOS_LCT_PersistentAuth)
		{
			Credentials.Id = NULL;
		}

		if (Options.Credentials.Token.IsEmpty() || Options.Credentials.Type == EEOSLoginCredentialType::EOS_LCT_PersistentAuth)
		{
			Credentials.Token = NULL;
		}

		EOS_Auth_LoginOptions AuthOptions = {};
		AuthOptions.ApiVersion = Options.ApiVersion;
		AuthOptions.Credentials = &Credentials;
		if (Options.ScopeFlags == 0)
		{
			AuthOptions.ScopeFlags = EOS_EAuthScopeFlags::EOS_AS_NoFlags;
		}
		else
		{
			if ((Options.ScopeFlags & (1 << static_cast<int32>(EOSEAuthScopeFlags::EOS_AS_Country))) != 0)
			{
				AuthOptions.ScopeFlags |= EOS_EAuthScopeFlags::EOS_AS_Country;
			}

			if ((Options.ScopeFlags & (1 << static_cast<int32>(EOSEAuthScopeFlags::EOS_AS_FriendsList))) != 0)
			{
				AuthOptions.ScopeFlags |= EOS_EAuthScopeFlags::EOS_AS_FriendsList;
			}

			if ((Options.ScopeFlags & (1 << static_cast<int32>(EOSEAuthScopeFlags::EOS_AS_Email))) != 0)
			{
				AuthOptions.ScopeFlags |= EOS_EAuthScopeFlags::EOS_AS_Email;
			}

			if ((Options.ScopeFlags & (1 << static_cast<int32>(EOSEAuthScopeFlags::EOS_AS_Presence))) != 0)
			{
				AuthOptions.ScopeFlags |= EOS_EAuthScopeFlags::EOS_AS_Presence;
			}

			if ((Options.ScopeFlags & (1 << static_cast<int32>(EOSEAuthScopeFlags::EOS_AS_BasicProfile))) != 0)
			{
				AuthOptions.ScopeFlags |= EOS_EAuthScopeFlags::EOS_AS_BasicProfile;
			}

			if ((Options.ScopeFlags & (1 << static_cast<int32>(EOSEAuthScopeFlags::EOS_AS_FriendsManagement))) != 0)
			{
				AuthOptions.ScopeFlags |= EOS_EAuthScopeFlags::EOS_AS_FriendsManagement;
			}
		}

		FAuthLoginCallback* Data = new FAuthLoginCallback(Callback);
		EOS_Auth_Login(GetAuthHandle(WorldContextObject), &AuthOptions, Data, Internal_OnAuthLoginCallback);
	}
}

void UCoreAuthentication::EOSAuthLogout(UObject* WorldContextObject, FEOSAuthLogoutOptions Options, const FOnAuthLogoutCallback& Callback)
{
	LogVerbose("");

	if (GetAuthHandle(WorldContextObject))
	{
		EOS_Auth_LogoutOptions AuthOptions = {};
		AuthOptions.ApiVersion = Options.ApiVersion;
		AuthOptions.LocalUserId = Options.LocalUserId;

		FAuthLogoutCallback* CallbackObj = new FAuthLogoutCallback(Callback);
		EOS_Auth_Logout(GetAuthHandle(WorldContextObject), &AuthOptions, CallbackObj, Internal_OnAuthLogoutCallback);
	}
}

void UCoreAuthentication::EOSAuthLinkAccount(UObject* WorldContextObject, FEOSAuthLinkAccountOptions Options, const FOnAuthLinkAccountCallback& Callback)
{
	LogVerbose("");

	if (GetAuthHandle(WorldContextObject))
	{
		EOS_Auth_LinkAccountOptions AuthOptions = {};
		AuthOptions.ApiVersion = Options.ApiVersion;
		AuthOptions.LinkAccountFlags = static_cast<EOS_ELinkAccountFlags>(Options.LinkAccountFlags);
		AuthOptions.ContinuanceToken = Options.ContinuanceToken;

		if (Options.LinkAccountFlags == EEOSELinkAccountFlags::EOS_LA_NintendoNsaId)
		{
			AuthOptions.LocalUserId = Options.LocalUserId;
		}
		else
		{
			AuthOptions.LocalUserId = NULL;
		}
		

		FAuthLinkAccountCallback* CallbackObj = new FAuthLinkAccountCallback(Callback);
		EOS_Auth_LinkAccount(GetAuthHandle(WorldContextObject), &AuthOptions, CallbackObj, Internal_OnAuthOnLinkAccountCallback);
	}
}

void UCoreAuthentication::EOSAuthDeletePersistentAuth(UObject* WorldContextObject, const FEOSAuthDeletePersistentAuthOptions& Options, const FOnAuthDeletePersistentAuthCallback& Callback)
{
	LogVerbose("");

	if (GetAuthHandle(WorldContextObject))
	{
		const FAuthDeletePersistentAuthOptions AuthOptions(Options.RefreshToken);

		FAuthDeletePersistentAuthCallback* CallbackObj = new FAuthDeletePersistentAuthCallback(Callback);
		EOS_Auth_DeletePersistentAuth(GetAuthHandle(WorldContextObject), &AuthOptions, CallbackObj, Internal_OnAuthDeletePersistentAuthCallback);
	}
}

void UCoreAuthentication::EOSAuthVerifyUserAuth(UObject* WorldContextObject, const FEOSAuthVerifyUserAuthOptions& Options, const FOnAuthVerifyUserAuthCallback& Callback)
{
	LogVerbose("");

	if (GetAuthHandle(WorldContextObject))
	{
		FAuthToken AuthToken(Options.AuthToken.ClientId, Options.AuthToken.App, Options.AuthToken.ExpiresAt, Options.AuthToken.RefreshToken, Options.AuthToken.RefreshExpiresAt, Options.AuthToken.AccessToken);
		AuthToken.AccountId = Options.AuthToken.AccountId;
		AuthToken.ExpiresIn = FCString::Atod(*Options.AuthToken.ExpiresIn);
		AuthToken.AuthType = static_cast<EOS_EAuthTokenType>(Options.AuthToken.AuthType);
		AuthToken.RefreshExpiresIn = FCString::Atod(*Options.AuthToken.RefreshExpiresIn);

		EOS_Auth_VerifyUserAuthOptions AuthOptions = {};
		AuthOptions.ApiVersion = EOS_AUTH_VERIFYUSERAUTH_API_LATEST;
		AuthOptions.AuthToken = &AuthToken;

		FAuthVerifyUserAuthCallback* CallbackObj = new FAuthVerifyUserAuthCallback(Callback);
		EOS_Auth_VerifyUserAuth(GetAuthHandle(WorldContextObject), &AuthOptions, CallbackObj, Internal_OnAuthVerifyUserAuthCallback);
	}
}

int32 UCoreAuthentication::EOSAuthGetLoggedInAccountsCount(UObject* WorldContextObject)
{
	LogVerbose("");

	int32 Value = 0;

	if (GetAuthHandle(WorldContextObject))
	{
		Value = EOS_Auth_GetLoggedInAccountsCount(GetAuthHandle(WorldContextObject));
	}

	return Value;
}

FEOSEpicAccountId UCoreAuthentication::EOSAuthGetLoggedInAccountByIndex(UObject* WorldContextObject, int32 Index)
{
	LogVerbose("");

	FEOSEpicAccountId Id;

	if (GetAuthHandle(WorldContextObject))
	{
		Id = EOS_Auth_GetLoggedInAccountByIndex(GetAuthHandle(WorldContextObject), Index);
	}

	return Id;
}

EOSELoginStatus UCoreAuthentication::EOSAuthGetLoginStatus(UObject* WorldContextObject, FEOSEpicAccountId LocalUserId)
{
	LogVerbose("");

	EOSELoginStatus Status = EOSELoginStatus::EOS_LS_NotLoggedIn;

	if (GetAuthHandle(WorldContextObject))
	{
		Status = static_cast<EOSELoginStatus>(EOS_Auth_GetLoginStatus(GetAuthHandle(WorldContextObject), LocalUserId));
	}

	return Status;
}

EOSResult UCoreAuthentication::EOSAuthCopyUserAuthToken(UObject* WorldContextObject, FEOSAuthCopyUserAuthTokenOptions Options, FEOSEpicAccountId LocalUserId, FEOSAuthToken& OutUserAuthToken)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_TimedOut;

	if (GetAuthHandle(WorldContextObject))
	{
		EOS_Auth_CopyUserAuthTokenOptions AuthOptions = {};
		AuthOptions.ApiVersion = Options.ApiVersion;
		EOS_Auth_Token* AuthToken = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Auth_CopyUserAuthToken(GetAuthHandle(WorldContextObject), &AuthOptions, LocalUserId, &AuthToken));

		if (Result == EOSResult::EOS_Success)
		{
			OutUserAuthToken = *AuthToken;
		}
	}

	return Result;
}

EOSResult UCoreAuthentication::EOSAuthCopyIdToken(UObject* WorldContextObject, FEOSAuthCopyIdTokenOptions Options, FEOSAuthIdToken& OutIdToken)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_UnexpectedError;

	if (GetAuthHandle(WorldContextObject))
	{
		EOS_Auth_CopyIdTokenOptions AuthOptions = {};
		AuthOptions.ApiVersion = EOS_AUTH_COPYIDTOKEN_API_LATEST;
		AuthOptions.AccountId = Options.AccountId;
		EOS_Auth_IdToken* IdToken = nullptr;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Auth_CopyIdToken(GetAuthHandle(WorldContextObject), &AuthOptions, &IdToken));

		if (Result == EOSResult::EOS_Success)
		{
			OutIdToken = *IdToken;
		}
	}

	return Result;
}

void UCoreAuthentication::EOSAuthQueryIdToken(UObject* WorldContextObject, FEOSAuthQueryIdTokenOptions Options, const FOnAuthQueryIdTokenCallback& Callback)
{
	LogVerbose("");

	if (GetAuthHandle(WorldContextObject))
	{
		EOS_Auth_QueryIdTokenOptions AuthOptions = { 0 };
		AuthOptions.ApiVersion = EOS_AUTH_QUERYIDTOKEN_API_LATEST;
		AuthOptions.LocalUserId = Options.LocalUserId;
		AuthOptions.TargetAccountId = Options.TargetAccountId;

		FAuthQueryIdToken* CallbackObj = new FAuthQueryIdToken(Callback);
		EOS_Auth_QueryIdToken(GetAuthHandle(WorldContextObject), &AuthOptions, CallbackObj, Internal_OnAuthQueryIdTokenCallback);
	}
}

void UCoreAuthentication::EOSAuthVerifyIdToken(UObject* WorldContextObject, FEOSAuthVerifyIdTokenOptions Options, const FOnAuthVerifyIdTokenCallback& Callback)
{
	LogVerbose("");

	if (GetAuthHandle(WorldContextObject))
	{
		const FTCHARToUTF8 JsonWebToken(*Options.IdToken.JsonWebToken);
		
		EOS_Auth_VerifyIdTokenOptions AuthOptions = { 0 };
		AuthOptions.ApiVersion = EOS_AUTH_VERIFYIDTOKEN_API_LATEST;

		EOS_Auth_IdToken Token;
		Token.ApiVersion = EOS_AUTH_IDTOKEN_API_LATEST;
		Token.AccountId = Options.IdToken.AccountId;
		Token.JsonWebToken = JsonWebToken.Get();
		AuthOptions.IdToken = &Token;

		FAuthVerifyIdToken* CallbackObj = new FAuthVerifyIdToken(Callback);
		EOS_Auth_VerifyIdToken(GetAuthHandle(WorldContextObject), &AuthOptions, CallbackObj, Internal_OnAuthVerifyIdTokenCallback);

		delete[] Token.JsonWebToken;
	}
}

EOSResult UCoreAuthentication::EOSAuthGetSelectedAccountId(UObject* WorldContextObject, FEOSEpicAccountId LocalUserId, FEOSEpicAccountId& OutSelectedAccountId)
{
	LogVerbose("");
	
	EOSResult Result = EOSResult::EOS_UnexpectedError;

	if (GetAuthHandle(WorldContextObject))
	{
		EOS_EpicAccountId AccountId;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Auth_GetSelectedAccountId(GetAuthHandle(WorldContextObject), LocalUserId, &AccountId));

		if (Result == EOSResult::EOS_Success)
		{
			OutSelectedAccountId = AccountId;
		}
	}
	
	return Result;
}

int32 UCoreAuthentication::EOSAuthGetMergedAccountsCount(UObject* WorldContextObject, FEOSEpicAccountId LocalUserId)
{
	LogVerbose("");
	
	int32 Result = 0;

	if (GetAuthHandle(WorldContextObject))
	{
		Result = EOS_Auth_GetMergedAccountsCount(GetAuthHandle(WorldContextObject), LocalUserId);
	}

	return Result;
}

FEOSEpicAccountId UCoreAuthentication::EOSAuthGetMergedAccountByIndex(UObject* WorldContextObject, FEOSEpicAccountId LocalUserId, int32 Index)
{
	LogVerbose("");

	FEOSEpicAccountId Result;
	
	if (GetAuthHandle(WorldContextObject))
	{
		Result = EOS_Auth_GetMergedAccountByIndex(GetAuthHandle(WorldContextObject), LocalUserId, Index);
	}

	return Result;
}

FEOSNotificationId UCoreAuthentication::EOSAuthAddNotifyLoginStatusChanged(UObject* WorldContextObject, const FOnAuthLoginStatusChangedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetAuthHandle(WorldContextObject))
	{
		EOS_Auth_AddNotifyLoginStatusChangedOptions AuthOptions = {EOS_AUTH_ADDNOTIFYLOGINSTATUSCHANGED_API_LATEST};

		FAuthLoginStatusChangedCallback* CallbackObj = new FAuthLoginStatusChangedCallback(Callback);

		Id = EOS_Auth_AddNotifyLoginStatusChanged(GetAuthHandle(WorldContextObject), &AuthOptions, CallbackObj, ([](const EOS_Auth_LoginStatusChangedCallbackInfo* data)
		{
			const FAuthLoginStatusChangedCallback* CallbackObj = static_cast<FAuthLoginStatusChangedCallback*>(data->ClientData);
			CallbackObj->m_Callback.ExecuteIfBound(*data);
		}));

		GetAuthentication(WorldContextObject)->m_AuthLoginStatusChangedCallbacks.Add(Id, CallbackObj);
	}

	return Id;
}

void UCoreAuthentication::EOSAuthRemoveNotifyLoginStatusChanged(UObject* WorldContextObject, FEOSNotificationId ID)
{
	LogVerbose("");

	if (GetAuthHandle(WorldContextObject))
	{
		EOS_Auth_RemoveNotifyLoginStatusChanged(GetAuthHandle(WorldContextObject), ID);
		GetAuthentication(WorldContextObject)->m_AuthLoginStatusChangedCallbacks.Remove(ID);
	}
}

void UCoreAuthentication::Internal_OnAuthLoginCallback(const EOS_Auth_LoginCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FAuthLoginCallback* CallbackObj = static_cast<FAuthLoginCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(Data);
		delete CallbackObj;
	}
}

void UCoreAuthentication::Internal_OnAuthLogoutCallback(const EOS_Auth_LogoutCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FAuthLogoutCallback* CallbackObj = static_cast<FAuthLogoutCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreAuthentication::Internal_OnAuthOnLinkAccountCallback(const EOS_Auth_LinkAccountCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FAuthLinkAccountCallback* CallbackObj = static_cast<FAuthLinkAccountCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreAuthentication::Internal_OnAuthDeletePersistentAuthCallback(const EOS_Auth_DeletePersistentAuthCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FAuthDeletePersistentAuthCallback* CallbackObj = static_cast<FAuthDeletePersistentAuthCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreAuthentication::Internal_OnAuthVerifyUserAuthCallback(const EOS_Auth_VerifyUserAuthCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FAuthVerifyUserAuthCallback* CallbackObj = static_cast<FAuthVerifyUserAuthCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreAuthentication::Internal_OnAuthQueryIdTokenCallback(const EOS_Auth_QueryIdTokenCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FAuthQueryIdToken* CallbackObj = static_cast<FAuthQueryIdToken*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreAuthentication::Internal_OnAuthVerifyIdTokenCallback(const EOS_Auth_VerifyIdTokenCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FAuthVerifyIdToken* CallbackObj = static_cast<FAuthVerifyIdToken*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}
