// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BlasterHud.generated.h"

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()

public:	
	UPROPERTY()
	class UTexture2D* CrosshairsCenter;	
	UPROPERTY()
	UTexture2D* CrosshairsLeft;
	UPROPERTY()
	UTexture2D* CrosshairsRight;
	UPROPERTY()
	UTexture2D* CrosshairsTop;
	UPROPERTY()
	UTexture2D* CrosshairsBottom;

	float CrosshairSpread;

	FLinearColor CrosshairsColor;
};

UCLASS()
class TACTICALSTRATEGYCPP_API ABlasterHud : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;	

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> CharacterOverlayClass;

	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;

	void AddCharacterOverlay();
	
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> AnnouncementClass;

	UPROPERTY()
	class UAnnouncement* Announcement;

	void AddAnnouncement();

	void AddElimAnnouncement(const FString& AttackerName, const FString& VictimName);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class APlayerController* OwningPlayer;
	
	FHUDPackage HudPackage;

	void DrawCrosshair(UTexture2D* Texture, const FVector2D& ViewportCenter, const FVector2D& Spread,
								const FLinearColor& CrosshairColor);

	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.f;	

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UElimAnnouncement> ELimAnnouncementClass;

	UPROPERTY(EditAnywhere)
	float ElimAnnouncementTime = 5;

	UFUNCTION()
	void ElimAnnouncementTimerFinished(UElimAnnouncement* MsgToRemove);

	UPROPERTY()
	TArray<UElimAnnouncement*> ElimMessages;

public:
	FORCEINLINE void SetHudPackage(const FHUDPackage& Package)
	{
		HudPackage = Package;
	}
};
