/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "EOSCoreModule.h"
#include "Core/EOSCoreLibrary.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include <OnlineSubsystem.h>

#define LOCTEXT_NAMESPACE "FEOSCoreModule"

DEFINE_LOG_CATEGORY(LogEOSCore);

IMPLEMENT_MODULE(FEOSCoreModule, EOSCore);

void FEOSCoreModule::StartupModule()
{
	auto& PluginManager = IPluginManager::Get();
	TSharedPtr<IPlugin> PluginPtr = PluginManager.FindPlugin("EOSCore");

	FString PluginVersion;

	if (PluginPtr)
	{
		PluginVersion = PluginPtr->GetDescriptor().VersionName;
	}

	UE_LOG(LogTemp, Log, TEXT("--------------------------------------------------------------------------------"));
	UE_LOG(LogTemp, Log, TEXT("Using EOSCore Version: %s"), *PluginVersion);
	UE_LOG(LogTemp, Log, TEXT("--------------------------------------------------------------------------------"));

}

void FEOSCoreModule::ShutdownModule()
{

}

FOnlineSubsystemEOSCore* UEOSCoreSubsystem::GetSubsystem(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (FOnlineSubsystemEOSCore* Subsystem = static_cast<FOnlineSubsystemEOSCore*>(Online::GetSubsystem(GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull), EOSCORE_SUBSYSTEM)))
		{
			return Subsystem;
		}
	}

	if (FOnlineSubsystemEOSCore* Subsystem = static_cast<FOnlineSubsystemEOSCore*>(IOnlineSubsystem::Get(EOSCORE_SUBSYSTEM)))
	{
		return Subsystem;
	}

	return nullptr;
}

EOS_HPlatform UEOSCoreSubsystem::GetPlatform(UObject* WorldContextObject)
{
	if (FOnlineSubsystemEOSCore* Subsystem = GetSubsystem(WorldContextObject))
	{
		return Subsystem->m_EOSPlatformHandle ? Subsystem->m_EOSPlatformHandle : nullptr;
	}
	return nullptr;
}

EOS_HAuth UEOSCoreSubsystem::GetAuthHandle(UObject* WorldContextObject)
{
	if (FOnlineSubsystemEOSCore* Subsystem = GetSubsystem(WorldContextObject))
	{
		return Subsystem->m_AuthHandle ? Subsystem->m_AuthHandle : nullptr;
	}
	return nullptr;
}

EOS_HFriends UEOSCoreSubsystem::GetFriendsHandle(UObject* WorldContextObject)
{
	if (FOnlineSubsystemEOSCore* Subsystem = GetSubsystem(WorldContextObject))
	{
		return Subsystem->m_FriendsHandle ? Subsystem->m_FriendsHandle : nullptr;
	}
	return nullptr;
}

EOS_HUserInfo UEOSCoreSubsystem::GetUserHandle(UObject* WorldContextObject)
{
	if (FOnlineSubsystemEOSCore* Subsystem = GetSubsystem(WorldContextObject))
	{
		return Subsystem->m_UserInfoHandle ? Subsystem->m_UserInfoHandle : nullptr;
	}
	return nullptr;
}

EOS_HPresence UEOSCoreSubsystem::GetPresenceHandle(UObject* WorldContextObject)
{
	if (FOnlineSubsystemEOSCore* Subsystem = GetSubsystem(WorldContextObject))
	{
		return Subsystem->m_PresenceHandle ? Subsystem->m_PresenceHandle : nullptr;
	}
	return nullptr;
}

EOS_HConnect UEOSCoreSubsystem::GetConnectHandle(UObject* WorldContextObject)
{
	if (FOnlineSubsystemEOSCore* Subsystem = GetSubsystem(WorldContextObject))
	{
		return Subsystem->m_ConnectHandle ? Subsystem->m_ConnectHandle : nullptr;
	}
	return nullptr;
}

