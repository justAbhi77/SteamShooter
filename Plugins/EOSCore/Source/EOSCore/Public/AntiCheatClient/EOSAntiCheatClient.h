/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "EOSCoreModule.h"
#include "AntiCheatClient/EOSAntiCheatClientTypes.h"
#include "EOSAntiCheatClient.generated.h"

/*
* The Anti-Cheat interfaces currently only support the Windows platform. Mac and Linux are coming soon.
* Anti-Cheat only supports 64-bit Windows versions. 32-bit games are supported as long as the operating system itself has been installed with 64-bit support.
 */

UCLASS()
class EOSCORE_API UCoreAntiCheatClient : public UEOSCoreSubsystem
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
	static UCoreAntiCheatClient* GetAntiCheatClient(UObject* WorldContextObject);

public:
	/**
	 * Add a callback issued when a new message must be dispatched to the game server. The bound function will only be called
	 * between a successful call to EOSAntiCheatClientBeginSession and the matching EOSAntiCheatClientEndSession call in mode EOS_ACCM_ClientServer.
	 * Mode: EOS_ACCM_ClientServer.
	 *
	 * @param Options Structure containing input data
	 * @param Callback The callback to be fired
	 * @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatClient", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatClient_AddNotifyMessageToServer"))
	static FEOSNotificationId EOSAntiCheatClientAddNotifyMessageToServer(UObject* WorldContextObject, FEOSAntiCheatClientAddNotifyMessageToServerOptions Options, const FOnAntiCheatClientOnMessageToServerCallback& Callback);

	/**
	 * Remove a previously bound EOSAntiCheatClientAddNotifyMessageToServer handler.
	 * Mode: Any.
	 *
	 * @param NotificationId The previously bound notification ID
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatClient", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatClient_RemoveNotifyMessageToServer"))
	static void EOSAntiCheatClientRemoveNotifyMessageToServer(UObject* WorldContextObject, FEOSNotificationId NotificationId);

	/**
	 * Add a callback issued when a new message must be dispatched to a connected peer. The bound function will only be called
	 * between a successful call to EOSAntiCheatClientBeginSession and the matching EOSAntiCheatClientEndSession call in mode EOS_ACCM_PeerToPeer.
	 * Mode: EOS_ACCM_PeerToPeer.
	 *
	 * @param Options Structure containing input data
	 * @param Callback The callback to be fired
	 * @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatClient", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatClient_AddNotifyMessageToPeer"))
	static FEOSNotificationId EOSAntiCheatClientAddNotifyMessageToPeer(UObject* WorldContextObject, FEOSAntiCheatClientAddNotifyMessageToPeerOptions Options, const FOnAntiCheatClientOnMessageToPeerCallback& Callback);

	/**
	 * Remove a previously bound EOSAntiCheatClientAddNotifyMessageToPeer handler.
	 * Mode: Any.
	 *
	 * @param NotificationId The previously bound notification ID
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatClient", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatClient_RemoveNotifyMessageToPeer"))
	static void EOSAntiCheatClientRemoveNotifyMessageToPeer(UObject* WorldContextObject, FEOSNotificationId NotificationId);

	/**
	 * Add a callback issued when an action must be applied to a connected client. The bound function will only be called
	 * between a successful call to EOSAntiCheatClientBeginSession and the matching EOSAntiCheatClientEndSession call in mode EOS_ACCM_PeerToPeer.
	 * Mode: EOS_ACCM_PeerToPeer.
	 *
	 * @param Options Structure containing input data
	 * @param Callback The callback to be fired
	 * @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatClient", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatClient_AddNotifyPeerActionRequired"))
	static FEOSNotificationId EOSAntiCheatClientAddNotifyPeerActionRequired(UObject* WorldContextObject, FEOSAntiCheatClientAddNotifyPeerActionRequiredOptions Options, const FOnAntiCheatClientOnPeerActionRequiredCallback& Callback);

	/**
	 * Remove a previously bound EOSAntiCheatClientAddNotifyPeerActionRequired handler.
	 * Mode: Any.
	 *
	 * @param NotificationId The previously bound notification ID
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatClient", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatClient_RemoveNotifyPeerActionRequired"))
	static void EOSAntiCheatClientRemoveNotifyPeerActionRequired(UObject* WorldContextObject, FEOSNotificationId NotificationId);

	/**
	 * Add an optional callback issued when a connected peer's authentication status changes. The bound function will only be called
	 * between a successful call to EOSAntiCheatClientBeginSession and the matching EOSAntiCheatClientEndSession call in mode EOS_ACCM_PeerToPeer.
	 * Mode: EOS_ACCM_PeerToPeer.
	 *
	 * @param Options Structure containing input data
	 * @param Callback The callback to be fired
	 * @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatClient", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatClient_AddNotifyPeerAuthStatusChanged"))
	static FEOSNotificationId EOSAntiCheatClientAddNotifyPeerAuthStatusChanged(UObject* WorldContextObject, FEOSAntiCheatClientAddNotifyPeerAuthStatusChangedOptions Options, const FOnAntiCheatClientOnPeerAuthStatusChangedCallback& Callback);

	/**
	 * Remove a previously bound EOSAntiCheatClientAddNotifyPeerAuthStatusChanged handler.
	 * Mode: Any.
	 *
	 * @param NotificationId The previously bound notification ID
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatClient", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatClient_RemoveNotifyPeerAuthStatusChanged"))
	static void EOSAntiCheatClientRemoveNotifyPeerAuthStatusChanged(UObject* WorldContextObject, FEOSNotificationId NotificationId);
	
	/**
     * Add a callback when a message must be displayed to the local client informing them on a local integrity violation,
     * which will prevent further online play.
     * Mode: Any.
     *
     * @param Options Structure containing input data
     * @param Callback The callback to be fired
     * @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise
     */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatClient", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatClient_AddNotifyClientIntegrityViolated"))
    FEOSNotificationId EOSAntiCheatClientAddNotifyClientIntegrityViolated(UObject* WorldContextObject, FEOSAntiCheatClientAddNotifyClientIntegrityViolatedOptions Options, const FOnAntiCheatClientOnClientIntegrityViolatedCallback& Callback);
    
    /**
     * Remove a previously bound EOS_AntiCheatClient_AddNotifyClientIntegrityViolated handler.
     * Mode: Any.
     *
     * @param NotificationId The previously bound notification ID
     */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatClient", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatClient_RemoveNotifyClientIntegrityViolated"))
    void EOSAntiCheatClientRemoveNotifyClientIntegrityViolated(UObject* WorldContextObject, FEOSNotificationId NotificationId);

	/**
	 * Begins a multiplayer game session. After this call returns successfully, the client is ready to exchange
	 * anti-cheat messages with a game server or peer(s). When leaving one game session and connecting to a
	 * different one, a new anti-cheat session must be created by calling EOSAntiCheatClientEndSession and EOSAntiCheatClientBeginSession again.
	 * Mode: All
	 *
	 * @param Options Structure containing input data.
	 *
	 * @return EOS_Success - If the session was started successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 *         EOS_AntiCheat_InvalidMode - If the current mode does not support this function
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatClient", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatClient_BeginSession"))
	static EOSResult EOSAntiCheatClientBeginSession(UObject* WorldContextObject, FEOSAntiCheatClientBeginSessionOptions Options);

	/**
	 * Ends a multiplayer game session, either by leaving an ongoing session or shutting it down entirely.
	 * Mode: All
	 *
	 * Must be called when the multiplayer session ends, or when the local user leaves a session in progress.
	 *
	 * @param Options Structure containing input data.
	 *
	 * @return EOS_Success - If the session was ended normally
	 *         EOS_InvalidParameters - If input data was invalid
	 *         EOS_AntiCheat_InvalidMode - If the current mode does not support this function
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatClient", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatClient_EndSession"))
	static EOSResult EOSAntiCheatClientEndSession(UObject* WorldContextObject, FEOSAntiCheatClientEndSessionOptions Options);

	/**
	 * Polls for changes in client integrity status.
	 * Mode: All
	 *
	 * The purpose of this function is to allow the game to display information
	 * about anti-cheat integrity problems to the user. These are often the result of a
	 * corrupt game installation rather than cheating attempts. This function does not
	 * check for violations, it only provides information about violations which have
	 * automatically been discovered by the anti-cheat client. Such a violation may occur
	 * at any time and afterwards the user will be unable to join any protected multiplayer
	 * session until after restarting the game.
	 *
	 * @param Options Structure containing input data.
	 * @param ViolationType On success, receives a code describing the violation that occurred.
	 * @param OutMessage On success, receives a string describing the violation which should be displayed to the user.
	 *
	 * @return EOS_Success - If violation information was returned successfully
	 *		   EOS_LimitExceeded - If outMessage is too small to receive the message string. Call again with a larger outMessage.
	 *         EOS_NotFound - If no violation has occurred since the last call
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatClient", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatClient_PollStatus"))
	static EOSResult EOSAntiCheatClientPollStatus(UObject* WorldContextObject, FEOSAntiCheatClientPollStatusOptions Options, EEOSEAntiCheatClientViolationType& ViolationType, FString& OutMessage);

	/**
	 * Optional. Adds an integrity catalog and certificate pair from outside the game directory,
	 * for example to support mods that load from elsewhere.
	 * Mode: All
	 *
	 * @param Options Structure containing input data.
	 *
	 * @return EOS_Success - If the integrity catalog was added successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatClient", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatClient_AddExternalIntegrityCatalog"))
	static EOSResult EOSAntiCheatClientAddExternalIntegrityCatalog(UObject* WorldContextObject, FEOSAntiCheatClientAddExternalIntegrityCatalogOptions Options);

	/**
	 * Call when an anti-cheat message is received from the game server.
	 * Mode: EOS_ACCM_ClientServer.
	 *
	 * @param Options Structure containing input data.
	 *
	 * @return EOS_Success - If the message was processed successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 *         EOS_AntiCheat_InvalidMode - If the current mode does not support this function
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatClient", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatClient_ReceiveMessageFromServer"))
	static EOSResult EOSAntiCheatClientReceiveMessageFromServer(UObject* WorldContextObject, FEOSAntiCheatClientReceiveMessageFromServerOptions Options);

	/**
	 * Optional NetProtect feature for game message encryption.
	 * Calculates the required decrypted buffer size for a given input data length.
	 * This will not change for a given SDK version, and allows one time allocation of reusable buffers.
	 * Mode: EOS_ACCM_ClientServer.
	 *
	 * @param Options Structure containing input data.
	 * @param OutBufferLengthBytes The length in bytes that is required to call ProtectMessage on the given input size.
	 *
	 * @return EOS_Success - If the output length was calculated successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 *         EOS_AntiCheat_InvalidMode - If the current mode does not support this function
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatClient", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatClient_GetProtectMessageOutputLength"))
	static EOSResult EOSAntiCheatClientGetProtectMessageOutputLength(UObject* WorldContextObject, FEOSAntiCheatClientGetProtectMessageOutputLengthOptions Options, int32& OutBufferLengthBytes);

	/**
	 * Optional NetProtect feature for game message encryption.
	 * Encrypts an arbitrary message that will be sent to the game server and decrypted on the other side.
	 * Mode: EOS_ACCM_ClientServer.
	 *
	 * Options.Data and outBuffer may refer to the same buffer to encrypt in place.
	 *
	 * @param Options Structure containing input data.
	 * @param OutBuffer On success, buffer where encrypted message data will be written.
	 *
	 * @return EOS_Success - If the message was protected successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 *         EOS_AntiCheat_InvalidMode - If the current mode does not support this function
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatClient", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatClient_ProtectMessage"))
	static EOSResult EOSAntiCheatClientProtectMessage(UObject* WorldContextObject, FEOSAntiCheatClientProtectMessageOptions Options, TArray<uint8>& OutBuffer);

	/**
	 * Optional NetProtect feature for game message encryption.
	 * Decrypts an encrypted message received from the game server.
	 * Mode: EOS_ACCM_ClientServer.
	 *
	 * Options.Data and outBuffer may refer to the same buffer to decrypt in place.
	 *
	 * @param Options Structure containing input data.
	 * @param OutBuffer On success, buffer where encrypted message data will be written.
	 *
	 * @return EOS_Success - If the message was unprotected successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 *         EOS_AntiCheat_InvalidMode - If the current mode does not support this function
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatClient", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatClient_UnprotectMessage"))
	static EOSResult EOSAntiCheatClientUnprotectMessage(UObject* WorldContextObject, FEOSAntiCheatClientUnprotectMessageOptions Options, TArray<uint8> OutBuffer);

	/**
	 * Registers a connected peer-to-peer client.
	 * Mode: EOS_ACCM_PeerToPeer.
	 *
	 * Must be paired with a call to EOSAntiCheatClientUnregisterPeer if this user leaves the session
	 * in progress, or EOSAntiCheatClientEndSession if the entire session is ending.
	 *
	 * @param Options Structure containing input data.
	 *
	 * @return EOS_Success - If the player was registered successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 *         EOS_AntiCheat_InvalidMode - If the current mode does not support this function
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatClient", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatClient_RegisterPeer"))
	static EOSResult EOSAntiCheatClientRegisterPeer(UObject* WorldContextObject, FEOSAntiCheatClientRegisterPeerOptions Options);

	/**
	 * Unregisters a disconnected peer-to-peer client.
	 * Mode: EOS_ACCM_PeerToPeer.
	 *
	 * Must be called when a user leaves a session in progress.
	 *
	 * @param Options Structure containing input data.
	 *
	 * @return EOS_Success - If the player was unregistered successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 *         EOS_AntiCheat_InvalidMode - If the current mode does not support this function
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatClient", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatClient_UnregisterPeer"))
	static EOSResult EOSAntiCheatClientUnregisterPeer(UObject* WorldContextObject, FEOSAntiCheatClientUnregisterPeerOptions Options);

	/**
	 * Call when an anti-cheat message is received from a peer.
	 * Mode: EOS_ACCM_PeerToPeer.
	 *
	 * @param Options Structure containing input data.
	 *
	 * @return EOS_Success - If the message was processed successfully
	 *         EOS_InvalidParameters - If input data was invalid
	 *         EOS_AntiCheat_InvalidMode - If the current mode does not support this function
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|AntiCheatClient", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_AntiCheatClient_ReceiveMessageFromPeer"))
	static EOSResult EOSAntiCheatClientReceiveMessageFromPeer(UObject* WorldContextObject, FEOSAntiCheatClientReceiveMessageFromPeerOptions Options);
protected:
	TMap<FEOSNotificationId, FAntiCheatClientMessageToServer*> m_AntiCheatClientMessageToServer;
	TMap<FEOSNotificationId, FAntiCheatClientMessageToPeer*> m_AntiCheatClientMessageToPeer;
	TMap<FEOSNotificationId, FAntiCheatClientPeerActionRequired*> m_AntiCheatClientPeerActionRequired;
	TMap<FEOSNotificationId, FAntiCheatClientPeerAuthStatusChanged*> m_AntiCheatClientPeerAuthStatusChanged;
	TMap<FEOSNotificationId, FAntiCheatClientOnClientIntegrityViolated*> m_AntiCheatClientOnClientIntegrityViolated;
};
