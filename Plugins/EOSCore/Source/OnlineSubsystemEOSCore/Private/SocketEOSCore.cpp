/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "SocketEOSCore.h"
#include "SocketSubsystemEOSCore.h"
#include "OnlineSubsystemModuleEOSCore.h"
#include "OnlineSubsystemEOSCorePrivatePCH.h"

FSocketEOSCore::FSocketEOSCore(FSocketSubsystemEOSCore& InSocketSubsystem, const FString& InSocketDescription)
	: FSocket(ESocketType::SOCKTYPE_Datagram, InSocketDescription, NAME_None)
	  , bIsListening(false)
	  , m_SocketSubsystem(InSocketSubsystem)
	  , m_ConnectNotifyCallback(nullptr)
	  , m_ConnectNotifyId(EOS_INVALID_NOTIFICATIONID)
	  , m_ClosedNotifyCallback(nullptr)
	  , m_ClosedNotifyId(EOS_INVALID_NOTIFICATIONID)
{
}

FSocketEOSCore::~FSocketEOSCore()
{
	LogVerbose("");

	Close();

	if (m_LocalAddress.IsValid())
	{
		m_SocketSubsystem.UnbindChannel(m_LocalAddress);
		m_LocalAddress = FInternetAddrEOSCore();
	}
}

bool FSocketEOSCore::Shutdown(ESocketShutdownMode Mode)
{
	LogVerbose("");

	m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return false;
}

bool FSocketEOSCore::Close()
{
	LogVerbose("");

	check(IsInGameThread() && "p2p does not support multithreading");

	if (m_ConnectNotifyId != EOS_INVALID_NOTIFICATIONID)
	{
		EOS_P2P_RemoveNotifyPeerConnectionRequest(m_SocketSubsystem.GetP2PHandle(), m_ConnectNotifyId);
	}
	delete m_ConnectNotifyCallback;
	m_ConnectNotifyCallback = nullptr;
	if (m_ClosedNotifyId != EOS_INVALID_NOTIFICATIONID)
	{
		EOS_P2P_RemoveNotifyPeerConnectionClosed(m_SocketSubsystem.GetP2PHandle(), m_ClosedNotifyId);
	}
	delete m_ClosedNotifyCallback;
	m_ClosedNotifyCallback = nullptr;

	if (m_LocalAddress.IsValid())
	{
		EOS_P2P_SocketId SocketId = {};
		SocketId.ApiVersion = EOS_P2P_SOCKETID_API_LATEST;
		FCStringAnsi::Strcpy(SocketId.SocketName, m_LocalAddress.GetSocketName());

		EOS_P2P_CloseConnectionsOptions Options = {};
		Options.ApiVersion = EOS_P2P_CLOSECONNECTIONS_API_LATEST;
		Options.LocalUserId = m_SocketSubsystem.GetLocalUserId();
		Options.SocketId = &SocketId;

		EOS_EResult Result = EOS_P2P_CloseConnections(m_SocketSubsystem.GetP2PHandle(), &Options);

		UE_LOG(LogEOSCoreSubsystem, Log, TEXT("Closing socket (%s) with result (%s)"), *m_LocalAddress.ToString(true), ANSI_TO_TCHAR(EOS_EResult_ToString(Result)));

		m_ClosedRemotes.Empty();
	}
	return true;
}

