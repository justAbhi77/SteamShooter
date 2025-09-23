/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "OnlineSessionEOSCore.h"
#include "OnlineSubsystemEOSCore.h"
#include "OnlineSubsystemTypesEOSCore.h"
#include "OnlineIdentityEOSCore.h"
#include "NboSerializerEOSCore.h"
#include "InternetAddrEOSCore.h"
#include "OnlineSubsystemEOSCorePrivatePCH.h"
#include "OnlineSubsystemModuleEOSCore.h"

static FString MakeStringFromAttributeValue(const EOS_Sessions_AttributeData* Attribute)
{
	switch (Attribute->ValueType)
	{
	case EOS_ESessionAttributeType::EOS_SAT_Int64:
		{
			const int32 Value = Attribute->Value.AsInt64;
			return FString::Printf(TEXT("%d"), Value);
		}
	case EOS_ESessionAttributeType::EOS_SAT_Double:
		{
			const double Value = Attribute->Value.AsDouble;
			return FString::Printf(TEXT("%f"), Value);
		}
	case EOS_ESessionAttributeType::EOS_SAT_String:
		{
			const FString Value = Attribute->Value.AsUtf8;
			return FString::Printf(TEXT("%s"), *Value);
		}
	default: ;
	}
	return TEXT("");
}

static bool IsSessionSettingTypeSupported(EOnlineKeyValuePairDataType::Type InType)
{
	switch (InType)
	{
	case EOnlineKeyValuePairDataType::Int32:
	case EOnlineKeyValuePairDataType::UInt32:
	case EOnlineKeyValuePairDataType::Int64:
	case EOnlineKeyValuePairDataType::Double:
	case EOnlineKeyValuePairDataType::String:
	case EOnlineKeyValuePairDataType::Float:
	case EOnlineKeyValuePairDataType::Bool:
		{
			return true;
		}
	}
	return false;
}

static EOS_EOnlineComparisonOp ToEOSSearchOp(EOnlineComparisonOp::Type Op)
{
	switch (Op)
	{
	case EOnlineComparisonOp::Equals:
		{
			return EOS_EOnlineComparisonOp::EOS_OCO_EQUAL;
		}
	case EOnlineComparisonOp::NotEquals:
		{
			return EOS_EOnlineComparisonOp::EOS_OCO_NOTEQUAL;
		}
	case EOnlineComparisonOp::GreaterThan:
		{
			return EOS_EOnlineComparisonOp::EOS_OCO_GREATERTHAN;
		}
	case EOnlineComparisonOp::GreaterThanEquals:
		{
			return EOS_EOnlineComparisonOp::EOS_OCO_GREATERTHANOREQUAL;
		}
	case EOnlineComparisonOp::LessThan:
		{
			return EOS_EOnlineComparisonOp::EOS_OCO_LESSTHAN;
		}
	case EOnlineComparisonOp::LessThanEquals:
		{
			return EOS_EOnlineComparisonOp::EOS_OCO_LESSTHANOREQUAL;
		}
	case EOnlineComparisonOp::Near:
		{
			return EOS_EOnlineComparisonOp::EOS_OCO_DISTANCE;
		}
	case EOnlineComparisonOp::In:
		{
			return EOS_EOnlineComparisonOp::EOS_OCO_ANYOF;
		}
	case EOnlineComparisonOp::NotIn:
		{
			return EOS_EOnlineComparisonOp::EOS_OCO_NOTANYOF;
		}
	}
	return EOS_EOnlineComparisonOp::EOS_OCO_EQUAL;
}

struct FAttributeOptions :
	public EOS_Sessions_AttributeData
{
	char KeyAnsi[EOSCORE_OSS_STRING_BUFFER_LENGTH + 1];
	char ValueAnsi[EOSCORE_OSS_STRING_BUFFER_LENGTH + 1];

	FAttributeOptions(const char* InKey, const char* InValue) :
		EOS_Sessions_AttributeData()
	{
		ApiVersion = EOS_SESSIONS_SESSIONATTRIBUTEDATA_API_LATEST;
		ValueType = EOS_ESessionAttributeType::EOS_SAT_String;
		Value.AsUtf8 = ValueAnsi;
		Key = KeyAnsi;
		FCStringAnsi::Strncpy(KeyAnsi, InKey, EOSCORE_OSS_STRING_BUFFER_LENGTH + 1);
		FCStringAnsi::Strncpy(ValueAnsi, InValue, EOSCORE_OSS_STRING_BUFFER_LENGTH + 1);
	}

	FAttributeOptions(const char* InKey, bool InValue) :
		EOS_Sessions_AttributeData()
	{
		ApiVersion = EOS_SESSIONS_SESSIONATTRIBUTEDATA_API_LATEST;
		ValueType = EOS_ESessionAttributeType::EOS_SAT_Boolean;
		Value.AsBool = InValue ? EOS_TRUE : EOS_FALSE;
		Key = KeyAnsi;
		FCStringAnsi::Strncpy(KeyAnsi, InKey, EOSCORE_OSS_STRING_BUFFER_LENGTH + 1);
	}

	FAttributeOptions(const char* InKey, float InValue) :
		EOS_Sessions_AttributeData()
	{
		ApiVersion = EOS_SESSIONS_SESSIONATTRIBUTEDATA_API_LATEST;
		ValueType = EOS_ESessionAttributeType::EOS_SAT_Double;
		Value.AsDouble = InValue;
		Key = KeyAnsi;
		FCStringAnsi::Strncpy(KeyAnsi, InKey, EOSCORE_OSS_STRING_BUFFER_LENGTH + 1);
	}

	FAttributeOptions(const char* InKey, int32 InValue) :
		EOS_Sessions_AttributeData()
	{
		ApiVersion = EOS_SESSIONS_SESSIONATTRIBUTEDATA_API_LATEST;
		ValueType = EOS_ESessionAttributeType::EOS_SAT_Int64;
		Value.AsInt64 = InValue;
		Key = KeyAnsi;
		FCStringAnsi::Strncpy(KeyAnsi, InKey, EOSCORE_OSS_STRING_BUFFER_LENGTH + 1);
	}

	FAttributeOptions(const char* InKey, const FVariantData& InValue) :
		EOS_Sessions_AttributeData()
	{
		ApiVersion = EOS_SESSIONS_SESSIONATTRIBUTEDATA_API_LATEST;

		switch (InValue.GetType())
		{
		case EOnlineKeyValuePairDataType::Int32:
			{
				ValueType = EOS_ESessionAttributeType::EOS_SAT_Int64;
				int32 RawValue = 0;
				InValue.GetValue(RawValue);
				Value.AsInt64 = RawValue;
				break;
			}
		case EOnlineKeyValuePairDataType::UInt32:
			{
				ValueType = EOS_ESessionAttributeType::EOS_SAT_Int64;
				uint32 RawValue = 0;
				InValue.GetValue(RawValue);
				Value.AsInt64 = RawValue;
				break;
			}
		case EOnlineKeyValuePairDataType::Int64:
			{
				ValueType = EOS_ESessionAttributeType::EOS_SAT_Int64;
				int64 RawValue = 0;
				InValue.GetValue(RawValue);
				Value.AsInt64 = RawValue;
				break;
			}
		case EOnlineKeyValuePairDataType::Bool:
			{
				ValueType = EOS_ESessionAttributeType::EOS_SAT_Boolean;
				bool RawValue = false;
				InValue.GetValue(RawValue);
				Value.AsBool = RawValue ? EOS_TRUE : EOS_FALSE;
				break;
			}
		case EOnlineKeyValuePairDataType::Double:
			{
				ValueType = EOS_ESessionAttributeType::EOS_SAT_Double;
				double RawValue = 0.0;
				InValue.GetValue(RawValue);
				Value.AsDouble = RawValue;
				break;
			}
		case EOnlineKeyValuePairDataType::Float:
			{
				ValueType = EOS_ESessionAttributeType::EOS_SAT_Double;
				float RawValue = 0.0;
				InValue.GetValue(RawValue);
				Value.AsDouble = RawValue;
				break;
			}
		case EOnlineKeyValuePairDataType::String:
			{
				ValueType = EOS_ESessionAttributeType::EOS_SAT_String;
				Value.AsUtf8 = ValueAnsi;
				Key = KeyAnsi;
				FString OutString;
				InValue.GetValue(OutString);
				FCStringAnsi::Strncpy(ValueAnsi, TCHAR_TO_UTF8(*OutString), EOSCORE_OSS_STRING_BUFFER_LENGTH + 1);
				break;
			}
		}
		Key = KeyAnsi;
		FCStringAnsi::Strncpy(KeyAnsi, InKey, EOSCORE_OSS_STRING_BUFFER_LENGTH + 1);
	}
};

FOnlineSessionInfoEOSCore::FOnlineSessionInfoEOSCore()
	: m_HostAddr(nullptr)
#if UE_VERSION_OLDER_THAN(5,1,0)
	  , m_SessionId(TEXT("INVALID"))
#else
	  , m_SessionId(FUniqueNetIdString::EmptyId())
#endif
	  , m_SessionHandle(nullptr)
	  , bIsFromClone(false)
{
}

FOnlineSessionInfoEOSCore::FOnlineSessionInfoEOSCore(const FString& InHostIp, const FString& InSessionId, EOS_HSessionDetails InSessionHandle)
	: FOnlineSessionInfo()
#if UE_VERSION_OLDER_THAN(5,1,0)
	  , m_SessionId(InSessionId)
#else
	  , m_SessionId(FUniqueNetIdString::Create(InSessionId, FName("EOS")))
#endif
	  , m_SessionHandle(InSessionHandle)
	  , bIsFromClone(false)
{
	if (InHostIp.StartsWith(EOSCORE_CONNECTION_URL_PREFIX, ESearchCase::IgnoreCase))
	{
		m_HostAddr = ISocketSubsystem::Get(EOSCORE_SUBSYSTEM)->GetAddressFromString(InHostIp);
		m_EOSAddress = InHostIp;
	}
	else
	{
		m_HostAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetAddressFromString(InHostIp);
		m_HostAddr->SetPort(FURL::UrlConfig.DefaultPort);
	}
}

FOnlineSessionInfoEOSCore::~FOnlineSessionInfoEOSCore()
{
	if (m_SessionHandle != nullptr && !bIsFromClone)
	{
		EOS_SessionDetails_Release(m_SessionHandle);
	}
}

void FOnlineSessionInfoEOSCore::InitLAN(const FOnlineSubsystemEOSCore* Subsystem)
{
	bool bCanBindAll;
	m_HostAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBindAll);

	uint32 HostIp = 0;
	m_HostAddr->GetIp(HostIp);

	if ((HostIp & 0xff000000) == 0x7f000000)
	{
		m_HostAddr->SetIp(0x7f000001);
	}

	m_HostAddr->SetPort(GetPortFromNetDriver(Subsystem->GetInstanceName()));

	FGuid OwnerGuid;
	FPlatformMisc::CreateGuid(OwnerGuid);
#if UE_VERSION_OLDER_THAN(5,1,0)
	m_SessionId = static_cast<FUniqueNetIdString>(FUniqueNetIdEOSCore(OwnerGuid.ToString()));
#else
	m_SessionId = FUniqueNetIdString::Create(OwnerGuid.ToString(), FName("EOS"));
#endif
}

typedef TEOSGlobalCallback<EOS_Sessions_OnSessionInviteReceivedCallback, EOS_Sessions_SessionInviteReceivedCallbackInfo> FSessionInviteReceivedCallback;
typedef TEOSGlobalCallback<EOS_Sessions_OnSessionInviteAcceptedCallback, EOS_Sessions_SessionInviteAcceptedCallbackInfo> FSessionInviteAcceptedCallback;


