/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "AntiCheatClient/EOSAntiCheatClient.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLogging.h"

void UCoreAntiCheatClient::Deinitialize()
{
	for (auto Element : m_AntiCheatClientMessageToServer)
	{
		EOS_AntiCheatClient_RemoveNotifyMessageToServer(GetAntiCheatClientHandle(this), Element.Key);
	}

	for (auto Element : m_AntiCheatClientMessageToPeer)
	{
		EOS_AntiCheatClient_RemoveNotifyMessageToPeer(GetAntiCheatClientHandle(this), Element.Key);
	}

	for (auto Element : m_AntiCheatClientPeerActionRequired)
	{
		EOS_AntiCheatClient_RemoveNotifyPeerActionRequired(GetAntiCheatClientHandle(this), Element.Key);
	}

	for (auto Element : m_AntiCheatClientPeerAuthStatusChanged)
	{
		EOS_AntiCheatClient_RemoveNotifyPeerAuthStatusChanged(GetAntiCheatClientHandle(this), Element.Key);
	}

	m_AntiCheatClientMessageToServer.Empty();
	m_AntiCheatClientMessageToPeer.Empty();
	m_AntiCheatClientPeerActionRequired.Empty();
	m_AntiCheatClientPeerAuthStatusChanged.Empty();
}

UCoreAntiCheatClient* UCoreAntiCheatClient::GetAntiCheatClient(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (GetAntiCheatClientHandle(WorldContextObject))
		{
			if (UWorld* World = WorldContextObject->GetWorld())
			{
				if (UGameInstance* GameInstance = World->GetGameInstance())
				{
					return GameInstance->GetSubsystem<UCoreAntiCheatClient>();
				}
			}	
		}
	}
	return nullptr;
}

FEOSNotificationId UCoreAntiCheatClient::EOSAntiCheatClientAddNotifyMessageToServer(UObject* WorldContextObject, FEOSAntiCheatClientAddNotifyMessageToServerOptions Options, const FOnAntiCheatClientOnMessageToServerCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetAntiCheatClientHandle(WorldContextObject))
	{
		EOS_AntiCheatClient_AddNotifyMessageToServerOptions AntiCheatClientOptions = {};
		AntiCheatClientOptions.ApiVersion = EOS_ANTICHEATCLIENT_ADDNOTIFYMESSAGETOSERVER_API_LATEST;

		FAntiCheatClientMessageToServer* CallbackObj = new FAntiCheatClientMessageToServer(Callback);
		Id = EOS_AntiCheatClient_AddNotifyMessageToServer(GetAntiCheatClientHandle(WorldContextObject), &AntiCheatClientOptions, CallbackObj, ([](const EOS_AntiCheatClient_OnMessageToServerCallbackInfo* Data)
		{
			const FAntiCheatClientMessageToServer* CallbackObj = static_cast<FAntiCheatClientMessageToServer*>(Data->ClientData);
			CallbackObj->m_Callback.ExecuteIfBound(*Data);
		}));

		GetAntiCheatClient(WorldContextObject)->m_AntiCheatClientMessageToServer.Add(Id, CallbackObj);
	}

	return Id;
}

void UCoreAntiCheatClient::EOSAntiCheatClientRemoveNotifyMessageToServer(UObject* WorldContextObject, FEOSNotificationId NotificationId)
{
	LogVerbose("");

	if (GetAntiCheatClientHandle(WorldContextObject))
	{
		EOS_AntiCheatClient_RemoveNotifyMessageToServer(GetAntiCheatClientHandle(WorldContextObject), NotificationId);
		GetAntiCheatClient(WorldContextObject)->m_AntiCheatClientMessageToServer.Remove(NotificationId);
	}
}

