/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "EOSCoreModule.h"
#include "AntiCheatServer/EOSAntiCheatServerTypes.h"
#include "EOSAntiCheatServer.generated.h"

/*
* The Anti-Cheat interfaces currently only support the Windows platform. Mac and Linux are coming soon.
* Anti-Cheat only supports 64-bit Windows versions. 32-bit games are supported as long as the operating system itself has been installed with 64-bit support.
 */

UCLASS()
class EOSCORE_API UCoreAntiCheatServer : public UEOSCoreSubsystem
{
	GENERATED_BODY()
public:
	virtual void Deinitialize() override;
public:
	/*
	* The Anti-Cheat interfaces currently only support the Windows platform. Mac and Linux are coming soon.
	* Anti-Cheat only supports 64-bit Windows versions. 32-bit games are supported as long as the operating system itself has been installed with 64-bit support.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Interfaces", meta = (WorldContext = "WorldContextObject"))
	static UCoreAntiCheatServer* GetAntiCheatServer(UObject* WorldContextObject);

public:
	/**
	 * Add a callback issued when a new message must be dispatched to a connected client. The bound function
	 * will only be called between a successful call to EOS_AntiCheatServer_BeginSession and the matching EOS_AntiCheatServer_EndSession call.
	 *
	 * @param Options Structure containing input data
	 * @param Callback The callback to be fired
	 * @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatServer", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatServer_AddNotifyMessageToClient"))
	static FEOSNotificationId EOSAntiCheatServerAddNotifyMessageToClient(UObject* WorldContextObject, FEOSAntiCheatServerAddNotifyMessageToClientOptions Options, const FOnAntiCheatServerOnMessageToClientCallback& Callback);

	/**
	 * Remove a previously bound EOS_AntiCheatServer_AddNotifyMessageToClient handler.
	 *
	 * @param NotificationId The previously bound notification ID
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatServer", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatServer_RemoveNotifyMessageToClient"))
	static void EOSAntiCheatServerRemoveNotifyMessageToClient(UObject* WorldContextObject, FEOSNotificationId NotificationId);

	/**
	 * Add a callback issued when an action must be applied to a connected client. The bound function
	 * will only be called between a successful call to EOS_AntiCheatServer_BeginSession and the matching EOS_AntiCheatServer_EndSession call.
	 *
	 * @param Options Structure containing input data
	 * @param Callback The callback to be fired
	 * @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatServer", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatServer_AddNotifyClientActionRequired"))
	static FEOSNotificationId EOSAntiCheatServerAddNotifyClientActionRequired(UObject* WorldContextObject, FEOSAntiCheatServerAddNotifyClientActionRequiredOptions Options, const FOnAntiCheatServerOnClientActionRequiredCallback& Callback);

	/**
	 * Remove a previously bound EOS_AntiCheatServer_AddNotifyClientActionRequired handler.
	 *
	 * @param NotificationId The previously bound notification ID
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatServer", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatServer_RemoveNotifyClientActionRequired"))
	static void EOSAntiCheatServerRemoveNotifyClientActionRequired(UObject* WorldContextObject, FEOSNotificationId NotificationId);

	/**
	 * Add an optional callback issued when a connected client's authentication status changes. The bound function
	 * will only be called between a successful call to EOS_AntiCheatServer_BeginSession and the matching EOS_AntiCheatServer_EndSession call.
	 *
	 * @param Options Structure containing input data
	 * @param Callback The callback to be fired
	 * @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatServer", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatServer_AddNotifyClientAuthStatusChanged"))
	static FEOSNotificationId EOSAntiCheatServerAddNotifyClientAuthStatusChanged(UObject* WorldContextObject, FEOSAntiCheatServerAddNotifyClientAuthStatusChangedOptions Options, const FOnAntiCheatServerOnClientAuthStatusChangedCallback& Callback);

	/**
	 * Remove a previously bound EOS_AntiCheatServer_AddNotifyClientAuthStatusChanged handler.
	 *
	 * @param NotificationId The previously bound notification ID
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatServer", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatServer_RemoveNotifyClientAuthStatusChanged"))
	static void EOSAntiCheatServerRemoveNotifyClientAuthStatusChanged(UObject* WorldContextObject, FEOSNotificationId NotificationId);

	/**
	 * Begin the gameplay session. Event callbacks must be configured with EOS_AntiCheatServer_AddNotifyMessageToClient
	 * and EOS_AntiCheatServer_AddNotifyClientActionRequired before calling this function.
	 *
	 * @param Options Structure containing input data.
	 *
	 * @return EOS_Success - If the initialization succeeded
	 *         EOS_InvalidParameters - If input data was invalid
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatServer", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatServer_BeginSession"))
	static EOSResult EOSAntiCheatServerBeginSession(UObject* WorldContextObject, FEOSAntiCheatServerBeginSessionOptions Options);

	/**
	 * End the gameplay session. Should be called when the server is shutting down or entering an idle state.
	 *
	 * @param Options Structure containing input data.
	 *
	 * @return EOS_Success - If the initialization succeeded
	 *         EOS_InvalidParameters - If input data was invalid
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatServer", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatServer_EndSession"))
	static EOSResult EOSAntiCheatServerEndSession(UObject* WorldContextObject, FEOSAntiCheatServerEndSessionOptions Options);

	/**
	 * Register a connected client. Must be paired with a call to UnregisterClient.
	 *
	 * This function may only be called between a successful call to EOS_AntiCheatServer_BeginSession and
	 * the matching EOS_AntiCheatServer_EndSession call.
	 *
	 * @param Options Structure containing input data.
	 *
	 * @return EOS_Success - If the player was registered successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatServer", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatServer_RegisterClient"))
	static EOSResult EOSAntiCheatServerRegisterClient(UObject* WorldContextObject, FEOSAntiCheatServerRegisterClientOptions Options);

	/**
	 * Unregister a disconnected client.
	 *
	 * This function may only be called between a successful call to EOS_AntiCheatServer_BeginSession and
	 * the matching EOS_AntiCheatServer_EndSession call.
	 *
	 * @param Options Structure containing input data.
	 *
	 * @return EOS_Success - If the player was unregistered successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatServer", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatServer_UnregisterClient"))
	static EOSResult EOSAntiCheatServerUnregisterClient(UObject* WorldContextObject, FEOSAntiCheatServerUnregisterClientOptions Options);

	/**
	 * Call when an anti-cheat message is received from a client.
	 *
	 * This function may only be called between a successful call to EOS_AntiCheatServer_BeginSession and
	 * the matching EOS_AntiCheatServer_EndSession call.
	 *
	 * @param Options Structure containing input data.
	 *
	 * @return EOS_Success - If the message was processed successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatServer", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatServer_ReceiveMessageFromClient"))
	static EOSResult EOSAntiCheatServerReceiveMessageFromClient(UObject* WorldContextObject, FEOSAntiCheatServerReceiveMessageFromClientOptions Options);

	/**
	 * Optional. Sets or updates client details including input device and admin status.
	 *
	 * This function may only be called between a successful call to EOS_AntiCheatServer_BeginSession and
	 * the matching EOS_AntiCheatServer_EndSession call.
	 *
	 * @param Options Structure containing input data.
	 *
	 * @return EOS_Success - If the flags were updated successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatServer", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatServer_SetClientDetails"))
	static EOSResult EOSAntiCheatServerSetClientDetails(UObject* WorldContextObject, FEOSAntiCheatCommonSetClientDetailsOptions Options);

	/**
	 * Optional. Sets or updates a game session identifier which can be attached to other data for reference.
	 * The identifier can be updated at any time for currently and subsequently registered clients.
	 *
	 * This function may only be called between a successful call to EOS_AntiCheatServer_BeginSession and
	 * the matching EOS_AntiCheatServer_EndSession call.
	 *
	 * @param Options Structure containing input data.
	 *
	 * @return EOS_Success - If the game session identifier was set successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatServer", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatServer_SetGameSessionId"))
	static EOSResult EOSAntiCheatServerSetGameSessionId(UObject* WorldContextObject, FEOSAntiCheatCommonSetGameSessionIdOptions Options);

	/**
	 * Optional. Can be used to indicate that a client is legitimately known to be
	 * temporarily unable to communicate, for example as a result of loading a new level.
	 *
	 * The bIsNetworkActive flag must be set back to true when users enter normal
	 * gameplay, otherwise anti-cheat enforcement will not work correctly.
	 *
	 * This function may only be called between a successful call to EOS_AntiCheatServer_BeginSession and
	 * the matching EOS_AntiCheatServer_EndSession call.
	 *
	 * @param Options Structure containing input data.
	 *
	 * @return EOS_Success - If the network state was updated successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatServer", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatServer_SetClientNetworkState"))
	static EOSResult EOSAntiCheatServerSetClientNetworkState(UObject* WorldContextObject, FEOSAntiCheatServerSetClientNetworkStateOptions Options);

	/**
	 * Optional NetProtect feature for game message encryption.
	 * Calculates the required decrypted buffer size for a given input data length.
	 * This will not change for a given SDK version, and allows one time allocation of reusable buffers.
	 *
	 * @param Options Structure containing input data.
	 * @param OutBufferLengthBytes The length in bytes that is required to call ProtectMessage on the given input size.
	 *
	 * @return EOS_Success - If the output length was calculated successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatServer", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatServer_GetProtectMessageOutputLength"))
	static EOSResult EOSAntiCheatServerGetProtectMessageOutputLength(UObject* WorldContextObject, FEOSAntiCheatServerGetProtectMessageOutputLengthOptions Options, int32& OutBufferLengthBytes);

	/**
	 * Optional NetProtect feature for game message encryption.
	 * Encrypts an arbitrary message that will be sent to a game client and decrypted on the other side.
	 *
	  options.Data and OutBuffer may refer to the same buffer to encrypt in place.
	 *
	 * @param Options Structure containing input data.
	 * @param OutBuffer On success, buffer where encrypted message data will be written.
	 * @param OutBufferLengthBytes Number of bytes that were written to OutBuffer.
	 *
	 * @return EOS_Success - If the message was protected successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 *         EOS_InvalidUser - If the specified ClientHandle was invalid or not currently registered. See RegisterClient.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatServer", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatServer_ProtectMessage"))
	static EOSResult EOSAntiCheatServerProtectMessage(UObject* WorldContextObject, FEOSAntiCheatServerProtectMessageOptions Options, TArray<uint8>& OutBuffer, int32& OutBufferLengthBytes);

	/**
	 * Optional NetProtect feature for game message encryption.
	 * Decrypts an encrypted message received from a game client.
	 *
	  options.Data and OutBuffer may refer to the same buffer to decrypt in place.
	 *
	 * @param Options Structure containing input data.
	 * @param OutBuffer On success, buffer where encrypted message data will be written.
	 * @param OutBufferLengthBytes Number of bytes that were written to OutBuffer.
	 *
	 * @return EOS_Success - If the message was unprotected successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatServer", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatServer_UnprotectMessage"))
	static EOSResult EOSAntiCheatServerUnprotectMessage(UObject* WorldContextObject, FEOSAntiCheatServerUnprotectMessageOptions Options, TArray<uint8>& OutBuffer, int32& OutBufferLengthBytes);

	/**
	 * Optional Cerberus feature for gameplay data collection.
	 * Registers a custom gameplay event.
	 *
	 * All custom game events must be registered before EOS_AntiCheatServer_BeginSession is called for the first time.
	 * After the first call to EOS_AntiCheatServer_BeginSession, this function cannot be called any longer.
	 *
	 * @param Options Structure containing input data.
	 *
	 * @return EOS_Success - If the event was registered successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatServer", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatServer_RegisterEvent"))
	static EOSResult EOSAntiCheatServerRegisterEvent(UObject* WorldContextObject, FEOSAntiCheatCommonRegisterEventOptions Options);

	/**
	 * Optional Cerberus feature for gameplay data collection.
	 * Logs a custom gameplay event.
	 *
	 * This function may only be called between a successful call to EOS_AntiCheatServer_BeginSession and
	 * the matching EOS_AntiCheatServer_EndSession call.
	 *
	 * @param Options Structure containing input data.
	 *
	 * @return EOS_Success - If the event was logged successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatServer", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatServer_LogEvent"))
	static EOSResult EOSAntiCheatServerLogEvent(UObject* WorldContextObject, FEOSAntiCheatCommonLogEventOptions Options);

	/**
	 * Optional Cerberus feature for gameplay data collection.
	 * Logs a new game round start.
	 *
	 * This function may only be called between a successful call to EOS_AntiCheatServer_BeginSession and
	 * the matching EOS_AntiCheatServer_EndSession call.
	 *
	 * @param Options Structure containing input data.
	 *
	 * @return EOS_Success - If the event was logged successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatServer", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatServer_LogGameRoundStart"))
	static EOSResult EOSAntiCheatServerLogGameRoundStart(UObject* WorldContextObject, FEOSAntiCheatCommonLogGameRoundStartOptions Options);

	/**
	 * Optional Cerberus feature for gameplay data collection.
	 * Logs a game round's end and outcome.
	 *
	 * This function may only be called between a successful call to EOS_AntiCheatServer_BeginSession and
	 * the matching EOS_AntiCheatServer_EndSession call.
	 *
	 * @param Options Structure containing input data.
	 *
	 * @return EOS_Success - If the event was logged successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatServer", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatServer_LogGameRoundEnd"))
	static EOSResult EOSAntiCheatServerLogGameRoundEnd(UObject* WorldContextObject, FEOSAntiCheatCommonLogGameRoundEndOptions Options);

	/**
	 * Optional Cerberus feature for gameplay data collection.
	 * Logs a player spawning into the game.
	 *
	 * This function may only be called between a successful call to EOS_AntiCheatServer_BeginSession and
	 * the matching EOS_AntiCheatServer_EndSession call.
	 *
	 * @param Options Structure containing input data.
	 *
	 * @return EOS_Success - If the event was logged successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatServer", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatServer_LogPlayerSpawn"))
	static EOSResult EOSAntiCheatServerLogPlayerSpawn(UObject* WorldContextObject, FEOSAntiCheatCommonLogPlayerSpawnOptions Options);

	/**
	 * Optional Cerberus feature for gameplay data collection.
	 * Logs a player despawning in the game, for example as a result of the character's death,
	 * switching to spectator mode, etc.
	 *
	 * This function may only be called between a successful call to EOS_AntiCheatServer_BeginSession and
	 * the matching EOS_AntiCheatServer_EndSession call.
	 *
	 * @param Options Structure containing input data.
	 *
	 * @return EOS_Success - If the event was logged successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatServer", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatServer_LogPlayerDespawn"))
	static EOSResult EOSAntiCheatServerLogPlayerDespawn(UObject* WorldContextObject, FEOSAntiCheatCommonLogPlayerDespawnOptions Options);

	/**
	 * Optional Cerberus feature for gameplay data collection.
	 * Logs a player being revived after being downed (see EOS_AntiCheatServer_LogPlayerTakeDamage options).
	 *
	 * This function may only be called between a successful call to EOS_AntiCheatServer_BeginSession and
	 * the matching EOS_AntiCheatServer_EndSession call.
	 *
	 * @param Options Structure containing input data.
	 *
	 * @return EOS_Success - If the event was logged successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatServer", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatServer_LogPlayerRevive"))
	static EOSResult EOSAntiCheatServerLogPlayerRevive(UObject* WorldContextObject, FEOSAntiCheatCommonLogPlayerReviveOptions Options);

	/**
	 * Optional Cerberus feature for gameplay data collection.
	 * Logs a player's general state including position and view direction.
	 *
	 * This function may only be called between a successful call to EOS_AntiCheatServer_BeginSession and
	 * the matching EOS_AntiCheatServer_EndSession call.
	 *
	 * @param Options Structure containing input data.
	 *
	 * @return EOS_Success - If the event was logged successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatServer", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatServer_LogPlayerTick"))
	static EOSResult EOSAntiCheatServerLogPlayerTick(UObject* WorldContextObject, FEOSAntiCheatCommonLogPlayerTickOptions Options);

	/**
	 * Optional Cerberus feature for gameplay data collection.
	 * Logs that a player has used a weapon, for example firing one bullet or making one melee attack.
	 *
	 * This function may only be called between a successful call to EOS_AntiCheatServer_BeginSession and
	 * the matching EOS_AntiCheatServer_EndSession call.
	 *
	 * @param Options Structure containing input data.
	 *
	 * @return EOS_Success - If the event was logged successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatServer", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatServer_LogPlayerUseWeapon"))
	static EOSResult EOSAntiCheatServerLogPlayerUseWeapon(UObject* WorldContextObject, FEOSAntiCheatCommonLogPlayerUseWeaponOptions Options);

	/**
	 * Optional Cerberus feature for gameplay data collection.
	 * Logs that a player has used a special ability or item which affects their character's capabilities,
	 * for example temporarily increasing their speed or allowing them to see nearby players behind walls.
	 *
	 * This function may only be called between a successful call to EOS_AntiCheatServer_BeginSession and
	 * the matching EOS_AntiCheatServer_EndSession call.
	 *
	 * @param Options Structure containing input data.
	 *
	 * @return EOS_Success - If the event was logged successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatServer", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatServer_LogPlayerUseAbility"))
	static EOSResult EOSAntiCheatServerLogPlayerUseAbility(UObject* WorldContextObject, FEOSAntiCheatCommonLogPlayerUseAbilityOptions Options);

	/**
	 * Optional Cerberus feature for gameplay data collection.
	 * Logs that a player has taken damage.
	 *
	 * This function may only be called between a successful call to EOS_AntiCheatServer_BeginSession and
	 * the matching EOS_AntiCheatServer_EndSession call.
	 *
	 * @param Options Structure containing input data.
	 *
	 * @return EOS_Success - If the event was logged successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatServer", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatServer_LogPlayerTakeDamage"))
	static EOSResult EOSAntiCheatServerLogPlayerTakeDamage(UObject* WorldContextObject, FEOSAntiCheatCommonLogPlayerTakeDamageOptions Options);

protected:
	TMap<FEOSNotificationId, FAntiCheatServerMessageToClient*> m_AntiCheatServerMessageToClient;
	TMap<FEOSNotificationId, FAntiCheatServerActionRequired*> m_AntiCheatServerActionRequired;
	TMap<FEOSNotificationId, FAntiCheatServerClientAuthStatusChanged*> m_AntiCheatServerClientAuthStatusChanged;
};
