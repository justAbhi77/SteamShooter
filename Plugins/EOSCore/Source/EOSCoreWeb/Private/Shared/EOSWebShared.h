/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "EOSWebTypes.h"
#include "HttpModule.h"
#include "Runtime/Launch/Resources/Version.h"
#include "EOSWebShared.generated.h"

UCLASS()
class UEOSWebShared : public UObject
{
	GENERATED_BODY()
public:
#if ENGINE_MINOR_VERSION > 25 || ENGINE_MAJOR_VERSION == 5
	static TSharedRef<IHttpRequest, ESPMode::ThreadSafe> CreateRequest()
	{
		return FHttpModule::Get().CreateRequest();
	}
#else
	static TSharedRef<IHttpRequest> CreateRequest()
	{
		return FHttpModule::Get().CreateRequest();
	}
#endif

	UFUNCTION(BlueprintCallable, Category = "EOSCoreWeb")
	static void GetPublicIp(const FOnPublicIpResponse& Callback);
};
