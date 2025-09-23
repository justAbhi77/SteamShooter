/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "AntiCheatServer/EOSAntiCheatServer.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLogging.h"
#include "Core/EOSHelpers.h"

void UCoreAntiCheatServer::Deinitialize()
{
	LogVerbose("");

	for (auto Element: m_AntiCheatServerMessageToClient)
	{
		EOS_AntiCheatClient_RemoveNotifyMessageToServer(GetAntiCheatClientHandle(this), Element.Key);
	}

	for (auto Element: m_AntiCheatServerActionRequired)
	{
		EOS_AntiCheatClient_RemoveNotifyPeerActionRequired(GetAntiCheatClientHandle(this), Element.Key);
	}

	for (auto Element: m_AntiCheatServerClientAuthStatusChanged)
	{
		EOS_AntiCheatClient_RemoveNotifyPeerAuthStatusChanged(GetAntiCheatClientHandle(this), Element.Key);
	}

	m_AntiCheatServerMessageToClient.Empty();
	m_AntiCheatServerActionRequired.Empty();
	m_AntiCheatServerClientAuthStatusChanged.Empty();
}

UCoreAntiCheatServer* UCoreAntiCheatServer::GetAntiCheatServer(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (GetAntiCheatServerHandle(WorldContextObject))
		{
			if (UWorld* World = WorldContextObject->GetWorld())
			{
				if (UGameInstance* GameInstance = World->GetGameInstance())
				{
					return GameInstance->GetSubsystem<UCoreAntiCheatServer>();
				}
			}	
		}
	}
	return nullptr;
}

FEOSNotificationId UCoreAntiCheatServer::EOSAntiCheatServerAddNotifyMessageToClient(UObject* WorldContextObject, FEOSAntiCheatServerAddNotifyMessageToClientOptions Options, const FOnAntiCheatServerOnMessageToClientCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetAntiCheatServerHandle(WorldContextObject))
	{
		EOS_AntiCheatServer_AddNotifyMessageToClientOptions AntiCheatServerOptions = {};
		AntiCheatServerOptions.ApiVersion = EOS_ANTICHEATSERVER_ADDNOTIFYMESSAGETOCLIENT_API_LATEST;

		FAntiCheatServerMessageToClient* CallbackObj = new FAntiCheatServerMessageToClient(Callback);

		Id = EOS_AntiCheatServer_AddNotifyMessageToClient(GetAntiCheatServerHandle(WorldContextObject), &AntiCheatServerOptions, CallbackObj, ([](const EOS_AntiCheatCommon_OnMessageToClientCallbackInfo* Data)
		{
			const FAntiCheatServerMessageToClient* CallbackObj = static_cast<FAntiCheatServerMessageToClient*>(Data->ClientData);
			CallbackObj->m_Callback.ExecuteIfBound(*Data);
		}));

		
		GetAntiCheatServer(WorldContextObject)->m_AntiCheatServerMessageToClient.Add(Id, CallbackObj);
	}

	return Id;
}

void UCoreAntiCheatServer::EOSAntiCheatServerRemoveNotifyMessageToClient(UObject* WorldContextObject, FEOSNotificationId NotificationId)
{
	LogVerbose("");

	if (GetAntiCheatServerHandle(WorldContextObject))
	{
		EOS_AntiCheatClient_RemoveNotifyMessageToServer(GetAntiCheatClientHandle(WorldContextObject), NotificationId);
		GetAntiCheatServer(WorldContextObject)->m_AntiCheatServerMessageToClient.Remove(NotificationId);
	}
}

FEOSNotificationId UCoreAntiCheatServer::EOSAntiCheatServerAddNotifyClientActionRequired(UObject* WorldContextObject, FEOSAntiCheatServerAddNotifyClientActionRequiredOptions Options, const FOnAntiCheatServerOnClientActionRequiredCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetAntiCheatServerHandle(WorldContextObject))
	{
		EOS_AntiCheatServer_AddNotifyClientActionRequiredOptions AntiCheatServerOptions = {};
		AntiCheatServerOptions.ApiVersion = EOS_ANTICHEATSERVER_ADDNOTIFYCLIENTACTIONREQUIRED_API_LATEST;

		FAntiCheatServerActionRequired* CallbackObj = new FAntiCheatServerActionRequired(Callback);

		Id = EOS_AntiCheatServer_AddNotifyClientActionRequired(GetAntiCheatServerHandle(WorldContextObject), &AntiCheatServerOptions, CallbackObj, ([](const EOS_AntiCheatCommon_OnClientActionRequiredCallbackInfo* Data)
		{
			const FAntiCheatServerActionRequired* CallbackObj = static_cast<FAntiCheatServerActionRequired*>(Data->ClientData);
			CallbackObj->m_Callback.ExecuteIfBound(*Data);
		}));

		GetAntiCheatServer(WorldContextObject)->m_AntiCheatServerActionRequired.Add(Id, CallbackObj);
	}

	return Id;
}

