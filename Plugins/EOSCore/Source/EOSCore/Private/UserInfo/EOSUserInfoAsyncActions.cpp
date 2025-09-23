/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "UserInfo/EOSUserInfoAsyncActions.h"
#include "UserInfo/EOSUserInfo.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLibrary.h"
#include "Core/EOSCoreLogging.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreUserInfoQueryUserInfo
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreUserInfoQueryUserInfo* UEOSCoreUserInfoQueryUserInfo::EOSUserInfoQueryUserInfoAsync(UObject* WorldContextObject, FEOSUserInfoQueryUserInfoOptions Options)
{
	if (UEOSCoreSubsystem::GetUserInfoHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreUserInfoQueryUserInfo>();
		AsyncObject->RegisterWithGameInstance(GameInstance);
		AsyncObject->m_Options = Options;
		AsyncObject->m_WorldContextObject = WorldContextObject;

		return AsyncObject;
	}
	else
	{
		LogError("Plugin not initialized!");
	}

	return nullptr;
}

void UEOSCoreUserInfoQueryUserInfo::Activate()
{
	Super::Activate();
	UCoreUserInfo::GetUserInfo(m_WorldContextObject)->EOSUserInfoQueryUserInfo(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreUserInfoQueryUserInfoByDisplayName
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreUserInfoQueryUserInfoByDisplayName* UEOSCoreUserInfoQueryUserInfoByDisplayName::EOSUserInfoQueryUserInfoByDisplayNameAsync(UObject* WorldContextObject, FEOSUserInfoQueryUserInfoByDisplayNameOptions Options)
{
	if (UEOSCoreSubsystem::GetUserInfoHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreUserInfoQueryUserInfoByDisplayName>();
		AsyncObject->RegisterWithGameInstance(GameInstance);
		AsyncObject->m_Options = Options;
		AsyncObject->m_WorldContextObject = WorldContextObject;

		return AsyncObject;
	}
	else
	{
		LogError("Plugin not initialized!");
	}

	return nullptr;
}

void UEOSCoreUserInfoQueryUserInfoByDisplayName::Activate()
{
	Super::Activate();
	UCoreUserInfo::GetUserInfo(m_WorldContextObject)->EOSUserInfoQueryUserInfoByDisplayName(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreUserInfoQueryUserInfoByExternalAccount
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreUserInfoByExternalAccount* UEOSCoreUserInfoByExternalAccount::EOSUserInfoQueryUserInfoByExternalAccountAsync(UObject* WorldContextObject, FEOSUserInfoQueryUserInfoByExternalAccountOptions Options)
{
	if (UEOSCoreSubsystem::GetUserInfoHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreUserInfoByExternalAccount>();
		AsyncObject->RegisterWithGameInstance(GameInstance);
		AsyncObject->m_Options = Options;
		AsyncObject->m_WorldContextObject = WorldContextObject;

		return AsyncObject;
	}
	else
	{
		LogError("Plugin not initialized!");
	}

	return nullptr;
}

void UEOSCoreUserInfoByExternalAccount::Activate()
{
	Super::Activate();
	UCoreUserInfo::GetUserInfo(m_WorldContextObject)->EOSUserInfoQueryUserInfoByExternalAccount(m_WorldContextObject, m_Options, m_Callback);
}
