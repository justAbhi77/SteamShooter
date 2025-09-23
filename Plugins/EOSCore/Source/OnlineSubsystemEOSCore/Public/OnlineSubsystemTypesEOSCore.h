/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemTypes.h"
#include "OnlineSubsystemPackageEOSCore.h"
#include "eos_sessions_types.h"
#include <IPAddress.h>
#include "UniqueNetIdEOSCore.h"

#define EOSCORE_CONNECTION_URL_PREFIX TEXT("EOS")
#define EOSCORE_URL_SEPARATOR TEXT(":")
#define EOSCORE_OSS_STRING_BUFFER_LENGTH 256

#ifndef AUTH_ATTR_REFRESH_TOKEN
#define AUTH_ATTR_REFRESH_TOKEN TEXT("refresh_token")
#endif
#ifndef AUTH_ATTR_ID_TOKEN
#define AUTH_ATTR_ID_TOKEN TEXT("id_token")
#endif
#ifndef AUTH_ATTR_AUTHORIZATION_CODE
#define AUTH_ATTR_AUTHORIZATION_CODE TEXT("authorization_code")
#endif

#define USER_ATTR_DISPLAY_NAME TEXT("display_name")
#define USER_ATTR_COUNTRY TEXT("country")
#define USER_ATTR_LANG TEXT("language")

class FOnlineSubsystemEOSCore;

typedef TSharedRef<const FUniqueNetId> FUniqueNetIdRef;
typedef TSharedPtr<FUniqueNetIdEOSCore> FUniqueNetIdEOSCorePtr;
typedef TSharedRef<FUniqueNetIdEOSCore> FUniqueNetIdEOSCoreRef;

class IAttributeAccessInterface
{
public:
	virtual void SetInternalAttribute(const FString& AttrName, const FString& AttrValue) {}
	virtual FUniqueNetIdEOSCorePtr GetUniqueNetIdEOS() const { return FUniqueNetIdEOSCorePtr(); }
};

typedef TSharedPtr<IAttributeAccessInterface> IAttributeAccessInterfacePtr;
typedef TSharedRef<IAttributeAccessInterface> IAttributeAccessInterfaceRef;

template<class BaseClass, class AttributeAccessClass>
class TOnlineUserEOS : public BaseClass, public AttributeAccessClass
{
public:
	TOnlineUserEOS(FUniqueNetIdEOSCoreRef InNetIdRef) 
		: m_UserIdRef(InNetIdRef) {}
	virtual ~TOnlineUserEOS() {}

	// FOnlineUser
public:
	virtual TSharedRef<const FUniqueNetId> GetUserId() const override
	{
		return m_UserIdRef;
	}

	virtual FString GetRealName() const override
	{
		return FString();
	}

	virtual FString GetDisplayName(const FString& Platform = FString()) const override
	{
		FString ReturnValue;
		GetUserAttribute(USER_ATTR_DISPLAY_NAME, ReturnValue);
		return ReturnValue;
	}

	virtual bool GetUserAttribute(const FString& AttrName, FString& OutAttrValue) const override
	{
		const FString* FoundAttr = m_UserAttributes.Find(AttrName);
		if (FoundAttr != nullptr)
		{
			OutAttrValue = *FoundAttr;
			return true;
		}
		return false;
	}
//~FOnlineUser
public:
	virtual void SetInternalAttribute(const FString& AttrName, const FString& AttrValue)
	{
		m_UserAttributes.Add(AttrName, AttrValue);
	}

	virtual FUniqueNetIdEOSCorePtr GetUniqueNetIdEOS() const
	{
		return m_UserIdRef;
	}

protected:
	FUniqueNetIdEOSCoreRef m_UserIdRef;
	TMap<FString, FString> m_UserAttributes;
};