void UCoreAntiCheatServer::EOSAntiCheatServerRemoveNotifyClientActionRequired(UObject* WorldContextObject, FEOSNotificationId NotificationId)
{
	LogVerbose("");

	if (GetAntiCheatServerHandle(WorldContextObject))
	{
		EOS_AntiCheatClient_RemoveNotifyPeerActionRequired(GetAntiCheatClientHandle(WorldContextObject), NotificationId);
		GetAntiCheatServer(WorldContextObject)->m_AntiCheatServerActionRequired.Remove(NotificationId);
	}
}

FEOSNotificationId UCoreAntiCheatServer::EOSAntiCheatServerAddNotifyClientAuthStatusChanged(UObject* WorldContextObject, FEOSAntiCheatServerAddNotifyClientAuthStatusChangedOptions Options, const FOnAntiCheatServerOnClientAuthStatusChangedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetAntiCheatServerHandle(WorldContextObject))
	{
		EOS_AntiCheatServer_AddNotifyClientAuthStatusChangedOptions AntiCheatServerOptions = {};
		AntiCheatServerOptions.ApiVersion = EOS_ANTICHEATSERVER_ADDNOTIFYCLIENTAUTHSTATUSCHANGED_API_LATEST;

		FAntiCheatServerClientAuthStatusChanged* CallbackObj = new FAntiCheatServerClientAuthStatusChanged(Callback);

		Id = EOS_AntiCheatServer_AddNotifyClientAuthStatusChanged(GetAntiCheatServerHandle(WorldContextObject), &AntiCheatServerOptions, CallbackObj, ([](const EOS_AntiCheatCommon_OnClientAuthStatusChangedCallbackInfo* Data)
		{
			const FAntiCheatServerClientAuthStatusChanged* CallbackObj = static_cast<FAntiCheatServerClientAuthStatusChanged*>(Data->ClientData);
			CallbackObj->m_Callback.ExecuteIfBound(*Data);
		}));

		GetAntiCheatServer(WorldContextObject)->m_AntiCheatServerClientAuthStatusChanged.Add(Id, CallbackObj);
	}

	return Id;
}

void UCoreAntiCheatServer::EOSAntiCheatServerRemoveNotifyClientAuthStatusChanged(UObject* WorldContextObject, FEOSNotificationId NotificationId)
{
	LogVerbose("");

	if (GetAntiCheatServerHandle(WorldContextObject))
	{
		EOS_AntiCheatClient_RemoveNotifyPeerAuthStatusChanged(GetAntiCheatClientHandle(WorldContextObject), NotificationId);
		GetAntiCheatServer(WorldContextObject)->m_AntiCheatServerClientAuthStatusChanged.Remove(NotificationId);
	}
}

EOSResult UCoreAntiCheatServer::EOSAntiCheatServerBeginSession(UObject* WorldContextObject, FEOSAntiCheatServerBeginSessionOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_InvalidParameters;

	if (GetAntiCheatServerHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ServerName(*Options.ServerName);
		
		EOS_AntiCheatServer_BeginSessionOptions AntiCheatServerOptions = {};
		AntiCheatServerOptions.ApiVersion = EOS_ANTICHEATSERVER_BEGINSESSION_API_LATEST;
		AntiCheatServerOptions.RegisterTimeoutSeconds = Options.RegisterTimeoutSeconds;
		AntiCheatServerOptions.ServerName = ServerName.Get();
		AntiCheatServerOptions.bEnableGameplayData = Options.bEnableGameplayData;
		AntiCheatServerOptions.LocalUserId = Options.LocalUserId;

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatServer_BeginSession(GetAntiCheatServerHandle(WorldContextObject), &AntiCheatServerOptions));
	}

	return Result;
}

