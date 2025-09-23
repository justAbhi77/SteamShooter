/**
* Copyright (C) 2017-2022 | eelDev AB
*/

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "EOSCoreEditorTypes.h"
#include "Runtime/Launch/Resources/Version.h"

class FEOSCoreEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual bool SupportsDynamicReloading() override
	{
		return false;
	}

	virtual bool SupportsAutomaticShutdown() override
	{
		return false;
	}
public:
	void UpdateDisableVersionCheck(bool bNewState);
private:
	void OnRequestResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	static TArray<FString> ParseResponseItem(const TArray<FString>& ResponseData);
	void CreatePopup(FXmlItemData Data);
private:
	FDelegateHandle m_InitHandle;
};