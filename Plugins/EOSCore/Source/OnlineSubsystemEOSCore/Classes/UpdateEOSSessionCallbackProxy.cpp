/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "UpdateEOSSessionCallbackProxy.h"
#include "OnlineSessionEOSCore.h"
#include "OnlineSubsystemEOSCore.h"
#include "OnlineSubsystemModuleEOSCore.h"
#include "OnlineSubsystemEOSCorePrivatePCH.h"

UUpdateEOSSessionCallbackProxy::UUpdateEOSSessionCallbackProxy()
	: Delegate(FOnUpdateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCompleted))
{
}

UUpdateEOSSessionCallbackProxy* UUpdateEOSSessionCallbackProxy::Update_EOS_Session(UObject* WorldContextObject, TMap<FString, FEOSSessionSearchSetting> Settings, FString SessionName, int32 PublicConnections)
{
	LogVerbose("");
	
	auto* AsyncObject = NewObject<UUpdateEOSSessionCallbackProxy>();
	AsyncObject->m_WorldContextObject = WorldContextObject;
	AsyncObject->m_Settings = Settings;
	AsyncObject->m_SessionName = SessionName;
	AsyncObject->m_PublicConnections = PublicConnections;

	return AsyncObject;
}

void UUpdateEOSSessionCallbackProxy::Activate()
{
	LogVerbose("");
	
	FOnlineSubsystemEOSCore* Subsystem = static_cast<FOnlineSubsystemEOSCore*>(Online::GetSubsystem(GEngine->GetWorldFromContextObject(m_WorldContextObject, EGetWorldErrorMode::ReturnNull), EOSCORE_SUBSYSTEM));

	if (Subsystem && Subsystem->m_SessionInterfacePtr)
	{
		if (Subsystem->m_SessionInterfacePtr->GetNumSessions() > 0)
		{
			FOnlineSessionSettings* Settings = Subsystem->m_SessionInterfacePtr->GetSessionSettings(NAME_GameSession);

			if (Settings)
			{
				DelegateHandle = Subsystem->m_SessionInterfacePtr->AddOnUpdateSessionCompleteDelegate_Handle(Delegate);

				Settings->NumPublicConnections = m_PublicConnections;
				Settings->Settings.Add("SESSION_NAME", *m_SessionName);

				FOnlineSessionSetting* SessionNameSetting = Settings->Settings.Find("SESSION_NAME");

				if (SessionNameSetting)
				{
					SessionNameSetting->Data = *m_SessionName;
				}

				for (auto& Element : m_Settings)
				{
					if (Element.Key.Len() == 0)
					{
						continue;
					}

					FOnlineSessionSetting* OldSetting = nullptr;
					
					OldSetting = Settings->Settings.Find(*Element.Key);

					if (OldSetting)
					{
						LogVerbose("Updated old setting");
						
						if (Element.Value.Data.IsType<bool>())
						{
							OldSetting->Data = Element.Value.Data.Get<bool>() ? 1 : 0;
						}
						else if (Element.Value.Data.IsType<int32>())
						{
							OldSetting->Data = Element.Value.Data.Get<int32>();
						}
						else if (Element.Value.Data.IsType<FString>())
						{
							OldSetting->Data = Element.Value.Data.Get<FString>();
						}
					}
					else
					{
						LogVerbose("Added new setting");
						
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

						Settings->Set(FName(*Element.Key), Setting);
					}
				}

				if (!Subsystem->m_SessionInterfacePtr->UpdateSession(NAME_GameSession, *Settings, true))
				{
					OnCompleted(NAME_GameSession, false);
				}
			}
			else
			{
				LogError("No session settings found");
			}
		}
		else
		{
			LogError("No active sessions found");
		}
	}
}

void UUpdateEOSSessionCallbackProxy::OnCompleted(FName SessionName, bool bWasSuccessful)
{
	LogVerbose("");
	
	FOnlineSubsystemEOSCore* Subsystem = static_cast<FOnlineSubsystemEOSCore*>(Online::GetSubsystem(GEngine->GetWorldFromContextObject(m_WorldContextObject, EGetWorldErrorMode::ReturnNull), EOSCORE_SUBSYSTEM));

	if (Subsystem && Subsystem->m_SessionInterfacePtr)
	{
		Subsystem->m_SessionInterfacePtr->ClearOnUpdateSessionCompleteDelegate_Handle(DelegateHandle);

		OnCallback.Broadcast();
	}

	SetReadyToDestroy();
}