EOSResult UCoreAntiCheatServer::EOSAntiCheatServerEndSession(UObject* WorldContextObject, FEOSAntiCheatServerEndSessionOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_InvalidParameters;

	if (GetAntiCheatServerHandle(WorldContextObject))
	{
		EOS_AntiCheatServer_EndSessionOptions AntiCheatServerOptions = {};
		AntiCheatServerOptions.ApiVersion = EOS_ANTICHEATSERVER_ENDSESSION_API_LATEST;

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatServer_EndSession(GetAntiCheatServerHandle(WorldContextObject), &AntiCheatServerOptions));
	}

	return Result;
}

EOSResult UCoreAntiCheatServer::EOSAntiCheatServerRegisterClient(UObject* WorldContextObject, FEOSAntiCheatServerRegisterClientOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_InvalidParameters;

	if (GetAntiCheatServerHandle(WorldContextObject))
	{
		const FTCHARToUTF8 IpAddress(*Options.IpAddress);
		
		EOS_AntiCheatServer_RegisterClientOptions AntiCheatServerOptions = {};
		AntiCheatServerOptions.ApiVersion = EOS_ANTICHEATSERVER_REGISTERCLIENT_API_LATEST;
		AntiCheatServerOptions.ClientHandle = Options.ClientHandle;
		AntiCheatServerOptions.ClientType = static_cast<EOS_EAntiCheatCommonClientType>(Options.ClientType);
		AntiCheatServerOptions.ClientPlatform = static_cast<EOS_EAntiCheatCommonClientPlatform>(Options.ClientPlatform);
		AntiCheatServerOptions.IpAddress = IpAddress.Get();
		AntiCheatServerOptions.UserId = Options.UserId;

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatServer_RegisterClient(GetAntiCheatServerHandle(WorldContextObject), &AntiCheatServerOptions));
	}

	return Result;
}

EOSResult UCoreAntiCheatServer::EOSAntiCheatServerUnregisterClient(UObject* WorldContextObject, FEOSAntiCheatServerUnregisterClientOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_InvalidParameters;

	if (GetAntiCheatServerHandle(WorldContextObject))
	{
		EOS_AntiCheatServer_UnregisterClientOptions AntiCheatServerOptions = {};
		AntiCheatServerOptions.ApiVersion = EOS_ANTICHEATSERVER_UNREGISTERCLIENT_API_LATEST;
		AntiCheatServerOptions.ClientHandle = Options.ClientHandle;

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatServer_UnregisterClient(GetAntiCheatServerHandle(WorldContextObject), &AntiCheatServerOptions));
	}

	return Result;
}

EOSResult UCoreAntiCheatServer::EOSAntiCheatServerReceiveMessageFromClient(UObject* WorldContextObject, FEOSAntiCheatServerReceiveMessageFromClientOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_InvalidParameters;

	if (GetAntiCheatServerHandle(WorldContextObject))
	{
		EOS_AntiCheatServer_ReceiveMessageFromClientOptions AntiCheatServerOptions = {};
		AntiCheatServerOptions.ApiVersion = EOS_ANTICHEATSERVER_RECEIVEMESSAGEFROMCLIENT_API_LATEST;
		AntiCheatServerOptions.ClientHandle = Options.ClientHandle;
		AntiCheatServerOptions.DataLengthBytes = Options.Data.Num();
		AntiCheatServerOptions.Data = Options.Data.GetData();

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatServer_ReceiveMessageFromClient(GetAntiCheatServerHandle(WorldContextObject), &AntiCheatServerOptions));
	}

	return Result;
}

EOSResult UCoreAntiCheatServer::EOSAntiCheatServerSetClientDetails(UObject* WorldContextObject, FEOSAntiCheatCommonSetClientDetailsOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_InvalidParameters;

	if (GetAntiCheatServerHandle(WorldContextObject))
	{
		EOS_AntiCheatCommon_SetClientDetailsOptions AntiCheatServerOptions = {};
		AntiCheatServerOptions.ApiVersion = Options.ApiVersion;
		AntiCheatServerOptions.ClientHandle = Options.ClientHandle;
		AntiCheatServerOptions.ClientFlags = static_cast<EOS_EAntiCheatCommonClientFlags>(Options.ClientFlags);
		AntiCheatServerOptions.ClientInputMethod = static_cast<EOS_EAntiCheatCommonClientInput>(Options.ClientInputMethod);

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatServer_SetClientDetails(GetAntiCheatServerHandle(WorldContextObject), &AntiCheatServerOptions));
	}

	return Result;
}

