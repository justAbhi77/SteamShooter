/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "NetDriverEOSCore.h"
#include "NetConnectionEOSCore.h"
#include "SocketEOSCore.h"
#include "SocketSubsystemEOSCore.h"
#include "OnlineSubsystemEOSCore.h"
#include "OnlineSubsystemModuleEOSCore.h"
#include "OnlineSubsystemEOSCorePrivatePCH.h"

bool UNetDriverEOSCore::IsAvailable() const
{
	LogVerbose("");

	if (IsRunningDedicatedServer())
	{
		bool bSocketsEnabled = false;
		GConfig->GetBool(TEXT("/Script/OnlineSubsystemEOSCore.NetDriverEOSCore"), TEXT("bIsUsingP2PSockets"), bSocketsEnabled, GEngineIni);

		if (bSocketsEnabled)
		{
			LogError(
				"You have Sockets enabled for the NetDriverEOS while running a Dedicated Server. This is a unsupported configuration. Make sure you set bIsUsingP2PSockets=false when using a Dedicated Server or you will not be able to connect.");
		}

		return false;
	}

	if (IOnlineSubsystem* Subsystem = Online::GetSubsystem(FindWorld(), EOSCORE_SUBSYSTEM))
	{
		if (ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(EOSCORE_SUBSYSTEM))
		{
			return true;
		}
	}

	return false;
}

bool UNetDriverEOSCore::InitBase(bool bInitAsClient, FNetworkNotify* InNotify, const FURL& URL, bool bReuseAddressAndPort, FString& Error)
{
	LogVerbose("");

	if (bIsPassthrough)
	{
		LogVerbose("Running as pass-through");

		TArray<FString> Tokens;

		if (URL.Host.ParseIntoArray(Tokens, TEXT("."), false) == 4 || !URL.Host.Contains(EOSCORE_CONNECTION_URL_PREFIX))
		{
			bool bSocketsEnabled = false;
			GConfig->GetBool(TEXT("/Script/OnlineSubsystemEOSCore.NetDriverEOSCore"), TEXT("bIsUsingP2PSockets"), bSocketsEnabled, GEngineIni);

			if (bSocketsEnabled)
			{
				LogError("You are connecting to a regular IPV4 Addr while using EOS Sockets. This is a unsupported configuration. Set bIsUsingP2PSockets=false in your DefaultEngine.ini");

//				return false;
			}
		}

		return Super::InitBase(bInitAsClient, InNotify, URL, bReuseAddressAndPort, Error);
	}

	if (!UNetDriver::InitBase(bInitAsClient, InNotify, URL, bReuseAddressAndPort, Error))
	{
		LogWarning("Failed to init driver base");
		return false;
	}

	FSocketSubsystemEOSCore* const SocketSubsystem = static_cast<FSocketSubsystemEOSCore*>(GetSocketSubsystem());

	if (!SocketSubsystem)
	{
		LogWarning("Could not get socket subsystem");
		return false;
	}

	const UWorld* const MyWorld = FindWorld();

	TSharedRef<FInternetAddr> LocalAddress = SocketSubsystem->GetLocalBindAddr(MyWorld, *GLog);

	if (!LocalAddress->IsValid())
	{
		Error = TEXT("Could not bind local address");
		LogWarning("Could not bind local address");
		return false;
	}

	SetSocketAndLocalAddress(SocketSubsystem->CreateSocket(TEXT("EOSCORE"), TEXT("UE4"), NAME_None));

	if (GetSocket() == nullptr)
	{
		LogWarning("Could not create socket");
		return false;
	}

	TSharedRef<FInternetAddrEOSCore> EOSLocalAddress = StaticCastSharedRef<FInternetAddrEOSCore>(LocalAddress);
	EOSLocalAddress->SetChannel(bInitAsClient ? GetClientPort() : URL.Port);

	static_cast<FSocketEOSCore*>(GetSocket())->SetLocalAddress(*EOSLocalAddress);

	LocalAddr = LocalAddress;

	return true;
}