bool FSocketEOSCore::Bind(const FInternetAddr& Addr)
{
	LogVerbose("");

	check(IsInGameThread() && "p2p does not support multithreading");

	if (!Addr.IsValid())
	{
		UE_LOG(LogEOSCoreSubsystem, Warning, TEXT("Attempted to bind to invalid address. Address = (%s)"), *Addr.ToString(true));
		m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EADDRNOTAVAIL);
		return false;
	}

	if (m_LocalAddress.GetRemoteUserId() != nullptr)
	{
		UE_LOG(LogEOSCoreSubsystem, Warning, TEXT("Attempted to bind on a socket that was not initialized. Address = (%s)"), *Addr.ToString(true));
		m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_NOTINITIALISED);
		return false;
	}

	if (m_LocalAddress.GetRemoteUserId() != nullptr)
	{
		UE_LOG(LogEOSCoreSubsystem, Warning, TEXT("Attempted to bind a socket that was already bound. ExistingAddress = (%s) NewAddress = (%s)"), *m_LocalAddress.ToString(true),
		       *Addr.ToString(true));
		m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EADDRINUSE);
		return false;
	}

	const FInternetAddrEOSCore& EOSAddr = static_cast<const FInternetAddrEOSCore&>(Addr);
	if (!m_SocketSubsystem.BindChannel(EOSAddr))
	{
		UE_LOG(LogEOSCoreSubsystem, Warning, TEXT("Attempted to bind a socket to a port in use. NewAddress = (%s)"), *Addr.ToString(true));
		return false;
	}

	EOS_ProductUserId LocalUserId = m_LocalAddress.GetLocalUserId();

	m_LocalAddress = EOSAddr;
	m_LocalAddress.SetLocalUserId(LocalUserId);

	UE_LOG(LogEOSCoreSubsystem, Verbose, TEXT("Successfully bound socket to address (%s)"), *m_LocalAddress.ToString(true));
	return true;
}

bool FSocketEOSCore::Connect(const FInternetAddr& Addr)
{
	/** Not supported - connectionless (UDP) only */
	m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return false;
}

bool FSocketEOSCore::Listen(int32)
{
	LogVerbose("");

	check(IsInGameThread() && "p2p does not support multithreading");

	if (!m_LocalAddress.IsValid())
	{
		UE_LOG(LogEOSCoreSubsystem, Warning, TEXT("Attempted to listen without a bound address. Address = (%s)"), *m_LocalAddress.ToString(true));
		m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EADDRINUSE);
		return false;
	}

	EOS_P2P_SocketId SocketId = {};
	SocketId.ApiVersion = EOS_P2P_SOCKETID_API_LATEST;
	FCStringAnsi::Strcpy(SocketId.SocketName, m_LocalAddress.GetSocketName());

	EOS_P2P_AddNotifyPeerConnectionRequestOptions Options = {};
	Options.ApiVersion = EOS_P2P_ADDNOTIFYPEERCONNECTIONREQUEST_API_LATEST;
	Options.LocalUserId = m_LocalAddress.GetLocalUserId();
	Options.SocketId = &SocketId;

	m_ConnectNotifyCallback = new FConnectNotifyCallback();
	m_ConnectNotifyCallback->m_CallbackLambda = [this](const EOS_P2P_OnIncomingConnectionRequestInfo* Info)
	{
		char PuidBuffer[64];
		int32 BufferLen = 64;
		if (EOS_ProductUserId_ToString(Info->RemoteUserId, PuidBuffer, &BufferLen) != EOS_EResult::EOS_Success)
		{
			PuidBuffer[0] = '\0';
		}
		
		const FString RemoteUser(PuidBuffer);

		if (Info->LocalUserId == m_LocalAddress.GetLocalUserId() && FCStringAnsi::Stricmp(Info->SocketId->SocketName, m_LocalAddress.GetSocketName()) == 0)
		{
			FInternetAddrEOSCore RemoteAddress(Info->RemoteUserId, Info->SocketId->SocketName, m_LocalAddress.GetChannel());
			RemoteAddress.SetLocalUserId(m_LocalAddress.GetLocalUserId());
			m_ClosedRemotes.Remove(RemoteAddress);

			EOS_P2P_SocketId SocketId = {};
			SocketId.ApiVersion = EOS_P2P_SOCKETID_API_LATEST;
			FCStringAnsi::Strcpy(SocketId.SocketName, Info->SocketId->SocketName);

			EOS_P2P_AcceptConnectionOptions Options = {};
			Options.ApiVersion = EOS_P2P_ACCEPTCONNECTION_API_LATEST;
			Options.LocalUserId = m_LocalAddress.GetLocalUserId();
			Options.RemoteUserId = Info->RemoteUserId;
			Options.SocketId = &SocketId;
			EOS_EResult AcceptResult = EOS_P2P_AcceptConnection(m_SocketSubsystem.GetP2PHandle(), &Options);
			if (AcceptResult == EOS_EResult::EOS_Success)
			{
				UE_LOG(LogEOSCoreSubsystem, Verbose, TEXT("Accepting connection request from (%s) on socket (%s)"), *RemoteUser, UTF8_TO_TCHAR(Info->SocketId->SocketName));
			}
			else
			{
				UE_LOG(LogEOSCoreSubsystem, Error, TEXT("EOS_P2P_AcceptConnection from (%s) on socket (%s) failed with (%s)"), *RemoteUser, UTF8_TO_TCHAR(Info->SocketId->SocketName),
				       ANSI_TO_TCHAR(EOS_EResult_ToString(AcceptResult)));
			}
		}
		else
		{
			UE_LOG(LogEOSCoreSubsystem, Warning, TEXT("Ignoring connection request from (%s) on socket (%s)"), *RemoteUser, UTF8_TO_TCHAR(Info->SocketId->SocketName));
		}
	};
	m_ConnectNotifyId = EOS_P2P_AddNotifyPeerConnectionRequest(m_SocketSubsystem.GetP2PHandle(), &Options, m_ConnectNotifyCallback, m_ConnectNotifyCallback->GetCallbackPtr());

	RegisterClosedNotification();

	bIsListening = true;

	return true;
}