FEOSNotificationId UCoreAntiCheatClient::EOSAntiCheatClientAddNotifyMessageToPeer(UObject* WorldContextObject, FEOSAntiCheatClientAddNotifyMessageToPeerOptions Options, const FOnAntiCheatClientOnMessageToPeerCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetAntiCheatClientHandle(WorldContextObject))
	{
		EOS_AntiCheatClient_AddNotifyMessageToPeerOptions AntiCheatClientOptions = {};
		AntiCheatClientOptions.ApiVersion = EOS_ANTICHEATCLIENT_ADDNOTIFYMESSAGETOPEER_API_LATEST;

		FAntiCheatClientMessageToPeer* CallbackObj = new FAntiCheatClientMessageToPeer(Callback);
		Id = EOS_AntiCheatClient_AddNotifyMessageToPeer(GetAntiCheatClientHandle(WorldContextObject), &AntiCheatClientOptions, CallbackObj, ([](const EOS_AntiCheatCommon_OnMessageToClientCallbackInfo* Data)
		{
			const FAntiCheatClientMessageToPeer* CallbackObj = static_cast<FAntiCheatClientMessageToPeer*>(Data->ClientData);
			CallbackObj->m_Callback.ExecuteIfBound(*Data);
		}));

		GetAntiCheatClient(WorldContextObject)->m_AntiCheatClientMessageToPeer.Add(Id, CallbackObj);
	}

	return Id;
}

void UCoreAntiCheatClient::EOSAntiCheatClientRemoveNotifyMessageToPeer(UObject* WorldContextObject, FEOSNotificationId NotificationId)
{
	LogVerbose("");

	if (GetAntiCheatClientHandle(WorldContextObject))
	{
		EOS_AntiCheatClient_RemoveNotifyMessageToPeer(GetAntiCheatClientHandle(WorldContextObject), NotificationId);
		GetAntiCheatClient(WorldContextObject)->m_AntiCheatClientMessageToPeer.Remove(NotificationId);
	}
}

FEOSNotificationId UCoreAntiCheatClient::EOSAntiCheatClientAddNotifyPeerActionRequired(UObject* WorldContextObject, FEOSAntiCheatClientAddNotifyPeerActionRequiredOptions Options, const FOnAntiCheatClientOnPeerActionRequiredCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetAntiCheatClientHandle(WorldContextObject))
	{
		EOS_AntiCheatClient_AddNotifyPeerActionRequiredOptions AntiCheatClientOptions = {};
		AntiCheatClientOptions.ApiVersion = EOS_ANTICHEATCLIENT_ADDNOTIFYPEERACTIONREQUIRED_API_LATEST;

		FAntiCheatClientPeerActionRequired* CallbackObj = new FAntiCheatClientPeerActionRequired(Callback);
		Id = EOS_AntiCheatClient_AddNotifyPeerActionRequired(GetAntiCheatClientHandle(WorldContextObject), &AntiCheatClientOptions, CallbackObj, ([](const EOS_AntiCheatCommon_OnClientActionRequiredCallbackInfo* Data)
		{
			const FAntiCheatClientPeerActionRequired* CallbackObj = static_cast<FAntiCheatClientPeerActionRequired*>(Data->ClientData);
			CallbackObj->m_Callback.ExecuteIfBound(*Data);
		}));

		GetAntiCheatClient(WorldContextObject)->m_AntiCheatClientPeerActionRequired.Add(Id, CallbackObj);
	}

	return Id;
}

void UCoreAntiCheatClient::EOSAntiCheatClientRemoveNotifyPeerActionRequired(UObject* WorldContextObject, FEOSNotificationId NotificationId)
{
	LogVerbose("");

	if (GetAntiCheatClientHandle(WorldContextObject))
	{
		EOS_AntiCheatClient_RemoveNotifyPeerActionRequired(GetAntiCheatClientHandle(WorldContextObject), NotificationId);
		GetAntiCheatClient(WorldContextObject)->m_AntiCheatClientPeerActionRequired.Remove(NotificationId);
	}
}

