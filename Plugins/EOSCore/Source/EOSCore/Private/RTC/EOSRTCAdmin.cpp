/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "RTC/EOSRTCAdmin.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLogging.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UCoreAdmin
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

struct FQueryJoinRoomTokenOptions : public EOS_RTCAdmin_QueryJoinRoomTokenOptions
{
	TArray<EOS_ProductUserId> PointerArray;
	TArray<char*> Addresses;

	FQueryJoinRoomTokenOptions(FString roomName, TArray<FEOSProductUserId> targetUserIds, TArray<FString> targetUserIpAddresses)
		: EOS_RTCAdmin_QueryJoinRoomTokenOptions()
	{
		ApiVersion = EOS_RTCADMIN_QUERYJOINROOMTOKEN_API_LATEST;
		TargetUserIdsCount = targetUserIds.Num();

		if (!roomName.IsEmpty())
		{
			RoomName = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(RoomName), TCHAR_TO_UTF8(*roomName), 256);
		}

		if (TargetUserIdsCount > 0)
		{
			PointerArray.AddZeroed(TargetUserIdsCount);
			Addresses.AddZeroed(TargetUserIdsCount);

			for (int32 i = 0; i < targetUserIds.Num(); i++)
			{
				PointerArray[i] = targetUserIds[i];
				Addresses[i] = new char[64];

				if (targetUserIpAddresses.IsValidIndex(i))
				{
					FCStringAnsi::Strncpy(Addresses[i], TCHAR_TO_UTF8(*targetUserIpAddresses[i]), 64);
				}
				else
				{
					Addresses[i] = NULL;
				}
			}

			TargetUserIpAddresses = const_cast<const char**>(Addresses.GetData());
		}
	}

	~FQueryJoinRoomTokenOptions()
	{
		delete[] RoomName;

		for (auto& Element : Addresses)
		{
			delete[] Element;
		}
	}
};

UCoreRTCAdmin* UCoreRTCAdmin::GetRTCAdmin(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (GetRTCAdminHandle(WorldContextObject))
		{
			if (UWorld* World = WorldContextObject->GetWorld())
			{
				if (UGameInstance* GameInstance = World->GetGameInstance())
				{
					return GameInstance->GetSubsystem<UCoreRTCAdmin>();
				}
			}	
		}
	}
	return nullptr;
}

void UCoreRTCAdmin::EOSRTCAdminQueryJoinRoomToken(UObject* WorldContextObject, FEOSQueryJoinRoomTokenOptions Options, const FOnQueryJoinRoomTokenCompleteCallback& Callback)
{
	LogVerbose("");

	if (GetRTCAdminHandle(WorldContextObject))
	{
		FQueryJoinRoomTokenOptions Parameters(Options.RoomName, Options.TargetUserIds, Options.TargetUserIpAddresses);
		Parameters.LocalUserId = Options.LocalUserId;

		FQueryJoinRoomTokenCallback* CallbackObj = new FQueryJoinRoomTokenCallback(Callback);
		EOS_RTCAdmin_QueryJoinRoomToken(GetRTCAdminHandle(WorldContextObject), &Parameters, CallbackObj, Internal_OnQueryJoinRoomTokenCallback);
	}
}

EOSResult UCoreRTCAdmin::EOSRTCAdminCopyUserTokenByIndex(UObject* WorldContextObject, FEOSCopyUserTokenByIndexOptions Options, FEOSUserToken& OutUserToken)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_UnexpectedError;

	OutUserToken = FEOSUserToken();

	if (GetRTCAdminHandle(WorldContextObject))
	{
		EOS_RTCAdmin_CopyUserTokenByIndexOptions Parameters = {};
		Parameters.ApiVersion = EOS_RTCADMIN_COPYUSERTOKENBYINDEX_API_LATEST;
		Parameters.UserTokenIndex = Options.UserTokenIndex;
		Parameters.QueryId = Options.QueryId;

		EOS_RTCAdmin_UserToken* UserToken = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_RTCAdmin_CopyUserTokenByIndex(GetRTCAdminHandle(WorldContextObject), &Parameters, &UserToken));

		if (Result == EOSResult::EOS_Success)
		{
			OutUserToken = *UserToken;
		}
	}

	return Result;
}

EOSResult UCoreRTCAdmin::EOSRTCAdminCopyUserTokenByUserId(UObject* WorldContextObject, FEOSCopyUserTokenByUserIdOptions Options, FEOSUserToken& OutUserToken)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_UnexpectedError;

	OutUserToken = FEOSUserToken();

	if (GetRTCAdminHandle(WorldContextObject))
	{
		EOS_RTCAdmin_CopyUserTokenByUserIdOptions Parameters = {};
		Parameters.ApiVersion = EOS_RTCADMIN_COPYUSERTOKENBYUSERID_API_LATEST;
		Parameters.TargetUserId = Options.TargetUserId;
		Parameters.QueryId = Options.QueryId;

		EOS_RTCAdmin_UserToken* UserToken = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_RTCAdmin_CopyUserTokenByUserId(GetRTCAdminHandle(WorldContextObject), &Parameters, &UserToken));

		if (Result == EOSResult::EOS_Success)
		{
			OutUserToken = *UserToken;
		}
	}

	return Result;
}

void UCoreRTCAdmin::EOSRTCAdminKick(UObject* WorldContextObject, FEOSKickOptions Options, const FOnKickCompleteCallback& Callback)
{
	LogVerbose("");

	if (GetRTCAdminHandle(WorldContextObject))
	{
		FKickOptions Parameters(Options.RoomName);
		Parameters.TargetUserId = Options.TargetUserId;

		FKickCallback* CallbackObj = new FKickCallback(Callback);
		EOS_RTCAdmin_Kick(GetRTCAdminHandle(WorldContextObject), &Parameters, CallbackObj, Internal_OnKickCallback);
	}
}

void UCoreRTCAdmin::EOSRTCAdminSetParticipantHardMute(UObject* WorldContextObject, FEOSSetParticipantHardMuteOptions Options, const FOnSetParticipantHardMuteCompleteCallback& Callback)
{
	LogVerbose("");

	if (GetRTCAdminHandle(WorldContextObject))
	{
		FSetParticipantHardMuteOptions Parameters(Options.RoomName);
		Parameters.TargetUserId = Options.TargetUserId;
		Parameters.bMute = Options.bMute;

		FSetParticipantHardMuteCallback* CallbackObj = new FSetParticipantHardMuteCallback(Callback);
		EOS_RTCAdmin_SetParticipantHardMute(GetRTCAdminHandle(WorldContextObject), &Parameters, CallbackObj, Internal_OnSetParticipantHardMuteCallback);
	}
}

void UCoreRTCAdmin::Internal_OnQueryJoinRoomTokenCallback(const EOS_RTCAdmin_QueryJoinRoomTokenCompleteCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FQueryJoinRoomTokenCallback* CallbackObj = static_cast<FQueryJoinRoomTokenCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreRTCAdmin::Internal_OnKickCallback(const EOS_RTCAdmin_KickCompleteCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FKickCallback* CallbackObj = static_cast<FKickCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreRTCAdmin::Internal_OnSetParticipantHardMuteCallback(const EOS_RTCAdmin_SetParticipantHardMuteCompleteCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FSetParticipantHardMuteCallback* CallbackObj = static_cast<FSetParticipantHardMuteCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}
