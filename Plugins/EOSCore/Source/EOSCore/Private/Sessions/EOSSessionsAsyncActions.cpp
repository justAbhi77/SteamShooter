/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "Sessions/EOSSessionsAsyncActions.h"
#include "Sessions/EOSSessions.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLibrary.h"
#include "Core/EOSCoreLogging.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreSessionsUpdateSession
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreSessionsUpdateSession* UEOSCoreSessionsUpdateSession::EOSSessionsUpdateSessionAsync(UObject* WorldContextObject, FEOSSessionsUpdateSessionOptions Options)
{
	if (UEOSCoreSubsystem::GetSessionsHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreSessionsUpdateSession>();
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

void UEOSCoreSessionsUpdateSession::Activate()
{
	Super::Activate();
	UCoreSessions::GetSessions(m_WorldContextObject)->EOSSessionsUpdateSession(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreSessionsDestroySession
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreSessionsDestroySession* UEOSCoreSessionsDestroySession::EOSSessionsDestroySessionAsync(UObject* WorldContextObject, FEOSSessionsDestroySessionOptions Options)
{
	if (UEOSCoreSubsystem::GetSessionsHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreSessionsDestroySession>();
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

void UEOSCoreSessionsDestroySession::Activate()
{
	Super::Activate();
	UCoreSessions::GetSessions(m_WorldContextObject)->EOSSessionsDestroySession(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreSessionsJoinSession
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreSessionsJoinSession* UEOSCoreSessionsJoinSession::EOSSessionsJoinSessionAsync(UObject* WorldContextObject, FEOSSessionsJoinSessionOptions Options)
{
	if (UEOSCoreSubsystem::GetSessionsHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreSessionsJoinSession>();
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

void UEOSCoreSessionsJoinSession::Activate()
{
	Super::Activate();
	UCoreSessions::GetSessions(m_WorldContextObject)->EOSSessionsJoinSession(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreSessionsStartSession
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreSessionsStartSession* UEOSCoreSessionsStartSession::EOSSessionsStartSessionAsync(UObject* WorldContextObject, FEOSSessionsStartSessionOptions Options)
{
	if (UEOSCoreSubsystem::GetSessionsHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreSessionsStartSession>();
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

void UEOSCoreSessionsStartSession::Activate()
{
	Super::Activate();
	UCoreSessions::GetSessions(m_WorldContextObject)->EOSSessionsStartSession(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreSessionsEndSession
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreSessionsEndSession* UEOSCoreSessionsEndSession::EOSSessionsEndSessionAsync(UObject* WorldContextObject, FEOSSessionsEndSessionOptions Options)
{
	if (UEOSCoreSubsystem::GetSessionsHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreSessionsEndSession>();
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

void UEOSCoreSessionsEndSession::Activate()
{
	Super::Activate();
	UCoreSessions::GetSessions(m_WorldContextObject)->EOSSessionsEndSession(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreSessionsRegisterPlayers
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreSessionsRegisterPlayers* UEOSCoreSessionsRegisterPlayers::EOSSessionsRegisterPlayersAsync(UObject* WorldContextObject, FEOSSessionsRegisterPlayersOptions Options)
{
	if (UEOSCoreSubsystem::GetSessionsHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreSessionsRegisterPlayers>();
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

void UEOSCoreSessionsRegisterPlayers::Activate()
{
	Super::Activate();
	UCoreSessions::GetSessions(m_WorldContextObject)->EOSSessionsRegisterPlayers(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreSessionsUnregisterPlayers
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreSessionsUnregisterPlayers* UEOSCoreSessionsUnregisterPlayers::EOSSessionsUnregisterPlayersAsync(UObject* WorldContextObject, FEOSSessionsUnregisterPlayersOptions Options)
{
	if (UEOSCoreSubsystem::GetSessionsHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreSessionsUnregisterPlayers>();
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

void UEOSCoreSessionsUnregisterPlayers::Activate()
{
	Super::Activate();
	UCoreSessions::GetSessions(m_WorldContextObject)->EOSSessionsUnregisterPlayers(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreSessionsSendInvite
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreSessionsSendInvite* UEOSCoreSessionsSendInvite::EOSSessionsSendInviteAsync(UObject* WorldContextObject, FEOSSessionsSendInviteOptions Options)
{
	if (UEOSCoreSubsystem::GetSessionsHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreSessionsSendInvite>();
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

void UEOSCoreSessionsSendInvite::Activate()
{
	Super::Activate();
	UCoreSessions::GetSessions(m_WorldContextObject)->EOSSessionsSendInvite(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreSessionsRejectInvite
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreSessionsRejectInvite* UEOSCoreSessionsRejectInvite::EOSSessionsRejectInviteAsync(UObject* WorldContextObject, FEOSSessionsRejectInviteOptions Options)
{
	if (UEOSCoreSubsystem::GetSessionsHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreSessionsRejectInvite>();
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

void UEOSCoreSessionsRejectInvite::Activate()
{
	Super::Activate();
	UCoreSessions::GetSessions(m_WorldContextObject)->EOSSessionsRejectInvite(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreSessionsQueryInvites
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreSessionsQueryInvites* UEOSCoreSessionsQueryInvites::EOSSessionsQueryInvitesAsync(UObject* WorldContextObject, FEOSSessionsQueryInvitesOptions Options)
{
	if (UEOSCoreSubsystem::GetSessionsHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreSessionsQueryInvites>();
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

void UEOSCoreSessionsQueryInvites::Activate()
{
	Super::Activate();
	UCoreSessions::GetSessions(m_WorldContextObject)->EOSSessionsQueryInvites(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreSessionsSearchFind
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreSessionsSearchFind* UEOSCoreSessionsSearchFind::EOSSessionSearchFindAsync(UObject* WorldContextObject, FEOSHSessionSearch SearchHandle, FEOSSessionSearchFindOptions Options)
{
	if (UEOSCoreSubsystem::GetSessionsHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreSessionsSearchFind>();
		AsyncObject->RegisterWithGameInstance(GameInstance);
		AsyncObject->m_Options = Options;
		AsyncObject->m_WorldContextObject = WorldContextObject;
		AsyncObject->m_SearchHandle = SearchHandle;

		return AsyncObject;
	}
	else
	{
		LogError("Plugin not initialized!");
	}

	return nullptr;
}

void UEOSCoreSessionsSearchFind::Activate()
{
	Super::Activate();
	UCoreSessions::GetSessions(m_WorldContextObject)->EOSSessionSearchFind(m_WorldContextObject, m_SearchHandle, m_Options, m_Callback);
}
