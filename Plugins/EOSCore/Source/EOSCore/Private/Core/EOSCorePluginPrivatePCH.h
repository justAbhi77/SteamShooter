/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include <CoreMinimal.h>
#include <Engine/World.h>
#include <HAL/RunnableThread.h>
#include <HAL/FileManager.h>
#include <GenericPlatform/GenericPlatformFile.h>
#include <Containers/Ticker.h>
#include <Interfaces/IPluginManager.h>
#include <Misc/MessageDialog.h>
#include <Misc/ConfigCacheIni.h>
#include <Containers/Queue.h>
#include <Misc/CommandLine.h>
#include <Modules/ModuleManager.h>
#include <Interfaces/OnlineIdentityInterface.h>
#include <OnlineSubsystemUtils.h>
#include <GameFramework/PlayerController.h>
#include <Runtime/Launch/Resources/Version.h>
#include <string>
#include <algorithm>
#include <Misc/EngineVersionComparison.h>
#include <Engine/GameInstance.h>
#include <GameFramework/PlayerState.h>
#include <OnlineSubsystemTypes.h>
#if UE_VERSION_NEWER_THAN(4,27,2)
#include <HAL/PlatformFileManager.h>
#else
#include <HAL/PlatformFilemanager.h>
#endif


#include <eos_platform_prereqs.h>

#if PLATFORM_WINDOWS
#include <Windows/eos_Windows_base.h>
#elif PLATFORM_ANDROID
#include <Android/eos_Android_base.h>
#elif PLATFORM_LINUX
#include <Linux/eos_Linux_base.h>
#elif PLATFORM_MAC
#include <Mac/eos_Mac_base.h>
#elif PLATFORM_SWITCH
#include <Switch/eos_Switch_base.h>
#elif PLATFORM_IOS
#include <eos_IOS_base.h>
#include <eos_ios.h>
#endif

#include <eos_sdk.h>
#include <eos_auth.h>
#include <eos_userinfo.h>
#include <eos_stats.h>
#include <eos_connect.h>
#include <eos_lobby.h>
#include <eos_ui.h>
#include <eos_friends.h>
#include <eos_achievements.h>
#include <eos_ecom.h>
#include <eos_leaderboards.h>
#include <eos_sessions.h>
#include <eos_playerdatastorage.h>
#include <eos_presence.h>
#include <eos_p2p.h>
#include <eos_metrics.h>
#include <eos_ecom.h>
#include <eos_logging.h>
#include <eos_mods.h>
#include <eos_reports.h>
#include <eos_sanctions.h>
#include <eos_anticheatclient.h>
#include <eos_anticheatserver.h>
#include <eos_rtc.h>
#include <eos_rtc_audio.h>
#include <eos_rtc_admin.h>
#include <eos_progressionsnapshot.h>
#include <eos_custominvites.h>

#include "OnlineSubsystemEOSCore.h"