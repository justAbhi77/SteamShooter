/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "eos_auth_types.h"
#include "eos_rtc_admin_types.h"
#include "eos_ui_types.h"
#include "eos_titlestorage_types.h"
#include "eos_rtc_audio_types.h"
#include "eos_types.h"
#include "EOSTypes.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogEOSCore, Log, All);

UENUM(BlueprintType)
enum class EEOSChannelType : uint8
{
	DEFAULT = 0,
	DATA_REQUEST = 251,
	DATA_HANDSHAKE = 252,
	DATA_SUBSYSTEM = 253,
	DATA_CHAT = 254
};

UENUM(BlueprintType)
enum class EOSResult : uint8
{
	EOS_Success,
	/** Failed due to no connection */
	EOS_NoConnection,
	/** Failed login due to invalid credentials */
	EOS_InvalidCredentials,
	/** Failed due to invalid or missing user */
	EOS_InvalidUser,
	/** Failed due to invalid or missing authentication token for user (e.g. not logged in) */
	EOS_InvalidAuth,
	/** Failed due to invalid access */
	EOS_AccessDenied,
	/** If the client does not possess the permission required */
	EOS_MissingPermissions,
	/** If the token provided does not represent an account */
	EOS_Token_Not_Account,
	/** Throttled due to too many requests */
	EOS_TooManyRequests,
	/** Async request was already pending */
	EOS_AlreadyPending,
	/** Invalid parameters specified for request */
	EOS_InvalidParameters,
	/** Invalid request */
	EOS_InvalidRequest,
	/** Failed due to unable to parse or recognize a backend response */
	EOS_UnrecognizedResponse,
	/** Incompatible client for backend version */
	EOS_IncompatibleVersion,
	/** Not configured correctly for use */
	EOS_NotConfigured,
	/** Already configured for use. */
	EOS_AlreadyConfigured,
	/** Feature not available on this implementation */
	EOS_NotImplemented,
	/** Operation was canceled (likely by user) */
	EOS_Canceled,
	/** The requested information was not found */
	EOS_NotFound,
	/** An error occurred during an asynchronous operation, and it will be retried. Callbacks receiving this result will be called again in the future. */
	EOS_OperationWillRetry,
	/** The request had no effect */
	EOS_NoChange,
	/** The request attempted to use multiple or inconsistent API versions */
	EOS_VersionMismatch,
	/** A maximum limit was exceeded on the client, different from EOS_TooManyRequests */
	EOS_LimitExceeded,
	/** Feature or client ID performing the operation has been disabled. */
	EOS_Disabled,
	/** Duplicate entry not allowed */
	EOS_DuplicateNotAllowed,
	/** Required parameters are missing. DEPRECATED: This error is no longer used. */
	EOS_MissingParameters_DEPRECATED,
	/** Sandbox ID is invalid */
	EOS_InvalidSandboxId,
	/** Request timed out */
	EOS_TimedOut,
	/** A query returned some but not all of the requested results.  */
	EOS_PartialResult,
	/** Client is missing the whitelisted role */
	EOS_Missing_Role,
	/** Client is missing the whitelisted feature */
	EOS_Missing_Feature,
	/** The sandbox given to the backend is invalid */
	EOS_Invalid_Sandbox,
	/** The deployment given to the backend is invalid */
	EOS_Invalid_Deployment,
	/** The product ID specified to the backend is invalid */
	EOS_Invalid_Product,
	/** The product user ID specified to the backend is invalid */
	EOS_Invalid_ProductUserID,
	/** There was a failure with the backend service */
	EOS_ServiceFailure,
	/** Cache directory is not set in platform options. */
	EOS_CacheDirectoryMissing,
	/** Cache directory is not accessible. */
	EOS_CacheDirectoryInvalid,
	/** The request failed because resource was in an invalid state */
	EOS_InvalidState,
	/** Request is in progress */
	EOS_RequestInProgress,
	/** Application is suspended */
	EOS_ApplicationSuspended,
	/** Network is disconnected */
	EOS_NetworkDisconnected,

	/** Account locked due to login failures */
	EOS_Auth_AccountLocked,
	/** Account locked by update operation. */
	EOS_Auth_AccountLockedForUpdate,
	/** Refresh token used was invalid */
	EOS_Auth_InvalidRefreshToken,
	/** Invalid access token, typically when switching between backend environments */
	EOS_Auth_InvalidToken,
	/** Invalid bearer token */
	EOS_Auth_AuthenticationFailure,
	/** Invalid platform token */
	EOS_Auth_InvalidPlatformToken,
	/** Auth parameters are not associated with this account */
	EOS_Auth_WrongAccount,
	/** Auth parameters are not associated with this client */
	EOS_Auth_WrongClient,
	/** Full account is required */
	EOS_Auth_FullAccountRequired,
	/** Headless account is required */
	EOS_Auth_HeadlessAccountRequired,
	/** Password reset is required */
	EOS_Auth_PasswordResetRequired,
	/** Password was previously used and cannot be reused */
	EOS_Auth_PasswordCannotBeReused,
	/** Authorization code/exchange code has expired */
	EOS_Auth_Expired,
	/** Consent has not been given by the user */
	EOS_Auth_ScopeConsentRequired,
	/** The application has no profile on the backend */
	EOS_Auth_ApplicationNotFound,
	/** The requested consent wasn't found on the backend */
	EOS_Auth_ScopeNotFound,
	/** This account has been denied access to login */
	EOS_Auth_AccountFeatureRestricted,
	/** The overlay failed to load the Account Portal. This can range from general overlay failure, to overlay failed to connect to the web server, to overlay failed to render the web page. */
	EOS_Auth_AccountPortalLoadError,
	/** An attempted login has failed due to the user needing to take corrective action on their account. */
	EOS_Auth_CorrectiveActionRequired,