FEOSNotificationId UCoreAntiCheatClient::EOSAntiCheatClientAddNotifyPeerAuthStatusChanged(UObject* WorldContextObject, FEOSAntiCheatClientAddNotifyPeerAuthStatusChangedOptions Options, const FOnAntiCheatClientOnPeerAuthStatusChangedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetAntiCheatClientHandle(WorldContextObject))
	{
		EOS_AntiCheatClient_AddNotifyPeerAuthStatusChangedOptions AntiCheatClientOptions = {};
		AntiCheatClientOptions.ApiVersion = EOS_ANTICHEATCLIENT_ADDNOTIFYPEERAUTHSTATUSCHANGED_API_LATEST;

		FAntiCheatClientPeerAuthStatusChanged* CallbackObj = new FAntiCheatClientPeerAuthStatusChanged(Callback);
		Id = EOS_AntiCheatClient_AddNotifyPeerAuthStatusChanged(GetAntiCheatClientHandle(WorldContextObject), &AntiCheatClientOptions, CallbackObj, ([](const EOS_AntiCheatCommon_OnClientAuthStatusChangedCallbackInfo* Data)
		{
			const FAntiCheatClientPeerAuthStatusChanged* CallbackObj = static_cast<FAntiCheatClientPeerAuthStatusChanged*>(Data->ClientData);
			CallbackObj->m_Callback.ExecuteIfBound(*Data);
		}));

		GetAntiCheatClient(WorldContextObject)->m_AntiCheatClientPeerAuthStatusChanged.Add(Id, CallbackObj);
	}

	return Id;
}

void UCoreAntiCheatClient::EOSAntiCheatClientRemoveNotifyPeerAuthStatusChanged(UObject* WorldContextObject, FEOSNotificationId NotificationId)
{
	LogVerbose("");

	if (GetAntiCheatClientHandle(WorldContextObject))
	{
		EOS_AntiCheatClient_RemoveNotifyPeerAuthStatusChanged(GetAntiCheatClientHandle(WorldContextObject), NotificationId);
		GetAntiCheatClient(WorldContextObject)->m_AntiCheatClientPeerAuthStatusChanged.Remove(NotificationId);
	}
}

FEOSNotificationId UCoreAntiCheatClient::EOSAntiCheatClientAddNotifyClientIntegrityViolated(UObject* WorldContextObject, FEOSAntiCheatClientAddNotifyClientIntegrityViolatedOptions Options, const FOnAntiCheatClientOnClientIntegrityViolatedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetAntiCheatClientHandle(WorldContextObject))
	{
		EOS_AntiCheatClient_AddNotifyClientIntegrityViolatedOptions AntiCheatClientOptions = {};
		AntiCheatClientOptions.ApiVersion = EOS_ANTICHEATCLIENT_ADDNOTIFYCLIENTINTEGRITYVIOLATED_API_LATEST;

		FAntiCheatClientOnClientIntegrityViolated* CallbackObj = new FAntiCheatClientOnClientIntegrityViolated(Callback);
		Id = EOS_AntiCheatClient_AddNotifyClientIntegrityViolated(GetAntiCheatClientHandle(WorldContextObject), &AntiCheatClientOptions, CallbackObj, ([](const EOS_AntiCheatClient_OnClientIntegrityViolatedCallbackInfo* Data)
		{
			const FAntiCheatClientOnClientIntegrityViolated* CallbackObj = static_cast<FAntiCheatClientOnClientIntegrityViolated*>(Data->ClientData);
			CallbackObj->m_Callback.ExecuteIfBound(*Data);
		}));

		GetAntiCheatClient(WorldContextObject)->m_AntiCheatClientOnClientIntegrityViolated.Add(Id, CallbackObj);
	}

	return Id;
}

void UCoreAntiCheatClient::EOSAntiCheatClientRemoveNotifyClientIntegrityViolated(UObject* WorldContextObject, FEOSNotificationId NotificationId)
{
	LogVerbose("");

	if (GetAntiCheatClientHandle(WorldContextObject))
	{
		EOS_AntiCheatClient_RemoveNotifyClientIntegrityViolated(GetAntiCheatClientHandle(WorldContextObject), NotificationId);
		GetAntiCheatClient(WorldContextObject)->m_AntiCheatClientOnClientIntegrityViolated.Remove(NotificationId);
	}
}