EOSResult UCoreAntiCheatServer::EOSAntiCheatServerSetGameSessionId(UObject* WorldContextObject, FEOSAntiCheatCommonSetGameSessionIdOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_InvalidParameters;

	if (GetAntiCheatServerHandle(WorldContextObject))
	{
		const FTCHARToUTF8 GameSessionId(*Options.GameSessionId);
		
		EOS_AntiCheatCommon_SetGameSessionIdOptions AntiCheatServerOptions = {};
		AntiCheatServerOptions.ApiVersion = Options.ApiVersion;
		AntiCheatServerOptions.GameSessionId = GameSessionId.Get();

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatServer_SetGameSessionId(GetAntiCheatServerHandle(WorldContextObject), &AntiCheatServerOptions));
	}

	return Result;
}

EOSResult UCoreAntiCheatServer::EOSAntiCheatServerSetClientNetworkState(UObject* WorldContextObject, FEOSAntiCheatServerSetClientNetworkStateOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_InvalidParameters;

	if (GetAntiCheatServerHandle(WorldContextObject))
	{
		EOS_AntiCheatServer_SetClientNetworkStateOptions AntiCheatServerOptions = {};
		AntiCheatServerOptions.ApiVersion = EOS_ANTICHEATSERVER_SETCLIENTNETWORKSTATE_API_LATEST;
		AntiCheatServerOptions.ClientHandle = Options.ClientHandle;
		AntiCheatServerOptions.bIsNetworkActive = Options.bIsNetworkActive;

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatServer_SetClientNetworkState(GetAntiCheatServerHandle(WorldContextObject), &AntiCheatServerOptions));
	}

	return Result;
}

EOSResult UCoreAntiCheatServer::EOSAntiCheatServerGetProtectMessageOutputLength(UObject* WorldContextObject, FEOSAntiCheatServerGetProtectMessageOutputLengthOptions Options, int32& OutBufferLengthBytes)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_InvalidParameters;

	OutBufferLengthBytes = 0;

	if (GetAntiCheatServerHandle(WorldContextObject))
	{
		EOS_AntiCheatServer_GetProtectMessageOutputLengthOptions AntiCheatServerOptions = {};
		AntiCheatServerOptions.ApiVersion = EOS_ANTICHEATSERVER_GETPROTECTMESSAGEOUTPUTLENGTH_API_LATEST;
		AntiCheatServerOptions.DataLengthBytes = Options.DataLengthBytes;
		uint32_t BufferLengthBytes = 0;

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatServer_GetProtectMessageOutputLength(GetAntiCheatServerHandle(WorldContextObject), &AntiCheatServerOptions, &BufferLengthBytes));

		if (Result == EOSResult::EOS_Success)
		{
			OutBufferLengthBytes = BufferLengthBytes;
		}
	}

	return Result;
}

EOSResult UCoreAntiCheatServer::EOSAntiCheatServerProtectMessage(UObject* WorldContextObject, FEOSAntiCheatServerProtectMessageOptions Options, TArray<uint8>& OutBuffer, int32& OutBufferLengthBytes)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_InvalidParameters;

	OutBuffer.Empty();
	OutBufferLengthBytes = 0;

	if (GetAntiCheatServerHandle(WorldContextObject))
	{
		EOS_AntiCheatServer_ProtectMessageOptions AntiCheatServerOptions = {};
		AntiCheatServerOptions.ApiVersion = EOS_ANTICHEATSERVER_PROTECTMESSAGE_API_LATEST;
		AntiCheatServerOptions.ClientHandle = Options.ClientHandle;
		AntiCheatServerOptions.DataLengthBytes = Options.DataLengthBytes;
		AntiCheatServerOptions.Data = Options.Data.GetData();
		AntiCheatServerOptions.OutBufferSizeBytes = Options.OutBufferSizeBytes;

		uint32_t BufferLengthBytes = 0;

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatServer_ProtectMessage(GetAntiCheatServerHandle(WorldContextObject), &AntiCheatServerOptions, Options.Data.GetData(), &BufferLengthBytes));

		if (Result == EOSResult::EOS_Success)
		{
			OutBuffer = Options.Data;
			OutBufferLengthBytes = BufferLengthBytes;
		}
	}

	return Result;
}

