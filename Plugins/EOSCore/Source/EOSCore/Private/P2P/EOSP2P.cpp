/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "P2P/EOSP2P.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLibrary.h"
#include "Core/EOSCoreLogging.h"

void UCoreP2P::Deinitialize()
{
	LogVerbose("");

	for (auto Element: m_IncomingPacketQueueFullCallbacks)
	{
		EOS_P2P_RemoveNotifyIncomingPacketQueueFull(GetP2PHandle(this), Element.Key);
	}

	m_IncomingPacketQueueFullCallbacks.Empty();
}

UCoreP2P* UCoreP2P::GetP2P(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (GetP2PHandle(WorldContextObject))
		{
			if (UWorld* World = WorldContextObject->GetWorld())
			{
				if (UGameInstance* GameInstance = World->GetGameInstance())
				{
					return GameInstance->GetSubsystem<UCoreP2P>();
				}
			}	
		}
	}
	return nullptr;
}

EOSResult UCoreP2P::EOSP2PSendPacket(UObject* WorldContextObject, FEOSP2PSendPacketOptions options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetP2PHandle(WorldContextObject))
	{
		EOS_P2P_SocketId SocketId = {  };
		SocketId.ApiVersion = EOS_P2P_SOCKETID_API_LATEST;

		FCStringAnsi::Strncpy(SocketId.SocketName, TCHAR_TO_UTF8(*options.SocketId.SocketName), options.SocketId.SocketName.Len() + 1);

		EOS_P2P_SendPacketOptions P2POptions = { };
		P2POptions.ApiVersion = EOS_P2P_SENDPACKET_API_LATEST;
		P2POptions.LocalUserId = options.LocalUserId;
		P2POptions.RemoteUserId = options.RemoteUserId;
		P2POptions.SocketId = &SocketId;
		P2POptions.Channel = static_cast<uint8_t>(options.Channel);
		P2POptions.DataLengthBytes = static_cast<uint32_t>(options.Data.Num());
		P2POptions.Data = options.Data.GetData();
		P2POptions.bAllowDelayedDelivery = options.bAllowDelayedDelivery;
		P2POptions.Reliability = static_cast<EOS_EPacketReliability>(options.Reliability);
		P2POptions.bDisableAutoAcceptConnection = options.bDisableAutoAcceptConnection;

		Result = EOSHelpers::ToEOSCoreResult(EOS_P2P_SendPacket(GetP2PHandle(WorldContextObject), &P2POptions));
	}

	return Result;
}

EOSResult UCoreP2P::EOSP2PGetNextReceivedPacketSize(UObject* WorldContextObject, FEOSP2PGetNextReceivedPacketSizeOptions Options, int32& OutPacketSizeBytes)
{
	LogVerbose("");

	OutPacketSizeBytes = 0;

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetP2PHandle(WorldContextObject))
	{
		uint8_t Channel = Options.RequestedChannel;
		uint32_t Size = 0;
		EOS_P2P_GetNextReceivedPacketSizeOptions P2POptions = { };
		P2POptions.ApiVersion = Options.ApiVersion;
		P2POptions.LocalUserId = Options.LocalUserId;
		P2POptions.RequestedChannel = (Options.RequestedChannel == -1 ? nullptr : &Channel);

		Result = EOSHelpers::ToEOSCoreResult(EOS_P2P_GetNextReceivedPacketSize(GetP2PHandle(WorldContextObject), &P2POptions, &Size));

		if (Result == EOSResult::EOS_Success)
		{
			OutPacketSizeBytes = Size;
		}
	}

	return Result;
}

