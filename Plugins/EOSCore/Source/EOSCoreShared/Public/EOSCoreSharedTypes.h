/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "eos_auth_types.h"
#include "eos_connect_types.h"
#include "EOSCoreSharedTypes.generated.h"

#if PLATFORM_IOS
#include "eos_ios.h"
#endif

struct EOSCORESHARED_API FAuthCredentials : public EOS_Auth_Credentials
{
	FAuthCredentials(FString id, FString token);
	~FAuthCredentials();

#if PLATFORM_IOS
	EOS_IOS_Auth_CredentialsOptions CredentialsOptions;
#endif
};

/* Flags that describe user permissions */
UENUM(BlueprintType, meta = (Bitflags))
enum class EOSEAuthScopeFlags : uint8
{
	EOS_AS_BasicProfile = 0,
	/** Permissions to see a list of your friends who use this application */
	EOS_AS_FriendsList,
	/** Permissions to set your online presence and see presence of your friends */
	EOS_AS_Presence,
	/** Permissions to manage the Epic friends list. This scope is restricted to Epic first party products, and attempting to use it will result in authentication failures. */
	EOS_AS_FriendsManagement,
	/** Permissions to see email in the response when fetching information for a user. This scope is restricted to Epic first party products, and attempting to use it will result in authentication failures. */
	EOS_AS_Email,
	/** Permissions to see your country */
	EOS_AS_Country,
};

ENUM_CLASS_FLAGS(EOSEAuthScopeFlags);

struct EOSCORESHARED_API FConnectCredentials : public EOS_Connect_Credentials
{
	FConnectCredentials()
	{
		ApiVersion = EOS_CONNECT_CREDENTIALS_API_LATEST;
		Token = nullptr;
		Type = EOS_EExternalCredentialType::EOS_ECT_DEVICEID_ACCESS_TOKEN;
	}
	
	FConnectCredentials(FString AccessToken)
		: EOS_Connect_Credentials()
	{
		ApiVersion = EOS_CONNECT_CREDENTIALS_API_LATEST;

		if (!AccessToken.IsEmpty())
		{
			Token = new char[2048];
			FCStringAnsi::Strncpy(const_cast<char*>(Token), TCHAR_TO_UTF8(*AccessToken), 2048);
		}
	}

	FConnectCredentials(EOS_EExternalCredentialType InType)
		: EOS_Connect_Credentials()
	{
		ApiVersion = EOS_CONNECT_CREDENTIALS_API_LATEST;
		Type = InType;
	}
	
	~FConnectCredentials()
	{
		delete[] Token;
	}
};

struct EOSCORESHARED_API FConnectUserLoginInfo : public EOS_Connect_UserLoginInfo
{
	FConnectUserLoginInfo()
	{
		ApiVersion = EOS_CONNECT_USERLOGININFO_API_LATEST;
		DisplayName = nullptr;
	}
	
	FConnectUserLoginInfo(FString displayName)
		: EOS_Connect_UserLoginInfo()
	{
		ApiVersion = EOS_CONNECT_USERLOGININFO_API_LATEST;

		if (!displayName.IsEmpty())
		{
			DisplayName = new char[EOS_CONNECT_USERLOGININFO_DISPLAYNAME_MAX_LENGTH];
			FCStringAnsi::Strncpy(const_cast<char*>(DisplayName), TCHAR_TO_UTF8(*displayName), EOS_CONNECT_USERLOGININFO_DISPLAYNAME_MAX_LENGTH);
		}
	}
	~FConnectUserLoginInfo()
	{
		delete[] DisplayName;
	}
};