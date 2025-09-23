/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Core/EOSHelpers.h"
#include "eos_auth_types.h"
#include "Connect/EOSConnectTypes.h"
#include "EOSAuthTypes.generated.h"

class UCoreAuthentication;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		ENUMS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

/**
 * Flags used to describe how the account linking operation is to be performed.
 *
 * @see EOS_Auth_LinkAccount
 */
UENUM(BlueprintType)
enum class EEOSELinkAccountFlags : uint8
{
	/**
	 * Default flag used for a standard account linking operation.
	 *
	 * This flag is set when using a continuance token received from a previous call to the EOS_Auth_Login API,
	 * when the local user has not yet been successfully logged in to an Epic Account yet.
	 */
	EOS_LA_NoFlags = 0x0,
	/**
	 * Specified when the EOS_ContinuanceToken describes a Nintendo NSA ID account type.
	 *
	 * This flag is used only with, and must be set, when the continuance token was received from a previous call
	 * to the EOS_Auth_Login API using the EOS_EExternalCredentialType::EOS_ECT_NINTENDO_NSA_ID_TOKEN login type.
	 */
	EOS_LA_NintendoNsaId = 0x1
};

/**
 * All possible types of login methods, availability depends on permissions granted to the client
 *
 * @see EOS_Auth_Login
 * @see EOS_Auth_Credentials
 */
