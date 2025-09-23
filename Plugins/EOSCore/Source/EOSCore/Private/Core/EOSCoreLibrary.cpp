/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "Core/EOSCoreLibrary.h"
#include "EOSCoreModule.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLogging.h"
#include <GameFramework/PlayerController.h>
#include <Engine/LocalPlayer.h>

#include "OnlineIdentityEOSCore.h"
#include "Commandlets/Commandlet.h"

static FDelegateHandle DelegateHandle;

bool UEOSCoreLibrary::EOS_Initialized(UObject* WorldContextObject)
{
	LogVeryVerbose("");
	
	FOnlineSubsystemEOSCore* Subsystem = static_cast<FOnlineSubsystemEOSCore*>(Online::GetSubsystem(GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull), EOSCORE_SUBSYSTEM));
	
	if (Subsystem)
	{
		return Subsystem->m_EOSPlatformHandle != nullptr;
	}

	return false;
}

bool UEOSCoreLibrary::UpdateUniqueNetIdFromOSS(APlayerController* PlayerController)
{
	LogVerbose("");

	bool bResult = false;

	if (PlayerController)
	{
		if (PlayerController->PlayerState)
		{
			FOnlineSubsystemEOSCore* Subsystem = static_cast<FOnlineSubsystemEOSCore*>(Online::GetSubsystem(GEngine->GetWorldFromContextObject(PlayerController, EGetWorldErrorMode::ReturnNull), EOSCORE_SUBSYSTEM));

			if (Subsystem && Subsystem->GetIdentityInterface())
			{
				int32 ControllerId = PlayerController->GetLocalPlayer()->GetControllerId();

				LogVerbose("PlayerId: %d", ControllerId);

				if (ControllerId != INDEX_NONE)
				{
					if (TSharedPtr<const FUniqueNetId> NetId = Subsystem->GetIdentityInterface()->GetUniquePlayerId(ControllerId))
					{
#if UE_VERSION_NEWER_THAN(4,27,2)
						const FUniqueNetIdRepl UserIdRepl(NetId);
						
						PlayerController->PlayerState->SetUniqueId(UserIdRepl);
						PlayerController->GetLocalPlayer()->SetCachedUniqueNetId(UserIdRepl);
#else
						PlayerController->PlayerState->SetUniqueId(NetId);
						PlayerController->GetLocalPlayer()->SetCachedUniqueNetId(NetId);
#endif

						bResult = true;
					}
					else
					{
						LogError("No NetId found for player %d", ControllerId);
					}
				}
				else
				{
					LogError("Invalid Player Controller Id %d", ControllerId);
				}
			}
			else
			{
				LogError("No EOSCore subsystem found || Invalid identity interface");
			}
		}
		else
		{
			LogError("Invalid player state");
		}
	}
	else
	{
		LogError("Invalid player controller");
	}

	return bResult;
}

void UEOSCoreLibrary::ListenForEOSMessages(UObject* WorldContextObject, const FOnEOSLogging& Callback)
{
	LogVerbose("");
	
	FOnlineSubsystemEOSCore* Subsystem = static_cast<FOnlineSubsystemEOSCore*>(Online::GetSubsystem(GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull), EOSCORE_SUBSYSTEM));

	if (Subsystem)
	{
		Subsystem->s_OnEOSCoreLog.BindLambda([Callback](const EOS_LogMessage* Message)
		{
			EEOSLogLevel EOSLogLevel = EEOSLogLevel::Off;
			
			switch (Message->Level) {
			case EOS_ELogLevel::EOS_LOG_Fatal:
				EOSLogLevel = EEOSLogLevel::Fatal;
				break;
			case EOS_ELogLevel::EOS_LOG_Error:
				EOSLogLevel = EEOSLogLevel::Error;
				break;
			case EOS_ELogLevel::EOS_LOG_Warning:
				EOSLogLevel = EEOSLogLevel::Warning;
				break;
			case EOS_ELogLevel::EOS_LOG_Info:
				EOSLogLevel = EEOSLogLevel::Info;
				break;
			case EOS_ELogLevel::EOS_LOG_Verbose:
				EOSLogLevel = EEOSLogLevel::Verbose;
				break;
			case EOS_ELogLevel::EOS_LOG_VeryVerbose:
				EOSLogLevel = EEOSLogLevel::VeryVerbose; 
				break;
			default: ;
			}
		
			Callback.ExecuteIfBound(ANSI_TO_TCHAR(Message->Category), ANSI_TO_TCHAR(Message->Message), EOSLogLevel);
		});
	}
}