EOSResult UCoreAntiCheatClient::EOSAntiCheatClientBeginSession(UObject* WorldContextObject, FEOSAntiCheatClientBeginSessionOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_UnexpectedError;

	if (GetAntiCheatClientHandle(WorldContextObject))
	{
		EOS_AntiCheatClient_BeginSessionOptions AntiCheatClientOptions = {};
		AntiCheatClientOptions.ApiVersion = EOS_ANTICHEATCLIENT_BEGINSESSION_API_LATEST;
		AntiCheatClientOptions.LocalUserId = Options.LocalUserId;
		AntiCheatClientOptions.Mode = static_cast<EOS_EAntiCheatClientMode>(Options.Mode);

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatClient_BeginSession(GetAntiCheatClientHandle(WorldContextObject), &AntiCheatClientOptions));
	}

	return Result;
}

EOSResult UCoreAntiCheatClient::EOSAntiCheatClientEndSession(UObject* WorldContextObject, FEOSAntiCheatClientEndSessionOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_UnexpectedError;

	if (GetAntiCheatClientHandle(WorldContextObject))
	{
		EOS_AntiCheatClient_EndSessionOptions AntiCheatClientOptions = {};
		AntiCheatClientOptions.ApiVersion = EOS_ANTICHEATCLIENT_ENDSESSION_API_LATEST;

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatClient_EndSession(GetAntiCheatClientHandle(WorldContextObject), &AntiCheatClientOptions));
	}

	return Result;
}

EOSResult UCoreAntiCheatClient::EOSAntiCheatClientPollStatus(UObject* WorldContextObject, FEOSAntiCheatClientPollStatusOptions Options, EEOSEAntiCheatClientViolationType& ViolationType, FString& OutMessage)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_UnexpectedError;

	if (GetAntiCheatClientHandle(WorldContextObject))
	{
		EOS_AntiCheatClient_PollStatusOptions AntiCheatClientOptions = {};
		AntiCheatClientOptions.ApiVersion = EOS_ANTICHEATCLIENT_POLLSTATUS_API_LATEST;
		AntiCheatClientOptions.OutMessageLength = Options.OutMessageLength;

		EOS_EAntiCheatClientViolationType AntiCheatClientViolationType = EOS_EAntiCheatClientViolationType::EOS_ACCVT_Invalid;

		char Message[1024];

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatClient_PollStatus(GetAntiCheatClientHandle(WorldContextObject), &AntiCheatClientOptions, &AntiCheatClientViolationType, Message));

		if (Result == EOSResult::EOS_Success)
		{
			OutMessage = UTF8_TO_TCHAR(Message);
			ViolationType = static_cast<EEOSEAntiCheatClientViolationType>(AntiCheatClientViolationType);
		}
	}

	return Result;
}

EOSResult UCoreAntiCheatClient::EOSAntiCheatClientAddExternalIntegrityCatalog(UObject* WorldContextObject, FEOSAntiCheatClientAddExternalIntegrityCatalogOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_UnexpectedError;

	if (GetAntiCheatClientHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedPathToBinFile(*Options.PathToBinFile);
		
		EOS_AntiCheatClient_AddExternalIntegrityCatalogOptions AntiCheatClientOptions = {};
		AntiCheatClientOptions.ApiVersion = EOS_ANTICHEATCLIENT_ADDEXTERNALINTEGRITYCATALOG_API_LATEST;
		AntiCheatClientOptions.PathToBinFile = ConvertedPathToBinFile.Get();

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatClient_AddExternalIntegrityCatalog(GetAntiCheatClientHandle(WorldContextObject), &AntiCheatClientOptions));
	}

	return Result;
}

