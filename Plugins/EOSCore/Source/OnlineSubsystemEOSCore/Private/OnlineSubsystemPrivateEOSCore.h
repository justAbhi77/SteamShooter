/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemEOSCorePrivatePCH.h"

#define INVALID_INDEX -1

/** URL Prefix when using EOS socket connection */
#define EOS_URL_PREFIX TEXT("EOS.")

/** pre-pended to all NULL logging */
#undef ONLINE_LOG_PREFIX
#define ONLINE_LOG_PREFIX TEXT("EOS: ")
