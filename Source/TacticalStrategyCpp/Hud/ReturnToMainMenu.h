// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReturnToMainMenu.generated.h"

DECLARE_DELEGATE(FMenuTornDown)

/**
 * 
 */
UCLASS()
class TACTICALSTRATEGYCPP_API UReturnToMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void MenuSetup();

	void MenuTearDown();

	FMenuTornDown OnMenuTornDown;

protected:
	virtual bool Initialize() override;

	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);
	
private:
	UPROPERTY(meta=(BindWidget))
	class UButton* ReturnButton;
	
	UPROPERTY(meta=(BindWidget))
	UButton* BackButton;

	UFUNCTION()
	void ReturnButtonClicked();
	
	UFUNCTION()
	void BackButtonClicked();

	UPROPERTY()
	class APlayerController* PlayerController;

	UPROPERTY()
	class UMultiplayerSessionSubsystem* MultiplayerSessionSubsystem;
};