EOS_HSessions UEOSCoreSubsystem::GetSessionsHandle(UObject* WorldContextObject)
{
	if (FOnlineSubsystemEOSCore* Subsystem = GetSubsystem(WorldContextObject))
	{
		return Subsystem->m_SessionsHandle ? Subsystem->m_SessionsHandle : nullptr;
	}
	return nullptr;
}

EOS_HStats UEOSCoreSubsystem::GetStatsHandle(UObject* WorldContextObject)
{
	if (FOnlineSubsystemEOSCore* Subsystem = GetSubsystem(WorldContextObject))
	{
		return Subsystem->m_StatsHandle ? Subsystem->m_StatsHandle : nullptr;
	}
	return nullptr;
}

EOS_HLeaderboards UEOSCoreSubsystem::GetLeaderboardsHandle(UObject* WorldContextObject)
{
	if (FOnlineSubsystemEOSCore* Subsystem = GetSubsystem(WorldContextObject))
	{
		return Subsystem->m_LeaderboardsHandle ? Subsystem->m_LeaderboardsHandle : nullptr;
	}
	return nullptr;
}

EOS_HMetrics UEOSCoreSubsystem::GetMetricsHandle(UObject* WorldContextObject)
{
	if (FOnlineSubsystemEOSCore* Subsystem = GetSubsystem(WorldContextObject))
	{
		return Subsystem->m_MetricsHandle ? Subsystem->m_MetricsHandle : nullptr;
	}
	return nullptr;
}

EOS_HAchievements UEOSCoreSubsystem::GetAchievementsHandle(UObject* WorldContextObject)
{
	if (FOnlineSubsystemEOSCore* Subsystem = GetSubsystem(WorldContextObject))
	{
		return Subsystem->m_AchievementsHandle ? Subsystem->m_AchievementsHandle : nullptr;
	}
	return nullptr;
}

EOS_HP2P UEOSCoreSubsystem::GetP2PHandle(UObject* WorldContextObject)
{
	if (FOnlineSubsystemEOSCore* Subsystem = GetSubsystem(WorldContextObject))
	{
		return Subsystem->m_P2PHandle ? Subsystem->m_P2PHandle : nullptr;
	}
	return nullptr;
}

EOS_HEcom UEOSCoreSubsystem::GetEcomHandle(UObject* WorldContextObject)
{
	if (FOnlineSubsystemEOSCore* Subsystem = GetSubsystem(WorldContextObject))
	{
		return Subsystem->m_EcomHandle ? Subsystem->m_EcomHandle : nullptr;
	}
	return nullptr;
}

EOS_HLobby UEOSCoreSubsystem::GetLobbyHandle(UObject* WorldContextObject)
{
	if (FOnlineSubsystemEOSCore* Subsystem = GetSubsystem(WorldContextObject))
	{
		return Subsystem->m_LobbyHandle ? Subsystem->m_LobbyHandle : nullptr;
	}
	return nullptr;
}

EOS_HPlayerDataStorage UEOSCoreSubsystem::GetPlayerDataStorageHandle(UObject* WorldContextObject)
{
	if (FOnlineSubsystemEOSCore* Subsystem = GetSubsystem(WorldContextObject))
	{
		return Subsystem->m_PlayerDataStorageHandle ? Subsystem->m_PlayerDataStorageHandle : nullptr;
	}
	return nullptr;
}

EOS_HTitleStorage UEOSCoreSubsystem::GetTitleStorageHandle(UObject* WorldContextObject)
{
	if (FOnlineSubsystemEOSCore* Subsystem = GetSubsystem(WorldContextObject))
	{
		return Subsystem->m_TitleStorageHandle ? Subsystem->m_TitleStorageHandle : nullptr;
	}
	return nullptr;
}

EOS_HUI UEOSCoreSubsystem::GetUiHandle(UObject* WorldContextObject)
{
	if (FOnlineSubsystemEOSCore* Subsystem = GetSubsystem(WorldContextObject))
	{
		return Subsystem->m_UiHandle ? Subsystem->m_UiHandle : nullptr;
	}
	return nullptr;
}