	/** Pin grant code initiated */
	EOS_Auth_PinGrantCode,
	/** Pin grant code attempt expired */
	EOS_Auth_PinGrantExpired,
	/** Pin grant code attempt pending */
	EOS_Auth_PinGrantPending,

	/** External auth source did not yield an account */
	EOS_Auth_ExternalAuthNotLinked,
	/** External auth access revoked */
	EOS_Auth_ExternalAuthRevoked,
	/** External auth token cannot be interpreted */
	EOS_Auth_ExternalAuthInvalid,
	/** External auth cannot be linked to his account due to restrictions */
	EOS_Auth_ExternalAuthRestricted,
	/** External auth cannot be used for login */
	EOS_Auth_ExternalAuthCannotLogin,
	/** External auth is expired */
	EOS_Auth_ExternalAuthExpired,
	/** External auth cannot be removed since it's the last possible way to login */
	EOS_Auth_ExternalAuthIsLastLoginType,

	/** Exchange code not found */
	EOS_Auth_ExchangeCodeNotFound,
	/** Originating exchange code session has expired */
	EOS_Auth_OriginatingExchangeCodeSessionExpired,

	/** The account has been disabled and cannot be used for authentication */
	EOS_Auth_AccountNotActive,

	/** MFA challenge required */
	EOS_Auth_MFARequired,

	/** Parental locks are in place */
	EOS_Auth_ParentalControls,

	/** Korea real ID association required but missing */
	EOS_Auth_NoRealId,
	
	/** Silent login failed when EOS_LF_NO_USER_INTERFACE was specified, and user interaction is needed before the user can be logged in. */
	EOS_Auth_UserInterfaceRequired,

	/** An outgoing friend invitation is awaiting acceptance; sending another invite to the same user is erroneous */
	EOS_Friends_InviteAwaitingAcceptance,
	/** There is no friend invitation to accept/reject */
	EOS_Friends_NoInvitation,
	/** Users are already friends, so sending another invite is erroneous */
	EOS_Friends_AlreadyFriends,
	/** Users are not friends, so deleting the friend is erroneous */
	EOS_Friends_NotFriends,
	/** Remote user has too many invites to receive new invites */
	EOS_Friends_TargetUserTooManyInvites,
	/** Local user has too many invites to send new invites */
	EOS_Friends_LocalUserTooManyInvites,
	/** Remote user has too many friends to make a new friendship */
	EOS_Friends_TargetUserFriendLimitExceeded,
	/** Local user has too many friends to make a new friendship */
	EOS_Friends_LocalUserFriendLimitExceeded,

	/** Request data was null or invalid */
	EOS_Presence_DataInvalid,
	/** Request contained too many or too few unique data items, or the request would overflow the maximum amount of data allowed */
	EOS_Presence_DataLengthInvalid,
	/** Request contained data with an invalid key */
	EOS_Presence_DataKeyInvalid,
	/** Request contained data with a key too long or too short */
	EOS_Presence_DataKeyLengthInvalid,
	/** Request contained data with an invalid value */
	EOS_Presence_DataValueInvalid,
	/** Request contained data with a value too long */
	EOS_Presence_DataValueLengthInvalid,
	/** Request contained an invalid rich text string */
	EOS_Presence_RichTextInvalid,
	/** Request contained a rich text string that was too long */
	EOS_Presence_RichTextLengthInvalid,
	/** Request contained an invalid status state */
	EOS_Presence_StatusInvalid,

