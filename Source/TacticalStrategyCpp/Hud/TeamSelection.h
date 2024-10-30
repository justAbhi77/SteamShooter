// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TeamSelection.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTeamSelectionChanged, ETeam, NewTeam);

/**
 * 
 */
UCLASS()
class TACTICALSTRATEGYCPP_API UTeamSelection : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	class UButton* RedTeam;
	
	UPROPERTY(meta=(BindWidget))
	UButton* BlueTeam;
	
	FOnTeamSelectionChanged OnTeamSelectionChanged;
	
	void MenuSetup();

	void MenuTearDown();
	
	UPROPERTY()
	class APlayerController* PlayerController;

protected:
	void TeamButtonClicked(const ETeam NewTeam);

	UFUNCTION()
	void OnRedButtonClicked();
	
	UFUNCTION()
	void OnBlueButtonClicked();
};