bool FSocketEOSCore::WaitForPendingConnection(bool& bHasPendingConnection, const FTimespan& WaitTime)
{
	/** Not supported - connectionless (UDP) only */
	m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return false;
}

bool FSocketEOSCore::HasPendingData(uint32& PendingDataSize)
{
	check(IsInGameThread() && "p2p does not support multithreading");

	PendingDataSize = 0;

	EOS_P2P_GetNextReceivedPacketSizeOptions Options = {};
	Options.ApiVersion = EOS_P2P_GETNEXTRECEIVEDPACKETSIZE_API_LATEST;
	Options.LocalUserId = m_LocalAddress.GetLocalUserId();
	const uint8 Channel = m_LocalAddress.GetChannel();
	Options.RequestedChannel = &Channel;

	const EOS_EResult Result = EOS_P2P_GetNextReceivedPacketSize(m_SocketSubsystem.GetP2PHandle(), &Options, &PendingDataSize);
	if (Result != EOS_EResult::EOS_Success)
	{
		UE_LOG(LogEOSCoreSubsystem, Warning, TEXT("Unable to check for data on address (%s) result code = (%s)"), *m_LocalAddress.ToString(true), ANSI_TO_TCHAR(EOS_EResult_ToString(Result)));

		m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EINVAL);
		return false;
	}

	return true;
}

FSocket* FSocketEOSCore::Accept(const FString& InSocketDescription)
{
	/** Not supported - connectionless (UDP) only */
	m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return nullptr;
}

FSocket* FSocketEOSCore::Accept(FInternetAddr& OutAddr, const FString& InSocketDescription)
{
	/** Not supported - connectionless (UDP) only */
	m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return nullptr;
}