	/** The entitlement retrieved is stale, requery for updated information */
	EOS_Ecom_EntitlementStale,
	/** The offer retrieved is stale, requery for updated information */
	EOS_Ecom_CatalogOfferStale,
	/** The item or associated structure retrieved is stale, requery for updated information */
	EOS_Ecom_CatalogItemStale,
	/** The one or more offers has an invalid price. This may be caused by the price setup. */
	EOS_Ecom_CatalogOfferPriceInvalid,
	/** The checkout page failed to load */
	EOS_Ecom_CheckoutLoadError,
	/** The player closed the purchase flow overlay after clicking the purchase button. The purchase may still go through, and the game needs to query unredeemed entitlements for a short time. */
	EOS_Ecom_PurchaseProcessing,
	/** Session is already in progress */
	EOS_Sessions_SessionInProgress,
	/** Too many players to register with this session */
	EOS_Sessions_TooManyPlayers,
	/** Client has no permissions to access this session */
	EOS_Sessions_NoPermission,
	/** Session already exists in the system */
	EOS_Sessions_SessionAlreadyExists,
	/** Session lock required for operation */
	EOS_Sessions_InvalidLock,
	/** Invalid session reference */
	EOS_Sessions_InvalidSession,
	/** Sandbox ID associated with auth didn't match */
	EOS_Sessions_SandboxNotAllowed,
	/** Invite failed to send */
	EOS_Sessions_InviteFailed,
	/** Invite was not found with the service */
	EOS_Sessions_InviteNotFound,
	/** This client may not modify the session */
	EOS_Sessions_UpsertNotAllowed,
	/** Backend nodes unavailable to process request */
	EOS_Sessions_AggregationFailed,
	/** Individual backend node is as capacity */
	EOS_Sessions_HostAtCapacity,
	/** Sandbox on node is at capacity */
	EOS_Sessions_SandboxAtCapacity,
	/** An anonymous operation was attempted on a non anonymous session */
	EOS_Sessions_SessionNotAnonymous,
	/** Session is currently out of sync with the backend, data is saved locally but needs to sync with backend */
	EOS_Sessions_OutOfSync,
	/** User has received too many invites */
	EOS_Sessions_TooManyInvites,
	/** Presence session already exists for the client */
	EOS_Sessions_PresenceSessionExists,
	/** Deployment on node is at capacity */
	EOS_Sessions_DeploymentAtCapacity,
	/** Session operation not allowed */
	EOS_Sessions_NotAllowed,
	/** Session operation not allowed */
	EOS_Sessions_PlayerSanctioned,

	/** Request filename was invalid */
	EOS_PlayerDataStorage_FilenameInvalid,
	/** Request filename was too long */
	EOS_PlayerDataStorage_FilenameLengthInvalid,
	/** Request filename contained invalid characters */
	EOS_PlayerDataStorage_FilenameInvalidChars,
	/** Request operation would grow file too large */
	EOS_PlayerDataStorage_FileSizeTooLarge,
	/** Request file length is not valid */
	EOS_PlayerDataStorage_FileSizeInvalid,
	/** Request file handle is not valid */
	EOS_PlayerDataStorage_FileHandleInvalid,
	/** Request data is invalid */
	EOS_PlayerDataStorage_DataInvalid,
	/** Request data length was invalid */
	EOS_PlayerDataStorage_DataLengthInvalid,
	/** Request start index was invalid */
	EOS_PlayerDataStorage_StartIndexInvalid,
	/** Request is in progress */
	EOS_PlayerDataStorage_RequestInProgress,
	/** User is marked as throttled which means he can't perform some operations because limits are exceeded.  */
	EOS_PlayerDataStorage_UserThrottled,
	/** Encryption key is not set during SDK init.  */
	EOS_PlayerDataStorage_EncryptionKeyNotSet,
	/** User data callback returned error (EOS_PlayerDataStorage_EWriteResult::EOS_WR_FailRequest or EOS_PlayerDataStorage_EReadResult::EOS_RR_FailRequest) */
	EOS_PlayerDataStorage_UserErrorFromDataCallback,
	/** User is trying to read file that has header from newer version of SDK. Game/SDK needs to be updated. */
	EOS_PlayerDataStorage_FileHeaderHasNewerVersion,
	/** The file is corrupted. In some cases retry can fix the issue. */
	EOS_PlayerDataStorage_FileCorrupted,

	/** EOS Auth service deemed the external token invalid */
	EOS_Connect_ExternalTokenValidationFailed,
	/** EOS Auth user already exists */
	EOS_Connect_UserAlreadyExists,
	/** EOS Auth expired */
	EOS_Connect_AuthExpired,
	/** EOS Auth invalid token */
	EOS_Connect_InvalidToken,
	/** EOS Auth doesn't support this token type */
	EOS_Connect_UnsupportedTokenType,
	/** EOS Auth Account link failure */
	EOS_Connect_LinkAccountFailed,
	/** EOS Auth External service for validation was unavailable */
	EOS_Connect_ExternalServiceUnavailable,
	/** EOS Auth External Service configuration failure with Dev Portal */
	EOS_Connect_ExternalServiceConfigurationFailure,
	/** EOS Auth Account link failure. Tried to link Nintendo Network Service Account without first linking Nintendo Account. DEPRECATED: The requirement has been removed and this error is no longer used. */
	EOS_Connect_LinkAccountFailedMissingNintendoIdAccount_DEPRECATED,

	/** The social overlay page failed to load */
	EOS_UI_SocialOverlayLoadError,

