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
		float SpreadScaled = CrosshairSpreadMax * HudPackage.CrosshairSpread;
		
		if(HudPackage.CrosshairsCenter)
		{
			Spread = FVector2D(0.f,0.f);
			DrawCrosshair(HudPackage.CrosshairsCenter, ViewportCenter, Spread);
		}
		if(HudPackage.CrosshairsLeft)
		{
			Spread = FVector2D(-SpreadScaled,0.f);
			DrawCrosshair(HudPackage.CrosshairsLeft, ViewportCenter, Spread);
		}
		if(HudPackage.CrosshairsRight)
		{
			Spread = FVector2D(SpreadScaled,0.f);
			DrawCrosshair(HudPackage.CrosshairsRight, ViewportCenter, Spread);
		}
		if(HudPackage.CrosshairsTop)
		{
			Spread = FVector2D(0.f,-SpreadScaled);
			DrawCrosshair(HudPackage.CrosshairsTop, ViewportCenter, Spread);
		}
		if(HudPackage.CrosshairsBottom)
		{
			Spread = FVector2D(0.f,SpreadScaled);
			DrawCrosshair(HudPackage.CrosshairsBottom, ViewportCenter, Spread);
		}
	}
}

void ABlasterHud::DrawCrosshair(UTexture2D* Texture, const FVector2D ViewportCenter, const FVector2D Spread)
{
	const float TextureWidth = Texture->GetSizeX(), TextureHeight = Texture->GetSizeY();

	const FVector2D TextureDrawPoint(
		ViewportCenter.X - (TextureWidth / 2.f) + Spread.X,
		ViewportCenter.Y - (TextureHeight / 2.f) + Spread.Y
	);

	DrawTexture(Texture, TextureDrawPoint.X, TextureDrawPoint.Y, TextureWidth, TextureHeight,
		0.f, 0.f, 1.f, 1.f, FLinearColor::White);
}
