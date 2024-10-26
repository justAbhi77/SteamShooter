// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ElimAnnouncement.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALSTRATEGYCPP_API UElimAnnouncement : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetElimAnnouncementText(const FString& AttackerName, const FString& VictimName) const;

	UPROPERTY(meta=(BindWidget))
	class UHorizontalBox* AnnouncementBox;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* AnnouncementText;
};