	/** Client has no permissions to modify this lobby */
	EOS_Lobby_NotOwner,
	/** Lobby lock required for operation */
	EOS_Lobby_InvalidLock,
	/** Lobby already exists in the system */
	EOS_Lobby_LobbyAlreadyExists,
	/** Lobby is already in progress */
	EOS_Lobby_SessionInProgress,
	/** Too many players to register with this lobby */
	EOS_Lobby_TooManyPlayers,
	/** Client has no permissions to access this lobby */
	EOS_Lobby_NoPermission,
	/** Invalid lobby session reference */
	EOS_Lobby_InvalidSession,
	/** Sandbox ID associated with auth didn't match */
	EOS_Lobby_SandboxNotAllowed,
	/** Invite failed to send */
	EOS_Lobby_InviteFailed,
	/** Invite was not found with the service */
	EOS_Lobby_InviteNotFound,
	/** This client may not modify the lobby */
	EOS_Lobby_UpsertNotAllowed,
	/** Backend nodes unavailable to process request */
	EOS_Lobby_AggregationFailed,
	/** Individual backend node is as capacity */
	EOS_Lobby_HostAtCapacity,
	/** Sandbox on node is at capacity */
	EOS_Lobby_SandboxAtCapacity,
	/** User has received too many invites */
	EOS_Lobby_TooManyInvites,
	/** Deployment on node is at capacity */
	EOS_Lobby_DeploymentAtCapacity,
	/** Lobby operation not allowed */
	EOS_Lobby_NotAllowed,
	/** While restoring a lost connection lobby ownership changed and only local member data was updated */
	EOS_Lobby_MemberUpdateOnly,
	/** Presence lobby already exists for the client */
	EOS_Lobby_PresenceLobbyExists,
	/** Operation requires lobby with voice enabled */
	EOS_Lobby_VoiceNotEnabled,
	/** The client platform does not match the allowed platform list for the lobby. */
	EOS_Lobby_PlatformNotAllowed,
	/** User callback that receives data from storage returned error. */
	EOS_TitleStorage_UserErrorFromDataCallback,
	/** User forgot to set Encryption key during platform init. Title Storage can't work without it. */
	EOS_TitleStorage_EncryptionKeyNotSet,
	/** Downloaded file is corrupted. */
	EOS_TitleStorage_FileCorrupted,
	/** Downloaded file's format is newer than client SDK version. */
	EOS_TitleStorage_FileHeaderHasNewerVersion,

	/** ModSdk process is already running. This error comes from the EOSSDK. */
	EOS_Mods_ModSdkProcessIsAlreadyRunning,
	/** ModSdk command is empty. Either the ModSdk configuration file is missing or the manifest location is empty. */
	EOS_Mods_ModSdkCommandIsEmpty,
	/** Creation of the ModSdk process failed. This error comes from the SDK. */
	EOS_Mods_ModSdkProcessCreationFailed,
	/** A critical error occurred in the external ModSdk process that we were unable to resolve. */
	EOS_Mods_CriticalError,
	/** A internal error occurred in the external ModSdk process that we were unable to resolve. */
	EOS_Mods_ToolInternalError,
	/** A IPC failure occurred in the external ModSdk process. */
	EOS_Mods_IPCFailure,
	/** A invalid IPC response received in the external ModSdk process. */
	EOS_Mods_InvalidIPCResponse,
	/** A URI Launch failure occurred in the external ModSdk process. */
	EOS_Mods_URILaunchFailure,
	/** Attempting to perform an action with a mod that is not installed. This error comes from the external ModSdk process. */
	EOS_Mods_ModIsNotInstalled,
	/** Attempting to perform an action on a game that the user doesn't own. This error comes from the external ModSdk process. */
	EOS_Mods_UserDoesNotOwnTheGame,
	/** Invalid result of the request to get the offer for the mod. This error comes from the external ModSdk process. */
	EOS_Mods_OfferRequestByIdInvalidResult,
	/** Could not find the offer for the mod. This error comes from the external ModSdk process. */
	EOS_Mods_CouldNotFindOffer,
	/** Request to get the offer for the mod failed. This error comes from the external ModSdk process. */
	EOS_Mods_OfferRequestByIdFailure,
	/** Request to purchase the mod failed. This error comes from the external ModSdk process. */
	EOS_Mods_PurchaseFailure,
	/** Attempting to perform an action on a game that is not installed or is partially installed. This error comes from the external ModSdk process. */
	EOS_Mods_InvalidGameInstallInfo,
	/** Failed to get manifest location. Either the ModSdk configuration file is missing or the manifest location is empty */
	EOS_Mods_CannotGetManifestLocation,
	/** Attempting to perform an action with a mod that does not support the current operating system. */
	EOS_Mods_UnsupportedOS,

