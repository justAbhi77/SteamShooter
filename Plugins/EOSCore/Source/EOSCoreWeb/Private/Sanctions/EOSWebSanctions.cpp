/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "EOSWebSanctions.h"
#include "EOSCoreWebModule.h"
#include "EOSCoreWebLogging.h"
#include "EOSCoreWebPrivatePCH.h"
#include "Shared/EOSWebShared.h"

void UEOSWebSanctionsLibrary::QueryActiveSanctionsForSpecificPlayer(FString AccessToken, FString ProductUserId, FString QueryParameters, const FSanctionsQueryActiveSanctionsForSpecificPlayerCallbackDelegate& Callback)
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

	if (QueryParameters.IsEmpty())
	{
		LogVerbose("No QueryParameters was used");
	}

	auto HttpRequest = UEOSWebShared::CreateRequest();

	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->AppendToHeader(TEXT("Accept"), TEXT("application/json"));
	HttpRequest->AppendToHeader(TEXT("Authorization"), *FString::Printf(TEXT("Bearer %s"), *AccessToken));
	HttpRequest->SetURL(FString::Printf(TEXT("https://api.epicgames.dev/sanctions/v1/productUser/%s/active"), *ProductUserId));
	HttpRequest->SetVerb("GET");

	HttpRequest->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr HttpRequestPtr, FHttpResponsePtr HttpResponsePtr, bool bConnectedSuccessfully)
	{
		TArray<FQueryActiveSanctionsForSpecificPlayerCallbackData> QueryActiveSanctionsForSpecificPlayerCallbackDataArray;

		if (bConnectedSuccessfully)
		{
			LogVerbose("QueryAntiCheatServiceStatusByDeployment Response: %s", *HttpResponsePtr->GetContentAsString());

			if (HttpResponsePtr->GetResponseCode() == 200)
			{
				TSharedPtr<FJsonObject> JsonObject;
				const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(HttpResponsePtr->GetContentAsString());

				if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
				{
					const TSharedPtr<FJsonValue> ParticipantsObject = JsonObject->TryGetField(TEXT("elements"));

					if (ParticipantsObject && ParticipantsObject->Type == EJson::Array)
					{
						TArray<TSharedPtr<FJsonValue>> ParticipantsArray = ParticipantsObject->AsArray();

						for (const auto& Element : ParticipantsArray)
						{
							if (Element->Type != EJson::Object) continue;

							auto& Object = Element->AsObject();

							FString ReferenceIdString;
							int64 Timestamp = 0;
							FString ActionString;
							int64 ExpirationTimestamp = 0;

							TSharedPtr<FJsonValue> referenceIdObject = Object->TryGetField(TEXT("referenceId"));
							TSharedPtr<FJsonValue> timestampObject = Object->TryGetField(TEXT("timestamp"));
							TSharedPtr<FJsonValue> actionObject = Object->TryGetField(TEXT("action"));
							TSharedPtr<FJsonValue> expirationTimestampObject = Object->TryGetField(TEXT("expirationTimestamp"));

							if (referenceIdObject)
							{
								ReferenceIdString = referenceIdObject->AsString();
							}

							if (timestampObject)
							{
								Timestamp = timestampObject->AsNumber();
							}

							if (actionObject)
							{
								ActionString = actionObject->AsString();
							}

							if (expirationTimestampObject)
							{
								ExpirationTimestamp = expirationTimestampObject->AsNumber();
							}

							QueryActiveSanctionsForSpecificPlayerCallbackDataArray.Add({ReferenceIdString, Timestamp, ActionString, ExpirationTimestamp});
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

		Callback.ExecuteIfBound((HttpResponsePtr->GetResponseCode() == 200), QueryActiveSanctionsForSpecificPlayerCallbackDataArray, FWebResponse(HttpResponsePtr->GetResponseCode(), HttpResponsePtr->GetContentAsString()));
	});

	LogVerbose("-------------------------------------------------\n");
	LogVerbose("Sending QueryActiveSanctionsForSpecificPlayer Request (%s)", *HttpRequest->GetURL());
	for (auto& Element : HttpRequest->GetAllHeaders())
	{
		LogVerbose("Header: %s", *Element);
	}
	LogVerbose("-------------------------------------------------\n");

	HttpRequest->ProcessRequest();
}

void UEOSWebSanctionsLibrary::SynchronizeSanctionsToAnExternalService(FString AccessToken, FString LastLogId, const FSynchronizeSanctionsToAnExternalServiceCallbackDelegate& Callback)
{
	LogVerbose("");

	if (AccessToken.IsEmpty())
	{
		LogError("AccessToken cannot be empty");
		return;
	}

	if (LastLogId.IsEmpty())
	{
		LogVerbose("No QueryParameters was used");
	}

	auto HttpRequest = UEOSWebShared::CreateRequest();

	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->AppendToHeader(TEXT("Accept"), TEXT("application/json"));
	HttpRequest->AppendToHeader(TEXT("Authorization"), *FString::Printf(TEXT("Bearer %s"), *AccessToken));
	HttpRequest->SetURL(FString::Printf(TEXT("https://api.epicgames.dev/sanctions/v1/sync")));
	HttpRequest->SetVerb("GET");

	HttpRequest->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr HttpRequestPtr, FHttpResponsePtr HttpResponsePtr, bool bConnectedSuccessfully)
	{
		TArray<FSynchronizeSanctionsToAnExternalServiceCallbackData> SynchronizeSanctionsToAnExternalServiceCallbackDataArray;

		if (bConnectedSuccessfully)
		{
			LogVerbose("SynchronizeSanctionsToAnExternalService Response: %s", *HttpResponsePtr->GetContentAsString());

			if (HttpResponsePtr->GetResponseCode() == 200)
			{
				TSharedPtr<FJsonObject> JsonObject;
				TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(HttpResponsePtr->GetContentAsString());

				if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
				{
					const TSharedPtr<FJsonValue> ElementsObject = JsonObject->TryGetField(TEXT("elements"));

					if (ElementsObject && ElementsObject->Type == EJson::Array)
					{
						TArray<TSharedPtr<FJsonValue>> ParticipantsArray = ElementsObject->AsArray();

						for (const auto& Element : ParticipantsArray)
						{
							if (Element->Type != EJson::Object) continue;

							auto& Object = Element->AsObject();

							FSynchronizeSanctionsToAnExternalServiceCallbackData Value;

							TSharedPtr<FJsonValue> ProductUserIdObject = Object->TryGetField(TEXT("productUserId"));
							TSharedPtr<FJsonValue> ActionObject = Object->TryGetField(TEXT("action"));
							TSharedPtr<FJsonValue> JustificationObject = Object->TryGetField(TEXT("justification"));
							TSharedPtr<FJsonValue> SourceObject = Object->TryGetField(TEXT("source"));
							TSharedPtr<FJsonValue> EventTypeObject = Object->TryGetField(TEXT("eventType"));
							TSharedPtr<FJsonValue> TagsObject = Object->TryGetField(TEXT("tags"));
							TSharedPtr<FJsonValue> LogIdObject = Object->TryGetField(TEXT("logId"));
							TSharedPtr<FJsonValue> ReferenceIdObject = Object->TryGetField(TEXT("referenceId"));
							TSharedPtr<FJsonValue> TimestampObject = Object->TryGetField(TEXT("timestamp"));
							TSharedPtr<FJsonValue> ExpirationTimestampObject = Object->TryGetField(TEXT("expirationTimestamp"));
							TSharedPtr<FJsonValue> BatchUuidObject = Object->TryGetField(TEXT("batchUuid"));
							TSharedPtr<FJsonValue> EpicAccountNameObject = Object->TryGetField(TEXT("epicAccountName"));
							TSharedPtr<FJsonValue> EpicAccountIdObject = Object->TryGetField(TEXT("epicAccountId"));
							TSharedPtr<FJsonValue> EosClientIdObject = Object->TryGetField(TEXT("eosClientId"));
							TSharedPtr<FJsonValue> EosClientRoleObject = Object->TryGetField(TEXT("eosClientRole"));
							TSharedPtr<FJsonValue> CreatedAtObject = Object->TryGetField(TEXT("createdAt"));
							TSharedPtr<FJsonValue> UpdatedAtObject = Object->TryGetField(TEXT("updatedAt"));
							TSharedPtr<FJsonValue> TrustedPartnerObject = Object->TryGetField(TEXT("trustedPartner"));
							TSharedPtr<FJsonValue> DeploymentIdObject = Object->TryGetField(TEXT("deploymentId"));
							TSharedPtr<FJsonValue> PendingObject = Object->TryGetField(TEXT("pending"));
							TSharedPtr<FJsonValue> AutomatedObject = Object->TryGetField(TEXT("automated"));
							TSharedPtr<FJsonValue> DisplayNameObject = Object->TryGetField(TEXT("displayName"));
							TSharedPtr<FJsonValue> IdentityProviderObject = Object->TryGetField(TEXT("identityProvider"));
							TSharedPtr<FJsonValue> AccountIdObject = Object->TryGetField(TEXT("accountId"));

							if (ProductUserIdObject && !ProductUserIdObject->IsNull())
							{
								Value.ProductUserId = ProductUserIdObject->AsString();
							}

							if (ActionObject && !ActionObject->IsNull())
							{
								Value.Action = ActionObject->AsString();
							}

							if (JustificationObject && !JustificationObject->IsNull())
							{
								Value.Justification = JustificationObject->AsString();
							}

							if (SourceObject && !SourceObject->IsNull())
							{
								Value.Source = SourceObject->AsString();
							}

							if (EventTypeObject && !EventTypeObject->IsNull())
							{
								Value.EventType = EventTypeObject->AsNumber();
							}

							if (TagsObject && !TagsObject->IsNull())
							{
								TArray<TSharedPtr<FJsonValue>> TagsArray = TagsObject->AsArray();

								for (const auto& Tag : TagsArray)
								{
									if (Tag->Type != EJson::Object) continue;

									auto& TagObject = Tag->AsObject();

									Value.Tags.Add(TagsObject->AsString());
								}
							}

							if (LogIdObject && !LogIdObject->IsNull())
							{
								Value.LogId = LogIdObject->AsString();
							}

							if (ReferenceIdObject && !ReferenceIdObject->IsNull())
							{
								Value.ReferenceId = ReferenceIdObject->AsString();
							}

							if (TimestampObject && !TimestampObject->IsNull())
							{
								Value.Timestamp = TimestampObject->AsString();
							}

							if (ExpirationTimestampObject && !ExpirationTimestampObject->IsNull())
							{
								Value.ExpirationTimestamp = ExpirationTimestampObject->AsString();
							}

							if (BatchUuidObject && !BatchUuidObject->IsNull())
							{
								Value.BatchUuid = BatchUuidObject->AsString();
							}

							if (EpicAccountNameObject && !EpicAccountNameObject->IsNull())
							{
								Value.EpicAccountName = EpicAccountNameObject->AsString();
							}

							if (EpicAccountIdObject && !EpicAccountIdObject->IsNull())
							{
								Value.EpicAccountId = EpicAccountIdObject->AsString();
							}

							if (EosClientIdObject && !EosClientIdObject->IsNull())
							{
								Value.EosClientId = EosClientIdObject->AsString();
							}

							if (EosClientRoleObject && !EosClientRoleObject->IsNull())
							{
								Value.EosClientRole = EosClientRoleObject->AsString();
							}

							if (CreatedAtObject && !CreatedAtObject->IsNull())
							{
								Value.CreatedAt = CreatedAtObject->AsString();
							}

							if (UpdatedAtObject && !UpdatedAtObject->IsNull())
							{
								Value.UpdatedAt = UpdatedAtObject->AsString();
							}

							if (TrustedPartnerObject && !TrustedPartnerObject->IsNull())
							{
								Value.TrustedPartner = TrustedPartnerObject->AsString();
							}

							if (DeploymentIdObject && !DeploymentIdObject->IsNull())
							{
								Value.DeploymentId = DeploymentIdObject->AsString();
							}

							if (PendingObject && !PendingObject->IsNull())
							{
								Value.bPending = PendingObject->AsBool();
							}

							if (AutomatedObject && !AutomatedObject->IsNull())
							{
								Value.bAutomated = AutomatedObject->AsBool();
							}

							if (DisplayNameObject && !DisplayNameObject->IsNull())
							{
								Value.DisplayName = DisplayNameObject->AsString();
							}

							if (IdentityProviderObject && !IdentityProviderObject->IsNull())
							{
								Value.IdentityProvider = IdentityProviderObject->AsString();
							}

							if (AccountIdObject && !AccountIdObject->IsNull())
							{
								Value.AccountId = AccountIdObject->AsString();
							}

							SynchronizeSanctionsToAnExternalServiceCallbackDataArray.Add(Value);
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

		Callback.ExecuteIfBound((HttpResponsePtr->GetResponseCode() == 200), SynchronizeSanctionsToAnExternalServiceCallbackDataArray, FWebResponse(HttpResponsePtr->GetResponseCode(), HttpResponsePtr->GetContentAsString()));
	});

	LogVerbose("-------------------------------------------------\n");
	LogVerbose("Sending QueryActiveSanctionsForSpecificPlayer Request (%s)", *HttpRequest->GetURL());
	for (auto& Element : HttpRequest->GetAllHeaders())
	{
		LogVerbose("Header: %s", *Element);
	}
	LogVerbose("-------------------------------------------------\n");

	HttpRequest->ProcessRequest();
}

struct FCreateSanctionsJson final : public FJsonSerializable
{
	explicit FCreateSanctionsJson(FSanctionPostPayload Request)
		: ProductUserId(Request.ProductUserId)
		, Action(Request.Action)
		, Justification(Request.Justification)
		, Source(Request.Source)
		, Tags(Request.Tags)
		, bPending(Request.bPending)
		, DisplayName(Request.DisplayName)
		, IdentityProvider(Request.IdentityProvider)
		, AccountId(Request.AccountId)
		, Duration(Request.Duration)
	{
		
	}

	FString ProductUserId;
	FString Action;
	FString Justification;
	FString Source;
	TArray<FString> Tags;
	bool bPending;
	FString DisplayName;
	FString IdentityProvider;
	FString AccountId;
	int32 Duration;

	BEGIN_JSON_SERIALIZER
	JSON_SERIALIZE("productUserId", ProductUserId);
	JSON_SERIALIZE("action", Action);
	JSON_SERIALIZE("justification", Justification);
	JSON_SERIALIZE("source", Source);
	JSON_SERIALIZE_ARRAY("tags", Tags);
	JSON_SERIALIZE("pending", bPending);
	JSON_SERIALIZE("displayName", DisplayName);
	JSON_SERIALIZE("identityProvider", IdentityProvider);
	JSON_SERIALIZE("accountId", AccountId);
	JSON_SERIALIZE("duration", Duration);
	END_JSON_SERIALIZER
};

void UEOSWebSanctionsLibrary::CreateSanction(FString AccessToken, FString DeploymentId, FSanctionPostPayload SanctionPostPayload, const FCreateSanctionsCallbackDelegate& Callback)
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
	}

	const auto HttpRequest = UEOSWebShared::CreateRequest();

	FCreateSanctionsJson CreateSanctionsJson(SanctionPostPayload);
	FString ContentString = CreateSanctionsJson.ToJson(false);
	ContentString = "[";
	ContentString += CreateSanctionsJson.ToJson(false);
	ContentString += "]";
	
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->AppendToHeader(TEXT("Accept"), TEXT("application/json"));
	HttpRequest->AppendToHeader(TEXT("Authorization"), *FString::Printf(TEXT("Bearer %s"), *AccessToken));
	HttpRequest->SetContentAsString(ContentString);
	HttpRequest->SetURL(FString::Printf(TEXT("https://api.epicgames.dev/sanctions/v1/%s/sanctions"), *DeploymentId));
	HttpRequest->SetVerb("POST");

	HttpRequest->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr HttpRequestPtr, FHttpResponsePtr HttpResponsePtr, bool bConnectedSuccessfully)
	{
		TArray<FSanction> Sanctions;

		if (bConnectedSuccessfully)
		{
			LogVerbose("CreateSanctions Response: %s", *HttpResponsePtr->GetContentAsString());

			if (HttpResponsePtr->GetResponseCode() == 200)
			{
				TSharedPtr<FJsonObject> JsonObject;
				TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(HttpResponsePtr->GetContentAsString());

				if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
				{
					const TSharedPtr<FJsonValue> ElementsObject = JsonObject->TryGetField(TEXT("elements"));

					if (ElementsObject && ElementsObject->Type == EJson::Array)
					{
						TArray<TSharedPtr<FJsonValue>> ParticipantsArray = ElementsObject->AsArray();

						for (const auto& Element : ParticipantsArray)
						{
							if (Element->Type != EJson::Object) continue;

							auto& Object = Element->AsObject();

							FSanction Value;

							TSharedPtr<FJsonValue> ProductUserIdObject = Object->TryGetField(TEXT("productUserId"));
							TSharedPtr<FJsonValue> ActionObject = Object->TryGetField(TEXT("action"));
							TSharedPtr<FJsonValue> JustificationObject = Object->TryGetField(TEXT("justification"));
							TSharedPtr<FJsonValue> SourceObject = Object->TryGetField(TEXT("source"));
							TSharedPtr<FJsonValue> TagsObject = Object->TryGetField(TEXT("tags"));
							TSharedPtr<FJsonValue> ReferenceIdObject = Object->TryGetField(TEXT("referenceId"));
							TSharedPtr<FJsonValue> TimestampObject = Object->TryGetField(TEXT("timestamp"));
							TSharedPtr<FJsonValue> ExpirationTimestampObject = Object->TryGetField(TEXT("expirationTimestamp"));
							TSharedPtr<FJsonValue> BatchUuidObject = Object->TryGetField(TEXT("batchUuid"));
							TSharedPtr<FJsonValue> EpicAccountNameObject = Object->TryGetField(TEXT("epicAccountName"));
							TSharedPtr<FJsonValue> EpicAccountIdObject = Object->TryGetField(TEXT("epicAccountId"));
							TSharedPtr<FJsonValue> EosClientIdObject = Object->TryGetField(TEXT("eosClientId"));
							TSharedPtr<FJsonValue> EosClientRoleObject = Object->TryGetField(TEXT("eosClientRole"));
							TSharedPtr<FJsonValue> CreatedAtObject = Object->TryGetField(TEXT("createdAt"));
							TSharedPtr<FJsonValue> UpdatedAtObject = Object->TryGetField(TEXT("updatedAt"));
							TSharedPtr<FJsonValue> RemovedAtObject = Object->TryGetField(TEXT("removedAt"));
							TSharedPtr<FJsonValue> TrustedPartnerObject = Object->TryGetField(TEXT("trustedPartner"));
							TSharedPtr<FJsonValue> DeploymentIdObject = Object->TryGetField(TEXT("deploymentId"));
							TSharedPtr<FJsonValue> PendingObject = Object->TryGetField(TEXT("pending"));
							TSharedPtr<FJsonValue> AutomatedObject = Object->TryGetField(TEXT("automated"));
							TSharedPtr<FJsonValue> DisplayNameObject = Object->TryGetField(TEXT("displayName"));
							TSharedPtr<FJsonValue> IdentityProviderObject = Object->TryGetField(TEXT("identityProvider"));
							TSharedPtr<FJsonValue> AccountIdObject = Object->TryGetField(TEXT("accountId"));
							TSharedPtr<FJsonValue> StatusObject = Object->TryGetField(TEXT("status"));

							if (ProductUserIdObject && !ProductUserIdObject->IsNull())
							{
								Value.ProductUserId = ProductUserIdObject->AsString();
							}

							if (ActionObject && !ActionObject->IsNull())
							{
								Value.Action = ActionObject->AsString();
							}

							if (JustificationObject && !JustificationObject->IsNull())
							{
								Value.Justification = JustificationObject->AsString();
							}

							if (SourceObject && !SourceObject->IsNull())
							{
								Value.Source = SourceObject->AsString();
							}

							if (TagsObject && !TagsObject->IsNull())
							{
								TArray<TSharedPtr<FJsonValue>> TagsArray = TagsObject->AsArray();

								for (const auto& Tag : TagsArray)
								{
									if (Tag->Type != EJson::Object) continue;

									auto& TagObject = Tag->AsObject();

									Value.Tags.Add(TagsObject->AsString());
								}
							}

							if (ReferenceIdObject && !ReferenceIdObject->IsNull())
							{
								Value.ReferenceId = ReferenceIdObject->AsString();
							}

							if (TimestampObject && !TimestampObject->IsNull())
							{
								Value.Timestamp = TimestampObject->AsString();
							}

							if (ExpirationTimestampObject && !ExpirationTimestampObject->IsNull())
							{
								Value.ExpirationTimestamp = ExpirationTimestampObject->AsString();
							}

							if (BatchUuidObject && !BatchUuidObject->IsNull())
							{
								Value.BatchUuid = BatchUuidObject->AsString();
							}

							if (EpicAccountNameObject && !EpicAccountNameObject->IsNull())
							{
								Value.EpicAccountName = EpicAccountNameObject->AsString();
							}

							if (EpicAccountIdObject && !EpicAccountIdObject->IsNull())
							{
								Value.EpicAccountId = EpicAccountIdObject->AsString();
							}

							if (EosClientIdObject && !EosClientIdObject->IsNull())
							{
								Value.EosClientId = EosClientIdObject->AsString();
							}

							if (EosClientRoleObject && !EosClientRoleObject->IsNull())
							{
								Value.EosClientRole = EosClientRoleObject->AsString();
							}

							if (CreatedAtObject && !CreatedAtObject->IsNull())
							{
								Value.CreatedAt = CreatedAtObject->AsString();
							}

							if (UpdatedAtObject && !UpdatedAtObject->IsNull())
							{
								Value.UpdatedAt = UpdatedAtObject->AsString();
							}

							if (RemovedAtObject && !RemovedAtObject->IsNull())
							{
								Value.RemovedAt = RemovedAtObject->AsString();
							}

							if (TrustedPartnerObject && !TrustedPartnerObject->IsNull())
							{
								Value.TrustedPartner = TrustedPartnerObject->AsString();
							}

							if (DeploymentIdObject && !DeploymentIdObject->IsNull())
							{
								Value.DeploymentId = DeploymentIdObject->AsString();
							}

							if (PendingObject && !PendingObject->IsNull())
							{
								Value.bPending = PendingObject->AsBool();
							}

							if (AutomatedObject && !AutomatedObject->IsNull())
							{
								Value.bAutomated = AutomatedObject->AsBool();
							}

							if (DisplayNameObject && !DisplayNameObject->IsNull())
							{
								Value.DisplayName = DisplayNameObject->AsString();
							}

							if (IdentityProviderObject && !IdentityProviderObject->IsNull())
							{
								Value.IdentityProvider = IdentityProviderObject->AsString();
							}

							if (AccountIdObject && !AccountIdObject->IsNull())
							{
								Value.AccountId = AccountIdObject->AsString();
							}

							if (StatusObject && !StatusObject->IsNull())
							{
								Value.Status = StatusObject->AsString();
							}

							Sanctions.Add(Value);
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

		Callback.ExecuteIfBound((HttpResponsePtr->GetResponseCode() == 200), Sanctions, FWebResponse(HttpResponsePtr->GetResponseCode(), HttpResponsePtr->GetContentAsString()));
	});

	LogVerbose("-------------------------------------------------\n");
	LogVerbose("Sending CreateSanctions Request (%s)", *HttpRequest->GetURL());
	for (auto& Element : HttpRequest->GetAllHeaders())
	{
		LogVerbose("Header: %s", *Element);
	}
	LogVerbose("-------------------------------------------------\n");

	HttpRequest->ProcessRequest();
}

void UEOSWebSanctionsLibrary::QuerySanctions(FString AccessToken, FString DeploymentId, int32 Limit, int32 Offset, const FQuerySanctionsCallbackDelegate& Callback)
{
	LogVerbose("");

	if (AccessToken.IsEmpty())
	{
		LogError("AccessToken cannot be empty");
		return;
	}

	if (DeploymentId.IsEmpty())
	{
		LogVerbose("DeploymentId cannot be empty");
	}

	if (Limit == 0)
		Limit = 100;

	const auto HttpRequest = UEOSWebShared::CreateRequest();

	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->AppendToHeader(TEXT("Accept"), TEXT("application/json"));
	HttpRequest->AppendToHeader(TEXT("Authorization"), *FString::Printf(TEXT("Bearer %s"), *AccessToken));
	HttpRequest->SetURL(FString::Printf(TEXT("https://api.epicgames.dev/sanctions/v1/%s/sanctions"), *DeploymentId));
	HttpRequest->SetVerb("GET");

	HttpRequest->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr HttpRequestPtr, FHttpResponsePtr HttpResponsePtr, bool bConnectedSuccessfully)
	{
		TArray<FQuerySanctionsResult> QuerySanctionsResult;

		int32 TotalResult = 0;
		int32 OffsetResult = 0;
		int32 LimitResult = 0;

		if (bConnectedSuccessfully)
		{
			LogVerbose("QuerySanctions Response: %s", *HttpResponsePtr->GetContentAsString());

			if (HttpResponsePtr->GetResponseCode() == 200)
			{
				TSharedPtr<FJsonObject> JsonObject;
				TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(HttpResponsePtr->GetContentAsString());

				if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
				{
					if (const TSharedPtr<FJsonValue> Paging = JsonObject->TryGetField(TEXT("paging")))
					{
						if (auto& PagingObject = Paging->AsObject())
						{
							if (TSharedPtr<FJsonValue> TotalObject = PagingObject->TryGetField(TEXT("total")))
							{
								TotalResult = TotalObject->AsNumber();
							}
							if (TSharedPtr<FJsonValue> OffsetObject = PagingObject->TryGetField(TEXT("offset")))
							{
								OffsetResult = OffsetObject->AsNumber();
							}
							if (TSharedPtr<FJsonValue> LimitObject = PagingObject->TryGetField(TEXT("limit")))
							{
								LimitResult = LimitObject->AsNumber();
							}
						}
					}
					
					const TSharedPtr<FJsonValue> ElementsObject = JsonObject->TryGetField(TEXT("elements"));

					if (ElementsObject && ElementsObject->Type == EJson::Array)
					{
						TArray<TSharedPtr<FJsonValue>> ParticipantsArray = ElementsObject->AsArray();

						for (const auto& Element : ParticipantsArray)
						{
							if (Element->Type != EJson::Object) continue;

							auto& Object = Element->AsObject();

							FQuerySanctionsResult Value;
							
							TSharedPtr<FJsonValue> ProductUserIdObject = Object->TryGetField(TEXT("productUserId"));
							TSharedPtr<FJsonValue> ActionObject = Object->TryGetField(TEXT("action"));
							TSharedPtr<FJsonValue> JustificationObject = Object->TryGetField(TEXT("justification"));
							TSharedPtr<FJsonValue> SourceObject = Object->TryGetField(TEXT("source"));
							TSharedPtr<FJsonValue> TagsObject = Object->TryGetField(TEXT("tags"));
							TSharedPtr<FJsonValue> ReferenceIdObject = Object->TryGetField(TEXT("referenceId"));
							TSharedPtr<FJsonValue> TimestampObject = Object->TryGetField(TEXT("timestamp"));
							TSharedPtr<FJsonValue> ExpirationTimestampObject = Object->TryGetField(TEXT("expirationTimestamp"));
							TSharedPtr<FJsonValue> BatchUuidObject = Object->TryGetField(TEXT("batchUuid"));
							TSharedPtr<FJsonValue> EpicAccountNameObject = Object->TryGetField(TEXT("epicAccountName"));
							TSharedPtr<FJsonValue> EpicAccountIdObject = Object->TryGetField(TEXT("epicAccountId"));
							TSharedPtr<FJsonValue> EosClientIdObject = Object->TryGetField(TEXT("eosClientId"));
							TSharedPtr<FJsonValue> EosClientRoleObject = Object->TryGetField(TEXT("eosClientRole"));
							TSharedPtr<FJsonValue> CreatedAtObject = Object->TryGetField(TEXT("createdAt"));
							TSharedPtr<FJsonValue> UpdatedAtObject = Object->TryGetField(TEXT("updatedAt"));
							TSharedPtr<FJsonValue> RemovedAtObject = Object->TryGetField(TEXT("removedAt"));
							TSharedPtr<FJsonValue> TrustedPartnerObject = Object->TryGetField(TEXT("trustedPartner"));
							TSharedPtr<FJsonValue> DeploymentIdObject = Object->TryGetField(TEXT("deploymentId"));
							TSharedPtr<FJsonValue> PendingObject = Object->TryGetField(TEXT("pending"));
							TSharedPtr<FJsonValue> AutomatedObject = Object->TryGetField(TEXT("automated"));
							TSharedPtr<FJsonValue> DisplayNameObject = Object->TryGetField(TEXT("displayName"));
							TSharedPtr<FJsonValue> IdentityProviderObject = Object->TryGetField(TEXT("identityProvider"));
							TSharedPtr<FJsonValue> AccountIdObject = Object->TryGetField(TEXT("accountId"));
							TSharedPtr<FJsonValue> StatusObject = Object->TryGetField(TEXT("status"));

							if (ProductUserIdObject && !ProductUserIdObject->IsNull())
							{
								Value.ProductUserId = ProductUserIdObject->AsString();
							}

							if (ActionObject && !ActionObject->IsNull())
							{
								Value.Action = ActionObject->AsString();
							}

							if (JustificationObject && !JustificationObject->IsNull())
							{
								Value.Justification = JustificationObject->AsString();
							}

							if (SourceObject && !SourceObject->IsNull())
							{
								Value.Source = SourceObject->AsString();
							}

							if (TagsObject && !TagsObject->IsNull())
							{
								TArray<TSharedPtr<FJsonValue>> TagsArray = TagsObject->AsArray();

								for (const auto& Tag : TagsArray)
								{
									if (Tag->Type != EJson::Object) continue;

									auto& TagObject = Tag->AsObject();

									Value.Tags.Add(TagsObject->AsString());
								}
							}

							if (ReferenceIdObject && !ReferenceIdObject->IsNull())
							{
								Value.ReferenceId = ReferenceIdObject->AsString();
							}

							if (TimestampObject && !TimestampObject->IsNull())
							{
								Value.Timestamp = TimestampObject->AsString();
							}

							if (ExpirationTimestampObject && !ExpirationTimestampObject->IsNull())
							{
								Value.ExpirationTimestamp = ExpirationTimestampObject->AsString();
							}

							if (BatchUuidObject && !BatchUuidObject->IsNull())
							{
								Value.BatchUuid = BatchUuidObject->AsString();
							}

							if (EpicAccountNameObject && !EpicAccountNameObject->IsNull())
							{
								Value.EpicAccountName = EpicAccountNameObject->AsString();
							}

							if (EpicAccountIdObject && !EpicAccountIdObject->IsNull())
							{
								Value.EpicAccountId = EpicAccountIdObject->AsString();
							}

							if (EosClientIdObject && !EosClientIdObject->IsNull())
							{
								Value.EosClientId = EosClientIdObject->AsString();
							}

							if (EosClientRoleObject && !EosClientRoleObject->IsNull())
							{
								Value.EosClientRole = EosClientRoleObject->AsString();
							}

							if (CreatedAtObject && !CreatedAtObject->IsNull())
							{
								Value.CreatedAt = CreatedAtObject->AsString();
							}

							if (UpdatedAtObject && !UpdatedAtObject->IsNull())
							{
								Value.UpdatedAt = UpdatedAtObject->AsString();
							}

							if (RemovedAtObject && !RemovedAtObject->IsNull())
							{
								Value.RemovedAt = RemovedAtObject->AsString();
							}

							if (TrustedPartnerObject && !TrustedPartnerObject->IsNull())
							{
								Value.TrustedPartner = TrustedPartnerObject->AsString();
							}

							if (DeploymentIdObject && !DeploymentIdObject->IsNull())
							{
								Value.DeploymentId = DeploymentIdObject->AsString();
							}

							if (PendingObject && !PendingObject->IsNull())
							{
								Value.bPending = PendingObject->AsBool();
							}

							if (AutomatedObject && !AutomatedObject->IsNull())
							{
								Value.bAutomated = AutomatedObject->AsBool();
							}

							if (DisplayNameObject && !DisplayNameObject->IsNull())
							{
								Value.DisplayName = DisplayNameObject->AsString();
							}

							if (IdentityProviderObject && !IdentityProviderObject->IsNull())
							{
								Value.IdentityProvider = IdentityProviderObject->AsString();
							}

							if (AccountIdObject && !AccountIdObject->IsNull())
							{
								Value.AccountId = AccountIdObject->AsString();
							}

							if (StatusObject && !StatusObject->IsNull())
							{
								Value.Status = StatusObject->AsString();
							}

							QuerySanctionsResult.Add(Value);
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

		Callback.ExecuteIfBound((HttpResponsePtr->GetResponseCode() == 200), QuerySanctionsResult, TotalResult, OffsetResult, LimitResult, FWebResponse(HttpResponsePtr->GetResponseCode(), HttpResponsePtr->GetContentAsString()));
	});

	LogVerbose("-------------------------------------------------\n");
	LogVerbose("Sending QuerySanctions Request (%s)", *HttpRequest->GetURL());
	for (auto& Element : HttpRequest->GetAllHeaders())
	{
		LogVerbose("Header: %s", *Element);
	}
	LogVerbose("-------------------------------------------------\n");

	HttpRequest->ProcessRequest();
}

void UEOSWebSanctionsLibrary::QuerySanctionsForPlayer(FString AccessToken, FString ProductUserId, FString DeploymentId, int32 Limit, int32 Offset, const FQuerySanctionsForPlayerCallbackDelegate& Callback)
{
	LogVerbose("");

	if (AccessToken.IsEmpty())
	{
		LogError("AccessToken cannot be empty");
		return;
	}

	if (DeploymentId.IsEmpty())
	{
		LogVerbose("DeploymentId cannot be empty");
	}

	if (ProductUserId.IsEmpty())
	{
		LogVerbose("ProductUserId cannot be empty");
	}

	if (Limit == 0)
		Limit = 100;

	auto HttpRequest = UEOSWebShared::CreateRequest();

	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->AppendToHeader(TEXT("Accept"), TEXT("application/json"));
	HttpRequest->AppendToHeader(TEXT("Authorization"), *FString::Printf(TEXT("Bearer %s"), *AccessToken));
	HttpRequest->SetURL(FString::Printf(TEXT("https://api.epicgames.dev/sanctions/v1/%s/users/%s"), *DeploymentId, *ProductUserId));
	HttpRequest->SetVerb("GET");

	HttpRequest->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr HttpRequestPtr, FHttpResponsePtr HttpResponsePtr, bool bConnectedSuccessfully)
	{
		TArray<FQuerySanctionsResult> QuerySanctionsResult;

		int32 TotalResult = 0;
		int32 OffsetResult = 0;
		int32 LimitResult = 0;

		if (bConnectedSuccessfully)
		{
			LogVerbose("QuerySanctionsForPlayer Response: %s", *HttpResponsePtr->GetContentAsString());

			if (HttpResponsePtr->GetResponseCode() == 200)
			{
				TSharedPtr<FJsonObject> JsonObject;
				TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(HttpResponsePtr->GetContentAsString());

				if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
				{
					if (const TSharedPtr<FJsonValue> Paging = JsonObject->TryGetField(TEXT("paging")))
					{
						if (auto& PagingObject = Paging->AsObject())
						{
							if (TSharedPtr<FJsonValue> TotalObject = PagingObject->TryGetField(TEXT("total")))
							{
								TotalResult = TotalObject->AsNumber();
							}
							if (TSharedPtr<FJsonValue> OffsetObject = PagingObject->TryGetField(TEXT("offset")))
							{
								OffsetResult = OffsetObject->AsNumber();
							}
							if (TSharedPtr<FJsonValue> LimitObject = PagingObject->TryGetField(TEXT("limit")))
							{
								LimitResult = LimitObject->AsNumber();
							}
						}
					}
					
					const TSharedPtr<FJsonValue> ElementsObject = JsonObject->TryGetField(TEXT("elements"));

					if (ElementsObject && ElementsObject->Type == EJson::Array)
					{
						TArray<TSharedPtr<FJsonValue>> ParticipantsArray = ElementsObject->AsArray();

						for (const auto& Element : ParticipantsArray)
						{
							if (Element->Type != EJson::Object) continue;

							auto& Object = Element->AsObject();

							FQuerySanctionsResult Value;

							TSharedPtr<FJsonValue> ProductUserIdObject = Object->TryGetField(TEXT("productUserId"));
							TSharedPtr<FJsonValue> ActionObject = Object->TryGetField(TEXT("action"));
							TSharedPtr<FJsonValue> JustificationObject = Object->TryGetField(TEXT("justification"));
							TSharedPtr<FJsonValue> SourceObject = Object->TryGetField(TEXT("source"));
							TSharedPtr<FJsonValue> TagsObject = Object->TryGetField(TEXT("tags"));
							TSharedPtr<FJsonValue> ReferenceIdObject = Object->TryGetField(TEXT("referenceId"));
							TSharedPtr<FJsonValue> TimestampObject = Object->TryGetField(TEXT("timestamp"));
							TSharedPtr<FJsonValue> ExpirationTimestampObject = Object->TryGetField(TEXT("expirationTimestamp"));
							TSharedPtr<FJsonValue> BatchUuidObject = Object->TryGetField(TEXT("batchUuid"));
							TSharedPtr<FJsonValue> EpicAccountNameObject = Object->TryGetField(TEXT("epicAccountName"));
							TSharedPtr<FJsonValue> EpicAccountIdObject = Object->TryGetField(TEXT("epicAccountId"));
							TSharedPtr<FJsonValue> EosClientIdObject = Object->TryGetField(TEXT("eosClientId"));
							TSharedPtr<FJsonValue> EosClientRoleObject = Object->TryGetField(TEXT("eosClientRole"));
							TSharedPtr<FJsonValue> CreatedAtObject = Object->TryGetField(TEXT("createdAt"));
							TSharedPtr<FJsonValue> UpdatedAtObject = Object->TryGetField(TEXT("updatedAt"));
							TSharedPtr<FJsonValue> RemovedAtObject = Object->TryGetField(TEXT("removedAt"));
							TSharedPtr<FJsonValue> TrustedPartnerObject = Object->TryGetField(TEXT("trustedPartner"));
							TSharedPtr<FJsonValue> DeploymentIdObject = Object->TryGetField(TEXT("deploymentId"));
							TSharedPtr<FJsonValue> PendingObject = Object->TryGetField(TEXT("pending"));
							TSharedPtr<FJsonValue> AutomatedObject = Object->TryGetField(TEXT("automated"));
							TSharedPtr<FJsonValue> DisplayNameObject = Object->TryGetField(TEXT("displayName"));
							TSharedPtr<FJsonValue> IdentityProviderObject = Object->TryGetField(TEXT("identityProvider"));
							TSharedPtr<FJsonValue> AccountIdObject = Object->TryGetField(TEXT("accountId"));
							TSharedPtr<FJsonValue> StatusObject = Object->TryGetField(TEXT("status"));

							if (ProductUserIdObject && !ProductUserIdObject->IsNull())
							{
								Value.ProductUserId = ProductUserIdObject->AsString();
							}

							if (ActionObject && !ActionObject->IsNull())
							{
								Value.Action = ActionObject->AsString();
							}

							if (JustificationObject && !JustificationObject->IsNull())
							{
								Value.Justification = JustificationObject->AsString();
							}

							if (SourceObject && !SourceObject->IsNull())
							{
								Value.Source = SourceObject->AsString();
							}

							if (TagsObject && !TagsObject->IsNull())
							{
								TArray<TSharedPtr<FJsonValue>> TagsArray = TagsObject->AsArray();

								for (const auto& Tag : TagsArray)
								{
									if (Tag->Type != EJson::Object) continue;

									auto& TagObject = Tag->AsObject();

									Value.Tags.Add(TagsObject->AsString());
								}
							}

							if (ReferenceIdObject && !ReferenceIdObject->IsNull())
							{
								Value.ReferenceId = ReferenceIdObject->AsString();
							}

							if (TimestampObject && !TimestampObject->IsNull())
							{
								Value.Timestamp = TimestampObject->AsString();
							}

							if (ExpirationTimestampObject && !ExpirationTimestampObject->IsNull())
							{
								Value.ExpirationTimestamp = ExpirationTimestampObject->AsString();
							}

							if (BatchUuidObject && !BatchUuidObject->IsNull())
							{
								Value.BatchUuid = BatchUuidObject->AsString();
							}

							if (EpicAccountNameObject && !EpicAccountNameObject->IsNull())
							{
								Value.EpicAccountName = EpicAccountNameObject->AsString();
							}

							if (EpicAccountIdObject && !EpicAccountIdObject->IsNull())
							{
								Value.EpicAccountId = EpicAccountIdObject->AsString();
							}

							if (EosClientIdObject && !EosClientIdObject->IsNull())
							{
								Value.EosClientId = EosClientIdObject->AsString();
							}

							if (EosClientRoleObject && !EosClientRoleObject->IsNull())
							{
								Value.EosClientRole = EosClientRoleObject->AsString();
							}

							if (CreatedAtObject && !CreatedAtObject->IsNull())
							{
								Value.CreatedAt = CreatedAtObject->AsString();
							}

							if (UpdatedAtObject && !UpdatedAtObject->IsNull())
							{
								Value.UpdatedAt = UpdatedAtObject->AsString();
							}

							if (RemovedAtObject && !RemovedAtObject->IsNull())
							{
								Value.RemovedAt = RemovedAtObject->AsString();
							}

							if (TrustedPartnerObject && !TrustedPartnerObject->IsNull())
							{
								Value.TrustedPartner = TrustedPartnerObject->AsString();
							}

							if (DeploymentIdObject && !DeploymentIdObject->IsNull())
							{
								Value.DeploymentId = DeploymentIdObject->AsString();
							}

							if (PendingObject && !PendingObject->IsNull())
							{
								Value.bPending = PendingObject->AsBool();
							}

							if (AutomatedObject && !AutomatedObject->IsNull())
							{
								Value.bAutomated = AutomatedObject->AsBool();
							}

							if (DisplayNameObject && !DisplayNameObject->IsNull())
							{
								Value.DisplayName = DisplayNameObject->AsString();
							}

							if (IdentityProviderObject && !IdentityProviderObject->IsNull())
							{
								Value.IdentityProvider = IdentityProviderObject->AsString();
							}

							if (AccountIdObject && !AccountIdObject->IsNull())
							{
								Value.AccountId = AccountIdObject->AsString();
							}

							if (StatusObject && !StatusObject->IsNull())
							{
								Value.Status = StatusObject->AsString();
							}

							QuerySanctionsResult.Add(Value);
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

		Callback.ExecuteIfBound((HttpResponsePtr->GetResponseCode() == 200), QuerySanctionsResult, TotalResult, OffsetResult, LimitResult, FWebResponse(HttpResponsePtr->GetResponseCode(), HttpResponsePtr->GetContentAsString()));
	});

	LogVerbose("-------------------------------------------------\n");
	LogVerbose("Sending QuerySanctionsForPlayer Request (%s)", *HttpRequest->GetURL());
	for (auto& Element : HttpRequest->GetAllHeaders())
	{
		LogVerbose("Header: %s", *Element);
	}
	LogVerbose("-------------------------------------------------\n");

	HttpRequest->ProcessRequest();
}

void UEOSWebSanctionsLibrary::RemoveSanction(FString AccessToken, FString DeploymentId, FString ReferenceId, const FRemoveSanctionCallbackDelegate& Callback)
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

	if (ReferenceId.IsEmpty())
	{
		LogVerbose("ReferenceId cannot be empty");
	}

	const auto HttpRequest = UEOSWebShared::CreateRequest();

	const FString ContentString = FString::Printf(TEXT("{\"referenceIds\":[\"%s\"]}"), *ReferenceId);
	
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->AppendToHeader(TEXT("Accept"), TEXT("application/json"));
	HttpRequest->AppendToHeader(TEXT("Authorization"), *FString::Printf(TEXT("Bearer %s"), *AccessToken));
	HttpRequest->SetURL(FString::Printf(TEXT("https://api.epicgames.dev/sanctions/v1/%s/sanctions"), *DeploymentId));
	HttpRequest->SetContentAsString(ContentString);
	HttpRequest->SetVerb("DELETE");

	HttpRequest->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr HttpRequestPtr, FHttpResponsePtr HttpResponsePtr, bool bConnectedSuccessfully)
	{
		Callback.ExecuteIfBound((HttpResponsePtr->GetResponseCode() == 200), FWebResponse(HttpResponsePtr->GetResponseCode(), HttpResponsePtr->GetContentAsString()));
	});

	LogVerbose("-------------------------------------------------\n");
	LogVerbose("Sending RemoveSanction Request (%s)", *HttpRequest->GetURL());
	for (auto& Element : HttpRequest->GetAllHeaders())
	{
		LogVerbose("Header: %s", *Element);
	}
	LogVerbose("-------------------------------------------------\n");

	HttpRequest->ProcessRequest();
}

struct FCreateSanctionAppealJson final : public FJsonSerializable
{
	explicit FCreateSanctionAppealJson(FAppealPostPayload Request)
		: ReferenceId(Request.ReferenceId)
		, Reason(Request.Reason)
	{
		
	}

	FString ReferenceId;
	int32 Reason;

	BEGIN_JSON_SERIALIZER
	JSON_SERIALIZE("referenceId", ReferenceId);
	JSON_SERIALIZE("reason", Reason);
	END_JSON_SERIALIZER
};

void UEOSWebSanctionsLibrary::CreateSanctionAppeal(FString AccessToken, FString DeploymentId, FAppealPostPayload Request, const FCreateSanctionAppealCallbackDelegate& Callback)
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

	const auto HttpRequest = UEOSWebShared::CreateRequest();

	FCreateSanctionAppealJson CreateSanctionAppealJson(Request);
	FString ContentString = CreateSanctionAppealJson.ToJson(false);

	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->AppendToHeader(TEXT("Accept"), TEXT("application/json"));
	HttpRequest->AppendToHeader(TEXT("Authorization"), *FString::Printf(TEXT("Bearer %s"), *AccessToken));
	HttpRequest->SetURL(FString::Printf(TEXT("https://api.epicgames.dev/sanctions/v1/%s/appeals"), *DeploymentId));
	HttpRequest->SetContentAsString(ContentString);
	HttpRequest->SetVerb("POST");

	HttpRequest->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr HttpRequestPtr, FHttpResponsePtr HttpResponsePtr, bool bConnectedSuccessfully)
	{
		FAppealResponse AppealResponse;

		if (bConnectedSuccessfully)
		{
			LogVeryVerbose("CreateSanctionAppeal Response: %s", *HttpResponsePtr->GetContentAsString());

			TSharedPtr<FJsonObject> JsonObject;
			const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(HttpResponsePtr->GetContentAsString());

			if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
			{
				const TSharedPtr<FJsonValue> IdObject = JsonObject->TryGetField(TEXT("id"));
				const TSharedPtr<FJsonValue> StatusObject = JsonObject->TryGetField(TEXT("status"));
				const TSharedPtr<FJsonValue> ReferenceIdObject = JsonObject->TryGetField(TEXT("referenceId"));
				const TSharedPtr<FJsonValue> CreatedAtObject = JsonObject->TryGetField(TEXT("createdAt"));
				const TSharedPtr<FJsonValue> UpdatedAtObject = JsonObject->TryGetField(TEXT("updatedAt"));
				const TSharedPtr<FJsonValue> ReasonObject = JsonObject->TryGetField(TEXT("reason"));

				if (IdObject && !IdObject->IsNull())
				{
					AppealResponse.Id = IdObject->AsString();
				}
				
				if (StatusObject && !StatusObject->IsNull())
				{
					AppealResponse.Status = StatusObject->AsString();
				}

				if (ReferenceIdObject && !ReferenceIdObject->IsNull())
				{
					AppealResponse.ReferenceId = ReferenceIdObject->AsString();
				}

				if (CreatedAtObject && !CreatedAtObject->IsNull())
				{
					AppealResponse.CreatedAt = CreatedAtObject->AsString();
				}
				
				if (UpdatedAtObject && !UpdatedAtObject->IsNull())
				{
					AppealResponse.UpdatedAt = UpdatedAtObject->AsString();
				}

				if (ReasonObject && !ReasonObject->IsNull())
				{
					AppealResponse.Reason = ReasonObject->AsNumber();
				}
			}
		}
		else
		{
			LogError("Connection failed");
		}

		Callback.ExecuteIfBound((HttpResponsePtr->GetResponseCode() == 200), AppealResponse, FWebResponse(HttpResponsePtr->GetResponseCode(), HttpResponsePtr->GetContentAsString()));
	});

	LogVerbose("-------------------------------------------------\n");
	LogVerbose("Sending CreateSanctionAppeal Request (%s)", *HttpRequest->GetURL());
	for (auto& Element : HttpRequest->GetAllHeaders())
	{
		LogVerbose("Header: %s", *Element);
	}
	LogVerbose("-------------------------------------------------\n");

	HttpRequest->ProcessRequest();
}

void UEOSWebSanctionsLibrary::QuerySanctionAppeals(FString AccessToken, FString DeploymentId, FQuerySanctionAppeals Request, const FQuerySanctionAppealsCallbackDelegate& Callback)
{
	LogVerbose("");

	if (AccessToken.IsEmpty())
	{
		LogError("AccessToken cannot be empty");
		return;
	}

	if (DeploymentId.IsEmpty())
	{
		LogVerbose("DeploymentId cannot be empty");
	}

	const auto HttpRequest = UEOSWebShared::CreateRequest();

	FString FormattedUrl = FString::Printf(TEXT("https://api.epicgames.dev/sanctions/v1/%s/appeals"), *DeploymentId);

	bool bIsFirstParameter = true;
	
	if (Request.Status.IsEmpty() == false)
	{
		FormattedUrl += FString::Printf(TEXT("%sstatus=%s"), bIsFirstParameter ? TEXT("?") : TEXT("&"), *Request.Status);
		bIsFirstParameter = false;
	}

	if (Request.ProductUserId.IsEmpty() == false)
	{
		FormattedUrl += FString::Printf(TEXT("%sproductUserId=%s"), bIsFirstParameter ? TEXT("?") : TEXT("&"), *Request.ProductUserId);
		bIsFirstParameter = false;
	}

	if (Request.ReferenceId.IsEmpty() == false)
	{
		FormattedUrl += FString::Printf(TEXT("%sreferenceId=%s"), bIsFirstParameter ? TEXT("?") : TEXT("&"), *Request.ReferenceId);
		bIsFirstParameter = false;
	}

	if (Request.Limit > 0)
	{
		FormattedUrl += FString::Printf(TEXT("%slimit=%d"), bIsFirstParameter ? TEXT("?") : TEXT("&"), Request.Limit);
		bIsFirstParameter = false;
	}

	if (Request.Offset > 0)
	{
		FormattedUrl += FString::Printf(TEXT("%soffset=%d"), bIsFirstParameter ? TEXT("?") : TEXT("&"), Request.Offset);
		bIsFirstParameter = false;
	}
	
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->AppendToHeader(TEXT("Accept"), TEXT("application/json"));
	HttpRequest->AppendToHeader(TEXT("Authorization"), *FString::Printf(TEXT("Bearer %s"), *AccessToken));
	HttpRequest->SetURL(FormattedUrl);
	HttpRequest->SetVerb("GET");

	HttpRequest->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr HttpRequestPtr, FHttpResponsePtr HttpResponsePtr, bool bConnectedSuccessfully)
	{
		TArray<FQuerySanctionAppealsResult> QuerySanctionAppealsResult;

		int32 TotalResult = 0;
		int32 OffsetResult = 0;
		int32 LimitResult = 0;

		if (bConnectedSuccessfully)
		{
			LogVerbose("QuerySanctionAppeals Response: %s", *HttpResponsePtr->GetContentAsString());

			if (HttpResponsePtr->GetResponseCode() == 200)
			{
				TSharedPtr<FJsonObject> JsonObject;
				TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(HttpResponsePtr->GetContentAsString());

				if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
				{
					if (const TSharedPtr<FJsonValue> Paging = JsonObject->TryGetField(TEXT("paging")))
					{
						if (auto& PagingObject = Paging->AsObject())
						{
							if (TSharedPtr<FJsonValue> TotalObject = PagingObject->TryGetField(TEXT("total")))
							{
								TotalResult = TotalObject->AsNumber();
							}
							if (TSharedPtr<FJsonValue> OffsetObject = PagingObject->TryGetField(TEXT("offset")))
							{
								OffsetResult = OffsetObject->AsNumber();
							}
							if (TSharedPtr<FJsonValue> LimitObject = PagingObject->TryGetField(TEXT("limit")))
							{
								LimitResult = LimitObject->AsNumber();
							}
						}
					}
					
					const TSharedPtr<FJsonValue> ElementsObject = JsonObject->TryGetField(TEXT("elements"));

					if (ElementsObject && ElementsObject->Type == EJson::Array)
					{
						TArray<TSharedPtr<FJsonValue>> ParticipantsArray = ElementsObject->AsArray();

						for (const auto& Element : ParticipantsArray)
						{
							if (Element->Type != EJson::Object) continue;

							auto& Object = Element->AsObject();

							FQuerySanctionAppealsResult Value;

							TSharedPtr<FJsonValue> IdObject = Object->TryGetField(TEXT("id"));
							TSharedPtr<FJsonValue> StatusObject = Object->TryGetField(TEXT("status"));
							TSharedPtr<FJsonValue> ReferenceObject = Object->TryGetField(TEXT("referenceId"));
							TSharedPtr<FJsonValue> CreatedAtObject = Object->TryGetField(TEXT("createdAt"));
							TSharedPtr<FJsonValue> UpdatedAtObject = Object->TryGetField(TEXT("updatedAt"));
							TSharedPtr<FJsonValue> ReasonObject = Object->TryGetField(TEXT("reason"));

							if (IdObject && !IdObject->IsNull())
							{
								Value.Id = IdObject->AsString();
							}

							if (StatusObject && !StatusObject->IsNull())
							{
								Value.Status = StatusObject->AsString();
							}

							if (ReferenceObject && !ReferenceObject->IsNull())
							{
								Value.ReferenceId = ReferenceObject->AsString();
							}

							if (CreatedAtObject && !CreatedAtObject->IsNull())
							{
								Value.CreatedAt = CreatedAtObject->AsString();
							}

							if (ReasonObject && !ReasonObject->IsNull())
							{
								Value.Reason = ReasonObject->AsNumber();
							}

							QuerySanctionAppealsResult.Add(Value);
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

		Callback.ExecuteIfBound((HttpResponsePtr->GetResponseCode() == 200), QuerySanctionAppealsResult, TotalResult, OffsetResult, LimitResult, FWebResponse(HttpResponsePtr->GetResponseCode(), HttpResponsePtr->GetContentAsString()));
	});

	LogVerbose("-------------------------------------------------\n");
	LogVerbose("Sending QuerySanctionAppeals Request (%s)", *HttpRequest->GetURL());
	for (auto& Element : HttpRequest->GetAllHeaders())
	{
		LogVerbose("Header: %s", *Element);
	}
	LogVerbose("-------------------------------------------------\n");

	HttpRequest->ProcessRequest();
}