bool FSocketEOSCore::SendTo(const uint8* Data, int32 Count, int32& OutBytesSent, const FInternetAddr& Destination)
{
	check(IsInGameThread() && "p2p does not support multithreading");

	OutBytesSent = 0;

	if (!Destination.IsValid())
	{
		UE_LOG(LogEOSCoreSubsystem, Warning, TEXT("Unable to send data, invalid destination address. DestinationAddress = (%s)"), *Destination.ToString(true));

		m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EADDRNOTAVAIL);
		return false;
	}

	if (Count > EOS_P2P_MAX_PACKET_SIZE)
	{
		UE_LOG(LogEOSCoreSubsystem, Warning, TEXT("Unable to send data, data over maximum size. Amount=[%d/%d] DestinationAddress = (%s)"), Count, EOS_P2P_MAX_PACKET_SIZE,
		       *Destination.ToString(true));

		m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EMSGSIZE);
		return false;
	}

	if (Count < 0)
	{
		UE_LOG(LogEOSCoreSubsystem, Warning, TEXT("Unable to send data, data invalid. Amount=[%d/%d] DestinationAddress = (%s)"), Count, EOS_P2P_MAX_PACKET_SIZE, *Destination.ToString(true));

		m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EINVAL);
		return false;
	}

	if (Data == nullptr && Count != 0)
	{
		UE_LOG(LogEOSCoreSubsystem, Warning, TEXT("Unable to send data, data invalid. DestinationAddress = (%s)"), *Destination.ToString(true));

		m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EINVAL);
		return false;
	}

	if (!m_LocalAddress.IsValid())
	{
		UE_LOG(LogEOSCoreSubsystem, Warning, TEXT("Unable to send data, socket was not initialized. DestinationAddress = (%s)"), *Destination.ToString(true));

		m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_NOTINITIALISED);
		return false;
	}

	const FInternetAddrEOSCore& DestinationAddress = static_cast<const FInternetAddrEOSCore&>(Destination);
	if (m_LocalAddress == DestinationAddress)
	{
		UE_LOG(LogEOSCoreSubsystem, Warning, TEXT("Unable to send data, unable to send data to ourselves. DestinationAddress = (%s)"), *Destination.ToString(true));

		m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_ECONNREFUSED);
		return false;
	}

	if (WasClosed(DestinationAddress))
	{
		UE_LOG(LogEOSCoreSubsystem, Warning, TEXT("Unable to send data to closed connection. DestinationAddress = (%s)"), *Destination.ToString(true));

		m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_ECONNREFUSED);
		return false;
	}

	RegisterClosedNotification();

	EOS_P2P_SocketId SocketId = {};
	SocketId.ApiVersion = EOS_P2P_SOCKETID_API_LATEST;
	FCStringAnsi::Strcpy(SocketId.SocketName, DestinationAddress.GetSocketName());

	EOS_P2P_SendPacketOptions Options = {};
	Options.ApiVersion = EOS_P2P_SENDPACKET_API_LATEST;
	Options.LocalUserId = m_LocalAddress.GetLocalUserId();
	Options.RemoteUserId = DestinationAddress.GetRemoteUserId();
	Options.SocketId = &SocketId;
	Options.bAllowDelayedDelivery = EOS_TRUE;
	Options.Reliability = EOS_EPacketReliability::EOS_PR_UnreliableUnordered;
	Options.Channel = DestinationAddress.GetChannel();
	Options.DataLengthBytes = Count;
	Options.Data = Data;
	const EOS_EResult Result = EOS_P2P_SendPacket(m_SocketSubsystem.GetP2PHandle(), &Options);
	if (Result != EOS_EResult::EOS_Success)
	{
		UE_LOG(LogEOSCoreSubsystem, Error, TEXT("Unable to send data to (%s) result code = (%s)"), *Destination.ToString(true), ANSI_TO_TCHAR(EOS_EResult_ToString(Result)));

		m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EINVAL);
		return false;
	}
	OutBytesSent = Count;
	return true;
}

bool FSocketEOSCore::Send(const uint8* Data, int32 Count, int32& BytesSent)
{
	/** Not supported - connectionless (UDP) only */
	m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	BytesSent = 0;
	return false;
}

