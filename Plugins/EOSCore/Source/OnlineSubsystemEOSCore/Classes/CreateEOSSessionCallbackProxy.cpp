/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "CreateEOSSessionCallbackProxy.h"
#include "EngineGlobals.h"
#include "Engine/Engine.h"
#include "OnlineSubsystemEOSCore.h"
#include "OnlineSessionEOSCore.h"
#include "OnlineSubsystemModuleEOSCore.h"
#include "OnlineSubsystemEOSCorePrivatePCH.h"

//////////////////////////////////////////////////////////////////////////
// UCreateEOSSessionCallbackProxy

UCreateEOSSessionCallbackProxy::UCreateEOSSessionCallbackProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, m_CreateCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateCompleted))
	, m_StartCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartCompleted))
	, m_NumPublicConnections(1)
{}

UCreateEOSSessionCallbackProxy* UCreateEOSSessionCallbackProxy::Create_EOS_Session(UObject* WorldContextObject, TMap<FString, FEOSSessionSetting> SessionSettings, FString SessionName, int32 PublicConnections, bool bIsLanMatch)
{
	LogVerbose("");
	
	UCreateEOSSessionCallbackProxy* Proxy = NewObject<UCreateEOSSessionCallbackProxy>();
	Proxy->m_NumPublicConnections = PublicConnections;
	Proxy->m_WorldContextObject = WorldContextObject;
	Proxy->m_SessionName = SessionName;
	Proxy->m_EOSSessionSettings = SessionSettings;
	Proxy->m_bIsLanMatch = bIsLanMatch;
	return Proxy;
}

void UCreateEOSSessionCallbackProxy::Activate()
{
	LogVerbose("");
	
	FOnlineSubsystemEOSCore* Subsystem = static_cast<FOnlineSubsystemEOSCore*>(Online::GetSubsystem(GEngine->GetWorldFromContextObject(m_WorldContextObject, EGetWorldErrorMode::ReturnNull), EOSCORE_SUBSYSTEM));

	if (Subsystem && Subsystem->m_SessionInterfacePtr)
	{
		m_CreateCompleteDelegateHandle = Subsystem->m_SessionInterfacePtr->AddOnCreateSessionCompleteDelegate_Handle(m_CreateCompleteDelegate);

		FOnlineSessionSettings Settings;
		Settings.NumPublicConnections = m_NumPublicConnections;
		Settings.bShouldAdvertise = true;
		Settings.bAllowJoinInProgress = true;
		Settings.bIsLANMatch = m_bIsLanMatch;
		Settings.bUsesPresence = true;
		Settings.bAllowJoinViaPresence = true;

		Settings.Settings.Add("SESSION_NAME", *m_SessionName);

		for (auto& Element : m_EOSSessionSettings)
		{
			if (Element.Key.Len() == 0)
			{
				continue;
			}

			FOnlineSessionSetting Setting;
			Setting.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineService;

			if (Element.Value.Data.IsType<bool>())
			{
				Setting.Data.SetValue(Element.Value.Data.Get<bool>() ? 1 : 0);
			}
			else if (Element.Value.Data.IsType<int32>())
			{
				Setting.Data.SetValue(Element.Value.Data.Get<int32>());
			}
			else if (Element.Value.Data.IsType<FString>())
			{
				Setting.Data.SetValue(Element.Value.Data.Get<FString>());
			}

			Settings.Set(FName(*Element.Key), Setting);
		}

		Subsystem->m_SessionInterfacePtr->CreateSession(0, NAME_GameSession, Settings);
		
		return;
	}
	else
	{
		FFrame::KismetExecutionMessage(TEXT("Sessions not supported by Online Subsystem"), ELogVerbosity::Warning);
	}

	OnFailure.Broadcast();
}

void UCreateEOSSessionCallbackProxy::OnCreateCompleted(FName SessionName, bool bWasSuccessful)
{
	LogVerbose("");
	
	FOnlineSubsystemEOSCore* Subsystem = static_cast<FOnlineSubsystemEOSCore*>(Online::GetSubsystem(GEngine->GetWorldFromContextObject(m_WorldContextObject, EGetWorldErrorMode::ReturnNull), EOSCORE_SUBSYSTEM));

	if (Subsystem && Subsystem->m_SessionInterfacePtr)
	{
		Subsystem->m_SessionInterfacePtr->ClearOnCreateSessionCompleteDelegate_Handle(m_CreateCompleteDelegateHandle);

		if (bWasSuccessful)
		{
			m_StartCompleteDelegateHandle = Subsystem->m_SessionInterfacePtr->AddOnStartSessionCompleteDelegate_Handle(m_StartCompleteDelegate);
			Subsystem->m_SessionInterfacePtr->StartSession(NAME_GameSession);

			return;
		}
	}

	if (!bWasSuccessful)
	{
		OnFailure.Broadcast();
	}
}

void UCreateEOSSessionCallbackProxy::OnStartCompleted(FName SessionName, bool bWasSuccessful)
{
	LogVerbose("");
	
	FOnlineSubsystemEOSCore* Subsystem = static_cast<FOnlineSubsystemEOSCore*>(Online::GetSubsystem(GEngine->GetWorldFromContextObject(m_WorldContextObject, EGetWorldErrorMode::ReturnNull), EOSCORE_SUBSYSTEM));

	if (Subsystem && Subsystem->m_SessionInterfacePtr)
	{
		Subsystem->m_SessionInterfacePtr->ClearOnStartSessionCompleteDelegate_Handle(m_StartCompleteDelegateHandle);
	}

	if (bWasSuccessful)
	{
		OnSuccess.Broadcast();
	}
	else
	{
		OnFailure.Broadcast();
	}
}
