/**
* Copyright (C) 2017-2022 | eelDev AB
*/

#include "Slate/EOSCoreEditorStyle.h"
#include "EOSCoreEditorTypes.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"
#include "EditorStyleSet.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/CoreStyle.h"
#include "Styling/SlateTypes.h"

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )

TSharedPtr<FSlateStyleSet> FEOSCoreEditorStyle::StyleInstance = NULL;

void FEOSCoreEditorStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FEOSCoreEditorStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FEOSCoreEditorStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("EOSCoreEditor"));
	return StyleSetName;
}

TSharedRef<FSlateStyleSet> FEOSCoreEditorStyle::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet("EOSCoreEditor"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin(PLUGIN_NAME)->GetBaseDir() / TEXT("Resources"));

	FScrollBarStyle ScrollbarStyle = FEditorStyle::Get().GetWidgetStyle<FScrollBarStyle>("Scrollbar");
	FButtonStyle UpdaterCoreButton = FEditorStyle::Get().GetWidgetStyle<FButtonStyle>("FlatButton.Success");
	FButtonStyle UpdaterCoreButtonRed = FEditorStyle::Get().GetWidgetStyle<FButtonStyle>("FlatButton.Success");
	FButtonStyle UpdaterCoreButtonGreen = FEditorStyle::Get().GetWidgetStyle<FButtonStyle>("FlatButton.Success");
	
	ScrollbarStyle.NormalThumbImage = *Style->GetBrush("EOSCoreEditor.ButtonCategoryBackground");
	ScrollbarStyle.HoveredThumbImage = *Style->GetBrush("EOSCoreEditor.ButtonCategoryBackgroundHovered");
	
	FWindowStyle WindowStyle = const_cast<FWindowStyle&>(FCoreStyle::Get().GetWidgetStyle<FWindowStyle>("Window"));
    	FSlateColor WindowBackgroundColor(FLinearColor::White);
    	FSlateBrush WindowBackgroundMain(IMAGE_BRUSH("Common/NoiseBackground", FVector2D(74, 74), FLinearColor::White, ESlateBrushTileType::Both));
    	FSlateBrush WindowBackgroundChild(IMAGE_BRUSH("Common/NoiseBackground", FVector2D(64, 64), FLinearColor::White, ESlateBrushTileType::Both));
    	WindowStyle
    	.SetBackgroundColor(WindowBackgroundColor)
        .SetBackgroundBrush(*FEditorStyle::GetBrush("Menu.Background"))
        .SetChildBackgroundBrush(*FEditorStyle::GetBrush("Menu.Background"));

	UpdaterCoreButton.Normal.TintColor = FLinearColor(.7,.7,.7);
	UpdaterCoreButton.Hovered.TintColor = FLinearColor(1,1,1);
	UpdaterCoreButton.Pressed.TintColor = UpdaterCoreButton.Normal.TintColor;
	
	UpdaterCoreButtonRed.Normal.TintColor = FLinearColor(.5,.1,.1);
	UpdaterCoreButtonRed.Hovered.TintColor = FLinearColor(.8,0,0);
	UpdaterCoreButtonRed.Pressed.TintColor = UpdaterCoreButtonRed.Normal.TintColor;

	UpdaterCoreButtonGreen.Normal.TintColor = FLinearColor(.1,.5,.1);
	UpdaterCoreButtonGreen.Hovered.TintColor = FLinearColor(0,.7,0);
	UpdaterCoreButtonGreen.Pressed.TintColor = UpdaterCoreButtonGreen.Normal.TintColor;

	Style->Set("EOSCoreEditor.UpdaterCoreButton", UpdaterCoreButton);
	Style->Set("EOSCoreEditor.UpdaterCoreButtonRed", UpdaterCoreButtonRed);
	Style->Set("EOSCoreEditor.UpdaterCoreButtonGreen", UpdaterCoreButtonGreen);
	Style->Set("EOSCoreEditor.TitleBackground", new IMAGE_BRUSH(TEXT("TitleBackground"), FVector2D(200.f, 80.f)));
	Style->Set("EOSCoreEditor.ContentBackground", new FSlateBrush(*FStyleDefaults::GetNoBrush()));
	Style->Set("EOSCoreEditor.MainMenuBackground", new IMAGE_BRUSH(TEXT("MainMenuBackground"), FVector2D(500.f, 80.f)));
	Style->Set("EOSCoreEditor.ScrollbarStyle", ScrollbarStyle);
	Style->Set("EOSCoreEditor.Window", WindowStyle);

	auto ContentBackground = Style->GetBrush("EOSCoreEditor.ContentBackground");
	const_cast<FSlateBrush*>(ContentBackground)->TintColor = FSlateColor(FLinearColor(0.038204, 0.038204, 0.038204, 1));
 
	return Style;
}

#undef IMAGE_BRUSH

void FEOSCoreEditorStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FEOSCoreEditorStyle::Get()
{
	return *StyleInstance;
}