EOSResult UCoreAntiCheatServer::EOSAntiCheatServerUnprotectMessage(UObject* WorldContextObject, FEOSAntiCheatServerUnprotectMessageOptions Options, TArray<uint8>& OutBuffer, int32& OutBufferLengthBytes)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_InvalidParameters;

	OutBuffer.Empty();
	OutBufferLengthBytes = 0;

	if (GetAntiCheatServerHandle(WorldContextObject))
	{
		EOS_AntiCheatServer_UnprotectMessageOptions AntiCheatServerOptions = {};
		AntiCheatServerOptions.ApiVersion = EOS_ANTICHEATSERVER_UNPROTECTMESSAGE_API_LATEST;
		AntiCheatServerOptions.ClientHandle = Options.ClientHandle;
		AntiCheatServerOptions.DataLengthBytes = Options.DataLengthBytes;
		AntiCheatServerOptions.Data = Options.Data.GetData();
		AntiCheatServerOptions.OutBufferSizeBytes = Options.OutBufferSizeBytes;

		uint32_t BufferLengthBytes = 0;

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatServer_UnprotectMessage(GetAntiCheatServerHandle(WorldContextObject), &AntiCheatServerOptions, Options.Data.GetData(), &BufferLengthBytes));

		if (Result == EOSResult::EOS_Success)
		{
			OutBuffer = Options.Data;
			OutBufferLengthBytes = BufferLengthBytes;
		}
	}

	return Result;
}

EOSResult UCoreAntiCheatServer::EOSAntiCheatServerRegisterEvent(UObject* WorldContextObject, FEOSAntiCheatCommonRegisterEventOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_InvalidParameters;

	if (GetAntiCheatServerHandle(WorldContextObject))
	{
		EOS_AntiCheatCommon_RegisterEventOptions AntiCheatServerOptions = {};
		AntiCheatServerOptions.ApiVersion = Options.ApiVersion;

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatServer_RegisterEvent(GetAntiCheatServerHandle(WorldContextObject), &AntiCheatServerOptions));
	}

	return Result;
}

EOSResult UCoreAntiCheatServer::EOSAntiCheatServerLogEvent(UObject* WorldContextObject, FEOSAntiCheatCommonLogEventOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_InvalidParameters;

	if (GetAntiCheatServerHandle(WorldContextObject))
	{
		EOS_AntiCheatCommon_LogEventOptions AntiCheatServerOptions = {};
		AntiCheatServerOptions.ApiVersion = Options.ApiVersion;
		AntiCheatServerOptions.ClientHandle = Options.ClientHandle;
		AntiCheatServerOptions.EventId = Options.EventId;

		TArray<EOS_AntiCheatCommon_LogEventParamPair> Events;

		for (auto& Element : Options.Params)
		{
			EOS_AntiCheatCommon_LogEventParamPair Event;
			Event.ParamValueType = static_cast<EOS_EAntiCheatCommonEventParamType>(Element.ParamValueType);
			switch (Element.ParamValueType)
			{
			case EEOSEAntiCheatCommonEventParamType::EOS_ACCEPT_ClientHandle: Event.ParamValue.ClientHandle = Element.ParamValue.ClientHandle;
				break;
			case EEOSEAntiCheatCommonEventParamType::EOS_ACCEPT_String: Event.ParamValue.String = Element.ParamValue.String;
				break;
			case EEOSEAntiCheatCommonEventParamType::EOS_ACCEPT_UInt32: Event.ParamValue.UInt32 = Element.ParamValue.UInt32;
				break;
			case EEOSEAntiCheatCommonEventParamType::EOS_ACCEPT_Int32: Event.ParamValue.Int32 = Element.ParamValue.Int32;
				break;
			case EEOSEAntiCheatCommonEventParamType::EOS_ACCEPT_UInt64: Event.ParamValue.UInt64 = Element.ParamValue.UInt64;
				break;
			case EEOSEAntiCheatCommonEventParamType::EOS_ACCEPT_Int64: Event.ParamValue.Int64 = Element.ParamValue.Int64;
				break;
			case EEOSEAntiCheatCommonEventParamType::EOS_ACCEPT_Vector3f: Event.ParamValue.Vec3f = Element.ParamValue.Vec3f;
				break;
			case EEOSEAntiCheatCommonEventParamType::EOS_ACCEPT_Quat: Event.ParamValue.Quat = Element.ParamValue.Quat;
				break;
			}

			Events.Add(Event);
		}

		AntiCheatServerOptions.Params = Events.GetData();
		AntiCheatServerOptions.ParamsCount = Events.Num();

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatServer_LogEvent(GetAntiCheatServerHandle(WorldContextObject), &AntiCheatServerOptions));
	}

	return Result;
}