bool FSocketEOSCore::RecvFrom(uint8* Data, int32 BufferSize, int32& BytesRead, FInternetAddr& Source, ESocketReceiveFlags::Type Flags)
{
	check(IsInGameThread() && "p2p does not support multithreading");
	BytesRead = 0;

	if (BufferSize < 0)
	{
		UE_LOG(LogEOSCoreSubsystem, Error, TEXT("Unable to receive data, receiving buffer was invalid. BufferSize = (%d)"), BufferSize);

		m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EINVAL);
		return false;
	}

	if (Flags != ESocketReceiveFlags::None)
	{
		UE_LOG(LogEOSCoreSubsystem, Error, TEXT("Socket receive flags (%d) are not supported"), int32(Flags));

		m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
		return false;
	}

	EOS_P2P_ReceivePacketOptions Options = {};
	Options.ApiVersion = EOS_P2P_RECEIVEPACKET_API_LATEST;
	Options.LocalUserId = m_LocalAddress.GetLocalUserId();
	Options.MaxDataSizeBytes = BufferSize;
	uint8 Channel = m_LocalAddress.GetChannel();
	Options.RequestedChannel = &Channel;

	EOS_ProductUserId RemoteUserId = nullptr;
	EOS_P2P_SocketId SocketId;

	EOS_EResult Result = EOS_P2P_ReceivePacket(m_SocketSubsystem.GetP2PHandle(), &Options, &RemoteUserId, &SocketId, &Channel, Data, (uint32*)&BytesRead);
	if (Result == EOS_EResult::EOS_NotFound)
	{
		m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EWOULDBLOCK);
		return false;
	}
	else if (Result != EOS_EResult::EOS_Success)
	{
		UE_LOG(LogEOSCoreSubsystem, Error, TEXT("Unable to receive data result code = (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(Result)));

		m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EINVAL);
		return false;
	}

	FInternetAddrEOSCore& SourceAddress = static_cast<FInternetAddrEOSCore&>(Source);
	SourceAddress.SetLocalUserId(m_LocalAddress.GetLocalUserId());
	SourceAddress.SetRemoteUserId(RemoteUserId);
	SourceAddress.SetSocketName(SocketId.SocketName);
	SourceAddress.SetChannel(Channel);

	return true;
}

bool FSocketEOSCore::Recv(uint8* Data, int32 BufferSize, int32& BytesRead, ESocketReceiveFlags::Type Flags)
{
	BytesRead = 0;
	m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return false;
}

bool FSocketEOSCore::Wait(ESocketWaitConditions::Type Condition, FTimespan WaitTime)
{
	m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return false;
}

ESocketConnectionState FSocketEOSCore::GetConnectionState()
{
	return ESocketConnectionState::SCS_NotConnected;
}

void FSocketEOSCore::GetAddress(FInternetAddr& OutAddr)
{
	OutAddr = m_LocalAddress;
}

bool FSocketEOSCore::GetPeerAddress(FInternetAddr& OutAddr)
{
	m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return false;
}

bool FSocketEOSCore::SetNonBlocking(bool bIsNonBlocking)
{
	return true;
}

bool FSocketEOSCore::SetBroadcast(bool bAllowBroadcast)
{
	return true;
}

bool FSocketEOSCore::SetNoDelay(bool bIsNoDelay)
{
	return true;
}

bool FSocketEOSCore::JoinMulticastGroup(const FInternetAddr& GroupAddress)
{
	m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return false;
}

bool FSocketEOSCore::LeaveMulticastGroup(const FInternetAddr& GroupAddress)
{
	m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return false;
}

bool FSocketEOSCore::SetMulticastLoopback(bool bLoopback)
{
	m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return false;
}

bool FSocketEOSCore::SetMulticastTtl(uint8 TimeToLive)
{
	m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return false;
}

bool FSocketEOSCore::JoinMulticastGroup(const FInternetAddr& GroupAddress, const FInternetAddr& InterfaceAddress)
{
	m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return false;
}

bool FSocketEOSCore::LeaveMulticastGroup(const FInternetAddr& GroupAddress, const FInternetAddr& InterfaceAddress)
{
	m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return false;
}

bool FSocketEOSCore::SetMulticastInterface(const FInternetAddr& InterfaceAddress)
{
	m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EOPNOTSUPP);
	return false;
}

bool FSocketEOSCore::SetReuseAddr(bool bAllowReuse)
{
	return true;
}

bool FSocketEOSCore::SetLinger(bool bShouldLinger, int32 Timeout)
{
	return true;
}