EOSResult UCoreP2P::EOSP2PReceivePacket(UObject* WorldContextObject, FEOSP2PReceivePacketOptions Options, FEOSProductUserId& OutPeerId, FEOSP2PSocketId& OutSocketId, int32& OutChannel, TArray<uint8>& OutData, int32& OutBytesWritten)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetP2PHandle(WorldContextObject))
	{
		uint8_t OptionsChannel = Options.RequestedChannel;

		EOS_P2P_ReceivePacketOptions P2POptions = { };
		P2POptions.ApiVersion = Options.ApiVersion;
		P2POptions.LocalUserId = Options.LocalUserId;
		P2POptions.MaxDataSizeBytes = static_cast<uint32_t>(Options.MaxDataSizeBytes);
		P2POptions.RequestedChannel = (OptionsChannel == -1 ? nullptr : &OptionsChannel);

		EOS_ProductUserId PeerId;
		EOS_P2P_SocketId SocketId;
		uint8_t Channel;
		TArray<uint8> Data;
		Data.SetNum(Options.MaxDataSizeBytes);
		uint32_t BytesWritten = 0;

		Result = EOSHelpers::ToEOSCoreResult(EOS_P2P_ReceivePacket(GetP2PHandle(WorldContextObject), &P2POptions, &PeerId, &SocketId, &Channel, Data.GetData(), &BytesWritten));

		if (Result == EOSResult::EOS_Success)
		{
			Data.SetNum(BytesWritten);

			OutPeerId = PeerId;
			OutSocketId = SocketId;
			OutChannel = Channel;
			OutData = Data;
			OutBytesWritten = BytesWritten;
		}
	}

	return Result;
}

static FOnP2PIncomingConnectionRequestCallback OnP2PIncomingConnectionRequestCallback;
FEOSNotificationId UCoreP2P::EOSP2PAddNotifyPeerConnectionRequest(UObject* WorldContextObject, FEOSP2PAddNotifyPeerConnectionRequestOptions Options, const FOnP2PIncomingConnectionRequestCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId NotifiationId;

	if (GetP2PHandle(WorldContextObject))
	{
		OnP2PIncomingConnectionRequestCallback = Callback;

		EOS_P2P_SocketId SocketId = { };
		SocketId.ApiVersion = EOS_P2P_SOCKETID_API_LATEST;

		FCStringAnsi::Strncpy(SocketId.SocketName, TCHAR_TO_UTF8(*Options.SocketId.SocketName), Options.SocketId.SocketName.Len() + 1);

		EOS_P2P_AddNotifyPeerConnectionRequestOptions P2POptions = { };
		P2POptions.ApiVersion = Options.ApiVersion;
		P2POptions.LocalUserId = Options.LocalUserId;
		P2POptions.SocketId = (Options.SocketId.SocketName.Len() > 0 ? &SocketId : nullptr);

		NotifiationId = EOS_P2P_AddNotifyPeerConnectionRequest(GetP2PHandle(WorldContextObject), &P2POptions, this, ([](const EOS_P2P_OnIncomingConnectionRequestInfo* data)
		{
			UCoreP2P* P2P = static_cast<UCoreP2P*>(data->ClientData);
			check(P2P);

			if (P2P)
			{
				OnP2PIncomingConnectionRequestCallback.ExecuteIfBound(*data, true);
			}

			LogVerbose("");
		}));
	}

	return NotifiationId;
}

void UCoreP2P::EOSP2PRemoveNotifyPeerConnectionRequest(UObject* WorldContextObject, FEOSNotificationId NotificationId)
{
	LogVerbose("");

	if (GetP2PHandle(WorldContextObject))
	{
		EOS_P2P_RemoveNotifyPeerConnectionRequest(GetP2PHandle(WorldContextObject), NotificationId);

		OnP2PIncomingConnectionRequestCallback.Clear();
		OnP2PIncomingConnectionRequestCallback.Unbind();
	}
}

FEOSNotificationId UCoreP2P::EOSP2PAddNotifyPeerConnectionClosed(UObject* WorldContextObject, FEOSP2PAddNotifyPeerConnectionClosedOptions Options)
{
	LogVerbose("");

	FEOSNotificationId NotifiationId;

	if (GetP2PHandle(WorldContextObject))
	{
		EOS_P2P_SocketId SocketId = { };
		SocketId.ApiVersion = EOS_P2P_SOCKETID_API_LATEST;
		FCStringAnsi::Strncpy(SocketId.SocketName, TCHAR_TO_UTF8(*Options.SocketId.SocketName), Options.SocketId.SocketName.Len() + 1);

		EOS_P2P_AddNotifyPeerConnectionClosedOptions P2POptions = { };
		P2POptions.ApiVersion = EOS_P2P_ADDNOTIFYPEERCONNECTIONCLOSED_API_LATEST;
		P2POptions.LocalUserId = Options.LocalUserId;
		P2POptions.SocketId = (Options.SocketId.SocketName.Len() > 0 ? &SocketId : nullptr);

		NotifiationId = EOS_P2P_AddNotifyPeerConnectionClosed(GetP2PHandle(WorldContextObject), &P2POptions, this, ([](const EOS_P2P_OnRemoteConnectionClosedInfo* data)
		{
			UCoreP2P* P2P = static_cast<UCoreP2P*>(data->ClientData);
			check(P2P);

			LogVerbose("");
		}));
	}

	return NotifiationId;
}