EOSResult UCoreAntiCheatServer::EOSAntiCheatServerLogGameRoundStart(UObject* WorldContextObject, FEOSAntiCheatCommonLogGameRoundStartOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_InvalidParameters;

	if (GetAntiCheatServerHandle(WorldContextObject))
	{
		const FTCHARToUTF8 SessionIdentifier(*Options.SessionIdentifier);
		const FTCHARToUTF8 LevelName(*Options.LevelName);
		const FTCHARToUTF8 ModeName(*Options.ModeName);
		
		EOS_AntiCheatCommon_LogGameRoundStartOptions AntiCheatServerOptions = {};
		AntiCheatServerOptions.ApiVersion = Options.ApiVersion;
		AntiCheatServerOptions.SessionIdentifier = SessionIdentifier.Get();
		AntiCheatServerOptions.LevelName = LevelName.Get();
		AntiCheatServerOptions.ModeName = ModeName.Get();
		AntiCheatServerOptions.RoundTimeSeconds = Options.RoundTimeSeconds;

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatServer_LogGameRoundStart(GetAntiCheatServerHandle(WorldContextObject), &AntiCheatServerOptions));
	}

	return Result;
}

EOSResult UCoreAntiCheatServer::EOSAntiCheatServerLogGameRoundEnd(UObject* WorldContextObject, FEOSAntiCheatCommonLogGameRoundEndOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_InvalidParameters;

	if (GetAntiCheatServerHandle(WorldContextObject))
	{
		EOS_AntiCheatCommon_LogGameRoundEndOptions AntiCheatServerOptions = {};
		AntiCheatServerOptions.ApiVersion = Options.ApiVersion;
		AntiCheatServerOptions.WinningTeamId = Options.WinningTeamId;

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatServer_LogGameRoundEnd(GetAntiCheatServerHandle(WorldContextObject), &AntiCheatServerOptions));
	}

	return Result;
}

EOSResult UCoreAntiCheatServer::EOSAntiCheatServerLogPlayerSpawn(UObject* WorldContextObject, FEOSAntiCheatCommonLogPlayerSpawnOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_InvalidParameters;

	if (GetAntiCheatServerHandle(WorldContextObject))
	{
		EOS_AntiCheatCommon_LogPlayerSpawnOptions AntiCheatServerOptions = {};
		AntiCheatServerOptions.ApiVersion = Options.ApiVersion;
		AntiCheatServerOptions.SpawnedPlayerHandle = Options.SpawnedPlayerHandle;
		AntiCheatServerOptions.TeamId = Options.TeamId;
		AntiCheatServerOptions.CharacterId = Options.CharacterId;

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatServer_LogPlayerSpawn(GetAntiCheatServerHandle(WorldContextObject), &AntiCheatServerOptions));
	}

	return Result;
}

EOSResult UCoreAntiCheatServer::EOSAntiCheatServerLogPlayerDespawn(UObject* WorldContextObject, FEOSAntiCheatCommonLogPlayerDespawnOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_InvalidParameters;

	if (GetAntiCheatServerHandle(WorldContextObject))
	{
		EOS_AntiCheatCommon_LogPlayerDespawnOptions AntiCheatServerOptions = {};
		AntiCheatServerOptions.ApiVersion = Options.ApiVersion;
		AntiCheatServerOptions.DespawnedPlayerHandle = Options.DespawnedPlayerHandle;

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatServer_LogPlayerDespawn(GetAntiCheatServerHandle(WorldContextObject), &AntiCheatServerOptions));
	}

	return Result;
}

