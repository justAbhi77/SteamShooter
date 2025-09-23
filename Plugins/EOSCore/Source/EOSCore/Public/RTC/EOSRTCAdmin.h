/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "EOSCoreModule.h"
#include "EOSRTCAdminTypes.h"
#include "Core/EOSTypes.h"
#include "EOSRTCAdmin.generated.h"

UCLASS()
class EOSCORE_API UCoreRTCAdmin : public UEOSCoreSubsystem
{
	GENERATED_BODY()
public:
	/**
	* The RTC Admin interface.
	*
	* This is used to manage admin-specific RTC features, such as requesting join tokens, kick users, etc.
	*
	* @see EOS_Platform_GetRTCAdminInterface
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Interfaces", meta = (WorldContext = "WorldContextObject"))
	static UCoreRTCAdmin* GetRTCAdmin(UObject* WorldContextObject);

public:
	/**
	 * Query for a list of user tokens for joining a room.
	 *
	 * Each query generates a query id ( see EOS_RTCAdmin_QueryJoinRoomTokenCompleteCallbackInfo ) which should be used
	 * to retrieve the tokens from inside the callback.
	 *
	 *This query id and query result itself are only valid for the duration of the callback.
	 *
	 * @param Options Structure containing information about the application whose user tokens we're retrieving.
	 * @param Callback This function is called when the query join room token operation completes.
	 *
	 * @return EOS_Success if the operation completes successfully
	 *         EOS_InvalidParameters if any of the options are incorrect
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAdmin", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAdmin_QueryJoinRoomToken"))
	void EOSRTCAdminQueryJoinRoomToken(UObject* WorldContextObject, FEOSQueryJoinRoomTokenOptions Options, const FOnQueryJoinRoomTokenCompleteCallback& Callback);

	/**
	 * Fetches a user token when called inside of the OnQueryJoinRoomTokenComplete callback.
	 *
	 * @param Options Structure containing the index being accessed
	 * @param OutUserToken The user token for the given index, if it exists and is valid. Use EOSRTCAdminUserToken_Release when finished
	 *
	 * @note The order of the tokens doesn't necessarily match the order of the EOS_ProductUserId array specified in the EOSRTCAdminQueryJoinRoomTokenOptions when
	 * initiating the query.
	 *
	 * @see EOSRTCAdminUserToken_Release
	 *
	 * @return EOS_Success if the information is available and passed out in OutUserToken
	 *         EOS_InvalidParameters if you pass a null pointer for the out parameter
	 *         EOS_NotFound if the user token is not found
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAdmin", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAdmin_CopyUserTokenByIndex"))
	EOSResult EOSRTCAdminCopyUserTokenByIndex(UObject* WorldContextObject, FEOSCopyUserTokenByIndexOptions Options, FEOSUserToken& OutUserToken);

	/**
	 * Fetches a user token for a given user ID when called inside of the OnQueryJoinRoomTokenComplete callback.
	 *
	 * @param Options Structure containing the user ID being accessed
	 * @param OutUserToken The user token for the given user ID, if it exists and is valid. Use EOSRTCAdminUserToken_Release when finished
	 *
	 * @see EOSRTCAdminUserToken_Release
	 *
	 * @return EOS_Success if the information is available and passed out in OutUserToken
	 *         EOS_InvalidParameters if you pass a null pointer for the out parameter
	 *         EOS_NotFound if the user token is not found
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAdmin", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAdmin_CopyUserTokenByUserId"))
	EOSResult EOSRTCAdminCopyUserTokenByUserId(UObject* WorldContextObject, FEOSCopyUserTokenByUserIdOptions Options, FEOSUserToken& OutUserToken);

	/**
	 * Starts an asynchronous task that removes a participant from a room and revokes their token.
	 *
	 * @param Options structure containing the room and user to revoke the token from.
	 * @param Callback a callback that is fired when the async operation completes, either successfully or in error
	 *
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAdmin", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAdmin_Kick"))
	void EOSRTCAdminKick(UObject* WorldContextObject, FEOSKickOptions Options, const FOnKickCompleteCallback& Callback);

	/**
	 * Starts an asynchronous task remotely mutes/unmutes a room participant.
	 *
	 * This remotely mutes the specified participant, so no audio is sent from that participant to any other participant in the room.
	 *
	 * @param Options structure containing the room and user to mute.
	 * @param Callback a callback that is fired when the async operation completes, either successfully or in error
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|RTCAdmin", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_RTCAdmin_SetParticipantHardMute"))
	void EOSRTCAdminSetParticipantHardMute(UObject* WorldContextObject, FEOSSetParticipantHardMuteOptions Options, const FOnSetParticipantHardMuteCompleteCallback& Callback);
private:
	static void Internal_OnQueryJoinRoomTokenCallback(const EOS_RTCAdmin_QueryJoinRoomTokenCompleteCallbackInfo* Data);
	static void Internal_OnKickCallback(const EOS_RTCAdmin_KickCompleteCallbackInfo* Data);
	static void Internal_OnSetParticipantHardMuteCallback(const EOS_RTCAdmin_SetParticipantHardMuteCompleteCallbackInfo* Data);
};