FEOSNotificationId UCoreP2P::EOSP2PAddNotifyPeerConnectionEstablished(UObject* WorldContextObject, FEOSP2PAddNotifyPeerConnectionEstablishedOptions Options, const FOnP2OnPeerConnectionEstablishedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetP2PHandle(WorldContextObject))
	{
		EOS_P2P_SocketId SocketId = { };
		SocketId.ApiVersion = EOS_P2P_SOCKETID_API_LATEST;
		FCStringAnsi::Strncpy(SocketId.SocketName, TCHAR_TO_UTF8(*Options.SocketId.SocketName), Options.SocketId.SocketName.Len() + 1);
		
		EOS_P2P_AddNotifyPeerConnectionEstablishedOptions P2POptions = {};
		P2POptions.ApiVersion = EOS_P2P_ADDNOTIFYPEERCONNECTIONESTABLISHED_API_LATEST;
		P2POptions.LocalUserId = Options.LocalUserId;
		P2POptions.SocketId = (Options.SocketId.SocketName.Len() > 0 ? &SocketId : nullptr);

		FPeerConnectionEstablishedCallback* CallbackObj = new FPeerConnectionEstablishedCallback(Callback);

		Id = EOS_P2P_AddNotifyPeerConnectionEstablished(GetP2PHandle(WorldContextObject), &P2POptions, CallbackObj, ([](const EOS_P2P_OnPeerConnectionEstablishedInfo* Data)
		{
			const FPeerConnectionEstablishedCallback* CallbackObj = static_cast<FPeerConnectionEstablishedCallback*>(Data->ClientData);
			CallbackObj->m_Callback.ExecuteIfBound(*Data);
		}));
		GetP2P(WorldContextObject)->m_PeerConnectionEstablishedCallbacks.Add(Id, CallbackObj);
	}

	return Id;
}

FEOSNotificationId UCoreP2P::EOSP2PAddNotifyPeerConnectionInterrupted(UObject* WorldContextObject, FEOSP2PAddNotifyPeerConnectionInterruptedOptions Options, const FOnP2OnPeerConnectionInterruptedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetP2PHandle(WorldContextObject))
	{
		EOS_P2P_SocketId SocketId = { };
		SocketId.ApiVersion = EOS_P2P_SOCKETID_API_LATEST;
		FCStringAnsi::Strncpy(SocketId.SocketName, TCHAR_TO_UTF8(*Options.SocketId.SocketName), Options.SocketId.SocketName.Len() + 1);
		
		EOS_P2P_AddNotifyPeerConnectionInterruptedOptions P2POptions = {};
		P2POptions.ApiVersion = EOS_P2P_ADDNOTIFYPEERCONNECTIONINTERRUPTED_API_LATEST;
		P2POptions.LocalUserId = Options.LocalUserId;
		P2POptions.SocketId = (Options.SocketId.SocketName.Len() > 0 ? &SocketId : nullptr);

		FPeerConnectionInterruptedCallback* CallbackObj = new FPeerConnectionInterruptedCallback(Callback);

		Id = EOS_P2P_AddNotifyPeerConnectionInterrupted(GetP2PHandle(WorldContextObject), &P2POptions, CallbackObj, ([](const EOS_P2P_OnPeerConnectionInterruptedInfo* Data)
		{
			const FPeerConnectionInterruptedCallback* CallbackObj = static_cast<FPeerConnectionInterruptedCallback*>(Data->ClientData);
			CallbackObj->m_Callback.ExecuteIfBound(*Data);
		}));
		GetP2P(WorldContextObject)->m_PeerConnectionInterruptedCallbacks.Add(Id, CallbackObj);
	}

	return Id;
}