UENUM(BlueprintType)
enum class EEOSLoginCredentialType : uint8
{
	/**
 * Login using account email address and password.
 *
 * @note Use of this login method is restricted and cannot be used in general.
 */
	EOS_LCT_Password = 0,
	/**
	 * A short-lived one-time use exchange code to login the local user.
	 *
	 * @details Typically retrieved via command-line parameters provided by a launcher that generated the exchange code for this application.
	 * When started, the application is expected to consume the exchange code by using the EOS_Auth_Login API as soon as possible.
	 * This is needed in order to authenticate the local user before the exchange code would expire.
	 * Attempting to consume an already expired exchange code will return EOS_EResult::EOS_Auth_ExchangeCodeNotFound error by the EOS_Auth_Login API.
	 */
	EOS_LCT_ExchangeCode = 1,
	/**
	 * Desktop and Mobile only; deprecated on Console platforms in favor of EOS_LCT_ExternalAuth login method.
	 * Used by standalone applications distributed outside the supported game platforms such as Epic Games Store or Steam.
	 *
	 * Persistent Auth is used in conjuction with the EOS_LCT_AccountPortal login method for automatic login of the local user across multiple runs of the application.
	 *
	 * Standalone applications implement the login sequence as follows:
	 * 1. Application calls EOS_Auth_Login with EOS_LCT_PersistentAuth to attempt automatic login.
	 * 2. If automatic login fails, the application calls EOS_Auth_Login with EOS_LCT_AccountPortal to prompt the user for manual login.
	 *
	 * @note On Desktop and Mobile platforms, the persistent refresh token is automatically managed by the SDK that stores it in the keychain of the currently logged in user of the local device.
	 * On Console platforms, after a successful login the refresh token must be retrieved using the EOS_Auth_CopyUserAuthToken API and stored by the application for the currently logged in user of the local device.
	 *
	 * @see EOS_LCT_AccountPortal
	 */
	EOS_LCT_PersistentAuth = 2,
	/**
	 * Deprecated and no longer used. Superseded by the EOS_LCT_ExternalAuth login method.
	 *
	 * Initiates a PIN grant login flow that is used to login a local user to their Epic Account for the first time,
	 * and also whenever their locally persisted login credentials would have expired.
	 *
	 * @details The flow is as following:
	 * 1. Game initiates the user login flow by calling EOS_Auth_Login API with the EOS_LCT_DeviceCode login type.
	 * 2. The SDK internally requests the authentication backend service to begin the login flow, and returns the game
	 * a new randomly generated device code along with authorization URL information needed to complete the flow.
	 * This information is returned via the EOS_Auth_Login API callback. The EOS_Auth_LoginCallbackInfo::ResultCode
	 * will be set to EOS_Auth_PinGrantCode and the EOS_Auth_PinGrantInfo struct will contain the needed information.
	 * 3. Game presents the device code and the authorization URL information on screen to the end-user.
	 * 4. The user will login to their Epic Account using an external device, e.g. a mobile device or a desktop PC,
	 * by browsing to the presented authentication URL and entering the device code presented by the game on the console.
	 * 5. Once the user has successfully logged in on their external device, the SDK will call the EOS_Auth_Login callback
	 * once more with the operation result code. If the user failed to login within the allowed time before the device code
	 * would expire, the result code returned by the callback will contain the appropriate error result.
	 *
	 * @details After logging in a local user for the first time, the game can remember the user login to allow automatically logging
	 * in the same user the next time they start the game. This avoids prompting the same user to go through the login flow
	 * across multiple game sessions over long periods of time.
	 * To do this, after a successful login using the EOS_LCT_DeviceCode login type, the game can call the EOS_Auth_CopyUserAuthToken API
	 * to retrieve a long-lived refresh token that is specifically created for this purpose on Console. The game can store
	 * the long-lived refresh token locally on the device, for the currently logged in local user of the device.
	 * Then, on subsequent game starts the game can call the EOS_Auth_Login API with the previously stored refresh token and
	 * using the EOS_LCT_PersistentAuth login type to automatically login the current local user of the device.
	 *
	 * @see EOS_LCT_ExternalAuth
	 */
	EOS_LCT_DeviceCode = 3,
	/**
	 * Login with named credentials hosted by the EOS SDK Developer Authentication Tool.
	 *
	 * @note Used for development purposes only.
	 */
	EOS_LCT_Developer = 4,
	/**
	 * Refresh token that was retrieved from a previous call to EOS_Auth_Login API in another local process context.
	 * Mainly used in conjunction with custom desktop launcher applications.
	 *
	 * @details Can be used for example when launching the game from Epic Games Launcher and having an intermediate process
	 * in-between that requires authenticating the user before eventually starting the actual game client application.
	 * In such scenario, an intermediate launcher will log in the user by consuming the exchange code it received from the
	 * Epic Games Launcher. To allow the game client to also authenticate the user, it can copy the refresh token using the
	 * EOS_Auth_CopyUserAuthToken API and pass it via launch parameters to the started game client. The game client can then
	 * use the refresh token to log in the user.
	 */
	EOS_LCT_RefreshToken = 5,
	/**
	 * Desktop and Mobile only.
	 * Used by standalone applications distributed outside the supported game platforms such as Epic Games Store or Steam.
	 *
	 * Login using the built-in user onboarding experience provided by the SDK, which will automatically store a persistent
	 * refresh token to enable automatic user login for consecutive application runs on the local device. Applications are
	 * expected to attempt automatic login using the EOS_LCT_PersistentAuth login method, and fall back to EOS_LCT_AccountPortal
	 * to prompt users for manual login.
	 *
	 * @note On Windows, using this login method requires applications to be started through the EOS Bootstrapper application
	 * and to have the local Epic Online Services redistributable installed on the local system. See EOS_Platform_GetDesktopCrossplayStatus
	 * for adding a readiness check prior to calling EOS_Auth_Login.
	 */
	EOS_LCT_AccountPortal = 6,
	/**
	 * Login using external account provider credentials, such as PlayStation(TM)Network, Steam, and Xbox Live.
	 *
	 * This is the intended login method on Console. On Desktop and Mobile, used when launched through any of the commonly supported platform clients.
	 *
	 * @details The user is seamlessly logged in to their Epic account using an external account access token.
	 * If the local platform account is already linked with the user's Epic account, the login will succeed and EOS_EResult::EOS_Success is returned.
	 * When the local platform account has not been linked with an Epic account yet,
	 * EOS_EResult::EOS_InvalidUser is returned and the EOS_ContinuanceToken will be set in the EOS_Auth_LoginCallbackInfo data.
	 * If EOS_EResult::EOS_InvalidUser is returned,
	 * the application should proceed to call the EOS_Auth_LinkAccount API with the EOS_ContinuanceToken to continue with the external account login
	 * and to link the external account at the end of the login flow.
	 *
	 * @details On Console, login flow when the platform user account has not been linked with an Epic account yet:
	 * 1. Game calls EOS_Auth_Login with the EOS_LCT_ExternalAuth credential type.
	 * 2. EOS_Auth_Login returns EOS_EResult::EOS_InvalidUser with a non-null EOS_ContinuanceToken in the EOS_Auth_LoginCallbackInfo data.
	 * 3. Game calls EOS_Auth_LinkAccount with the EOS_ContinuanceToken to initiate the login flow for linking the platform account with the user's Epic account.
	 *    - During the login process, the user will be able to login to their existing Epic account or create a new account if needed.
	 * 4. EOS_Auth_LinkAccount will make an intermediate callback to provide the caller with EOS_Auth_PinGrantInfo struct set in the EOS_Auth_LoginCallbackInfo data.
	 * 5. Game examines the retrieved EOS_Auth_PinGrantInfo struct for a website URI and user code that the user needs to access off-device via a PC or mobile device.
	 *    - Game visualizes the URI and user code so that the user can proceed with the login flow outside the console.
	 *    - In the meantime, EOS SDK will internally keep polling the backend for a completion status of the login flow.
	 * 6. Once user completes the login, cancels it or if the login flow times out, EOS_Auth_LinkAccount makes the second and final callback to the caller with the operation result status.
	 *    - If the user was logged in successfully, EOS_EResult::EOS_Success is returned in the EOS_Auth_LoginCallbackInfo. Otherwise, an error result code is returned accordingly.
	 *
	 * @details On Desktop and Mobile, login flow when the platform user account has not been linked with an Epic account yet:
	 * 1. Game calls EOS_Auth_Login with the EOS_LCT_ExternalAuth credential type.
	 * 2. EOS_Auth_Login returns EOS_EResult::EOS_InvalidUser with a non-null EOS_ContinuanceToken in the EOS_Auth_LoginCallbackInfo data.
	 * 3. Game calls EOS_Auth_LinkAccount with the EOS_ContinuanceToken to initiate the login flow for linking the platform account with the user's Epic account.
	 * 4. EOS SDK automatically opens the local default web browser and takes the user to the Epic account portal web page.
	 *    - The user is able to login to their existing Epic account or create a new account if needed.
	 *    - In the meantime, EOS SDK will internally keep polling the backend for a completion status of the login flow.
	 * 5. Once user completes the login, cancels it or if the login flow times out, EOS_Auth_LinkAccount invokes the completion callback to the caller.
	 *    - If the user was logged in successfully, EOS_EResult::EOS_Success is returned in the EOS_Auth_LoginCallbackInfo. Otherwise, an error result code is returned accordingly.
	 *
	 * @note On Windows, using this login method requires applications to be started through the EOS Bootstrapper application
	 * and to have the local Epic Online Services redistributable installed on the local system. See EOS_Platform_GetDesktopCrossplayStatus
	 * for adding a readiness check prior to calling EOS_Auth_Login.
	 */
	EOS_LCT_ExternalAuth = 7
};

