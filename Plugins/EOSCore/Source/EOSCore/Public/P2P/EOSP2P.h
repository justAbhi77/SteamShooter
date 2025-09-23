/**
* Copyright (C) 2017-2022 | eelDev AB
*
 official EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "EOSCoreModule.h"
#include "eos_p2p_types.h"
#include "P2P/EOSP2PTypes.h"
#include "EOSP2P.generated.h"

/**
 * P2P functions to help manage sending and receiving of messages to peers
 *
 * These functions will attempt to perform NAT-punching, but will fallback to relays if a direct connection cannot be established
 */

UCLASS()
class EOSCORE_API UCoreP2P : public UEOSCoreSubsystem
{
	GENERATED_BODY()
public:
	virtual void Deinitialize() override;
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Interfaces", meta = (WorldContext = "WorldContextObject"))
	static UCoreP2P* GetP2P(UObject* WorldContextObject);
public:
	/**
	* Send a packet to a peer at the specified address. If there is already an open connection to this peer, it will be
	* sent immediately. If there is no open connection, an attempt to connect to the peer will be made. An EOS_Success
	* result only means the data was accepted to be sent, not that it has been successfully delivered to the peer.
	*
	* @param Options Information about the data being sent, by who, to who
	* @return EOS_EResult::EOS_Success           - If packet was queued to be sent successfully
	*         EOS_EResult::EOS_InvalidParameters - If input was invalid
	*         EOS_EResult::EOS_LimitExceeded     - If amount of data being sent is too large, or the outgoing packet queue was full
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|P2P", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_P2P_SendPacket"))
	static EOSResult EOSP2PSendPacket(UObject* WorldContextObject, FEOSP2PSendPacketOptions options);

	/**
	* Gets the size of the packet that will be returned by ReceivePacket for a particular user, if there is any available
	* packets to be retrieved.
	*
	* @param Options Information about who is requesting the size of their next packet
	* @param OutPacketSizeBytes The amount of bytes required to store the data of the next packet for the requested user
	* @return EOS_EResult::EOS_Success - If OutPacketSizeBytes was successfully set and there is data to be received
	*         EOS_EResult::EOS_InvalidParameters - If input was invalid
	*         EOS_EResult::EOS_NotFound  - If there are no packets available for the requesting user
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|P2P", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_P2P_GetNextReceivedPacketSize"))
	static EOSResult EOSP2PGetNextReceivedPacketSize(UObject* WorldContextObject, FEOSP2PGetNextReceivedPacketSizeOptions Options, int32& OutPacketSizeBytes);

	/**
	* Receive the next packet for the local user, and information associated with this packet, if it exists.
	*
	* @param Options Information about who is requesting the size of their next packet, and how much data can be stored safely
	* @param OutPeerId The Remote User who sent data. Only set if there was a packet to receive.
	* @param OutSocketId The Socket ID of the data that was sent. Only set if there was a packet to receive.
	* @param OutChannel The channel the data was sent on. Only set if there was a packet to receive.
	* @param OutData Buffer to store the data being received. Must be at least EOS_P2P_GetNextReceivedPacketSize in length or data will be truncated
	* @param OutBytesWritten The amount of bytes written to OutData. Only set if there was a packet to receive.
	* @return EOS_EResult::EOS_Success - If the packet was received successfully
	*         EOS_EResult::EOS_InvalidParameters - If input was invalid
	*         EOS_EResult::EOS_NotFound - If there are no packets available for the requesting user
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|P2P", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_P2P_ReceivePacket"))
	static EOSResult EOSP2PReceivePacket(UObject* WorldContextObject, FEOSP2PReceivePacketOptions Options, FEOSProductUserId& OutPeerId, FEOSP2PSocketId& OutSocketId, int32& OutChannel, TArray<uint8>& OutData, int32& OutBytesWritten);

	/**
	* Listen for incoming connection requests on a particular Socket ID, or optionally all Socket IDs. The bound function
	* will only be called if the connection has not already been accepted.
	*
	* @param Options Information about who would like notifications, and (optionally) only for a specific socket
	* @param Callback The callback to be fired when we receive a connection request
	* @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|P2P", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_P2P_AddNotifyPeerConnectionRequest"))
	FEOSNotificationId EOSP2PAddNotifyPeerConnectionRequest(UObject* WorldContextObject, FEOSP2PAddNotifyPeerConnectionRequestOptions Options, const FOnP2PIncomingConnectionRequestCallback& Callback);

	/**
	* Stop listening for connection requests on a previously bound handler.
	*
	* @param NotificationId The previously bound notification ID
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|P2P", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_P2P_RemoveNotifyPeerConnectionRequest"))
	static void EOSP2PRemoveNotifyPeerConnectionRequest(UObject* WorldContextObject, FEOSNotificationId NotificationId);

	/**
	* Listen for when a previously opened connection is closed.
	*
	* @param Options Information about who would like notifications about closed connections, and for which socket
	* @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|P2P", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_P2P_AddNotifyPeerConnectionClosed"))
	FEOSNotificationId EOSP2PAddNotifyPeerConnectionClosed(UObject* WorldContextObject, FEOSP2PAddNotifyPeerConnectionClosedOptions Options);

	/**
	 * Listen for when a connection is established. This is fired when we first connect to a peer, when we reconnect to a peer after a connection interruption,
	 * and when our underlying network connection type changes (for example, from a direct connection to relay, or vice versa). Network Connection Type changes
	 * will always be broadcast with a EOS_CET_Reconnection connection type, even if the connection was not interrupted.
	 *
	 * @param Options Information about who would like notifications about established connections, and for which socket
	 * @param Callback The callback to be fired when a connection has been established
	 * @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise
	 *
	 * @see EOS_P2P_AddNotifyPeerConnectionInterrupted
	 * @see EOS_P2P_AddNotifyPeerConnectionClosed
	 * @see EOS_P2P_RemoveNotifyPeerConnectionEstablished
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|P2P", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_P2P_AddNotifyPeerConnectionEstablished"))
	static FEOSNotificationId EOSP2PAddNotifyPeerConnectionEstablished(UObject* WorldContextObject, FEOSP2PAddNotifyPeerConnectionEstablishedOptions Options, const FOnP2OnPeerConnectionEstablishedCallback& Callback);


	/**
	 * Listen for when a previously opened connection is interrupted. The connection will automatically attempt to reestablish, but it may not be successful.
	 *
	 * If a connection reconnects, it will trigger the P2P PeerConnectionEstablished notification with the EOS_CET_Reconnection connection type.
	 * If a connection fails to reconnect, it will trigger the P2P PeerConnectionClosed notification.
	 *
	 * @param Options Information about who would like notifications about interrupted connections, and for which socket
	 * @param Callback The callback to be fired when an open connection has been interrupted
	 * @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise
	 *
	 * @see EOS_P2P_AddNotifyPeerConnectionEstablished
	 * @see EOS_P2P_AddNotifyPeerConnectionClosed
	 * @see EOS_P2P_RemoveNotifyPeerConnectionInterrupted
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|P2P", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_P2P_AddNotifyPeerConnectionInterrupted"))
	static FEOSNotificationId EOSP2PAddNotifyPeerConnectionInterrupted(UObject* WorldContextObject, FEOSP2PAddNotifyPeerConnectionInterruptedOptions Options, const FOnP2OnPeerConnectionInterruptedCallback& Callback);

	/**
	 * Stop notifications for connections being interrupted on a previously bound handler.
	 *
	 * @param NotificationId The previously bound notification ID
	 *
	 * @see EOS_P2P_AddNotifyPeerConnectionInterrupted
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|P2P", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_P2P_RemoveNotifyPeerConnectionInterrupted"))
	static void EOSP2PRemoveNotifyPeerConnectionInterrupted(UObject* WorldContextObject, FEOSNotificationId NotificationId);

	/**
	 * Stop notifications for connections being established on a previously bound handler.
	 *
	 * @param NotificationId The previously bound notification ID
	 *
	 * @see EOS_P2P_AddNotifyPeerConnectionEstablished
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|P2P", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_P2P_RemoveNotifyPeerConnectionEstablished"))
	static void EOSP2PRemoveNotifyPeerConnectionEstablished(UObject* WorldContextObject, FEOSNotificationId NotificationId);

	/**
	* Stop notifications for connections being closed on a previously bound handler.
	*
	* @param NotificationId The previously bound notification ID
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|P2P", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_P2P_RemoveNotifyPeerConnectionClosed"))
	static void EOSP2PRemoveNotifyPeerConnectionClosed(UObject* WorldContextObject, FEOSNotificationId NotificationId);

	/**
	* Accept connections from a specific peer. If this peer has not attempted to connect yet, when they do, they will automatically be accepted.
	*
	* @param Options Information about who would like to accept a connection, and which connection
	* @return EOS_EResult::EOS_Success - if the provided data is valid
	*         EOS_EResult::EOS_InvalidParameters - if the provided data is invalid
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|P2P", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_P2P_AcceptConnection"))
	static EOSResult EOSP2PAcceptConnection(UObject* WorldContextObject, FEOSP2PAcceptConnectionOptions Options);

	/**
	* Stop accepting new connections from a specific peer and close any open connections.
	*
	* @param Options Information about who would like to close a connection, and which connection.
	* @return EOS_EResult::EOS_Success - if the provided data is valid
	*         EOS_EResult::EOS_InvalidParameters - if the provided data is invalid
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|P2P", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_P2P_CloseConnection"))
	static EOSResult EOSP2PCloseConnection(UObject* WorldContextObject, FEOSP2PCloseConnectionOptions Options);

	/**
	* Close any open Connections for a specific Peer Connection ID.
	*
	* @param Options Information about who would like to close connections, and by what socket ID
	* @return EOS_EResult::EOS_Success - if the provided data is valid
	*         EOS_EResult::EOS_InvalidParameters - if the provided data is invalid
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|P2P", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_P2P_CloseConnections"))
	static EOSResult EOSP2PCloseConnections(UObject* WorldContextObject, FEOSP2PCloseConnectionsOptions Options);

	/**
	* Query the current NAT-type of our connection.
	*
	* @param Callback The callback to be fired when we finish querying our NAT type
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|P2P", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_P2P_QueryNATType"))
	void EOSP2PQueryNATType(UObject* WorldContextObject, const FOnP2PQueryNATTypeCompleteCallback& Callback);

	/**
	* Get our last-queried NAT-type, if it has been successfully queried.
	*
	* @param Options Information about what version of the EOS_P2P_GetNATType API is supported
	* @param OutNATType The queried NAT Type, or unknown if unknown
	* @return EOS_EResult::EOS_Success - if we have cached data
	*         EOS_EResult::EOS_NotFound - If we do not have queried data cached
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|P2P", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_P2P_GetNATType"))
	static EOSResult EOSP2PGetNATType(UObject* WorldContextObject, FEOSP2PGetNATTypeOptions Options, EEOSENATType& OutNatType);

	/**
	* Set how relay servers are to be used. This setting does not immediately apply to existing connections, but may apply to existing
	* connections if the connection requires renegotiation.
	*
	* @param Options Information about relay server config options
	* @return EOS_EResult::EOS_Success - if the options were set successfully
	*         EOS_EResult::EOS_InvalidParameters - if the options are invalid in some way
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|P2P", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_P2P_SetRelayControl"))
	static EOSResult EOSP2PSetRelayControl(UObject* WorldContextObject, FEOSP2PSetRelayControlOptions Options);

	/**
	* Get the current relay control setting.
	*
	* @param Options Information about what version of the EOS_P2P_GetRelayControl API is supported
	* @param OutRelayControl The relay control setting currently configured
	* @return EOS_EResult::EOS_Success - if the input was valid
	*         EOS_EResult::EOS_InvalidParameters - if the input was invalid in some way
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|P2P", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_P2P_GetRelayControl"))
	static EOSResult EOSP2PGetRelayControl(UObject* WorldContextObject, FEOSP2PGetRelayControlOptions Options, EEOSERelayControl& OutRelayControl);

	/**
	* Set configuration options related to network ports.
	*
	* @param Options Information about network ports config options
	* @return EOS_EResult::EOS_Success - if the options were set successfully
	*         EOS_EResult::EOS_InvalidParameters - if the options are invalid in some way
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|P2P", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_P2P_SetPortRange"))
	static EOSResult EOSP2PSetPortRange(UObject* WorldContextObject, FEOSP2PSetPortRangeOptions Options);

	/**
	* Get the current chosen port and the amount of other ports to try above the chosen port if the chosen port is unavailable.
	*
	* @param Options Information about what version of the EOS_P2P_GetPortRange API is supported
	* @param OutPort The port that will be tried first
	* @param OutNumAdditionalPortsToTry The amount of ports to try above the value in OutPort, if OutPort is unavailable
	* @return EOS_EResult::EOS_Success - if the input options were valid
	*         EOS_EResult::EOS_InvalidParameters - if the input was invalid in some way
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|P2P", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_P2P_GetPortRange"))
	static EOSResult EOSP2PGetPortRange(UObject* WorldContextObject, FEOSP2PGetPortRangeOptions Options, int32& OutPort, int32 OutNumAdditionalPortsToTry);

	/**
	* Sets the maximum packet queue sizes that packets waiting to be sent or received can use. If the packet queue
	* size is made smaller than the current queue size while there are packets in the queue that would push this
	* packet size over, existing packets are kept but new packets may not be added to the full queue until enough
	* packets are sent or received.
	*
	* @param Options Information about packet queue size
	* @return EOS_EResult::EOS_Success - if the input options were valid
	*         EOS_EResult::EOS_InvalidParameters - if the input was invalid in some way
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|P2P", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_P2P_SetPacketQueueSize"))
	static EOSResult EOSP2PSetPacketQueueSize(UObject* WorldContextObject, FEOSP2PSetPacketQueueSizeOptions Options);

	/**
	* Gets the current cached information related to the incoming and outgoing packet queues.
	*
	* @param Options Information about what version of the EOS_P2P_GetPacketQueueInfo API is supported
	* @param OutPacketQueueInfo The current information of the incoming and outgoing packet queues
	* @return EOS_EResult::EOS_Success - if the input options were valid
	*         EOS_EResult::EOS_InvalidParameters - if the input was invalid in some way
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|P2P", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_P2P_GetPacketQueueInfo"))
	static EOSResult EOSP2PGetPacketQueueInfo(UObject* WorldContextObject, FEOSP2PGetPacketQueueInfoOptions Options, FEOSP2PPacketQueueInfo& OutPacketQueueInfo);

	/**
	* Listen for when our packet queue has become full. This event gives an opportunity to read packets to make
	* room for new incoming packets. If this event fires and no packets are read by calling EOS_P2P_ReceivePacket
	* or the packet queue size is not increased by EOS_P2P_SetPacketQueueSize, any packets that are received after
	* this event are discarded until there is room again in the queue.
	*
	* @param Options Information about what version of the EOS_P2P_AddNotifyIncomingPacketQueueFull API is supported
	* @param Callback The callback to be fired when the incoming packet queue is full
	* @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|P2P", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_P2P_AddNotifyIncomingPacketQueueFull"))
	static FEOSNotificationId EOSP2PAddNotifyIncomingPacketQueueFull(UObject* WorldContextObject, FEOSP2PAddNotifyIncomingPacketQueueFullOptions Options, const FOnP2POnIncomingPacketQueueFullCallback& Callback);

	/**
	* Stop listening for full incoming packet queue events on a previously bound handler.
	*
	* @param NotificationId The previously bound notification ID
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|P2P", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_P2P_RemoveNotifyIncomingPacketQueueFull"))
	static void EOSP2PRemoveNotifyIncomingPacketQueueFull(UObject* WorldContextObject, FEOSNotificationId NotificationId);

private:
	static void Internal_OnQueryNatTypeCallback(const EOS_P2P_OnQueryNATTypeCompleteInfo* Data);
protected:
	TMap<FEOSNotificationId, FIncomingPacketQueueFullCallback*> m_IncomingPacketQueueFullCallbacks;
	TMap<FEOSNotificationId, FPeerConnectionInterruptedCallback*> m_PeerConnectionInterruptedCallbacks;
	TMap<FEOSNotificationId, FPeerConnectionEstablishedCallback*> m_PeerConnectionEstablishedCallbacks;
	
};
