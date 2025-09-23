/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "EOSWebShared.h"
#include "EOSCoreWebModule.h"
#include "EOSCoreWebLogging.h"
#include "EOSCoreWebPrivatePCH.h"
#include "Shared/EOSWebTypes.h"

void UEOSWebShared::GetPublicIp(const FOnPublicIpResponse& Callback)
{
	LogVeryVerbose("");

	auto HttpRequest = UEOSWebShared::CreateRequest();

	HttpRequest->SetVerb("GET");
	HttpRequest->SetURL(FString("http://api.ipify.org"));
	HttpRequest->SetHeader("User-Agent", "X-EOSCore/1.0");
	HttpRequest->SetHeader("Content-Type", "text/html");

	HttpRequest->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr HttpRequestPtr, FHttpResponsePtr HttpResponsePtr, bool bConnectedSuccessfully)
	{
		FString Ip;

		if (HttpResponsePtr)
		{
			Ip = HttpResponsePtr->GetContentAsString();
		}

		Callback.ExecuteIfBound(Ip);
	});

	HttpRequest->ProcessRequest();
}
