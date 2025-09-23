/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include <Misc/EngineVersionComparison.h>
#if UE_VERSION_NEWER_THAN(4,27,2)
#include <Online/CoreOnline.h>
#else
#include <UObject/CoreOnline.h>
#endif
#include <Features/IModularFeature.h>
#include <Features/IModularFeatures.h>
#include <SocketSubsystem.h>
#include <SocketTypes.h>
#include <SocketSubsystemModule.h>
#include <IPAddress.h>
#include <Modules/ModuleManager.h>
#include <Containers/Ticker.h>
#include <OnlineSessionSettings.h>
#include <OnlineError.h>
#include <OnlineSubsystem.h>
#include <OnlineSubsystemNames.h>
#include <OnlineSubsystemUtils.h>
#include <OnlineAsyncTaskManager.h>
#include <Misc/CoreDelegates.h>
#include <Misc/MessageDialog.h>
#include <Misc/Paths.h>
#include <Misc/EngineVersionComparison.h>
#include <Misc/ConfigCacheIni.h>
#include <Misc/Guid.h>
#include <Misc/NetworkVersion.h>
#include <Misc/App.h>
#include <Misc/CommandLine.h>
#include <Misc/OutputDeviceRedirector.h>
#include <Interfaces/OnlineSessionInterface.h>
#include <Interfaces/OnlineIdentityInterface.h>
#include <Interfaces/IPluginManager.h>
#include <Runtime/Launch/Resources/Version.h>
#include <GameFramework/PlayerController.h>
#include <eos_platform_prereqs.h>
#include <IPAddress.h>
#include <OnlineSubsystemTypes.h>
#include <VoipListenerSynthComponent.h>
#if UE_VERSION_NEWER_THAN(5,1,0)
#include <Online/OnlineBase.h>
#include <Online/OnlineSessionNames.h>
#include <Net/VoiceConfig.h>
#include <UObject/UObjectIterator.h>
#endif

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

#if WITH_EDITOR
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#endif

#include "eos_achievements.h"
#include "eos_leaderboards.h"
#include "eos_userinfo.h"
#include "eos_sessions.h"
#include "eos_metrics.h"
#include "eos_stats.h"
#include "eos_ecom.h"
#include "eos_logging.h"
#include "eos_p2p.h"
#include "eos_auth.h"
#include "eos_userinfo.h"
#include "eos_friends.h"
#include "eos_presence.h"
#include "eos_mods.h"
#include "eos_anticheatclient.h"
#include "eos_anticheatserver.h"


#include "OnlineSubsystemEOSCoreLogging.h"