/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "FindEOSSessionsCallbackProxy.h"
#include "EngineGlobals.h"
#include "Engine/Engine.h"
#include "OnlineSubsystemEOSCore.h"
#include "OnlineSessionEOSCore.h"
#include "OnlineSubsystemModuleEOSCore.h"
#include "OnlineSubsystemEOSCorePrivatePCH.h"

UFindEOSSessionsCallbackProxy::UFindEOSSessionsCallbackProxy()
	: Delegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnCompleted))
{
}

UFindEOSSessionsCallbackProxy* UFindEOSSessionsCallbackProxy::FindEOSSessions(UObject* WorldContextObject, TMap<FString, FEOSSessionSearchSetting> SearchSettings, int32 MaxResults, bool bIsLanQuery)
{
	LogVerbose("");
	
	auto* AsyncObject = NewObject<UFindEOSSessionsCallbackProxy>();
	AsyncObject->m_WorldContextObject = WorldContextObject;
	AsyncObject->m_MaxResults = MaxResults;
	AsyncObject->m_AdditionalSearchSettings = SearchSettings;
	AsyncObject->m_bIsLanQuery = bIsLanQuery;
	

	return AsyncObject;
}

void UFindEOSSessionsCallbackProxy::Activate()
{
	LogVerbose("");
	
	FOnlineSubsystemEOSCore* Subsystem = static_cast<FOnlineSubsystemEOSCore*>(Online::GetSubsystem(GEngine->GetWorldFromContextObject(m_WorldContextObject, EGetWorldErrorMode::ReturnNull), EOSCORE_SUBSYSTEM));

	if (Subsystem && Subsystem->m_SessionInterfacePtr)
	{
		DelegateHandle = Subsystem->m_SessionInterfacePtr->AddOnFindSessionsCompleteDelegate_Handle(Delegate);

		m_SearchSettings = MakeShareable(new FOnlineSessionSearch);

		m_SearchSettings->MaxSearchResults = m_MaxResults;
		FOnlineSearchSettings& Settings = m_SearchSettings->QuerySettings;

		for (auto& Element : m_AdditionalSearchSettings)
		{
			if (Element.Key.Len() == 0)
			{
				continue;
			}

			if (Element.Value.Data.IsType<bool>())
			{
				Settings.Set(FName(*Element.Key), Element.Value.Data.Get<bool>() ? 1 : 0, static_cast<EOnlineComparisonOp::Type>(Element.Value.ComparisonOp));
			}
			else if (Element.Value.Data.IsType<int32>())
			{
				Settings.Set(FName(*Element.Key), Element.Value.Data.Get<int32>(), static_cast<EOnlineComparisonOp::Type>(Element.Value.ComparisonOp));
			}
			else if (Element.Value.Data.IsType<FString>())
			{
				Settings.Set(FName(*Element.Key), Element.Value.Data.Get<FString>(), static_cast<EOnlineComparisonOp::Type>(Element.Value.ComparisonOp));
			}
		}

		m_SearchSettings->bIsLanQuery = m_bIsLanQuery;

		if (!Subsystem->m_SessionInterfacePtr->FindSessions(0, m_SearchSettings.ToSharedRef()))
		{
			OnCompleted(false);
		}
	}
}

void UFindEOSSessionsCallbackProxy::OnCompleted(bool bSuccessful)
{
	LogVerbose("");
	
	FOnlineSubsystemEOSCore* Subsystem = static_cast<FOnlineSubsystemEOSCore*>(Online::GetSubsystem(GEngine->GetWorldFromContextObject(m_WorldContextObject, EGetWorldErrorMode::ReturnNull), EOSCORE_SUBSYSTEM));

	if (Subsystem && Subsystem->m_SessionInterfacePtr)
	{
		Subsystem->m_SessionInterfacePtr->ClearOnFindSessionsCompleteDelegate_Handle(DelegateHandle);

		if (m_SearchSettings.IsValid())
		{
			TArray<FEOSSessionResult> Results;

			for (FOnlineSessionSearchResult& Element : m_SearchSettings->SearchResults)
			{
				TArray<FEOSSessionSetting> AdditionalSessionSettings;

				for (auto& SettingsElement : Element.Session.SessionSettings.Settings)
				{
					FEOSSessionSetting Setting;
					Setting.Key = SettingsElement.Key.ToString();

					switch (SettingsElement.Value.Data.GetType())
					{
					case EOnlineKeyValuePairDataType::Bool:
						bool bValue;
						SettingsElement.Value.Data.GetValue(bValue);
						Setting.Data.Set<bool>(bValue);
						break;
					case EOnlineKeyValuePairDataType::Int32:
						int32 IntValue;
						SettingsElement.Value.Data.GetValue(IntValue);
						Setting.Data.Set<int32>(IntValue);
						break;
					case EOnlineKeyValuePairDataType::Int64:
						int64 Int64Value;
						SettingsElement.Value.Data.GetValue(Int64Value);
						Setting.Data.Set<int32>(Int64Value);
						break;
					case EOnlineKeyValuePairDataType::String:
						FString Value;
						SettingsElement.Value.Data.GetValue(Value);
						Setting.Data.Set<FString>(Value);
						break;
					}

					AdditionalSessionSettings.Add(Setting);
				}

				FBlueprintSessionResult BPResult;
				BPResult.OnlineResult = Element;
				FString CustomSessionName;
				BPResult.OnlineResult.Session.SessionSettings.Get("OWNINGNAME", CustomSessionName);

				if (CustomSessionName.Len() > 0)
				{
					BPResult.OnlineResult.Session.OwningUserName = CustomSessionName;
				}

				FString ResultText = FString::Printf(TEXT("Found a session (%s). Ping is %d"), *BPResult.OnlineResult.Session.OwningUserName, Element.PingInMs);

				FFrame::KismetExecutionMessage(*ResultText, ELogVerbosity::Log);

				m_SearchResults.Add(BPResult);

				Results.Add(FEOSSessionResult(BPResult, AdditionalSessionSettings));
			}

			OnCallback.Broadcast(Results);
		}
	}
	SetReadyToDestroy();
}