template<class BaseClass>
class TUserOnlineAccountEOS : public TOnlineUserEOS<BaseClass, IAttributeAccessInterface>
{
public:
	TUserOnlineAccountEOS(FUniqueNetIdEOSCoreRef InNetIdRef)
		: TOnlineUserEOS<BaseClass, IAttributeAccessInterface>(InNetIdRef)
	{}

// FUserOnlineAccount
	virtual FString GetAccessToken() const override
	{
		FString Token;
		GetAuthAttribute(AUTH_ATTR_ID_TOKEN, Token);
		return Token;
	}

	virtual bool GetAuthAttribute(const FString& AttrName, FString& OutAttrValue) const override
	{
		const FString* FoundAttr = m_AdditionalAuthData.Find(AttrName);
		if (FoundAttr != nullptr)
		{
			OutAttrValue = *FoundAttr;
			return true;
		}
		return false;
	}

	virtual bool SetUserAttribute(const FString& AttrName, const FString& AttrValue) override
	{
		const FString* FoundAttr = this->m_UserAttributes.Find(AttrName);
		if (FoundAttr == nullptr || *FoundAttr != AttrValue)
		{
			this->m_UserAttributes.Add(AttrName, AttrValue);
			return true;
		}
		return false;
	}
//~FUserOnlineAccount

	void SetAuthAttribute(const FString& AttrName, const FString& AttrValue)
	{
		m_AdditionalAuthData.Add(AttrName, AttrValue);
	}

protected:
	TMap<FString, FString> m_AdditionalAuthData;
};


static inline FString MakeStringFromProductUserId(EOS_ProductUserId UserId)
{
	FString StringId;

	char ProductIdString[EOS_PRODUCTUSERID_MAX_LENGTH + 1];
	ProductIdString[0] = '\0';
	int32_t BufferSize = EOS_PRODUCTUSERID_MAX_LENGTH + 1;
	EOS_EResult Result = EOS_ProductUserId_ToString(UserId, ProductIdString, &BufferSize);
	ensure(Result == EOS_EResult::EOS_Success);
	StringId += ProductIdString;

	return StringId;
}

static inline FString MakeStringFromEpicAccountId(EOS_EpicAccountId AccountId)
{
	FString StringId;

	char AccountIdString[EOS_EPICACCOUNTID_MAX_LENGTH + 1];
	AccountIdString[0] = '\0';
	int32_t BufferSize = EOS_EPICACCOUNTID_MAX_LENGTH + 1;
	EOS_EResult Result = EOS_EpicAccountId_ToString(AccountId, AccountIdString, &BufferSize);
	ensure(Result == EOS_EResult::EOS_Success);
	StringId += AccountIdString;

	return StringId;
}

static inline FString MakeNetIdStringFromIds(EOS_EpicAccountId AccountId, EOS_ProductUserId UserId)
{
	FString NetId;

	if (EOS_EpicAccountId_IsValid(AccountId) == EOS_TRUE)
	{
		NetId = MakeStringFromEpicAccountId(AccountId);
	}
	else
	{
		NetId = EMPTY_EASID;
	}
	if (EOS_ProductUserId_IsValid(UserId) == EOS_TRUE)
	{
		NetId += EOSCORE_ID_SEPARATOR + MakeStringFromProductUserId(UserId);
	}

	return NetId;
}

class FCallbackBase
{
public:
	virtual ~FCallbackBase() {}
};

template<typename CallbackFuncType, typename CallbackType>
class TEOSCallback : public FCallbackBase
{
public:
	TFunction<void(const CallbackType*)> m_CallbackLambda;
	TEOSCallback() {}
	virtual ~TEOSCallback() = default;

	CallbackFuncType GetCallbackPtr()
	{
		return &CallbackImpl;
	}

private:
	static void EOS_CALL CallbackImpl(const CallbackType* Data)
	{
		if (EOS_EResult_IsOperationComplete(Data->ResultCode) == EOS_FALSE)
		{
			return;
		}
		check(IsInGameThread());

		TEOSCallback* CallbackThis = (TEOSCallback*)Data->ClientData;
		check(CallbackThis);

		check(CallbackThis->m_CallbackLambda);
		CallbackThis->m_CallbackLambda(Data);

		delete CallbackThis;
	}
};

