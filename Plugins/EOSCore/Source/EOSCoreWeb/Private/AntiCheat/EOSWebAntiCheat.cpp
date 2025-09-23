/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "EOSWebAntiCheat.h"
#include "EOSCoreWebModule.h"
#include "EOSCoreWebLogging.h"
#include "EOSCoreWebPrivatePCH.h"
#include "Shared/EOSWebShared.h"

void UEOSWebAntiCheatLibrary::QueryAntiCheatServiceStatusByDeployment(FString AccessToken, FString DeploymentId, const FAntiCheatStatusCallbackDelegate& Callback)
{
	LogVerbose("");

	if (AccessToken.IsEmpty())
	{
		LogError("AccessToken cannot be empty");
		return;
	}

	if (DeploymentId.IsEmpty())
	{
		LogError("DeploymentId cannot be empty");
		return;
	}

	auto HttpRequest = UEOSWebShared::CreateRequest();

	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->AppendToHeader(TEXT("Accept"), TEXT("application/json"));
	HttpRequest->AppendToHeader(TEXT("Authorization"), *FString::Printf(TEXT("Bearer %s"), *AccessToken));
	HttpRequest->SetURL(FString::Printf(TEXT("https://api.epicgames.dev/anticheat/v1/%s/status"), *DeploymentId));
	HttpRequest->SetVerb("GET");

	HttpRequest->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr HttpRequestPtr, FHttpResponsePtr HttpResponsePtr, bool bConnectedSuccessfully)
	{
		bool bServerKick = false;
		
		if (bConnectedSuccessfully)
		{
			LogVerbose("QueryAntiCheatServiceStatusByDeployment Response: %s", *HttpResponsePtr->GetContentAsString());

			if (HttpResponsePtr->GetResponseCode() == 200)
			{
				TSharedPtr<FJsonObject> JsonObject;
				const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(HttpResponsePtr->GetContentAsString());

				if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
				{
					const TSharedPtr<FJsonValue> ServerKickObject = JsonObject->TryGetField(TEXT("serverKick"));

					if (!ServerKickObject)
					{
						LogError("serverKick was not found");
					}

					if (ServerKickObject)
					{
						bServerKick = ServerKickObject->AsBool();
					}
				}
			}
			else
			{
				LogError("Error Code: %d", HttpResponsePtr->GetResponseCode());
			}
		}
		else
		{
			LogError("Connection failed");
		}

		Callback.ExecuteIfBound(HttpResponsePtr->GetResponseCode() == 200, FQueryAntiCheatServiceStatusByDeploymentCallbackData(bServerKick), FWebResponse(HttpResponsePtr->GetResponseCode(), HttpResponsePtr->GetContentAsString()));
	});

	LogVerbose("-------------------------------------------------\n");
	LogVerbose("Sending QueryAntiCheatServiceStatusByDeployment Request (%s)", *HttpRequest->GetURL());
	for (auto& Element : HttpRequest->GetAllHeaders())
	{
		LogVerbose("Header: %s", *Element);
	}
	LogVerbose("-------------------------------------------------\n");

	HttpRequest->ProcessRequest();
}