void FOnlineSessionEOSCore::Init(const char* InBucketId)
{
	FCStringAnsi::Strncpy(s_BucketId, InBucketId, EOSCORE_OSS_STRING_BUFFER_LENGTH + 1);

	bIsDedicatedServer = IsRunningDedicatedServer();

	bIsUsingP2PSockets = false;
	GConfig->GetBool(TEXT("/Script/OnlineSubsystemEOSCore.NetDriverEOSCore"), TEXT("bIsUsingP2PSockets"), bIsUsingP2PSockets, GEngineIni);

	/* Listen for Received Session Invites */
	{
		FSessionInviteReceivedCallback* CallbackObj = new FSessionInviteReceivedCallback();
		m_SessionInviteReceivedCallback = CallbackObj;
		CallbackObj->m_CallbackLambda = [this](const EOS_Sessions_SessionInviteReceivedCallbackInfo* data)
		{
			UE_LOG_ONLINE_SESSION(Log, TEXT("Invite received"));

			EOS_HSessionDetails m_SessionDetailsHandle = NULL;
			const EOS_Sessions_CopySessionHandleByInviteIdOptions SessionHandleByInviteIdOptions = {EOS_SESSIONS_COPYSESSIONHANDLEBYINVITEID_API_LATEST, data->InviteId};

			const EOS_EResult CopySessionHandleResult = EOS_Sessions_CopySessionHandleByInviteId(m_Subsystem->m_SessionsHandle, &SessionHandleByInviteIdOptions, &m_SessionDetailsHandle);

			if (CopySessionHandleResult == EOS_EResult::EOS_Success)
			{
				EOS_SessionDetails_Info* SessionInfo = new EOS_SessionDetails_Info;
				constexpr EOS_SessionDetails_CopyInfoOptions CopyInfoOptions = {EOS_SESSIONDETAILS_COPYINFO_API_LATEST};

				const EOS_EResult CopySessionDetailsResult = EOS_SessionDetails_CopyInfo(m_SessionDetailsHandle, &CopyInfoOptions, &SessionInfo);

				char PuidBuffer[64];
				int32 BufferLen = 64;
				EOS_ProductUserId_ToString(data->TargetUserId, PuidBuffer, &BufferLen);
				const FString RemoteUserIdStr(PuidBuffer);

				const TSharedPtr<FUniqueNetIdEOSCore> NetId = MakeShareable(new FUniqueNetIdEOSCore(RemoteUserIdStr));

				if (CopySessionDetailsResult == EOS_EResult::EOS_Success)
				{
					FOnlineSessionSearchResult SearchResult;
					SearchResult.PingInMs = -1;
					SearchResult.Session.SessionInfo = MakeShareable(new FOnlineSessionInfoEOSCore(SessionInfo->HostAddress, SessionInfo->SessionId, m_SessionDetailsHandle));

					CopyAttributes(m_SessionDetailsHandle, SearchResult.Session);
					TriggerOnSessionInviteReceivedDelegates(*m_Subsystem->GetIdentityInterface()->GetUniquePlayerId(0), *NetId, data->InviteId, SearchResult);
				}
				else
				{
					UE_LOG_ONLINE_SESSION(Error, TEXT("EOS_SessionDetails_CopyInfo() failed with EOS result code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(CopySessionDetailsResult)));
				}
			}
			else
			{
				UE_LOG_ONLINE_SESSION(Error, TEXT("EOS_Sessions_CopySessionHandleByInviteId() failed with EOS result code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(CopySessionHandleResult)));
			}
		};

		EOS_Sessions_AddNotifySessionInviteReceivedOptions Options = {};
		Options.ApiVersion = EOS_SESSIONS_ADDNOTIFYSESSIONINVITERECEIVED_API_LATEST;
		m_SessionInviteReceivedId = EOS_Sessions_AddNotifySessionInviteReceived(m_Subsystem->m_SessionsHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());
	}

	/* Listen for Session Invites */
	{
		FSessionInviteAcceptedCallback* CallbackObj = new FSessionInviteAcceptedCallback();
		m_SessionInviteAcceptedCallback = CallbackObj;
		CallbackObj->m_CallbackLambda = [this](const EOS_Sessions_SessionInviteAcceptedCallbackInfo* data)
		{
			UE_LOG_ONLINE_SESSION(Log, TEXT("Invite accepted"));

			EOS_HSessionDetails m_SessionDetailsHandle = NULL;
			const EOS_Sessions_CopySessionHandleByInviteIdOptions SessionHandleByInviteIdOptions = {EOS_SESSIONS_COPYSESSIONHANDLEBYINVITEID_API_LATEST, data->InviteId};

			const EOS_EResult CopySessionHandleResult = EOS_Sessions_CopySessionHandleByInviteId(m_Subsystem->m_SessionsHandle, &SessionHandleByInviteIdOptions, &m_SessionDetailsHandle);

			if (CopySessionHandleResult == EOS_EResult::EOS_Success)
			{
				EOS_SessionDetails_Info* SessionInfo = new EOS_SessionDetails_Info;
				constexpr EOS_SessionDetails_CopyInfoOptions CopyInfoOptions = {EOS_SESSIONDETAILS_COPYINFO_API_LATEST};

				const EOS_EResult CopySessionDetailsResult = EOS_SessionDetails_CopyInfo(m_SessionDetailsHandle, &CopyInfoOptions, &SessionInfo);

				if (CopySessionHandleResult == EOS_EResult::EOS_Success)
				{
					FOnlineSessionSearchResult SearchResult;
					SearchResult.PingInMs = -1;
					SearchResult.Session.SessionInfo = MakeShareable(new FOnlineSessionInfoEOSCore(SessionInfo->HostAddress, SessionInfo->SessionId, m_SessionDetailsHandle));

					CopyAttributes(m_SessionDetailsHandle, SearchResult.Session);
					TriggerOnSessionUserInviteAcceptedDelegates(true, 0, m_Subsystem->GetIdentityInterface()->GetUniquePlayerId(0), SearchResult);
				}
				else
				{
					UE_LOG_ONLINE_SESSION(Error, TEXT("EOS_SessionDetails_CopyInfo() failed with EOS result code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(CopySessionDetailsResult)));
				}
			}
			else
			{
				UE_LOG_ONLINE_SESSION(Error, TEXT("EOS_Sessions_CopySessionHandleByInviteId() failed with EOS result code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(CopySessionHandleResult)));
			}
		};

		EOS_Sessions_AddNotifySessionInviteAcceptedOptions Options = {};
		Options.ApiVersion = EOS_SESSIONS_ADDNOTIFYSESSIONINVITEACCEPTED_API_LATEST;
		m_SessionInviteAcceptedId = EOS_Sessions_AddNotifySessionInviteAccepted(m_Subsystem->m_SessionsHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());
	}
	const FOnStartSessionCompleteDelegate StartSessionDelegate = FOnStartSessionCompleteDelegate::CreateLambda([this](FName SessionName, bool bWasSuccessful)
	{
		FNamedOnlineSession* Session = GetNamedSession(SessionName);

		if (Session)
		{
			this->UpdatePresenceJoinInfo(0, Session);
		}
	});

	AddOnStartSessionCompleteDelegate_Handle(StartSessionDelegate);

	const FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateLambda([this](FName SessionName, EOnJoinSessionCompleteResult::Type result)
	{
		FNamedOnlineSession* Session = GetNamedSession(SessionName);

		if (Session)
		{
			this->UpdatePresenceJoinInfo(0, Session);
		}
	});

	AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	const FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateLambda([this](FName SessionName, bool)
	{
		FNamedOnlineSession* Session = GetNamedSession(SessionName);

		if (Session)
		{
			this->UpdatePresenceJoinInfo(0, Session);
		}
	});

	AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);
}

bool FOnlineSessionEOSCore::CreateSession(int32 HostingPlayerNum, FName SessionName, const FOnlineSessionSettings& NewSessionSettings)
{
	uint32 Result = ONLINE_FAIL;

	FNamedOnlineSession* Session = GetNamedSession(SessionName);
	if (Session == nullptr)
	{
		if (bIsDedicatedServer || m_Subsystem->m_IdentityInterfacePtr->GetLoginStatus(HostingPlayerNum) >= ELoginStatus::UsingLocalProfile)
		{
			Session = AddNamedSession(SessionName, NewSessionSettings);
			check(Session);

			Session->SessionState = EOnlineSessionState::Creating;
			Session->OwningUserId = m_Subsystem->m_IdentityInterfacePtr->GetUniquePlayerId(HostingPlayerNum);

			if (const auto SessionSetting = NewSessionSettings.Settings.Find("SESSION_NAME"))
			{
				FString NewSessionName;
				SessionSetting->Data.GetValue(NewSessionName);
				Session->OwningUserName = NewSessionName;
			}

			if (Session->OwningUserName.Len() == 0)
			{
				Session->OwningUserName = m_Subsystem->m_IdentityInterfacePtr->GetPlayerNickname(HostingPlayerNum);
			}

			if (bIsDedicatedServer || (Session->OwningUserId.IsValid() && Session->OwningUserId->IsValid()))
			{
				Session->NumOpenPrivateConnections = NewSessionSettings.NumPrivateConnections;
				Session->NumOpenPublicConnections = NewSessionSettings.NumPublicConnections;
				Session->HostingPlayerNum = HostingPlayerNum;
				Session->SessionSettings.BuildUniqueId = GetBuildUniqueId();
				Session->SessionSettings.bIsDedicated = IsRunningDedicatedServer();
				Session->SessionSettings.bAntiCheatProtected = false;
				Session->SessionSettings.bUsesStats = false;

				if (!NewSessionSettings.bIsLANMatch)
				{
					Result = CreateEOSSession(HostingPlayerNum, Session);
				}
				else
				{
					Result = CreateLANSession(HostingPlayerNum, Session);
				}
			}
			else
			{
				UE_LOG_ONLINE_SESSION(Warning, TEXT("Cannot create session '%s': invalid user (%d)."), *SessionName.ToString(), HostingPlayerNum);
			}

			if (Result != ONLINE_IO_PENDING)
			{
				Session->SessionState = EOnlineSessionState::Pending;

				if (Result != ONLINE_SUCCESS)
				{
					RemoveNamedSession(SessionName);
				}
				else
				{
					RegisterLocalPlayers(Session);
				}
			}
		}
		else
		{
			UE_LOG_ONLINE_SESSION(Warning, TEXT("Cannot create session '%s': user not logged in (%d)."), *SessionName.ToString(), HostingPlayerNum);
		}
	}
	else
	{
		UE_LOG_ONLINE_SESSION(Warning, TEXT("Cannot create session '%s': session already exists."), *SessionName.ToString());
	}

	if (Result != ONLINE_IO_PENDING)
	{
		TriggerOnCreateSessionCompleteDelegates(SessionName, (Result == ONLINE_SUCCESS) ? true : false);
	}

	return Result == ONLINE_IO_PENDING || Result == ONLINE_SUCCESS;
}

bool FOnlineSessionEOSCore::CreateSession(const FUniqueNetId& HostingPlayerId, FName SessionName, const FOnlineSessionSettings& NewSessionSettings)
{
	return CreateSession(m_Subsystem->m_IdentityInterfacePtr->GetLocalUserNumFromUniqueNetId(HostingPlayerId), SessionName, NewSessionSettings);
}

void FOnlineSessionEOSCore::SetPermissionLevel(EOS_HSessionModification SessionModHandle, const FNamedOnlineSession* Session) const
{
	EOS_SessionModification_SetPermissionLevelOptions Options = {};
	Options.ApiVersion = EOS_SESSIONMODIFICATION_SETPERMISSIONLEVEL_API_LATEST;
	if (Session->SessionSettings.NumPublicConnections > 0)
	{
		Options.PermissionLevel = EOS_EOnlineSessionPermissionLevel::EOS_OSPF_PublicAdvertised;
	}
	else if (Session->SessionSettings.bAllowJoinViaPresence)
	{
		Options.PermissionLevel = EOS_EOnlineSessionPermissionLevel::EOS_OSPF_JoinViaPresence;
	}
	else
	{
		Options.PermissionLevel = EOS_EOnlineSessionPermissionLevel::EOS_OSPF_InviteOnly;
	}

	UE_LOG_ONLINE_SESSION(Log, TEXT("EOS_SessionModification_SetPermissionLevel() set to (%d)"), static_cast<int32>(Options.PermissionLevel));

	const EOS_EResult ResultCode = EOS_SessionModification_SetPermissionLevel(SessionModHandle, &Options);
	if (ResultCode != EOS_EResult::EOS_Success)
	{
		UE_LOG_ONLINE_SESSION(Error, TEXT("EOS_SessionModification_SetPermissionLevel() failed with EOS result code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(ResultCode)));
	}
}

void FOnlineSessionEOSCore::SetJoinInProgress(EOS_HSessionModification SessionModHandle, const FNamedOnlineSession* Session) const
{
	EOS_SessionModification_SetJoinInProgressAllowedOptions Options = {};
	Options.ApiVersion = EOS_SESSIONMODIFICATION_SETJOININPROGRESSALLOWED_API_LATEST;
	Options.bAllowJoinInProgress = Session->SessionSettings.bAllowJoinInProgress ? EOS_TRUE : EOS_FALSE;

	UE_LOG_ONLINE_SESSION(Log, TEXT("EOS_SessionModification_SetJoinInProgressAllowed() set to (%d)"), Options.bAllowJoinInProgress);

	const EOS_EResult ResultCode = EOS_SessionModification_SetJoinInProgressAllowed(SessionModHandle, &Options);
	if (ResultCode != EOS_EResult::EOS_Success)
	{
		UE_LOG_ONLINE_SESSION(Error, TEXT("EOS_SessionModification_SetJoinInProgressAllowed() failed with EOS result code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(ResultCode)));
	}
}

void FOnlineSessionEOSCore::AddAttribute(EOS_HSessionModification SessionModHandle, const EOS_Sessions_AttributeData* Attribute)
{
	EOS_SessionModification_AddAttributeOptions Options = {};
	Options.ApiVersion = EOS_SESSIONMODIFICATION_ADDATTRIBUTE_API_LATEST;
	Options.AdvertisementType = EOS_ESessionAttributeAdvertisementType::EOS_SAAT_Advertise;
	Options.SessionAttribute = Attribute;

	UE_LOG_ONLINE_SESSION(Log, TEXT("EOS_SessionModification_AddAttribute() named (%s) with value (%s)"), *FString(Attribute->Key), *MakeStringFromAttributeValue(Attribute));

	const EOS_EResult ResultCode = EOS_SessionModification_AddAttribute(SessionModHandle, &Options);
	if (ResultCode != EOS_EResult::EOS_Success)
	{
		UE_LOG_ONLINE_SESSION(Error, TEXT("EOS_SessionModification_AddAttribute() failed for attribute name (%s) with EOS result code (%s)"), *FString(Attribute->Key),
		                      ANSI_TO_TCHAR(EOS_EResult_ToString(ResultCode)));
	}
}

void FOnlineSessionEOSCore::SetAttributes(EOS_HSessionModification SessionModHandle, FNamedOnlineSession* Session)
{
	FAttributeOptions Opt1("NumPrivateConnections", Session->SessionSettings.NumPrivateConnections);
	AddAttribute(SessionModHandle, &Opt1);

	FAttributeOptions Opt2("NumPublicConnections", Session->SessionSettings.NumPublicConnections);
	AddAttribute(SessionModHandle, &Opt2);

	if (Session->OwningUserName.IsEmpty())
	{
		FString OwningPlayerName(TEXT("[DEDICATED] - "));

		FString UserName = FPlatformProcess::UserName();

		if (UserName.IsEmpty())
		{
			FString ComputerName = FPlatformProcess::ComputerName();
			OwningPlayerName += ComputerName;
		}
		else
		{
			OwningPlayerName += UserName;
		}

		Session->OwningUserName = OwningPlayerName;
	}

	FAttributeOptions Opt3("OwningPlayerName", TCHAR_TO_UTF8(*Session->OwningUserName));
	AddAttribute(SessionModHandle, &Opt3);

	FString NetId = Session->OwningUserId->ToString();
	FAttributeOptions Opt4("OwningNetId", TCHAR_TO_UTF8(*NetId));
	AddAttribute(SessionModHandle, &Opt4);

	FAttributeOptions Opt5("bAntiCheatProtected", Session->SessionSettings.bAntiCheatProtected);
	AddAttribute(SessionModHandle, &Opt5);

	FAttributeOptions Opt6("bUsesStats", Session->SessionSettings.bUsesStats);
	AddAttribute(SessionModHandle, &Opt6);

	FAttributeOptions Opt7("bIsDedicated", Session->SessionSettings.bIsDedicated);
	AddAttribute(SessionModHandle, &Opt7);

	FAttributeOptions Opt8("BuildUniqueId", Session->SessionSettings.BuildUniqueId);
	AddAttribute(SessionModHandle, &Opt8);

	FString ServerPort;
	FParse::Value(FCommandLine::Get(), TEXT("Port="), ServerPort);

	if (ServerPort.IsEmpty() == true)
	{
		if (UWorld* World = GetWorldForOnline(m_Subsystem->GetInstanceName()))
		{
			ServerPort = FString::FromInt(World->URL.Port);
		}
	}
	
	if (ServerPort.Len() > 0)
	{
		FAttributeOptions Opt9("ServerPort", ServerPort);
		AddAttribute(SessionModHandle, &Opt9);
	}

	if (auto SessionSetting = Session->SessionSettings.Settings.Find("SESSION_NAME"))
	{
		FString CustomSessionName;
		SessionSetting->Data.GetValue(CustomSessionName);

		if (CustomSessionName.Len() > 0)
		{
			FAttributeOptions Opt9("SESSION_NAME", TCHAR_TO_UTF8(*CustomSessionName));
			AddAttribute(SessionModHandle, &Opt9);
		}
	}

	for (FSessionSettings::TConstIterator It(Session->SessionSettings.Settings); It; ++It)
	{
		const FName KeyName = It.Key();
		const FOnlineSessionSetting& Setting = It.Value();

		if (Setting.AdvertisementType < EOnlineDataAdvertisementType::ViaOnlineService || !IsSessionSettingTypeSupported(Setting.Data.GetType()))
		{
			continue;
		}

		FAttributeOptions Attribute(TCHAR_TO_UTF8(*KeyName.ToString()), Setting.Data);

		AddAttribute(SessionModHandle, &Attribute);
	}
}

struct FBeginMetricsOptions :
	public EOS_Metrics_BeginPlayerSessionOptions
{
	char DisplayNameAnsi[EOSCORE_OSS_STRING_BUFFER_LENGTH + 1];
	char ServerIpAnsi[EOSCORE_OSS_STRING_BUFFER_LENGTH + 1];
	char SessionIdAnsi[EOSCORE_OSS_STRING_BUFFER_LENGTH + 1];
	char ExternalIdAnsi[EOSCORE_OSS_STRING_BUFFER_LENGTH + 1];

	FBeginMetricsOptions() :
		EOS_Metrics_BeginPlayerSessionOptions()
	{
		ApiVersion = EOS_METRICS_BEGINPLAYERSESSION_API_LATEST;
		GameSessionId = SessionIdAnsi;
		DisplayName = DisplayNameAnsi;
		ServerIp = ServerIpAnsi;
		AccountId.External = ExternalIdAnsi;
	}
};

void FOnlineSessionEOSCore::BeginSessionAnalytics(const FNamedOnlineSession* Session) const
{
	if (m_Subsystem)
	{
		const int32 LocalUserNum = m_Subsystem->m_IdentityInterfacePtr->GetDefaultLocalUser();

		if (EOS_EpicAccountId_IsValid(m_Subsystem->m_IdentityInterfacePtr->GetLocalEpicAccountId(LocalUserNum)))
		{
			const FOnlineUserPtr LocalUser = m_Subsystem->m_IdentityInterfacePtr->GetLocalOnlineUser(LocalUserNum);

			if (LocalUser.IsValid())
			{
				const TSharedPtr<const FOnlineSessionInfoEOSCore> SessionInfo = StaticCastSharedPtr<const FOnlineSessionInfoEOSCore>(Session->SessionInfo);

				FBeginMetricsOptions Options;
				FCStringAnsi::Strncpy(Options.ServerIpAnsi, TCHAR_TO_UTF8(*SessionInfo->m_HostAddr->ToString(false)), EOSCORE_OSS_STRING_BUFFER_LENGTH + 1);
				const FString DisplayName = LocalUser->GetDisplayName();
				FCStringAnsi::Strncpy(Options.DisplayNameAnsi, TCHAR_TO_UTF8(*DisplayName), EOSCORE_OSS_STRING_BUFFER_LENGTH + 1);
				Options.AccountIdType = EOS_EMetricsAccountIdType::EOS_MAIT_Epic;
				Options.AccountId.Epic = m_Subsystem->m_IdentityInterfacePtr->GetLocalEpicAccountId(LocalUserNum);

				const EOS_EResult Result = EOS_Metrics_BeginPlayerSession(m_Subsystem->m_MetricsHandle, &Options);

				if (Result != EOS_EResult::EOS_Success)
				{
					UE_LOG_ONLINE_SESSION(Error, TEXT("EOS_Metrics_BeginPlayerSession() returned EOS result code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(Result)));
				}
			}
		}
	}
}

void FOnlineSessionEOSCore::UpdatePresenceJoinInfo(int32 LocalUserNum, FNamedOnlineSession* Session) const
{
	UE_LOG_ONLINE_SESSION(Log, TEXT("UpdatePresenceJoinInfo() (%d) %s"), LocalUserNum, *Session->SessionName.ToString());

	if (m_Subsystem && EOS_EpicAccountId_IsValid(m_Subsystem->m_IdentityInterfacePtr->GetLocalEpicAccountId(LocalUserNum)))
	{
		EOS_Presence_CreatePresenceModificationOptions CreatePresenceOptions = {};
		CreatePresenceOptions.ApiVersion = EOS_PRESENCE_CREATEPRESENCEMODIFICATION_API_LATEST;
		CreatePresenceOptions.LocalUserId = m_Subsystem->m_IdentityInterfacePtr->GetLocalEpicAccountId(LocalUserNum);

		EOS_HPresenceModification PresenceHandle = NULL;
		EOS_Presence_CreatePresenceModification(m_Subsystem->m_PresenceHandle, &CreatePresenceOptions, &PresenceHandle);

		const TSharedPtr<const FOnlineSessionInfoEOSCore> SessionInfo = StaticCastSharedPtr<const FOnlineSessionInfoEOSCore>(Session->SessionInfo);

		char JoinInfoAnsi[256];
		FCStringAnsi::Strncpy(JoinInfoAnsi, TCHAR_TO_UTF8(*SessionInfo->m_HostAddr->ToString(true)), (SessionInfo->m_HostAddr->ToString(true).Len() + 1));

		EOS_PresenceModification_SetJoinInfoOptions SetJoinInfoOptions = {};
		SetJoinInfoOptions.ApiVersion = EOS_PRESENCEMODIFICATION_SETJOININFO_API_LATEST;
		SetJoinInfoOptions.JoinInfo = JoinInfoAnsi;

		EOS_PresenceModification_SetJoinInfo(PresenceHandle, &SetJoinInfoOptions);

		const EOS_Presence_SetPresenceOptions SetPresenceOptions = {EOS_PRESENCE_SETPRESENCE_API_LATEST, m_Subsystem->m_IdentityInterfacePtr->GetLocalEpicAccountId(LocalUserNum), PresenceHandle};

		char RichTextAnsi[256];
		FCStringAnsi::Strncpy(RichTextAnsi, TCHAR_TO_UTF8(*Session->SessionName.ToString()), (Session->SessionName.ToString().Len() + 1));

		EOS_PresenceModification_SetRawRichTextOptions RichTextOptions = {};
		RichTextOptions.ApiVersion = EOS_PRESENCEMODIFICATION_SETRAWRICHTEXT_API_LATEST;
		RichTextOptions.RichText = RichTextAnsi;

		EOS_PresenceModification_SetRawRichText(PresenceHandle, &RichTextOptions);
		EOS_Presence_SetPresence(m_Subsystem->m_PresenceHandle, &SetPresenceOptions, nullptr, ([](const EOS_Presence_SetPresenceCallbackInfo* callbackData)
		{
			UE_LOG_ONLINE_SESSION(Log, TEXT("EOS_Presence_SetPresence() %s"), *FString(EOS_EResult_ToString(callbackData->ResultCode)));
		}));
	}
}

uint32 FOnlineSessionEOSCore::CreateEOSSession(int32 HostingPlayerNum, FNamedOnlineSession* Session)
{
	check(Session != nullptr);

	EOS_HSessionModification SessionModHandle = nullptr;

	FSessionCreateOptions Options(TCHAR_TO_UTF8(*Session->SessionName.ToString()));
	Options.MaxPlayers = Session->SessionSettings.NumPrivateConnections + Session->SessionSettings.NumPublicConnections;
	Options.LocalUserId = m_Subsystem->m_IdentityInterfacePtr->GetLocalProductUserId(HostingPlayerNum);
	Options.bPresenceEnabled = (Session->SessionSettings.bUsesPresence ||
		                           Session->SessionSettings.bAllowJoinViaPresence ||
		                           Session->SessionSettings.bAllowJoinViaPresenceFriendsOnly ||
		                           Session->SessionSettings.bAllowInvites)
		                           ? EOS_TRUE
		                           : EOS_FALSE;

	const EOS_EResult ResultCode = EOS_Sessions_CreateSessionModification(m_Subsystem->m_SessionsHandle, &Options, &SessionModHandle);

	if (ResultCode != EOS_EResult::EOS_Success)
	{
		UE_LOG_ONLINE_SESSION(Error, TEXT("EOS_Sessions_CreateSessionModification() failed with EOS result code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(ResultCode)));
		return ONLINE_FAIL;
	}

	Session->SessionState = EOnlineSessionState::Creating;

	FString HostAddr;
	if (!bIsDedicatedServer && bIsUsingP2PSockets)
	{
		const FInternetAddrEOSCore TempAddr(MakeStringFromProductUserId(Options.LocalUserId), Session->SessionName.ToString(), FURL::UrlConfig.DefaultPort);
		HostAddr = TempAddr.ToString(true);
		char HostAddrAnsi[EOSCORE_OSS_STRING_BUFFER_LENGTH + 1];
		FCStringAnsi::Strncpy(HostAddrAnsi, TCHAR_TO_UTF8(*HostAddr), EOSCORE_OSS_STRING_BUFFER_LENGTH + 1);

		EOS_SessionModification_SetHostAddressOptions HostOptions = {};
		HostOptions.ApiVersion = EOS_SESSIONMODIFICATION_SETHOSTADDRESS_API_LATEST;
		HostOptions.HostAddress = HostAddrAnsi;
		const EOS_EResult HostResult = EOS_SessionModification_SetHostAddress(SessionModHandle, &HostOptions);
		UE_LOG_ONLINE_SESSION(Log, TEXT("EOS_SessionModification_SetHostAddress(%s) returned (%s)"), *HostAddr, ANSI_TO_TCHAR(EOS_EResult_ToString(HostResult)));
	}
	else
	{
		HostAddr = TEXT("127.0.0.1");
	}

	Session->SessionInfo = MakeShareable(new FOnlineSessionInfoEOSCore(HostAddr, Session->SessionName.ToString(), nullptr));

	FUpdateSessionCallback* CallbackObj = new FUpdateSessionCallback();
	CallbackObj->m_CallbackLambda = [this, Session](const EOS_Sessions_UpdateSessionCallbackInfo* Data)
	{
		const bool bWasSuccessful = Data->ResultCode == EOS_EResult::EOS_Success || Data->ResultCode == EOS_EResult::EOS_Sessions_OutOfSync;

		if (bWasSuccessful)
		{
			Session->SessionState = EOnlineSessionState::Pending;
			BeginSessionAnalytics(Session);
			RegisterLocalPlayers(Session);
		}
		else
		{
			Session->SessionState = EOnlineSessionState::NoSession;
			UE_LOG_ONLINE_SESSION(Error, TEXT("EOS_Sessions_UpdateSession() failed with EOS result code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
		}

		TriggerOnCreateSessionCompleteDelegates(Session->SessionName, bWasSuccessful);
	};


	return SharedSessionUpdate(SessionModHandle, Session, CallbackObj);
}

uint32 FOnlineSessionEOSCore::SharedSessionUpdate(EOS_HSessionModification SessionModHandle, FNamedOnlineSession* Session, FUpdateSessionCallback* Callback)
{
	SetPermissionLevel(SessionModHandle, Session);
	SetJoinInProgress(SessionModHandle, Session);
	SetAttributes(SessionModHandle, Session);

	EOS_Sessions_UpdateSessionOptions CreateOptions = {};
	CreateOptions.ApiVersion = EOS_SESSIONS_UPDATESESSION_API_LATEST;
	CreateOptions.SessionModificationHandle = SessionModHandle;

	EOS_Sessions_UpdateSession(m_Subsystem->m_SessionsHandle, &CreateOptions, Callback, Callback->GetCallbackPtr());

	EOS_SessionModification_Release(SessionModHandle);

	return ONLINE_IO_PENDING;
}

bool FOnlineSessionEOSCore::StartSession(FName SessionName)
{
	uint32 Result = ONLINE_FAIL;
	FNamedOnlineSession* Session = GetNamedSession(SessionName);
	if (Session)
	{
		if (Session->SessionState == EOnlineSessionState::Pending ||
			Session->SessionState == EOnlineSessionState::Ended)
		{
			if (!Session->SessionSettings.bIsLANMatch)
			{
				Result = StartEOSSession(Session);
			}
			else
			{
				if (!Session->SessionSettings.bAllowJoinInProgress)
				{
					m_LANSession->StopLANSession();
				}
				Result = ONLINE_SUCCESS;
				Session->SessionState = EOnlineSessionState::InProgress;
			}
		}
		else
		{
			UE_LOG_ONLINE_SESSION(Warning, TEXT("Can't start an online session (%s) in state %s"),
			                      *SessionName.ToString(),
			                      EOnlineSessionState::ToString(Session->SessionState));
		}
	}
	else
	{
		UE_LOG_ONLINE_SESSION(Warning, TEXT("Can't start an online game for session (%s) that hasn't been created"), *SessionName.ToString());
	}

	if (Result != ONLINE_IO_PENDING)
	{
		TriggerOnStartSessionCompleteDelegates(SessionName, (Result == ONLINE_SUCCESS) ? true : false);
	}

	return Result == ONLINE_SUCCESS || Result == ONLINE_IO_PENDING;
}

typedef TEOSCallback<EOS_Sessions_OnStartSessionCallback, EOS_Sessions_StartSessionCallbackInfo> FStartSessionCallback;

uint32 FOnlineSessionEOSCore::StartEOSSession(FNamedOnlineSession* Session)
{
	Session->SessionState = EOnlineSessionState::Starting;

	const FSessionStartOptions Options(TCHAR_TO_UTF8(*Session->SessionName.ToString()));
	FStartSessionCallback* CallbackObj = new FStartSessionCallback();
	CallbackObj->m_CallbackLambda = [this, Session](const EOS_Sessions_StartSessionCallbackInfo* Data)
	{
		Session->SessionState = EOnlineSessionState::InProgress;

		const bool bWasSuccessful = Data->ResultCode == EOS_EResult::EOS_Success;
		if (!bWasSuccessful)
		{
			UE_LOG_ONLINE_SESSION(Error, TEXT("EOS_Sessions_StartSession() failed with EOS result code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
		}
		TriggerOnStartSessionCompleteDelegates(Session->SessionName, bWasSuccessful);
	};

	EOS_Sessions_StartSession(m_Subsystem->m_SessionsHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());

	return ONLINE_IO_PENDING;
}

bool FOnlineSessionEOSCore::UpdateSession(FName SessionName, FOnlineSessionSettings& UpdatedSessionSettings, bool bShouldRefreshOnlineData)
{
	int32 Result = ONLINE_FAIL;

	FNamedOnlineSession* Session = GetNamedSession(SessionName);
	if (Session)
	{
		Session->SessionSettings = UpdatedSessionSettings;

		if (!Session->SessionSettings.bIsLANMatch)
		{
			Result = UpdateEOSSession(Session, UpdatedSessionSettings);
		}
		else
		{
			Result = ONLINE_SUCCESS;
		}
	}
	else
	{
		UE_LOG_ONLINE_SESSION(Warning, TEXT("No session (%s) found for update!"), *SessionName.ToString());
	}

	if (Result != ONLINE_IO_PENDING)
	{
		TriggerOnUpdateSessionCompleteDelegates(SessionName, Result == ONLINE_SUCCESS);
	}

	return Result == ONLINE_SUCCESS || Result == ONLINE_IO_PENDING;
}

uint32 FOnlineSessionEOSCore::UpdateEOSSession(FNamedOnlineSession* Session, FOnlineSessionSettings& UpdatedSessionSettings)
{
	if (Session->SessionState == EOnlineSessionState::Creating)
	{
		return ONLINE_IO_PENDING;
	}

	EOS_HSessionModification SessionModHandle = NULL;
	const FSessionUpdateOptions Options(TCHAR_TO_UTF8(*Session->SessionName.ToString()));

	const EOS_EResult ResultCode = EOS_Sessions_UpdateSessionModification(m_Subsystem->m_SessionsHandle, &Options, &SessionModHandle);
	if (ResultCode != EOS_EResult::EOS_Success)
	{
		UE_LOG_ONLINE_SESSION(Error, TEXT("EOS_Sessions_UpdateSessionModification() failed with EOS result code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(ResultCode)));
		return ONLINE_FAIL;
	}

	FUpdateSessionCallback* CallbackObj = new FUpdateSessionCallback();
	CallbackObj->m_CallbackLambda = [this, Session](const EOS_Sessions_UpdateSessionCallbackInfo* Data)
	{
		const bool bWasSuccessful = Data->ResultCode == EOS_EResult::EOS_Success || Data->ResultCode == EOS_EResult::EOS_Sessions_OutOfSync;
		if (!bWasSuccessful)
		{
			Session->SessionState = EOnlineSessionState::NoSession;
			UE_LOG_ONLINE_SESSION(Error, TEXT("EOS_Sessions_UpdateSession() failed with EOS result code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
		}
		TriggerOnUpdateSessionCompleteDelegates(Session->SessionName, bWasSuccessful);
	};

	return SharedSessionUpdate(SessionModHandle, Session, CallbackObj);
}

bool FOnlineSessionEOSCore::EndSession(FName SessionName)
{
	uint32 Result = ONLINE_FAIL;

	FNamedOnlineSession* Session = GetNamedSession(SessionName);
	if (Session)
	{
		if (Session->SessionState == EOnlineSessionState::InProgress)
		{
			if (!Session->SessionSettings.bIsLANMatch)
			{
				Result = EndEOSSession(Session);
			}
			else
			{
				if (Session->SessionSettings.bShouldAdvertise &&
					!m_LANSession.IsValid() &&
					m_LANSession->LanBeacon == nullptr &&
					m_Subsystem->IsServer())
				{
					Result = CreateLANSession(Session->HostingPlayerNum, Session);
				}
				else
				{
					Result = ONLINE_SUCCESS;
				}
			}
		}
		else
		{
			UE_LOG_ONLINE_SESSION(Warning, TEXT("Can't end session (%s) in state %s"),
			                      *SessionName.ToString(),
			                      EOnlineSessionState::ToString(Session->SessionState));
		}
	}
	else
	{
		UE_LOG_ONLINE_SESSION(Warning, TEXT("Can't end an online game for session (%s) that hasn't been created"),
		                      *SessionName.ToString());
	}

	if (Result != ONLINE_IO_PENDING)
	{
		if (Session)
		{
			Session->SessionState = EOnlineSessionState::Ended;
		}

		TriggerOnEndSessionCompleteDelegates(SessionName, (Result == ONLINE_SUCCESS) ? true : false);
	}

	return Result == ONLINE_SUCCESS || Result == ONLINE_IO_PENDING;
}

typedef TEOSCallback<EOS_Sessions_OnEndSessionCallback, EOS_Sessions_EndSessionCallbackInfo> FEndSessionCallback;

uint32 FOnlineSessionEOSCore::EndEOSSession(FNamedOnlineSession* Session)
{
	check(Session && Session->SessionState == EOnlineSessionState::InProgress);

	Session->SessionState = EOnlineSessionState::Ending;

	const FSessionEndOptions Options(TCHAR_TO_UTF8(*Session->SessionName.ToString()));
	FEndSessionCallback* CallbackObj = new FEndSessionCallback();
	CallbackObj->m_CallbackLambda = [this, Session](const EOS_Sessions_EndSessionCallbackInfo* Data)
	{
		Session->SessionState = EOnlineSessionState::Ended;

		const bool bWasSuccessful = Data->ResultCode == EOS_EResult::EOS_Success;
		if (!bWasSuccessful)
		{
			UE_LOG_ONLINE_SESSION(Error, TEXT("EOS_Sessions_EndSession() failed with EOS result code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
		}
		TriggerOnEndSessionCompleteDelegates(Session->SessionName, bWasSuccessful);
	};

	EOS_Sessions_EndSession(m_Subsystem->m_SessionsHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());

	return ONLINE_IO_PENDING;
}

bool FOnlineSessionEOSCore::DestroySession(FName SessionName, const FOnDestroySessionCompleteDelegate& CompletionDelegate)
{
	uint32 Result = ONLINE_FAIL;

	FNamedOnlineSession* Session = GetNamedSession(SessionName);
	if (Session)
	{
		if (Session->SessionState != EOnlineSessionState::Destroying)
		{
			if (!Session->SessionSettings.bIsLANMatch)
			{
				if (Session->SessionState == EOnlineSessionState::InProgress)
				{
					Result = EndEOSSession(Session);
				}

				Result = DestroyEOSSession(Session, CompletionDelegate);
			}
			else
			{
				if (m_LANSession.IsValid())
				{
					m_LANSession->StopLANSession();
					m_LANSession = nullptr;
				}

				Result = ONLINE_SUCCESS;
			}

			if (Result != ONLINE_IO_PENDING)
			{
				RemoveNamedSession(Session->SessionName);
				CompletionDelegate.ExecuteIfBound(SessionName, (Result == ONLINE_SUCCESS) ? true : false);
				TriggerOnDestroySessionCompleteDelegates(SessionName, (Result == ONLINE_SUCCESS) ? true : false);
			}
		}
		else
		{
			UE_LOG_ONLINE_SESSION(Warning, TEXT("Already in process of destroying session (%s)"), *SessionName.ToString());
		}
	}
	else
	{
		UE_LOG_ONLINE_SESSION(Warning, TEXT("Can't destroy a null online session (%s)"), *SessionName.ToString());
		CompletionDelegate.ExecuteIfBound(SessionName, false);
		TriggerOnDestroySessionCompleteDelegates(SessionName, false);
	}

	return Result == ONLINE_SUCCESS || Result == ONLINE_IO_PENDING;
}

void FOnlineSessionEOSCore::EndSessionAnalytics() const
{
	if (m_Subsystem)
	{
		const int32 LocalUserNum = m_Subsystem->m_IdentityInterfacePtr->GetDefaultLocalUser();

		if (EOS_EpicAccountId_IsValid(m_Subsystem->m_IdentityInterfacePtr->GetLocalEpicAccountId(LocalUserNum)))
		{
			const FOnlineUserPtr LocalUser = m_Subsystem->m_IdentityInterfacePtr->GetLocalOnlineUser(LocalUserNum);
			if (LocalUser.IsValid())
			{
				FEndMetricsOptions Options;
				Options.AccountIdType = EOS_EMetricsAccountIdType::EOS_MAIT_Epic;
				Options.AccountId.Epic = m_Subsystem->m_IdentityInterfacePtr->GetLocalEpicAccountId(LocalUserNum);

				const EOS_EResult Result = EOS_Metrics_EndPlayerSession(m_Subsystem->m_MetricsHandle, &Options);
				if (Result != EOS_EResult::EOS_Success)
				{
					UE_LOG_ONLINE_SESSION(Error, TEXT("EOS_Metrics_EndPlayerSession() returned EOS result code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(Result)));
				}
			}
		}
	}
}


typedef TEOSCallback<EOS_Sessions_OnDestroySessionCallback, EOS_Sessions_DestroySessionCallbackInfo> FDestroySessionCallback;

uint32 FOnlineSessionEOSCore::DestroyEOSSession(FNamedOnlineSession* Session, const FOnDestroySessionCompleteDelegate& CompletionDelegate)
{
	Session->SessionState = EOnlineSessionState::Destroying;

	const FSessionDestroyOptions Options(TCHAR_TO_UTF8(*Session->SessionName.ToString()));
	FDestroySessionCallback* CallbackObj = new FDestroySessionCallback();
	CallbackObj->m_CallbackLambda = [this, Session](const EOS_Sessions_DestroySessionCallbackInfo* Data)
	{
		EndSessionAnalytics();

		Session->SessionState = EOnlineSessionState::NoSession;
		const FName SessionName = Session->SessionName;

		const bool bWasSuccessful = Data->ResultCode == EOS_EResult::EOS_Success;
		if (!bWasSuccessful)
		{
			UE_LOG_ONLINE_SESSION(Error, TEXT("EOS_Sessions_DestroySession() failed with EOS result code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
		}
		RemoveNamedSession(SessionName);
		TriggerOnDestroySessionCompleteDelegates(SessionName, bWasSuccessful);
	};

	IOnlineVoicePtr VoiceInt = m_Subsystem->GetVoiceInterface();
	if (VoiceInt.IsValid())
	{
		// Destruct the VOIP Synth components to avoid FScene->Release crash after transitioning scene once the session is destroyed.
		// NOTE: This is a well known oversight by Epic without a rewrite of either the engine or the VoiceEngine...
		// NOTE: The reason these need to be destroyed is because the VOIP synth component is handled in the VoiceEngine with a custom GC implemented via a FVoiceSerializeHelper of type FGCObject,
		//       this does note take into account that the Synth components base class creates an AudioComponent with no owner, thus causing the FScene->Release crash on travel.
		// NOTE: If they are still needed for whatever reason they will still be recreated (From what I have read and tested this is a good practice and should really have been implemented by epic.)
		DestroyLocalVOIPComponents();
		
		VoiceInt->UnregisterLocalTalkers();
		VoiceInt->RemoveAllRemoteTalkers();
		UVOIPStatics::ClearAllSettings();
	}

	EOS_Sessions_DestroySession(m_Subsystem->m_SessionsHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());

	return ONLINE_IO_PENDING;
}

bool FOnlineSessionEOSCore::IsPlayerInSession(FName SessionName, const FUniqueNetId& UniqueId)
{
	return IsPlayerInSessionImpl(this, SessionName, UniqueId);
}

bool FOnlineSessionEOSCore::StartMatchmaking(const TArray<TSharedRef<const FUniqueNetId>>& LocalPlayers, FName SessionName, const FOnlineSessionSettings& NewSessionSettings, TSharedRef<FOnlineSessionSearch>& SearchSettings)
{
	UE_LOG_ONLINE_SESSION(Warning, TEXT("StartMatchmaking is not supported on this platform. Use FindSessions or FindSessionById."));
	TriggerOnMatchmakingCompleteDelegates(SessionName, false);
	return false;
}

bool FOnlineSessionEOSCore::CancelMatchmaking(int32 SearchingPlayerNum, FName SessionName)
{
	UE_LOG_ONLINE_SESSION(Warning, TEXT("CancelMatchmaking is not supported on this platform. Use CancelFindSessions."));
	TriggerOnCancelMatchmakingCompleteDelegates(SessionName, false);
	return false;
}

bool FOnlineSessionEOSCore::CancelMatchmaking(const FUniqueNetId& SearchingPlayerId, FName SessionName)
{
	UE_LOG_ONLINE_SESSION(Warning, TEXT("CancelMatchmaking is not supported on this platform. Use CancelFindSessions."));
	TriggerOnCancelMatchmakingCompleteDelegates(SessionName, false);
	return false;
}

bool FOnlineSessionEOSCore::FindSessions(int32 SearchingPlayerNum, const TSharedRef<FOnlineSessionSearch>& SearchSettings)
{
	uint32 Return = ONLINE_FAIL;

	if (!m_CurrentSessionSearch.IsValid() || SearchSettings->SearchState != EOnlineAsyncTaskState::InProgress)
	{
		m_SessionSearchStartInSeconds = FPlatformTime::Seconds();

		SearchSettings->SearchResults.Empty();
		m_CurrentSessionSearch = SearchSettings;

		if (SearchSettings->bIsLanQuery == false)
		{
			Return = FindEOSSession(SearchingPlayerNum, SearchSettings);
		}
		else
		{
			Return = FindLANSession();
		}

		if (Return == ONLINE_IO_PENDING)
		{
			SearchSettings->SearchState = EOnlineAsyncTaskState::InProgress;
		}
	}
	else
	{
		UE_LOG_ONLINE_SESSION(Warning, TEXT("Ignoring game search request while another search is pending"));
		Return = ONLINE_IO_PENDING;
	}

	return Return == ONLINE_SUCCESS || Return == ONLINE_IO_PENDING;
}

bool FOnlineSessionEOSCore::FindSessions(const FUniqueNetId& SearchingPlayerId, const TSharedRef<FOnlineSessionSearch>& SearchSettings)
{
	return FindSessions(m_Subsystem->m_IdentityInterfacePtr->GetLocalUserNumFromUniqueNetId(SearchingPlayerId), SearchSettings);
}

bool FOnlineSessionEOSCore::FindSessionById(const FUniqueNetId& SearchingUserId, const FUniqueNetId& SessionId, const FUniqueNetId& FriendId, const FOnSingleSessionResultCompleteDelegate& CompletionDelegates)
{
	const FOnlineSessionSearchResult EmptyResult;
	CompletionDelegates.ExecuteIfBound(m_Subsystem->m_IdentityInterfacePtr->GetLocalUserNumFromUniqueNetId(SearchingUserId), false, EmptyResult);
	return true;
}

void FOnlineSessionEOSCore::AddSearchAttribute(EOS_HSessionSearch SearchHandle, const EOS_Sessions_AttributeData* Attribute, EOS_EOnlineComparisonOp ComparisonOp) const
{
	EOS_SessionSearch_SetParameterOptions Options = {};
	Options.ApiVersion = EOS_SESSIONSEARCH_SETPARAMETER_API_LATEST;
	Options.Parameter = Attribute;
	Options.ComparisonOp = ComparisonOp;

	const EOS_EResult ResultCode = EOS_SessionSearch_SetParameter(SearchHandle, &Options);
	
	if (ResultCode != EOS_EResult::EOS_Success)
	{
		UE_LOG_ONLINE_SESSION(Error, TEXT("EOS_SessionSearch_SetParameter() failed with EOS result code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(ResultCode)));
	}
}

void FOnlineSessionEOSCore::CopySearchResult(EOS_HSessionDetails SessionHandle, const EOS_SessionDetails_Info* SessionInfo, FOnlineSession& OutSession) const
{
	OutSession.SessionSettings.bAllowJoinInProgress = SessionInfo->Settings->bAllowJoinInProgress == EOS_TRUE;
	switch (SessionInfo->Settings->PermissionLevel)
	{
	case EOS_EOnlineSessionPermissionLevel::EOS_OSPF_InviteOnly:
		{
			OutSession.SessionSettings.bAllowInvites = true;
			OutSession.SessionSettings.bUsesPresence = 0;
			OutSession.SessionSettings.bAllowJoinViaPresence = 0;
			OutSession.SessionSettings.NumPrivateConnections = SessionInfo->Settings->NumPublicConnections;
			OutSession.NumOpenPrivateConnections = SessionInfo->NumOpenPublicConnections;
			break;
		}
	case EOS_EOnlineSessionPermissionLevel::EOS_OSPF_JoinViaPresence:
		{
			OutSession.SessionSettings.bAllowInvites = true;
			OutSession.SessionSettings.bUsesPresence = true;
			OutSession.SessionSettings.bAllowJoinViaPresence = true;
			OutSession.SessionSettings.NumPrivateConnections = SessionInfo->Settings->NumPublicConnections;
			OutSession.NumOpenPrivateConnections = SessionInfo->NumOpenPublicConnections;
			break;
		}
	case EOS_EOnlineSessionPermissionLevel::EOS_OSPF_PublicAdvertised:
		{
			OutSession.SessionSettings.bAllowInvites = true;
			OutSession.SessionSettings.bUsesPresence = true;
			OutSession.SessionSettings.bAllowJoinViaPresence = true;
			OutSession.SessionSettings.NumPublicConnections = SessionInfo->Settings->NumPublicConnections;
			OutSession.NumOpenPublicConnections = SessionInfo->NumOpenPublicConnections;
			break;
		}
	}

	CopyAttributes(SessionHandle, OutSession);
}

void FOnlineSessionEOSCore::CopyAttributes(EOS_HSessionDetails SessionHandle, FOnlineSession& OutSession) const
{
	EOS_SessionDetails_GetSessionAttributeCountOptions CountOptions = {};
	CountOptions.ApiVersion = EOS_SESSIONDETAILS_GETSESSIONATTRIBUTECOUNT_API_LATEST;
	const int32 Count = EOS_SessionDetails_GetSessionAttributeCount(SessionHandle, &CountOptions);

	for (int32 Index = 0; Index < Count; Index++)
	{
		EOS_SessionDetails_CopySessionAttributeByIndexOptions AttrOptions = {};
		AttrOptions.ApiVersion = EOS_SESSIONDETAILS_COPYSESSIONATTRIBUTEBYINDEX_API_LATEST;
		AttrOptions.AttrIndex = Index;

		EOS_SessionDetails_Attribute* Attribute = NULL;
		const EOS_EResult ResultCode = EOS_SessionDetails_CopySessionAttributeByIndex(SessionHandle, &AttrOptions, &Attribute);
		if (ResultCode == EOS_EResult::EOS_Success)
		{
			FString Key = Attribute->Data->Key;
			if (Key == TEXT("NumPublicConnections"))
			{
				OutSession.SessionSettings.NumPublicConnections = Attribute->Data->Value.AsInt64;
			}
			else if (Key == TEXT("NumPrivateConnections"))
			{
				OutSession.SessionSettings.NumPrivateConnections = Attribute->Data->Value.AsInt64;
			}
			else if (Key == TEXT("bAntiCheatProtected"))
			{
				OutSession.SessionSettings.bAntiCheatProtected = Attribute->Data->Value.AsBool == EOS_TRUE;
			}
			else if (Key == TEXT("bUsesStats"))
			{
				OutSession.SessionSettings.bUsesStats = Attribute->Data->Value.AsBool == EOS_TRUE;
			}
			else if (Key == TEXT("bIsDedicated"))
			{
				OutSession.SessionSettings.bIsDedicated = Attribute->Data->Value.AsBool == EOS_TRUE;
			}
			else if (Key == TEXT("BuildUniqueId"))
			{
				OutSession.SessionSettings.BuildUniqueId = Attribute->Data->Value.AsInt64;
			}
			else if (Key == TEXT("OwningPlayerName"))
			{
				OutSession.OwningUserName = UTF8_TO_TCHAR(Attribute->Data->Value.AsUtf8);
			}
			else if (Key == TEXT("OwningNetId"))
			{
				OutSession.OwningUserId = MakeShareable(new FUniqueNetIdEOSCore(UTF8_TO_TCHAR(Attribute->Data->Value.AsUtf8)));
			}
			else
			{
				FOnlineSessionSetting Setting;

				switch (Attribute->Data->ValueType)
				{
				case EOS_ESessionAttributeType::EOS_SAT_Boolean:
					{
						Setting.Data.SetValue(Attribute->Data->Value.AsBool == EOS_TRUE);
						break;
					}
				case EOS_ESessionAttributeType::EOS_SAT_Int64:
					{
						Setting.Data.SetValue(static_cast<int64>(Attribute->Data->Value.AsInt64));
						break;
					}
				case EOS_ESessionAttributeType::EOS_SAT_Double:
					{
						Setting.Data.SetValue(Attribute->Data->Value.AsDouble);
						break;
					}
				case EOS_ESessionAttributeType::EOS_SAT_String:
					{
						Setting.Data.SetValue(UTF8_TO_TCHAR(Attribute->Data->Value.AsUtf8));
						break;
					}
				}
				OutSession.SessionSettings.Settings.Add(FName(Key), Setting);
			}
		}

		EOS_SessionDetails_Attribute_Release(Attribute);
	}
}

void FOnlineSessionEOSCore::AddSearchResult(EOS_HSessionDetails SessionHandle, const TSharedRef<FOnlineSessionSearch>& SearchSettings)
{
	EOS_SessionDetails_Info* SessionInfo = nullptr;
	EOS_SessionDetails_CopyInfoOptions CopyOptions = {};
	CopyOptions.ApiVersion = EOS_SESSIONDETAILS_COPYINFO_API_LATEST;
	const EOS_EResult CopyResult = EOS_SessionDetails_CopyInfo(SessionHandle, &CopyOptions, &SessionInfo);
	if (CopyResult == EOS_EResult::EOS_Success)
	{
		const int32 Position = SearchSettings->SearchResults.AddZeroed();
		FOnlineSessionSearchResult& SearchResult = SearchSettings->SearchResults[Position];
		SearchResult.Session.SessionInfo = MakeShareable(new FOnlineSessionInfoEOSCore(SessionInfo->HostAddress, SessionInfo->SessionId, SessionHandle));

		CopySearchResult(SessionHandle, SessionInfo, SearchResult.Session);

		EOS_SessionDetails_Info_Release(SessionInfo);
	}
}

typedef TEOSCallback<EOS_SessionSearch_OnFindCallback, EOS_SessionSearch_FindCallbackInfo> FFindSessionsCallback;

uint32 FOnlineSessionEOSCore::FindEOSSession(int32 SearchingPlayerNum, const TSharedRef<FOnlineSessionSearch>& SearchSettings)
{
	EOS_HSessionSearch SearchHandle = nullptr;
	EOS_Sessions_CreateSessionSearchOptions HandleOptions = {};
	HandleOptions.ApiVersion = EOS_SESSIONS_CREATESESSIONSEARCH_API_LATEST;
	HandleOptions.MaxSearchResults = FMath::Clamp(SearchSettings->MaxSearchResults, 0, EOS_SESSIONS_MAX_SEARCH_RESULTS);

	const EOS_EResult Result = EOS_Sessions_CreateSessionSearch(m_Subsystem->m_SessionsHandle, &HandleOptions, &SearchHandle);
	if (Result != EOS_EResult::EOS_Success)
	{
		UE_LOG_ONLINE_SESSION(Error, TEXT("EOS_Sessions_CreateSessionSearch() failed with EOS result code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(Result)));
		return ONLINE_FAIL;
	}
	
	m_CurrentSearchHandle = MakeShareable(new FSessionSearchEOSCore(SearchHandle));

	const FAttributeOptions NumPublicConnections("NumPublicConnections", 1);
	AddSearchAttribute(SearchHandle, &NumPublicConnections, EOS_EOnlineComparisonOp::EOS_OCO_GREATERTHANOREQUAL);

	const FAttributeOptions BucketId(EOS_SESSIONS_SEARCH_BUCKET_ID, s_BucketId);
	AddSearchAttribute(SearchHandle, &BucketId, EOS_EOnlineComparisonOp::EOS_OCO_EQUAL);

	for (FSearchParams::TConstIterator It(SearchSettings->QuerySettings.SearchParams); It; ++It)
	{
		const FName Key = It.Key();
		const FOnlineSessionSearchParam& SearchParam = It.Value();

		if (!IsSessionSettingTypeSupported(SearchParam.Data.GetType()))
		{
			continue;
		}

		if (Key == SETTING_MAPNAME)
		{
			continue;
		}

		if (Key == SEARCH_DEDICATED_ONLY)
		{
			continue;
		}

		if (Key == SEARCH_EMPTY_SERVERS_ONLY)
		{
			continue;
		}
		
		if (Key == SEARCH_SECURE_SERVERS_ONLY)
		{
			continue;
		}
		
		if (Key == SEARCH_PRESENCE)
		{
			continue;
		}
		
		UE_LOG_ONLINE_SESSION(Log, TEXT("Adding search param named (%s), (%s)"), *Key.ToString(), *SearchParam.ToString());

		FString ParamName = Key.ToString();
		FAttributeOptions Attribute(TCHAR_TO_UTF8(*ParamName), SearchParam.Data);

		AddSearchAttribute(SearchHandle, &Attribute, ToEOSSearchOp(SearchParam.ComparisonOp));
	}

	FFindSessionsCallback* CallbackObj = new FFindSessionsCallback();
	CallbackObj->m_CallbackLambda = [this, SearchSettings](const EOS_SessionSearch_FindCallbackInfo* Data)
	{
		const bool bWasSuccessful = Data->ResultCode == EOS_EResult::EOS_Success;
		if (bWasSuccessful)
		{
			EOS_SessionSearch_GetSearchResultCountOptions SearchResultOptions = {};
			SearchResultOptions.ApiVersion = EOS_SESSIONSEARCH_GETSEARCHRESULTCOUNT_API_LATEST;
			const int32 NumSearchResults = EOS_SessionSearch_GetSearchResultCount(m_CurrentSearchHandle->SearchHandle, &SearchResultOptions);

			EOS_SessionSearch_CopySearchResultByIndexOptions IndexOptions = {};
			IndexOptions.ApiVersion = EOS_SESSIONSEARCH_COPYSEARCHRESULTBYINDEX_API_LATEST;
			for (int32 Index = 0; Index < NumSearchResults; Index++)
			{
				EOS_HSessionDetails SessionHandle = nullptr;
				IndexOptions.SessionIndex = Index;
				const EOS_EResult Result = EOS_SessionSearch_CopySearchResultByIndex(m_CurrentSearchHandle->SearchHandle, &IndexOptions, &SessionHandle);
				if (Result == EOS_EResult::EOS_Success)
				{
					AddSearchResult(SessionHandle, SearchSettings);
				}
			}
			SearchSettings->SearchState = EOnlineAsyncTaskState::Done;
		}
		else
		{
			SearchSettings->SearchState = EOnlineAsyncTaskState::Failed;
			UE_LOG_ONLINE_SESSION(Error, TEXT("EOS_SessionSearch_Find() failed with EOS result code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
		}
		TriggerOnFindSessionsCompleteDelegates(bWasSuccessful);
	};

	SearchSettings->SearchState = EOnlineAsyncTaskState::InProgress;

	EOS_SessionSearch_FindOptions Options = {};
	Options.ApiVersion = EOS_SESSIONSEARCH_FIND_API_LATEST;
	Options.LocalUserId = m_Subsystem->m_IdentityInterfacePtr->GetLocalProductUserId(SearchingPlayerNum);
	EOS_SessionSearch_Find(SearchHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());

	return ONLINE_IO_PENDING;
}

bool FOnlineSessionEOSCore::CancelFindSessions()
{
	uint32 Return = ONLINE_FAIL;
	if (m_CurrentSessionSearch.IsValid() && m_CurrentSessionSearch->SearchState == EOnlineAsyncTaskState::InProgress)
	{
		if (m_CurrentSessionSearch->bIsLanQuery)
		{
			check(m_LANSession);
			Return = ONLINE_SUCCESS;
			m_LANSession->StopLANSession();
			m_CurrentSessionSearch->SearchState = EOnlineAsyncTaskState::Failed;
			m_CurrentSessionSearch = nullptr;
		}
		else
		{
			Return = ONLINE_SUCCESS;
			m_CurrentSessionSearch->SearchState = EOnlineAsyncTaskState::Failed;
			m_CurrentSessionSearch = nullptr;
		}
	}
	else
	{
		UE_LOG_ONLINE_SESSION(Warning, TEXT("Can't cancel a search that isn't in progress"));
	}

	if (Return != ONLINE_IO_PENDING)
	{
		TriggerOnCancelFindSessionsCompleteDelegates(true);
	}

	return Return == ONLINE_SUCCESS || Return == ONLINE_IO_PENDING;
}

bool FOnlineSessionEOSCore::JoinSession(int32 PlayerNum, FName SessionName, const FOnlineSessionSearchResult& DesiredSession)
{
	uint32 Return = ONLINE_FAIL;
	FNamedOnlineSession* Session = GetNamedSession(SessionName);
	if (Session == nullptr)
	{
		Session = AddNamedSession(SessionName, DesiredSession.Session);
		Session->HostingPlayerNum = PlayerNum;

		if (!Session->SessionSettings.bIsLANMatch)
		{
			if (DesiredSession.Session.SessionInfo.IsValid())
			{
				const TSharedPtr<const FOnlineSessionInfoEOSCore> SearchSessionInfo = StaticCastSharedPtr<const FOnlineSessionInfoEOSCore>(DesiredSession.Session.SessionInfo);

				FOnlineSessionInfoEOSCore* NewSessionInfo = new FOnlineSessionInfoEOSCore(*SearchSessionInfo);
				Session->SessionInfo = MakeShareable(NewSessionInfo);

				Return = JoinEOSSession(PlayerNum, Session, &DesiredSession.Session);
			}
			else
			{
				UE_LOG_ONLINE_SESSION(Warning, TEXT("Invalid session info on search result"), *SessionName.ToString());
			}
		}
		else
		{
			FOnlineSessionInfoEOSCore* NewSessionInfo = new FOnlineSessionInfoEOSCore();
			Session->SessionInfo = MakeShareable(NewSessionInfo);

			Return = JoinLANSession(PlayerNum, Session, &DesiredSession.Session);
		}

		if (Return != ONLINE_IO_PENDING)
		{
			if (Return != ONLINE_SUCCESS)
			{
				RemoveNamedSession(SessionName);
			}
			else
			{
				RegisterLocalPlayers(Session);
			}
		}
	}
	else
	{
		UE_LOG_ONLINE_SESSION(Warning, TEXT("Session (%s) already exists, can't join twice"), *SessionName.ToString());
	}

	if (Return != ONLINE_IO_PENDING)
	{
		TriggerOnJoinSessionCompleteDelegates(SessionName, Return == ONLINE_SUCCESS ? EOnJoinSessionCompleteResult::Success : EOnJoinSessionCompleteResult::UnknownError);
	}

	return Return == ONLINE_SUCCESS || Return == ONLINE_IO_PENDING;
}

bool FOnlineSessionEOSCore::JoinSession(const FUniqueNetId& SearchingUserId, FName SessionName, const FOnlineSessionSearchResult& DesiredSession)
{
	return JoinSession(m_Subsystem->m_IdentityInterfacePtr->GetLocalUserNumFromUniqueNetId(SearchingUserId), SessionName, DesiredSession);
}

typedef TEOSCallback<EOS_Sessions_OnJoinSessionCallback, EOS_Sessions_JoinSessionCallbackInfo> FJoinSessionCallback;

uint32 FOnlineSessionEOSCore::JoinEOSSession(int32 PlayerNum, FNamedOnlineSession* Session, const FOnlineSession* SearchSession)
{
	if (!Session->SessionInfo.IsValid())
	{
		UE_LOG_ONLINE_SESSION(Error, TEXT("Session (%s) has invalid session info"), *Session->SessionName.ToString());
		return ONLINE_FAIL;
	}
	const EOS_ProductUserId ProductUserId = m_Subsystem->m_IdentityInterfacePtr->GetLocalProductUserId(PlayerNum);
	if (ProductUserId == nullptr)
	{
		UE_LOG_ONLINE_SESSION(Error, TEXT("Session (%s) invalid user id (%d)"), *Session->SessionName.ToString(), PlayerNum);
		return ONLINE_FAIL;
	}
	const TSharedPtr<FOnlineSessionInfoEOSCore> EOSSessionInfo = StaticCastSharedPtr<FOnlineSessionInfoEOSCore>(Session->SessionInfo);
#if UE_VERSION_OLDER_THAN(5,1,0)
	if (!EOSSessionInfo->m_SessionId.IsValid())
	{
		UE_LOG_ONLINE_SESSION(Error, TEXT("Session (%s) has invalid session id"), *Session->SessionName.ToString());
		return ONLINE_FAIL;
	}
#else
	if (!EOSSessionInfo->m_SessionId->IsValid())
	{
		UE_LOG_ONLINE_SESSION(Error, TEXT("Session (%s) has invalid session id"), *Session->SessionName.ToString());
		return ONLINE_FAIL;
	}
#endif

	const TSharedPtr<const FOnlineSessionInfoEOSCore> SearchSessionInfo = StaticCastSharedPtr<const FOnlineSessionInfoEOSCore>(SearchSession->SessionInfo);
	EOSSessionInfo->m_HostAddr = SearchSessionInfo->m_HostAddr->Clone();

	Session->SessionState = EOnlineSessionState::Pending;

	FJoinSessionCallback* CallbackObj = new FJoinSessionCallback();
	CallbackObj->m_CallbackLambda = [this, Session](const EOS_Sessions_JoinSessionCallbackInfo* Data)
	{
		const bool bWasSuccessful = Data->ResultCode == EOS_EResult::EOS_Success;
		if (bWasSuccessful)
		{
			BeginSessionAnalytics(Session);
		}
		else
		{
			UE_LOG_ONLINE_SESSION(Error, TEXT("EOS_Sessions_JoinSession() failed for session (%s) with EOS result code (%s)"), *Session->SessionName.ToString(), ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
		}
		TriggerOnJoinSessionCompleteDelegates(Session->SessionName, bWasSuccessful ? EOnJoinSessionCompleteResult::Success : EOnJoinSessionCompleteResult::UnknownError);
	};

	FJoinSessionOptions Options(TCHAR_TO_UTF8(*Session->SessionName.ToString()));
	Options.LocalUserId = ProductUserId;
	Options.SessionHandle = EOSSessionInfo->m_SessionHandle;
	EOS_Sessions_JoinSession(m_Subsystem->m_SessionsHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());

	return ONLINE_SUCCESS;
}

bool FOnlineSessionEOSCore::FindFriendSession(int32 LocalUserNum, const FUniqueNetId& Friend)
{
	const TArray<FOnlineSessionSearchResult> EmptySearchResult;
	TriggerOnFindFriendSessionCompleteDelegates(LocalUserNum, false, EmptySearchResult);
	return false;
};

bool FOnlineSessionEOSCore::FindFriendSession(const FUniqueNetId& LocalUserId, const FUniqueNetId& Friend)
{
	const TArray<FOnlineSessionSearchResult> EmptySearchResult;
	TriggerOnFindFriendSessionCompleteDelegates(m_Subsystem->m_IdentityInterfacePtr->GetLocalUserNumFromUniqueNetId(LocalUserId), false, EmptySearchResult);
	return false;
}

bool FOnlineSessionEOSCore::FindFriendSession(const FUniqueNetId& LocalUserId, const TArray<TSharedRef<const FUniqueNetId>>& FriendList)
{
	const TArray<FOnlineSessionSearchResult> EmptySearchResult;
	TriggerOnFindFriendSessionCompleteDelegates(m_Subsystem->m_IdentityInterfacePtr->GetLocalUserNumFromUniqueNetId(LocalUserId), false, EmptySearchResult);
	return false;
}

typedef TEOSCallback<EOS_Sessions_OnSendInviteCallback, EOS_Sessions_SendInviteCallbackInfo> FSendSessionInviteCallback;

bool FOnlineSessionEOSCore::SendSessionInvite(FName SessionName, EOS_ProductUserId SenderId, EOS_ProductUserId ReceiverId) const
{
	FSendSessionInviteOptions Options(TCHAR_TO_UTF8(*SessionName.ToString()));
	Options.LocalUserId = SenderId;
	Options.TargetUserId = ReceiverId;

	FSendSessionInviteCallback* CallbackObj = new FSendSessionInviteCallback();
	CallbackObj->m_CallbackLambda = [this, SessionName](const EOS_Sessions_SendInviteCallbackInfo* Data)
	{
		const bool bWasSuccessful = Data->ResultCode == EOS_EResult::EOS_Success;
		if (!bWasSuccessful)
		{
			UE_LOG_ONLINE_SESSION(Error, TEXT("SendSessionInvite() failed for session (%s) with EOS result code (%s)"), *SessionName.ToString(), ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
		}
	};

	EOS_Sessions_SendInvite(m_Subsystem->m_SessionsHandle, &Options, CallbackObj, CallbackObj->GetCallbackPtr());

	return true;
}

bool FOnlineSessionEOSCore::SendSessionInviteToFriend(int32 LocalUserNum, FName SessionName, const FUniqueNetId& Friend)
{
	const EOS_ProductUserId LocalUserId = m_Subsystem->m_IdentityInterfacePtr->GetLocalProductUserId(LocalUserNum);
	if (LocalUserId == nullptr)
	{
		UE_LOG_ONLINE_SESSION(Error, TEXT("SendSessionInviteToFriend() failed due to user (%d) being not logged in"), (int32)LocalUserNum);
		return false;
	}
	const EOS_ProductUserId TargetUserId = m_Subsystem->m_IdentityInterfacePtr->GetProductUserId(Friend);
	if (TargetUserId == nullptr)
	{
		UE_LOG_ONLINE_SESSION(Error, TEXT("SendSessionInviteToFriend() failed due to target user (%s) having not played this game"), *Friend.ToDebugString());
		return false;
	}

	return SendSessionInvite(SessionName, LocalUserId, TargetUserId);
};

bool FOnlineSessionEOSCore::SendSessionInviteToFriend(const FUniqueNetId& LocalNetId, FName SessionName, const FUniqueNetId& Friend)
{
	const EOS_ProductUserId LocalUserId = m_Subsystem->m_IdentityInterfacePtr->GetProductUserId(LocalNetId);
	if (LocalUserId == nullptr)
	{
		UE_LOG_ONLINE_SESSION(Error, TEXT("SendSessionInviteToFriend() failed due to user (%s) being not logged in"), *LocalNetId.ToDebugString());
		return false;
	}
	const EOS_ProductUserId TargetUserId = m_Subsystem->m_IdentityInterfacePtr->GetProductUserId(Friend);
	if (TargetUserId == nullptr)
	{
		UE_LOG_ONLINE_SESSION(Error, TEXT("SendSessionInviteToFriend() failed due to target user (%s) having not played this game"), *Friend.ToDebugString());
		return false;
	}

	return SendSessionInvite(SessionName, LocalUserId, TargetUserId);
}

bool FOnlineSessionEOSCore::SendSessionInviteToFriends(int32 LocalUserNum, FName SessionName, const TArray<TSharedRef<const FUniqueNetId>>& Friends)
{
	for (TSharedRef<const FUniqueNetId> NetId : Friends)
	{
		if (SendSessionInviteToFriend(LocalUserNum, SessionName, *NetId) == false)
		{
			return false;
		}
	}
	return true;
};

bool FOnlineSessionEOSCore::SendSessionInviteToFriends(const FUniqueNetId& LocalUserId, FName SessionName, const TArray<TSharedRef<const FUniqueNetId>>& Friends)
{
	for (TSharedRef<const FUniqueNetId> NetId : Friends)
	{
		if (SendSessionInviteToFriend(LocalUserId, SessionName, *NetId) == false)
		{
			return false;
		}
	}
	return true;
}

bool FOnlineSessionEOSCore::PingSearchResults(const FOnlineSessionSearchResult& SearchResult)
{
	return false;
}

/** Get a resolved connection string from a session info */
static bool GetConnectStringFromSessionInfo(const TSharedPtr<FOnlineSessionInfoEOSCore>& SessionInfo, FString& ConnectInfo, int32 PortOverride = 0)
{
	if (!SessionInfo.IsValid() || !SessionInfo->m_HostAddr.IsValid())
	{
		return false;
	}

	if (PortOverride != 0)
	{
		ConnectInfo = FString::Printf(TEXT("%s:%d"), *SessionInfo->m_HostAddr->ToString(false), PortOverride);
	}
	else if (SessionInfo->m_EOSAddress.Len() > 0)
	{
		ConnectInfo = SessionInfo->m_EOSAddress;
	}
	else
	{
		ConnectInfo = SessionInfo->m_HostAddr->ToString(true);
	}

	return true;
}

bool FOnlineSessionEOSCore::GetResolvedConnectString(FName SessionName, FString& ConnectInfo, FName PortType)
{
	bool bSuccess = false;
	FNamedOnlineSession* Session = GetNamedSession(SessionName);

	if (Session != nullptr)
	{
		FString CustomPort;
		const auto PortSetting = Session->SessionSettings.Settings.Find(TEXT("ServerPort"));
		if (PortSetting)
		{
			PortSetting->Data.GetValue(CustomPort);
		}

		TSharedPtr<FOnlineSessionInfoEOSCore> SessionInfo = StaticCastSharedPtr<FOnlineSessionInfoEOSCore>(Session->SessionInfo);
		if (PortType == NAME_BeaconPort)
		{
			const int32 BeaconListenPort = GetBeaconPortFromSessionSettings(Session->SessionSettings);
			bSuccess = GetConnectStringFromSessionInfo(SessionInfo, ConnectInfo, BeaconListenPort);
		}
		else if (PortType == NAME_GamePort)
		{
			if (CustomPort.Len() > 0)
			{
				UE_LOG_ONLINE_SESSION(Log, TEXT("%s"), *CustomPort);

				bSuccess = GetConnectStringFromSessionInfo(SessionInfo, ConnectInfo, FCString::Atoi(*CustomPort));
			}
			else
			{
				bSuccess = GetConnectStringFromSessionInfo(SessionInfo, ConnectInfo);
			}
		}

		if (!bSuccess)
		{
			UE_LOG_ONLINE_SESSION(Warning, TEXT("Invalid session info for session %s in GetResolvedConnectString()"), *SessionName.ToString());
		}
	}
	else
	{
		UE_LOG_ONLINE_SESSION(Warning,
		                      TEXT("Unknown session name (%s) specified to GetResolvedConnectString()"),
		                      *SessionName.ToString());
	}

	return bSuccess;
}

bool FOnlineSessionEOSCore::GetResolvedConnectString(const FOnlineSessionSearchResult& SearchResult, FName PortType, FString& ConnectInfo)
{
	bool bSuccess = false;
	if (SearchResult.Session.SessionInfo.IsValid())
	{
		// Try getting custom port settings
		FString CustomPort;
		const auto PortSetting = SearchResult.Session.SessionSettings.Settings.Find(TEXT("ServerPort"));
		if (PortSetting)
		{
			PortSetting->Data.GetValue(CustomPort);
		}

		TSharedPtr<FOnlineSessionInfoEOSCore> SessionInfo = StaticCastSharedPtr<FOnlineSessionInfoEOSCore>(SearchResult.Session.SessionInfo);

		if (PortType == NAME_BeaconPort)
		{
			const int32 BeaconListenPort = GetBeaconPortFromSessionSettings(SearchResult.Session.SessionSettings);
			bSuccess = GetConnectStringFromSessionInfo(SessionInfo, ConnectInfo, BeaconListenPort);
		}
		else if (PortType == NAME_GamePort)
		{
			if (CustomPort.Len() > 0)
			{
				UE_LOG_ONLINE_SESSION(Log, TEXT("%s"), *CustomPort);

				bSuccess = GetConnectStringFromSessionInfo(SessionInfo, ConnectInfo, FCString::Atoi(*CustomPort));
			}
			else
			{
				bSuccess = GetConnectStringFromSessionInfo(SessionInfo, ConnectInfo);
			}
		}
	}

	if (!bSuccess || ConnectInfo.IsEmpty())
	{
		UE_LOG_ONLINE_SESSION(Warning, TEXT("Invalid session info in search result to GetResolvedConnectString()"));
	}

	return bSuccess;
}

FOnlineSessionSettings* FOnlineSessionEOSCore::GetSessionSettings(FName SessionName)
{
	FNamedOnlineSession* Session = GetNamedSession(SessionName);
	if (Session)
	{
		return &Session->SessionSettings;
	}
	return nullptr;
}

void FOnlineSessionEOSCore::RegisterLocalPlayers(FNamedOnlineSession* Session)
{
	LogVerbose("");
	
	if (!m_Subsystem->IsDedicated())
	{
		IOnlineVoicePtr VoiceInt = m_Subsystem->GetVoiceInterface();
		if (VoiceInt.IsValid())
		{
			for (int32 Index = 0; Index < MAX_LOCAL_PLAYERS; Index++)
			{
				// Register the local player as a local talker
				VoiceInt->RegisterLocalTalker(Index);
			}
		}
	}
}

bool FOnlineSessionEOSCore::RegisterPlayer(FName SessionName, const FUniqueNetId& PlayerId, bool bWasInvited)
{
	TArray<TSharedRef<const FUniqueNetId>> Players;
	Players.Add(MakeShareable(new FUniqueNetIdEOSCore(PlayerId)));
	return RegisterPlayers(SessionName, Players, bWasInvited);
}

bool FOnlineSessionEOSCore::RegisterPlayers(FName SessionName, const TArray<TSharedRef<const FUniqueNetId>>& Players, bool bWasInvited)
{
	LogVerbose("");
	
	bool bSuccess = false;
	FNamedOnlineSession* Session = GetNamedSession(SessionName);
	TArray<EOS_ProductUserId> PlayersToRegister;
	if (Session)
	{
		bSuccess = true;

		for (int32 PlayerIdx = 0; PlayerIdx < Players.Num(); PlayerIdx++)
		{
			const TSharedRef<const FUniqueNetId>& PlayerId = Players[PlayerIdx];

			const FUniqueNetIdMatcher PlayerMatchId(*PlayerId);
			
			if (Session->RegisteredPlayers.IndexOfByPredicate(PlayerMatchId) == INDEX_NONE)
			{
				Session->RegisteredPlayers.Add(PlayerId);

				const TSharedPtr<FUniqueNetIdEOSCore> NetId = MakeShareable(new FUniqueNetIdEOSCore(Players[PlayerIdx]->ToString()));
				PlayersToRegister.Add(EOS_ProductUserId_FromString(TCHAR_TO_UTF8(*NetId->m_ProductUserIdStr)));

				if (Session->NumOpenPublicConnections > 0)
				{
					Session->NumOpenPublicConnections--;
				}
				else if (Session->NumOpenPrivateConnections > 0)
				{
					Session->NumOpenPrivateConnections--;
				}
			}
			else
			{
				UE_LOG_ONLINE_SESSION(Log, TEXT("Player %s already registered in session %s"), *PlayerId->ToDebugString(), *SessionName.ToString());
			}

			RegisterVoice(*PlayerId);
		}
	}
	else
	{
		UE_LOG_ONLINE_SESSION(Warning, TEXT("No game present to join for session (%s)"), *SessionName.ToString());
	}

	auto m_Callback = [](const EOS_Sessions_RegisterPlayersCallbackInfo* Data)
	{
		UE_LOG_ONLINE_SESSION(Log, TEXT("EOS_Sessions_RegisterPlayers() Code: (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
	};

	const FTCHARToUTF8 ConvertedSessionName(*SessionName.ToString());
	
	EOS_Sessions_RegisterPlayersOptions RegisterPlayersOptions = {};
	RegisterPlayersOptions.ApiVersion = EOS_SESSIONS_REGISTERPLAYERS_API_LATEST;
 	RegisterPlayersOptions.SessionName =  ConvertedSessionName.Get();
	RegisterPlayersOptions.PlayersToRegister = PlayersToRegister.GetData();
	RegisterPlayersOptions.PlayersToRegisterCount = PlayersToRegister.Num();
	
	EOS_Sessions_RegisterPlayers(m_Subsystem->m_SessionsHandle, &RegisterPlayersOptions, nullptr, m_Callback);

	TriggerOnRegisterPlayersCompleteDelegates(SessionName, Players, bSuccess);
	return bSuccess;
}

bool FOnlineSessionEOSCore::UnregisterPlayer(FName SessionName, const FUniqueNetId& PlayerId)
{
	TArray<TSharedRef<const FUniqueNetId>> Players;
	Players.Add(MakeShareable(new FUniqueNetIdEOSCore(PlayerId)));
	return UnregisterPlayers(SessionName, Players);
}

bool FOnlineSessionEOSCore::UnregisterPlayers(FName SessionName, const TArray<TSharedRef<const FUniqueNetId>>& Players)
{
	LogVerbose("");
	
	bool bSuccess = true;

	FNamedOnlineSession* Session = GetNamedSession(SessionName);
	TArray<EOS_ProductUserId> PlayersToRegister;

	if (Session)
	{
		for (int32 PlayerIdx = 0; PlayerIdx < Players.Num(); PlayerIdx++)
		{
			const TSharedRef<const FUniqueNetId>& PlayerId = Players[PlayerIdx];
			const FUniqueNetIdMatcher PlayerMatchId(*PlayerId);
			int32 RegistrantIndex = Session->RegisteredPlayers.IndexOfByPredicate(PlayerMatchId);
			
			if (RegistrantIndex != INDEX_NONE)
			{
				Session->RegisteredPlayers.RemoveAtSwap(RegistrantIndex);
				TSharedRef<FUniqueNetIdEOSCore> NetId = MakeShareable(new FUniqueNetIdEOSCore(Players[PlayerIdx]->ToString()));

				PlayersToRegister.Add(EOS_ProductUserId_FromString(TCHAR_TO_UTF8(*NetId->m_ProductUserIdStr)));

				if (Session->NumOpenPublicConnections < Session->SessionSettings.NumPublicConnections)
				{
					Session->NumOpenPublicConnections++;
				}
				else if (Session->NumOpenPrivateConnections < Session->SessionSettings.NumPrivateConnections)
				{
					Session->NumOpenPrivateConnections++;
				}
				
				UnregisterVoice(*PlayerId);
			}
			else
			{
				UE_LOG_ONLINE_SESSION(Warning, TEXT("Player %s is not part of session (%s)"), *PlayerId->ToDebugString(), *SessionName.ToString());
			}
		}
	}
	else
	{
		UE_LOG_ONLINE_SESSION(Warning, TEXT("No game present to leave for session (%s)"), *SessionName.ToString());
		bSuccess = false;
	}

	auto Callback = [](const EOS_Sessions_UnregisterPlayersCallbackInfo* Data)
	{
		UE_LOG_ONLINE_SESSION(Log, TEXT("EOS_Sessions_UnregisterPlayers() Code: (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(Data->ResultCode)));
	};

	const FTCHARToUTF8 ConvertedSessionName(*SessionName.ToString());
	
	EOS_Sessions_UnregisterPlayersOptions RegisterPlayersOptions = {};
	RegisterPlayersOptions.ApiVersion = EOS_SESSIONS_UNREGISTERPLAYERS_API_LATEST;
	RegisterPlayersOptions.SessionName = ConvertedSessionName.Get();
	RegisterPlayersOptions.PlayersToUnregister = PlayersToRegister.GetData();
	RegisterPlayersOptions.PlayersToUnregisterCount = PlayersToRegister.Num();

	EOS_Sessions_UnregisterPlayers(m_Subsystem->m_SessionsHandle, &RegisterPlayersOptions, nullptr, Callback);

	TriggerOnUnregisterPlayersCompleteDelegates(SessionName, Players, bSuccess);
	return bSuccess;
}

void FOnlineSessionEOSCore::Tick(float DeltaTime)
{
	SCOPE_CYCLE_COUNTER(STAT_Session_Interface);
	TickLAN(DeltaTime);
}

void FOnlineSessionEOSCore::TickLAN(float DeltaTime) const
{
	if (m_LANSession.IsValid() &&
		m_LANSession->GetBeaconState() > ELanBeaconState::NotUsingLanBeacon)
	{
		m_LANSession->Tick(DeltaTime);
	}
}

void FOnlineSessionEOSCore::AppendSessionToPacket(FNboSerializeToBufferEOSCore& Packet, const FOnlineSession* Session) const
{
	((FNboSerializeToBuffer&)Packet) << Session->OwningUserId->ToString()
		<< Session->OwningUserName
		<< Session->NumOpenPrivateConnections
		<< Session->NumOpenPublicConnections;

	SetPortFromNetDriver(*m_Subsystem, Session->SessionInfo);

	Packet << *StaticCastSharedPtr<FOnlineSessionInfoEOSCore>(Session->SessionInfo);

	AppendSessionSettingsToPacket(Packet, &Session->SessionSettings);
}

void FOnlineSessionEOSCore::AppendSessionSettingsToPacket(FNboSerializeToBufferEOSCore& Packet, const FOnlineSessionSettings* SessionSettings)
{
#if DEBUG_LAN_BEACON
	UE_LOG_ONLINE_SESSION(Verbose, TEXT("Sending session settings to client"));
#endif

	((FNboSerializeToBuffer&)Packet) << SessionSettings->NumPublicConnections
		<< SessionSettings->NumPrivateConnections
		<< (uint8)SessionSettings->bShouldAdvertise
		<< (uint8)SessionSettings->bIsLANMatch
		<< (uint8)SessionSettings->bIsDedicated
		<< (uint8)SessionSettings->bUsesStats
		<< (uint8)SessionSettings->bAllowJoinInProgress
		<< (uint8)SessionSettings->bAllowInvites
		<< (uint8)SessionSettings->bUsesPresence
		<< (uint8)SessionSettings->bAllowJoinViaPresence
		<< (uint8)SessionSettings->bAllowJoinViaPresenceFriendsOnly
		<< (uint8)SessionSettings->bAntiCheatProtected
		<< SessionSettings->BuildUniqueId;

	int32 NumAdvertisedProperties = 0;
	for (FSessionSettings::TConstIterator It(SessionSettings->Settings); It; ++It)
	{
		const FOnlineSessionSetting& Setting = It.Value();
		if (Setting.AdvertisementType >= EOnlineDataAdvertisementType::ViaOnlineService)
		{
			NumAdvertisedProperties++;
		}
	}

	((FNboSerializeToBuffer&)Packet) << (int32)NumAdvertisedProperties;
	for (FSessionSettings::TConstIterator It(SessionSettings->Settings); It; ++It)
	{
		const FOnlineSessionSetting& Setting = It.Value();
		if (Setting.AdvertisementType >= EOnlineDataAdvertisementType::ViaOnlineService)
		{
			((FNboSerializeToBuffer&)Packet) << It.Key();
			Packet << Setting;
#if DEBUG_LAN_BEACON
			UE_LOG_ONLINE_SESSION(Verbose, TEXT("%s"), *Setting.ToString());
#endif
		}
	}
}

void FOnlineSessionEOSCore::OnValidQueryPacketReceived(uint8* PacketData, int32 PacketLength, uint64 ClientNonce)
{
	FScopeLock ScopeLock(&m_SessionLock);
	for (int32 SessionIndex = 0; SessionIndex < m_Sessions.Num(); SessionIndex++)
	{
		FNamedOnlineSession* Session = &m_Sessions[SessionIndex];

		if (Session != nullptr)
		{
			const FOnlineSessionSettings& Settings = Session->SessionSettings;

			const bool bIsMatchInProgress = Session->SessionState == EOnlineSessionState::InProgress;

			const bool bIsMatchJoinable = Settings.bIsLANMatch &&
				(!bIsMatchInProgress || Settings.bAllowJoinInProgress) &&
				Settings.NumPublicConnections > 0;

			if (bIsMatchJoinable)
			{
				FNboSerializeToBufferEOSCore Packet(LAN_BEACON_MAX_PACKET_SIZE);
				m_LANSession->CreateHostResponsePacket(Packet, ClientNonce);
				AppendSessionToPacket(Packet, Session);
				m_LANSession->BroadcastPacket(Packet, Packet.GetByteCount());
			}
		}
	}
}

void FOnlineSessionEOSCore::ReadSessionFromPacket(FNboSerializeFromBufferEOSCore& Packet, FOnlineSession* Session)
{
#if DEBUG_LAN_BEACON
	UE_LOG_ONLINE_SESSION(Verbose, TEXT("Reading session information from server"));
#endif

	/** Owner of the session */
	FString OwningUserIdStr;
	Packet >> OwningUserIdStr
		>> Session->OwningUserName
		>> Session->NumOpenPrivateConnections
		>> Session->NumOpenPublicConnections;

	FUniqueNetIdEOSCore* UniqueId = new FUniqueNetIdEOSCore(OwningUserIdStr);
	
	Session->OwningUserId = MakeShareable(UniqueId);

	FOnlineSessionInfoEOSCore* EOSSessionInfo = new FOnlineSessionInfoEOSCore();
	EOSSessionInfo->m_HostAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	Packet >> *EOSSessionInfo;
	Session->SessionInfo = MakeShareable(EOSSessionInfo);

	ReadSettingsFromPacket(Packet, Session->SessionSettings);
}

void FOnlineSessionEOSCore::ReadSettingsFromPacket(FNboSerializeFromBufferEOSCore& Packet, FOnlineSessionSettings& SessionSettings)
{
#if DEBUG_LAN_BEACON
	UE_LOG_ONLINE_SESSION(Verbose, TEXT("Reading game settings from server"));
#endif

	SessionSettings.Settings.Empty();

	Packet >> SessionSettings.NumPublicConnections
		>> SessionSettings.NumPrivateConnections;
	uint8 Read = 0;
	Packet >> Read;
	SessionSettings.bShouldAdvertise = !!Read;
	Packet >> Read;
	SessionSettings.bIsLANMatch = !!Read;
	Packet >> Read;
	SessionSettings.bIsDedicated = !!Read;
	Packet >> Read;
	SessionSettings.bUsesStats = !!Read;
	Packet >> Read;
	SessionSettings.bAllowJoinInProgress = !!Read;
	Packet >> Read;
	SessionSettings.bAllowInvites = !!Read;
	Packet >> Read;
	SessionSettings.bUsesPresence = !!Read;
	Packet >> Read;
	SessionSettings.bAllowJoinViaPresence = !!Read;
	Packet >> Read;
	SessionSettings.bAllowJoinViaPresenceFriendsOnly = !!Read;
	Packet >> Read;
	SessionSettings.bAntiCheatProtected = !!Read;

	Packet >> SessionSettings.BuildUniqueId;

	int32 NumAdvertisedProperties = 0;
	Packet >> NumAdvertisedProperties;
	if (Packet.HasOverflow() == false)
	{
		FName Key;

		for (int32 Index = 0;
		     Index < NumAdvertisedProperties && Packet.HasOverflow() == false;
		     Index++)
		{
			FOnlineSessionSetting Setting;
			Packet >> Key;
			Packet >> Setting;
			SessionSettings.Set(Key, Setting);

#if DEBUG_LAN_BEACON
			UE_LOG_ONLINE_SESSION(Verbose, TEXT("%s"), *Setting->ToString());
#endif
		}
	}

	if (Packet.HasOverflow())
	{
		SessionSettings.Settings.Empty();
		UE_LOG_ONLINE_SESSION(Verbose, TEXT("Packet overflow detected in ReadGameSettingsFromPacket()"));
	}
}

void FOnlineSessionEOSCore::OnValidResponsePacketReceived(uint8* PacketData, int32 PacketLength)
{
	FOnlineSessionSettings NewServer;
	if (m_CurrentSessionSearch.IsValid())
	{
		FOnlineSessionSearchResult* NewResult = new(m_CurrentSessionSearch->SearchResults) FOnlineSessionSearchResult();
		NewResult->PingInMs = static_cast<int32>((FPlatformTime::Seconds() - m_SessionSearchStartInSeconds) * 1000);

		FOnlineSession* NewSession = &NewResult->Session;

		FNboSerializeFromBufferEOSCore Packet(PacketData, PacketLength);

		ReadSessionFromPacket(Packet, NewSession);
	}
	else
	{
		UE_LOG_ONLINE_SESSION(Warning, TEXT("Failed to create new online game settings object"));
	}
}

int32 FOnlineSessionEOSCore::GetNumSessions()
{
	FScopeLock ScopeLock(&m_SessionLock);
	return m_Sessions.Num();
}

void FOnlineSessionEOSCore::DumpSessionState()
{
	FScopeLock ScopeLock(&m_SessionLock);

	for (int32 SessionIdx = 0; SessionIdx < m_Sessions.Num(); SessionIdx++)
	{
		DumpNamedSession(&m_Sessions[SessionIdx]);
	}
}

void FOnlineSessionEOSCore::RegisterLocalPlayer(const FUniqueNetId& PlayerId, FName SessionName, const FOnRegisterLocalPlayerCompleteDelegate& Delegate)
{
	Delegate.ExecuteIfBound(PlayerId, EOnJoinSessionCompleteResult::Success);
}

void FOnlineSessionEOSCore::UnregisterLocalPlayer(const FUniqueNetId& PlayerId, FName SessionName, const FOnUnregisterLocalPlayerCompleteDelegate& Delegate)
{
	Delegate.ExecuteIfBound(PlayerId, true);
}

void FOnlineSessionEOSCore::SetPortFromNetDriver(const FOnlineSubsystemEOSCore& Subsystem, const TSharedPtr<FOnlineSessionInfo>& SessionInfo)
{
	const auto NetDriverPort = GetPortFromNetDriver(Subsystem.GetInstanceName());
	const auto SessionInfoEOS = StaticCastSharedPtr<FOnlineSessionInfoEOSCore>(SessionInfo);
	if (SessionInfoEOS.IsValid() && SessionInfoEOS->m_HostAddr.IsValid())
	{
		SessionInfoEOS->m_HostAddr->SetPort(NetDriverPort);
	}
}

bool FOnlineSessionEOSCore::IsHost(const FNamedOnlineSession& Session) const
{
	if (m_Subsystem->IsDedicated())
	{
		return true;
	}

	TSharedPtr<const FUniqueNetId> UserId = m_Subsystem->m_IdentityInterfacePtr->GetUniquePlayerId(Session.HostingPlayerNum);
	return (UserId.IsValid() && (*UserId == *Session.OwningUserId));
}

void FOnlineSessionEOSCore::RegisterVoice(const FUniqueNetId& PlayerId)
{
	LogVerbose("%s", *PlayerId.ToString());
	
	if (!m_Subsystem->IsDedicated())
	{
		if (PlayerId.IsValid())
		{
			const IOnlineVoicePtr VoiceInt = m_Subsystem->GetVoiceInterface();
			
			if (VoiceInt.IsValid())
			{
				if (!m_Subsystem->IsLocalPlayer(PlayerId))
				{
					VoiceInt->RegisterRemoteTalker(PlayerId);
				}
				else
				{
					VoiceInt->ProcessMuteChangeNotification();
				}
			}
		}
	}
}

void FOnlineSessionEOSCore::UnregisterVoice(const FUniqueNetId& PlayerId)
{
	LogVerbose("%s", *PlayerId.ToString());
	
	if (!m_Subsystem->IsDedicated())
	{
		if (PlayerId.IsValid())
		{
			const IOnlineVoicePtr VoiceInt = m_Subsystem->GetVoiceInterface();
			if (PlayerId.IsValid() && !m_Subsystem->IsLocalPlayer(PlayerId) && VoiceInt)
			{
				if (!m_Subsystem->IsLocalPlayer(PlayerId))
				{
					VoiceInt->UnregisterRemoteTalker(PlayerId);
				}
			}
		}
	}
}

void FOnlineSessionEOSCore::DestroyLocalVOIPComponents()
{
	LogVerbose("");
	
	if (UWorld* World = GetWorldForOnline(m_Subsystem->GetInstanceName()))
	{
		for (TObjectIterator<UVoipListenerSynthComponent> It; It; ++It)
		{
			UVoipListenerSynthComponent* VoipSynthComp = Cast<UVoipListenerSynthComponent>(*It);
			if (!VoipSynthComp || VoipSynthComp->GetWorld() != World)
			{
				continue;
			}

			VoipSynthComp->DestroyComponent();
		}
	}
}

TSharedPtr<const FUniqueNetId> FOnlineSessionEOSCore::CreateSessionIdFromString(const FString& SessionIdStr)
{
	TSharedPtr<const FUniqueNetId> SessionId;
	if (!SessionIdStr.IsEmpty())
	{
#if UE_VERSION_OLDER_THAN(5,1,0)
		SessionId = MakeShared<FUniqueNetIdString>(SessionIdStr);
#else
		SessionId = FUniqueNetIdString::Create(SessionIdStr, FName("EOS"));
#endif
	}
	return SessionId;
}

uint32 FOnlineSessionEOSCore::CreateLANSession(int32 HostingPlayerNum, FNamedOnlineSession* Session)
{
	check(Session);
	uint32 Result = ONLINE_SUCCESS;

	FOnlineSessionInfoEOSCore* NewSessionInfo = new FOnlineSessionInfoEOSCore();
	NewSessionInfo->InitLAN(m_Subsystem);
	Session->SessionInfo = MakeShareable(NewSessionInfo);

	if (Session->SessionSettings.bShouldAdvertise)
	{
		if (!m_LANSession.IsValid())
		{
			m_LANSession = MakeShareable(new FLANSession());
		}

		FOnValidQueryPacketDelegate QueryPacketDelegate = FOnValidQueryPacketDelegate::CreateRaw(this, &FOnlineSessionEOSCore::OnValidQueryPacketReceived);
		if (!m_LANSession->Host(QueryPacketDelegate))
		{
			Result = ONLINE_FAIL;
		}
	}

	return Result;
}

uint32 FOnlineSessionEOSCore::JoinLANSession(int32 PlayerNum, FNamedOnlineSession* Session, const FOnlineSession* SearchSession)
{
	uint32 Result = ONLINE_FAIL;
	Session->SessionState = EOnlineSessionState::Pending;

	if (Session->SessionInfo.IsValid())
	{
		TSharedPtr<const FOnlineSessionInfoEOSCore> SearchSessionInfo = StaticCastSharedPtr<const FOnlineSessionInfoEOSCore>(SearchSession->SessionInfo);
		TSharedPtr<FOnlineSessionInfoEOSCore> SessionInfo = StaticCastSharedPtr<FOnlineSessionInfoEOSCore>(Session->SessionInfo);
		SessionInfo->m_HostAddr = SearchSessionInfo->m_HostAddr->Clone();
		Result = ONLINE_SUCCESS;
	}
	else
	{
		UE_LOG_ONLINE_SESSION(Warning, TEXT("Session (%s) has invalid session info"), *Session->SessionName.ToString());
	}

	return Result;
}

uint32 FOnlineSessionEOSCore::FindLANSession()
{
	uint32 Return = ONLINE_FAIL;

	if (!m_LANSession.IsValid())
	{
		m_LANSession = MakeShareable(new FLANSession());
	}

	GenerateNonce((uint8*)&m_LANSession->LanNonce, 8);

	FOnValidResponsePacketDelegate ResponseDelegate = FOnValidResponsePacketDelegate::CreateRaw(this, &FOnlineSessionEOSCore::OnValidResponsePacketReceived);
	FOnSearchingTimeoutDelegate TimeoutDelegate = FOnSearchingTimeoutDelegate::CreateRaw(this, &FOnlineSessionEOSCore::OnSearchingTimeout);

	FNboSerializeToBufferEOSCore Packet(LAN_BEACON_MAX_PACKET_SIZE);
	m_LANSession->CreateClientQueryPacket(Packet, m_LANSession->LanNonce);
	if (m_LANSession->Search(Packet, ResponseDelegate, TimeoutDelegate))
	{
		Return = ONLINE_IO_PENDING;
	}

	if (Return == ONLINE_FAIL)
	{
		m_CurrentSessionSearch->SearchState = EOnlineAsyncTaskState::Failed;

		TriggerOnFindSessionsCompleteDelegates(false);
	}

	return Return;
}

void FOnlineSessionEOSCore::OnSearchingTimeout()
{
	bool bWasHosting = false;

	{
		FScopeLock ScopeLock(&m_SessionLock);
		for (int32 SessionIdx = 0; SessionIdx < m_Sessions.Num(); SessionIdx++)
		{
			FNamedOnlineSession& Session = m_Sessions[SessionIdx];
			if (Session.SessionSettings.bShouldAdvertise &&
				Session.SessionSettings.bIsLANMatch &&
				m_Subsystem->IsServer())
			{
				bWasHosting = true;
				break;
			}
		}
	}

	if (bWasHosting)
	{
		FOnValidQueryPacketDelegate QueryPacketDelegate = FOnValidQueryPacketDelegate::CreateRaw(this, &FOnlineSessionEOSCore::OnValidQueryPacketReceived);

		if (m_LANSession->Host(QueryPacketDelegate))
		{
			UE_LOG_ONLINE_SESSION(Warning, TEXT("Failed to restart hosted LAN session after search completion"));
		}
	}
	else
	{
		m_LANSession->StopLANSession();
	}

	if (m_CurrentSessionSearch.IsValid())
	{
		if (m_CurrentSessionSearch->SearchResults.Num() > 0)
		{
			m_CurrentSessionSearch->SortSearchResults();
		}

		m_CurrentSessionSearch->SearchState = EOnlineAsyncTaskState::Done;

		m_CurrentSessionSearch = nullptr;
	}

	TriggerOnFindSessionsCompleteDelegates(true);
}