EOSResult UCoreAntiCheatServer::EOSAntiCheatServerLogPlayerRevive(UObject* WorldContextObject, FEOSAntiCheatCommonLogPlayerReviveOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_InvalidParameters;

	if (GetAntiCheatServerHandle(WorldContextObject))
	{
		EOS_AntiCheatCommon_LogPlayerReviveOptions AntiCheatServerOptions = {};
		AntiCheatServerOptions.ApiVersion = Options.ApiVersion;
		AntiCheatServerOptions.ReviverPlayerHandle = Options.ReviverPlayerHandle;
		AntiCheatServerOptions.RevivedPlayerHandle = Options.RevivedPlayerHandle;

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatServer_LogPlayerRevive(GetAntiCheatServerHandle(WorldContextObject), &AntiCheatServerOptions));
	}

	return Result;
}

EOSResult UCoreAntiCheatServer::EOSAntiCheatServerLogPlayerTick(UObject* WorldContextObject, FEOSAntiCheatCommonLogPlayerTickOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_InvalidParameters;

	if (GetAntiCheatServerHandle(WorldContextObject))
	{
		EOS_AntiCheatCommon_LogPlayerTickOptions AntiCheatServerOptions = {};
		AntiCheatServerOptions.ApiVersion = Options.ApiVersion;
		AntiCheatServerOptions.PlayerHandle = Options.PlayerHandle;

		EOS_AntiCheatCommon_Vec3f PlayerPosition = Options.PlayerPosition;
		AntiCheatServerOptions.PlayerPosition = &PlayerPosition;

		EOS_AntiCheatCommon_Quat PlayerViewRotation = Options.PlayerViewRotation;
		AntiCheatServerOptions.PlayerViewRotation = &PlayerViewRotation;

		AntiCheatServerOptions.PlayerPosition = &PlayerPosition;
		AntiCheatServerOptions.PlayerViewRotation = &PlayerViewRotation;
		AntiCheatServerOptions.bIsPlayerViewZoomed = Options.bIsPlayerViewZoomed;
		AntiCheatServerOptions.PlayerHealth = Options.PlayerHealth;
		AntiCheatServerOptions.PlayerMovementState = static_cast<EOS_EAntiCheatCommonPlayerMovementState>(Options.PlayerMovementState);

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatServer_LogPlayerTick(GetAntiCheatServerHandle(WorldContextObject), &AntiCheatServerOptions));
	}

	return Result;
}

EOSResult UCoreAntiCheatServer::EOSAntiCheatServerLogPlayerUseWeapon(UObject* WorldContextObject, FEOSAntiCheatCommonLogPlayerUseWeaponOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_InvalidParameters;

	if (GetAntiCheatServerHandle(WorldContextObject))
	{
		const FTCHARToUTF8 WeaponName(*Options.UseWeaponData.WeaponName);
		
		EOS_AntiCheatCommon_LogPlayerUseWeaponOptions AntiCheatServerOptions = {};
		AntiCheatServerOptions.ApiVersion = Options.ApiVersion;
		EOS_AntiCheatCommon_LogPlayerUseWeaponData WeaponData;
		WeaponData.PlayerHandle = Options.UseWeaponData.PlayerHandle;

		EOS_AntiCheatCommon_Vec3f PlayerPosition = Options.UseWeaponData.PlayerPosition;
		WeaponData.PlayerPosition = &PlayerPosition;

		EOS_AntiCheatCommon_Quat PlayerViewRotation = Options.UseWeaponData.PlayerViewRotation;
		WeaponData.PlayerViewRotation = &PlayerViewRotation;

		WeaponData.bIsPlayerViewZoomed = Options.UseWeaponData.bIsPlayerViewZoomed;
		WeaponData.bIsMeleeAttack = Options.UseWeaponData.bIsMeleeAttack;
		WeaponData.WeaponName = WeaponName.Get();
		AntiCheatServerOptions.UseWeaponData = &WeaponData;

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatServer_LogPlayerUseWeapon(GetAntiCheatServerHandle(WorldContextObject), &AntiCheatServerOptions));
	}

	return Result;
}

EOSResult UCoreAntiCheatServer::EOSAntiCheatServerLogPlayerUseAbility(UObject* WorldContextObject, FEOSAntiCheatCommonLogPlayerUseAbilityOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_InvalidParameters;

	if (GetAntiCheatServerHandle(WorldContextObject))
	{
		EOS_AntiCheatCommon_LogPlayerUseAbilityOptions AntiCheatServerOptions = {};
		AntiCheatServerOptions.ApiVersion = Options.ApiVersion;
		AntiCheatServerOptions.PlayerHandle = Options.PlayerHandle;
		AntiCheatServerOptions.AbilityId = Options.AbilityId;
		AntiCheatServerOptions.AbilityDurationMs = Options.AbilityDurationMs;
		AntiCheatServerOptions.AbilityCooldownMs = Options.AbilityCooldownMs;

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatServer_LogPlayerUseAbility(GetAntiCheatServerHandle(WorldContextObject), &AntiCheatServerOptions));
	}

	return Result;
}