template<typename CallbackFuncType, typename CallbackType>
class TEOSGlobalCallback : public FCallbackBase
{
public:
	TEOSGlobalCallback() = default;
	virtual ~TEOSGlobalCallback() = default;
public:
	TFunction<void(const CallbackType*)> m_CallbackLambda;
public:
	CallbackFuncType GetCallbackPtr()
	{
		return &CallbackImpl;
	}
private:
	static void EOS_CALL CallbackImpl(const CallbackType* Data)
	{
		check(IsInGameThread());

		TEOSGlobalCallback* CallbackThis = (TEOSGlobalCallback*)Data->ClientData;
		check(CallbackThis);

		check(CallbackThis->m_CallbackLambda);
		CallbackThis->m_CallbackLambda(Data);
	}
};

class FOnlineSessionInfoEOSCore : public FOnlineSessionInfo
{
protected:
	FOnlineSessionInfoEOSCore& operator=(const FOnlineSessionInfoEOSCore& Src) { return *this; }
PACKAGE_SCOPE:
	FOnlineSessionInfoEOSCore();
	FOnlineSessionInfoEOSCore(const FOnlineSessionInfoEOSCore& Src)
		: FOnlineSessionInfo(Src)
		, m_HostAddr(Src.m_HostAddr)
		, m_SessionId(Src.m_SessionId)
		, m_SessionHandle(Src.m_SessionHandle)
		, bIsFromClone(true)
	{}
	FOnlineSessionInfoEOSCore(const FString& InHostIp, const FString& InSessionId, EOS_HSessionDetails InSessionHandle);

PACKAGE_SCOPE:
	void InitLAN(const FOnlineSubsystemEOSCore* Subsystem);
PACKAGE_SCOPE:
	FString m_EOSAddress;
	TSharedPtr<class FInternetAddr> m_HostAddr;
#if UE_VERSION_NEWER_THAN(5,0,3)
	FUniqueNetIdStringRef m_SessionId;
#else
	FUniqueNetIdString m_SessionId;
#endif
	EOS_HSessionDetails m_SessionHandle;
	bool bIsFromClone;
public:
	virtual ~FOnlineSessionInfoEOSCore() override;
	bool operator==(const FOnlineSessionInfoEOSCore& Other) const
	{
		return false;
	}
	virtual const uint8* GetBytes() const override
	{
		return nullptr;
	}
	virtual int32 GetSize() const override
	{
		return sizeof(uint64) + sizeof(TSharedPtr<class FInternetAddr>);
	}
	virtual bool IsValid() const override
	{
		// LAN
		return m_HostAddr.IsValid() && m_HostAddr->IsValid();
	}
#if UE_VERSION_NEWER_THAN(5,0,3)
	virtual FString ToString() const override
	{
		return m_SessionId->ToString();
	}

	virtual FString ToDebugString() const override
	{
		return FString::Printf(TEXT("HostIP: %s SessionId: %s"),
			m_HostAddr.IsValid() ? *m_HostAddr->ToString(true) : TEXT("INVALID"),
			*m_SessionId->ToDebugString());
	}
	virtual const FUniqueNetId& GetSessionId() const override
	{
		return *m_SessionId;
	}
#else
	virtual FString ToString() const override
	{
		return m_SessionId.ToString();
	}

	virtual FString ToDebugString() const override
	{
		return FString::Printf(TEXT("HostIP: %s SessionId: %s"),
			m_HostAddr.IsValid() ? *m_HostAddr->ToString(true) : TEXT("INVALID"),
			*m_SessionId.ToDebugString());
	}
	virtual const FUniqueNetId& GetSessionId() const override
	{
		return m_SessionId;
	}
#endif
};