EOS_HUserInfo UEOSCoreSubsystem::GetUserInfoHandle(UObject* WorldContextObject)
{
	if (FOnlineSubsystemEOSCore* Subsystem = GetSubsystem(WorldContextObject))
	{
		return Subsystem->m_UserInfoHandle ? Subsystem->m_UserInfoHandle : nullptr;
	}
	return nullptr;
}

EOS_HMods UEOSCoreSubsystem::GetModsHandle(UObject* WorldContextObject)
{
	if (FOnlineSubsystemEOSCore* Subsystem = GetSubsystem(WorldContextObject))
	{
		return Subsystem->m_ModsHandle ? Subsystem->m_ModsHandle : nullptr;
	}
	return nullptr;
}

EOS_HReports UEOSCoreSubsystem::GetReportsHandle(UObject* WorldContextObject)
{
	if (FOnlineSubsystemEOSCore* Subsystem = GetSubsystem(WorldContextObject))
	{
		return Subsystem->m_ReportsHandle ? Subsystem->m_ReportsHandle : nullptr;
	}
	return nullptr;
}

EOS_HSanctions UEOSCoreSubsystem::GetSanctionssHandle(UObject* WorldContextObject)
{
	if (FOnlineSubsystemEOSCore* Subsystem = GetSubsystem(WorldContextObject))
	{
		return Subsystem->m_SanctionsHandle ? Subsystem->m_SanctionsHandle : nullptr;
	}
	return nullptr;
}

EOS_HAntiCheatClient UEOSCoreSubsystem::GetAntiCheatClientHandle(UObject* WorldContextObject)
{
	if (FOnlineSubsystemEOSCore* Subsystem = GetSubsystem(WorldContextObject))
	{
		return Subsystem->m_AntiCheatClientHandle ? Subsystem->m_AntiCheatClientHandle : nullptr;
	}
	return nullptr;
}

EOS_HAntiCheatServer UEOSCoreSubsystem::GetAntiCheatServerHandle(UObject* WorldContextObject)
{
	if (FOnlineSubsystemEOSCore* Subsystem = GetSubsystem(WorldContextObject))
	{
		return Subsystem->m_AntiCheatServerHandle ? Subsystem->m_AntiCheatServerHandle : nullptr;
	}
	return nullptr;
}

EOS_HRTC UEOSCoreSubsystem::GetRTCHandle(UObject* WorldContextObject)
{
	if (FOnlineSubsystemEOSCore* Subsystem = GetSubsystem(WorldContextObject))
	{
		return Subsystem->m_HRTCHandle ? Subsystem->m_HRTCHandle : nullptr;
	}
	return nullptr;
}

EOS_HRTCAdmin UEOSCoreSubsystem::GetRTCAdminHandle(UObject* WorldContextObject)
{
	if (FOnlineSubsystemEOSCore* Subsystem = GetSubsystem(WorldContextObject))
	{
		return Subsystem->m_HRTCAdminHandle ? Subsystem->m_HRTCAdminHandle : nullptr;
	}
	return nullptr;
}

EOS_HProgressionSnapshot UEOSCoreSubsystem::GetProgressionSnapshotHandle(UObject* WorldContextObject)
{
	if (FOnlineSubsystemEOSCore* Subsystem = GetSubsystem(WorldContextObject))
	{
		return Subsystem->m_ProgressionSnapshotHandle ? Subsystem->m_ProgressionSnapshotHandle : nullptr;
	}
	return nullptr;
}

EOS_HCustomInvites UEOSCoreSubsystem::GetCustomInvitesHandle(UObject* WorldContextObject)
{
	if (FOnlineSubsystemEOSCore* Subsystem = GetSubsystem(WorldContextObject))
	{
		return Subsystem->m_CustomInvitesHandle ? Subsystem->m_CustomInvitesHandle : nullptr;
	}
	return nullptr;
}

#undef LOCTEXT_NAMESPACE
