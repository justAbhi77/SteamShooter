/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemTypesEOSCore.h"
#include <Misc/EngineVersionComparison.h>
#if UE_VERSION_OLDER_THAN(5,1,0)
#include <NboSerializer.h>
#else
#include <Online/NboSerializer.h>
#endif

#if UE_VERSION_OLDER_THAN(5,1,0)
class FNboSerializeToBufferEOSCore : public FNboSerializeToBuffer
{
public:
	FNboSerializeToBufferEOSCore() :
		FNboSerializeToBuffer(512)
	{
	}

	FNboSerializeToBufferEOSCore(uint32 Size) :
		FNboSerializeToBuffer(Size)
	{
	}

	friend inline FNboSerializeToBufferEOSCore& operator<<(FNboSerializeToBufferEOSCore& Ar, const FOnlineSessionInfoEOSCore& SessionInfo)
	{
		check(SessionInfo.m_HostAddr.IsValid());
		Ar << SessionInfo.m_SessionId;
		Ar << *SessionInfo.m_HostAddr;
		return Ar;
	}

	friend inline FNboSerializeToBufferEOSCore& operator<<(FNboSerializeToBufferEOSCore& Ar, const FUniqueNetIdEOSCore& UniqueId)
	{
		Ar << UniqueId.UniqueNetIdStr;
		return Ar;
	}

	friend inline FNboSerializeToBufferEOSCore& operator<<(FNboSerializeToBufferEOSCore& Ar, const FUniqueNetIdString& UniqueId)
	{
		Ar << UniqueId.UniqueNetIdStr;
		return Ar;
	}
};

class FNboSerializeFromBufferEOSCore : public FNboSerializeFromBuffer
{
public:
	FNboSerializeFromBufferEOSCore(uint8* Packet, int32 Length)
		: FNboSerializeFromBuffer(Packet, Length)
	{
	}

	friend inline FNboSerializeFromBufferEOSCore& operator>>(FNboSerializeFromBufferEOSCore& Ar, FOnlineSessionInfoEOSCore& SessionInfo)
	{
		check(SessionInfo.m_HostAddr.IsValid());
		Ar >> SessionInfo.m_SessionId;
		Ar >> *SessionInfo.m_HostAddr;
		return Ar;
	}

	friend inline FNboSerializeFromBufferEOSCore& operator>>(FNboSerializeFromBufferEOSCore& Ar, FUniqueNetIdEOSCore& UniqueId)
	{
		Ar >> UniqueId.UniqueNetIdStr;
		return Ar;
	}

	friend inline FNboSerializeFromBufferEOSCore& operator>>(FNboSerializeFromBufferEOSCore& Ar, FUniqueNetIdString& UniqueId)
	{
		Ar >> UniqueId.UniqueNetIdStr;
		return Ar;
	}
};
#else
#include "NboSerializerOSS.h"
class FNboSerializeToBufferEOSCore : public FNboSerializeToBufferOSS
{
public:
	FNboSerializeToBufferEOSCore() :
		FNboSerializeToBufferOSS(512)
	{
	}

	FNboSerializeToBufferEOSCore(uint32 Size) :
		FNboSerializeToBufferOSS(Size)
	{
	}

	friend inline FNboSerializeToBufferEOSCore& operator<<(FNboSerializeToBufferEOSCore& Ar, const FOnlineSessionInfoEOSCore& SessionInfo)
	{
		check(SessionInfo.m_HostAddr.IsValid());
		Ar << *SessionInfo.m_SessionId;
		((FNboSerializeToBuffer&)Ar) << *SessionInfo.m_HostAddr;
		return Ar;
	}

	friend inline FNboSerializeToBufferEOSCore& operator<<(FNboSerializeToBufferEOSCore& Ar, const FUniqueNetIdString& UniqueId)
	{
		((FNboSerializeToBuffer&)Ar) << UniqueId.UniqueNetIdStr;
		return Ar;
	}
};

class FNboSerializeFromBufferEOSCore : public FNboSerializeFromBufferOSS
{
public:
	FNboSerializeFromBufferEOSCore(uint8* Packet, int32 Length)
		: FNboSerializeFromBufferOSS(Packet, Length)
	{
	}

	friend inline FNboSerializeFromBufferEOSCore& operator>>(FNboSerializeFromBufferEOSCore& Ar, FOnlineSessionInfoEOSCore& SessionInfo)
	{
		check(SessionInfo.m_HostAddr.IsValid());

		FString SessionId;
		Ar >> SessionId;
		Ar >> *SessionInfo.m_HostAddr;

		SessionInfo.m_SessionId = FUniqueNetIdString::Create(MoveTemp(SessionId), FName("EOS"));

		return Ar;
	}

	friend inline FNboSerializeFromBufferEOSCore& operator>>(FNboSerializeFromBufferEOSCore& Ar, FUniqueNetIdString& UniqueId)
	{
		Ar >> UniqueId.UniqueNetIdStr;
		return Ar;
	}
};

#endif




