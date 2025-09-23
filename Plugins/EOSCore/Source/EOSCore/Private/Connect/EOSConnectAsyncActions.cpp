/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "Connect/EOSConnectAsyncActions.h"
#include "Connect/EOSConnect.h"
#include "Core/EOSCorePluginPrivatePCH.h" 
#include "Core/EOSCoreLibrary.h"
#include "Core/EOSCoreLogging.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreConnectLogin
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreConnectLogin* UEOSCoreConnectLogin::EOSConnectLoginAsync(UObject* WorldContextObject, FEOSConnectLoginOptions Options)
{
	if (UEOSCoreSubsystem::GetConnectHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreConnectLogin>();
		AsyncObject->m_Options = Options;
		AsyncObject->RegisterWithGameInstance(GameInstance);
		AsyncObject->m_WorldContextObject = WorldContextObject;

		return AsyncObject;
	}
	else
	{
		LogError("Plugin not initialized!");
	}

	return nullptr;
}

void UEOSCoreConnectLogin::Activate()
{
	Super::Activate();

	UCoreConnect::GetConnect(m_WorldContextObject)->EOSConnectLogin(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreConnectCreateUser
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreConnectCreateUser* UEOSCoreConnectCreateUser::EOSConnectCreateUserAsync(UObject* WorldContextObject, FEOSConnectCreateUserOptions Options)
{
	if (UEOSCoreSubsystem::GetConnectHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreConnectCreateUser>();
		AsyncObject->RegisterWithGameInstance(GameInstance);
		AsyncObject->m_WorldContextObject = WorldContextObject;
		AsyncObject->m_Options = Options;

		return AsyncObject;
	}
	else
	{
		LogError("Plugin not initialized!");
	}

	return nullptr;
}

void UEOSCoreConnectCreateUser::Activate()
{
	Super::Activate();

	UCoreConnect::GetConnect(m_WorldContextObject)->EOSConnectCreateUser(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreConnectLinkAccount
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreConnectLinkAccount* UEOSCoreConnectLinkAccount::EOSConnectLinkAccountAsync(UObject* WorldContextObject, FEOSConnectLinkAccountOptions Options)
{
	if (UEOSCoreSubsystem::GetConnectHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreConnectLinkAccount>();
		AsyncObject->RegisterWithGameInstance(GameInstance);
		AsyncObject->m_WorldContextObject = WorldContextObject;
		AsyncObject->m_Options = Options;

		return AsyncObject;
	}
	else
	{
		LogError("Plugin not initialized!");
	}

	return nullptr;
}

void UEOSCoreConnectLinkAccount::Activate()
{
	Super::Activate();

	UCoreConnect::GetConnect(m_WorldContextObject)->EOSConnectLinkAccount(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreConnectUnlinkAccount
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreConnectUnlinkAccount* UEOSCoreConnectUnlinkAccount::EOSConnectUnlinkAccountAsync(UObject* WorldContextObject, FEOSConnectUnlinkAccountOptions Options)
{
	if (UEOSCoreSubsystem::GetConnectHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreConnectUnlinkAccount>();
		AsyncObject->RegisterWithGameInstance(GameInstance);
		AsyncObject->m_WorldContextObject = WorldContextObject;
		AsyncObject->m_Options = Options;

		return AsyncObject;
	}
	else
	{
		LogError("Plugin not initialized!");
	}

	return nullptr;
}

void UEOSCoreConnectUnlinkAccount::Activate()
{
	Super::Activate();

	UCoreConnect::GetConnect(m_WorldContextObject)->EOSConnectUnlinkAccount(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreConnectCreateDeviceId
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreConnectCreateDeviceId* UEOSCoreConnectCreateDeviceId::EOSConnectCreateDeviceIdAsync(UObject* WorldContextObject, FEOSConnectCreateDeviceIdOptions Options)
{
	if (UEOSCoreSubsystem::GetConnectHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreConnectCreateDeviceId>();
		AsyncObject->RegisterWithGameInstance(GameInstance);
		AsyncObject->m_WorldContextObject = WorldContextObject;
		AsyncObject->m_Options = Options;

		return AsyncObject;
	}
	else
	{
		LogError("Plugin not initialized!");
	}

	return nullptr;
}

void UEOSCoreConnectCreateDeviceId::Activate()
{
	Super::Activate();

	UCoreConnect::GetConnect(m_WorldContextObject)->EOSConnectCreateDeviceId(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreConnectDeleteDeviceId
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreConnectDeleteDeviceId* UEOSCoreConnectDeleteDeviceId::EOSConnectDeleteDeviceIdAsync(UObject* WorldContextObject, FEOSConnectDeleteDeviceIdOptions Options)
{
	if (UEOSCoreSubsystem::GetConnectHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreConnectDeleteDeviceId>();
		AsyncObject->RegisterWithGameInstance(GameInstance);
		AsyncObject->m_WorldContextObject = WorldContextObject;
		AsyncObject->m_Options = Options;

		return AsyncObject;
	}
	else
	{
		LogError("Plugin not initialized!");
	}

	return nullptr;
}

void UEOSCoreConnectDeleteDeviceId::Activate()
{
	Super::Activate();

	UCoreConnect::GetConnect(m_WorldContextObject)->EOSConnectDeleteDeviceId(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreConnectTransferDeviceIdAccount
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreConnectTransferDeviceIdAccount* UEOSCoreConnectTransferDeviceIdAccount::EOSConnectTransferDeviceIdAccountAsync(UObject* WorldContextObject, FEOSConnectTransferDeviceIdAccountOptions Options)
{
	if (UEOSCoreSubsystem::GetConnectHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreConnectTransferDeviceIdAccount>();
		AsyncObject->RegisterWithGameInstance(GameInstance);
		AsyncObject->m_WorldContextObject = WorldContextObject;
		AsyncObject->m_Options = Options;

		return AsyncObject;
	}
	else
	{
		LogError("Plugin not initialized!");
	}

	return nullptr;
}

void UEOSCoreConnectTransferDeviceIdAccount::Activate()
{
	Super::Activate();

	UCoreConnect::GetConnect(m_WorldContextObject)->EOSConnectTransferDeviceIdAccount(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreConnectQueryExternalAccountMappings
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreConnectQueryExternalAccountMappings* UEOSCoreConnectQueryExternalAccountMappings::EOSConnectQueryExternalAccountMappingsAsync(UObject* WorldContextObject, FEOSConnectQueryExternalAccountMappingsOptions Options)
{
	if (UEOSCoreSubsystem::GetConnectHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreConnectQueryExternalAccountMappings>();
		AsyncObject->RegisterWithGameInstance(GameInstance);
		AsyncObject->m_WorldContextObject = WorldContextObject;
		AsyncObject->m_Options = Options;

		return AsyncObject;
	}
	else
	{
		LogError("Plugin not initialized!");
	}

	return nullptr;
}

void UEOSCoreConnectQueryExternalAccountMappings::Activate()
{
	Super::Activate();

	UCoreConnect::GetConnect(m_WorldContextObject)->EOSConnectQueryExternalAccountMappings(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreConnectQueryProductUserIdMappings
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreConnectQueryProductUserIdMappings* UEOSCoreConnectQueryProductUserIdMappings::EOSConnectQueryProductUserIdMappingsAsync(UObject* WorldContextObject, FEOSConnectQueryProductUserIdMappingsOptions Options)
{
	if (UEOSCoreSubsystem::GetConnectHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreConnectQueryProductUserIdMappings>();
		AsyncObject->RegisterWithGameInstance(GameInstance);
		AsyncObject->m_WorldContextObject = WorldContextObject;
		AsyncObject->m_Options = Options;

		return AsyncObject;
	}
	else
	{
		LogError("Plugin not initialized!");
	}

	return nullptr;
}

void UEOSCoreConnectQueryProductUserIdMappings::Activate()
{
	Super::Activate();

	UCoreConnect::GetConnect(m_WorldContextObject)->EOSConnectQueryProductUserIdMappings(m_WorldContextObject, m_Options, m_Callback);
}