EOSResult UCoreAntiCheatClient::EOSAntiCheatClientReceiveMessageFromServer(UObject* WorldContextObject, FEOSAntiCheatClientReceiveMessageFromServerOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_UnexpectedError;

	if (GetAntiCheatClientHandle(WorldContextObject))
	{
		EOS_AntiCheatClient_ReceiveMessageFromServerOptions AntiCheatClientOptions = {};
		AntiCheatClientOptions.ApiVersion = EOS_ANTICHEATCLIENT_RECEIVEMESSAGEFROMSERVER_API_LATEST;
		AntiCheatClientOptions.DataLengthBytes = Options.Data.Num();
		AntiCheatClientOptions.Data = Options.Data.GetData();

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatClient_ReceiveMessageFromServer(GetAntiCheatClientHandle(WorldContextObject), &AntiCheatClientOptions));
	}

	return Result;
}

EOSResult UCoreAntiCheatClient::EOSAntiCheatClientGetProtectMessageOutputLength(UObject* WorldContextObject, FEOSAntiCheatClientGetProtectMessageOutputLengthOptions Options, int32& OutBufferLengthBytes)
{
	LogVerbose("");

	OutBufferLengthBytes = 0;

	EOSResult Result = EOSResult::EOS_UnexpectedError;

	if (GetAntiCheatClientHandle(WorldContextObject))
	{
		EOS_AntiCheatClient_GetProtectMessageOutputLengthOptions AntiCheatClientOptions = {};
		AntiCheatClientOptions.ApiVersion = EOS_ANTICHEATCLIENT_GETPROTECTMESSAGEOUTPUTLENGTH_API_LATEST;
		AntiCheatClientOptions.DataLengthBytes = Options.DataLengthBytes;

		uint32_t BufferLengthBytes = 0;
		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatClient_GetProtectMessageOutputLength(GetAntiCheatClientHandle(WorldContextObject), &AntiCheatClientOptions, &BufferLengthBytes));

		if (Result == EOSResult::EOS_Success)
		{
			OutBufferLengthBytes = BufferLengthBytes;
		}
	}

	return Result;
}

EOSResult UCoreAntiCheatClient::EOSAntiCheatClientProtectMessage(UObject* WorldContextObject, FEOSAntiCheatClientProtectMessageOptions Options, TArray<uint8>& OutBuffer)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_UnexpectedError;

	OutBuffer.Empty();

	if (GetAntiCheatClientHandle(WorldContextObject))
	{
		EOS_AntiCheatClient_ProtectMessageOptions AntiCheatClientOptions = {};
		AntiCheatClientOptions.ApiVersion = EOS_ANTICHEATCLIENT_PROTECTMESSAGE_API_LATEST;
		AntiCheatClientOptions.DataLengthBytes = Options.Data.Num();
		AntiCheatClientOptions.Data = Options.Data.GetData();
		AntiCheatClientOptions.OutBufferSizeBytes = Options.OutBufferSizeBytes;

		uint32_t OutBufferLengthBytes = 0;
		void* Buffer = nullptr;
		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatClient_ProtectMessage(GetAntiCheatClientHandle(WorldContextObject), &AntiCheatClientOptions, Buffer, &OutBufferLengthBytes));

		if (Result == EOSResult::EOS_Success)
		{
			OutBuffer.SetNum(OutBufferLengthBytes);
			FMemory::Memcpy(OutBuffer.GetData(), Buffer, OutBufferLengthBytes);
		}
	}

	return Result;
}