EOSResult UCoreAntiCheatServer::EOSAntiCheatServerLogPlayerTakeDamage(UObject* WorldContextObject, FEOSAntiCheatCommonLogPlayerTakeDamageOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_InvalidParameters;

	if (GetAntiCheatServerHandle(WorldContextObject))
	{
		const FTCHARToUTF8 WeaponName(*Options.PlayerUseWeaponData.WeaponName);
		
		EOS_AntiCheatCommon_LogPlayerTakeDamageOptions AntiCheatServerOptions = {};
		AntiCheatServerOptions.ApiVersion = Options.ApiVersion;
		AntiCheatServerOptions.VictimPlayerHandle = Options.VictimPlayerHandle;
		EOS_AntiCheatCommon_Vec3f VictimPlayerPosition = Options.VictimPlayerPosition;
		AntiCheatServerOptions.VictimPlayerPosition = &VictimPlayerPosition;

		EOS_AntiCheatCommon_Quat VictimPlayerViewRotation = Options.VictimPlayerViewRotation;
		AntiCheatServerOptions.VictimPlayerViewRotation = &VictimPlayerViewRotation;
		AntiCheatServerOptions.AttackerPlayerHandle = Options.AttackerPlayerHandle;

		EOS_AntiCheatCommon_Vec3f AttackerPlayerPosition = Options.AttackerPlayerPosition;
		AntiCheatServerOptions.AttackerPlayerPosition = &AttackerPlayerPosition;

		EOS_AntiCheatCommon_Quat AttackerPlayerViewRotation = Options.AttackerPlayerViewRotation;
		AntiCheatServerOptions.AttackerPlayerViewRotation = &AttackerPlayerViewRotation;
		AntiCheatServerOptions.bIsHitscanAttack = Options.bIsHitscanAttack;
		AntiCheatServerOptions.bHasLineOfSight = Options.bHasLineOfSight;
		AntiCheatServerOptions.bIsCriticalHit = Options.bIsCriticalHit;
		AntiCheatServerOptions.DamageTaken = Options.DamageTaken;
		AntiCheatServerOptions.HealthRemaining = Options.HealthRemaining;
		AntiCheatServerOptions.DamageSource = static_cast<EOS_EAntiCheatCommonPlayerTakeDamageSource>(Options.DamageSource);
		AntiCheatServerOptions.DamageType = static_cast<EOS_EAntiCheatCommonPlayerTakeDamageType>(Options.DamageType);
		AntiCheatServerOptions.DamageResult = static_cast<EOS_EAntiCheatCommonPlayerTakeDamageResult>(Options.DamageResult);

		EOS_AntiCheatCommon_LogPlayerUseWeaponData WeaponData;
		WeaponData.PlayerHandle = Options.PlayerUseWeaponData.PlayerHandle;

		EOS_AntiCheatCommon_Vec3f PlayerPosition = Options.PlayerUseWeaponData.PlayerPosition;
		WeaponData.PlayerPosition = &PlayerPosition;

		EOS_AntiCheatCommon_Quat PlayerViewRotation = Options.PlayerUseWeaponData.PlayerViewRotation;
		WeaponData.PlayerViewRotation = &PlayerViewRotation;

		WeaponData.bIsPlayerViewZoomed = Options.PlayerUseWeaponData.bIsPlayerViewZoomed;
		WeaponData.bIsMeleeAttack = Options.PlayerUseWeaponData.bIsMeleeAttack;
		WeaponData.WeaponName = WeaponName.Get();

		AntiCheatServerOptions.PlayerUseWeaponData = &WeaponData;
		AntiCheatServerOptions.TimeSincePlayerUseWeaponMs = Options.TimeSincePlayerUseWeaponMs;

		Result = EOSHelpers::ToEOSCoreResult(EOS_AntiCheatServer_LogPlayerTakeDamage(GetAntiCheatServerHandle(WorldContextObject), &AntiCheatServerOptions));
	}

	return Result;
}
