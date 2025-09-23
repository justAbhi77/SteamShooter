/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "Shared/EOSWebTypes.h"
#include "EOSWebConnect.generated.h"

UCLASS()
class UEOSWebConnectLibrary : public UObject
{
	GENERATED_BODY()
public:
	/*
	* Request an AccessToken to authenticate with
	* The AccessToken can be used to create a Room token or other API features that requires authentication
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCoreWeb|Connect")
	static void RequestAccessToken(FRequestAccessTokenRequest Request, const FAccessTokenCallbackDelegate& Callback);
};
