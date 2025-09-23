/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "eos_auth_types.h"
#include "eos_connect_types.h"
#include "Core/EOSTypes.h"
#include "Sessions/EOSSessionsTypes.h"
#include "Lobby/EOSLobbyTypes.h"
#include "CreateEOSSessionCallbackProxy.h"
#include "EOSCoreAntiCheatCommon.h"
#include "FindEOSSessionsCallbackProxy.h"
#include "Auth/EOSAuthTypes.h"
#include "Connect/EOSConnectTypes.h"
#include "Ecom/EOSEcomTypes.h"

#include "EOSCoreLibrary.generated.h"

UENUM(BlueprintType)
enum class EEOSLogLevel : uint8
{
	Off = 0,
	Fatal,
	Error,
	Warning,
	Info,
	Verbose,
	VeryVerbose
};

DECLARE_DYNAMIC_DELEGATE_TwoParams(FEOSLoginCallback, bool, bWasSuccessful, const FString&, ErrorStr);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnEOSLogging, const FString&, Category, const FString&, Message, EEOSLogLevel, EOSLogLevel);

UCLASS()
class EOSCORE_API UEOSCoreLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "EOSCore", meta = (WorldContext = "WorldContextObject"))
	static bool EOS_Initialized(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "EOSCore")
	static bool UpdateUniqueNetIdFromOSS(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, Category = "EOSCore", meta = (WorldContext = "WorldContextObject"))
	static void ListenForEOSMessages(UObject* WorldContextObject, const FOnEOSLogging& Callback);

	UFUNCTION(BlueprintCallable, Category = "EOSCore", meta = (WorldContext = "WorldContextObject"))
	static void RemoveListenForEOSMessages(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "EOSCore", meta = (WorldContext = "WorldContextObject"))
	static void TestRefreshConnectLogin(UObject* WorldContextObject);
	/*
	* Attempt to login to the EOSCore Subsystem.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore", meta = (WorldContext = "WorldContextObject"))
	static void Login(UObject* WorldContextObject, APlayerController* PlayerController, FString LoginId, FString Password, EEOSLoginCredentialType AuthType, EEOSEExternalCredentialType CredentialsType, FString AdditionalData, const FEOSLoginCallback& Callback);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Utilities")
	static FDateTime FromUnixTimestamp(FString Timestamp);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Utilities")
	static void BreakUIEventIdStruct(FEOSUIEventId eventId, FString& outEventId);

	UFUNCTION(BlueprintCallable, Category = "EOSCore|Utilities", meta = (ExpandEnumAsExecs = "result"))
	static void EOS_Success(EOSResult status, ESuccessFail& result);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Utilities", meta = (CompactNodeTitle = "==", Keywords = "equal == identical"))
	static bool IsProductUserIdIdenticalWith(FEOSProductUserId a, FEOSProductUserId b);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Utilities", meta = (CompactNodeTitle = "==", Keywords = "equal == identical"))
	static bool IsEpicAccountIdIdenticalWith(FEOSEpicAccountId a, FEOSEpicAccountId b);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Sessions|Utilities")
	static bool GetSessionAttributeBool(const FEOSSessionsAttributeData& Data);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Sessions|Utilities")
	static FString GetSessionAttributeInt64(const FEOSSessionsAttributeData& Data);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Sessions|Utilities")
	static FString GetSessionAttributeDouble(const FEOSSessionsAttributeData& Data);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Sessions|Utilities")
	static FString GetSessionAttributeString(const FEOSSessionsAttributeData& Data);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Lobby|Utilities")
	static bool GetLobbyAttributeBool(const FEOSLobbyAttributeData& Data);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Lobby|Utilities")
	static FString GetLobbyAttributeInt64(const FEOSLobbyAttributeData& Data);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Lobby|Utilities")
	static FString GetLobbyAttributeDouble(const FEOSLobbyAttributeData& Data);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Lobby|Utilities")
	static FString GetLobbyAttributeString(const FEOSLobbyAttributeData& Data);

	/**
	* Returns a string representation of an EOS_EResult.
	* The return value is never null.
	* The return value must not be freed.
	*
	* Example: EOS_EResult_ToString(EOS_Success) returns "EOS_Success"
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Utilities")
	static FString EOSEResultToString(EOSResult Result);

	/**
	 * Gets the string representation of an EOS_EApplicationStatus value.
	 *
	 * Example: EOS_EApplicationStatus_ToString(EOS_EApplicationStatus::EOS_AS_Foreground) returns "EOS_AS_Foreground".
	 *
	 * @param ApplicationStatus EOS_EApplicationStatus value to get as string.
	 *
	 * @return Pointer to a static string representing the input enum value.
	 *         The returned string is guaranteed to be non-null, and must not be freed by the application.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Utilities", meta = (DisplayName = "EOS_EApplicationStatus_ToString"))
	static FString EOSEApplicationStatusToString(EOSEApplicationStatus ApplicationStatus);

	/**
	 * Gets the string representation of an EOS_ENetworkStatus value.
	 *
	 * Example: EOS_ENetworkStatus_ToString(EOS_ENetworkStatus::EOS_NS_Online) returns "EOS_NS_Online".
	 *
	 * @param NetworkStatus EOS_ENetworkStatus value to get as string.
	 *
	 * @return Pointer to a static string representing the input enum value.
	 *         The returned string is guaranteed to be non-null, and must not be freed by the application.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Utilities", meta = (DisplayName = "EOS_ENetworkStatus_ToString"))
	static FString EOSENetworkStatusToString(EOSENetworkStatus NetworkStatus);

	/**
	 * Returns whether a result is to be considered the final result, or false if the callback that returned this result
	 * will be called again either after some time or from another action.
	 *
	 * @param Result The result to check against being a final result for an operation
	 * @return True if this result means the operation is complete, false otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Utilities")
	static bool EOSEResultIsOperationComplete(EOSResult result);

	/**
	 * Encode a byte array into hex encoded string
	 *
	 * @return An EOS_EResult that indicates whether the byte array was converted and copied into the OutBuffer.
	 *         EOS_Success if the encoding was successful and passed out in OutBuffer
	 *         EOS_InvalidParameters if you pass a null pointer on invalid length for any of the parameters
	 *         EOS_LimitExceeded - The OutBuffer is not large enough to receive the encoding. InOutBufferLength contains the required minimum length to perform the operation successfully.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Utilities")
	static EOSResult EOSByteArrayToString(const TArray<uint8> array, int32 length, FString& string);

	/**
	 * Check whether or not the given account unique id is considered valid
	 *
	 * @param AccountId The account id to check for validity
	 * @return EOS_TRUE if the EOS_EpicAccountId is valid, otherwise EOS_FALSE
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Utilities")
	static bool EOSEpicAccountIdIsValid(FEOSEpicAccountId id);

	/**
	 * Check whether or not the given account unique id is considered valid
	 *
	 * @param AccountId The account id to check for validity
	 * @return EOS_TRUE if the EOS_EpicAccountId is valid, otherwise EOS_FALSE
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Utilities", meta = (DisplayName = "EOSEpic Account Id is Valid (Pure)"))
	static bool EOSEpicAccountIdIsValidPure(FEOSEpicAccountId id) { return EOSEpicAccountIdIsValid(id); }

	/**
	 * Retrieve a string-ified account ID from an EOS_EpicAccountId. This is useful for replication of Epic account IDs in multiplayer games.
	 *
	 * @param AccountId The account ID for which to retrieve the string-ified version.
	 * @param OutBuffer The buffer into which the character data should be written
	 * @param InOutBufferLength The size of the OutBuffer in characters.
	 *                          The input buffer should include enough space to be null-terminated.
	 *                          When the function returns, this parameter will be filled with the length of the string copied into OutBuffer.
	 *
	 * @return An EOS_EResult that indicates whether the account ID string was copied into the OutBuffer.
	 *         EOS_Success - The OutBuffer was filled, and InOutBufferLength contains the number of characters copied into OutBuffer excluding the null terminator.
	 *         EOS_InvalidParameters - Either OutBuffer or InOutBufferLength were passed as NULL parameters.
	 *         EOS_InvalidUser - The AccountId is invalid and cannot be string-ified
	 *         EOS_LimitExceeded - The OutBuffer is not large enough to receive the account ID string. InOutBufferLength contains the required minimum length to perform the operation successfully.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Utilities")
	static EOSResult EOSEpicAccountIdToString(FEOSEpicAccountId id, FString& string);

	/**
	 * Retrieve an EOS_EpicAccountId from a raw account ID string. The input string must be null-terminated.
	 *
	 * @param AccountIdString The string-ified account ID for which to retrieve the EOS_EpicAccountId
	 * @return The EOS_EpicAccountId that corresponds to the AccountIdString
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Utilities")
	static FEOSEpicAccountId EOSEpicAccountIdFromString(FString String);

	/**
	 * Check whether or not the given account unique id is considered valid
	 *
	 * @param AccountId The account id to check for validity
	 * @return EOS_TRUE if the EOS_ProductUserId is valid, otherwise EOS_FALSE
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Utilities")
	static bool EOSProductUserIdIsValid(FEOSProductUserId id);

	/**
	 * Check whether or not the given account unique id is considered valid
	 *
	 * @param AccountId The account id to check for validity
	 * @return EOS_TRUE if the EOS_ProductUserId is valid, otherwise EOS_FALSE
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Utilities", meta = (DisplayName = "EOSProduct User Id is Valid (Pure)"))
	static bool EOSProductUserIdIsValidPure(FEOSProductUserId id) { return EOSProductUserIdIsValid(id); }

	/**
	 * Retrieve a string-ified account ID from an EOS_ProductUserId. This is useful for replication of Product User IDs in multiplayer games.
	 *
	 * @param AccountId The account ID for which to retrieve the string-ified version.
	 * @param OutBuffer The buffer into which the character data should be written
	 * @param InOutBufferLength The size of the OutBuffer in characters.
	 *                          The input buffer should include enough space to be null-terminated.
	 *                          When the function returns, this parameter will be filled with the length of the string copied into OutBuffer.
	 *
	 * @return An EOS_EResult that indicates whether the account ID string was copied into the OutBuffer.
	 *         EOS_Success - The OutBuffer was filled, and InOutBufferLength contains the number of characters copied into OutBuffer excluding the null terminator.
	 *         EOS_InvalidParameters - Either OutBuffer or InOutBufferLength were passed as NULL parameters.
	 *         EOS_InvalidUser - The AccountId is invalid and cannot be string-ified
	 *         EOS_LimitExceeded - The OutBuffer is not large enough to receive the account ID string. InOutBufferLength contains the required minimum length to perform the operation successfully.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Utilities")
	static EOSResult EOSProductUserIdToString(FEOSProductUserId id, FString& string);

	/**
	 * Retrieve an EOS_EpicAccountId from a raw account ID string. The input string must be null-terminated.
	 *
	 * @param AccountIdString The string-ified account ID for which to retrieve the EOS_ProductUserId
	 * @return The EOS_ProductUserId that corresponds to the AccountIdString
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Utilities")
	static FEOSProductUserId EOSProductUserIdFromString(FString String);

	/**
	 * Convert a string to a byte array, useful when sending chat messages
	 *
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Utilities")
	static TArray<uint8> CoreStringToByte(FString string);

	/**
	 * Convert a byte array to a string, useful when receiving chat messages for example
	 *
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Utilities")
	static FString CoreBytesToString(const TArray<uint8>& data);

	/*
	* Get the current product id of the last successful login
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Utilities", meta = (WorldContext = "WorldContextObject"))
	static FEOSProductUserId GetCurrentProductId(UObject* WorldContextObject, int32 userIndex = 0);

	/*
	* Get the current account id of the last successful login
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Utilities", meta = (WorldContext = "WorldContextObject"))
	static FEOSEpicAccountId GetCurrentAccountId(UObject* WorldContextObject, int32 userIndex = 0);

	/**
	 * Retrieve a null-terminated string-ified continuance token from an EOS_ContinuanceToken.
	 *
	 * To get the required buffer size, call once with OutBuffer set to NULL, InOutBufferLength will contain the buffer size needed.
	 * Call again with valid params to get the string-ified continuance token which will only contain UTF8-encoded printable characters (excluding the null-terminator).
	 *
	 * @param ContinuanceToken The continuance token for which to retrieve the string-ified version.
	 * @param OutBuffer The buffer into which the character data should be written
	 * @param InOutBufferLength The size of the OutBuffer in characters.
	 *                          The input buffer should include enough space to be null-terminated.
	 *                          When the function returns, this parameter will be filled with the length of the string copied into OutBuffer including the null termination character.
	 *
	 * @return An EOS_EResult that indicates whether the Epic Online Services Account ID string was copied into the OutBuffer.
	 *         EOS_Success - The OutBuffer was filled, and InOutBufferLength contains the number of characters copied into OutBuffer including the null terminator.
	 *         EOS_InvalidParameters - Either OutBuffer or InOutBufferLength were passed as NULL parameters.
	 *         EOS_InvalidUser - The AccountId is invalid and cannot be string-ified
	 *         EOS_LimitExceeded - The OutBuffer is not large enough to receive the continuance token string. InOutBufferLength contains the required minimum length to perform the operation successfully.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Utilities")
	static EOSResult EOSContinuanceTokenToString(FContinuanceToken id, FString& string);

	// Try and find the authentication token from the CommandLine
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "EOSCore|Utilities")
	static FString FindExchangeCodePassword();	

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
	//		Session Settings
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Utilities", meta = (NativeMakeFunc))
	static FEOSSessionSetting MakeBool(bool value);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Utilities", meta = (NativeMakeFunc))
	static FEOSSessionSetting MakeString(FString value);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Utilities", meta = (NativeMakeFunc))
	static FEOSSessionSetting MakeInteger(int32 value);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Utilities", meta = (NativeMakeFunc))
	static FEOSSessionSearchSetting MakeSearchBool(bool value);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Utilities", meta = (NativeMakeFunc))
	static FEOSSessionSearchSetting MakeSearchString(FString value);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Utilities", meta = (NativeMakeFunc))
	static FEOSSessionSearchSetting MakeSearchInteger(int32 value);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Utilities")
	static bool GetBool(FEOSSessionSetting settings, FString& key)
	{
		bool Result = false;

		if (settings.Data.IsType<bool>())
		{
			key = settings.Key;
			Result = settings.Data.Get<bool>();
		}

		return Result;
	};

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Utilities")
	static int32 GetInteger(FEOSSessionSetting settings, FString& key)
	{
		int32 Result = -1;
		key.Empty();

		if (settings.Data.IsType<int32>())
		{
			key = settings.Key;
			Result = settings.Data.Get<int32>();
		}

		return Result;
	};

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Utilities")
	static FString GetString(FEOSSessionSetting settings, FString& key)
	{
		FString Result;
		key.Empty();

		if (settings.Data.IsType<FString>())
		{
			key = settings.Key;
			Result = settings.Data.Get<FString>();
		}

		return Result;
	};

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
	//		EventParamPairs
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Utilities")
	static FEOSAntiCheatCommonLogEventParamPair MakeParamInt32(int32 Value)
	{
		FEOSAntiCheatCommonLogEventParamPair Data;
		Data.ParamValueType = EEOSEAntiCheatCommonEventParamType::EOS_ACCEPT_Int32;
		Data.ParamValue.Int32 = Value;
		return Data;
	};
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Utilities")
	static FEOSAntiCheatCommonLogEventParamPair MakeParamInt64(int64 Value)
	{
		FEOSAntiCheatCommonLogEventParamPair Data;
		Data.ParamValueType = EEOSEAntiCheatCommonEventParamType::EOS_ACCEPT_Int64;
		Data.ParamValue.Int64 = Value;
		return Data;
	};
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Utilities")
	static FEOSAntiCheatCommonLogEventParamPair MakeParamString(FString String)
	{
		FEOSAntiCheatCommonLogEventParamPair Data;
		Data.ParamValueType = EEOSEAntiCheatCommonEventParamType::EOS_ACCEPT_String;
		Data.ParamValue.String = TCHAR_TO_UTF8(*String);
		return Data;
	};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		EOS SDK
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
	/**
	 * Windows only.
	 * Checks that the application is ready to use desktop crossplay functionality, with the necessary prerequisites having been met.
	 *
	 * This function verifies that the application was launched through the Bootstrapper application,
	 * the redistributable service has been installed and is running in the background,
	 * and that the overlay has been loaded successfully.
	 *
	 * On Windows, the desktop crossplay functionality is required to use Epic accounts login
	 * with applications that are distributed outside the Epic Games Store.
	 *
	 * @param Options input structure that specifies the API version.
	 * @param OutDesktopCrossplayStatusInfo output structure to receive the desktop crossplay status information.
	 *
	 * @return An EOS_EResult is returned to indicate success or an error.
	 *		   EOS_NotImplemented is returned on non-Windows platforms.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Platform_GetDesktopCrossplayStatus"))
	EOSResult EOSPlatformGetDesktopCrossplayStatus(UObject* WorldContextObject, FEOSPlatformGetDesktopCrossplayStatusOptions Options, FEOSPlatformGetDesktopCrossplayStatusInfo& OutDesktopCrossplayStatusInfo);

	/**
	 * Notify a change in application state.
	 *
	 * @note Calling SetApplicationStatus must happen before Tick when foregrounding for the cases where we won't get the background notification.
	 *
	 * @param NewStatus The new status for the application.
	 *
	 * @return An EOSResult that indicates whether we changed the application status successfully.
	 *         EOS_Success if the application was changed successfully.
	 *         EOS_InvalidParameters if the value of NewStatus is invalid.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Platform_SetApplicationStatus"))
	EOSResult EOSPlatformSetApplicationStatus(UObject* WorldContextObject, const EOSEApplicationStatus NewStatus);

	/**
	 * Retrieves the current application state as told to the SDK by the application.
	 *
	 * @return The current application status.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Platform_GetApplicationStatus"))
	EOSEApplicationStatus EOSPlatformGetApplicationStatus(UObject* WorldContextObject);

	/**
	 * Notify a change in network state.
	 *
	 * @param NewStatus The new network status.
	 *
	 * @return An EOS_EResult that indicates whether we changed the network status successfully.
	 *         EOS_Success if the network was changed successfully.
	 *         EOS_InvalidParameters if the value of NewStatus is invalid.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Platform_SetNetworkStatus"))
	EOSResult EOSPlatformSetNetworkStatus(UObject* WorldContextObject, EOSENetworkStatus NewStatus);

	/**
	 * Retrieves the current network state as told to the SDK by the application.
	 *
	 * @return The current network status.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Platform_GetNetworkStatus"))
	EOSENetworkStatus EOSPlatformGetNetworkStatus(UObject* WorldContextObject);
};
