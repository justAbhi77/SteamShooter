/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "EOSWebVoice.h"
#include "EOSCoreWebModule.h"
#include "EOSCoreWebLogging.h"
#include "EOSCoreWebPrivatePCH.h"
#include "Shared/EOSWebShared.h"

struct FWebRequestData : public FJsonSerializable
{
	struct FRequestUser : public FJsonSerializable
	{
		explicit FRequestUser() = default;
		FString ProductUserId;
		FString ClientIp;
		bool bHardMuted;

		BEGIN_JSON_SERIALIZER
			JSON_SERIALIZE("puid", ProductUserId);
		JSON_SERIALIZE("clientIp", ClientIp);
		JSON_SERIALIZE("hardMuted", bHardMuted);
		END_JSON_SERIALIZER
};

	FWebRequestData()
	{
	}

	FWebRequestData(TArray<FWebRequestParticipantData> Users)
	{
		for (auto& Element : Users)
		{
			FRequestUser User;
			User.ProductUserId = Element.ProductUserId;
			User.ClientIp = Element.ClientIp;
			User.bHardMuted = Element.bHardMuted;

			UsersArray.Add(User);
		}
	}

	TArray<FRequestUser> UsersArray;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE_ARRAY_SERIALIZABLE("participants", UsersArray, FRequestUser);
	END_JSON_SERIALIZER
};

void UEOSWebVoiceLibrary::CreateRoomToken(FString AccessToken, TArray<FWebRequestParticipantData> Participants, FString DeploymentId, FString RoomName, const FCreateRoomTokenCallbackDelegate& Callback)
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

	if (Participants.Num() == 0)
	{
		LogError("Participants cannot be empty");
		return;
	}

	if (RoomName.IsEmpty())
	{
		LogError("RoomName cannot be empty");
		return;
	}

	auto HttpRequest = UEOSWebShared::CreateRequest();

	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->AppendToHeader(TEXT("Accept"), TEXT("application/json"));
	HttpRequest->AppendToHeader(TEXT("Authorization"), *FString::Printf(TEXT("Bearer %s"), *AccessToken));

	FWebRequestData RequestData(Participants);
	const FString ContentString = *RequestData.ToJson(false);

	HttpRequest->SetContentAsString(ContentString);
	HttpRequest->SetURL(FString::Printf(TEXT("https://api.epicgames.dev/rtc/v1/%s/room/%s"), *DeploymentId, *RoomName));
	HttpRequest->SetVerb("POST");
	HttpRequest->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr HttpRequestPtr, FHttpResponsePtr HttpResponsePtr, bool bConnectedSuccessfully)
	{
		FString RoomIdString;
		FString DeploymentIdString;
		FString ClientBaseUrlString;
		FString PuidString;
		FString TokenString;

		if (bConnectedSuccessfully)
		{
			LogVeryVerbose("CreateRoomToken Response: %s", *HttpResponsePtr->GetContentAsString());

			if (HttpResponsePtr->GetResponseCode() == 200)
			{
				TSharedPtr<FJsonObject> JsonObject;
				const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(HttpResponsePtr->GetContentAsString());

				if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
				{
					const TSharedPtr<FJsonValue> RoomIdObject = JsonObject->TryGetField(TEXT("roomId"));
					const TSharedPtr<FJsonValue> DeploymentIdObject = JsonObject->TryGetField(TEXT("deploymentId"));
					const TSharedPtr<FJsonValue> ClientBaseUrlObject = JsonObject->TryGetField(TEXT("clientBaseUrl"));
					const TSharedPtr<FJsonValue> ParticipantsObject = JsonObject->TryGetField(TEXT("participants"));

					if (!RoomIdObject)
					{
						LogError("No RoomId was found");
					}

					if (!DeploymentIdObject)
					{
						LogError("No DeploymentId was found");
					}

					if (!ClientBaseUrlObject)
					{
						LogError("No ClientBaseUrl was found");
					}

					if (!ParticipantsObject)
					{
						LogError("No Participants was found");
					}

					if (RoomIdObject && DeploymentIdObject && ClientBaseUrlObject && ParticipantsObject)
					{
						RoomIdString = RoomIdObject->AsString();
						DeploymentIdString = DeploymentIdObject->AsString();
						ClientBaseUrlString = ClientBaseUrlObject->AsString();

						if (ParticipantsObject && ParticipantsObject->Type == EJson::Array)
						{
							TArray<TSharedPtr<FJsonValue>> ParticipantsArray = ParticipantsObject->AsArray();

							for (const auto& Element : ParticipantsArray)
							{
								if (Element->Type != EJson::Object)
									continue;

								auto& Object = Element->AsObject();

								TokenString = Object->TryGetField("token")->AsString();
								PuidString = Object->TryGetField("puid")->AsString();
							}
						}
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

		if (!RoomIdString.IsEmpty() && !DeploymentIdString.IsEmpty() && !ClientBaseUrlString.IsEmpty() && !TokenString.IsEmpty() && !PuidString.IsEmpty())
		{
			Callback.ExecuteIfBound(true, FCreateRoomTokenCallback(RoomIdString, DeploymentIdString, ClientBaseUrlString, TokenString, PuidString), FWebResponse(HttpResponsePtr->GetResponseCode(), HttpResponsePtr->GetContentAsString()));
		}
		else
		{
			Callback.ExecuteIfBound(false, FCreateRoomTokenCallback(RoomIdString, DeploymentIdString, ClientBaseUrlString, TokenString, PuidString), FWebResponse(HttpResponsePtr->GetResponseCode(), HttpResponsePtr->GetContentAsString()));
		}
	});

	LogVerbose("-------------------------------------------------\n");
	LogVerbose("Sending CreateRoom Request (%s)", *HttpRequest->GetURL());
	for (auto& Element : HttpRequest->GetAllHeaders())
	{
		LogVerbose("Header: %s", *Element);
	}
	LogVerbose("Content: %s", *ContentString);
	LogVerbose("-------------------------------------------------\n");

	HttpRequest->ProcessRequest();
}

void UEOSWebVoiceLibrary::RemoveParticipant(FString AccessToken, FString ProductUserId, FString DeploymentId, FString RoomName, const FRemoveParticipantCallbackDelegate& Callback)
{
	LogVerbose("");

	if (AccessToken.IsEmpty())
	{
		LogError("AccessToken cannot be empty");
		return;
	}

	if (ProductUserId.IsEmpty())
	{
		LogError("ProductUserId cannot be empty");
		return;
	}

	if (DeploymentId.IsEmpty())
	{
		LogError("DeploymentId cannot be empty");
		return;
	}

	if (RoomName.IsEmpty())
	{
		LogError("RoomName cannot be empty");
		return;
	}

	auto HttpRequest = UEOSWebShared::CreateRequest();

	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->AppendToHeader(TEXT("Accept"), TEXT("application/json"));
	HttpRequest->AppendToHeader(TEXT("Authorization"), *FString::Printf(TEXT("Bearer %s"), *AccessToken));
	HttpRequest->SetURL(FString::Printf(TEXT("https://api.epicgames.dev/rtc/v1/%s/room/%s/participants/%s"), *DeploymentId, *RoomName, *ProductUserId));
	HttpRequest->SetVerb("DELETE");
	HttpRequest->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr HttpRequestPtr, FHttpResponsePtr HttpResponsePtr, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
		{
			LogVeryVerbose("RemoveParticipant Response: (%d) (%s)", HttpResponsePtr->GetResponseCode(), *HttpResponsePtr->GetContentAsString());
		}
		else
		{
			LogError("Connection failed");
		}

		Callback.ExecuteIfBound(HttpResponsePtr->GetResponseCode() == 204, FWebResponse(HttpResponsePtr->GetResponseCode(), HttpResponsePtr->GetContentAsString()));
	});

	LogVerbose("-------------------------------------------------\n");
	LogVerbose("Sending RemoveParticipant Request (%s)", *HttpRequest->GetURL());
	for (auto& Element : HttpRequest->GetAllHeaders())
	{
		LogVerbose("Header: %s", *Element);
	}
	LogVerbose("-------------------------------------------------\n");

	HttpRequest->ProcessRequest();
}