bool UNetDriverEOSCore::InitConnect(FNetworkNotify* InNotify, const FURL& ConnectURL, FString& Error)
{
	LogVerbose("");

	if (!bIsUsingP2PSockets || !IsAvailable() || !ConnectURL.Host.StartsWith(EOSCORE_CONNECTION_URL_PREFIX, ESearchCase::IgnoreCase))
	{
		LogVerbose("Connecting using IPNetDriver passthrough. ConnectUrl = (%s)", *ConnectURL.ToString());

		bIsPassthrough = true;
		return Super::InitConnect(InNotify, ConnectURL, Error);
	}

	bool bIsValid = false;
	TSharedRef<FInternetAddrEOSCore> RemoteHost = MakeShared<FInternetAddrEOSCore>();
	RemoteHost->SetIp(*ConnectURL.Host, bIsValid);
	if (!bIsValid || ConnectURL.Port < 0)
	{
		Error = TEXT("Invalid remote address");
		LogWarning("Invalid Remote Address. ConnectUrl = (%s)", *ConnectURL.ToString());
		return false;
	}

	LogVerbose("Connecting using EOSNetDriver. ConnectUrl = (%s)", *ConnectURL.ToString());

	if (!InitBase(true, InNotify, ConnectURL, false, Error))
	{
		return false;
	}

	LocalAddr = RemoteHost;

	FSocket* CurSocket = GetSocket();

	FSocketSubsystemEOSCore* const SocketSubsystem = static_cast<FSocketSubsystemEOSCore*>(GetSocketSubsystem());
	check(SocketSubsystem);
	if (!SocketSubsystem->BindNextPort(CurSocket, *LocalAddr, MaxPortCountToTry + 1, 1))
	{
		Error = TEXT("Could not bind local port");
		LogWarning("Could not bind local port in %d attempts", MaxPortCountToTry);
		return false;
	}

	UNetConnectionEOSCore* Connection = NewObject<UNetConnectionEOSCore>(NetConnectionClass);
	check(Connection);

	ServerConnection = Connection;
	Connection->InitLocalConnection(this, CurSocket, ConnectURL, USOCK_Pending);

	CreateInitialClientChannels();

	return true;
}

bool UNetDriverEOSCore::InitListen(FNetworkNotify* InNotify, FURL& LocalURL, bool bReuseAddressAndPort, FString& Error)
{
	LogVerbose("");

	if (!bIsUsingP2PSockets || !IsAvailable() || LocalURL.HasOption(TEXT("bIsLanMatch")) || LocalURL.HasOption(TEXT("bUseIPSockets")))
	{
		LogVerbose("Init as IPNetDriver listen server. LocalURL = (%s)", *LocalURL.ToString());

		bIsPassthrough = true;
		return Super::InitListen(InNotify, LocalURL, bReuseAddressAndPort, Error);
	}

	LogVerbose("Init as EOSNetDriver listen server. LocalURL = (%s)", *LocalURL.ToString());

	if (!InitBase(false, InNotify, LocalURL, bReuseAddressAndPort, Error))
	{
		return false;
	}

	FSocket* CurSocket = GetSocket();

	if (!CurSocket->Listen(0))
	{
		Error = TEXT("Could not listen");
		LogWarning("Could not listen on socket");
		return false;
	}

	InitConnectionlessHandler();

	LogVerbose("Initialized as an EOSP2P listen server");
	return true;
}

ISocketSubsystem* UNetDriverEOSCore::GetSocketSubsystem()
{
#if UE_EDITOR
	if (FindWorld())
	{
		FOnlineSubsystemEOSCore* m_Subsystem = static_cast<FOnlineSubsystemEOSCore*>(Online::GetSubsystem(FindWorld(), EOSCORE_SUBSYSTEM));

		if (m_Subsystem && m_Subsystem->m_SocketSubsystem)
			return m_Subsystem->m_SocketSubsystem.Get();
	}
#endif
	return ISocketSubsystem::Get(bIsPassthrough ? PLATFORM_SOCKETSUBSYSTEM : EOSCORE_SUBSYSTEM);
}

void UNetDriverEOSCore::Shutdown()
{
	LogVerbose("Shutting down NetDriver");

	Super::Shutdown();

	if (!bIsPassthrough)
	{
		if (UNetConnectionEOSCore* const EOSServerConnection = Cast<UNetConnectionEOSCore>(ServerConnection))
		{
			EOSServerConnection->DestroyEOSConnection();
		}
		for (UNetConnection* Client : ClientConnections)
		{
			if (UNetConnectionEOSCore* const EOSClient = Cast<UNetConnectionEOSCore>(Client))
			{
				EOSClient->DestroyEOSConnection();
			}
		}
	}
}

int UNetDriverEOSCore::GetClientPort()
{
	LogVerbose("");

	if (bIsPassthrough)
	{
		return Super::GetClientPort();
	}

	return 1025;
}

UWorld* UNetDriverEOSCore::FindWorld() const
{
	UWorld* MyWorld = GetWorld();

	if (!MyWorld && GEngine)
	{
		if (FWorldContext* WorldContext = GEngine->GetWorldContextFromPendingNetGameNetDriver(this))
		{
			MyWorld = WorldContext->World();
		}
	}

	return MyWorld;
}