bool FSocketEOSCore::SetRecvErr(bool bUseErrorQueue)
{
	return true;
}

bool FSocketEOSCore::SetSendBufferSize(int32 Size, int32& NewSize)
{
	return true;
}

bool FSocketEOSCore::SetReceiveBufferSize(int32 Size, int32& NewSize)
{
	return true;
}

int32 FSocketEOSCore::GetPortNo()
{
	return m_LocalAddress.GetChannel();
}

void FSocketEOSCore::SetLocalAddress(const FInternetAddrEOSCore& InLocalAddress)
{
	m_LocalAddress = InLocalAddress;
}

bool FSocketEOSCore::Close(const FInternetAddrEOSCore& RemoteAddress)
{
	LogVerbose("");

	check(IsInGameThread() && "p2p does not support multithreading");

	if (!RemoteAddress.IsValid())
	{
		UE_LOG(LogEOSCoreSubsystem, Error, TEXT("Unable to close socket with remote address as it is invalid RemoteAddress = (%s)"), *RemoteAddress.ToString(true));
		m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EADDRNOTAVAIL);
		return false;
	}

	m_ClosedRemotes.Add(RemoteAddress);

	EOS_P2P_SocketId SocketId = {};
	SocketId.ApiVersion = EOS_P2P_SOCKETID_API_LATEST;
	FCStringAnsi::Strcpy(SocketId.SocketName, RemoteAddress.GetSocketName());

	EOS_P2P_CloseConnectionOptions Options = {};
	Options.ApiVersion = EOS_P2P_CLOSECONNECTION_API_LATEST;
	Options.LocalUserId = m_LocalAddress.GetLocalUserId();
	Options.RemoteUserId = RemoteAddress.GetRemoteUserId();
	Options.SocketId = &SocketId;

	EOS_EResult Result = EOS_P2P_CloseConnection(m_SocketSubsystem.GetP2PHandle(), &Options);
	if (Result != EOS_EResult::EOS_Success)
	{
		UE_LOG(LogEOSCoreSubsystem, Error, TEXT("Unable to close socket with remote address RemoteAddress (%s) due to error (%s)"), *RemoteAddress.ToString(true),
		       ANSI_TO_TCHAR(EOS_EResult_ToString(Result)));

		m_SocketSubsystem.SetLastSocketError(ESocketErrors::SE_EINVAL);
		return false;
	}

	return true;
}

void FSocketEOSCore::RegisterClosedNotification()
{
	if (m_ClosedNotifyId != EOS_INVALID_NOTIFICATIONID)
	{
		return;
	}

	LogVerbose("");

	EOS_P2P_SocketId SocketId = {};
	SocketId.ApiVersion = EOS_P2P_SOCKETID_API_LATEST;
	FCStringAnsi::Strcpy(SocketId.SocketName, m_LocalAddress.GetSocketName());

	EOS_P2P_AddNotifyPeerConnectionClosedOptions Options = {};
	Options.ApiVersion = EOS_P2P_ADDNOTIFYPEERCONNECTIONCLOSED_API_LATEST;
	Options.LocalUserId = m_LocalAddress.GetLocalUserId();
	Options.SocketId = &SocketId;

	m_ClosedNotifyCallback = new FClosedNotifyCallback();
	m_ClosedNotifyCallback->m_CallbackLambda = [this](const EOS_P2P_OnRemoteConnectionClosedInfo* Info)
	{
		FInternetAddrEOSCore RemoteAddress(Info->RemoteUserId, Info->SocketId->SocketName, m_LocalAddress.GetChannel());
		RemoteAddress.SetLocalUserId(m_LocalAddress.GetLocalUserId());
		m_ClosedRemotes.Add(RemoteAddress);
	};
	m_ClosedNotifyId = EOS_P2P_AddNotifyPeerConnectionClosed(m_SocketSubsystem.GetP2PHandle(), &Options, m_ClosedNotifyCallback, m_ClosedNotifyCallback->GetCallbackPtr());
}
