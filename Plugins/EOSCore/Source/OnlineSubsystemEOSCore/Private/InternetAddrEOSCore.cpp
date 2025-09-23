/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "InternetAddrEOSCore.h"
#include "OnlineSubsystemTypesEOSCore.h"
#include "OnlineSubsystemModuleEOSCore.h"
#include "OnlineSubsystemEOSCorePrivatePCH.h"

inline uint8 PortToChannel(int32 InPort)
{
	return InPort > 255 ? InPort % 256 : FMath::Clamp(InPort, 0, 255);
}

FInternetAddrEOSCore::FInternetAddrEOSCore()
	: LocalUserId(nullptr)
	  , RemoteUserId(nullptr)
	  , Channel(0)
{
	SocketName[0] = '\0';
}

FInternetAddrEOSCore::FInternetAddrEOSCore(const FString& InRemoteUserId, const FString& InSocketName, const int32 InChannel)
	: LocalUserId(nullptr)
	  , RemoteUserId(nullptr)
{
	RemoteUserId = EOS_ProductUserId_FromString(TCHAR_TO_UTF8(*InRemoteUserId));
	FCStringAnsi::Strcpy(SocketName, TCHAR_TO_UTF8(*InSocketName));
	Channel = PortToChannel(InChannel);
}

FInternetAddrEOSCore::FInternetAddrEOSCore(const EOS_ProductUserId InRemoteUserId, const FString& InSocketName, const int32 InChannel)
	: LocalUserId(nullptr)
	  , RemoteUserId(InRemoteUserId)
	  , Channel(InChannel)
{
	FCStringAnsi::Strcpy(SocketName, TCHAR_TO_UTF8(*InSocketName));
	Channel = PortToChannel(InChannel);
}

void FInternetAddrEOSCore::SetIp(uint32)
{
	LogWarning("Calls to FInternetAddrEOSCore::SetIp are not valid");
}

void FInternetAddrEOSCore::SetIp(const TCHAR* InAddr, bool& bIsValid)
{
	bIsValid = false;

	if (InAddr == nullptr)
	{
		return;
	}

	Channel = 0;

	TArray<FString> UrlParts;
	FString FullAddress = InAddr;
	FullAddress.ParseIntoArray(UrlParts, EOSCORE_URL_SEPARATOR, false);

	if (UrlParts.Num() < 3 || UrlParts.Num() > 4)
	{
		return;
	}
	if (UrlParts[0] != EOSCORE_CONNECTION_URL_PREFIX)
	{
		return;
	}
	RemoteUserId = EOS_ProductUserId_FromString(TCHAR_TO_UTF8(*UrlParts[1]));
	if (EOS_ProductUserId_IsValid(RemoteUserId) == EOS_FALSE)
	{
		return;
	}
	if (UrlParts[2].Len() == 0)
	{
		return;
	}
	SetSocketName(UrlParts[2]);
	if (UrlParts.Num() == 4)
	{
		Channel = FCString::Atoi(*UrlParts[3]);
	}
	bIsValid = true;
}

void FInternetAddrEOSCore::GetIp(uint32& OutAddr) const
{
	OutAddr = 0u;

	LogWarning("Calls to FInternetAddrEOSCore::GetIp are not valid");
}

void FInternetAddrEOSCore::SetPort(int32 InPort)
{
	Channel = PortToChannel(InPort);
}

int32 FInternetAddrEOSCore::GetPort() const
{
	return Channel;
}

void FInternetAddrEOSCore::SetRawIp(const TArray<uint8>& RawAddr)
{
	const auto ConvertedTCHARData = StringCast<TCHAR>(reinterpret_cast<const ANSICHAR*>(RawAddr.GetData()), RawAddr.Num());
	const FString IpAsString(ConvertedTCHARData.Length(), ConvertedTCHARData.Get());

	bool bUnused;
	SetIp(*IpAsString, bUnused);
}

TArray<uint8> FInternetAddrEOSCore::GetRawIp() const
{
	const FString StringVersion = ToString(true);
	const auto ConvertedANSIData = StringCast<ANSICHAR>(*StringVersion, StringVersion.Len());

	TArray<uint8> OutData;
	for (int32 Index = 0; Index < ConvertedANSIData.Length(); ++Index)
	{
		OutData.Add(ConvertedANSIData.Get()[Index]);
	}

	return OutData;
}

void FInternetAddrEOSCore::SetAnyAddress()
{
}

void FInternetAddrEOSCore::SetBroadcastAddress()
{
}

void FInternetAddrEOSCore::SetLoopbackAddress()
{
}

FString FInternetAddrEOSCore::ToString(bool bAppendPort) const
{
	char PuidBuffer[64];
	int32 BufferLen = 64;
	if (EOS_ProductUserId_ToString(RemoteUserId, PuidBuffer, &BufferLen) != EOS_EResult::EOS_Success)
	{
		PuidBuffer[0] = '\0';
	}

	if (bAppendPort)
	{
		return FString::Printf(TEXT("%s%s%s%s%s%s%u"), EOSCORE_CONNECTION_URL_PREFIX, EOSCORE_URL_SEPARATOR, UTF8_TO_TCHAR(PuidBuffer), EOSCORE_URL_SEPARATOR, UTF8_TO_TCHAR(SocketName), EOSCORE_URL_SEPARATOR, Channel);
	}

	return FString::Printf(TEXT("%s%s%s%s%s"), EOSCORE_CONNECTION_URL_PREFIX, EOSCORE_URL_SEPARATOR, UTF8_TO_TCHAR(PuidBuffer), EOSCORE_URL_SEPARATOR, UTF8_TO_TCHAR(SocketName));
}

uint32 FInternetAddrEOSCore::GetTypeHash() const
{
#if UE_VERSION_OLDER_THAN(5,2,0)
	return ::GetTypeHash(ToString(true));
#else
	return HashCombine(HashCombine(HashCombine(GetTypeHashHelper((void*)LocalUserId), GetTypeHashHelper((void*)RemoteUserId)), GetTypeHashHelper(FAnsiStringView(SocketName, EOSCORE_SOCKET_NAME_SIZE))), Channel);
#endif
}

bool FInternetAddrEOSCore::IsValid() const
{
	return (EOS_ProductUserId_IsValid(LocalUserId) == EOS_TRUE || EOS_ProductUserId_IsValid(RemoteUserId) == EOS_TRUE) && FCStringAnsi::Strlen(SocketName) > 0;
}

TSharedRef<FInternetAddr> FInternetAddrEOSCore::Clone() const
{
	return MakeShared<FInternetAddrEOSCore>(*this);
}
