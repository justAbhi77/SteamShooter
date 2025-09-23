/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "OnlineIdentityEOSCore.h"

#include "EOSCoreSettings.h"
#include "OnlineSubsystemEOSCore.h"
#include "OnlineSubsystemModuleEOSCore.h"
#include "OnlineSubsystemEOSCorePrivatePCH.h"
#include "EOSCoreSharedTypes.h"

#ifndef STEAMCORE_SUBSYSTEM
#define STEAMCORE_SUBSYSTEM FName(TEXT("STEAMCORE"))
#endif

typedef TEOSCallback<EOS_Connect_OnLoginCallback, EOS_Connect_LoginCallbackInfo> FConnectLoginCallback;

inline EOS_EExternalCredentialType ToEOS_EExternalCredentialType(FName OSSName, const FOnlineAccountCredentials& AccountCredentials)
{
#if PLATFORM_DESKTOP
	if (OSSName == STEAM_SUBSYSTEM || OSSName == STEAMCORE_SUBSYSTEM)
	{
		return EOS_EExternalCredentialType::EOS_ECT_STEAM_APP_TICKET;
	}
#endif
	if (OSSName == PS4_SUBSYSTEM)
	{
		return EOS_EExternalCredentialType::EOS_ECT_PSN_ID_TOKEN;
	}
		else if (OSSName == SWITCH_SUBSYSTEM)
		{
			if (AccountCredentials.Type == TEXT("NintendoAccount"))
			{
				return EOS_EExternalCredentialType::EOS_ECT_NINTENDO_ID_TOKEN;
			}
			else
			{
				return EOS_EExternalCredentialType::EOS_ECT_NINTENDO_NSA_ID_TOKEN;
			}
		}
		else if (OSSName == APPLE_SUBSYSTEM)
		{
			return EOS_EExternalCredentialType::EOS_ECT_APPLE_ID_TOKEN;
		}
	// Unknown means OpenID
	return EOS_EExternalCredentialType::EOS_ECT_OPENID_ACCESS_TOKEN;
}

FOnlineIdentityEOSCore::FOnlineIdentityEOSCore(FOnlineSubsystemEOSCore* InSubsystem)
	: m_Subsystem(InSubsystem)
	, m_DefaultLocalUser(-1)
	, bInitialized(false)
	, LoginNotificationId(0)
	, ConnectLoginNotificationId(0)
	, AuthLoginNotificationCallback(nullptr)
	, ConnectLoginNotificationCallback(nullptr)
{
}

FOnlineIdentityEOSCore::~FOnlineIdentityEOSCore()
{
	m_LastUsedCredentialsType.Empty();
	m_LastUsedUserLoginInfoDisplayName.Empty();
}

void FOnlineIdentityEOSCore::ConnectLoginStatusChanged(const EOS_Connect_LoginStatusChangedCallbackInfo* data)
{
	LogVerbose("");

	if (data->CurrentStatus == EOS_ELoginStatus::EOS_LS_NotLoggedIn)
	{
		if (ProductUserIdToUserNumMap.Contains(data->LocalUserId))
		{
			const int32 LocalUserNum = ProductUserIdToUserNumMap[data->LocalUserId];
			if (UserNumToNetIdMap.Find(LocalUserNum))
			{
				const FUniqueNetIdEOSCorePtr UserNetId = UserNumToNetIdMap[LocalUserNum];
				TriggerOnLoginStatusChangedDelegates(LocalUserNum, ELoginStatus::LoggedIn, ELoginStatus::NotLoggedIn, *UserNetId);
			}
			else
			{
				const FUniqueNetIdEOSCorePtr NetId = GetLocalUniqueNetIdEOS(LocalUserNum);
				TriggerOnLoginStatusChangedDelegates(LocalUserNum, ELoginStatus::LoggedIn, ELoginStatus::NotLoggedIn, *NetId);
			}

			RemoveLocalUser(LocalUserNum);

			if (LocalUserNumToConnectLoginNotifcationMap.Contains(LocalUserNum))
			{
				const FNotificationIdCallbackPair* NotificationPair = LocalUserNumToConnectLoginNotifcationMap[LocalUserNum];
				LocalUserNumToConnectLoginNotifcationMap.Remove(LocalUserNum);

				EOS_Connect_RemoveNotifyAuthExpiration(m_Subsystem->m_ConnectHandle, NotificationPair->NotificationId);

				delete NotificationPair;
			}
		}
	}
	else
	{
		FullLoginCallback(0);
	}
}

void FOnlineIdentityEOSCore::LoginStatusChanged(const EOS_Auth_LoginStatusChangedCallbackInfo* Data)
{
	LogVerbose("");

	if (Data->CurrentStatus == EOS_ELoginStatus::EOS_LS_NotLoggedIn)
	{
		if (AccountIdToUserNumMap.Contains(Data->LocalUserId))
		{
			const int32 LocalUserNum = AccountIdToUserNumMap[Data->LocalUserId];

			if (UserNumToNetIdMap.Find(LocalUserNum))
			{
				const FUniqueNetIdEOSCorePtr UserNetId = UserNumToNetIdMap[LocalUserNum];
				TriggerOnLoginStatusChangedDelegates(LocalUserNum, ELoginStatus::LoggedIn, ELoginStatus::NotLoggedIn, *UserNetId);
			}
			else
			{
				const FUniqueNetIdEOSCorePtr NetId = GetLocalUniqueNetIdEOS(LocalUserNum);
				TriggerOnLoginStatusChangedDelegates(LocalUserNum, ELoginStatus::LoggedIn, ELoginStatus::NotLoggedIn, *NetId);
			}
			
			RemoveLocalUser(LocalUserNum);

			if (UserNumToNetIdMap.Num() == 0)
			{
				if (LocalUserNumToConnectLoginNotifcationMap.Contains(LocalUserNum))
				{
					FNotificationIdCallbackPair* NotificationPair = LocalUserNumToConnectLoginNotifcationMap[LocalUserNum];
					LocalUserNumToConnectLoginNotifcationMap.Remove(LocalUserNum);

					EOS_Connect_RemoveNotifyAuthExpiration(m_Subsystem->m_ConnectHandle, NotificationPair->NotificationId);

					delete NotificationPair;
				}
			}
		}
	}
	else
	{
		if (!AccountIdToUserNumMap.Contains(Data->LocalUserId))
		{
			ConnectLogin(AccountIdToUserNumMap.Num(), false);
		}
	}
}

typedef TEOSCallback<EOS_Auth_OnLoginCallback, EOS_Auth_LoginCallbackInfo> FLoginCallback;
typedef TEOSCallback<EOS_Connect_OnCreateUserCallback, EOS_Connect_CreateUserCallbackInfo> FCreateUserCallback;
typedef TEOSGlobalCallback<EOS_Connect_OnAuthExpirationCallback, EOS_Connect_AuthExpirationCallbackInfo> FRefreshAuthCallback;
typedef TEOSGlobalCallback<EOS_Auth_OnLoginStatusChangedCallback, EOS_Auth_LoginStatusChangedCallbackInfo> FLoginStatusChangedCallback;
typedef TEOSGlobalCallback<EOS_Connect_OnLoginStatusChangedCallback, EOS_Connect_LoginStatusChangedCallbackInfo> FConnectLoginStatusChangedCallback;