	/** The anti-cheat client protection is not available. Check that the game was started using the anti-cheat bootstrapper. */
	EOS_AntiCheat_ClientProtectionNotAvailable,
	/** The current anti-cheat mode is incorrect for using this API */
	EOS_AntiCheat_InvalidMode,
	/** The ProductId provided to the anti-cheat client helper executable does not match what was used to initialize the EOS SDK */
	EOS_AntiCheat_ClientProductIdMismatch,
	/** The SandboxId provided to the anti-cheat client helper executable does not match what was used to initialize the EOS SDK */
	EOS_AntiCheat_ClientSandboxIdMismatch,
	/** (ProtectMessage/UnprotectMessage) No session key is available, but it is required to complete this operation */
	EOS_AntiCheat_ProtectMessageSessionKeyRequired,
	/** (ProtectMessage/UnprotectMessage) Message integrity is invalid */
	EOS_AntiCheat_ProtectMessageValidationFailed,
	/** (ProtectMessage/UnprotectMessage) Initialization failed */
	EOS_AntiCheat_ProtectMessageInitializationFailed,
	/** (RegisterPeer) Peer is already registered */
	EOS_AntiCheat_PeerAlreadyRegistered,
	/** (UnregisterPeer) Peer does not exist */
	EOS_AntiCheat_PeerNotFound,
	/** (ReceiveMessageFromPeer) Invalid call: Peer is not protected */
	EOS_AntiCheat_PeerNotProtected,
	/** The DeploymentId provided to the anti-cheat client helper executable does not match what was used to initialize the EOS SDK */
	EOS_AntiCheat_ClientDeploymentIdMismatch,
	/** EOS Connect DeviceID auth method is not supported for anti-cheat */
	EOS_AntiCheat_DeviceIdAuthIsNotSupported,

	/** EOS RTC room cannot accept more participants */
	EOS_RTC_TooManyParticipants,
	/** EOS RTC room already exists*/
	EOS_RTC_RoomAlreadyExists,
	/** The user kicked out from the room */
	EOS_RTC_UserKicked,
	/** The user is banned */
	EOS_RTC_UserBanned,
	/** EOS RTC room was left successfully */
	EOS_RTC_RoomWasLeft,
	/** Connection dropped due to long timeout */
	EOS_RTC_ReconnectionTimegateExpired,
	/** EOS RTC room was left due to platform release */
	EOS_RTC_ShutdownInvoked,
	/** EOS RTC operation failed because the user is in the local user's block list */
	EOS_RTC_UserIsInBlocklist,

	/** The number of available Snapshot IDs have all been exhausted. */
	EOS_ProgressionSnapshot_SnapshotIdUnavailable,

	/** The KWS user does not have a parental email associated with the account.  The parent account was unlinked or deleted */
	EOS_KWS_ParentEmailMissing,
	/** The KWS user is no longer a minor and trying to update the parent email */
	EOS_KWS_UserGraduated,

	/** EOS Android VM not stored */
	EOS_Android_JavaVMNotStored,

	/** Patch required before the user can use the privilege */
	EOS_Permission_RequiredPatchAvailable,
	/** System update required before the user can use the privilege */
	EOS_Permission_RequiredSystemUpdate,
	/** Parental control failure usually */
	EOS_Permission_AgeRestrictionFailure,
	/** Premium Account Subscription required but not available */
	EOS_Permission_AccountTypeFailure,
	/** User restricted from chat */
	EOS_Permission_ChatRestriction,
	/** User restricted from User Generated Content */
	EOS_Permission_UGCRestriction,
	/** Online play is restricted */
	EOS_Permission_OnlinePlayRestricted,

	/** The application was not launched through the Bootstrapper. Desktop crossplay functionality is unavailable. */
	EOS_DesktopCrossplay_ApplicationNotBootstrapped,
	/** The redistributable service is not installed. */
	EOS_DesktopCrossplay_ServiceNotInstalled,
	/** The desktop crossplay service failed to start. */
	EOS_DesktopCrossplay_ServiceStartFailed,
	/** The desktop crossplay service is no longer running for an unknown reason. */
	EOS_DesktopCrossplay_ServiceNotRunning,

	/** When sending the custom invite failed. */
	EOS_CustomInvites_InviteFailed,

	/** The best display name could not be safely determined. */
	EOS_UserInfo_BestDisplayNameIndeterminate,

	/** An unexpected error that we cannot identify has occurred. */
	EOS_UnexpectedError,
	MAX
};

UENUM(BlueprintType)
enum class EOSELoginStatus : uint8
{
	/** Player has not logged in or chosen a local profile */
	EOS_LS_NotLoggedIn = 0,
	/** Player is using a local profile but is not logged in */
	EOS_LS_UsingLocalProfile = 1,
	/** Player has been validated by the platform specific authentication service */
	EOS_LS_LoggedIn = 2
};

/**
 * All supported external account providers
 *
 * @see EOS_Connect_QueryExternalAccountMappings
 */
