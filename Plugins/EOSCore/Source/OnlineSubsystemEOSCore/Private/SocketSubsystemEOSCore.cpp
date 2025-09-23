/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "SocketSubsystemEOSCore.h"
#include "InternetAddrEOSCore.h"
#include "SocketEOSCore.h"
#include "OnlineSubsystemEOSCore.h"
#include "OnlineIdentityEOSCore.h"
#include "OnlineSubsystemEOSCorePrivatePCH.h"

FSocketSubsystemEOSCore::FSocketSubsystemEOSCore(FOnlineSubsystemEOSCore* InSubsystemEOS)
	: m_Subsystem(InSubsystemEOS)
	  , m_LastSocketError(ESocketErrors::SE_NO_ERROR)
{
}

FSocketSubsystemEOSCore::~FSocketSubsystemEOSCore() = default;

bool FSocketSubsystemEOSCore::Init(FString& Error)
{
	FSocketSubsystemModule& SocketSubsystem = FModuleManager::LoadModuleChecked<FSocketSubsystemModule>("Sockets");
	SocketSubsystem.RegisterSocketSubsystem(EOSCORE_SUBSYSTEM, this, false);

	return true;
}

void FSocketSubsystemEOSCore::Shutdown()
{
	m_TrackedSockets.Reset();
}

FSocket* FSocketSubsystemEOSCore::CreateSocket(const FName& SocketTypeName, const FString& SocketDescription, const FName& /*unused*/)
{
	return m_TrackedSockets.Emplace_GetRef(MakeUnique<FSocketEOSCore>(*this, SocketDescription)).Get();
}

FResolveInfoCached* FSocketSubsystemEOSCore::CreateResolveInfoCached(TSharedPtr<FInternetAddr> Addr) const
{
	return nullptr;
}

void FSocketSubsystemEOSCore::DestroySocket(FSocket* Socket)
{
	for (auto It = m_TrackedSockets.CreateIterator(); It; ++It)
	{
		if (It->IsValid() && It->Get() == Socket)
		{
			It.RemoveCurrent();
			return;
		}
	}
}

FAddressInfoResult FSocketSubsystemEOSCore::GetAddressInfo(const TCHAR* HostName, const TCHAR* ServiceName, EAddressInfoFlags /*unused*/, const FName /*unused*/, ESocketType /*unused*/)
{
	return FAddressInfoResult(HostName, ServiceName);
}

bool FSocketSubsystemEOSCore::RequiresChatDataBeSeparate()
{
	return false;
}

bool FSocketSubsystemEOSCore::RequiresEncryptedPackets()
{
	return false;
}

bool FSocketSubsystemEOSCore::GetHostName(FString& HostName)
{
	return false;
}

TSharedRef<FInternetAddr> FSocketSubsystemEOSCore::CreateInternetAddr()
{
	return MakeShared<FInternetAddrEOSCore>();
}

TSharedPtr<FInternetAddr> FSocketSubsystemEOSCore::GetAddressFromString(const FString& InString)
{
	bool bUnused;
	TSharedPtr<FInternetAddrEOSCore> NewAddress = StaticCastSharedRef<FInternetAddrEOSCore>(CreateInternetAddr());
	NewAddress->SetIp(*InString, bUnused);
	return NewAddress;
}

bool FSocketSubsystemEOSCore::HasNetworkDevice()
{
	return true;
}

const TCHAR* FSocketSubsystemEOSCore::GetSocketAPIName() const
{
	return TEXT("p2pSocketsEOS");
}

ESocketErrors FSocketSubsystemEOSCore::GetLastErrorCode()
{
	return TranslateErrorCode(m_LastSocketError);
}

ESocketErrors FSocketSubsystemEOSCore::TranslateErrorCode(int32 Code)
{
	return static_cast<ESocketErrors>(Code);
}

bool FSocketSubsystemEOSCore::GetLocalAdapterAddresses(TArray<TSharedPtr<FInternetAddr>>& OutAddresses)
{
	TSharedRef<FInternetAddr> AdapterAddress = GetLocalBindAddr(nullptr, *GLog);
	OutAddresses.Add(AdapterAddress);
	return true;
}

TArray<TSharedRef<FInternetAddr>> FSocketSubsystemEOSCore::GetLocalBindAddresses()
{
	TArray<TSharedRef<FInternetAddr>> OutAddresses;
	OutAddresses.Add(GetLocalBindAddr(nullptr, *GLog));
	return OutAddresses;
}

