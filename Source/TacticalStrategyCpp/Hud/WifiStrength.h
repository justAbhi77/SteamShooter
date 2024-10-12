// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WifiStrength.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALSTRATEGYCPP_API UWifiStrength : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly ,meta=(BindWidget))
	class UImage* WifiSymbol;
};
