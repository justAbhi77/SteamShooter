/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "RTC/EOSRTC.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLogging.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UCoreRTC
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

void UCoreRTC::Deinitialize()
{
	LogVerbose("");

	for (auto Element: m_DisconnectedCallbacks)
	{
		EOS_RTC_RemoveNotifyDisconnected(GetRTCHandle(this), Element.Key);
	}

	for (auto Element: m_ParticipantStatusChangedCallbacks)
	{
		EOS_RTC_RemoveNotifyParticipantStatusChanged(GetRTCHandle(this), Element.Key);
	}

	for (auto Element: m_RoomStatisticsUpdatedCallback)
	{
		EOS_RTC_RemoveNotifyRoomStatisticsUpdated(GetRTCHandle(this), Element.Key);
	}

	

	m_DisconnectedCallbacks.Empty();
	m_ParticipantStatusChangedCallbacks.Empty();
}

UCoreRTC* UCoreRTC::GetRTC(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (GetRTCHandle(WorldContextObject))
		{
			if (UWorld* World = WorldContextObject->GetWorld())
			{
				if (UGameInstance* GameInstance = World->GetGameInstance())
				{
					return GameInstance->GetSubsystem<UCoreRTC>();
				}
			}	
		}
	}
	return nullptr;
}

FEOSHRTCAudio UCoreRTC::EOSRTCGetAudioInterface(UObject* WorldContextObject)
{
	LogVeryVerbose("");

	FEOSHRTCAudio Result;

	if (GetRTCHandle(WorldContextObject))
	{
		Result = EOS_RTC_GetAudioInterface(GetRTCHandle(WorldContextObject));
	}

	return Result;
}

void UCoreRTC::EOSRTCJoinRoom(UObject* WorldContextObject, FJoinRoomOptions Options, const FOnJoinRoomCallback& Callback)
{
	LogVerbose("");

	if (GetRTCHandle(WorldContextObject))
	{
		FEOSRTCJoinRoomOptions Parameters(Options.RoomName, Options.ClientBaseUrl, Options.ParticipantToken);
		Parameters.LocalUserId = Options.LocalUserId;
		Parameters.Flags = Options.Flags;
		Parameters.bManualAudioInputEnabled = Options.bManualAudioInputEnabled;
		Parameters.bManualAudioOutputEnabled = Options.bManualAudioOutputEnabled;

		FJoinRoomCallback* CallbackObj = new FJoinRoomCallback(Callback);
		EOS_RTC_JoinRoom(GetRTCHandle(WorldContextObject), &Parameters, CallbackObj, Internal_OnJoinRoomCallback);
	}
}

void UCoreRTC::EOSRTCLeaveRoom(UObject* WorldContextObject, FLeaveRoomOptions Options, const FOnLeaveRoomCallback& Callback)
{
	LogVerbose("");

	if (GetRTCHandle(WorldContextObject))
	{
		FEOSRTCLeaveRoomOptions Parameters(Options.RoomName);
		Parameters.LocalUserId = Options.LocalUserId;

		FLeaveRoomCallback* CallbackObj = new FLeaveRoomCallback(Callback);
		EOS_RTC_LeaveRoom(GetRTCHandle(WorldContextObject), &Parameters, CallbackObj, Internal_OnLeaveRoomCallback);
	}
}

void UCoreRTC::EOSRTCBlockParticipant(UObject* WorldContextObject, FBlockParticipantOptions Options, const FOnBlockParticipantCallback& Callback)
{
	LogVerbose("");

	if (GetRTCHandle(WorldContextObject))
	{
		FEOSRTCBlockParticipantOptions Parameters(Options.RoomName);
		Parameters.LocalUserId = Options.LocalUserId;
		Parameters.ParticipantId = Options.ParticipantId;
		Parameters.bBlocked = Options.bBlocked;

		FBlockParticipantCallback* CallbackObj = new FBlockParticipantCallback(Callback);
		EOS_RTC_BlockParticipant(GetRTCHandle(WorldContextObject), &Parameters, CallbackObj, Internal_OnBlockParticipantCallback);
	}
}

FEOSNotificationId UCoreRTC::EOSRTCAddNotifyDisconnected(UObject* WorldContextObject, FAddNotifyDisconnectedOptions Options, const FOnDisconnectedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetRTCHandle(WorldContextObject))
	{
		FEOSRTCAddNotifyDisconnectedOptions Parameters(Options.RoomName);
		Parameters.LocalUserId = Options.LocalUserId;

		FDisconnectedCallback* CallbackObj = new FDisconnectedCallback(Callback);

		Id = EOS_RTC_AddNotifyDisconnected(GetRTCHandle(WorldContextObject), &Parameters, CallbackObj, ([](const EOS_RTC_DisconnectedCallbackInfo* Data)
		{
			const FDisconnectedCallback* CallbackObj = static_cast<FDisconnectedCallback*>(Data->ClientData);
			CallbackObj->m_Callback.ExecuteIfBound(*Data);
		}));

		GetRTC(WorldContextObject)->m_DisconnectedCallbacks.Add(Id, CallbackObj);
	}

	return Id;
}

void UCoreRTC::EOSRTCRemoveNotifyDisconnected(UObject* WorldContextObject, FEOSNotificationId NotificationId)
{
	LogVerbose("");

	if (GetRTCHandle(WorldContextObject))
	{
		EOS_RTC_RemoveNotifyDisconnected(GetRTCHandle(WorldContextObject), NotificationId);
		GetRTC(WorldContextObject)->m_DisconnectedCallbacks.Remove(NotificationId);
	}
}

