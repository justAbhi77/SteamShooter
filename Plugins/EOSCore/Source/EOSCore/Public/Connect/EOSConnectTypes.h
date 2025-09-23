/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "eos_connect_types.h"
#include "Core/EOSHelpers.h"
#include "EOSConnectTypes.generated.h"

class UCoreConnect;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		ENUMS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

/**
 * List of the supported identity providers to authenticate a user.
 *
 * The type of authentication token is specific to each provider.
 * Tokens in string format should be passed as-is to the function,
 * and tokens retrieved as raw byte arrays should be converted to string using
 * the EOS_ByteArray_ToString API before passing them to the Connect Interface API.
 *
 * @see EOS_Connect_Login
 * @see EOS_Connect_Credentials
 */
UENUM(BlueprintType)
enum class EEOSEExternalCredentialType : uint8
{
	/**
	 * Epic Games User Token
	 *
	 * Acquired using EOS_Auth_CopyUserAuthToken that returns EOS_Auth_Token::AccessToken.
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EOS_ECT_EPIC = 0,
	/**
	 * Steam Encrypted App Ticket
	 *
	 * Generated using the ISteamUser::RequestEncryptedAppTicket API of Steamworks SDK.
	 * For ticket generation parameters, use pDataToInclude(NULL) and cbDataToInclude(0).
	 *
	 * The retrieved App Ticket byte buffer needs to be converted into a hex-encoded UTF-8 string (e.g. "FA87097A..") before passing it to the EOS_Auth_Login or EOS_Connect_Login APIs.
	 * EOS_ByteArray_ToString can be used for this conversion.
	 *
	 * Supported with EOS_Auth_Login, EOS_Connect_Login.
	 */
	EOS_ECT_STEAM_APP_TICKET = 1,
	/**
	 * PlayStation(TM)Network ID Token
	 *
	 * Retrieved from the PlayStation(R) SDK. Please see first-party documentation for additional information.
	 *
	 * Supported with EOS_Auth_Login, EOS_Connect_Login.
	 */
	EOS_ECT_PSN_ID_TOKEN = 2,
	/**
	 * Xbox Live XSTS Token
	 *
	 * Retrieved from the GDK and XDK. Please see first-party documentation for additional information.
	 *
	 * Supported with EOS_Auth_Login, EOS_Connect_Login.
	 */
	EOS_ECT_XBL_XSTS_TOKEN = 3,
	/**
	 * Discord Access Token
	 *
	 * Retrieved using the ApplicationManager::GetOAuth2Token API of Discord SDK.
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EOS_ECT_DISCORD_ACCESS_TOKEN = 4,
	/**
	 * GOG Galaxy Encrypted App Ticket
	 *
	 * Generated using the IUser::RequestEncryptedAppTicket API of GOG Galaxy SDK.
	 * For ticket generation parameters, use data(NULL) and dataSize(0).
	 *
	 * The retrieved App Ticket byte buffer needs to be converted into a hex-encoded UTF-8 string (e.g. "FA87097A..") before passing it to the EOS_Connect_Login API.
	 * For C/C++ API integration, use the EOS_ByteArray_ToString API for the conversion.
	 * For C# integration, you can use <see cref="Helper.ToHexString" /> for the conversion.
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EOS_ECT_GOG_SESSION_TICKET = 5,
	/**
	 * Nintendo Account ID Token
	 *
	 * Identifies a Nintendo user account and is acquired through web flow authentication where the local user logs in using their email address/sign-in ID and password.
	 * This is the common Nintendo account that users login with outside the Nintendo Switch device.
	 *
	 * Supported with EOS_Auth_Login, EOS_Connect_Login.
	 */
	EOS_ECT_NINTENDO_ID_TOKEN = 6,
	/**
	 * Nintendo Service Account ID Token (NSA ID)
	 *
	 * The NSA ID identifies uniquely the local Nintendo Switch device. The authentication token is acquired locally without explicit user credentials.
	 * As such, it is the primary authentication method for seamless login on Nintendo Switch.
	 *
	 * The NSA ID is not exposed directly to the user and does not provide any means for login outside the local device.
	 * Because of this, Nintendo Switch users will need to link their Nintendo Account or another external user account
	 * to their Product User ID in order to share their game progression across other platforms. Otherwise, the user will
	 * not be able to login to their existing Product User ID on another platform due to missing login credentials to use.
	 * It is recommended that the game explicitly communicates this restriction to the user so that they will know to add
	 * the first linked external account on the Nintendo Switch device and then proceed with login on another platform.
	 *
	 * In addition to sharing cross-platform game progression, linking the Nintendo Account or another external account
	 * will allow preserving the game progression permanently. Otherwise, the game progression will be tied only to the
	 * local device. In case the user loses access to their local device, they will not be able to recover the game
	 * progression if it is only associated with this account type.
	 *
	 * Supported with EOS_Auth_Login, EOS_Connect_Login.
	 */
	EOS_ECT_NINTENDO_NSA_ID_TOKEN = 7,
	/**
	 * Uplay Access Token
	 */
	EOS_ECT_UPLAY_ACCESS_TOKEN = 8,
	/**
	 * OpenID Provider Access Token
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EOS_ECT_OPENID_ACCESS_TOKEN = 9,
	/**
	 * Device ID access token that identifies the current locally logged in user profile on the local device.
	 * The local user profile here refers to the operating system user login, for example the user's Windows Account
	 * or on a mobile device the default active user profile.
	 *
	 * This credential type is used to automatically login the local user using the EOS Connect Device ID feature.
	 *
	 * The intended use of the Device ID feature is to allow automatically logging in the user on a mobile device
	 * and to allow playing the game without requiring the user to necessarily login using a real user account at all.
	 * This makes a seamless first-time experience possible and allows linking the local device with a real external
	 * user account at a later time, sharing the same EOS_ProductUserId that is being used with the Device ID feature.
	 *
	 * Supported with EOS_Connect_Login.
	 *
	 * @see EOS_Connect_CreateDeviceId
	 */
	EOS_ECT_DEVICEID_ACCESS_TOKEN = 10,
	/**
	 * Apple ID Token
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EOS_ECT_APPLE_ID_TOKEN = 11,
	/**
	 * Google ID Token
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EOS_ECT_GOOGLE_ID_TOKEN = 12,
	/**
	 * Oculus User ID and Nonce
	 *
	 * Call ovr_User_GetUserProof(), or Platform.User.GetUserProof() if you are using Unity, to retrieve the nonce.
	 * Then pass the local User ID and the Nonce as a "{UserID}|{Nonce}" formatted string for the EOS_Connect_Login Token parameter.
	 *
	 * Note that in order to successfully retrieve a valid non-zero id for the local user using ovr_User_GetUser(),
	 * your Oculus App needs to be configured in the Oculus Developer Dashboard to have the User ID feature enabled.
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EOS_ECT_OCULUS_USERID_NONCE = 13,
	/**
	 * itch.io JWT Access Token
	 *
	 * Use the itch.io app manifest to receive a JWT access token for the local user via the ITCHIO_API_KEY process environment variable.
	 * The itch.io access token is valid for 7 days after which the game needs to be restarted by the user as otherwise EOS Connect
	 * authentication session can no longer be refreshed.
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EOS_ECT_ITCHIO_JWT = 14,
	/**
	 * itch.io Key Access Token
	 *
	 * This access token type is retrieved through the OAuth 2.0 authentication flow for the itch.io application.
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EOS_ECT_ITCHIO_KEY = 15,
	/**
	 * Epic Games ID Token
	 * 
	 * Acquired using EOS_Auth_CopyIdToken that returns EOS_Auth_IdToken::JsonWebToken.
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EOS_ECT_EPIC_ID_TOKEN = 16,
	/**
	 * Amazon Access Token
	 *
	 * Supported with EOS_Connect_Login.
	 */
	EOS_ECT_AMAZON_ACCESS_TOKEN = 17,
	/**
	 * Steam Auth Session Ticket
	 *
	 * Generated using the ISteamUser::GetAuthTicketForWebApi API of Steamworks SDK.
	 *
	 * @attention
	 * The pchIdentity input parameter of GetAuthTicketForWebApi API must be set to a valid non-empty string value.
	 * The string value used by the game client must match identically to the backend-configured value in EOS Dev Portal.
	 * The recommended value to use is "epiconlineservices" in lowercase, matching the default value for new Steam identity provider credentials in EOS Dev Portal.
	 * This identifier is important for security reasons to prevent session hijacking. Applications must use a dedicated unique identity identifier for Session Tickets passed to the EOS SDK APIs.
	 * Session Tickets using the EOS-assigned identifier must not be used with anything else than the EOS SDK APIs. You must use a different identifier when generating Session Tickets to authenticate with other parties.
	 *
	 * @warning
	 * To update an already live game to use the new GetAuthTicketForWebApi API instead of the deprecated GetAuthSessionTicket API, follow these steps in this order to prevent breaking the live game for players:
	 * 1. Update your game client code to use the new ISteamUser::GetAuthTicketForWebApi API.
	 * 2. Publish the new game client update to end-users.
	 * 3. Update the existing Steam identity provider credentials entry in EOS Dev Portal to use the same identity string identifier as the game client.
	 *
	 * @example
	 * SteamUser()->GetAuthTicketForWebApi("epiconlineservices");
	 *
	 * The retrieved Auth Session Ticket byte buffer needs to be converted into a hex-encoded UTF-8 string (e.g. "FA87097A..") before passing it to the EOS_Auth_Login or EOS_Connect_Login APIs.
	 * EOS_ByteArray_ToString can be used for this conversion.
	 *
	 * Supported with EOS_Auth_Login, EOS_Connect_Login.
	 *
	 * @version 1.15.1+
	 */
	EOS_ECT_STEAM_SESSION_TICKET = 18
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		STRUCTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
/**
 * A structure that contains external login credentials.
 *
 * This is part of the input structure EOS_Connect_LoginOptions
 *
 * @see EOS_EExternalCredentialType
 * @see EOS_Connect_Login
 */
USTRUCT(BlueprintType)
struct FEOSConnectCredentials
{
	GENERATED_BODY()
public:
	/** External token associated with the user logging in */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FString AccessToken;
	/** Type of external login. Needed to identify the auth method to use */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	EEOSEExternalCredentialType Type;
public:
	FEOSConnectCredentials()
		: Type(EEOSEExternalCredentialType::EOS_ECT_EPIC)
	{
	}

