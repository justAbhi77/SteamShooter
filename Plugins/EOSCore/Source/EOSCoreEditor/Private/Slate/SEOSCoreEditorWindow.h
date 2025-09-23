/**
* Copyright (C) 2017-2022 | eelDev AB
*/

#pragma once

#include "CoreMinimal.h"
#include "EOSCoreEditorTypes.h"
#include "EOSCoreEditorModule.h"
#include "SWebBrowserView.h"

class SWebBrowserView;

class SUpdaterCoreWindow : public SCompoundWidget
{
public:
SLATE_BEGIN_ARGS(SUpdaterCoreWindow)
	{}
	
	SLATE_ATTRIBUTE(TSharedPtr<FEOSCoreEditorModule>, ModulePtr)
	SLATE_ATTRIBUTE(FXmlItemData, XmlData)
	SLATE_ATTRIBUTE(TSharedPtr<SWebBrowserView>, BrowserView)
	SLATE_END_ARGS()
public:
	void Construct(const FArguments& InArgs);
	~SUpdaterCoreWindow()
	{
		m_ModulePtr.Reset();
		m_BrowserView.Reset();
	}
private:
	TSharedPtr<FEOSCoreEditorModule> m_ModulePtr;
	TSharedPtr<SWebBrowserView> m_BrowserView;
	FXmlItemData m_XmlData;
};