TSharedRef<FInternetAddr> FSocketSubsystemEOSCore::GetLocalBindAddr(FOutputDevice& Out)
{
	return GetLocalBindAddr(nullptr, Out);
}

EOS_HP2P FSocketSubsystemEOSCore::GetP2PHandle()
{
	check(m_Subsystem != nullptr);
	return m_Subsystem->m_P2PHandle;
}

EOS_ProductUserId FSocketSubsystemEOSCore::GetLocalUserId()
{
	if (m_Subsystem != nullptr)
	{
		return m_Subsystem->m_IdentityInterfacePtr->GetLocalProductUserId();
	}
	return nullptr;
}

TSharedRef<FInternetAddr> FSocketSubsystemEOSCore::GetLocalBindAddr(const UWorld* const OwningWorld, FOutputDevice& Out)
{
	TSharedRef<FInternetAddrEOSCore> BoundAddr = MakeShared<FInternetAddrEOSCore>();

	EOS_ProductUserId LocalUserId = GetLocalUserId();
	if (LocalUserId == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No local user to send p2p packets with"));
		return BoundAddr;
	}
	BoundAddr->SetLocalUserId(LocalUserId);

	FString SessionId;
	if (const IOnlineSubsystem* const DefaultSubsystem = Online::GetSubsystem(OwningWorld))
	{
		const IOnlineSessionPtr DefaultSessionInt = DefaultSubsystem->GetSessionInterface();
		if (DefaultSessionInt.IsValid())
		{
			if (const FNamedOnlineSession* const NamedSession = DefaultSessionInt->GetNamedSession(NAME_GameSession))
			{
				SessionId = NamedSession->GetSessionIdStr();
			}
		}
	}

	if (SessionId.IsEmpty())
	{
		SessionId = FName(NAME_GameSession).ToString();
	}

	BoundAddr->SetSocketName(SessionId);

	return BoundAddr;
}

bool FSocketSubsystemEOSCore::IsSocketWaitSupported() const
{
	return false;
}

void FSocketSubsystemEOSCore::SetLastSocketError(const ESocketErrors NewSocketError)
{
	m_LastSocketError = NewSocketError;
}

bool FSocketSubsystemEOSCore::BindChannel(const FInternetAddrEOSCore& Address)
{
	if (!Address.IsValid())
	{
		SetLastSocketError(ESocketErrors::SE_EINVAL);
		return false;
	}

	const uint8 Channel = Address.GetChannel();

	FChannelSet& ExistingBoundPorts = m_BoundAddresses.FindOrAdd(Address.GetSocketName());
	if (ExistingBoundPorts.Contains(Channel))
	{
		SetLastSocketError(ESocketErrors::SE_EADDRINUSE);
		return false;
	}

	ExistingBoundPorts.Add(Channel);
	return true;
}

bool FSocketSubsystemEOSCore::UnbindChannel(const FInternetAddrEOSCore& Address)
{
	if (!Address.IsValid())
	{
		SetLastSocketError(ESocketErrors::SE_EINVAL);
		return false;
	}

	const FString SocketName = Address.GetSocketName();
	const uint8 Channel = Address.GetChannel();

	FChannelSet* ExistingBoundPorts = m_BoundAddresses.Find(SocketName);
	if (!ExistingBoundPorts)
	{
		SetLastSocketError(ESocketErrors::SE_ENOTSOCK);
		return false;
	}

	if (ExistingBoundPorts->Remove(Channel) == 0)
	{
		SetLastSocketError(ESocketErrors::SE_ENOTSOCK);
		return false;
	}

	if (ExistingBoundPorts->Num() == 0)
	{
		m_BoundAddresses.Remove(SocketName);
		ExistingBoundPorts = nullptr;
	}

	return true;
}

void DestroyEOSSocketSubsystem()
{
	FModuleManager& ModuleManager = FModuleManager::Get();

	if (ModuleManager.IsModuleLoaded("Sockets"))
	{
		FSocketSubsystemModule& SSS = FModuleManager::GetModuleChecked<FSocketSubsystemModule>("Sockets");
		SSS.UnregisterSocketSubsystem(EOSCORE_SUBSYSTEM);
	}
}
