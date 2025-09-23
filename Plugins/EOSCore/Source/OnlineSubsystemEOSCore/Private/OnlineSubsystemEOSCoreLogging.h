#pragma once

#if PLATFORM_ANDROID
#include "Android/AndroidPlatformMisc.h"
#define LogDebug(format, ...) FPlatformMisc::LowLevelOutputDebugStringf(TEXT("LOG: [%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)
#define LogVerbose(format, ...) FPlatformMisc::LowLevelOutputDebugStringf(TEXT("VERBOSE: [%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)
#define LogVeryVerbose(format, ...) FPlatformMisc::LowLevelOutputDebugStringf(TEXT("VERY-VERBOSE: [%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)
#define LogError(format, ...) FPlatformMisc::LowLevelOutputDebugStringf(TEXT("ERROR: [%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)
#define LogWarning(format, ...) FPlatformMisc::LowLevelOutputDebugStringf(TEXT("WARNING: [%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)
#else
#define LogDebug(format, ...) UE_LOG(LogEOSCoreSubsystem, Log, TEXT("[%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)
#define LogVerbose(format, ...) UE_LOG(LogEOSCoreSubsystem, Verbose, TEXT("[%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)
#define LogVeryVerbose(format, ...) UE_LOG(LogEOSCoreSubsystem, VeryVerbose, TEXT("[%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)
#define LogError(format, ...) UE_LOG(LogEOSCoreSubsystem, Error, TEXT("[%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)
#define LogWarning(format, ...) UE_LOG(LogEOSCoreSubsystem, Warning, TEXT("[%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)
#endif