	FEOSConnectCredentials(const EOS_Connect_Credentials& data)
		: Type(static_cast<EEOSEExternalCredentialType>(data.Type))
	{
		const FUTF8ToTCHAR AccessTokenChar(data.Token);
		AccessToken = AccessTokenChar.Get();
	}
};

/**
 * Additional information about the local user.
 *
 * As the information passed here is client-controlled and not part of the user authentication tokens, it is only treated as non-authoritative informational data to be used by some of the feature services. For example displaying player names in Leaderboards rankings.
 */
USTRUCT(BlueprintType)
struct FEOSConnectUserLoginInfo
{
	GENERATED_BODY()
public:
	/**
	 * The user's display name on the identity provider systems as UTF-8 encoded null-terminated string.
	 * The length of the name can be at maximum up to EOS_CONNECT_USERLOGININFO_DISPLAYNAME_MAX_LENGTH bytes.
	 *
	 * As the display name passed here is client-controlled and not part of user authentication tokens,
	 * it is only treated as non-authoritative informational data to be used by some of the feature services.
	 * For example displaying player names in Leaderboards rankings.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FString DisplayName;
	/**
	 * Nintendo Service Account ID Token (NSA ID).
	 * 
	 * This field is required to be set and only used when running on the Nintendo Switch device,
	 * and the user is being authenticated using any other credential type than EOS_ECT_NINTENDO_NSA_ID_TOKEN.
	 *
	 * In order to use the Lobbies and Sessions interfaces, a valid NSA ID Token is required to be provided
	 * for the active local Nintendo Switch user. Otherwise, attempting to use either of the Lobbies or
	 * Sessions interfaces will return the EOS_Permission_OnlinePlayRestricted error result.
	 */
	FString NsaIdToken;
public:
	explicit FEOSConnectUserLoginInfo() = default;