void UEOSWebVoiceLibrary::ModifyParticipant(FString AccessToken, FString ProductUserId, FString DeploymentId, FString RoomName, bool bHardMuted, const FModifyParticipantCallbackDelegate& Callback)
{
	LogVerbose("");

	if (AccessToken.IsEmpty())
	{
		LogError("AccessToken cannot be empty");
		return;
	}

	if (ProductUserId.IsEmpty())
	{
		LogError("ProductUserId cannot be empty");
		return;
	}

	if (DeploymentId.IsEmpty())
	{
		LogError("DeploymentId cannot be empty");
		return;
	}

	if (RoomName.IsEmpty())
	{
		LogError("RoomName cannot be empty");
		return;
	}

	auto HttpRequest = UEOSWebShared::CreateRequest();

	const FString ContentString = FString::Printf(TEXT("{\"hardMuted\":%s}"), *LexToString(bHardMuted));

	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->AppendToHeader(TEXT("Accept"), TEXT("application/json"));
	HttpRequest->AppendToHeader(TEXT("Authorization"), *FString::Printf(TEXT("Bearer %s"), *AccessToken));
	HttpRequest->SetContentAsString(ContentString);
	HttpRequest->SetURL(FString::Printf(TEXT("https://api.epicgames.dev/rtc/v1/%s/room/%s/participants/%s"), *DeploymentId, *RoomName, *ProductUserId));
	HttpRequest->SetVerb("POST");

	HttpRequest->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr HttpRequestPtr, FHttpResponsePtr HttpResponsePtr, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
		{
			LogVeryVerbose("ModifyParticipant Response: (%d) (%s)", HttpResponsePtr->GetResponseCode(), *HttpResponsePtr->GetContentAsString());
		}
		else
		{
			LogError("Connection failed");
		}

		Callback.ExecuteIfBound(HttpResponsePtr->GetResponseCode() == 204, FWebResponse(HttpResponsePtr->GetResponseCode(), HttpResponsePtr->GetContentAsString()));
	});

	LogVerbose("-------------------------------------------------\n");
	LogVerbose("Sending ModifyParticipant Request (%s)", *HttpRequest->GetURL());
	for (auto& Element : HttpRequest->GetAllHeaders())
	{
		LogVerbose("Header: %s", *Element);
	}
	LogVerbose("Content: %s", *ContentString);
	LogVerbose("-------------------------------------------------\n");

	HttpRequest->ProcessRequest();
}