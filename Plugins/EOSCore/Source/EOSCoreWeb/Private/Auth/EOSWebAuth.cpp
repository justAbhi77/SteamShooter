/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "EOSWebAuth.h"
#include "EOSCoreWebModule.h"
#include "EOSCoreWebLogging.h"
#include "EOSCoreWebPrivatePCH.h" 
#include "Shared/EOSWebShared.h"

void UEOSWebAuthLibrary::RequestAuthAccessToken(const FAuthAccessTokenCallbackDelegate& Callback, FRequestAuthAccessTokenRequest Request)
{
	LogVerbose("");

	if (Request.GrantType == EWebGrantType::NOT_SET)
	{
		LogError("GrantType must be set");
		return;
	}

	if (Request.DeploymentId.IsEmpty())
	{
		LogError("DeploymentId cannot be empty");
		return;
	}

	auto HttpRequest = UEOSWebShared::CreateRequest();

	FString Base64Credentials;
	FString GrantTypeString;

	if (!Request.Credentials.ClientId.IsEmpty() && !Request.Credentials.ClientSecret.IsEmpty())
	{
		Base64Credentials = FBase64::Encode(FString::Printf(TEXT("%s:%s"), *Request.Credentials.ClientId, *Request.Credentials.ClientSecret));
	}

	switch (Request.GrantType)
	{
	case EWebGrantType::ExchangeCode:
		GrantTypeString = "exchange_code";
		break;
	case EWebGrantType::Password:
		GrantTypeString = "password";
		break;
	case EWebGrantType::RefreshToken:
		GrantTypeString = "refresh_token";
		break;
	case EWebGrantType::ClientCredentials:
		GrantTypeString = "client_credentials";
		break;
	case EWebGrantType::AuthorizationCode:
		GrantTypeString = "authorization_code";
		break;
	default: ;
	}

	FString ContentString = FString::Printf(TEXT("grant_type=%s"), *GrantTypeString);

	if (!Request.DeploymentId.IsEmpty())
	{
		ContentString += FString::Printf(TEXT("&deployment_id=%s"), *Request.DeploymentId);
	}

	if (!Request.Scope.IsEmpty())
	{
		ContentString += FString::Printf(TEXT("&scope=%s"), *Request.Scope);
	}

	if (!Request.Username.IsEmpty())
	{
		ContentString += FString::Printf(TEXT("&username=%s"), *Request.Username);
	}

	if (!Request.Password.IsEmpty())
	{
		ContentString += FString::Printf(TEXT("&password=%s"), *Request.Password);
	}

	if (!Request.ExchangeCode.IsEmpty())
	{
		ContentString += FString::Printf(TEXT("&exchange_code=%s"), *Request.ExchangeCode);
	}

	if (!Request.Code.IsEmpty())
	{
		ContentString += FString::Printf(TEXT("&code=%s"), *Request.Code);
	}

	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	if (!Base64Credentials.IsEmpty())
	{
		HttpRequest->AppendToHeader(TEXT("Authorization"), *FString::Printf(TEXT("Basic %s"), *Base64Credentials));
	}
	HttpRequest->SetContentAsString(ContentString);
	HttpRequest->SetURL("https://api.epicgames.dev/epic/oauth/v1/token");
	HttpRequest->SetVerb("POST");

	HttpRequest->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr HttpRequestPtr, FHttpResponsePtr HttpResponsePtr, bool bConnectedSuccessfully)
	{
		FString AccessTokenString;
		FString ExpiresInString;
		FString ExpiresAtString;
		FString AccountIdString;
		FString ClientIdString;
		FString ApplicationIdString;
		FString TokenIdString;
		FString RefreshTokenString;
		FString RefreshExpiresString;
		FString RefreshExpiresAtString;

		if (bConnectedSuccessfully)
		{
			LogVeryVerbose("RequestAuthAccessToken Response: %s", *HttpResponsePtr->GetContentAsString());

			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(HttpResponsePtr->GetContentAsString());

			if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
			{
				TSharedPtr<FJsonValue> AccessTokenObject = JsonObject->TryGetField(TEXT("access_token"));
				TSharedPtr<FJsonValue> ExpiresInObject = JsonObject->TryGetField(TEXT("expires_in"));
				TSharedPtr<FJsonValue> ExpiresAtObject = JsonObject->TryGetField(TEXT("expires_at"));
				TSharedPtr<FJsonValue> AccountIdObject = JsonObject->TryGetField(TEXT("account_id"));
				TSharedPtr<FJsonValue> ClientIdObject = JsonObject->TryGetField(TEXT("client_id"));
				TSharedPtr<FJsonValue> ApplicationIdObject = JsonObject->TryGetField(TEXT("application_id"));
				TSharedPtr<FJsonValue> TokenIdObject = JsonObject->TryGetField(TEXT("token_type"));
				TSharedPtr<FJsonValue> RefreshTokenObject = JsonObject->TryGetField(TEXT("refresh_token"));
				TSharedPtr<FJsonValue> RefreshExpiresObject = JsonObject->TryGetField(TEXT("expires_at"));
				TSharedPtr<FJsonValue> RefreshExpiresAtObject = JsonObject->TryGetField(TEXT("refresh_expires_at"));

				if (AccessTokenObject)
				{
					AccessTokenString = AccessTokenObject->AsString();
				}
				if (ExpiresInObject)
				{
					ExpiresInString = ExpiresInObject->AsString();
				}
				if (ExpiresAtObject)
				{
					ExpiresAtString = ExpiresAtObject->AsString();
				}
				if (AccountIdObject)
				{
					AccountIdString = AccountIdObject->AsString();
				}
				if (ClientIdObject)
				{
					ClientIdString = ClientIdObject->AsString();
				}
				if (ApplicationIdObject)
				{
					ApplicationIdString = ApplicationIdObject->AsString();
				}
				if (TokenIdObject)
				{
					TokenIdString = TokenIdObject->AsString();
				}
				if (RefreshTokenObject)
				{
					RefreshTokenString = RefreshTokenObject->AsString();
				}
				if (RefreshExpiresObject)
				{
					RefreshExpiresString = RefreshExpiresObject->AsString();
				}
				if (RefreshExpiresAtObject)
				{
					RefreshExpiresAtString = RefreshExpiresAtObject->AsString();
				}
			}
		}
		else
		{
			LogError("Connection failed");
		}

		if (bConnectedSuccessfully && !AccessTokenString.IsEmpty())
		{
			Callback.ExecuteIfBound(true, FAuthAccessTokenCallbackData(AccessTokenString, ExpiresInString, ExpiresAtString, AccountIdString, ClientIdString, ApplicationIdString, TokenIdString, RefreshTokenString, RefreshExpiresString, RefreshExpiresAtString), FWebResponse(HttpResponsePtr->GetResponseCode(), HttpResponsePtr->GetContentAsString()));
		}
		else
		{
			Callback.ExecuteIfBound(false, FAuthAccessTokenCallbackData(), FWebResponse(HttpResponsePtr->GetResponseCode(), HttpResponsePtr->GetContentAsString()));
		}
	});

	LogVerbose("-------------------------------------------------\n");
	LogVerbose("Sending Request Auth AccessToken (%s)", *HttpRequest->GetURL());
	for (auto& Element : HttpRequest->GetAllHeaders())
	{
		LogVerbose("Header: %s", *Element);
	}
	LogVerbose("Content: %s", *ContentString);
	LogVerbose("-------------------------------------------------\n");

	HttpRequest->ProcessRequest();
}