void UEOSCoreLibrary::RemoveListenForEOSMessages(UObject* WorldContextObject)
{
	LogVerbose("");
	
	FOnlineSubsystemEOSCore* Subsystem = static_cast<FOnlineSubsystemEOSCore*>(Online::GetSubsystem(GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull), EOSCORE_SUBSYSTEM));

	if (Subsystem)
	{
		Subsystem->s_OnEOSCoreLog.Unbind();
	}
}

void UEOSCoreLibrary::TestRefreshConnectLogin(UObject* WorldContextObject)
{
	FOnlineSubsystemEOSCore* Subsystem = static_cast<FOnlineSubsystemEOSCore*>(Online::GetSubsystem(GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull), EOSCORE_SUBSYSTEM));

	if (Subsystem)
	{
		auto IdentityInterface = Subsystem->GetIdentityInterface();

		if (FOnlineIdentityEOSCore* OnlineIdentityEosCore = reinterpret_cast<FOnlineIdentityEOSCore*>(IdentityInterface.Get()))
		{
			OnlineIdentityEosCore->RefreshConnectLogin(0);
		}
	}
}

void UEOSCoreLibrary::Login(UObject* WorldContextObject, APlayerController* PlayerController, FString LoginId, FString Password, EEOSLoginCredentialType AuthType, EEOSEExternalCredentialType CredentialsType, FString AdditionalData, const FEOSLoginCallback& Callback)
{
	LogVerbose("");
	
	FString ErrorMessage;
	
	if (!WorldContextObject)
	{
		LogError("No world context object found!");
		ErrorMessage = "No world context object found!";
	}

	if (!PlayerController)
	{
		LogWarning("No player controller specified, will be unable to update NetId");
	}

	FOnlineSubsystemEOSCore* Subsystem = static_cast<FOnlineSubsystemEOSCore*>(Online::GetSubsystem(GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull), EOSCORE_SUBSYSTEM));

	if (!Subsystem)
	{
		LogError("EOS Subsystem not found");
		ErrorMessage = "EOS Subsystem not found!";
	}

	if (!Subsystem->GetIdentityInterface())
	{
		LogError("Subsystem IdentityInterface not valid");
		ErrorMessage = "Subsystem IdentityInterface not valid!";
	}

	if (!ErrorMessage.IsEmpty())
	{
		Callback.ExecuteIfBound(false, ErrorMessage);
		return;
	}

	int32 ControllerId = 0;

	if (PlayerController)
	{
		ControllerId = PlayerController->GetLocalPlayer()->GetControllerId();

		DelegateHandle = Subsystem->GetIdentityInterface()->AddOnLoginCompleteDelegate_Handle(ControllerId, FOnLoginCompleteDelegate::CreateWeakLambda(WorldContextObject, [=](int32 Unused, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& ErrorStr)
		{
			check(PlayerController);
			check(Subsystem);
			check(Subsystem->GetIdentityInterface());

			if (PlayerController)
			{
				if (bWasSuccessful)
				{
					if (PlayerController && PlayerController->PlayerState)
					{
						const FUniqueNetIdRepl UserIdRepl(UserId);
						PlayerController->PlayerState->SetUniqueId(UserIdRepl);
#if UE_VERSION_NEWER_THAN(4,27,2)
						PlayerController->GetLocalPlayer()->SetCachedUniqueNetId(UserIdRepl);
#else
						PlayerController->GetLocalPlayer()->SetCachedUniqueNetId(UserId.AsShared());
#endif
					}
				}
			}

			if (Subsystem)
			{
				Subsystem->ExecuteNextTick([Subsystem, ControllerId, Callback, bWasSuccessful, ErrorStr]()
				{
					Subsystem->GetIdentityInterface()->ClearOnLoginCompleteDelegate_Handle(ControllerId, DelegateHandle);
					Callback.ExecuteIfBound(bWasSuccessful, ErrorStr);
				});
			}
		}));
	}

	const FString Type = FString::Printf(TEXT("%d|%d|%s"), static_cast<int32>(AuthType), static_cast<int32>(CredentialsType), *AdditionalData);
	const FOnlineAccountCredentials Creds(Type, LoginId, Password);

	Subsystem->GetIdentityInterface()->Login(ControllerId, Creds);
}

FDateTime UEOSCoreLibrary::FromUnixTimestamp(FString Timestamp)
{
	return FDateTime::FromUnixTimestamp(FCString::Atoi64(*Timestamp));
}