bool FOnlineIdentityEOSCore::Login(int32 LocalUserNum, const FOnlineAccountCredentials& AccountCredentials)
{
	LogVerbose("Login: %s", *AccountCredentials.ToDebugString());

	LocalUserNumToLastLoginCredentials.Emplace(LocalUserNum, MakeShared<FOnlineAccountCredentials>(AccountCredentials));

	if (GetLoginStatus(LocalUserNum) == ELoginStatus::LoggedIn)
	{
		UE_LOG_ONLINE(Warning, TEXT("User (%d) already logged in."), LocalUserNum);
		TriggerOnLoginCompleteDelegates(LocalUserNum, false, *GetLocalUniqueNetIdEOS(LocalUserNum), FString(TEXT("Already logged in")));
		return true;
	}

	EOS_Auth_LoginOptions LoginOptions = {};
	LoginOptions.ApiVersion = EOS_AUTH_LOGIN_API_LATEST;

	int32 AuthScopeFlags = UEOSCoreSettings::GetAuthScopeFlags();

	if (AuthScopeFlags == 0)
	{
		LoginOptions.ScopeFlags = EOS_EAuthScopeFlags::EOS_AS_NoFlags;
	}
	
	if ((AuthScopeFlags & (1 << static_cast<int32>(EOSEAuthScopeFlags::EOS_AS_Country))) != 0)
	{
		LoginOptions.ScopeFlags |= EOS_EAuthScopeFlags::EOS_AS_Country;
	}

	if ((AuthScopeFlags & (1 << static_cast<int32>(EOSEAuthScopeFlags::EOS_AS_FriendsList))) != 0)
	{
		LoginOptions.ScopeFlags |= EOS_EAuthScopeFlags::EOS_AS_FriendsList;
	}

	if ((AuthScopeFlags & (1 << static_cast<int32>(EOSEAuthScopeFlags::EOS_AS_Email))) != 0)
	{
		LoginOptions.ScopeFlags |= EOS_EAuthScopeFlags::EOS_AS_Email;
	}

	if ((AuthScopeFlags & (1 << static_cast<int32>(EOSEAuthScopeFlags::EOS_AS_Presence))) != 0)
	{
		LoginOptions.ScopeFlags |= EOS_EAuthScopeFlags::EOS_AS_Presence;
	}

	if ((AuthScopeFlags & (1 << static_cast<int32>(EOSEAuthScopeFlags::EOS_AS_BasicProfile))) != 0)
	{
		LoginOptions.ScopeFlags |= EOS_EAuthScopeFlags::EOS_AS_BasicProfile;
	}

	if ((AuthScopeFlags & (1 << static_cast<int32>(EOSEAuthScopeFlags::EOS_AS_FriendsManagement))) != 0)
	{
		LoginOptions.ScopeFlags |= EOS_EAuthScopeFlags::EOS_AS_FriendsManagement;
	}

	LogVerbose("AuthScopeFlags: %d", AuthScopeFlags);

	FAuthCredentials Credentials(AccountCredentials.Id, AccountCredentials.Token);
	LoginOptions.Credentials = &Credentials;

	const TCHAR* Delims[1] = {TEXT("|")};
	TArray<FString> Values;
	AccountCredentials.Type.ParseIntoArray(Values, Delims, 1, false);

	FString LoginType = TEXT("0");
	FString LoginCredentialsType = TEXT("0");
	FString LoginData;

	if (Values.IsValidIndex(0))
	{
		LoginType = Values[0];
	}

	if (Values.IsValidIndex(1))
	{
		LoginCredentialsType = Values[1];
	}

	if (Values.IsValidIndex(2))
	{
		LoginData = Values[2];
	}

	// LoginTypes
	// 0 = Password
	// 1 = ExchangeCode
	// 2 = PersistentAuth
	// 3 = DeviceCode
	// 4 = Developer
	// 5 = RefreshToken
	// 6 = AccountPortal
	// 7 = ExternalAuth

	// CredentialTypes
	// 0 = Epic
	// 1 = STEAM_APP_TICKET
	// 2 = PSN_ID_TOKEN
	// 3 = XBL_XSTS_TOKEN
	// 4 = DISCORD_ACCESS_TOKEN
	// 5 = GOG_SESSION_TICKET
	// 6 = NINTENDO_ID_TOKEN
	// 7 = NINTENDO_NSA_ID_TOKEN
	// 8 = UPLAY_ACCESS_TOKEN
	// 9 = OPENID_ACCESS_TOKEN
	// 10 = DEVICEID_ACCESS_TOKEN
	// 11 = APPLE_ID_TOKEN
	// 12 = EOS_ECT_GOOGLE_ID_TOKEN
	// 13 = EOS_ECT_OCULUS_USERID_NONCE
	// 14 = EOS_ECT_ITCHIO_JWT
	// 15 = EOS_ECT_ITCHIO_KEY
	// 16 = EOS_ECT_EPIC_ID_TOKEN
	// 17 = EOS_ECT_AMAZON_ACCESS_TOKEN
	// 18 = EOS_ECT_STEAM_SESSION_TICKET	
	
	if (LoginType == TEXT("0")) // password
	{
		LogVerbose("Logging in using Password Authentication");

		Credentials.Type = EOS_ELoginCredentialType::EOS_LCT_Password;
	}
	else if (LoginType == TEXT("1")) // exchange code from epic launcher
	{
		LogVerbose("Logging in using ExhcangeCode Authentication");

		Credentials.Type = EOS_ELoginCredentialType::EOS_LCT_ExchangeCode;
	}
	else if (LoginType == TEXT("2")) // persistent auth
	{
		LogVerbose("Logging in using Persistent Authentication");

		Credentials.Type = EOS_ELoginCredentialType::EOS_LCT_PersistentAuth;
	}
	else if (LoginType == TEXT("4")) // dev tool
	{
		LogVerbose("Logging in using DevTool Authentication");

		Credentials.Type = EOS_ELoginCredentialType::EOS_LCT_Developer;
	}
	else if (LoginType == TEXT("6")) // account portal
	{
		LogVerbose("Logging in using Account Portal Authentication");

		Credentials.Type = EOS_ELoginCredentialType::EOS_LCT_AccountPortal;
	}
	else if (LoginType == TEXT("7"))
	{
		LogVerbose("Logging in using External Authentication");

		Credentials.Type = EOS_ELoginCredentialType::EOS_LCT_ExternalAuth;
	}
	else
	{
		UE_LOG_ONLINE(Warning, TEXT("Unable to Login() user (%d) due to missing auth parameters"), LocalUserNum);
		TriggerOnLoginCompleteDelegates(LocalUserNum, false, FUniqueNetIdEOSCore(), FString(TEXT("Missing auth parameters")));
		return false;
	}

	if (LoginCredentialsType == TEXT("0"))
	{
		LogVerbose("Logging in using Epic Credentials Type");

		FLoginCallback* CallbackObj = new FLoginCallback();
		CallbackObj->m_CallbackLambda = [this, LocalUserNum](const EOS_Auth_LoginCallbackInfo* Data)
		{
			if (Data->ResultCode == EOS_EResult::EOS_Success)
			{
				ConnectLogin(LocalUserNum, true);
			}
			else
			{
				FString ErrorString = FString::Printf(TEXT("Login(%d) failed with EOS result code (%s)"), LocalUserNum, ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
				UE_LOG_ONLINE(Warning, TEXT("%s"), *ErrorString);
				TriggerOnLoginCompleteDelegates(LocalUserNum, false, FUniqueNetIdEOSCore(), ErrorString);
			}
		};

		EOS_Auth_Login(m_Subsystem->m_AuthHandle, &LoginOptions, (void*)CallbackObj, CallbackObj->GetCallbackPtr());
	}
	else if (LoginCredentialsType == TEXT("1"))
	{
		LogVerbose("Logging in using SteamAppTicket");

		if (LoginData.IsEmpty())
		{
			LogError("No SteamAppTicket was provided");
			return false;
		}

		LogVerbose("Using SteamAppTicket: %s", *LoginData);

		Credentials.Type = EOS_ELoginCredentialType::EOS_LCT_ExternalAuth;
		Credentials.ExternalType = EOS_EExternalCredentialType::EOS_ECT_STEAM_APP_TICKET;

		FConnectLoginCallback* SteamCallbackObj = new FConnectLoginCallback();
		SteamCallbackObj->m_CallbackLambda = [LocalUserNum, this](const EOS_Connect_LoginCallbackInfo* Data)
		{
			if (Data->ResultCode == EOS_EResult::EOS_Success)
			{
				FullLoginCallback(LocalUserNum);
			}
			else if (Data->ResultCode == EOS_EResult::EOS_InvalidUser)
			{
				CreateConnectedLogin(LocalUserNum, Data->ContinuanceToken);
			}
		};

		EOS_Connect_LoginOptions SteamLoginOptions;
		SteamLoginOptions.ApiVersion = EOS_CONNECT_LOGIN_API_LATEST;

		FConnectCredentials SteamCredentials(LoginData);
		SteamCredentials.Type = EOS_EExternalCredentialType::EOS_ECT_STEAM_APP_TICKET;

		SteamLoginOptions.Credentials = &SteamCredentials;
		SteamLoginOptions.UserLoginInfo = NULL;

		m_LastUsedCredentialsType.Emplace(LocalUserNum, EOS_EExternalCredentialType::EOS_ECT_STEAM_APP_TICKET);
		EOS_Connect_Login(m_Subsystem->m_ConnectHandle, &SteamLoginOptions, SteamCallbackObj, SteamCallbackObj->GetCallbackPtr());

		return false;
	}
	else if (LoginCredentialsType == TEXT("2"))
	{
		LogError("Logging in using PSN ID Token - Not yet supported");
		return false;
	}
	else if (LoginCredentialsType == TEXT("3"))
	{
		LogError("Logging in using Xbox XSTS Token - Not yet supported");
		return false;
	}
	else if (LoginCredentialsType == TEXT("4"))
	{
		LogError("Logging in using Discord Token - Not yet supported");
		return false;
	}
	else if (LoginCredentialsType == TEXT("5"))
	{
		LogError("Logging in using GOG Session Ticket - Not yet supported");
		return false;
	}
	else if (LoginCredentialsType == TEXT("6"))
	{
		LogError("Logging in using Nintento ID Token - Not yet supported");
		return false;
	}
	else if (LoginCredentialsType == TEXT("7"))
	{
		LogError("Logging in using Nintendo NSA Id Token - Not yet supported");
		return false;
	}
	else if (LoginCredentialsType == TEXT("8"))
	{
		LogError("Logging in using Uplay Access Token - Not yet supported");
		return false;
	}
	else if (LoginCredentialsType == TEXT("9"))
	{
		LogVerbose("Logging in using Open Id");

		if (LoginData.IsEmpty())
		{
			LogError("No Open ID Token was provided");
			return false;
		}

		LogVerbose("Using Open ID Token: %s", *LoginData);

		Credentials.Type = EOS_ELoginCredentialType::EOS_LCT_ExternalAuth;
		Credentials.ExternalType = EOS_EExternalCredentialType::EOS_ECT_OPENID_ACCESS_TOKEN;

		FConnectLoginCallback* OpenIDCallbackObj = new FConnectLoginCallback();
		OpenIDCallbackObj->m_CallbackLambda = [LocalUserNum, this](const EOS_Connect_LoginCallbackInfo* Data)
		{
			if (Data->ResultCode == EOS_EResult::EOS_Success)
			{
				FullLoginCallback(LocalUserNum);
			}
			else if (Data->ResultCode == EOS_EResult::EOS_InvalidUser)
			{
				CreateConnectedLogin(LocalUserNum, Data->ContinuanceToken);
			}
		};

		EOS_Connect_LoginOptions OpenIDLoginOptions;
		OpenIDLoginOptions.ApiVersion = EOS_CONNECT_LOGIN_API_LATEST;

		FConnectCredentials OpenIDCredentials = FConnectCredentials(LoginData);

		OpenIDCredentials.Type = EOS_EExternalCredentialType::EOS_ECT_OPENID_ACCESS_TOKEN;

		OpenIDLoginOptions.Credentials = &OpenIDCredentials;
		OpenIDLoginOptions.UserLoginInfo = nullptr;

		m_LastUsedCredentialsType.Emplace(LocalUserNum, EOS_EExternalCredentialType::EOS_ECT_OPENID_ACCESS_TOKEN);
		EOS_Connect_Login(m_Subsystem->m_ConnectHandle, &OpenIDLoginOptions, OpenIDCallbackObj, OpenIDCallbackObj->GetCallbackPtr());

		return true;
	}
	else if (LoginCredentialsType == TEXT("10"))
	{
		LogVerbose("Logging in using Local Device Id");

		FConnectLoginCallback* CallbackObj = new FConnectLoginCallback();

		EOS_Connect_Credentials DeviceIdCredentials = {};
		DeviceIdCredentials.ApiVersion = EOS_CONNECT_CREDENTIALS_API_LATEST;
		DeviceIdCredentials.Token = nullptr;
		DeviceIdCredentials.Type = EOS_EExternalCredentialType::EOS_ECT_DEVICEID_ACCESS_TOKEN;

		FConnectUserLoginInfo DeviceIdUserLoginInfo(AccountCredentials.Id);
		m_LastUsedUserLoginInfoDisplayName.Emplace(LocalUserNum, AccountCredentials.Id);

		EOS_Connect_LoginOptions DeviceIdOptions = {};
		DeviceIdOptions.ApiVersion = EOS_CONNECT_LOGIN_API_LATEST;
		DeviceIdOptions.Credentials = &DeviceIdCredentials;
		DeviceIdOptions.UserLoginInfo = &DeviceIdUserLoginInfo;

		CallbackObj->m_CallbackLambda = [LocalUserNum, AccountCredentials, this](const EOS_Connect_LoginCallbackInfo* Data)
		{
			if (Data->ResultCode == EOS_EResult::EOS_Success)
			{
				UE_LOG_ONLINE(Warning, TEXT("Successfully logged in local device"));
				AddLocalUser(LocalUserNum, nullptr, Data->LocalUserId);

				FUniqueNetIdEOSCorePtr UserNetId = GetLocalUniqueNetIdEOS(LocalUserNum);
				check(UserNetId.IsValid());
			}
			else if (Data->ResultCode == EOS_EResult::EOS_NotFound)
			{
				CreateDeviceId(LocalUserNum, AccountCredentials, true);
			}
			else
			{
				FString ErrorString = FString::Printf(TEXT("Login(%d) failed with EOS result code (%s)"), LocalUserNum, ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));

				UE_LOG_ONLINE(Warning, TEXT("%s"), *ErrorString);

				TriggerOnLoginCompleteDelegates(LocalUserNum, false, FUniqueNetIdEOSCore(), ErrorString);
			}
		};

		m_LastUsedCredentialsType.Emplace(LocalUserNum, EOS_EExternalCredentialType::EOS_ECT_DEVICEID_ACCESS_TOKEN);
		
		EOS_Connect_Login(m_Subsystem->m_ConnectHandle, &DeviceIdOptions, CallbackObj, CallbackObj->GetCallbackPtr());
	}
	else if (LoginCredentialsType == TEXT("11"))
	{
		LogError("Logging in using Apple ID Token - Not yet supported");
		return false;
	}
	else if (LoginCredentialsType == TEXT("12"))
	{
		LogError("Logging in using Google Id Token - Not yet supported");
		return false;
	}
	else if (LoginCredentialsType == TEXT("13"))
	{
		LogError("Logging in using EOS_ECT_OCULUS_USERID_NONCE - Not yet supported");
		return false;
	}
	else if (LoginCredentialsType == TEXT("14"))
	{
		LogError("Logging in using EOS_ECT_ITCHIO_JWT - Not yet supported");
		return false;
	}
	else if (LoginCredentialsType == TEXT("15"))
	{
		LogError("Logging in using EOS_ECT_ITCHIO_KEY - Not yet supported");
		return false;
	}
	else if (LoginCredentialsType == TEXT("16"))
	{
		LogError("Logging in using EOS_ECT_EPIC_ID_TOKEN - Not yet supported");
		return false;
	}
	else if (LoginCredentialsType == TEXT("17"))
	{
		LogError("Logging in using EOS_ECT_AMAZON_ACCESS_TOKEN - Not yet supported");
		return false;
	}
	else if (LoginCredentialsType == TEXT("18"))
	{
		LogVerbose("Logging in using SteamSessionTicket");

		if (LoginData.IsEmpty())
		{
			LogError("No SteamSessionTicket was provided");
			return false;
		}

		LogVerbose("Using SteamSessionTicket: %s", *LoginData);

		Credentials.Type = EOS_ELoginCredentialType::EOS_LCT_ExternalAuth;
		Credentials.ExternalType = EOS_EExternalCredentialType::EOS_ECT_STEAM_SESSION_TICKET;

		FConnectLoginCallback* SteamCallbackObj = new FConnectLoginCallback();
		SteamCallbackObj->m_CallbackLambda = [LocalUserNum, this](const EOS_Connect_LoginCallbackInfo* Data)
		{
			if (Data->ResultCode == EOS_EResult::EOS_Success)
			{
				FullLoginCallback(LocalUserNum);
			}
			else if (Data->ResultCode == EOS_EResult::EOS_InvalidUser)
			{
				CreateConnectedLogin(LocalUserNum, Data->ContinuanceToken);
			}
		};

		EOS_Connect_LoginOptions SteamLoginOptions;
		SteamLoginOptions.ApiVersion = EOS_CONNECT_LOGIN_API_LATEST;

		FConnectCredentials SteamCredentials(LoginData);
		SteamCredentials.Type = EOS_EExternalCredentialType::EOS_ECT_STEAM_SESSION_TICKET;

		SteamLoginOptions.Credentials = &SteamCredentials;
		SteamLoginOptions.UserLoginInfo = NULL;

		m_LastUsedCredentialsType.Emplace(LocalUserNum, EOS_EExternalCredentialType::EOS_ECT_STEAM_SESSION_TICKET);
		EOS_Connect_Login(m_Subsystem->m_ConnectHandle, &SteamLoginOptions, SteamCallbackObj, SteamCallbackObj->GetCallbackPtr());

		return false;
	}

	return true;
}


