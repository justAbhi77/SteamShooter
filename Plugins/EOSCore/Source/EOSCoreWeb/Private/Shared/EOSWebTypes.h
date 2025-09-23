/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "EOSWebTypes.generated.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		ENUMS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UENUM(BlueprintType)
enum class EWebGrantType : uint8
{
	NOT_SET,
	ExchangeCode,
	Password,
	RefreshToken,
	AuthorizationCode,
	ClientCredentials
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		STRUCTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

USTRUCT(BlueprintType)
struct FWebClientCredentials
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ClientId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ClientSecret;
};

USTRUCT(BlueprintType)
struct FWebRequestParticipantData
{
	GENERATED_BODY()

public:
	explicit FWebRequestParticipantData()
		: ClientIp("10.0.0.1")
		, bHardMuted(false)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ProductUserId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ClientIp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	bool bHardMuted;
};

USTRUCT(BlueprintType)
struct FCreateRoomTokenCallback
{
	GENERATED_BODY()

public:
	explicit FCreateRoomTokenCallback() = default;

	FCreateRoomTokenCallback(FString roomName, FString deploymentId, FString clientBaseUrl, FString token, FString puid)
		: RoomName(roomName)
		, DeploymentId(deploymentId)
		, ClientBaseUrl(clientBaseUrl)
		, Token(token)
		, Puid(puid)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString RoomName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString DeploymentId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ClientBaseUrl;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Token;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Puid;
};

USTRUCT(BlueprintType)
struct FAccessTokenCallbackData
{
	GENERATED_BODY()

public:
	FAccessTokenCallbackData()
		: ExpiresIn(0)
	{
	}

