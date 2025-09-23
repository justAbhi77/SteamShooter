/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include <Misc/EngineVersionComparison.h>
#if UE_VERSION_NEWER_THAN(4,27,2)
#include <Online/CoreOnline.h>
#else
#include <UObject/CoreOnline.h>
#endif
#include "OnlineSubsystem.h"
#include "VoiceInterfaceImpl.h"
#include "Interfaces/VoiceInterface.h"

class FOnlineSubsystemEOSCore;

class ONLINESUBSYSTEMEOSCORE_API FOnlineVoiceImplEOSCore : public FOnlineVoiceImpl
{
public:
	FOnlineVoiceImplEOSCore()
		: FOnlineVoiceImpl()
	{
		
	};
	
	FOnlineVoiceImplEOSCore(IOnlineSubsystem* InOnlineSubsystem)
		: FOnlineVoiceImpl(InOnlineSubsystem)
	{
		
	};
	
	virtual ~FOnlineVoiceImplEOSCore() override
	{
		
	};
};

typedef TSharedPtr<FOnlineVoiceImpl, ESPMode::ThreadSafe> FOnlineVoiceImplPtr;