void UEOSWebAuthLibrary::InitiateEpicAuthentication(const FInitiateEpicAuthenticationCallbackDelegate& Callback, FInitiateEpicAuthenticationRequest Request)
{
	LogVerbose("");

	if (Request.ClientId.IsEmpty())
	{
		LogError("ClientId cannot be empty");
		return;
	}

	if (Request.RedirectURI.IsEmpty())
	{
		LogError("RedirectURI cannot be empty");
		return;
	}

	if (Request.Scope.IsEmpty())
	{
		LogError("Scope cannot be empty");
		return;
	}

	if (Request.Timeout < 30)
	{
		LogError("Timeout cannot be < 30 seconds");
		return;
	}

	const FDateTime StartTime = FDateTime::UtcNow();

	FString BrowserError;
	FPlatformProcess::LaunchURL(*FString::Printf(TEXT("https://www.epicgames.com/id/authorize?client_id=%s&redirect_uri=%s&response_type=code&scope=%s"), *Request.ClientId, *Request.RedirectURI, *Request.Scope), TEXT(""), &BrowserError);

	if (BrowserError.IsEmpty())
	{
		const auto TickerDelegate = FTickerDelegate::CreateLambda([=](float)
		{
			bool bFoundAuthenticationCode = false;
			
			FString ErrorMessage;
			const FDateTime CurrentTime = FDateTime::UtcNow();
			FString AuthenticationCode;
			const int32 Duration = (CurrentTime - StartTime).GetTotalSeconds();

			if (Duration >= Request.Timeout)
			{
				ErrorMessage = "Timed out";
			}

			if (ErrorMessage.IsEmpty())
			{
				LogVerbose("Finding Authentication Code... (%d)", Duration);
				
				FString Title;
				if (FPlatformApplicationMisc::GetWindowTitleMatchingText(*Request.RedirectURI, Title))
				{
					if (FParse::Value(*Title, TEXT("code="), AuthenticationCode))
					{
						LogVerbose("Found AuthenticationCode: %s", *AuthenticationCode);

						bFoundAuthenticationCode = true;
					}
				}
			}
			else
			{
				LogError("%s", *ErrorMessage);
			}

			if (bFoundAuthenticationCode || !ErrorMessage.IsEmpty())
			{
				Callback.ExecuteIfBound(ErrorMessage.IsEmpty(), AuthenticationCode, ErrorMessage);

				return false;
			}

			return true;
		});

#if UE_VERSION_NEWER_THAN(4,27,2)
		FTSTicker::GetCoreTicker().AddTicker(TickerDelegate, 1.0f);
#else
		FTicker::GetCoreTicker().AddTicker(TickerDelegate, 1.0f);
#endif
	}
	else
	{
		Callback.ExecuteIfBound(false, "", BrowserError);
	}
}
