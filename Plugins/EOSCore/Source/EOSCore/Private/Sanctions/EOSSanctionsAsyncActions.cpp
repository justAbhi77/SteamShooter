/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "Sanctions/EOSSanctionsAsyncActions.h"
#include "Sanctions/EOSSanctions.h"
#include "Core/EOSCorePluginPrivatePCH.h" 
#include "Core/EOSCoreLibrary.h"
#include "Core/EOSCoreLogging.h"

UEOSSanctionsQueryActivePlayerSanctions* UEOSSanctionsQueryActivePlayerSanctions::
EOSSanctionsQueryActivePlayerSanctionsAsync(UObject* WorldContextObject,
	FEOSSanctionsQueryActivePlayerSanctionsOptions options)
{
	if (UEOSCoreSubsystem::GetLobbyHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSSanctionsQueryActivePlayerSanctions>();
		AsyncObject->RegisterWithGameInstance(GameInstance);
		AsyncObject->Options = options;
		AsyncObject->m_WorldContextObject = WorldContextObject;

		return AsyncObject;
	}
	else
	{
		LogError("Plugin not initialized!");
	}

	return nullptr;
}

void UEOSSanctionsQueryActivePlayerSanctions::Activate()
{
	Super::Activate();
	UCoreSanctions::GetSanctions(m_WorldContextObject)->EOSSanctionsQueryActivePlayerSanctions(m_WorldContextObject, Options, Callback);
}