void FOnlineIdentityEOSCore::Tick(float deltaTime)
{
	if (!bInitialized)
	{
		if (m_Subsystem)
		{
			if (m_Subsystem->IsEnabled()) 
			{
				if (LoginNotificationId == 0)
				{
					FLoginStatusChangedCallback* CallbackObj = new FLoginStatusChangedCallback();
					AuthLoginNotificationCallback = CallbackObj;
					CallbackObj->m_CallbackLambda = [this](const EOS_Auth_LoginStatusChangedCallbackInfo* Data)
					{
						LoginStatusChanged(Data);
					};

					const EOS_Auth_AddNotifyLoginStatusChangedOptions Options = { EOS_AUTH_ADDNOTIFYLOGINSTATUSCHANGED_API_LATEST };
					LoginNotificationId = EOS_Auth_AddNotifyLoginStatusChanged(m_Subsystem->m_AuthHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());
				}

				if (ConnectLoginNotificationId == 0)
				{
					const EOS_Connect_AddNotifyLoginStatusChangedOptions ConnectLoginStatusChangedOptions = { EOS_CONNECT_ADDNOTIFYLOGINSTATUSCHANGED_API_LATEST };

					FConnectLoginStatusChangedCallback* CallbackObj = new FConnectLoginStatusChangedCallback();
					ConnectLoginNotificationCallback = CallbackObj;
					CallbackObj->m_CallbackLambda = [this](const EOS_Connect_LoginStatusChangedCallbackInfo* Data)
					{
						ConnectLoginStatusChanged(Data);
					};

					ConnectLoginNotificationId = EOS_Connect_AddNotifyLoginStatusChanged(m_Subsystem->m_ConnectHandle, &ConnectLoginStatusChangedOptions, CallbackObj, CallbackObj->GetCallbackPtr());
				}
			}
			
			bInitialized = true;
		}
	}
}