EOSResult UCoreAntiCheatClient::EOSAntiCheatClientUnprotectMessage(UObject* WorldContextObject, FEOSAntiCheatClientUnprotectMessageOptions Options, TArray<uint8> OutBuffer)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_UnexpectedError;

	OutBuffer.Empty();

	if (GetAntiCheatClientHandle(WorldContextObject))
	{
		EOS_AntiCheatClient_UnprotectMessageOptions AntiCheatClientOptions = {};
		AntiCheatClientOptions.ApiVersion = EOS_ANTICHEATCLIENT_UNPROTECTMESSAGE_API_LATEST;
		AntiCheatClientOptions.DataLengthBytes = Options.Data.Num();
		AntiCheatClientOptions.Data = Options.Data.GetData();
		AntiCheatClientOptions.OutBufferSizeBytes = Options.OutBufferSizeBytes;

		uint32_t OutBufferLengthBytes = 0;
		void* Buffer = nullptr;
		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatClient_UnprotectMessage(GetAntiCheatClientHandle(WorldContextObject), &AntiCheatClientOptions, Buffer, &OutBufferLengthBytes));

		if (Result == EOSResult::EOS_Success)
		{
			OutBuffer.SetNum(OutBufferLengthBytes);
			FMemory::Memcpy(OutBuffer.GetData(), Buffer, OutBufferLengthBytes);
		}
	}

	return Result;
}

EOSResult UCoreAntiCheatClient::EOSAntiCheatClientRegisterPeer(UObject* WorldContextObject, FEOSAntiCheatClientRegisterPeerOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_UnexpectedError;

	if (GetAntiCheatClientHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedAccountId(*Options.AccountId_DEPRECATED);
		const FTCHARToUTF8 ConvertedIpAddress(*Options.IpAddress);
		
		EOS_AntiCheatClient_RegisterPeerOptions AntiCheatClientOptions = {};
		AntiCheatClientOptions.ApiVersion = EOS_ANTICHEATCLIENT_REGISTERPEER_API_LATEST;
		AntiCheatClientOptions.PeerHandle = Options.PeerHandle;
		AntiCheatClientOptions.ClientType = static_cast<EOS_EAntiCheatCommonClientType>(Options.ClientType);
		AntiCheatClientOptions.ClientPlatform = static_cast<EOS_EAntiCheatCommonClientPlatform>(Options.ClientPlatform);
		AntiCheatClientOptions.AuthenticationTimeout = Options.AuthenticationTimeout;
		AntiCheatClientOptions.AccountId_DEPRECATED = ConvertedAccountId.Get();
		AntiCheatClientOptions.IpAddress = ConvertedIpAddress.Get();
		AntiCheatClientOptions.PeerProductUserId = Options.PeerProductUserId;

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatClient_RegisterPeer(GetAntiCheatClientHandle(WorldContextObject), &AntiCheatClientOptions));
	}

	return Result;
}

EOSResult UCoreAntiCheatClient::EOSAntiCheatClientUnregisterPeer(UObject* WorldContextObject, FEOSAntiCheatClientUnregisterPeerOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_UnexpectedError;

	if (GetAntiCheatClientHandle(WorldContextObject))
	{
		EOS_AntiCheatClient_UnregisterPeerOptions AntiCheatClientOptions = {};
		AntiCheatClientOptions.ApiVersion = EOS_ANTICHEATCLIENT_UNREGISTERPEER_API_LATEST;
		AntiCheatClientOptions.PeerHandle = Options.PeerHandle;

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatClient_UnregisterPeer(GetAntiCheatClientHandle(WorldContextObject), &AntiCheatClientOptions));
	}

	return Result;
}

EOSResult UCoreAntiCheatClient::EOSAntiCheatClientReceiveMessageFromPeer(UObject* WorldContextObject, FEOSAntiCheatClientReceiveMessageFromPeerOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_UnexpectedError;

	if (GetAntiCheatClientHandle(WorldContextObject))
	{
		EOS_AntiCheatClient_ReceiveMessageFromPeerOptions AntiCheatClientOptions = {};
		AntiCheatClientOptions.ApiVersion = EOS_ANTICHEATCLIENT_RECEIVEMESSAGEFROMPEER_API_LATEST;
		AntiCheatClientOptions.PeerHandle = Options.PeerHandle;
		AntiCheatClientOptions.DataLengthBytes = Options.Data.Num();
		AntiCheatClientOptions.Data = Options.Data.GetData();

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatClient_ReceiveMessageFromPeer(GetAntiCheatClientHandle(WorldContextObject), &AntiCheatClientOptions));
	}

	return Result;
}
