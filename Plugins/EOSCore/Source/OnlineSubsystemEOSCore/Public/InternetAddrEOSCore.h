/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "IPAddress.h"
#include "eos_common.h"

#define EOSCORE_SOCKET_NAME_SIZE 33

class FUniqueNetId;

class FInternetAddrEOSCore : public FInternetAddr
{
public:
	FInternetAddrEOSCore();
	FInternetAddrEOSCore(const FString& InRemoteUserId, const FString& InSocketName, const int32 InChannel);
	FInternetAddrEOSCore(const EOS_ProductUserId InRemoteUserId, const FString& InSocketName, const int32 InChannel);
	virtual ~FInternetAddrEOSCore() = default;

	//~ Begin FInternetAddr Interface
	virtual void SetIp(uint32 InAddr) override;
	virtual void SetIp(const TCHAR* InAddr, bool& bIsValid) override;
	virtual void GetIp(uint32& OutAddr) const override;
	virtual void SetPort(int32 InPort) override;
	virtual int32 GetPort() const override;
	virtual void SetRawIp(const TArray<uint8>& RawAddr) override;
	virtual TArray<uint8> GetRawIp() const override;
	virtual void SetAnyAddress() override;
	virtual void SetBroadcastAddress() override;
	virtual void SetLoopbackAddress() override;
	virtual FString ToString(bool bAppendPort) const override;
	virtual uint32 GetTypeHash() const override;
	virtual bool IsValid() const override;
	virtual TSharedRef<FInternetAddr> Clone() const override;
	//~ End FInternetAddr Interface

	FORCEINLINE FInternetAddrEOSCore& operator=(const FInternetAddrEOSCore& Other)
	{
		LocalUserId = Other.LocalUserId;
		RemoteUserId = Other.RemoteUserId;
		FCStringAnsi::Strcpy(SocketName, Other.SocketName);
		Channel = Other.Channel;
		return *this;
	}

	FORCEINLINE friend bool operator==(const FInternetAddrEOSCore& A, const FInternetAddrEOSCore& B)
	{
		return (A.Channel == B.Channel && A.LocalUserId == B.LocalUserId && A.RemoteUserId == B.RemoteUserId &&
			FCStringAnsi::Stricmp(A.GetSocketName(), B.GetSocketName()));
	}

	FORCEINLINE friend bool operator!=(const FInternetAddrEOSCore& A, const FInternetAddrEOSCore& B)
	{
		return !(A == B);
	}

	FORCEINLINE friend uint32 GetTypeHash(const FInternetAddrEOSCore& Address)
	{
		return Address.GetTypeHash();
	}

	friend bool operator<(const FInternetAddrEOSCore& Left, const FInternetAddrEOSCore& Right)
	{
		if (Left.GetLocalUserId() < Right.GetLocalUserId())
		{
			return true;
		}
		else if (Left.GetRemoteUserId() < Right.GetRemoteUserId())
		{
			return true;
		}
		else if (FCStringAnsi::Stricmp(Left.GetSocketName(), Right.GetSocketName()) < 0)
		{
			return true;
		}

		return Left.GetChannel() < Right.GetChannel();
	}

	EOS_ProductUserId GetLocalUserId() const
	{
		return LocalUserId;
	}

	void SetLocalUserId(EOS_ProductUserId InLocalUserId)
	{
		LocalUserId = InLocalUserId;
	}

	void SetRemoteUserId(EOS_ProductUserId InRemoteUserId)
	{
		RemoteUserId = InRemoteUserId;
	}

	EOS_ProductUserId GetRemoteUserId() const
	{
		return RemoteUserId;
	}

	const char* GetSocketName() const
	{
		return SocketName;
	}

	void SetSocketName(const FString& InSocketName)
	{
		FCStringAnsi::Strncpy(SocketName, TCHAR_TO_UTF8(*InSocketName), EOSCORE_SOCKET_NAME_SIZE);
	}

	void SetSocketName(const char* InSocketName)
	{
		FCStringAnsi::Strncpy(SocketName, InSocketName, EOSCORE_SOCKET_NAME_SIZE);
	}

	uint8 GetChannel() const
	{
		return Channel;
	}

	void SetChannel(uint8 InChannel)
	{
		Channel = InChannel;
	}

private:
	EOS_ProductUserId LocalUserId;
	EOS_ProductUserId RemoteUserId;

	char SocketName[EOSCORE_SOCKET_NAME_SIZE];
	uint8 Channel;

	friend class SocketSubsystemEOS;
};