UENUM(BlueprintType)
enum class EEOSEExternalAccountType : uint8
{
	/** External account is associated with Epic Games */
	EOS_EAT_EPIC = 0,
	/** External account is associated with Steam */
	EOS_EAT_STEAM = 1,
	/** External account is associated with PlayStation(TM)Network */
	EOS_EAT_PSN = 2,
	/**
	* External account is associated with Xbox Live
	*
	* With EOS Connect API, the associated account type is Partner XUID (PXUID).
	* With EOS UserInfo API, the associated account type is Xbox Live ID (XUID).
	*/
	EOS_EAT_XBL = 3,
	/** External account is associated with Discord */
	EOS_EAT_DISCORD = 4,
	/** External account is associated with GOG */
	EOS_EAT_GOG = 5,
	/**
	* External account is associated with Nintendo
	*
	* With both EOS Connect and EOS UserInfo APIs, the associated account type is Nintendo Service Account ID.
	* Local user authentication is possible using Nintendo Account ID, while the account type does not get exposed to the SDK in queries related to linked accounts information.
	*/
	EOS_EAT_NINTENDO = 6,
	/** External account is associated with Uplay */
	EOS_EAT_UPLAY = 7,
	/** External account is associated with an OpenID Provider */
	EOS_EAT_OPENID = 8,
	/** External account is associated with Apple */
	EOS_EAT_APPLE = 9,
	/** External account is associated with Google */
	EOS_EAT_GOOGLE = 10,
	/** External account is associated with Oculus */
	EOS_EAT_OCULUS = 11,
	/** External account is associated with itch.io */
	EOS_EAT_ITCHIO = 12,
	/** External account is associated with Amazon */
	EOS_EAT_AMAZON = 13
};

/**
 * All possible states of a local user
 *
 * @see EOS_Auth_AddNotifyLoginStatusChanged
 * @see EOS_Auth_GetLoginStatus
 * @see EOS_Auth_Login
 * @see EOS_Connect_AddNotifyLoginStatusChanged
 * @see EOS_Connect_GetLoginStatus
 * @see EOS_Connect_Login
 */
UENUM(BlueprintType)
enum class EEOSELoginStatus : uint8
{
	/** Player has not logged in or chosen a local profile */
	EOS_LS_NotLoggedIn = 0,
	/** Player is using a local profile but is not logged in */
	EOS_LS_UsingLocalProfile = 1,
	/** Player has been validated by the platform specific authentication service */
	EOS_LS_LoggedIn = 2
};

/** Supported types of data that can be stored with inside an attribute (used by sessions/lobbies/etc) */
UENUM(BlueprintType)
enum class EEOSEAttributeType : uint8
{
	/** Boolean value (true/false) */
	EOS_AT_BOOLEAN = 0,
	/** 64 bit integers */
	EOS_AT_INT64 = 1,
	/** Double/floating point precision */
	EOS_AT_DOUBLE = 2,
	/** UTF8 Strings */
	EOS_AT_STRING = 3
};

UENUM(BlueprintType)
enum class EEOSEComparisonOp : uint8
{
	/** Value must equal the one stored on the lobby/session */
	EOS_CO_EQUAL = 0,
	/** Value must not equal the one stored on the lobby/session */
	EOS_CO_NOTEQUAL = 1,
	/** Value must be strictly greater than the one stored on the lobby/session */
	EOS_CO_GREATERTHAN = 2,
	/** Value must be greater than or equal to the one stored on the lobby/session */
	EOS_CO_GREATERTHANOREQUAL = 3,
	/** Value must be strictly less than the one stored on the lobby/session */
	EOS_CO_LESSTHAN = 4,
	/** Value must be less than or equal to the one stored on the lobby/session */
	EOS_CO_LESSTHANOREQUAL = 5,
	/** Prefer values nearest the one specified ie. abs(SearchValue-SessionValue) closest to 0 */
	EOS_CO_DISTANCE = 6,
	/** Value stored on the lobby/session may be any from a specified list */
	EOS_CO_ANYOF = 7,
	/** Value stored on the lobby/session may NOT be any from a specified list */
	EOS_CO_NOTANYOF = 8
};

/**
 * All possible states of the application
 */
UENUM(BlueprintType)
enum class EOSEApplicationStatus : uint8
{
	/**
	 * Xbox only.
	 *
	 * Notifies the SDK that the application has entered constrained mode.
	 * While in constrained mode, the application has reduced access to reserved system resources.
	 */
	EOS_AS_BackgroundConstrained = 0,
	/**
	 * Xbox only.
	 *
	 * Notifies the SDK that the application has returned from constrained mode,
	 * and is back to running in a regular state with full access to system resources.
	 * 
	 * The SDK will handle this state change and automatically transition its active state to EOS_AS_Foreground.
	 * As result, after the application has set the EOS_AS_BackgroundUnconstrained state,
	 * calling EOS_Platform_GetApplicationStatus will return EOS_AS_Foreground as the persisted active state.
	 */
	EOS_AS_BackgroundUnconstrained = 1,
	/**
	 * Notifies the SDK that the application has been put into suspended state by the platform system.
	 */
	EOS_AS_BackgroundSuspended = 2,
	/**
	 * Notifies the SDK that the application has been resumed from suspended state.
	 *
	 * This is the default active state on all platforms.
	 */
	EOS_AS_Foreground = 3
};

/**
 * All possible states of the network
 */
