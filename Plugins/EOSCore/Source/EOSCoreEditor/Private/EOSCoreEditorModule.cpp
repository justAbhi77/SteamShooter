/**
* Copyright (C) 2017-2022 | eelDev AB
*/

#include "EOSCoreEditorModule.h"
#include "Slate/SEOSCoreEditorWindow.h"
#include "Slate/EOSCoreEditorStyle.h"
#include "SWebBrowser.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Docking/TabManager.h"
#include "Misc/ConfigCacheIni.h"

#define LOCTEXT_NAMESPACE "FEOSCoreEditorModule"

class UVersionUpdater : public UObject
{
public:
	void SaveIni()
	{
		GConfig->Flush(false, IniConfig);
		SaveConfig(CPF_Config, *IniConfig);
	};

	FString GetCachedVersion()
	{
		FString Result;

		GConfig->GetString(PLUGIN_INI_CONFIG_NAME, TEXT("CachedVersion"), Result, IniConfig);

		return Result;
	};

	bool IsVersionCheckDisabled()
	{
		bool bResult = false;

		GConfig->GetBool(PLUGIN_INI_CONFIG_NAME, TEXT("bDisableVersionChecking"), bResult, IniConfig);

		return bResult;
	};

	void UpdateCachedVersion(FString NewVersion)
	{
		GConfig->SetString(PLUGIN_INI_CONFIG_NAME, TEXT("CachedVersion"), *NewVersion, IniConfig);

		SaveIni();
	};

	void UpdateDisableVersionCheck(bool NewState)
	{
		GConfig->SetBool(PLUGIN_INI_CONFIG_NAME, TEXT("bDisableVersionChecking"), NewState, IniConfig);

		SaveIni();
	};
};

IMPLEMENT_MODULE(FEOSCoreEditorModule, EOSCoreEditor);

void FEOSCoreEditorModule::StartupModule()
{
	FEOSCoreEditorStyle::Initialize();
	FEOSCoreEditorStyle::ReloadTextures();
	
	UVersionUpdater* VersionChecker = GetMutableDefault<UVersionUpdater>();

	if (VersionChecker->IsVersionCheckDisabled())
	{
		UE_LOG(LogTemp, Log, TEXT("(%s) Plugin update check is disabled"), PLUGIN_NAME);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("(%s) Checking for plugin updates"), PLUGIN_NAME);

		m_InitHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([this](float)
		{

#if ENGINE_MINOR_VERSION > 25 || ENGINE_MAJOR_VERSION == 5
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> m_HTTPRequest = FHttpModule::Get().CreateRequest();
#else 
	TSharedRef<IHttpRequest> m_HTTPRequest = FHttpModule::Get().CreateRequest();
#endif

			m_HTTPRequest->SetHeader(TEXT("Content-Type"), TEXT("text/html; charset=utf-8"));
			m_HTTPRequest->SetURL(PLUGIN_URL);
			m_HTTPRequest->SetVerb("GET");
			m_HTTPRequest->OnProcessRequestComplete().BindRaw(this, &FEOSCoreEditorModule::OnRequestResponse);
			m_HTTPRequest->ProcessRequest();

			return false;
		}), 3.0f);
	}
}

void FEOSCoreEditorModule::ShutdownModule()
{
#if !PLATFORM_MAC
	FEOSCoreEditorStyle::Shutdown();
#endif
	FTicker::GetCoreTicker().RemoveTicker(m_InitHandle);
}

void FEOSCoreEditorModule::UpdateDisableVersionCheck(bool bNewState)
{
	UVersionUpdater* VersionChecker = GetMutableDefault<UVersionUpdater>();

	VersionChecker->UpdateDisableVersionCheck(bNewState);
}

void FEOSCoreEditorModule::OnRequestResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully)
	{
		const FString Result = Response->GetContentAsString();

		TArray<FString> Array;
		Result.ParseIntoArray(Array, TEXT("\n"));

		// check for new update
		for (auto& Element : Array)
		{
			if (Element.Contains("lastBuildDate"))
			{
				bool bNewVersion = false;

				// format the string
				const FString FormattedString = PrepareXmlString(Element);
				const FString HashedVersion = FMD5::HashAnsiString(*FormattedString);

				UVersionUpdater* VersionChecker = GetMutableDefault<UVersionUpdater>();

				bNewVersion = VersionChecker->GetCachedVersion() != HashedVersion;

				if (bNewVersion)
				{
					UE_LOG(LogTemp, Log, TEXT("(%s) New plugin update"), PLUGIN_NAME);

					VersionChecker->UpdateCachedVersion(HashedVersion);

					const FXmlItemData ParsedXml(ParseResponseItem(Array));
					CreatePopup(ParsedXml);
				}
				else
				{
					UE_LOG(LogTemp, Log, TEXT("(%s) No new plugin update"), PLUGIN_NAME);
				}

				break;
			}
		}
	}

//	m_HTTPRequest->OnProcessRequestComplete().Unbind();
}

TArray<FString> FEOSCoreEditorModule::ParseResponseItem(const TArray<FString>& ResponseData)
{
	TArray<FString> ItemArray;

	int32 FirstIndex = 0;
	int32 SecondIndex = 0;

	for (int32 i = 0; i < ResponseData.Num(); i++)
	{
		if (ResponseData[i].Contains("<item>"))
		{
			FirstIndex = i;
			break;
		}
	}

	for (int32 i = 0; i < ResponseData.Num(); i++)
	{
		if (ResponseData[i].Contains("</item>"))
		{
			SecondIndex = i;
			break;
		}
	}

	for (int32 i = FirstIndex; i < SecondIndex; i++)
	{
		ItemArray.Add(ResponseData[i]);
	}

	return ItemArray;
}

void FEOSCoreEditorModule::CreatePopup(FXmlItemData Data)
{
	TSharedPtr<SWindow> UpdaterCoreWindow = SNew(SWindow).Title(LOCTEXT("LatestUpdateText", "Latest Plugin Update")).SupportsMaximize(false).SupportsMinimize(false).MinHeight(550.f).Style(&FEOSCoreEditorStyle::Get().GetWidgetStyle<FWindowStyle>("EOSCoreEditor.Window")).MinWidth(800.f);

	const TSharedPtr<SWebBrowserView> BrowserWidget = SNew(SWebBrowserView).InitialURL(Data.Link + "?print=print");

	UpdaterCoreWindow->SetContent(SNew(SUpdaterCoreWindow).ModulePtr(MakeShareable(this)).XmlData(Data).BrowserView(BrowserWidget));

	FSlateApplication::Get().AddWindowAsNativeChild(UpdaterCoreWindow.ToSharedRef(), FGlobalTabmanager::Get()->GetRootWindow().ToSharedRef());
}

#undef LOCTEXT_NAMESPACE