	FEOSConnectUserLoginInfo(const EOS_Connect_UserLoginInfo& Data)
	{
		const FUTF8ToTCHAR DisplayNameChar(Data.DisplayName);
		DisplayName = DisplayNameChar.Get();

		const FUTF8ToTCHAR NsaIdTokenChar(Data.NsaIdToken);
		NsaIdToken = NsaIdTokenChar.Get();
	}
};

/**
 * Input parameters for the EOS_Connect_Login Function.
 */
USTRUCT(BlueprintType)
struct FEOSConnectLoginOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Credentials specified for a given login method */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSConnectCredentials Credentials;
	/**
	 * Additional non-authoritative information about the local user.
	 *
	 * This field is required to be set for user authentication with Apple and Nintendo, as well as with the Device ID feature login. When using other identity providers, set to NULL.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSConnectUserLoginInfo UserLoginInfo;
public:
	explicit FEOSConnectLoginOptions()
		: ApiVersion(EOS_CONNECT_LOGIN_API_LATEST)
	{
	}

	FEOSConnectLoginOptions(const EOS_Connect_LoginOptions& data)
		: ApiVersion(EOS_CONNECT_LOGIN_API_LATEST)
		  , Credentials(*data.Credentials)
		  , UserLoginInfo(*data.UserLoginInfo)
	{
	}
};

/**
 * Output parameters for the EOS_Connect_Login Function.
 */
USTRUCT(BlueprintType)
struct FEOSConnectLoginCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful query, otherwise one of the error codes is returned. See eos_result.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Connect_Login */
	void* ClientData;
	/** If login was succesful, this is the account ID of the local player that logged in */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSProductUserId LocalUserId;
	/**
	 * If the user was not found with credentials passed into EOS_Connect_Login,
	 * this continuance token can be passed to either EOS_Connect_CreateUser
	 * or EOS_Connect_LinkAccount to continue the flow
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FContinuanceToken ContinuanceToken;
public:
	explicit FEOSConnectLoginCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSConnectLoginCallbackInfo(const EOS_Connect_LoginCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
		  , ContinuanceToken(data.ContinuanceToken)
	{
	}
};

/**
 * Input parameters for the EOS_Connect_CreateUser Function.
 */
USTRUCT(BlueprintType)
struct FEOSConnectCreateUserOptions
{
	GENERATED_BODY()
public:
	/** Continuance token from previous call to EOS_Connect_Login */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FContinuanceToken ContinuanceToken;
public:
	explicit FEOSConnectCreateUserOptions() = default;

	FEOSConnectCreateUserOptions(const EOS_Connect_CreateUserOptions& data)
		: ContinuanceToken(data.ContinuanceToken)
	{
	}
};

/**
 * Output parameters for the EOS_Connect_CreateUser Function.
 */
USTRUCT(BlueprintType)
struct FEOSConnectCreateUserCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful query, otherwise one of the error codes is returned. See eos_result.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Connect_CreateUser */
	void* ClientData;
	/** Account ID of the local player created by this operation */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSProductUserId LocalUserId;
public:
	explicit FEOSConnectCreateUserCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSConnectCreateUserCallbackInfo(const EOS_Connect_CreateUserCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
	{
	}
};

/**
 * Input parameters for the EOS_Connect_LinkAccount Function.
 */
USTRUCT(BlueprintType)
struct FEOSConnectLinkAccountOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Existing logged in user that will link to the external account referenced by the continuance token */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSProductUserId LocalUserId;
	/** Continuance token from previous call to EOS_Connect_Login */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FContinuanceToken ContinuanceToken;
public:
	explicit FEOSConnectLinkAccountOptions()
		: ApiVersion(EOS_CONNECT_LINKACCOUNT_API_LATEST)
	{
	}

	FEOSConnectLinkAccountOptions(const EOS_Connect_LinkAccountOptions& data)
		: ApiVersion(EOS_CONNECT_LINKACCOUNT_API_LATEST)
		  , LocalUserId(data.LocalUserId)
		  , ContinuanceToken(data.ContinuanceToken)
	{
	}
};

/**
 * Output parameters for the EOS_Connect_LinkAccount Function.
 */
USTRUCT(BlueprintType)
struct FEOSConnectLinkAccountCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful query, otherwise one of the error codes is returned. See eos_result.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Connect_LinkAccount */
	void* ClientData;
	/** Existing logged in user that had external auth linked */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSProductUserId LocalUserId;
public:
	explicit FEOSConnectLinkAccountCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSConnectLinkAccountCallbackInfo(const EOS_Connect_LinkAccountCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
	{
	}
};

/**
 * Input parameters for the EOS_Connect_UnlinkAccount Function.
 */
USTRUCT(BlueprintType)
struct FEOSConnectUnlinkAccountOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/**
	 * Existing logged in product user that is subject for the unlinking operation.
	 * The external account that was used to login to the product user will be unlinked from the owning keychain.
	 *
	 * On a successful operation, the product user will be logged out as the external account used to authenticate the user was unlinked from the owning keychain.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSProductUserId LocalUserId;
public:
	explicit FEOSConnectUnlinkAccountOptions()
		: ApiVersion(EOS_CONNECT_UNLINKACCOUNT_API_LATEST)
	{
	}
};

/**
 * Output parameters for the EOS_Connect_UnlinkAccount Function.
 */
USTRUCT(BlueprintType)
struct FEOSConnectUnlinkAccountCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful query, otherwise one of the error codes is returned. See eos_result.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Connect_UnlinkAccount */
	void* ClientData;
	/**
		* The product user that was subject for the unlinking operation.
		*
		* On a successful operation, the local authentication session for the product user will have been invalidated.
		* As such, the LocalUserId value will no longer be valid in any context unless the user is logged into it again.
		*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSProductUserId LocalUserId;
public:
	explicit FEOSConnectUnlinkAccountCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSConnectUnlinkAccountCallbackInfo(const EOS_Connect_UnlinkAccountCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSConnectCreateDeviceIdOptions
{
	GENERATED_BODY()
public:
	/**
	 * A freeform text description identifying the device type and model,
	 * which can be used in account linking management to allow the player
	 * and customer support to identify different devices linked to an EOS
	 * user keychain. For example 'iPhone 6S' or 'PC Windows'.
	 *
	 * The input string must be in UTF-8 character format, with a maximum
	 * length of 64 characters. Longer string will be silently truncated.
	 *
	 * This field is required to be present.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FString DeviceModel;
public:
	explicit FEOSConnectCreateDeviceIdOptions() = default;

	FEOSConnectCreateDeviceIdOptions(const EOS_Connect_CreateDeviceIdOptions& data)
	{
		const FUTF8ToTCHAR DeviceModelChar(data.DeviceModel);
		DeviceModel = DeviceModelChar.Get();
	}
};

/**
 * Output parameters for the EOS_Connect_CreateDeviceId Function.
 */
USTRUCT(BlueprintType)
struct FEOSConnectCreateDeviceIdCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful query, otherwise one of the error codes is returned. See eos_result.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Connect_CreateDeviceId */
	void* ClientData;
public:
	explicit FEOSConnectCreateDeviceIdCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSConnectCreateDeviceIdCallbackInfo(const EOS_Connect_CreateDeviceIdCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
	{
	}
};

/**
 * Input parameters for the EOS_Connect_DeleteDeviceId Function.
 */
USTRUCT(BlueprintType)
struct FEOSConnectDeleteDeviceIdOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	explicit FEOSConnectDeleteDeviceIdOptions()
		: ApiVersion(EOS_CONNECT_DELETEDEVICEID_API_LATEST)
	{
	}

	FEOSConnectDeleteDeviceIdOptions(const EOS_Connect_DeleteDeviceIdOptions& data)
		: ApiVersion(EOS_CONNECT_DELETEDEVICEID_API_LATEST)
	{
	}
};