UENUM(BlueprintType)
enum class EOSENetworkStatus : uint8
{
	/**
	 * Networking unavailable.
	 */
	EOS_NS_Disabled = 0,
	/**
	 * Not connected to the internet. Only local area networking (LAN) may be available.
	 */
	EOS_NS_Offline = 1,
	/**
	 * Connected to the internet.
	 */
	EOS_NS_Online = 2
};

/**
 * Possible statuses for the availability of desktop crossplay functionality.
 *
 * @see EOS_Platform_GetDesktopCrossplayStatus
 */
UENUM(BlueprintType)
enum class EEOSEDesktopCrossplayStatus : uint8
{
	/**
	 * Desktop crossplay is ready to use.
	 */
	EOS_DCS_OK = 0,
	/**
	 * The application was not launched through the Bootstrapper.
	 */
	EOS_DCS_ApplicationNotBootstrapped = 1,
	/**
	 * The redistributable service is not installed.
	 */
	EOS_DCS_ServiceNotInstalled = 2,
	/**
	 * The service failed to start.
	 */
	EOS_DCS_ServiceStartFailed = 3,
	/**
	 * The service was started successfully, but is no longer running in the background, for an unknown reason.
	 */
	EOS_DCS_ServiceNotRunning = 4,
	/**
	 * The application has explicitly disabled the overlay through SDK initialization flags.
	 */
	EOS_DCS_OverlayDisabled = 5,
	/**
	 * The overlay is not installed.
	 *
	 * As the overlay is automatically installed and kept up-to-date by the redistributable service,
	 * this indicates that the user may have separately manually removed the installed overlay files.
	 */
	EOS_DCS_OverlayNotInstalled = 6,
	/**
	 * The overlay was not loaded due to failing trust check on the digital signature of the file on disk.
	 *
	 * This error typically indicates one of the following root causes:
	 * - The Operating System's local certificate store is out of date.
	 * - The local system clock has skewed and is in the wrong time.
	 * - The file has been tampered with.
	 * - The file trust check timed out, either due to an issue with the local system or network connectivity.
	 *
	 * The first troubleshooting steps should be to check for any available Operating System updates,
	 * for example using the Windows Update, as well as verifying that the system time is correctly set.
	 */
	EOS_DCS_OverlayTrustCheckFailed = 7,
	/**
	 * The overlay failed to load.
	 */
	EOS_DCS_OverlayLoadFailed = 8
};

UENUM(BlueprintType)
enum class ESuccessFail : uint8
{
	Success = 0,
	Fail
};

USTRUCT(BlueprintType)
struct FEOSEpicAccountId
{
	GENERATED_BODY()
public:
	FEOSEpicAccountId()
	{
		m_AccountId[0] = '\0';
	}

	FEOSEpicAccountId(const EOS_EpicAccountId& Data)
	{
		int32_t Length = EOS_EPICACCOUNTID_MAX_LENGTH + 1;
		EOS_EpicAccountId_ToString(Data, m_AccountId, &Length);
	}

public:
	FString ToString() const
	{
		return FString(UTF8_TO_TCHAR(m_AccountId));
	}

public:
	operator EOS_EpicAccountId() { return EOS_EpicAccountId_FromString(m_AccountId); }
	operator EOS_EpicAccountId() const { return EOS_EpicAccountId_FromString(m_AccountId); }
private:
	char m_AccountId[EOS_EPICACCOUNTID_MAX_LENGTH + 1];
};

USTRUCT(BlueprintType)
struct FEOSProductUserId
{
	GENERATED_BODY()
public:
	FEOSProductUserId()
	{
		m_AccountId[0] = '\0';
	}

	FEOSProductUserId(const EOS_ProductUserId& Data)
	{
		int32_t Size = EOS_EPICACCOUNTID_MAX_LENGTH + 1;
		EOS_ProductUserId_ToString(Data, m_AccountId, &Size);
	}

public:
	FString ToString() const
	{
		return FString(UTF8_TO_TCHAR(m_AccountId));
	}

public:
	operator EOS_ProductUserId() { return EOS_ProductUserId_FromString(m_AccountId); }
	operator EOS_ProductUserId() const { return EOS_ProductUserId_FromString(m_AccountId); }
private:
	char m_AccountId[EOS_EPICACCOUNTID_MAX_LENGTH + 1];
};

USTRUCT(BlueprintType)
struct FEOSTitleStorageFileTransferRequestHandle
{
	GENERATED_BODY()
public:
	EOS_HTitleStorageFileTransferRequest m_TitleStorageFileTransferRequest;
public:
	FEOSTitleStorageFileTransferRequestHandle()
		: m_TitleStorageFileTransferRequest(nullptr)
	{
	}

	FEOSTitleStorageFileTransferRequestHandle(EOS_HTitleStorageFileTransferRequest Data)
		: m_TitleStorageFileTransferRequest(Data)
	{
	}

public:
	operator EOS_HTitleStorageFileTransferRequest() { return m_TitleStorageFileTransferRequest; }
	operator EOS_HTitleStorageFileTransferRequest() const { return m_TitleStorageFileTransferRequest; }
};

