/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "Reports/EOSReports.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLogging.h"

UCoreReports* UCoreReports::GetReports(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (GetReportsHandle(WorldContextObject))
		{
			if (UWorld* World = WorldContextObject->GetWorld())
			{
				if (UGameInstance* GameInstance = World->GetGameInstance())
				{
					return GameInstance->GetSubsystem<UCoreReports>();
				}
			}	
		}
	}
	return nullptr;
}

void UCoreReports::EOSReportsSendPlayerBehaviorReport(UObject* WorldContextObject, FEOSReportsSendPlayerBehaviorReportOptions Options, const FOnSendPlayerBehaviorReportCompleteCallback& Callback)
{
	LogVerbose("");

	if (GetReportsHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedMessage(*Options.Message);
		const FTCHARToUTF8 ConvertedContext(*Options.Context);
		
		EOS_Reports_SendPlayerBehaviorReportOptions ReportOptions = {};
		ReportOptions.ApiVersion = Options.ApiVersion;
		ReportOptions.ReporterUserId = Options.ReporterUserId;
		ReportOptions.ReportedUserId = Options.ReportedUserId;
		ReportOptions.Category = static_cast<EOS_EPlayerReportsCategory>(Options.ReportCategory);
		ReportOptions.Message = ConvertedMessage.Get();
		ReportOptions.Context = ConvertedContext.Get();

		FSendPlayerBehaviorReportCallback* CallbackObj = new FSendPlayerBehaviorReportCallback({this, Callback});
		EOS_Reports_SendPlayerBehaviorReport(GetReportsHandle(WorldContextObject), &ReportOptions, CallbackObj, Internal_OnSendPlayerBehaviorReportCallback);
	}
}

void UCoreReports::Internal_OnSendPlayerBehaviorReportCallback(const EOS_Reports_SendPlayerBehaviorReportCompleteCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FSendPlayerBehaviorReportCallback* CallbackObj = static_cast<FSendPlayerBehaviorReportCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		const UCoreReports* Reports = CallbackObj->ReportsObject;
		check(Reports);

		if (Reports)
		{
			CallbackObj->Callback.ExecuteIfBound(*Data);
		}

		delete CallbackObj;
	}
}