/**
 * Types of auth tokens
 *
 * @see EOS_Auth_CopyUserAuthToken
 * @see EOS_Auth_Token
 */
UENUM(BlueprintType)
enum class EOSEAuthTokenType : uint8
{
	/** Auth token is for a validated client */
	EOS_ATT_Client = 0,
	/** Auth token is for a validated user */
	EOS_ATT_User = 1
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		STRUCTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

/** A structure that contains an auth token. These structures are created by EOS_Auth_CopyUserAuthToken and must be passed to EOS_Auth_Token_Release. */
USTRUCT(BlueprintType)
struct FEOSAuthToken
{
	GENERATED_BODY()
public:
	/** Name of the app related to the client id involved with this token */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FString App;
	/** Client id that requested this token */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FString ClientId;
	/** User account associated with this auth token */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FEOSEpicAccountId AccountId;
	/** Access token for the current user login session */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FString AccessToken;
	/** Time before the access token expires, in seconds, relative to the call to EOS_Auth_CopyUserAuthToken */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FString ExpiresIn;
	/** Absolute time in UTC before the access token expires, in ISO 8601 format */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FString ExpiresAt;
	/** Type of auth token */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	EOSEAuthTokenType AuthType;
	/**
	 * Refresh token.
	 * This member has two separate use cases:
	 *
	 * 1. Launching the game from Epic Games Launcher, having an intermediate process (such as another launcher) that uses
	 * Epic authentication in-between.
	 * In this use case, an intermediate launcher that has logged in the user using the EOS_LCT_ExchangeCode login type
	 * can copy the refresh token using the EOS_Auth_CopyUserAuthToken API and pass it in launch parameters for the started
	 * game client that can then login the user again using the refresh token this time with the EOS_LCT_RefreshToken
	 * login type.
	 *
	 * 2. Persistent auth on Console platforms, to remember user login across game sessions over longer period of times
	 * without having to prompt the user to explicitly login each time they run the game.
	 * In this use case, a local user is logged in with their Epic Account for the first time using the EOS_LCT_DeviceCode
	 * login type. Specific to the EOS_LCT_DeviceCode login type, the SDK receives a long-lived refresh token instead of a
	 * regular short-lived refresh token. The game can use the EOS_Auth_CopyUserAuthToken API to copy the long-lived refresh
	 * token and store it locally for the currently logged in local user of the console device.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FString RefreshToken;
	/** Time before the access token expires, in seconds, relative to the call to EOS_Auth_CopyUserAuthToken */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FString RefreshExpiresIn;
	/** Absolute time in UTC before the refresh token expires, in ISO 8601 format */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FString RefreshExpiresAt;
public:
	FEOSAuthToken()
		: AuthType(EOSEAuthTokenType::EOS_ATT_Client)
	{
	}