void UEOSCoreLibrary::BreakUIEventIdStruct(FEOSUIEventId eventId, FString& outEventId)
{
	outEventId = LexToString(eventId.EventId);
}

void UEOSCoreLibrary::EOS_Success(EOSResult status, ESuccessFail& result)
{
	if (status == EOSResult::EOS_Success)
		result = ESuccessFail::Success;
		else
		result = ESuccessFail::Fail;
}

bool UEOSCoreLibrary::IsProductUserIdIdenticalWith(FEOSProductUserId a, FEOSProductUserId b)
{
	if (EOSProductUserIdIsValid(a) && EOSProductUserIdIsValid(b))
	{
		FString aString;
		FString bString;

		EOSProductUserIdToString(a, aString);
		EOSProductUserIdToString(b, bString);

		return aString == bString;
	}
	else
	{
		return false;
	}
}

bool UEOSCoreLibrary::IsEpicAccountIdIdenticalWith(FEOSEpicAccountId a, FEOSEpicAccountId b)
{
	if (EOSEpicAccountIdIsValid(a) && EOSEpicAccountIdIsValid(b))
	{
		FString aString;
		FString bString;

		EOSEpicAccountIdToString(a, aString);
		EOSEpicAccountIdToString(b, bString);

		return aString == bString;
	}
	else
	{
		return false;
	}
}

bool UEOSCoreLibrary::GetSessionAttributeBool(const FEOSSessionsAttributeData& Data)
{
	return (Data.AttributeData.Value.AsBool > 0);
}

FString UEOSCoreLibrary::GetSessionAttributeInt64(const FEOSSessionsAttributeData& Data)
{
	return LexToString(Data.AttributeData.Value.AsInt64);
}

FString UEOSCoreLibrary::GetSessionAttributeDouble(const FEOSSessionsAttributeData& Data)
{
	return LexToString(Data.AttributeData.Value.AsDouble);
}

FString UEOSCoreLibrary::GetSessionAttributeString(const FEOSSessionsAttributeData& Data)
{
	return UTF8_TO_TCHAR(Data.AttributeData.Value.AsUtf8);
}

bool UEOSCoreLibrary::GetLobbyAttributeBool(const FEOSLobbyAttributeData& Data)
{
	return (Data.AttributeData.Value.AsBool > 0);
}

FString UEOSCoreLibrary::GetLobbyAttributeInt64(const FEOSLobbyAttributeData& Data)
{
	return LexToString(Data.AttributeData.Value.AsInt64);
}

FString UEOSCoreLibrary::GetLobbyAttributeDouble(const FEOSLobbyAttributeData& Data)
{
	return LexToString(Data.AttributeData.Value.AsDouble);
}

FString UEOSCoreLibrary::GetLobbyAttributeString(const FEOSLobbyAttributeData& Data)
{
	return UTF8_TO_TCHAR(Data.AttributeData.Value.AsUtf8);
}

FString UEOSCoreLibrary::EOSEResultToString(EOSResult Result)
{
	return EOS_EResult_ToString(static_cast<EOS_EResult>(Result));
}

FString UEOSCoreLibrary::EOSEApplicationStatusToString(EOSEApplicationStatus ApplicationStatus)
{
	return UTF8_TO_TCHAR(EOS_EApplicationStatus_ToString(static_cast<EOS_EApplicationStatus>(ApplicationStatus)));
}

FString UEOSCoreLibrary::EOSENetworkStatusToString(EOSENetworkStatus NetworkStatus)
{
	return UTF8_TO_TCHAR(EOS_ENetworkStatus_ToString(static_cast<EOS_ENetworkStatus>(NetworkStatus)));
}

bool UEOSCoreLibrary::EOSEResultIsOperationComplete(EOSResult result)
{
	return EOS_EResult_IsOperationComplete(static_cast<EOS_EResult>(result)) > 0;
}

EOSResult UEOSCoreLibrary::EOSByteArrayToString(const TArray<uint8> array, int32 length, FString& outString)
{
	EOSResult Result = EOSResult::EOS_ServiceFailure;

	TArray<char> Buffer;
	Buffer.SetNum(4096);
	uint32_t BufferSize = 4096;

	Result = EOSHelpers::ToEOSCoreResult(EOS_ByteArray_ToString(array.GetData(), array.Num(), Buffer.GetData(), &BufferSize));
	
	if (Result == EOSResult::EOS_Success)
	{
		outString = UTF8_TO_TCHAR(Buffer.GetData());
	}

	return Result;
}

