// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterOverlay.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALSTRATEGYCPP_API UCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* HealthBar;
	
	UPROPERTY(meta=(BindWidget))
	UProgressBar* ShieldBar;
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* HealthText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ShieldText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ScoreText; // ScoreAmount
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* DefeatText; // DefeatsAmount
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* WeaponAmmoText; // WeaponAmmoAmount
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CarriedAmmoText; // CarriedAmmoAmount
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* MatchCountDownText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* GrenadesText;

	UPROPERTY(meta=(BindWidget))
	class UWifiStrength* WifiStrength;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* HighPingAnimation;
};