void FOnlineIdentityEOSCore::ConnectLogin(int32 LocalUserNum, bool bCreateUserAccountIfInvalidUser)
{
	LogVerbose("");

	EOS_Auth_Token* AuthToken = nullptr;
	EOS_Auth_CopyUserAuthTokenOptions CopyOptions = {};
	CopyOptions.ApiVersion = EOS_AUTH_COPYUSERAUTHTOKEN_API_LATEST;

	const EOS_EpicAccountId AccountId = EOS_Auth_GetLoggedInAccountByIndex(m_Subsystem->m_AuthHandle, LocalUserNum);
	const EOS_EResult CopyResult = EOS_Auth_CopyUserAuthToken(m_Subsystem->m_AuthHandle, &CopyOptions, AccountId, &AuthToken);

	if (CopyResult == EOS_EResult::EOS_Success)
	{
		EOS_Connect_Credentials Credentials = {};
		Credentials.ApiVersion = EOS_CONNECT_CREDENTIALS_API_LATEST;
		Credentials.Type = EOS_EExternalCredentialType::EOS_ECT_EPIC;
		Credentials.Token = AuthToken->AccessToken;

		EOS_Connect_LoginOptions Options = {};
		Options.ApiVersion = EOS_CONNECT_LOGIN_API_LATEST;
		Options.Credentials = &Credentials;

		FConnectLoginCallback* CallbackObj = new FConnectLoginCallback();
		CallbackObj->m_CallbackLambda = [LocalUserNum, this, bCreateUserAccountIfInvalidUser](const EOS_Connect_LoginCallbackInfo* Data)
		{
			if (Data->ResultCode == EOS_EResult::EOS_Success)
			{
				FullLoginCallback(LocalUserNum);
			}
			else if (Data->ResultCode == EOS_EResult::EOS_InvalidUser && bCreateUserAccountIfInvalidUser)
			{
				CreateConnectedLogin(LocalUserNum, Data->ContinuanceToken);
			}
			else
			{
				UE_LOG_ONLINE(Error, TEXT("ConnectLogin(%d) failed with EOS result code (%s)"), LocalUserNum, ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
				Logout(LocalUserNum);
			}
		};

		m_LastUsedCredentialsType.Emplace(LocalUserNum, EOS_EExternalCredentialType::EOS_ECT_EPIC);
		EOS_Connect_Login(m_Subsystem->m_ConnectHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());
		EOS_Auth_Token_Release(AuthToken);
	}
	else
	{
		UE_LOG_ONLINE(Error, TEXT("ConnectLogin(%d) failed with EOS result code (%s)"), LocalUserNum, ANSI_TO_TCHAR(EOS_EResult_ToString(CopyResult)));
		Logout(LocalUserNum);
	}
}

static FString LoginStatusToString(EOS_ELoginStatus status)
{
	switch (status)
	{
	case EOS_ELoginStatus::EOS_LS_NotLoggedIn:
		return "EOS_LS_NotLoggedIn";
		break;
	case EOS_ELoginStatus::EOS_LS_UsingLocalProfile:
		return "EOS_LS_UsingLocalProfile";
		break;
	case EOS_ELoginStatus::EOS_LS_LoggedIn:
		return "EOS_LS_LoggedIn";
		break;
	}

	return "";
}

void FOnlineIdentityEOSCore::RefreshConnectLogin(int32 LocalUserNum)
{
	LogVerbose("");

	if (m_LastUsedCredentialsType.Contains(LocalUserNum))
	{
		const auto CredentialsType = m_LastUsedCredentialsType[LocalUserNum];

		if (CredentialsType != EOS_EExternalCredentialType::EOS_ECT_EPIC && CredentialsType != EOS_EExternalCredentialType::EOS_ECT_DEVICEID_ACCESS_TOKEN)
		{
			LogWarning("Only Epic Accounts and DeviceId accounts supports refreshing login tokens automatically. -- Aborting");
			return;
		}
	}

	EOS_ELoginStatus AuthLogin = GetAuthLoggedInStatus(LocalUserNum);
	EOS_ELoginStatus ConnectLogin = GetConnectLoggedInStatus(LocalUserNum);

	LogVerbose("AuthLogin Status: %s", *LoginStatusToString(AuthLogin));
	LogVerbose("ConnectLogin Status: %s", *LoginStatusToString(ConnectLogin));

	EOS_EpicAccountId AccountId = GetLocalEpicAccountId(LocalUserNum);
	EOS_ProductUserId ProductUserId = GetLocalProductUserId(LocalUserNum);
	EOS_Auth_Token* AuthToken = nullptr;
	EOS_Auth_CopyUserAuthTokenOptions CopyOptions = {};
	CopyOptions.ApiVersion = EOS_AUTH_COPYUSERAUTHTOKEN_API_LATEST;

	const EOS_EResult CopyResult = EOS_Auth_CopyUserAuthToken(m_Subsystem->m_AuthHandle, &CopyOptions, AccountId, &AuthToken);
	
	if (CopyResult == EOS_EResult::EOS_Success)
	{
		EOS_Connect_Credentials Credentials = {};
		Credentials.ApiVersion = EOS_CONNECT_CREDENTIALS_API_LATEST;
		Credentials.Type = EOS_EExternalCredentialType::EOS_ECT_EPIC;
		Credentials.Token = AuthToken->AccessToken;

		EOS_Connect_LoginOptions Options = {};
		Options.ApiVersion = EOS_CONNECT_LOGIN_API_LATEST;
		Options.Credentials = &Credentials;

		FConnectLoginCallback* CallbackObj = new FConnectLoginCallback();
		CallbackObj->m_CallbackLambda = [LocalUserNum, AccountId, this](const EOS_Connect_LoginCallbackInfo* Data)
		{
			if (Data->ResultCode != EOS_EResult::EOS_Success)
			{
				UE_LOG_ONLINE(Error, TEXT("Failed to refresh ConnectLogin(%d) failed with EOS result code (%s)"), LocalUserNum, ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
				Logout(LocalUserNum);
			}
			else
			{
				UE_LOG_ONLINE(Log, TEXT("RefreshConnectLogin (AUTH) user (%d) Result: (%s) "), LocalUserNum, ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
			}
		};

		EOS_Connect_Login(m_Subsystem->m_ConnectHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());
		EOS_Auth_Token_Release(AuthToken);
	}
	else
	{
		EOS_Connect_UserLoginInfo DeviceIdUserLoginInfo = {};
		DeviceIdUserLoginInfo.ApiVersion = EOS_CONNECT_USERLOGININFO_API_LATEST;
		
		if (m_LastUsedUserLoginInfoDisplayName.Contains(LocalUserNum))
		{
			DeviceIdUserLoginInfo.DisplayName = TCHAR_TO_UTF8(*m_LastUsedUserLoginInfoDisplayName[LocalUserNum]);
		}
		else
		{
			DeviceIdUserLoginInfo.DisplayName = "INVALID";
		}

		EOS_Connect_Credentials Credentials = {};
		Credentials.ApiVersion = EOS_CONNECT_CREDENTIALS_API_LATEST;
		Credentials.Type = EOS_EExternalCredentialType::EOS_ECT_DEVICEID_ACCESS_TOKEN;
		Credentials.Token = nullptr;

		EOS_Connect_LoginOptions Options = {};
		Options.ApiVersion = EOS_CONNECT_LOGIN_API_LATEST;
		Options.Credentials = &Credentials;
		Options.UserLoginInfo = &DeviceIdUserLoginInfo;

		FConnectLoginCallback* CallbackObj = new FConnectLoginCallback();
		CallbackObj->m_CallbackLambda = [LocalUserNum, AccountId, this](const EOS_Connect_LoginCallbackInfo* Data)
		{
			if (Data->ResultCode != EOS_EResult::EOS_Success)
			{
				UE_LOG_ONLINE(Error, TEXT("Failed to refresh ConnectLogin(%d) failed with EOS result code (%s)"), LocalUserNum, ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
				Logout(LocalUserNum);
			}
			else
			{
				UE_LOG_ONLINE(Log, TEXT("RefreshConnectLogin user (%d) Result: (%s) "), LocalUserNum, ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
			}
		};

		EOS_Connect_Login(m_Subsystem->m_ConnectHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());
	}
}
	
void FOnlineIdentityEOSCore::CreateConnectedLogin(int32 LocalUserNum, EOS_ContinuanceToken Token)
{
	LogVerbose("");

	EOS_Connect_CreateUserOptions Options = {};
	Options.ApiVersion = EOS_CONNECT_CREATEUSER_API_LATEST;
	Options.ContinuanceToken = Token;

	FCreateUserCallback* CallbackObj = new FCreateUserCallback();
	CallbackObj->m_CallbackLambda = [LocalUserNum, this](const EOS_Connect_CreateUserCallbackInfo* Data)
	{
		if (Data->ResultCode == EOS_EResult::EOS_Success)
		{
			FullLoginCallback(LocalUserNum);
		}
		else
		{
			const FString ErrorString = FString::Printf(TEXT("Login(%d) failed with EOS result code (%s)"), LocalUserNum, ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
			TriggerOnLoginCompleteDelegates(LocalUserNum, false, FUniqueNetIdEOSCore(), ErrorString);
		}
	};

	EOS_Connect_CreateUser(m_Subsystem->m_ConnectHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());
}

struct FCreateDeviceIdCallbackOSS
{
	FOnlineIdentityEOSCore* m_IdentityPtr;
	int32 m_LocalUserNum;
	FOnlineAccountCredentials m_Credentials;
	bool bInvokeLoginOnSuccess;
};

void FOnlineIdentityEOSCore::CreateDeviceId(int32 localUserNum, const FOnlineAccountCredentials& credentials, bool bInvokeLoginOnSuccess /* = false */)
{
	LogVerbose("");

	FCreateDeviceIdCallbackOSS* CreateDeviceIdCallbackObj = new FCreateDeviceIdCallbackOSS;
	CreateDeviceIdCallbackObj->m_Credentials = credentials;
	CreateDeviceIdCallbackObj->m_IdentityPtr = this;
	CreateDeviceIdCallbackObj->m_LocalUserNum = localUserNum;
	CreateDeviceIdCallbackObj->bInvokeLoginOnSuccess = bInvokeLoginOnSuccess;

	auto CreateDeviceIdCallback = [](const EOS_Connect_CreateDeviceIdCallbackInfo* Data)
	{
		const FCreateDeviceIdCallbackOSS* CallbackObj = static_cast<FCreateDeviceIdCallbackOSS*>(Data->ClientData);
		check(CallbackObj);

		if (CallbackObj)
		{
			const FString ErrorString = FString::Printf(TEXT("EOS_Connect_CreateDeviceId EOS result code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));

			UE_LOG_ONLINE(Warning, TEXT("%s"), *ErrorString);

			if (Data->ResultCode == EOS_EResult::EOS_Success && CallbackObj->bInvokeLoginOnSuccess)
			{
				CallbackObj->m_IdentityPtr->Login(CallbackObj->m_LocalUserNum, CallbackObj->m_Credentials);
			}
		}
	};

	const FTCHARToUTF8 ConvertedDeviceModel(*credentials.Id);

	EOS_Connect_CreateDeviceIdOptions CreateDeviceIdOptions = { };
	CreateDeviceIdOptions.ApiVersion = EOS_CONNECT_CREATEDEVICEID_API_LATEST;
	CreateDeviceIdOptions.DeviceModel = ConvertedDeviceModel.Get();

	EOS_Connect_CreateDeviceId(m_Subsystem->m_ConnectHandle, &CreateDeviceIdOptions, CreateDeviceIdCallbackObj, CreateDeviceIdCallback);
}

void FOnlineIdentityEOSCore::FullLoginCallback(int32 LocalUserNum)
{
	LogVerbose("");

	const EOS_EpicAccountId AccountId = EOS_Auth_GetLoggedInAccountByIndex(m_Subsystem->m_AuthHandle, LocalUserNum);
	const EOS_ProductUserId ProductUserId = EOS_Connect_GetLoggedInUserByIndex(m_Subsystem->m_ConnectHandle, LocalUserNum);

	if (!LocalUserNumToConnectLoginNotifcationMap.Contains(LocalUserNum))
	{
		FNotificationIdCallbackPair* NotificationPair = new FNotificationIdCallbackPair();
		LocalUserNumToConnectLoginNotifcationMap.Add(LocalUserNum, NotificationPair);

		FRefreshAuthCallback* CallbackObj = new FRefreshAuthCallback();
		NotificationPair->Callback = CallbackObj;

		CallbackObj->m_CallbackLambda = [LocalUserNum, this](const EOS_Connect_AuthExpirationCallbackInfo* Data)
		{
			RefreshConnectLogin(LocalUserNum);
		};

		EOS_Connect_AddNotifyAuthExpirationOptions Options = {};
		Options.ApiVersion = EOS_CONNECT_ADDNOTIFYAUTHEXPIRATION_API_LATEST;
		NotificationPair->NotificationId = EOS_Connect_AddNotifyAuthExpiration(m_Subsystem->m_ConnectHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());
	}

	AddLocalUser(LocalUserNum, AccountId, ProductUserId);
	const FUniqueNetIdEOSCorePtr UserNetId = GetLocalUniqueNetIdEOS(LocalUserNum);
	check(UserNetId.IsValid());

	TriggerOnLoginCompleteDelegates(LocalUserNum, true, *UserNetId, FString());
	TriggerOnLoginStatusChangedDelegates(LocalUserNum, ELoginStatus::NotLoggedIn, ELoginStatus::LoggedIn, *UserNetId);
}

typedef TEOSCallback<EOS_Auth_OnLogoutCallback, EOS_Auth_LogoutCallbackInfo> FLogoutCallback;

bool FOnlineIdentityEOSCore::Logout(int32 LocalUserNum)
{
	LogVerbose("");

	const FUniqueNetIdEOSCorePtr UserId = GetLocalUniqueNetIdEOS(LocalUserNum);
	if (!UserId.IsValid())
	{
		UE_LOG_ONLINE(Warning, TEXT("No logged in user found for LocalUserNum=%d."), LocalUserNum);
		TriggerOnLogoutCompleteDelegates(LocalUserNum, false);
		return false;
	}

	if (GetAuthLoggedInStatus(LocalUserNum) != EOS_ELoginStatus::EOS_LS_NotLoggedIn)
	{
		FLogoutCallback* CallbackObj = new FLogoutCallback();
		CallbackObj->m_CallbackLambda = [LocalUserNum, this](const EOS_Auth_LogoutCallbackInfo* Data)
		{
			if (Data->ResultCode == EOS_EResult::EOS_Success)
			{
				RemoveLocalUser(LocalUserNum);

				TriggerOnLogoutCompleteDelegates(LocalUserNum, true);
			}
			else
			{
				TriggerOnLogoutCompleteDelegates(LocalUserNum, false);
			}
		};

		EOS_Auth_LogoutOptions LogoutOptions = {};
		LogoutOptions.ApiVersion = EOS_AUTH_LOGOUT_API_LATEST;
		LogoutOptions.LocalUserId = StringToAccountIdMap[UserId->UniqueNetIdStr];

		EOS_Auth_Logout(m_Subsystem->m_AuthHandle, &LogoutOptions, CallbackObj, CallbackObj->GetCallbackPtr());
	}
	else
	{
		RemoveLocalUser(LocalUserNum);
		TriggerOnLogoutCompleteDelegates(LocalUserNum, true);
	}

	LocalUserNumToLastLoginCredentials.Remove(LocalUserNum);
	
	return true;
}

bool FOnlineIdentityEOSCore::AutoLogin(int32 LocalUserNum)
{
	LogVerbose("");

	FString LoginId;
	FString Token;
	FString AuthType;

	FParse::Value(FCommandLine::Get(), TEXT("AUTH_LOGIN="), LoginId);
	FParse::Value(FCommandLine::Get(), TEXT("AUTH_TOKEN="), Token);
	FParse::Value(FCommandLine::Get(), TEXT("AUTH_TYPE="), AuthType);

	int32 Type = 0;
	int32 CredentialsType = 0;

	if (AuthType.Contains("password"))
	{
		Type = 0;
	}
	else if (AuthType.Contains("exchangecode"))
	{
		Type = 1;
	}
	else if (AuthType.Contains("persistentauth") || AuthType.Contains("autologin"))
	{
		Type = 2;
	}
	else if (AuthType.Contains("devicecode"))
	{
		Type = 3;
	}
	else if (AuthType.Contains("developer") || AuthType.Contains("devtool"))
	{
		Type = 4;
	}
	else if (AuthType.Contains("refreshtoken"))
	{
		Type = 5;
	}
	else if (AuthType.Contains("portal"))
	{
		Type = 6;
	}
	else if (AuthType.Contains("external"))
	{
		Type = 7;
	}
	else if (AuthType.Contains("deviceid"))
	{
		Type = 0;
		CredentialsType = 10;

		if (IsRunningDedicatedServer() && LoginId.Len() == 0)
		{
			LoginId = "DedicatedServer";
		}
	}

	AuthType = FString::Printf(TEXT("%d|%d"), Type, CredentialsType);

	if (AuthType.IsEmpty())
	{
		UE_LOG_ONLINE(Warning, TEXT("Unable to AutoLogin user (%d) due to missing auth command line args"), LocalUserNum);
		return false;
	}

	FOnlineAccountCredentials Creds(AuthType, LoginId, Token);

	LocalUserNumToLastLoginCredentials.Emplace(LocalUserNum, MakeShared<FOnlineAccountCredentials>(Creds));
	
	return Login(LocalUserNum, Creds);
}

void FOnlineIdentityEOSCore::AddLocalUser(int32 LocalUserNum, EOS_EpicAccountId EpicAccountId, EOS_ProductUserId UserId)
{
	LogVerbose("");

	if (m_DefaultLocalUser == -1)
	{
		m_DefaultLocalUser = LocalUserNum;
	}

	const FString& NetId = MakeNetIdStringFromIds(EpicAccountId, UserId);
	const FUniqueNetIdEOSCoreRef UserNetId(new FUniqueNetIdEOSCore(NetId));
	const FUserOnlineAccountEOSRef UserAccountRef(new FUserOnlineAccountEOS(UserNetId));

	UserNumToNetIdMap.Add(LocalUserNum, UserNetId);
	AccountIdToUserNumMap.Add(EpicAccountId, LocalUserNum);
	NetIdStringToOnlineUserMap.Add(*NetId, UserAccountRef);
	StringToUserAccountMap.Add(NetId, UserAccountRef);
	AccountIdToStringMap.Add(EpicAccountId, NetId);
	ProductUserIdToStringMap.Add(UserId, *NetId);
	StringToAccountIdMap.Add(NetId, EpicAccountId);
	EpicAccountIdToAttributeAccessMap.Add(EpicAccountId, UserAccountRef);
	ProductUserIdToUserNumMap.Add(UserId, LocalUserNum);
	StringToProductUserIdMap.Add(NetId, UserId);

	EOS_Auth_Token* AuthToken = nullptr;
	EOS_Auth_CopyUserAuthTokenOptions Options = {};
	Options.ApiVersion = EOS_AUTH_COPYUSERAUTHTOKEN_API_LATEST;

	const EOS_EResult CopyResult = EOS_Auth_CopyUserAuthToken(m_Subsystem->m_AuthHandle, &Options, EpicAccountId, &AuthToken);
	if (CopyResult == EOS_EResult::EOS_Success)
	{
		UserAccountRef->SetAuthAttribute(AUTH_ATTR_ID_TOKEN, AuthToken->AccessToken);
		EOS_Auth_Token_Release(AuthToken);

		UpdateUserInfo(UserAccountRef, EpicAccountId, EpicAccountId);
	}
}

void FOnlineIdentityEOSCore::UpdateUserInfo(IAttributeAccessInterfaceRef AttributeAccessRef, EOS_EpicAccountId LocalId, EOS_EpicAccountId AccountId)
{
	LogVerbose("");

	EOS_UserInfo_CopyUserInfoOptions Options = {};
	Options.ApiVersion = EOS_USERINFO_COPYUSERINFO_API_LATEST;
	Options.LocalUserId = LocalId;
	Options.TargetUserId = AccountId;

	EOS_UserInfo* UserInfo = nullptr;

	const EOS_EResult CopyResult = EOS_UserInfo_CopyUserInfo(m_Subsystem->m_UserInfoHandle, &Options, &UserInfo);
	if (CopyResult == EOS_EResult::EOS_Success)
	{
		AttributeAccessRef->SetInternalAttribute(USER_ATTR_DISPLAY_NAME, UserInfo->DisplayName);
		AttributeAccessRef->SetInternalAttribute(USER_ATTR_COUNTRY, UserInfo->Country);
		AttributeAccessRef->SetInternalAttribute(USER_ATTR_LANG, UserInfo->PreferredLanguage);
		EOS_UserInfo_Release(UserInfo);
	}
}

TSharedPtr<FUserOnlineAccount> FOnlineIdentityEOSCore::GetUserAccount(const FUniqueNetId& UserId) const
{
	LogVerbose("");
	
	TSharedPtr<FUserOnlineAccount> Result;

	const FUniqueNetIdEOSCore EOSID(UserId);
	const FUserOnlineAccountEOSRef* FoundUserAccount = StringToUserAccountMap.Find(EOSID.UniqueNetIdStr);
	if (FoundUserAccount != nullptr)
	{
		return *FoundUserAccount;
	}
	return nullptr;
}

TArray<TSharedPtr<FUserOnlineAccount>> FOnlineIdentityEOSCore::GetAllUserAccounts() const
{
	TArray<TSharedPtr<FUserOnlineAccount>> Result;

	for (TMap<FString, FUserOnlineAccountEOSRef>::TConstIterator It(StringToUserAccountMap); It; ++It)
	{
		Result.Add(It.Value());
	}
	return Result;
}

TSharedPtr<const FUniqueNetId> FOnlineIdentityEOSCore::GetUniquePlayerId(int32 LocalUserNum) const
{
	return GetLocalUniqueNetIdEOS(LocalUserNum);
}

int32 FOnlineIdentityEOSCore::GetLocalUserNumFromUniqueNetId(const FUniqueNetId& NetId) const
{
	FUniqueNetIdEOSCore EosId(NetId);
	if (StringToAccountIdMap.Contains(EosId.UniqueNetIdStr))
	{
		EOS_EpicAccountId AccountId = StringToAccountIdMap[EosId.UniqueNetIdStr];
		if (AccountIdToUserNumMap.Contains(AccountId))
		{
			return AccountIdToUserNumMap[AccountId];
		}
	}
	return m_DefaultLocalUser;
}

FUniqueNetIdEOSCorePtr FOnlineIdentityEOSCore::GetLocalUniqueNetIdEOS(int32 LocalUserNum) const
{
	const FUniqueNetIdEOSCorePtr* FoundId = UserNumToNetIdMap.Find(LocalUserNum);
	if (FoundId != nullptr)
	{
		return *FoundId;
	}
	return nullptr;
}

FUniqueNetIdEOSCorePtr FOnlineIdentityEOSCore::GetLocalUniqueNetIdEOS(EOS_ProductUserId UserId) const
{
	if (ProductUserIdToUserNumMap.Contains(UserId))
	{
		return GetLocalUniqueNetIdEOS(ProductUserIdToUserNumMap[UserId]);
	}
	return nullptr;
}

FUniqueNetIdEOSCorePtr FOnlineIdentityEOSCore::GetLocalUniqueNetIdEOS(EOS_EpicAccountId AccountId) const
{
	if (AccountIdToUserNumMap.Contains(AccountId))
	{
		return GetLocalUniqueNetIdEOS(AccountIdToUserNumMap[AccountId]);
	}
	return nullptr;
}


EOS_ELoginStatus FOnlineIdentityEOSCore::GetAuthLoggedInStatus(int32 UserId) const
{
	return EOS_Auth_GetLoginStatus(m_Subsystem->m_AuthHandle, EOS_Auth_GetLoggedInAccountByIndex(m_Subsystem->m_AuthHandle, UserId));
}


EOS_ELoginStatus FOnlineIdentityEOSCore::GetConnectLoggedInStatus(int32 UserId) const
{
	return EOS_Connect_GetLoginStatus(m_Subsystem->m_ConnectHandle, EOS_Connect_GetLoggedInUserByIndex(m_Subsystem->m_ConnectHandle, UserId));
}

EOS_EpicAccountId FOnlineIdentityEOSCore::GetLocalEpicAccountId(int32 LocalUserNum) const
{
	return EOS_Auth_GetLoggedInAccountByIndex(m_Subsystem->m_AuthHandle, LocalUserNum);
}

EOS_EpicAccountId FOnlineIdentityEOSCore::GetLocalEpicAccountId() const
{
	return GetLocalEpicAccountId(m_DefaultLocalUser);
}

EOS_ProductUserId FOnlineIdentityEOSCore::GetLocalProductUserId(int32 LocalUserNum) const
{
	EOS_ProductUserId ProductId = EOS_Connect_GetLoggedInUserByIndex(m_Subsystem->m_ConnectHandle, LocalUserNum);

	return ProductId;
}

EOS_ProductUserId FOnlineIdentityEOSCore::GetLocalProductUserId() const
{
	return GetLocalProductUserId(m_DefaultLocalUser);
}

EOS_EpicAccountId FOnlineIdentityEOSCore::GetLocalEpicAccountId(EOS_ProductUserId UserId) const
{
	if (ProductUserIdToUserNumMap.Contains(UserId))
	{
		return GetLocalEpicAccountId(ProductUserIdToUserNumMap[UserId]);
	}
	return nullptr;
}

EOS_ProductUserId FOnlineIdentityEOSCore::GetLocalProductUserId(EOS_EpicAccountId AccountId) const
{
	if (AccountIdToUserNumMap.Contains(AccountId))
	{
		return GetLocalProductUserId(AccountIdToUserNumMap[AccountId]);
	}
	return nullptr;
}

EOS_EpicAccountId FOnlineIdentityEOSCore::GetEpicAccountId(const FUniqueNetId& NetId) const
{
	const FUniqueNetIdEOSCore EOSId(NetId);

	if (StringToAccountIdMap.Contains(EOSId.UniqueNetIdStr))
	{
		return StringToAccountIdMap[EOSId.UniqueNetIdStr];
	}
	return nullptr;
}

EOS_ProductUserId FOnlineIdentityEOSCore::GetProductUserId(const FUniqueNetId& NetId) const
{
	const FUniqueNetIdEOSCore EOSId(NetId);

	if (StringToProductUserIdMap.Contains(EOSId.UniqueNetIdStr))
	{
		return StringToProductUserIdMap[EOSId.UniqueNetIdStr];
	}
	return nullptr;
}

FOnlineUserPtr FOnlineIdentityEOSCore::GetLocalOnlineUser(int32 LocalUserNum) const
{
	FOnlineUserPtr OnlineUser;
	if (UserNumToNetIdMap.Contains(LocalUserNum))
	{
		const FUniqueNetIdEOSCorePtr NetId = UserNumToNetIdMap.FindRef(LocalUserNum);
		if (NetIdStringToOnlineUserMap.Contains(*NetId->UniqueNetIdStr))
		{
			OnlineUser = NetIdStringToOnlineUserMap.FindRef(*NetId->UniqueNetIdStr);
		}
	}
	return OnlineUser;
}

FOnlineUserPtr FOnlineIdentityEOSCore::GetOnlineUser(EOS_ProductUserId UserId) const
{
	FOnlineUserPtr OnlineUser;
	if (ProductUserIdToStringMap.Contains(UserId))
	{
		const FString& NetId = ProductUserIdToStringMap.FindRef(UserId);
		if (NetIdStringToOnlineUserMap.Contains(*NetId))
		{
			OnlineUser = NetIdStringToOnlineUserMap.FindRef(*NetId);
		}
	}
	return OnlineUser;
}

FOnlineUserPtr FOnlineIdentityEOSCore::GetOnlineUser(EOS_EpicAccountId AccountId) const
{
	FOnlineUserPtr OnlineUser;
	if (AccountIdToStringMap.Contains(AccountId))
	{
		const FString& NetId = AccountIdToStringMap.FindRef(AccountId);
		if (NetIdStringToOnlineUserMap.Contains(*NetId))
		{
			OnlineUser = NetIdStringToOnlineUserMap.FindRef(*NetId);
		}
	}
	return OnlineUser;
}

void FOnlineIdentityEOSCore::GetPlatformAuthToken(int32 LocalUserNum, const FOnGetLinkedAccountAuthTokenCompleteDelegate& Delegate) const
{
	const IOnlineSubsystem* PlatformOSS = GetPlatformOSS();
	if (PlatformOSS == nullptr)
	{
		UE_LOG_ONLINE(Error, TEXT("ConnectLoginNoEAS(%d) failed due to no platform OSS"), LocalUserNum);
		Delegate.ExecuteIfBound(LocalUserNum, false, FExternalAuthToken());
		return;
	}
	const IOnlineIdentityPtr PlatformIdentity = PlatformOSS->GetIdentityInterface();
	if (!PlatformIdentity.IsValid())
	{
		UE_LOG_ONLINE(Error, TEXT("ConnectLoginNoEAS(%d) failed due to no platform OSS identity interface"), LocalUserNum);
		Delegate.ExecuteIfBound(LocalUserNum, false, FExternalAuthToken());
		return;
	}
	// Request the auth token from the platform
	PlatformIdentity->GetLinkedAccountAuthToken(LocalUserNum, Delegate);
}

IOnlineSubsystem* FOnlineIdentityEOSCore::GetPlatformOSS() const
{
	IOnlineSubsystem* PlatformOSS = IOnlineSubsystem::GetByPlatform();
	if (PlatformOSS == nullptr)
#if !PLATFORM_DESKTOP
	{
		UE_LOG_ONLINE(Error, TEXT("GetPlatformOSS() failed due to no platform OSS being configured"));
	}
#else
{
	// Attempt to load Steam before treating it as an error
	PlatformOSS = IOnlineSubsystem::Get(STEAM_SUBSYSTEM);
}
#endif
	return PlatformOSS;
}

void FOnlineIdentityEOSCore::RemoveLocalUser(int32 LocalUserNum)
{
	const FUniqueNetIdEOSCorePtr* FoundId = UserNumToNetIdMap.Find(LocalUserNum);
	if (FoundId != nullptr)
	{
		const FString& NetId = (*FoundId)->UniqueNetIdStr;
		const EOS_EpicAccountId AccountId = StringToAccountIdMap[NetId];
		AccountIdToStringMap.Remove(AccountId);
		AccountIdToUserNumMap.Remove(AccountId);
		NetIdStringToOnlineUserMap.Remove(NetId);
		StringToAccountIdMap.Remove(NetId);
		StringToUserAccountMap.Remove(NetId);
		UserNumToNetIdMap.Remove(LocalUserNum);
		m_LastUsedUserLoginInfoDisplayName.Remove(LocalUserNum);
		m_LastUsedCredentialsType.Remove(LocalUserNum);
	}
	if (LocalUserNum == m_DefaultLocalUser)
	{
		m_DefaultLocalUser = -1;
	}
}

TSharedPtr<const FUniqueNetId> FOnlineIdentityEOSCore::CreateUniquePlayerId(uint8* Bytes, int32 Size)
{
	if (Bytes != nullptr && Size >= 32)
	{
		return MakeShareable(new FUniqueNetIdEOSCore(Bytes, 32));
	}
	return nullptr;
}

TSharedPtr<const FUniqueNetId> FOnlineIdentityEOSCore::CreateUniquePlayerId(const FString& Str)
{
	return MakeShareable(new FUniqueNetIdEOSCore(Str));
}

ELoginStatus::Type FOnlineIdentityEOSCore::GetLoginStatus(int32 LocalUserNum) const
{
	const FUniqueNetIdEOSCorePtr UserId = GetLocalUniqueNetIdEOS(LocalUserNum);
	if (UserId.IsValid())
	{
		return GetLoginStatus(*UserId);
	}
	return ELoginStatus::NotLoggedIn;
}

ELoginStatus::Type FOnlineIdentityEOSCore::GetLoginStatus(const FUniqueNetIdEOSCore& UserId) const
{
	if (!StringToAccountIdMap.Contains(UserId.UniqueNetIdStr))
	{
		return ELoginStatus::NotLoggedIn;
	}

	// are we using a local account?
	if (UserId.m_EpicAccountIdStr == EMPTY_EASID)
	{
		const EOS_ProductUserId ProductUserId = EOS_ProductUserId_FromString(TCHAR_TO_UTF8(*UserId.m_ProductUserIdStr));

		if (EOS_ProductUserId_IsValid(ProductUserId))
		{
			const EOS_ELoginStatus ConnectLoginStatus = EOS_Connect_GetLoginStatus(m_Subsystem->m_ConnectHandle, ProductUserId);

			switch (ConnectLoginStatus)
			{
			case EOS_ELoginStatus::EOS_LS_LoggedIn:
				{
					return ELoginStatus::LoggedIn;
				}
			case EOS_ELoginStatus::EOS_LS_UsingLocalProfile:
				{
					return ELoginStatus::UsingLocalProfile;
				}
			}
		}
	}

	const EOS_EpicAccountId AccountId = StringToAccountIdMap[UserId.UniqueNetIdStr];
	if (AccountId == nullptr)
	{
		return ELoginStatus::NotLoggedIn;
	}

	const EOS_ELoginStatus LoginStatus = EOS_Auth_GetLoginStatus(m_Subsystem->m_AuthHandle, AccountId);
	switch (LoginStatus)
	{
	case EOS_ELoginStatus::EOS_LS_LoggedIn:
		{
			return ELoginStatus::LoggedIn;
		}
	case EOS_ELoginStatus::EOS_LS_UsingLocalProfile:
		{
			return ELoginStatus::UsingLocalProfile;
		}
	}
	return ELoginStatus::NotLoggedIn;
}

ELoginStatus::Type FOnlineIdentityEOSCore::GetLoginStatus(const FUniqueNetId& UserId) const
{
	FUniqueNetIdEOSCore EosId(UserId);
	return GetLoginStatus(EosId);
}

FString FOnlineIdentityEOSCore::GetPlayerNickname(int32 LocalUserNum) const
{
	const FUniqueNetIdEOSCorePtr UserId = GetLocalUniqueNetIdEOS(LocalUserNum);
	if (UserId.IsValid())
	{
		const TSharedPtr<FUserOnlineAccount> UserAccount = GetUserAccount(*UserId);
		if (UserAccount.IsValid())
		{
			return UserAccount->GetDisplayName();
		}
	}
	return FString();
}

FString FOnlineIdentityEOSCore::GetPlayerNickname(const FUniqueNetId& UserId) const
{
	const TSharedPtr<FUserOnlineAccount> UserAccount = GetUserAccount(UserId);
	if (UserAccount.IsValid())
	{
		return UserAccount->GetDisplayName();
	}
	return FString();
}

FString FOnlineIdentityEOSCore::GetAuthToken(int32 LocalUserNum) const
{
	LogVerbose("GetAuthToken");
	
	const TSharedPtr<const FUniqueNetId> UserId = GetUniquePlayerId(LocalUserNum);
	if (UserId.IsValid())
	{
		const TSharedPtr<FUserOnlineAccount> UserAccount = GetUserAccount(*UserId);
		if (UserAccount.IsValid())
		{
			FString AccessToken = UserAccount->GetAccessToken();
			LogVerbose("UserAccount GetAccessToken: %s", *AccessToken);
			
			return AccessToken;
		}
	}
	return FString();
}

void FOnlineIdentityEOSCore::RevokeAuthToken(const FUniqueNetId& LocalUserId, const FOnRevokeAuthTokenCompleteDelegate& Delegate)
{
	Delegate.ExecuteIfBound(LocalUserId, FOnlineError(EOnlineErrorResult::NotImplemented));
}

FPlatformUserId FOnlineIdentityEOSCore::GetPlatformUserIdFromUniqueNetId(const FUniqueNetId& UniqueNetId) const
{
#if UE_VERSION_OLDER_THAN(5,0,3)
	return GetLocalUserNumFromUniqueNetId(UniqueNetId);
#else
	return GetPlatformUserIdFromLocalUserNum(GetLocalUserNumFromUniqueNetId(UniqueNetId));
#endif
}

void FOnlineIdentityEOSCore::GetLinkedAccountAuthToken(int32 LocalUserNum, const FOnGetLinkedAccountAuthTokenCompleteDelegate& Delegate) const
{
	LogVerbose("GetLinkedAccountAuthToken");
	
	FExternalAuthToken ExternalToken;
	ExternalToken.TokenString = GetAuthToken(LocalUserNum);
	Delegate.ExecuteIfBound(LocalUserNum, ExternalToken.HasTokenString(), ExternalToken);
}

void FOnlineIdentityEOSCore::GetUserPrivilege(const FUniqueNetId& UserId, EUserPrivileges::Type Privilege, const FOnGetUserPrivilegeCompleteDelegate& Delegate)
{
	Delegate.ExecuteIfBound(UserId, Privilege, static_cast<uint32>(EPrivilegeResults::NoFailures));
}

FString FOnlineIdentityEOSCore::GetAuthType() const
{
	return TEXT("epic");
}

typedef TEOSCallback<EOS_UserInfo_OnQueryUserInfoCallback, EOS_UserInfo_QueryUserInfoCallbackInfo> FReadUserInfoCallback;

void FOnlineIdentityEOSCore::ReadUserInfo(EOS_EpicAccountId EpicAccountId)
{
	FReadUserInfoCallback* CallbackObj = new FReadUserInfoCallback();
	CallbackObj->m_CallbackLambda = [this](const EOS_UserInfo_QueryUserInfoCallbackInfo* Data)
	{
		if (Data->ResultCode == EOS_EResult::EOS_Success)
		{
			const IAttributeAccessInterfaceRef AttributeAccessRef = EpicAccountIdToAttributeAccessMap[Data->TargetUserId];
			UpdateUserInfo(AttributeAccessRef, Data->LocalUserId, Data->TargetUserId);
		}
	};

	EOS_UserInfo_QueryUserInfoOptions Options = {};
	Options.ApiVersion = EOS_USERINFO_QUERYUSERINFO_API_LATEST;
	Options.LocalUserId = GetLocalEpicAccountId(m_DefaultLocalUser);
	Options.TargetUserId = EpicAccountId;
	EOS_UserInfo_QueryUserInfo(m_Subsystem->m_UserInfoHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());
}

struct FQueryByDisplayNameOptions : public EOS_UserInfo_QueryUserInfoByDisplayNameOptions
{
	FQueryByDisplayNameOptions()
		: EOS_UserInfo_QueryUserInfoByDisplayNameOptions()
	{
		ApiVersion = EOS_USERINFO_QUERYUSERINFOBYDISPLAYNAME_API_LATEST;
		DisplayName = DisplayNameAnsi;
	}

	char DisplayNameAnsi[EOSCORE_OSS_STRING_BUFFER_LENGTH];
};

typedef TEOSCallback<EOS_UserInfo_OnQueryUserInfoByDisplayNameCallback, EOS_UserInfo_QueryUserInfoByDisplayNameCallbackInfo> FQueryInfoByNameCallback;