/**
 * Output parameters for the EOS_Connect_DeleteDeviceId Function.
 */
USTRUCT(BlueprintType)
struct FEOSConnectDeleteDeviceIdCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful query, otherwise one of the error codes is returned. See eos_result.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Connect_DeleteDeviceId */
	void* ClientData;
public:
	explicit FEOSConnectDeleteDeviceIdCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSConnectDeleteDeviceIdCallbackInfo(const EOS_Connect_DeleteDeviceIdCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
	{
	}
};

/**
 * Input parameters for the EOS_Connect_DeleteDeviceId Function.
 */
USTRUCT(BlueprintType)
struct FEOSConnectTransferDeviceIdAccountOptions
{
	GENERATED_BODY()
public:
	/**
	 * The primary product user id, currently logged in, that is already associated with a real external user account (such as Epic Games, Playstation, Xbox and other).
	 *
	 * The account linking keychain that owns this product user will be preserved and receive
	 * the Device ID login credentials under it.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSProductUserId PrimaryLocalUserId;
	/**
	 * The product user id, currently logged in, that has been originally created using the anonymous local Device ID login type,
	 * and whose Device ID login will be transferred to the keychain of the PrimaryLocalUserId.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSProductUserId LocalDeviceUserId;
	/**
	 * Specifies which FEOSProductUserId (i.e. game progression) will be preserved in the operation.
	 *
	 * After a successful transfer operation, subsequent logins using the same external account or
	 * the same local Device ID login will return user session for the ProductUserIdToPreserve.
	 *
	 * Set to either PrimaryLocalUserId or LocalDeviceUserId.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSProductUserId ProductUserIdToPreserve;
public:
	explicit FEOSConnectTransferDeviceIdAccountOptions() = default;
};

/**
 * Output parameters for the EOS_Connect_DeleteDeviceId Function.
 */
USTRUCT(BlueprintType)
struct FEOSConnectTransferDeviceIdAccountCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful query, otherwise one of the error codes is returned. See eos_result.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Connect_DeleteDeviceId */
	void* ClientData;
	/**
 * The ProductUserIdToPreserve that was passed to the original EOS_Connect_TransferDeviceIdAccount call.
 *
 * On successful operation, this EOS_ProductUserId will have a valid authentication session
 * and the other EOS_ProductUserId value has been discarded and lost forever.
 *
 * The application should remove any registered notification callbacks for the discarded EOS_ProductUserId as obsolete.
 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSProductUserId LocalUserId;
public:
	explicit FEOSConnectTransferDeviceIdAccountCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSConnectTransferDeviceIdAccountCallbackInfo(const EOS_Connect_TransferDeviceIdAccountCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSConnectQueryExternalAccountMappingsOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Existing logged in user that is querying account mappings */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSProductUserId LocalUserId;
	/** External auth service supplying the account ids in string form */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	EEOSEExternalAccountType AccountIdType;
	/** An array of external account ids to map to the product user id representation */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	TArray<FString> ExternalAccountIds;
public:
	explicit FEOSConnectQueryExternalAccountMappingsOptions()
		: ApiVersion(EOS_CONNECT_QUERYEXTERNALACCOUNTMAPPINGS_API_LATEST)
		  , AccountIdType(EEOSEExternalAccountType::EOS_EAT_EPIC)
	{
	}

	FEOSConnectQueryExternalAccountMappingsOptions(const EOS_Connect_QueryExternalAccountMappingsOptions& data)
		: ApiVersion(EOS_CONNECT_QUERYEXTERNALACCOUNTMAPPINGS_API_LATEST)
		  , LocalUserId(data.LocalUserId)
		  , AccountIdType(static_cast<EEOSEExternalAccountType>(data.AccountIdType))
	{
		for (uint32_t i = 0; i < data.ExternalAccountIdCount; i++)
			ExternalAccountIds.Add(data.ExternalAccountIds[i]);
	}
};

/**
 * Output parameters for the EOS_Connect_QueryExternalAccountMappings Function.
 */
USTRUCT(BlueprintType)
struct FEOSConnectQueryExternalAccountMappingsCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful query, otherwise one of the error codes is returned. See eos_result.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Connect_QueryExternalAccountMappings */
	void* ClientData;
	/** Existing logged in user that made the request */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSProductUserId LocalUserId;
public:
	explicit FEOSConnectQueryExternalAccountMappingsCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSConnectQueryExternalAccountMappingsCallbackInfo(const EOS_Connect_QueryExternalAccountMappingsCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
	{
	}
};

/**
 * Input parameters for the EOS_Connect_GetExternalAccountMappings Function.
 */
USTRUCT(BlueprintType)
struct FEOSConnectGetExternalAccountMappingsOptions
{
	GENERATED_BODY()
public:
	/** Existing logged in user that is querying account mappings */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSProductUserId LocalUserId;
	/** External auth service supplying the account ids in string form */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	EEOSEExternalAccountType AccountIdType;
	/** Target user to retrieve the mapping for, as an external account id */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FString TargetExternalUserId;
public:
	explicit FEOSConnectGetExternalAccountMappingsOptions()
		: AccountIdType(EEOSEExternalAccountType::EOS_EAT_EPIC)
	{
	}

	FEOSConnectGetExternalAccountMappingsOptions(const EOS_Connect_GetExternalAccountMappingsOptions& data)
		: LocalUserId(data.LocalUserId)
		  , AccountIdType(static_cast<EEOSEExternalAccountType>(data.AccountIdType))
	{
		const FUTF8ToTCHAR TargetExternalUserIdChar(data.TargetExternalUserId);
		TargetExternalUserId = TargetExternalUserIdChar.Get();
	}
};

/**
 * Input parameters for the EOS_Connect_QueryProductUserIdMappings Function.
 */
USTRUCT(BlueprintType)
struct FEOSConnectQueryProductUserIdMappingsOptions
{
	GENERATED_BODY()
public:
	/** Existing logged in user that is querying account mappings */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSProductUserId LocalUserId;
	/** Deprecated - all external mappings are included in this call, it is no longer necessary to specify this value */
	EOS_EExternalAccountType AccountIdType_DEPRECATED;
	/** An array of product user ids to query for the given external account representation */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	TArray<FEOSProductUserId> ProductUserIds;
public:
	explicit FEOSConnectQueryProductUserIdMappingsOptions() = default;

	FEOSConnectQueryProductUserIdMappingsOptions(const EOS_Connect_QueryProductUserIdMappingsOptions& data)
		: LocalUserId(data.LocalUserId)
		  , AccountIdType_DEPRECATED(data.AccountIdType_DEPRECATED)
	{
		for (uint32_t i = 0; i < data.ProductUserIdCount; i++)
			ProductUserIds.Add(data.ProductUserIds[i]);
	}
};

