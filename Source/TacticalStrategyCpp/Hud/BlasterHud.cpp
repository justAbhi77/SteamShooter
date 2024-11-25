﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "BlasterHud.h"
#include "Announcement.h"
#include "CharacterOverlay.h"
#include "ElimAnnouncement.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"

void ABlasterHud::DrawHUD()
{
	Super::DrawHUD();

	if(GEngine)
	{
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);

		FVector2D Spread;
		const float SpreadScaled = CrosshairSpreadMax * HudPackage.CrosshairSpread;

		const FLinearColor CrosshairsColor = HudPackage.CrosshairsColor;

		if(HudPackage.CrosshairsCenter)
		{
			Spread = FVector2D(0.f,0.f);
			DrawCrosshair(HudPackage.CrosshairsCenter, ViewportCenter, Spread, CrosshairsColor);
		}
		if(HudPackage.CrosshairsLeft)
		{
			Spread = FVector2D(-SpreadScaled,0.f);
			DrawCrosshair(HudPackage.CrosshairsLeft, ViewportCenter, Spread, CrosshairsColor);
		}
		if(HudPackage.CrosshairsRight)
		{
			Spread = FVector2D(SpreadScaled,0.f);
			DrawCrosshair(HudPackage.CrosshairsRight, ViewportCenter, Spread, CrosshairsColor);
		}
		if(HudPackage.CrosshairsTop)
		{
			Spread = FVector2D(0.f,-SpreadScaled);
			DrawCrosshair(HudPackage.CrosshairsTop, ViewportCenter, Spread, CrosshairsColor);
		}
		if(HudPackage.CrosshairsBottom)
		{
			Spread = FVector2D(0.f,SpreadScaled);
			DrawCrosshair(HudPackage.CrosshairsBottom, ViewportCenter, Spread, CrosshairsColor);
		}
	}
}

void ABlasterHud::AddAnnouncement()
{
	if(APlayerController* OwningPlayerController = GetOwningPlayerController(); OwningPlayerController && AnnouncementClass)
	{
		if(Announcement == nullptr)
			Announcement = CreateWidget<UAnnouncement>(OwningPlayerController, AnnouncementClass);
		Announcement->AddToViewport();
	}	
}

void ABlasterHud::AddElimAnnouncement(const FString& AttackerName, const FString& VictimName)
{
	OwningPlayer = OwningPlayer == nullptr ? GetOwningPlayerController() : OwningPlayer;
	if(OwningPlayer && ELimAnnouncementClass)
	{
		if(UElimAnnouncement* ElimAnnouncementWidget = CreateWidget<UElimAnnouncement>(OwningPlayer, ELimAnnouncementClass))
		{
			ElimAnnouncementWidget->SetElimAnnouncementText(AttackerName, VictimName);
			ElimAnnouncementWidget->AddToViewport();

			for(auto Msg : ElimMessages)
			{
				if(Msg && Msg->AnnouncementBox)
				{
					if(UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Msg->AnnouncementBox))
					{
						const FVector2D Position = CanvasSlot->GetPosition();
						const FVector2D NewPosition{ CanvasSlot->GetPosition().X, Position.Y - CanvasSlot->GetSize().Y};
						CanvasSlot->SetPosition(NewPosition);
					}
				}
			}
			ElimMessages.Add(ElimAnnouncementWidget);

			FTimerHandle ElimMsgTimer;
			FTimerDelegate ElimMsgDelegate;
			ElimMsgDelegate.BindUFunction(this, FName("ElimAnnouncementTimerFinished"),
				ElimAnnouncementWidget);
			GetWorldTimerManager().SetTimer(ElimMsgTimer, ElimMsgDelegate, ElimAnnouncementTime, false);
		}
	}
}

void ABlasterHud::AddCharacterOverlay()
{
	if(APlayerController* OwningPlayerController = GetOwningPlayerController(); OwningPlayerController && CharacterOverlayClass)
	{
		if(CharacterOverlay == nullptr)
			CharacterOverlay = CreateWidget<UCharacterOverlay>(OwningPlayerController, CharacterOverlayClass);
		CharacterOverlay->AddToViewport();
	}
}

void ABlasterHud::DrawCrosshair(UTexture2D* Texture, const FVector2D& ViewportCenter, const FVector2D& Spread,
                                const FLinearColor& CrosshairColor)
{
	if(Texture == nullptr) return;
	
	const float TextureWidth = Texture->GetSizeX(), TextureHeight = Texture->GetSizeY();

	const FVector2D TextureDrawPoint(
		ViewportCenter.X - (TextureWidth / 2.f) + Spread.X,
		ViewportCenter.Y - (TextureHeight / 2.f) + Spread.Y
	);

	DrawTexture(Texture, TextureDrawPoint.X, TextureDrawPoint.Y, TextureWidth, TextureHeight,
		0.f, 0.f, 1.f, 1.f, CrosshairColor);
}

void ABlasterHud::ElimAnnouncementTimerFinished(UElimAnnouncement* MsgToRemove)
{
	if(MsgToRemove)
		MsgToRemove->RemoveFromParent();
}
