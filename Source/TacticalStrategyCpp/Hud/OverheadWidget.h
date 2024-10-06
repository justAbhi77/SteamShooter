// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverheadWidget.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALSTRATEGYCPP_API UOverheadWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UTextBlock* DisplayText;

	void SetDisplayText(const FString& TextToDisplay) const;

	UFUNCTION(BlueprintCallable)
	void ShowPlayerNetRole(const APawn* InPawn) const;
	
	UFUNCTION(BlueprintCallable)
	void ShowPlayerName(const FString& TextToDisplay) const;

protected:	
	virtual void NativeDestruct() override; // used instead of OnLevelRemoveFromWorld
};