void UCoreP2P::EOSP2PRemoveNotifyPeerConnectionInterrupted(UObject* WorldContextObject, FEOSNotificationId NotificationId)
{
	LogVerbose("");

	if (GetP2PHandle(WorldContextObject))
	{
		EOS_P2P_RemoveNotifyPeerConnectionInterrupted(GetP2PHandle(WorldContextObject), NotificationId);
		GetP2P(WorldContextObject)->m_PeerConnectionInterruptedCallbacks.Remove(NotificationId);
	}
}

void UCoreP2P::EOSP2PRemoveNotifyPeerConnectionEstablished(UObject* WorldContextObject, FEOSNotificationId NotificationId)
{
	LogVerbose("");

	if (GetP2PHandle(WorldContextObject))
	{
		EOS_P2P_RemoveNotifyPeerConnectionEstablished(GetP2PHandle(WorldContextObject), NotificationId);
		GetP2P(WorldContextObject)->m_PeerConnectionEstablishedCallbacks.Remove(NotificationId);
	}
}

void UCoreP2P::EOSP2PRemoveNotifyPeerConnectionClosed(UObject* WorldContextObject, FEOSNotificationId NotificationId)
{
	LogVerbose("");

	if (GetP2PHandle(WorldContextObject))
	{
		EOS_P2P_RemoveNotifyPeerConnectionClosed(GetP2PHandle(WorldContextObject), NotificationId);
	}
}

EOSResult UCoreP2P::EOSP2PAcceptConnection(UObject* WorldContextObject, FEOSP2PAcceptConnectionOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetP2PHandle(WorldContextObject))
	{
		EOS_P2P_SocketId SocketId = { };
		SocketId.ApiVersion = EOS_P2P_SOCKETID_API_LATEST;

		FCStringAnsi::Strncpy(SocketId.SocketName, TCHAR_TO_UTF8(*Options.SocketId.SocketName), Options.SocketId.SocketName.Len() + 1);

		EOS_P2P_AcceptConnectionOptions P2POptions = { };
		P2POptions.ApiVersion = Options.ApiVersion;
		P2POptions.LocalUserId = Options.LocalUserId;
		P2POptions.RemoteUserId = Options.RemoteUserId;
		P2POptions.SocketId = &SocketId;

		Result = EOSHelpers::ToEOSCoreResult(EOS_P2P_AcceptConnection(GetP2PHandle(WorldContextObject), &P2POptions));
	}

	return Result;
}

EOSResult UCoreP2P::EOSP2PCloseConnection(UObject* WorldContextObject, FEOSP2PCloseConnectionOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetP2PHandle(WorldContextObject))
	{
		EOS_P2P_SocketId SocketId = { };
		SocketId.ApiVersion = EOS_P2P_SOCKETID_API_LATEST;

		FCStringAnsi::Strncpy(SocketId.SocketName, TCHAR_TO_UTF8(*Options.SocketId.SocketName), Options.SocketId.SocketName.Len() + 1);

		EOS_P2P_CloseConnectionOptions P2POptions = { };
		P2POptions.ApiVersion = Options.ApiVersion;
		P2POptions.LocalUserId = Options.LocalUserId;
		P2POptions.RemoteUserId = Options.RemoteUserId;
		P2POptions.SocketId = &SocketId;

		Result = EOSHelpers::ToEOSCoreResult(EOS_P2P_CloseConnection(GetP2PHandle(WorldContextObject), &P2POptions));
	}

	return Result;
}

EOSResult UCoreP2P::EOSP2PCloseConnections(UObject* WorldContextObject, FEOSP2PCloseConnectionsOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetP2PHandle(WorldContextObject))
	{
		EOS_P2P_SocketId SocketId = { };
		SocketId.ApiVersion = EOS_P2P_SOCKETID_API_LATEST;

		FCStringAnsi::Strncpy(SocketId.SocketName, TCHAR_TO_UTF8(*Options.SocketId.SocketName), Options.SocketId.SocketName.Len() + 1);

		EOS_P2P_CloseConnectionsOptions P2POptions = { };
		P2POptions.ApiVersion = Options.ApiVersion;
		P2POptions.LocalUserId = Options.LocalUserId;
		P2POptions.SocketId = &SocketId;

		Result = EOSHelpers::ToEOSCoreResult(EOS_P2P_CloseConnections(GetP2PHandle(WorldContextObject), &P2POptions));
	}

	return Result;
}

