/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "CoreMinimal.h"
#include "OnlineSubsystemTypes.h"

#define EOSCORE_ID_SEPARATOR TEXT("|")
#define EMPTY_EASID TEXT("00000000000000000000000000000000")
#define EMPTY_PUID TEXT("00000000000000000000000000000000")
#define EOS_ID_BYTE_SIZE (16 * 2)

class FUniqueNetIdEOSCore :
	public FUniqueNetIdString
{
public:
	FUniqueNetIdEOSCore()
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
		: FUniqueNetIdString(EMPTY_EASID EOSCORE_ID_SEPARATOR EMPTY_PUID)
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
	{
	}

	explicit FUniqueNetIdEOSCore(const FString& InUniqueNetId)
		: FUniqueNetIdString(InUniqueNetId)
	{
		ParseAccountIds();
	}

	explicit FUniqueNetIdEOSCore(FString&& InUniqueNetId)
		: FUniqueNetIdString(MoveTemp(InUniqueNetId))
	{
		ParseAccountIds();
	}

	explicit FUniqueNetIdEOSCore(const FUniqueNetId& Src)
		: FUniqueNetIdString(Src)
	{
		ParseAccountIds();
	}

	explicit FUniqueNetIdEOSCore(uint8* Bytes, int32 Size)
		: FUniqueNetIdString()
	{
		check(Size == EOS_ID_BYTE_SIZE);
		m_EpicAccountIdStr = BytesToHex(Bytes, 16);
		m_ProductUserIdStr = BytesToHex(Bytes + 16, 16);
		UniqueNetIdStr = m_EpicAccountIdStr + EOSCORE_ID_SEPARATOR + m_ProductUserIdStr;
	}

#if UE_VERSION_OLDER_THAN(5,2,0)
	friend uint32 GetTypeHash(const FUniqueNetIdEOSCore& A)
	{
		return ::GetTypeHash(A.UniqueNetIdStr);
	}
#else
	friend uint32 GetTypeHash(const FUniqueNetIdEOSCore& A)
	{
		return A.GetTypeHash();
	}
#endif

	/** global static instance of invalid (zero) id */
	static const TSharedRef<const FUniqueNetId>& EmptyId()
	{
		static const TSharedRef<const FUniqueNetId> EmptyId(MakeShared<FUniqueNetIdEOSCore>());
		return EmptyId;
	}

	virtual FName GetType() const override
	{
		static FName NAME_Eos(TEXT("EOSCore"));
		return NAME_Eos;
	}

	virtual const uint8* GetBytes() const override
	{
		return m_RawBytes;
	}

	virtual int32 GetSize() const override
	{
		return EOS_ID_BYTE_SIZE;
	}

PACKAGE_SCOPE:
	void UpdateNetIdStr(const FString& InNetIdStr)
	{
		UniqueNetIdStr = InNetIdStr;
		ParseAccountIds();
	}

	void ParseAccountIds()
	{
		TArray<FString> AccountIds;
		UniqueNetIdStr.ParseIntoArray(AccountIds, EOSCORE_ID_SEPARATOR, false);
		if (AccountIds.Num() > 0)
		{
			m_EpicAccountIdStr = AccountIds[0];
		}
		else
		{
			m_EpicAccountIdStr = EMPTY_EASID;
		}
		AddToBuffer(m_RawBytes, m_EpicAccountIdStr);
		if (AccountIds.Num() > 1)
		{
			m_ProductUserIdStr = AccountIds[1];
		}
		else
		{
			m_ProductUserIdStr = EMPTY_PUID;
		}
		AddToBuffer(m_RawBytes + 16, m_ProductUserIdStr);
	}

	void AddToBuffer(uint8* Buffer, const FString& Source)
	{
		check(Source.Len() == 32);
		for (int32 ReadOffset = 0, WriteOffset = 0; ReadOffset < 32; ReadOffset += 2, WriteOffset++)
		{
			FString HexStr = Source.Mid(ReadOffset, 2);
			// String is in HEX so use the version that takes a base
			uint8 ToByte = (uint8)FCString::Strtoi(*HexStr, nullptr, 16);
			Buffer[WriteOffset] = ToByte;
		}
	}

	FString m_EpicAccountIdStr;
	FString m_ProductUserIdStr;
	uint8 m_RawBytes[EOS_ID_BYTE_SIZE];
};