	FEOSAuthToken(const EOS_Auth_Token& data)
		: AccountId(data.AccountId)
		  , ExpiresIn(LexToString(data.ExpiresIn))
		  , AuthType(static_cast<EOSEAuthTokenType>(data.AuthType))
		  , RefreshExpiresIn(LexToString(data.RefreshExpiresIn))
	{
		const FUTF8ToTCHAR AppChar(data.App);
		App = AppChar.Get();

		const FUTF8ToTCHAR ClientIdChar(data.ClientId);
		ClientId = ClientIdChar.Get();

		const FUTF8ToTCHAR AccessTokenChar(data.AccessToken);
		AccessToken = AccessTokenChar.Get();

		const FUTF8ToTCHAR ExpiresAtChar(data.ExpiresAt);
		ExpiresAt = ExpiresAtChar.Get();

		const FUTF8ToTCHAR RefreshTokenChar(data.RefreshToken);
		RefreshToken = RefreshTokenChar.Get();

		const FUTF8ToTCHAR RefreshExpiresAtChar(data.RefreshExpiresAt);
		RefreshExpiresAt = RefreshExpiresAtChar.Get();
	}
};

/**
 * A structure that contains login credentials. What is required is dependent on the type of login being initiated.
 *
 * This is part of the input structure EOS_Auth_LoginOptions and related to device auth.
 *
 * Use of the Id and Token fields differs based on the Type. They should be null, unless specified:
 * EOS_LCT_Password - Id is the email address, and Token is the password.
 * EOS_LCT_ExchangeCode - Token is the exchange code.
 * EOS_LCT_PersistentAuth - If targeting console platforms, Token is the long lived refresh token. Otherwise N/A.
 * EOS_LCT_DeviceCode - N/A.
 * EOS_LCT_Developer - Id is the host (e.g. 127.0.0.1:10000), and Token is the credential name.
 * EOS_LCT_RefreshToken - Token is the refresh token.
 * EOS_LCT_AccountPortal - SystemAuthCredentialsOptions may be required if targeting mobile platforms. Otherwise N/A.
 * EOS_LCT_ExternalAuth - Token is the external auth token specified by ExternalType.
 *
 * @see EOS_ELoginCredentialType
 * @see EOS_Auth_Login
 * @see EOS_Auth_DeletePersistentAuthOptions
 */
USTRUCT(BlueprintType)
struct FEOSAuthCredentials
{
	GENERATED_BODY()
public:
	/** Version of the API */
	int32 ApiVersion;
public:
	/** Id of the user logging in, based on EOS_ELoginCredentialType */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FString ID;
	/** Credentials or token related to the user logging in */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FString Token;
	/** Type of login. Needed to identify the auth method to use */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	EEOSLoginCredentialType Type;
	/**
	 * Type of external login. Needed to identify the external auth method to use.
	 * Used when login type is set to EOS_LCT_ExternalAuth, ignored for other EOS_ELoginCredentialType methods.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	EEOSEExternalCredentialType ExternalType;
public:
	explicit FEOSAuthCredentials()
		: ApiVersion(EOS_AUTH_CREDENTIALS_API_LATEST)
		  , Type(EEOSLoginCredentialType::EOS_LCT_AccountPortal)
		  , ExternalType(EEOSEExternalCredentialType::EOS_ECT_EPIC)
	{
	}
};

/**
 * Input parameters for the EOS_Auth_Login Function.
 */
USTRUCT(BlueprintType)
struct FEOSAuthLoginOptions
{
	GENERATED_BODY()
public:
	/** Version of the API */
	int32 ApiVersion;
public:
	/** Credentials specified for a given login method */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FEOSAuthCredentials Credentials;
	/** Auth scope flags are permissions to request from the user while they are logging in. This is a bitwise-or union of EOS_EAuthScopeFlags flags defined above */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth", meta = (Bitmask, BitmaskEnum = "/Script/EOSCoreShared.EOSEAuthScopeFlags"))
	int32 ScopeFlags;
public:
	explicit FEOSAuthLoginOptions()
		: ApiVersion(EOS_AUTH_LOGIN_API_LATEST)
		  , ScopeFlags(0)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAuthPinGrantInfo
{
	GENERATED_BODY()
public:
	/** Code the user must input on an external device to activate the login */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FString UserCode;
	/** The end-user verification URI. Users can be asked to manually type this into their browser. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FString VerificationURI;
	/** Time the user has, in seconds, to complete the process or else timeout */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	int32 ExpiresIn;
	/** A verification URI that includes the user code. Useful for non-textual transmission. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FString VerificationURIComplete;
public:
	explicit FEOSAuthPinGrantInfo()
		  : ExpiresIn(0)
	{
	}

	FEOSAuthPinGrantInfo(const EOS_Auth_PinGrantInfo* Data)
	{
		if (Data)
		{
			ExpiresIn = Data->ExpiresIn;
			
			if (Data->UserCode != nullptr)
			{
				UserCode = UTF8_TO_TCHAR(Data->UserCode);
			}
			if (Data->VerificationURI != nullptr)
			{
				VerificationURI = UTF8_TO_TCHAR(Data->VerificationURI);
			}
			if (Data->VerificationURIComplete != nullptr)
			{
				VerificationURIComplete = UTF8_TO_TCHAR(Data->VerificationURIComplete);
			}
		}
	}
};

/**
 * Intermediate data needed to complete account restriction verification during login flow, returned by EOS_Auth_LoginCallbackInfo when the ResultCode is EOS_Auth_AccountFeatureRestricted
 * The URI inside should be exposed to the user for entry in a web browser. The URI must be copied out of this struct before completion of the callback.
 */
USTRUCT(BlueprintType)
struct FEOSAuthAccountFeatureRestrictedInfo
{
	GENERATED_BODY()
public:
	/** Version of the API */
	int32 ApiVersion;
public:
	/** The end-user verification URI. Users must be asked to open the page in a browser to address the restrictions */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FString VerificationURI;
public:
	explicit FEOSAuthAccountFeatureRestrictedInfo()
		: ApiVersion(EOS_AUTH_ACCOUNTFEATURERESTRICTEDINFO_API_LATEST)
	{
	}

	FEOSAuthAccountFeatureRestrictedInfo(const EOS_Auth_AccountFeatureRestrictedInfo* Data)
		: ApiVersion(EOS_AUTH_ACCOUNTFEATURERESTRICTEDINFO_API_LATEST)
	{
		if (Data)
		{
			const FUTF8ToTCHAR VerificationURIChar(Data->VerificationURI);
			VerificationURI = VerificationURIChar.Get();
		}
	}
};

USTRUCT(BlueprintType)
struct FEOSAuthLoginCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful query, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Auth_Login */
	void* ClientData;
	/** Account ID of the local player requesting the information */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FEOSEpicAccountId LocalUserId;
	/** Optional data returned in the middle of a EOS_LCT_DeviceCode request */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FEOSAuthPinGrantInfo PinGrantInfo;
	/** If the user was not found with external auth credentials passed into EOS_Auth_Login, this continuance token can be passed to EOS_Auth_LinkAccount to continue the flow. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FContinuanceToken ContinuanceToken;
	/** If the user trying to login is restricted from doing so, the ResultCode of this structure will be EOS_Auth_AccountFeatureRestricted, and AccountFeatureRestrictedInfo will be populated with the data needed to get past the restriction */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth", meta = (DeprecatedProperty))
	FEOSAuthAccountFeatureRestrictedInfo AccountFeatureRestrictedInfo;
	/**
	 * The Epic Account ID that has been previously selected to be used for the current application.
	 * Applications should use this ID to authenticate with online backend services that store game-scoped data for users.
	 *
	 * Note: This ID may be different from LocalUserId if the user has previously merged Epic accounts into the account
	 * represented by LocalUserId, and one of the accounts that got merged had game data associated with it for the application.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FEOSEpicAccountId SelectedAccountId;

public:
	explicit FEOSAuthLoginCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSAuthLoginCallbackInfo(const EOS_Auth_LoginCallbackInfo* Data)
	{
		if (Data)
		{
			ResultCode = EOSHelpers::ToEOSCoreResult(Data->ResultCode);
			ClientData = Data->ClientData;
			LocalUserId = Data->LocalUserId;

			if (Data->PinGrantInfo)
				PinGrantInfo = Data->PinGrantInfo;

			if (Data->ContinuanceToken)
				ContinuanceToken = Data->ContinuanceToken;
			
			if (Data->AccountFeatureRestrictedInfo_DEPRECATED)
				AccountFeatureRestrictedInfo = Data->AccountFeatureRestrictedInfo_DEPRECATED;
			
			SelectedAccountId = Data->SelectedAccountId;	
		}
		else
		{
			ResultCode = EOSResult::EOS_UnexpectedError; 
		}
	}
};

USTRUCT(BlueprintType)
struct FEOSAuthLogoutOptions
{
	GENERATED_BODY()
public:
	/** Version of the API */
	int32 ApiVersion;
public:
	/** Account ID of the local player requesting the information */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FEOSEpicAccountId LocalUserId;
public:
	explicit FEOSAuthLogoutOptions()
		: ApiVersion(EOS_AUTH_LOGOUT_API_LATEST)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAuthLogoutCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful query, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Auth_Login */
	void* ClientData;
	/** Account ID of the local player requesting the information */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FEOSEpicAccountId LocalUserId;
public:
	explicit FEOSAuthLogoutCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSAuthLogoutCallbackInfo(const EOS_Auth_LogoutCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAuthLinkAccountOptions
{
	GENERATED_BODY()
public:
	/** Version of the API */
	int32 ApiVersion;
public:
	/** Account ID of the local player requesting the information */
	/**
	 * Combination of the enumeration flags to specify how the account linking operation will be performed.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	EEOSELinkAccountFlags LinkAccountFlags;
	/**
	 * Continuance token received from a previous call to the EOS_Auth_Login API.
	 *
	 * A continuance token is received in the case when the external account used for login was not found to be linked
	 * against any existing Epic Account. In such case, the application needs to proceed with an account linking operation in which case
	 * the user is first asked to create a new account or login into their existing Epic Account, and then link their external account to it.
	 * Alternatively, the application may suggest the user to login using another external account that they have already linked to their existing Epic Account.
	 * In this flow, the external account is typically the currently logged in local platform user account.
	 * It can also be another external user account that the user is offered to login with.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FContinuanceToken ContinuanceToken;

	/**
	 * The Epic Online Services Account ID of the logged in local user whose Epic Account will be linked with the local Nintendo NSA ID Account. By default set to NULL.
	 *
	 * This parameter is only used and required to be set when EOS_ELinkAccountFlags::EOS_LA_NintendoNsaId is specified.
	 * Otherwise, set to NULL, as the standard account linking and login flow using continuance token will handle logging in the user to their Epic Account.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FEOSEpicAccountId LocalUserId;
public:
	explicit FEOSAuthLinkAccountOptions()
		: ApiVersion(EOS_AUTH_LINKACCOUNT_API_LATEST)
		  , LinkAccountFlags(EEOSELinkAccountFlags::EOS_LA_NoFlags)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAuthLinkAccountCallbackInfo
{
	GENERATED_BODY()
public:
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Auth_LinkAccount */
	void* ClientData;
	/** The Epic Online Services Account ID of the local user whose account has been linked during login */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FEOSEpicAccountId LocalUserId;
	/**
	 * Optional data returned when ResultCode is EOS_Auth_PinGrantCode.
	 *
	 * Once the user has logged in with their Epic Online Services account, the account will be linked with the external account supplied when EOS_Auth_Login was called.
	 * EOS_Auth_OnLinkAccountCallback will be fired again with ResultCode in EOS_Auth_LinkAccountCallbackInfo set to EOS_Success.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FEOSAuthPinGrantInfo PinGrantInfo;
	/**
	* The Epic Account ID that has been previously selected to be used for the current application.
	* Applications should use this ID to authenticate with online backend services that store game-scoped data for users.
	*
	* Note: This ID may be different from LocalUserId if the user has previously merged Epic accounts into the account
	* represented by LocalUserId, and one of the accounts that got merged had game data associated with it for the application.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FEOSEpicAccountId SelectedAccountId;
public:
	explicit FEOSAuthLinkAccountCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSAuthLinkAccountCallbackInfo(const EOS_Auth_LinkAccountCallbackInfo& Data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(Data.ResultCode))
		  , ClientData(Data.ClientData)
		  , LocalUserId(Data.LocalUserId)
		  , PinGrantInfo(Data.PinGrantInfo)
		  , SelectedAccountId(Data.SelectedAccountId)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAuthVerifyUserAuthOptions
{
	GENERATED_BODY()
public:
	/** Auth token to verify against the backend service */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FEOSAuthToken AuthToken;
public:
	explicit FEOSAuthVerifyUserAuthOptions() = default;
};

USTRUCT(BlueprintType)
struct FEOSAuthVerifyUserAuthCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful query, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Auth_Login */
	void* ClientData;
public:
	explicit FEOSAuthVerifyUserAuthCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSAuthVerifyUserAuthCallbackInfo(const EOS_Auth_VerifyUserAuthCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAuthCopyUserAuthTokenOptions
{
	GENERATED_BODY()
public:
	/** Version of the API */
	int32 ApiVersion;
public:
	explicit FEOSAuthCopyUserAuthTokenOptions()
		: ApiVersion(EOS_AUTH_COPYUSERAUTHTOKEN_API_LATEST)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAuthAddNotifyLoginStatusChangedOptions
{
	GENERATED_BODY()
public:
	/** Version of the API */
	int32 ApiVersion;
public:
	explicit FEOSAuthAddNotifyLoginStatusChangedOptions()
		: ApiVersion(EOS_AUTH_ADDNOTIFYLOGINSTATUSCHANGED_API_LATEST)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAuthDeletePersistentAuthOptions
{
	GENERATED_BODY()
public:
	/**
	 * A long-lived refresh token that is used with the EOS_LCT_PersistentAuth login type and is to be revoked from the authentication server. Only used on Console platforms.
	 * On Desktop and Mobile platforms, set this parameter to NULL.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FString RefreshToken;
public:
	explicit FEOSAuthDeletePersistentAuthOptions() = default;
};

USTRUCT(BlueprintType)
struct FEOSAuthDeletePersistentAuthCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful query, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Auth_DeletePersistentAuth */
	void* ClientData;
public:
	explicit FEOSAuthDeletePersistentAuthCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSAuthDeletePersistentAuthCallbackInfo(const EOS_Auth_DeletePersistentAuthCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAuthLoginStatusChangedCallbackInfo
{
	GENERATED_BODY()
public:
	/** Context that was passed into EOS_Auth_AddNotifyLoginStatusChanged */
	void* ClientData;
	/** Account ID of the local player whose status has changed */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FEOSEpicAccountId LocalUserId;
	/** The status prior to the change */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	EOSELoginStatus PrevStatus;
	/** The status at the time of the notification */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	EOSELoginStatus CurrentStatus;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FEOSNotificationId NotificationID;
public:
	explicit FEOSAuthLoginStatusChangedCallbackInfo()
		: ClientData(nullptr)
		  , PrevStatus(EOSELoginStatus::EOS_LS_NotLoggedIn)
		  , CurrentStatus(EOSELoginStatus::EOS_LS_NotLoggedIn)
	{
	}

	FEOSAuthLoginStatusChangedCallbackInfo(const EOS_Auth_LoginStatusChangedCallbackInfo& data)
		: ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
		  , PrevStatus(static_cast<EOSELoginStatus>(data.PrevStatus))
		  , CurrentStatus(static_cast<EOSELoginStatus>(data.CurrentStatus))
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSHAuth
{
	GENERATED_BODY()
public:
	EOS_HAuth Handle;
public:
	explicit FEOSHAuth()
		: Handle(nullptr)
	{
	}

	FEOSHAuth(const EOS_HAuth& data)
		: Handle(data)
	{
	}
};

/**
* A structure that contains an ID token.
* These structures are created by EOS_Auth_CopyIdToken and must be passed to EOS_Auth_IdToken_Release when finished.
*/
USTRUCT(BlueprintType)
struct FEOSAuthIdToken
{
	GENERATED_BODY()
public:
	/**
	* The Epic Account ID described by the ID token.
	* Use EOS_EpicAccountId_FromString to populate this field when validating a received ID token.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FEOSEpicAccountId AccountId;
	/** The ID token as a Json Web Token (JWT) string. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FString JsonWebToken;
public:
	FEOSAuthIdToken() = default;

	FEOSAuthIdToken(const EOS_Auth_IdToken& Data)
	{
		AccountId = Data.AccountId;
		const FUTF8ToTCHAR JsonWebTokenChar(Data.JsonWebToken);
		JsonWebToken = JsonWebTokenChar.Get();
	}
};

USTRUCT(BlueprintType)
struct FEOSAuthQueryIdTokenCallbackInfo
{
	GENERATED_BODY()
public:
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Auth_QueryIdToken */
	void* ClientData;
	/** The Epic Account ID of the local authenticated user. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FEOSEpicAccountId LocalUserId;
	/** The target Epic Account ID for which the ID token was retrieved. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FEOSEpicAccountId TargetAccountId;
public:
	FEOSAuthQueryIdTokenCallbackInfo()
		: ResultCode(EOSResult::MAX)
		  , ClientData(nullptr)
	{
	}

	FEOSAuthQueryIdTokenCallbackInfo(const EOS_Auth_QueryIdTokenCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
		  , TargetAccountId(data.TargetAccountId)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSAuthVerifyIdTokenCallbackInfo
{
	GENERATED_BODY()
public:
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Auth_VerifyIdToken */
	void* ClientData;
	/**
	 * Epic Account Services Application ID.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FString ApplicationId;
	/**
	 * Client ID of the authorized client.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FString ClientId;
	/**
	 * Product ID.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FString ProductId;
	/**
	 * Sandbox ID.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FString SandboxId;
	/**
	 * Deployment ID.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FString DeploymentId;
	/**
	 * Epic Account display name.
	 *
	 * This value may be set to an empty string.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FString DisplayName;
	/**
	 * Flag set to indicate whether external account information is present.
	 * Applications must always first check this value to be set before attempting
	 * to read the ExternalAccountIdType, ExternalAccountId, ExternalAccountDisplayName and Platform fields.
	 *
	 * This flag is set when the user has logged in to their Epic Account using external account credentials, e.g. through local platform authentication.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	bool bIsExternalAccountInfoPresent;
	/**
	 * The identity provider that the user logged in with to their Epic Account.
	 *
	 * If bIsExternalAccountInfoPresent is set, this field describes the external account type.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	EEOSEExternalAccountType ExternalAccountIdType;
	/**
	 * The external account ID of the logged in user.
	 *
	 * This value may be set to an empty string.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FString ExternalAccountId;
	/**
	 * The external account display name.
	 *
	 * This value may be set to an empty string.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FString ExternalAccountDisplayName;
	/**
	 * Platform that the user is connected from.
	 *
	 * This value may be set to an empty string.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FString Platform;
public:
	FEOSAuthVerifyIdTokenCallbackInfo()
		: ResultCode(EOSResult::MAX)
		  , ClientData(nullptr)
		  , bIsExternalAccountInfoPresent(false)
		  , ExternalAccountIdType(EEOSEExternalAccountType::EOS_EAT_EPIC)
	{
	}

	FEOSAuthVerifyIdTokenCallbackInfo(const EOS_Auth_VerifyIdTokenCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
		  , bIsExternalAccountInfoPresent(data.bIsExternalAccountInfoPresent > 0)
		  , ExternalAccountIdType(static_cast<EEOSEExternalAccountType>(data.ExternalAccountIdType))
	{
		const FUTF8ToTCHAR ApplicationIdChar(data.ApplicationId);
		ApplicationId = ApplicationIdChar.Get();

		const FUTF8ToTCHAR ClientIdChar(data.ClientId);
		ClientId = ClientIdChar.Get();

		const FUTF8ToTCHAR ProductIdChar(data.ProductId);
		ProductId = ProductIdChar.Get();

		const FUTF8ToTCHAR SandboxIdChar(data.SandboxId);
		SandboxId = SandboxIdChar.Get();

		const FUTF8ToTCHAR DeploymentIdChar(data.DeploymentId);
		DeploymentId = DeploymentIdChar.Get();

		const FUTF8ToTCHAR DisplayNameChar(data.DisplayName);
		DisplayName = DisplayNameChar.Get();

		const FUTF8ToTCHAR ExternalAccountIdChar(data.ExternalAccountId);
		ExternalAccountId = ExternalAccountIdChar.Get();

		const FUTF8ToTCHAR ExternalAccountDisplayNameChar(data.ExternalAccountDisplayName);
		ExternalAccountDisplayName = ExternalAccountDisplayNameChar.Get();

		const FUTF8ToTCHAR PlatformChar(data.Platform);
		Platform = PlatformChar.Get();
	}
};

/**
* Input parameters for the EOS_Auth_QueryIdToken function.
*/
USTRUCT(BlueprintType)
struct FEOSAuthQueryIdTokenOptions
{
	GENERATED_BODY()
public:
	/** The Epic Account ID of the local authenticated user. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FEOSEpicAccountId LocalUserId;
	/**
	* The target Epic Account ID for which to query an ID token.
	* This account id may be the same as the input LocalUserId or another merged account id associated with the local user's Epic account.
	*
	* An ID token for the selected account id of a locally authenticated user will always be readily available.
	* To retrieve it for the selected account ID, you can use EOS_Auth_CopyIdToken directly after a successful user login.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FEOSEpicAccountId TargetAccountId;
public:
	FEOSAuthQueryIdTokenOptions() = default;
};

/**
* Input parameters for the EOS_Auth_VerifyIdToken function.
*/
USTRUCT(BlueprintType)
struct FEOSAuthVerifyIdTokenOptions
{
	GENERATED_BODY()
public:
	/**
	* The ID token to verify.
	* Use EOS_EpicAccountId_FromString to populate the AccountId field of this struct.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FEOSAuthIdToken IdToken;
public:
	FEOSAuthVerifyIdTokenOptions() = default;
};

/**
* Input parameters for the EOS_Auth_CopyIdToken function.
*/
USTRUCT(BlueprintType)
struct FEOSAuthCopyIdTokenOptions
{
	GENERATED_BODY()
public:
	/** The Epic Account ID of the user being queried. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Auth")
	FEOSEpicAccountId AccountId;
public:
	FEOSAuthCopyIdTokenOptions() = default;
};


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		DELEGATES
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAuthLoginCallback, const FEOSAuthLoginCallbackInfo&, data);

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAuthLogoutCallback, const FEOSAuthLogoutCallbackInfo&, data);

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAuthLinkAccountCallback, const FEOSAuthLinkAccountCallbackInfo&, data);

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAuthVerifyUserAuthCallback, const FEOSAuthVerifyUserAuthCallbackInfo&, data);

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAuthDeletePersistentAuthCallback, const FEOSAuthDeletePersistentAuthCallbackInfo&, data);

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAuthLoginStatusChangedCallback, const FEOSAuthLoginStatusChangedCallbackInfo&, data);

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAuthQueryIdTokenCallback, const FEOSAuthQueryIdTokenCallbackInfo&, data);

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAuthVerifyIdTokenCallback, const FEOSAuthVerifyIdTokenCallbackInfo&, data);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAuthLoginCallbackDelegate, const FEOSAuthLoginCallbackInfo&, data);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAuthLogoutCallbackDelegate, const FEOSAuthLogoutCallbackInfo&, data);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAuthLinkAccountCallbackDelegate, const FEOSAuthLinkAccountCallbackInfo&, data);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAuthVerifyUserAuthCallbackDelegate, const FEOSAuthVerifyUserAuthCallbackInfo&, data);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAuthDeletePersistentAuthCallbackDelegate, const FEOSAuthDeletePersistentAuthCallbackInfo&, data);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAuthLoginStatusChangedCallbackDelegate, const FEOSAuthLoginStatusChangedCallbackInfo&, data);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		CALLBACK OBJECTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

struct FAuthLoginCallback
{
public:
	FOnAuthLoginCallback m_Callback;
public:
	FAuthLoginCallback(const FOnAuthLoginCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FAuthLoginCallback()
	{
		m_Callback.Unbind();
	}
};

struct FAuthLogoutCallback
{
public:
	FOnAuthLogoutCallback m_Callback;
public:
	FAuthLogoutCallback(const FOnAuthLogoutCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FAuthLogoutCallback()
	{
		m_Callback.Unbind();
	}
};

struct FAuthLinkAccountCallback
{
public:
	FOnAuthLinkAccountCallback m_Callback;
public:
	FAuthLinkAccountCallback(const FOnAuthLinkAccountCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FAuthLinkAccountCallback()
	{
		m_Callback.Unbind();
	}
};

struct FAuthDeletePersistentAuthCallback
{
public:
	FOnAuthDeletePersistentAuthCallback m_Callback;
public:
	FAuthDeletePersistentAuthCallback(const FOnAuthDeletePersistentAuthCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FAuthDeletePersistentAuthCallback()
	{
		m_Callback.Unbind();
	}
};

struct FAuthVerifyUserAuthCallback
{
public:
	FOnAuthVerifyUserAuthCallback m_Callback;
public:
	FAuthVerifyUserAuthCallback(const FOnAuthVerifyUserAuthCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FAuthVerifyUserAuthCallback()
	{
		m_Callback.Unbind();
	}
};

struct FAuthLoginStatusChangedCallback
{
public:
	FAuthLoginStatusChangedCallback(const FOnAuthLoginStatusChangedCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FAuthLoginStatusChangedCallback()
	{
		m_Callback.Unbind();
	}

public:
	FOnAuthLoginStatusChangedCallback m_Callback;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		REQUEST OBJECTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

struct FAuthToken : public EOS_Auth_Token
{
	FAuthToken(FString clientId, FString app, FString expiresAt, FString refreshToken, FString refreshExpiresAt, FString accessToken)
		: EOS_Auth_Token()
	{
		ApiVersion = EOS_AUTH_TOKEN_API_LATEST;
		if (!clientId.IsEmpty())
		{
			ClientId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(ClientId), TCHAR_TO_UTF8(*clientId), 256);
		}
		if (!app.IsEmpty())
		{
			App = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(App), TCHAR_TO_UTF8(*app), 256);
		}
		if (!expiresAt.IsEmpty())
		{
			ExpiresAt = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(ExpiresAt), TCHAR_TO_UTF8(*expiresAt), 256);
		}
		if (!refreshToken.IsEmpty())
		{
			RefreshToken = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(RefreshToken), TCHAR_TO_UTF8(*refreshToken), 256);
		}
		if (!refreshExpiresAt.IsEmpty())
		{
			RefreshExpiresAt = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(RefreshExpiresAt), TCHAR_TO_UTF8(*refreshExpiresAt), 256);
		}
		if (!accessToken.IsEmpty())
		{
			AccessToken = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(AccessToken), TCHAR_TO_UTF8(*accessToken), 256);
		}
	}

	~FAuthToken()
	{
		delete[] ClientId;
		delete[] App;
		delete[] ExpiresAt;
		delete[] RefreshToken;
		delete[] RefreshExpiresAt;
	}
};

struct FAuthDeletePersistentAuthOptions : public EOS_Auth_DeletePersistentAuthOptions
{
	FAuthDeletePersistentAuthOptions(FString refreshToken)
		: EOS_Auth_DeletePersistentAuthOptions()
	{
		ApiVersion = EOS_AUTH_DELETEPERSISTENTAUTH_API_LATEST;

		if (!refreshToken.IsEmpty())
		{
			RefreshToken = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(RefreshToken), TCHAR_TO_UTF8(*refreshToken), 256);
		}
	}

	~FAuthDeletePersistentAuthOptions()
	{
		delete[] RefreshToken;
	}
};

struct FAuthQueryIdToken
{
public:
	FOnAuthQueryIdTokenCallback m_Callback;
public:
	FAuthQueryIdToken(const FOnAuthQueryIdTokenCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FAuthQueryIdToken()
	{
		m_Callback.Unbind();
	}
};

struct FAuthVerifyIdToken
{
public:
	FOnAuthVerifyIdTokenCallback m_Callback;
public:
	FAuthVerifyIdToken(const FOnAuthVerifyIdTokenCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FAuthVerifyIdToken()
	{
		m_Callback.Unbind();
	}
};