struct FQueryNATTypeCallback
{
public:
	UCoreP2P* m_P2PObject;
	FOnP2PQueryNATTypeCompleteCallback m_Callback;
public:
	FQueryNATTypeCallback(UCoreP2P* obj, const FOnP2PQueryNATTypeCompleteCallback& callback)
		: m_P2PObject(obj)
		, m_Callback(callback)
	{
	}
};

void UCoreP2P::EOSP2PQueryNATType(UObject* WorldContextObject, const FOnP2PQueryNATTypeCompleteCallback& Callback)
{
	LogVerbose("");

	if (GetP2PHandle(WorldContextObject))
	{
		EOS_P2P_QueryNATTypeOptions P2POptions = { };
		P2POptions.ApiVersion = EOS_P2P_QUERYNATTYPE_API_LATEST;
		
		FQueryNATTypeCallback* CallbackObj = new FQueryNATTypeCallback({ this, Callback });
		EOS_P2P_QueryNATType(GetP2PHandle(WorldContextObject), &P2POptions, CallbackObj, Internal_OnQueryNatTypeCallback);
	}
}

EOSResult UCoreP2P::EOSP2PGetNATType(UObject* WorldContextObject, FEOSP2PGetNATTypeOptions Options, EEOSENATType& OutNatType)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_NotFound;

	if (GetP2PHandle(WorldContextObject))
	{
		EOS_P2P_GetNATTypeOptions P2POptions = { };
		P2POptions.ApiVersion = Options.ApiVersion;

		EOS_ENATType NAT = EOS_ENATType::EOS_NAT_Unknown;

		Result = EOSHelpers::ToEOSCoreResult(EOS_P2P_GetNATType(GetP2PHandle(WorldContextObject), &P2POptions, &NAT));

		if (Result == EOSResult::EOS_Success)
		{
			OutNatType = static_cast<EEOSENATType>(NAT);
		}
	}

	return Result;
}

EOSResult UCoreP2P::EOSP2PSetRelayControl(UObject* WorldContextObject, FEOSP2PSetRelayControlOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetP2PHandle(WorldContextObject))
	{
		EOS_P2P_SetRelayControlOptions P2POptions = { };
		P2POptions.ApiVersion = Options.ApiVersion;
		P2POptions.RelayControl = static_cast<EOS_ERelayControl>(Options.RelayControl);

		Result = EOSHelpers::ToEOSCoreResult(EOS_P2P_SetRelayControl(GetP2PHandle(WorldContextObject), &P2POptions));
	}

	return Result;
}

EOSResult UCoreP2P::EOSP2PGetRelayControl(UObject* WorldContextObject, FEOSP2PGetRelayControlOptions Options, EEOSERelayControl& OutRelayControl)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetP2PHandle(WorldContextObject))
	{
		EOS_P2P_GetRelayControlOptions P2POptions = { };
		P2POptions.ApiVersion = Options.ApiVersion;

		EOS_ERelayControl RelayControl = EOS_ERelayControl::EOS_RC_AllowRelays;

		Result = EOSHelpers::ToEOSCoreResult(EOS_P2P_GetRelayControl(GetP2PHandle(WorldContextObject), &P2POptions, &RelayControl));

		if (Result == EOSResult::EOS_Success)
		{
			OutRelayControl = static_cast<EEOSERelayControl>(RelayControl);
		}
	}

	return Result;
}

EOSResult UCoreP2P::EOSP2PSetPortRange(UObject* WorldContextObject, FEOSP2PSetPortRangeOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetP2PHandle(WorldContextObject))
	{
		EOS_P2P_SetPortRangeOptions P2POptions = { };
		P2POptions.ApiVersion = Options.ApiVersion;
		P2POptions.Port = static_cast<uint16_t>(Options.Port);
		P2POptions.MaxAdditionalPortsToTry = static_cast<uint16_t>(Options.MaxAdditionalPortsToTry);

		Result = EOSHelpers::ToEOSCoreResult(EOS_P2P_SetPortRange(GetP2PHandle(WorldContextObject), &P2POptions));
	}

	return Result;
}