/**
 * Output parameters for the EOS_Connect_QueryProductUserIdMappings Function.
 */
USTRUCT(BlueprintType)
struct FEOSConnectQueryProductUserIdMappingsCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful query, otherwise one of the error codes is returned. See eos_result.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Connect_QueryProductUserIdMappings */
	void* ClientData;
	/** Existing logged in user that made the request */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSProductUserId LocalUserId;
public:
	explicit FEOSConnectQueryProductUserIdMappingsCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		  , ClientData(nullptr)
	{
	}

	FEOSConnectQueryProductUserIdMappingsCallbackInfo(const EOS_Connect_QueryProductUserIdMappingsCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
	{
	}
};

/**
 * Input parameters for the EOS_Connect_GetProductUserIdMapping Function.
 */
USTRUCT(BlueprintType)
struct FEOSConnectGetProductUserIdMappingOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Existing logged in user that is querying account mappings */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSProductUserId LocalUserId;
	/** External auth service supplying the account ids in string form */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	EEOSEExternalAccountType AccountIdType;
	/** Target product user id to retrieve */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSProductUserId TargetProductUserId;
public:
	explicit FEOSConnectGetProductUserIdMappingOptions()
		: ApiVersion(EOS_CONNECT_GETPRODUCTUSERIDMAPPING_API_LATEST)
		  , AccountIdType(EEOSEExternalAccountType::EOS_EAT_EPIC)
	{
	}

	FEOSConnectGetProductUserIdMappingOptions(const EOS_Connect_GetProductUserIdMappingOptions& data)
		: ApiVersion(EOS_CONNECT_GETPRODUCTUSERIDMAPPING_API_LATEST)
		  , LocalUserId(data.LocalUserId)
		  , AccountIdType(static_cast<EEOSEExternalAccountType>(data.AccountIdType))
		  , TargetProductUserId(data.TargetProductUserId)
	{
	}
};

/**
 * Input parameters for the EOS_Connect_GetProductUserExternalAccountCount Function.
 */
USTRUCT(BlueprintType)
struct FEOSConnectGetProductUserExternalAccountCountOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Product user id to look for when getting external account info count from the cache. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSProductUserId TargetUserId;
public:
	explicit FEOSConnectGetProductUserExternalAccountCountOptions()
		: ApiVersion(EOS_CONNECT_GETPRODUCTUSEREXTERNALACCOUNTCOUNT_API_LATEST)
	{
	}

	FEOSConnectGetProductUserExternalAccountCountOptions(const EOS_Connect_GetProductUserExternalAccountCountOptions& data)
		: ApiVersion(EOS_CONNECT_GETPRODUCTUSEREXTERNALACCOUNTCOUNT_API_LATEST)
		  , TargetUserId(data.TargetUserId)
	{
	}
};

/**
 * Input parameters for the EOS_Connect_CopyProductUserExternalAccountByIndex Function.
 */
USTRUCT(BlueprintType)
struct FEOSConnectCopyProductUserExternalAccountByIndexOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Product user id to look for when copying external account info from the cache. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSProductUserId TargetUserId;
	/** Index of the external account info to retrieve from the cache. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	int32 ExternalAccountInfoIndex;
public:
	explicit FEOSConnectCopyProductUserExternalAccountByIndexOptions()
		: ApiVersion(EOS_CONNECT_COPYPRODUCTUSEREXTERNALACCOUNTBYINDEX_API_LATEST)
		  , ExternalAccountInfoIndex(0)
	{
	}

	FEOSConnectCopyProductUserExternalAccountByIndexOptions(const EOS_Connect_CopyProductUserExternalAccountByIndexOptions& data)
		: ApiVersion(EOS_CONNECT_COPYPRODUCTUSEREXTERNALACCOUNTBYINDEX_API_LATEST)
		  , TargetUserId(data.TargetUserId)
		  , ExternalAccountInfoIndex(data.ExternalAccountInfoIndex)
	{
	}
};

/**
 * Input parameters for the EOS_Connect_CopyProductUserExternalAccountByAccountType Function.
 */
USTRUCT(BlueprintType)
struct FEOSConnectCopyProductUserExternalAccountByAccountTypeOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Product user id to look for when copying external account info from the cache. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSProductUserId TargetUserId;
	/** External auth service account type to look for when copying external account info from the cache. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	EEOSEExternalAccountType AccountIdType;
public:
	explicit FEOSConnectCopyProductUserExternalAccountByAccountTypeOptions()
		: ApiVersion(EOS_CONNECT_COPYPRODUCTUSEREXTERNALACCOUNTBYACCOUNTTYPE_API_LATEST)
		  , AccountIdType(EEOSEExternalAccountType::EOS_EAT_EPIC)
	{
	}

	FEOSConnectCopyProductUserExternalAccountByAccountTypeOptions(const EOS_Connect_CopyProductUserExternalAccountByAccountTypeOptions& data)
		: ApiVersion(EOS_CONNECT_COPYPRODUCTUSEREXTERNALACCOUNTBYACCOUNTTYPE_API_LATEST)
		  , TargetUserId(data.TargetUserId)
		  , AccountIdType(static_cast<EEOSEExternalAccountType>(data.AccountIdType))
	{
	}
};

/**
 * Input parameters for the EOS_Connect_CopyProductUserExternalAccountByAccountId Function.
 */
USTRUCT(BlueprintType)
struct FEOSConnectCopyProductUserExternalAccountByAccountIdOptions
{
	GENERATED_BODY()
public:
	/** Product user id to look for when copying external account info from the cache. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSProductUserId TargetUserId;
	/** External auth service account id to look for when copying external account info from the cache. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FString AccountId;
public:
	explicit FEOSConnectCopyProductUserExternalAccountByAccountIdOptions() = default;

	FEOSConnectCopyProductUserExternalAccountByAccountIdOptions(const EOS_Connect_CopyProductUserExternalAccountByAccountIdOptions& data)
		: TargetUserId(data.TargetUserId)
	{
		const FUTF8ToTCHAR AccountIdChar(data.AccountId);
		AccountId = AccountIdChar.Get();
	}
};

/**
 * Input parameters for the EOS_Connect_CopyProductUserInfo Function.
 */
USTRUCT(BlueprintType)
struct FEOSConnectCopyProductUserInfoOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Product user id to look for when copying external account info from the cache. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSProductUserId TargetUserId;
public:
	explicit FEOSConnectCopyProductUserInfoOptions()
		: ApiVersion(EOS_CONNECT_COPYPRODUCTUSERINFO_API_LATEST)
	{
	}

	FEOSConnectCopyProductUserInfoOptions(const EOS_Connect_CopyProductUserInfoOptions& data)
		: ApiVersion(EOS_CONNECT_COPYPRODUCTUSERINFO_API_LATEST)
		  , TargetUserId(data.TargetUserId)
	{
	}
};