FEOSNotificationId UCoreRTC::EOSRTCAddNotifyParticipantStatusChanged(UObject* WorldContextObject, FAddNotifyParticipantStatusChangedOptions Options, const FOnParticipantStatusChangedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetRTCHandle(WorldContextObject))
	{
		FEOSRTCAddNotifyParticipantStatusChangedOptions Parameters(Options.RoomName);
		Parameters.LocalUserId = Options.LocalUserId;

		FParticipantStatusChangedCallback* CallbackObj = new FParticipantStatusChangedCallback(Callback);

		Id = EOS_RTC_AddNotifyParticipantStatusChanged(GetRTCHandle(WorldContextObject), &Parameters, CallbackObj, ([](const EOS_RTC_ParticipantStatusChangedCallbackInfo* Data)
		{
			const FParticipantStatusChangedCallback* CallbackObj = static_cast<FParticipantStatusChangedCallback*>(Data->ClientData);
			CallbackObj->m_Callback.ExecuteIfBound(*Data);
		}));

		GetRTC(WorldContextObject)->m_ParticipantStatusChangedCallbacks.Add(Id, CallbackObj);
	}

	return Id;
}

void UCoreRTC::EOSRTCRemoveNotifyParticipantStatusChanged(UObject* WorldContextObject, FEOSNotificationId NotificationId)
{
	LogVerbose("");

	if (GetRTCHandle(WorldContextObject))
	{
		EOS_RTC_RemoveNotifyParticipantStatusChanged(GetRTCHandle(WorldContextObject), NotificationId);
		GetRTC(WorldContextObject)->m_ParticipantStatusChangedCallbacks.Remove(NotificationId);
	}
}

EOSResult UCoreRTC::EOSRTCSetSetting(UObject* WorldContextObject, FEOSRTCSetSettingOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_UnexpectedError;

	if (GetRTCHandle(WorldContextObject))
	{
		EOS_RTC_SetSettingOptions Parameters = {};
		Parameters.ApiVersion = EOS_RTC_SETSETTING_API_LATEST;

		const FTCHARToUTF8 SettingName(*Options.SettingName);
		const FTCHARToUTF8 SettingValue(*Options.SettingValue);

		Options.SettingName = SettingName.Get();
		Options.SettingValue = SettingValue.Get();

		Result = EOSHelpers::ToEOSCoreResult(EOS_RTC_SetSetting(GetRTCHandle(WorldContextObject), &Parameters));
	}

	return Result;
}

EOSResult UCoreRTC::EOSRTCSetRoomSetting(UObject* WorldContextObject, FEOSRTCSetRoomSettingOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_UnexpectedError;

	if (GetRTCHandle(WorldContextObject))
	{
		EOS_RTC_SetRoomSettingOptions Parameters = {};
		Parameters.ApiVersion = EOS_RTC_SETSETTING_API_LATEST;

		const FTCHARToUTF8 RoomName(*Options.RoomName);
		const FTCHARToUTF8 SettingName(*Options.SettingName);
		const FTCHARToUTF8 SettingValue(*Options.SettingValue);

		Options.RoomName = RoomName.Get();
		Options.SettingName = SettingName.Get();
		Options.SettingValue = SettingValue.Get();

		Result = EOSHelpers::ToEOSCoreResult(EOS_RTC_SetRoomSetting(GetRTCHandle(WorldContextObject), &Parameters));
	}

	return Result;
}

FEOSNotificationId UCoreRTC::EOSRTCAddNotifyRoomStatisticsUpdated(UObject* WorldContextObject, FEOSProductUserId LocalUserId, FString RoomName, const FOnRoomStatisticsUpdatedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId Id;

	if (GetRTCHandle(WorldContextObject))
	{
		FEOSRTCAddNotifyRoomStatisticsUpdatedOptions Parameters(RoomName);
		Parameters.LocalUserId = LocalUserId;

		FRoomStatisticsUpdatedCallback* CallbackObj = new FRoomStatisticsUpdatedCallback(Callback);

		Id = EOS_RTC_AddNotifyRoomStatisticsUpdated(GetRTCHandle(WorldContextObject), &Parameters, CallbackObj, ([](const EOS_RTC_RoomStatisticsUpdatedInfo* Data)
		{
			const FRoomStatisticsUpdatedCallback* CallbackObj = static_cast<FRoomStatisticsUpdatedCallback*>(Data->ClientData);
			CallbackObj->m_Callback.ExecuteIfBound(*Data);
		}));

		GetRTC(WorldContextObject)->m_RoomStatisticsUpdatedCallback.Add(Id, CallbackObj);
	}

	return Id;
}

void UCoreRTC::EOSRTCRemoveNotifyRoomStatisticsUpdated(UObject* WorldContextObject, FEOSNotificationId NotificationId)
{
	LogVerbose("");

	if (GetRTCHandle(WorldContextObject))
	{
		EOS_RTC_RemoveNotifyRoomStatisticsUpdated(GetRTCHandle(WorldContextObject), NotificationId);
		GetRTC(WorldContextObject)->m_RoomStatisticsUpdatedCallback.Remove(NotificationId);
	}
}

void UCoreRTC::Internal_OnJoinRoomCallback(const EOS_RTC_JoinRoomCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FJoinRoomCallback* CallbackObj = static_cast<FJoinRoomCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreRTC::Internal_OnLeaveRoomCallback(const EOS_RTC_LeaveRoomCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FLeaveRoomCallback* CallbackObj = static_cast<FLeaveRoomCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreRTC::Internal_OnBlockParticipantCallback(const EOS_RTC_BlockParticipantCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FBlockParticipantCallback* CallbackObj = static_cast<FBlockParticipantCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}