bool UEOSCoreLibrary::EOSEpicAccountIdIsValid(FEOSEpicAccountId id)
{
	if (id.ToString().Len() > 0)
		return EOS_EpicAccountId_IsValid(id) != EOS_FALSE;
	
	return false;
}

EOSResult UEOSCoreLibrary::EOSEpicAccountIdToString(FEOSEpicAccountId id, FString& string)
{
	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (EOSEpicAccountIdIsValid(id))
	{
		char Buffer[EOS_EPICACCOUNTID_MAX_LENGTH+1];
		int32_t BufferLen = EOS_EPICACCOUNTID_MAX_LENGTH+1;
		Result = EOSHelpers::ToEOSCoreResult(EOS_EpicAccountId_ToString(id, Buffer, &BufferLen));
		if (Result == EOSResult::EOS_Success)
		{
			string = Buffer;
		}
	}

	return Result;
}

FEOSEpicAccountId UEOSCoreLibrary::EOSEpicAccountIdFromString(FString String)
{
	const FTCHARToUTF8 EpicAccountId(*String);
	
	return EOS_EpicAccountId_FromString(EpicAccountId.Get());
}

bool UEOSCoreLibrary::EOSProductUserIdIsValid(FEOSProductUserId id)
{
	if (id.ToString().Len() > 0)
		return EOS_ProductUserId_IsValid(id) != EOS_FALSE;

	return false;
}

EOSResult UEOSCoreLibrary::EOSProductUserIdToString(FEOSProductUserId id, FString& string)
{
	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (EOSProductUserIdIsValid(id))
	{
		char Buffer[EOS_EPICACCOUNTID_MAX_LENGTH+1];
		int32_t OutBuffer = EOS_EPICACCOUNTID_MAX_LENGTH+1;
		Result = EOSHelpers::ToEOSCoreResult(EOS_ProductUserId_ToString(id, Buffer, &OutBuffer));
		if (Result == EOSResult::EOS_Success)
		{
			string = Buffer;
		}
	}
	
	return Result;
}

FEOSProductUserId UEOSCoreLibrary::EOSProductUserIdFromString(FString String)
{
	const FTCHARToUTF8 ProductUserId(*String);
	
	return EOS_ProductUserId_FromString(ProductUserId.Get());
}

TArray<uint8> UEOSCoreLibrary::CoreStringToByte(FString string)
{
	TArray<uint8> Arr;
	Arr.SetNumUninitialized(string.Len());

	StringToBytes(string, Arr.GetData(), Arr.Num());

	return Arr;
}

FString UEOSCoreLibrary::CoreBytesToString(const TArray<uint8>& data)
{
	return BytesToString(data.GetData(), data.Num());
}

FEOSProductUserId UEOSCoreLibrary::GetCurrentProductId(UObject* WorldContextObject, int32 userIndex /* = 0 */)
{
	FEOSProductUserId UserId = {};

	if (UEOSCoreSubsystem::GetConnectHandle(WorldContextObject))
	{
		UserId = EOS_Connect_GetLoggedInUserByIndex(UEOSCoreSubsystem::GetConnectHandle(WorldContextObject), userIndex);
	}

	return UserId;
}

FEOSEpicAccountId UEOSCoreLibrary::GetCurrentAccountId(UObject* WorldContextObject, int32 userIndex /* = 0 */)
{
	FEOSEpicAccountId AccountId = {};

	if (UEOSCoreSubsystem::GetAuthHandle(WorldContextObject))
	{
		AccountId = EOS_Auth_GetLoggedInAccountByIndex(UEOSCoreSubsystem::GetAuthHandle(WorldContextObject), userIndex);
	}

	return AccountId;
}

EOSResult UEOSCoreLibrary::EOSContinuanceTokenToString(FContinuanceToken token, FString& string)
{
	EOSResult Result = EOSResult::EOS_ServiceFailure;

	char Buffer[1024];
	int32_t OutBuffer = 1024;
	Result = EOSHelpers::ToEOSCoreResult(EOS_ContinuanceToken_ToString(token.m_ContinuanceTokenDetails, Buffer, &OutBuffer));
	if (Result == EOSResult::EOS_Success)
	{
		string = UTF8_TO_TCHAR(Buffer);
	}

	return Result;
}

FString UEOSCoreLibrary::FindExchangeCodePassword()
{
	FString Result;
	FString CommandLine(FCommandLine::Get());

	TArray<FString> Tokens;
	TArray<FString> Switches;
	TMap<FString, FString> Params;

	UCommandlet::ParseCommandLine(FCommandLine::Get(), Tokens, Switches, Params);

	if (Params.Contains("AUTH_PASSWORD"))
	{
		Result = *Params.Find("AUTH_PASSWORD");
	}

	return Result;
}