USTRUCT(BlueprintType)
struct FContinuanceToken
{
	GENERATED_BODY()
public:
	EOS_ContinuanceTokenDetails* m_ContinuanceTokenDetails;
public:
	FContinuanceToken()
		: m_ContinuanceTokenDetails(nullptr)
	{
	}

	FContinuanceToken(const EOS_ContinuanceToken& Data)
		: m_ContinuanceTokenDetails(Data)
	{
	}

public:
	operator EOS_ContinuanceToken() { return m_ContinuanceTokenDetails; }
	operator EOS_ContinuanceToken() const { return m_ContinuanceTokenDetails; }
};

USTRUCT(BlueprintType)
struct FEOSHRTCAudio
{
	GENERATED_BODY()
public:
	FEOSHRTCAudio()
		: m_RTCAudioHandle(nullptr)
	{
	}

	FEOSHRTCAudio(const EOS_HRTCAudio& Data)
		: m_RTCAudioHandle(Data)
	{
	}

public:
	operator EOS_HRTCAudio() { return m_RTCAudioHandle; }
	operator EOS_HRTCAudio() const { return m_RTCAudioHandle; }
public:
	EOS_RTCAudioHandle* m_RTCAudioHandle;
};

USTRUCT(BlueprintType)
struct FEOSNotificationId
{
	GENERATED_BODY()
public:
	FEOSNotificationId()
		: Id(0)
	{
	}

	FEOSNotificationId(const EOS_NotificationId& Data)
		: Id(Data)
	{
	}

	FEOSNotificationId(const EOS_NotificationId* Data)
		: Id(*Data)
	{
	}

public:
	bool operator==(const FEOSNotificationId& Other)
	{
		return Id == Other.Id;
	}

	operator uint64_t() { return Id; }
	operator uint64_t() const { return Id; }
public:
	uint64_t Id;
};

FORCEINLINE uint32 GetTypeHash(const FEOSNotificationId& b)
{
	return FCrc::MemCrc_DEPRECATED(&b, sizeof(FEOSNotificationId));
}

/** ID representing a specific UI event. */
USTRUCT(BlueprintType, meta = (HasNativeBreak = "/Script/EOSCore.EOSCoreLibrary:BreakUIEventIdStruct"))
struct FEOSUIEventId
{
	GENERATED_BODY()
public:
	FEOSUIEventId()
		: EventId(EOS_UI_EVENTID_INVALID)
	{
	}

	FEOSUIEventId(const EOS_UI_EventId& Data)
		: EventId(Data)
	{
	}

public:
	operator uint64_t() { return EventId; }
	operator uint64_t() const { return EventId; }
public:
	uint64_t EventId;
};

USTRUCT(BlueprintType)
struct FEOSPageQuery
{
	GENERATED_BODY()
public:
	FEOSPageQuery()
		: StartIndex(0)
		  , MaxCount(0)
	{
	}

public:
	/** The index into the ordered query results to start the page at. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOSCore")
	int32 StartIndex;
	/** The maximum number of results to have in the page. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOSCore")
	int32 MaxCount;
};

/**
 * Input parameters for the EOS_Platform_GetDesktopCrossplayStatus function.
 */
USTRUCT(BlueprintType)
struct FEOSPlatformGetDesktopCrossplayStatusOptions
{
	GENERATED_BODY()
public:
	/** Account ID of the user whose friend list is being shown. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	FEOSEpicAccountId LocalUserId;
public:
	FEOSPlatformGetDesktopCrossplayStatusOptions()
	{
	}
};

/**
 * Input parameters for the EOS_Platform_GetDesktopCrossplayStatus function.
 */
USTRUCT(BlueprintType)
struct FEOSPlatformGetDesktopCrossplayStatusInfo
{
	GENERATED_BODY()
public:
	/**
	 * Status for the availability of desktop crossplay functionality.
	 *
	 * It is recommended to include this value in application logs, and as as part of
	 * any player-facing error screens to help troubleshooting possible issues.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	EEOSEDesktopCrossplayStatus Status;
	/**
	 * This field is set when the Status is EOS_DCS_ServiceStartFailed.
	 *
	 * Possible values for this field are not documented. However, it is recommended
	 * to be also included in application logs, and as part of any player-facing
	 * error screens.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	int32 ServiceInitResult;
public:
	FEOSPlatformGetDesktopCrossplayStatusInfo()
		: Status(EEOSEDesktopCrossplayStatus::EOS_DCS_OverlayDisabled)
		, ServiceInitResult(0)
	{
	}

	FEOSPlatformGetDesktopCrossplayStatusInfo(const EOS_Platform_GetDesktopCrossplayStatusInfo& Data)
		: Status(static_cast<EEOSEDesktopCrossplayStatus>(Data.Status))
		, ServiceInitResult(Data.ServiceInitResult)
	{
	}
};