EOSResult UCoreP2P::EOSP2PGetPortRange(UObject* WorldContextObject, FEOSP2PGetPortRangeOptions Options, int32& OutPort, int32 OutNumAdditionalPortsToTry)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetP2PHandle(WorldContextObject))
	{
		EOS_P2P_GetPortRangeOptions P2POptions = { };
		P2POptions.ApiVersion = Options.ApiVersion;

		uint16_t Port = 0;
		uint16_t AdditionalPorts = 0;

		Result = EOSHelpers::ToEOSCoreResult(EOS_P2P_GetPortRange(GetP2PHandle(WorldContextObject), &P2POptions, &Port, &AdditionalPorts));

		if (Result == EOSResult::EOS_Success)
		{
			OutPort = Port;
			OutNumAdditionalPortsToTry = AdditionalPorts;
		}
	}

	return Result;
}

EOSResult UCoreP2P::EOSP2PSetPacketQueueSize(UObject* WorldContextObject, FEOSP2PSetPacketQueueSizeOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetP2PHandle(WorldContextObject))
	{
		EOS_P2P_SetPacketQueueSizeOptions P2POptions = { };
		P2POptions.ApiVersion = Options.ApiVersion;
		P2POptions.IncomingPacketQueueMaxSizeBytes = FCString::Atoi64(*Options.IncomingPacketQueueMaxSizeBytes);
		P2POptions.OutgoingPacketQueueMaxSizeBytes = FCString::Atoi64(*Options.OutgoingPacketQueueMaxSizeBytes);

		Result = EOSHelpers::ToEOSCoreResult(EOS_P2P_SetPacketQueueSize(GetP2PHandle(WorldContextObject), &P2POptions));
	}

	return Result;
}

EOSResult UCoreP2P::EOSP2PGetPacketQueueInfo(UObject* WorldContextObject, FEOSP2PGetPacketQueueInfoOptions Options,
	FEOSP2PPacketQueueInfo& OutPacketQueueInfo)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetP2PHandle(WorldContextObject))
	{
		EOS_P2P_GetPacketQueueInfoOptions P2POptions = { };
		P2POptions.ApiVersion = Options.ApiVersion;
		
		EOS_P2P_PacketQueueInfo PacketQueueInfo = {};

		Result = EOSHelpers::ToEOSCoreResult(EOS_P2P_GetPacketQueueInfo(GetP2PHandle(WorldContextObject), &P2POptions, &PacketQueueInfo));

		if (Result == EOSResult::EOS_Success)
		{
			OutPacketQueueInfo = PacketQueueInfo;
		}
	}

	return Result;
}

FEOSNotificationId UCoreP2P::EOSP2PAddNotifyIncomingPacketQueueFull(UObject* WorldContextObject, 
	FEOSP2PAddNotifyIncomingPacketQueueFullOptions Options, const FOnP2POnIncomingPacketQueueFullCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetP2PHandle(WorldContextObject))
	{
		const EOS_P2P_AddNotifyIncomingPacketQueueFullOptions ConnectOptions = { EOS_CONNECT_ADDNOTIFYAUTHEXPIRATION_API_LATEST };

		FIncomingPacketQueueFullCallback* CallbackObj = new FIncomingPacketQueueFullCallback(Callback);

		Id = EOS_P2P_AddNotifyIncomingPacketQueueFull(GetP2PHandle(WorldContextObject), &ConnectOptions, CallbackObj, ([](const EOS_P2P_OnIncomingPacketQueueFullInfo* data)
        {
	        const FIncomingPacketQueueFullCallback* CallbackObj = static_cast<FIncomingPacketQueueFullCallback*>(data->ClientData);
            CallbackObj->m_Callback.ExecuteIfBound(*data);
        }));
		
		GetP2P(WorldContextObject)->m_IncomingPacketQueueFullCallbacks.Add(Id, CallbackObj);
	}

	return Id;
}

void UCoreP2P::EOSP2PRemoveNotifyIncomingPacketQueueFull(UObject* WorldContextObject, FEOSNotificationId NotificationId)
{
	LogVerbose("");

	if (GetP2PHandle(WorldContextObject))
	{
		EOS_P2P_RemoveNotifyIncomingPacketQueueFull(GetP2PHandle(WorldContextObject), NotificationId);
		GetP2P(WorldContextObject)->m_IncomingPacketQueueFullCallbacks.Remove(NotificationId);
	}
}

void UCoreP2P::Internal_OnQueryNatTypeCallback(const EOS_P2P_OnQueryNATTypeCompleteInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FQueryNATTypeCallback* CallbackObj = static_cast<FQueryNATTypeCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}