FEOSSessionSetting UEOSCoreLibrary::MakeBool(bool value)
{
	return FEOSSessionSetting(value);
}

FEOSSessionSetting UEOSCoreLibrary::MakeString(FString value)
{
	return FEOSSessionSetting(value);
}

FEOSSessionSetting UEOSCoreLibrary::MakeInteger(int32 value)
{
	return FEOSSessionSetting(value);
}

FEOSSessionSearchSetting UEOSCoreLibrary::MakeSearchBool(bool value)
{
	return FEOSSessionSearchSetting(value);
}

FEOSSessionSearchSetting UEOSCoreLibrary::MakeSearchString(FString value)
{
	return FEOSSessionSearchSetting(value);
}

FEOSSessionSearchSetting UEOSCoreLibrary::MakeSearchInteger(int32 value)
{
	return FEOSSessionSearchSetting(value);
}

EOSResult UEOSCoreLibrary::EOSPlatformGetDesktopCrossplayStatus(UObject* WorldContextObject, FEOSPlatformGetDesktopCrossplayStatusOptions Options, FEOSPlatformGetDesktopCrossplayStatusInfo& OutDesktopCrossplayStatusInfo)
{
	LogVerbose("");

	EOSResult Result = EOSResult::MAX;

	if (UEOSCoreSubsystem::GetPlatform(WorldContextObject))
	{
		EOS_Platform_GetDesktopCrossplayStatusOptions Parameters = {};
		Parameters.ApiVersion = EOS_PLATFORM_GETDESKTOPCROSSPLAYSTATUS_API_LATEST;
		
		EOS_Platform_GetDesktopCrossplayStatusInfo* GetDesktopCrossplayStatusInfo = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Platform_GetDesktopCrossplayStatus(UEOSCoreSubsystem::GetPlatform(WorldContextObject), &Parameters, GetDesktopCrossplayStatusInfo));

		if (Result == EOSResult::EOS_Success)
		{
			OutDesktopCrossplayStatusInfo = *GetDesktopCrossplayStatusInfo;
		}
	}

	return Result;
}

EOSResult UEOSCoreLibrary::EOSPlatformSetApplicationStatus(UObject* WorldContextObject, const EOSEApplicationStatus NewStatus)
{
	LogVerbose("");

	EOSResult Result = EOSResult::MAX;

	if (UEOSCoreSubsystem::GetPlatform(WorldContextObject))
	{
		Result = EOSHelpers::ToEOSCoreResult(EOS_Platform_SetApplicationStatus(UEOSCoreSubsystem::GetPlatform(WorldContextObject), static_cast<EOS_EApplicationStatus>(NewStatus)));
	}

	return Result;
}

EOSEApplicationStatus UEOSCoreLibrary::EOSPlatformGetApplicationStatus(UObject* WorldContextObject)
{
	LogVerbose("");

	EOSEApplicationStatus Result = EOSEApplicationStatus::EOS_AS_Foreground;

	if (UEOSCoreSubsystem::GetPlatform(WorldContextObject))
	{
		Result = static_cast<EOSEApplicationStatus>(EOS_Platform_GetApplicationStatus(UEOSCoreSubsystem::GetPlatform(WorldContextObject)));
	}

	return Result;
}

EOSResult UEOSCoreLibrary::EOSPlatformSetNetworkStatus(UObject* WorldContextObject, EOSENetworkStatus NewStatus)
{
	LogVerbose("");

	EOSResult Result = EOSResult::MAX;

	if (UEOSCoreSubsystem::GetPlatform(WorldContextObject))
	{
		Result = EOSHelpers::ToEOSCoreResult(EOS_Platform_SetNetworkStatus(UEOSCoreSubsystem::GetPlatform(WorldContextObject), static_cast<EOS_ENetworkStatus>(NewStatus)));
	}

	return Result;
}

EOSENetworkStatus UEOSCoreLibrary::EOSPlatformGetNetworkStatus(UObject* WorldContextObject)
{
	LogVerbose("");

	EOSENetworkStatus Result = EOSENetworkStatus::EOS_NS_Disabled;

	if (UEOSCoreSubsystem::GetPlatform(WorldContextObject))
	{
		Result = static_cast<EOSENetworkStatus>(EOS_Platform_GetNetworkStatus(UEOSCoreSubsystem::GetPlatform(WorldContextObject)));
	}

	return Result;
}

