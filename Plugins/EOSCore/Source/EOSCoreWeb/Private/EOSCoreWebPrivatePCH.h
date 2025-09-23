/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include <Misc/Base64.h>
#include <Dom/JsonObject.h>
#include <Interfaces/IHttpResponse.h>
#include <Serialization/JsonSerializerMacros.h>
#include <Interfaces/IHttpRequest.h>
#include <Interfaces/IPluginManager.h>
#include <Containers/Ticker.h>

#include <Misc/EngineVersionComparison.h>

#if PLATFORM_WINDOWS
#include "Windows/WindowsPlatformApplicationMisc.h"
#elif PLATFORM_ANDROID
#include "Android/AndroidPlatformApplicationMisc.h"
#elif PLATFORM_IOS
#include "IOS/IOSPlatformApplicationMisc.h"
#elif PLATFORM_LINUX
#include "Linux/LinuxPlatformApplicationMisc.h"
#elif PLATFORM_MAC
#include "Mac/MacPlatformApplicationMisc.h"
#endif