/**
 * Contains information about an external account info
 */
USTRUCT(BlueprintType)
struct FEOSConnectExternalAccountInfo
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Product user id of the target user. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSProductUserId ProductUserId;
	/** Display name, can be null if not set. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FString DisplayName;
	/**
	 * External account ID.
	 *
	 * May be set to an empty string if the AccountIdType of another user belongs
	 * to different account system than the local user's authenticated account.
	 * The availability of this field is dependent on account system specifics.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FString AccountId;
	/** The identity provider that owns the external account. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	EEOSEExternalAccountType AccountIdType;
	/** The POSIX timestamp for the time the user last logged in, or EOS_CONNECT_TIME_UNDEFINED. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FString LastLoginTime;
public:
	explicit FEOSConnectExternalAccountInfo()
		: ApiVersion(EOS_CONNECT_EXTERNALACCOUNTINFO_API_LATEST)
		  , AccountIdType(EEOSEExternalAccountType::EOS_EAT_EPIC)
	{
	}

	FEOSConnectExternalAccountInfo(const EOS_Connect_ExternalAccountInfo& data)
		: ApiVersion(EOS_CONNECT_EXTERNALACCOUNTINFO_API_LATEST)
		  , ProductUserId(data.ProductUserId)
		  , AccountIdType(static_cast<EEOSEExternalAccountType>(data.AccountIdType))
		  , LastLoginTime(LexToString(data.LastLoginTime))
	{
		const FUTF8ToTCHAR DisplayNameChar(data.DisplayName);
		DisplayName = DisplayNameChar.Get();

		const FUTF8ToTCHAR AccountIdChar(data.AccountId);
		AccountId = AccountIdChar.Get();
	}
};

/**
 * Structure containing information for the auth expiration notification callback
 */
USTRUCT(BlueprintType)
struct FEOSConnectAddNotifyAuthExpirationOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	explicit FEOSConnectAddNotifyAuthExpirationOptions()
		: ApiVersion(EOS_CONNECT_ADDNOTIFYAUTHEXPIRATION_API_LATEST)
	{
	}

	FEOSConnectAddNotifyAuthExpirationOptions(const EOS_Connect_AddNotifyAuthExpirationOptions& data)
		: ApiVersion(EOS_CONNECT_ADDNOTIFYAUTHEXPIRATION_API_LATEST)
	{
	}
};

/**
 * Output parameters for the EOS_Connect_OnAuthExpirationCallback Function.
 */
USTRUCT(BlueprintType)
struct FEOSConnectAuthExpirationCallbackInfo
{
	GENERATED_BODY()
public:
	/** Context that was passed into EOS_Connect_AddNotifyAuthExpiration */
	void* ClientData;
	/** Account ID of the local player whose status has changed */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSProductUserId LocalUserId;
public:
	explicit FEOSConnectAuthExpirationCallbackInfo()
		: ClientData(nullptr)
	{
	}

	FEOSConnectAuthExpirationCallbackInfo(const EOS_Connect_AuthExpirationCallbackInfo& data)
		: ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
	{
	}
};

/**
 * Structure containing information or the connect user login status change callback
 */
USTRUCT(BlueprintType)
struct FEOSConnectAddNotifyLoginStatusChangedOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	explicit FEOSConnectAddNotifyLoginStatusChangedOptions()
		: ApiVersion(EOS_CONNECT_ADDNOTIFYLOGINSTATUSCHANGED_API_LATEST)
	{
	}

	FEOSConnectAddNotifyLoginStatusChangedOptions(const EOS_Connect_AddNotifyLoginStatusChangedOptions& data)
		: ApiVersion(EOS_CONNECT_ADDNOTIFYLOGINSTATUSCHANGED_API_LATEST)
	{
	}
};

/**
 * Output parameters for the EOS_Connect_OnLoginStatusChangedCallback Function.
 */
USTRUCT(BlueprintType)
struct FEOSConnectLoginStatusChangedCallbackInfo
{
	GENERATED_BODY()
public:
	/** Context that was passed into EOS_Connect_AddNotifyAuthExpiration */
	void* ClientData;
	/** Account ID of the local player whose status has changed */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSProductUserId LocalUserId;
	/** The status prior to the change */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	EEOSELoginStatus PreviousStatus;
	/** The status at the time of the notification */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	EEOSELoginStatus CurrentStatus;
public:
	explicit FEOSConnectLoginStatusChangedCallbackInfo()
		: ClientData(nullptr)
		  , PreviousStatus(EEOSELoginStatus::EOS_LS_NotLoggedIn)
		  , CurrentStatus(EEOSELoginStatus::EOS_LS_NotLoggedIn)
	{
	}

	FEOSConnectLoginStatusChangedCallbackInfo(const EOS_Connect_LoginStatusChangedCallbackInfo& data)
		: ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
		  , PreviousStatus(static_cast<EEOSELoginStatus>(data.PreviousStatus))
		  , CurrentStatus(static_cast<EEOSELoginStatus>(data.CurrentStatus))
	{
	}
};