	FAccessTokenCallbackData(FString accessToken, FString tokenType, FString expiresAt, FString expiresIn, FString nonce, TArray<FString> features, FString organizationId, FString productId, FString sandboxId, FString deploymentId)
		: AccessToken(accessToken)
		, TokenType(tokenType)
		, ExpiresAt(expiresAt)
		, ExpiresIn(FCString::Atoi(*expiresIn))
		, Features(features)
		, OrganizationId(organizationId)
		, ProductId(productId)
		, SandboxId(sandboxId)
		, DeploymentId(deploymentId)
	{
	}

public:
	/*
	* The generated EOS Client Token as Base64Url encoded string.
	* The token describes the verified client caller.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString AccessToken;

	// Always set to bearer
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString TokenType;

	// Token expiration time. Contains a NumericDate number value, describing the time point in seconds from the Unix epoch.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ExpiresAt;

	// Token lifetime. Seconds since the issue time to when the token will expire.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	int32 ExpiresIn;

	/*
	* An arbitrary string value provided by the client.
	* Used by the client for added security:
	* When receiving an access token in HTTP response, the client can verify that the token response includes the correct nonce value.
	 * 
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Nonce;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	TArray<FString> Features;

	// Your organization identifier.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString OrganizationId;

	// Your product identifier.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ProductId;

	// Your sandbox identifier.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString SandboxId;

	// Your deployment identifier.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString DeploymentId;
};

USTRUCT(BlueprintType)
struct FQueryAntiCheatServiceStatusByDeploymentCallbackData
{
	GENERATED_BODY()

public:
	FQueryAntiCheatServiceStatusByDeploymentCallbackData()
		: bServerKick(false)
	{
	}

	FQueryAntiCheatServiceStatusByDeploymentCallbackData(bool bInServerKick)
		: bServerKick(bInServerKick)
	{
	}

public:
	/*
	 * True if anti-cheat Server Kicks are enabled in the EOS Developer Portal, otherwise false.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	bool bServerKick;
};

USTRUCT(BlueprintType)
struct FQueryActiveSanctionsForSpecificPlayerCallbackData
{
	GENERATED_BODY()

public:
	FQueryActiveSanctionsForSpecificPlayerCallbackData()
		: Timestamp(0)
		, ExpirationTimestamp(0)
	{
	}

	FQueryActiveSanctionsForSpecificPlayerCallbackData(FString InReferenceIdString, int64 InTimestamp, FString InActionString, int64 InExpirationTimestamp)
		: ReferenceIdString(InReferenceIdString)
		, Timestamp(InTimestamp)
		, ActionString(InActionString)
		, ExpirationTimestamp(InExpirationTimestamp)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ReferenceIdString;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	int64 Timestamp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ActionString;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	int64 ExpirationTimestamp;
};

USTRUCT(BlueprintType)
struct FSynchronizeSanctionsToAnExternalServiceCallbackData
{
	GENERATED_BODY()

public:
	FSynchronizeSanctionsToAnExternalServiceCallbackData()
		: EventType(0)
		, bPending(false)
		, bAutomated(false)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ProductUserId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Action;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Justification;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Source;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	int32 EventType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	TArray<FString> Tags;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString LogId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ReferenceId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Timestamp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ExpirationTimestamp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString BatchUuid;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString EpicAccountName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString EpicAccountId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString EosClientId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString EosClientRole;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString CreatedAt;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString UpdatedAt;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString TrustedPartner;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString DeploymentId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	bool bPending;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	bool bAutomated;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString DisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString IdentityProvider;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString AccountId;
};


USTRUCT(BlueprintType)
struct FAuthAccessTokenCallbackData
{
	GENERATED_BODY()

public:
	FAuthAccessTokenCallbackData()
		: ExpiresIn(0)
	{
	}

	FAuthAccessTokenCallbackData(FString accessToken, FString expiresIn, FString expiresAt, FString accountId, FString clientId, FString applicationId, FString tokenId, FString refreshToken, FString refreshExpires, FString refreshExpiresAt)
		: AccessToken(accessToken)
		, ExpiresIn(FCString::Atoi(*expiresIn))
		, ExpiresAt(expiresAt)
		, AccountId(accountId)
		, ClientId(clientId)
		, ApplicationId(applicationId)
		, TokenId(tokenId)
		, RefreshToken(refreshToken)
		, RefreshExpires(refreshExpires)
		, RefreshExpiresAt(refreshExpiresAt)
	{
	}

public:
	/*
	* The access token, which may optionally have a prefix (for example: eg1~[token]). This value should be passed as is in the Authorization header using the Bearer type on any requests to Epic services.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString AccessToken;

	// The number of seconds until the token expires.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	int32 ExpiresIn;

	// The expire date in ISO 8601 format.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ExpiresAt;

	// The Epic Account ID for the user that the token was generated for.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString AccountId;

	// The Client ID which was used to generate this token.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ClientId;

	// The Application ID which the Client is associated with.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ApplicationId;

	// The type of token generated; value will always be bearer.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString TokenId;

	// The refresh token will optionally be returned depending on the client configuration. This refresh token can be used to extend a session before or after the access token has expired.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString RefreshToken;

	// The number of seconds until the refresh token expires.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString RefreshExpires;

	// The refresh token expire date in ISO 8601 format.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString RefreshExpiresAt;
};

USTRUCT(BlueprintType)
struct FWebResponse
{
	GENERATED_BODY()

public:
	FWebResponse()
		: Code(0)
	{
	}

	FWebResponse(int32 code, FString message)
		: Code(code)
		, Message(message)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	int32 Code;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Message;
};

UENUM(BlueprintType)
enum class ESubjectType : uint8
{
	NOT_SET,
	BAN_APPEAL,
	CHEAT_REPORT,
	OPEN_QUESTION,
	TECHNICAL_SUPPORT,
};

USTRUCT(BlueprintType)
struct FSubmitTicketRequest
{
	GENERATED_BODY()

public:
	FSubmitTicketRequest()
		: Subject(ESubjectType::NOT_SET)
	{
	}

	/*
	* One of the following:
	* ban-appeal
	* cheat-report
	* open-question
	* technical-support
	*
	* Required: YES
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	ESubjectType Subject;
	/*
	 * The email text, written by the end-user
	 *
	 * Required: YES
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Message;
	/*
	 * The end-user's email address
	 *
	 * Required: YES
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString SenderEmail;

	// The end-user's name
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString SenderName;

	// Your game's unique identifier for the end-user (e.g. the productUserId)
	// Note: Any type of unique identifier works, there are no restrictions enforced regarding the guid.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString GUID;

	// An error code for technical support emails
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ErrorCode;

	// The end-user's operating system
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString SystemOS;

	// The end-user's anti-malware system
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString SystemAntiMalware;

	// Other information about the end-user's system
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString SystemOther;
};

USTRUCT(BlueprintType)
struct FInitiateEpicAuthenticationRequest
{
	GENERATED_BODY()

	// The OAuth Client ID for your application.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ClientId;

	// After the user logs in, they will be redirected back to the configured redirect URL with an authorization code
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString RedirectURI;

	// Space-delimited list of scopes (permissions) that will be requested from the user. For example: basic profile, friends_list, and presence
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Scope;

	// Timeout in seconds before timing out
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	int32 Timeout;

	FInitiateEpicAuthenticationRequest()
		: RedirectURI("https://eeldev.com/auth/")
		, Scope("basic_profile friends_list presence")
		, Timeout(60)
	{
	}
};

USTRUCT(BlueprintType)
struct FRequestAccessTokenRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FWebClientCredentials Credentials;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString DeploymentId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString AdditionalData;
};

USTRUCT(BlueprintType)
struct FRequestAuthAccessTokenRequest
{
	GENERATED_BODY()

	FRequestAuthAccessTokenRequest()
		: GrantType(EWebGrantType::NOT_SET)
		, Scope("basic_profile friends_list presence")
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	EWebGrantType GrantType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString DeploymentId;

	// For the password grant type
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Username;

	// For the password grant type
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Password;

	// For the exchange_code grant type
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ExchangeCode;

	// For the authorization_code grant type
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Code;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FWebClientCredentials Credentials;

	// Space-delimited list of scopes (permissions) that will be requested from the user. For example: basic profile, friends_list, and presence
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Scope;
};

USTRUCT(BlueprintType)
struct FSubmitTicketResponse
{
	GENERATED_BODY()
	FSubmitTicketResponse() = default;

	FSubmitTicketResponse(FString prodName, FString prodSlug, FString guid, FString senderName, FString senderEmail, FString subject, FString message, FString errorCode, FString systemOS, FString systemAntiMalware, FString systemOther, FString timestamp)
		: ProdName(prodName)
		, ProdSlug(prodSlug)
		, GUID(guid)
		, SenderName(senderName)
		, SenderEmail(senderEmail)
		, Subject(subject)
		, Message(message)
		, ErrorCode(errorCode)
		, SystemOS(systemOS)
		, SystemAntiMalware(systemAntiMalware)
		, SystemOther(systemOther)
		, Timestamp(timestamp)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ProdName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ProdSlug;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString GUID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString SenderName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString SenderEmail;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Subject;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Message;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ErrorCode;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString SystemOS;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString SystemAntiMalware;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString SystemOther;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Timestamp;
};

USTRUCT(BlueprintType)
struct FSanctionPostPayload
{
	GENERATED_BODY()

	FSanctionPostPayload()
		: bPending(false)
		, Duration(0)
	{
	}

	// Sanctioned user’s EOS ProductUserId
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ProductUserId;

	// Action string associated with this sanction. Format: [a-zA-Z0-9_-]+. Min length: 1. Max length: 64.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Action;

	// Justification string associated with this sanction. Min length: 1 Max length: 2048
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Justification;

	// Source which created this sanction, e.g. developer-portal. Format: [a-zA-Z0-9_-]+. Min length: 2. Max length: 64.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Source;

	// List of tags associated with this sanction. Items are case insensitive and unique. Item format: [a-zA-Z0-9_-]+. Item max length: 16
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	TArray<FString> Tags;

	// True if this sanction is currently pending
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	bool bPending;

	// Display name of sanctioned user. Max length: 64
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString DisplayName;

	// Identity provider that the sanctioned user authenticated with. Max length: 64
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString IdentityProvider;

	// Sanctioned user's account ID with the specified identityProvider. Max length: 64
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString AccountId;

	// The length of the sanction in seconds
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	int32 Duration;
};

USTRUCT(BlueprintType)
struct FSanction
{
	GENERATED_BODY()
	FSanction()
		: bPending(false)
		, bAutomated(false)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ProductUserId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Action;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Justification;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Source;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	TArray<FString> Tags;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ReferenceId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Timestamp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ExpirationTimestamp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString BatchUuid;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString EpicAccountName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString EpicAccountId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString EosClientId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString EosClientRole;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString CreatedAt;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString UpdatedAt;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString RemovedAt;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString TrustedPartner;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString DeploymentId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	bool bPending;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	bool bAutomated;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString DisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString IdentityProvider;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString AccountId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Status;
};

USTRUCT(BlueprintType)
struct FQuerySanctionsResult
{
	GENERATED_BODY()
	FQuerySanctionsResult()
		: bPending(false)
		, bAutomated(false)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ProductUserId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Action;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Justification;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Source;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	TArray<FString> Tags;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ReferenceId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Timestamp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ExpirationTimestamp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString BatchUuid;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString EpicAccountName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString EpicAccountId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString EosClientId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString EosClientRole;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString CreatedAt;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString UpdatedAt;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString RemovedAt;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString TrustedPartner;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString DeploymentId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	bool bPending;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	bool bAutomated;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString DisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString IdentityProvider;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString AccountId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Status;
};

USTRUCT(BlueprintType)
struct FAppealPostPayload
{
	GENERATED_BODY()
	FAppealPostPayload()
		: Reason(0)
	{
	}

	// Unique identifier for the sanction being appealed.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ReferenceId;

	// The reason code for the appeal.
	// 1: Incorrect sanction.
	// 2: Compromised account.
	// 3: Unfair punishment.
	// 4: Appeal for forgiveness.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	int32 Reason;
};

USTRUCT(BlueprintType)
struct FAppealResponse
{
	GENERATED_BODY()
	FAppealResponse()
		: Reason(0)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Id;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Status;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ReferenceId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString CreatedAt;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString UpdatedAt;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	int32 Reason;
};

USTRUCT(BlueprintType)
struct FQuerySanctionAppeals
{
	GENERATED_BODY()
	FQuerySanctionAppeals()
		: Limit(100)
		, Offset(0)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	int32 Limit;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	int32 Offset;

	// The sanction identifier filter. Multiple allowed.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ReferenceId;

	// The appeal status filter. Multiple allowed.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Status;

	// 	The product user id filter. Multiple allowed.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ProductUserId;
};

USTRUCT(BlueprintType)
struct FQuerySanctionAppealsResult
{
	GENERATED_BODY()
	FQuerySanctionAppealsResult()
		: Reason(0)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Id;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Status;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString ReferenceId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString CreatedAt;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString UpdatedAt;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	int32 Reason;
};


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		DELEGATES
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FAccessTokenCallbackDelegate, bool, bWasSuccessful, const FAccessTokenCallbackData&, Data, const FWebResponse&, WebResponse);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FAuthAccessTokenCallbackDelegate, bool, bWasSuccessful, const FAuthAccessTokenCallbackData&, Data, const FWebResponse&, WebResponse);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FCreateRoomTokenCallbackDelegate, bool, bWasSuccessful, const FCreateRoomTokenCallback&, Data, const FWebResponse&, WebResponse);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FRemoveParticipantCallbackDelegate, bool, bWasSuccessful, const FWebResponse&, WebResponse);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FModifyParticipantCallbackDelegate, bool, bWasSuccessful, const FWebResponse&, WebResponse);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FSubmitTicketCallbackDelegate, bool, bWasSuccessful, const FSubmitTicketResponse&, Response, const FWebResponse&, WebResponse);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FInitiateEpicAuthenticationCallbackDelegate, bool, bWasSuccessful, const FString&, AuthenticationCode, const FString&, ErrorMessage);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnPublicIpResponse, const FString&, Response);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FAntiCheatStatusCallbackDelegate, bool, bWasSuccessful, const FQueryAntiCheatServiceStatusByDeploymentCallbackData&, Data, const FWebResponse&, WebResponse);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FSanctionsQueryActiveSanctionsForSpecificPlayerCallbackDelegate, bool, bWasSuccessful, const TArray<FQueryActiveSanctionsForSpecificPlayerCallbackData>&, Data, const FWebResponse&, WebResponse);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FSynchronizeSanctionsToAnExternalServiceCallbackDelegate, bool, bWasSuccessful, const TArray<FSynchronizeSanctionsToAnExternalServiceCallbackData>&, Data, const FWebResponse&, WebResponse);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FCreateSanctionsCallbackDelegate, bool, bWasSuccessful, const TArray<FSanction>&, Data, const FWebResponse&, WebResponse);
DECLARE_DYNAMIC_DELEGATE_SixParams(FQuerySanctionsCallbackDelegate, bool, bWasSuccessful, const TArray<FQuerySanctionsResult>&, Data, const int32&, Total, const int32&, Offset, const int32&, Limit, const FWebResponse&, WebResponse);
DECLARE_DYNAMIC_DELEGATE_SixParams(FQuerySanctionsForPlayerCallbackDelegate, bool, bWasSuccessful, const TArray<FQuerySanctionsResult>&, Data, const int32&, Total, const int32&, Offset, const int32&, Limit, const FWebResponse&, WebResponse);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FRemoveSanctionCallbackDelegate, bool, bWasSuccessful, const FWebResponse&, WebResponse);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FCreateSanctionAppealCallbackDelegate, bool, bWasSuccessful, const FAppealResponse&, Data, const FWebResponse&, WebResponse);
DECLARE_DYNAMIC_DELEGATE_SixParams(FQuerySanctionAppealsCallbackDelegate, bool, bWasSuccessful, const TArray<FQuerySanctionAppealsResult>&, Data, const int32&, Total, const int32&, Offset, const int32&, Limit, const FWebResponse&, WebResponse);