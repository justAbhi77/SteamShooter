// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterHud.h"

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

void ABlasterHud::DrawCrosshair(UTexture2D* Texture, const FVector2D ViewportCenter, const FVector2D Spread,
	FLinearColor CrosshairColor)
{
	const float TextureWidth = Texture->GetSizeX(), TextureHeight = Texture->GetSizeY();

	const FVector2D TextureDrawPoint(
		ViewportCenter.X - (TextureWidth / 2.f) + Spread.X,
		ViewportCenter.Y - (TextureHeight / 2.f) + Spread.Y
	);

	DrawTexture(Texture, TextureDrawPoint.X, TextureDrawPoint.Y, TextureWidth, TextureHeight,
		0.f, 0.f, 1.f, 1.f, CrosshairColor);
}