/**
* Input parameters for the EOS_Connect_CopyIdToken function.
*/
USTRUCT(BlueprintType)
struct FEOSConnectCopyIdTokenOptions
{
	GENERATED_BODY()
public:
	/** The local Product User ID whose ID token should be copied. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSProductUserId LocalUserId;
public:
	FEOSConnectCopyIdTokenOptions() = default;
};

USTRUCT(BlueprintType)
struct FEOSConnectIdToken
{
	GENERATED_BODY()
public:
	/**
	* The Product User ID described by the ID token.
	* Use EOS_ProductUserId_FromString to populate this field when validating a received ID token.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSProductUserId ProductUserId;
	/** The ID token as a Json Web Token (JWT) string. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FString JsonWebToken;
public:
	FEOSConnectIdToken() = default;

	FEOSConnectIdToken(const EOS_Connect_IdToken& data)
	{
		ProductUserId = data.ProductUserId;
		const FUTF8ToTCHAR JsonWebTokenChar(data.JsonWebToken);
		JsonWebToken = JsonWebTokenChar.Get();
	}
};

USTRUCT(BlueprintType)
struct FEOSConnectVerifyIdTokenOptions
{
	GENERATED_BODY()
public:
	/**
	* The ID token to verify.
	* Use EOS_ProductUserId_FromString to populate the ProductUserId field of this struct.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSConnectIdToken IdToken;
public:
	FEOSConnectVerifyIdTokenOptions() = default;
};

USTRUCT(BlueprintType)
struct FEOSConnectOnVerifyIdTokenCallback
{
	GENERATED_BODY()
public:
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Connect_VerifyIdToken */
	void* ClientData;
	/** The Product User ID associated with the ID token. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FEOSProductUserId ProductUserId;
	/**
	 * Flag set to indicate whether account information is available.
	 * Applications must always first check this value to be set before attempting
	 * to read the AccountType, AccountId, Platform and DeviceType fields.
	 *
	 * This flag is always false for users that authenticated using EOS Connect Device ID.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	bool bIsAccountInfoPresent;
	/**
	 * The identity provider that the user authenticated with to EOS Connect.
	 *
	 * If bIsAccountInfoPresent is set, this field describes the external account type.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	EEOSEExternalAccountType AccountIdType;
	/**
	 * The external account ID of the authenticated user.
	 *
	 * This value may be set to an empty string.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FString AccountId;
	/**
	 * Platform that the user is connected from.
	 *
	 * This value may be set to an empty string.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FString Platform;
	/**
	 * Identifies the device type that the user is connected from.
	 * Can be used to securely verify that the user is connected through a real Console device.
	 *
	 * This value may be set to an empty string.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FString DeviceType;

	/**
	 * Client ID of the authorized client.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FString ClientId;
	/**
	 * Product ID.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FString ProductId;
	/**
	 * Sandbox ID.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FString SandboxId;
	/**
	 * Deployment ID.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Connect")
	FString DeploymentId;
public:
	FEOSConnectOnVerifyIdTokenCallback()
		: ResultCode(EOSResult::EOS_UnexpectedError)
		  , ClientData(nullptr)
		  , bIsAccountInfoPresent(false)
		  , AccountIdType(EEOSEExternalAccountType::EOS_EAT_EPIC)
	{
	}

	FEOSConnectOnVerifyIdTokenCallback(const EOS_Connect_VerifyIdTokenCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
		  , ProductUserId(data.ProductUserId)
		  , bIsAccountInfoPresent((data.bIsAccountInfoPresent > 0))
		  , AccountIdType(static_cast<EEOSEExternalAccountType>(data.AccountIdType))
	{
		const FUTF8ToTCHAR AccountIdChar(data.AccountId);
		AccountId = AccountIdChar.Get();

		const FUTF8ToTCHAR PlatformChar(data.Platform);
		Platform = PlatformChar.Get();

		const FUTF8ToTCHAR DeviceTypeChar(data.DeviceType);
		DeviceType = DeviceTypeChar.Get();

		const FUTF8ToTCHAR ClientIdChar(data.ClientId);
		ClientId = ClientIdChar.Get();

		const FUTF8ToTCHAR ProductIdChar(data.ProductId);
		ProductId = ProductIdChar.Get();

		const FUTF8ToTCHAR SandboxIdChar(data.SandboxId);
		SandboxId = SandboxIdChar.Get();

		const FUTF8ToTCHAR DeploymentIdChar(data.DeploymentId);
		DeploymentId = DeploymentIdChar.Get();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		DELEGATES
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLoginCallback, const FEOSConnectLoginCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCreateUserCallback, const FEOSConnectCreateUserCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLinkAccountCallback, const FEOSConnectLinkAccountCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnUnlinkAccountCallback, const FEOSConnectUnlinkAccountCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCreateDeviceIdCallback, const FEOSConnectCreateDeviceIdCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnDeleteDeviceIdCallback, const FEOSConnectDeleteDeviceIdCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnTransferDeviceIdAccountCallback, const FEOSConnectTransferDeviceIdAccountCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnQueryExternalAccountMappingsCallback, const FEOSConnectQueryExternalAccountMappingsCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnQueryProductUserIdMappingsCallback, const FEOSConnectQueryProductUserIdMappingsCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLoginStatusChangedCallback, const FEOSConnectLoginStatusChangedCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAuthExpirationCallback, const FEOSConnectAuthExpirationCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnConnectVerifyIdTokenCallback, const FEOSConnectOnVerifyIdTokenCallback&, Data);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginCallbackDelegate, const FEOSConnectLoginCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreateUserCallbackDelegate, const FEOSConnectCreateUserCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLinkAccountCallbackDelegate, const FEOSConnectLinkAccountCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnlinkAccountCallbackDelegate, const FEOSConnectUnlinkAccountCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreateDeviceIdCallbackDelegate, const FEOSConnectCreateDeviceIdCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeleteDeviceIdCallbackDelegate, const FEOSConnectDeleteDeviceIdCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTransferDeviceIdAccountCallbackDelegate, const FEOSConnectTransferDeviceIdAccountCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQueryExternalAccountMappingsCallbackDelegate, const FEOSConnectQueryExternalAccountMappingsCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQueryProductUserIdMappingsCallbackDelegate, const FEOSConnectQueryProductUserIdMappingsCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAuthExpirationCallbackDelegate, const FEOSConnectAuthExpirationCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginStatusChangedCallbackDelegate, const FEOSConnectLoginStatusChangedCallbackInfo&, Data);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		CALLBACK OBJECTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

struct FLoginCallback
{
public:
	FOnLoginCallback m_Callback;
public:
	FLoginCallback(const FOnLoginCallback& Callback)
		: m_Callback(Callback)
	{
	}

	virtual ~FLoginCallback()
	{
		m_Callback.Unbind();
	}
};

struct FQueryExternalAccountMappingsCallback
{
public:
	FOnQueryExternalAccountMappingsCallback m_Callback;
public:
	FQueryExternalAccountMappingsCallback(const FOnQueryExternalAccountMappingsCallback& Callback)
		: m_Callback(Callback)
	{
	}

	virtual ~FQueryExternalAccountMappingsCallback()
	{
		m_Callback.Unbind();
	}
};

struct FDeleteDeviceIdCallback
{
public:
	FOnDeleteDeviceIdCallback m_Callback;
public:
	FDeleteDeviceIdCallback(const FOnDeleteDeviceIdCallback& Callback)
		: m_Callback(Callback)
	{
	}

	virtual ~FDeleteDeviceIdCallback()
	{
		m_Callback.Unbind();
	}
};

struct FTransferDeviceIdAccountCallback
{
public:
	FOnTransferDeviceIdAccountCallback m_Callback;
public:
	FTransferDeviceIdAccountCallback(const FOnTransferDeviceIdAccountCallback& Callback)
		: m_Callback(Callback)
	{
	}

	virtual ~FTransferDeviceIdAccountCallback()
	{
		m_Callback.Unbind();
	}
};

struct FCreateDeviceIdCallback
{
public:
	FOnCreateDeviceIdCallback m_Callback;
public:
	FCreateDeviceIdCallback(const FOnCreateDeviceIdCallback& Callback)
		: m_Callback(Callback)
	{
	}

	virtual ~FCreateDeviceIdCallback()
	{
		m_Callback.Unbind();
	}
};

struct FLinkAccountCallback
{
public:
	FOnLinkAccountCallback m_Callback;
public:
	FLinkAccountCallback(const FOnLinkAccountCallback& Callback)
		: m_Callback(Callback)
	{
	}

	virtual ~FLinkAccountCallback()
	{
		m_Callback.Unbind();
	}
};

struct FUnlinkAccountCallback
{
public:
	FOnUnlinkAccountCallback m_Callback;
public:
	FUnlinkAccountCallback(const FOnUnlinkAccountCallback& Callback)
		: m_Callback(Callback)
	{
	}

	virtual ~FUnlinkAccountCallback()
	{
		m_Callback.Unbind();
	}
};

struct FCreateUserCallback
{
public:
	FOnCreateUserCallback m_Callback;
public:
	FCreateUserCallback(const FOnCreateUserCallback& Callback)
		: m_Callback(Callback)
	{
	}

	virtual ~FCreateUserCallback()
	{
		m_Callback.Unbind();
	}
};

struct FQueryProductUserIdMappingsCallback
{
public:
	FOnQueryProductUserIdMappingsCallback m_Callback;
public:
	FQueryProductUserIdMappingsCallback(const FOnQueryProductUserIdMappingsCallback& Callback)
		: m_Callback(Callback)
	{
	}

	virtual ~FQueryProductUserIdMappingsCallback()
	{
		m_Callback.Unbind();
	}
};

struct FAuthExpirationCallback
{
public:
	FAuthExpirationCallback(const FOnAuthExpirationCallback& Callback)
		: Callback(Callback)
	{
	}

	virtual ~FAuthExpirationCallback()
	{
		Callback.Unbind();
	}

public:
	FOnAuthExpirationCallback Callback;
};

struct FLoginStatusChangedCallback
{
public:
	FLoginStatusChangedCallback(const FOnLoginStatusChangedCallback& Callback)
		: Callback(Callback)
	{
	}

	virtual ~FLoginStatusChangedCallback()
	{
		Callback.Unbind();
	}

public:
	FOnLoginStatusChangedCallback Callback;
};

struct FConnectAuthExpirationCallback
{
public:
	FConnectAuthExpirationCallback(const FOnConnectVerifyIdTokenCallback& Callback)
		: Callback(Callback)
	{
	}

	virtual ~FConnectAuthExpirationCallback()
	{
		Callback.Unbind();
	}

public:
	FOnConnectVerifyIdTokenCallback Callback;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		Operations
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

struct FQueryExternalAccountMappingsOperation : public EOS_Connect_QueryExternalAccountMappingsOptions
{
	TArray<char*> PointerArray;

	FQueryExternalAccountMappingsOperation(int32 numExternalAccountIds, EOS_ProductUserId localUserId, EOS_EExternalAccountType accountIdType)
		: EOS_Connect_QueryExternalAccountMappingsOptions()
	{
		PointerArray.AddZeroed(numExternalAccountIds);

		for (int32 i = 0; i < numExternalAccountIds; i++)
		{
			PointerArray[i] = new char[EOS_CONNECT_EXTERNAL_ACCOUNT_ID_MAX_LENGTH];
		}

		ApiVersion = EOS_CONNECT_QUERYEXTERNALACCOUNTMAPPINGS_API_LATEST;
		LocalUserId = localUserId;
		AccountIdType = accountIdType;
		ExternalAccountIds = const_cast<const char**>(PointerArray.GetData());
		ExternalAccountIdCount = numExternalAccountIds;
	}

	~FQueryExternalAccountMappingsOperation()
	{
		for (int32 i = 0; i < PointerArray.Num(); i++)
		{
			delete[] PointerArray[i];
		}
	}
};

struct FConnectQueryProductUserIdMappingsOptions : public EOS_Connect_QueryProductUserIdMappingsOptions
{
	TArray<EOS_ProductUserId> PointerArray;

	FConnectQueryProductUserIdMappingsOptions(int32 productUserIdCount)
		: EOS_Connect_QueryProductUserIdMappingsOptions()
	{
		ApiVersion = EOS_CONNECT_QUERYPRODUCTUSERIDMAPPINGS_API_LATEST;

		PointerArray.AddZeroed(productUserIdCount);

		ProductUserIds = PointerArray.GetData();
		ProductUserIdCount = PointerArray.Num();
	}

	~FConnectQueryProductUserIdMappingsOptions()
	{
	}
};


struct FConnectCreateDeviceIdOptions : public EOS_Connect_CreateDeviceIdOptions
{
	FConnectCreateDeviceIdOptions(FString deviceModel)
		: EOS_Connect_CreateDeviceIdOptions()
	{
		ApiVersion = EOS_CONNECT_CREATEDEVICEID_API_LATEST;

		if (!deviceModel.IsEmpty())
		{
			DeviceModel = new char[EOS_CONNECT_CREATEDEVICEID_DEVICEMODEL_MAX_LENGTH];
			FCStringAnsi::Strncpy(const_cast<char*>(DeviceModel), TCHAR_TO_UTF8(*deviceModel), EOS_CONNECT_CREATEDEVICEID_DEVICEMODEL_MAX_LENGTH);
		}
	}

	~FConnectCreateDeviceIdOptions()
	{
		delete[] DeviceModel;
	}
};

struct FGetExternalAccountMappingsOptions : public EOS_Connect_GetExternalAccountMappingsOptions
{
	FGetExternalAccountMappingsOptions(FString targetExternalUserId)
		: EOS_Connect_GetExternalAccountMappingsOptions()
	{
		ApiVersion = EOS_CONNECT_GETEXTERNALACCOUNTMAPPINGS_API_LATEST;

		if (!targetExternalUserId.IsEmpty())
		{
			TargetExternalUserId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(TargetExternalUserId), TCHAR_TO_UTF8(*targetExternalUserId), 256);
		}
	}

	~FGetExternalAccountMappingsOptions()
	{
		delete[] TargetExternalUserId;
	}
};


struct FCopyProductUserExternalAccountByAccountIdOptions : public EOS_Connect_CopyProductUserExternalAccountByAccountIdOptions
{
	FCopyProductUserExternalAccountByAccountIdOptions(FString accoutnId)
		: EOS_Connect_CopyProductUserExternalAccountByAccountIdOptions()
	{
		ApiVersion = EOS_CONNECT_COPYPRODUCTUSEREXTERNALACCOUNTBYACCOUNTID_API_LATEST;

		if (!accoutnId.IsEmpty())
		{
			AccountId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(AccountId), TCHAR_TO_UTF8(*accoutnId), 256);
		}
	}

	~FCopyProductUserExternalAccountByAccountIdOptions()
	{
		delete[] AccountId;
	}
};
