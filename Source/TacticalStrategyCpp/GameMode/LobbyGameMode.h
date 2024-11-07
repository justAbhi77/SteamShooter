// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALSTRATEGYCPP_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;
	
	UPROPERTY(EditAnywhere);
	FString FreeForAllMapPath;
	
	UPROPERTY(EditAnywhere);
	FString TeamsMapPath;

	UPROPERTY(EditAnywhere);
	FString CaptureFlagMapPath;

	FTimerHandle PostLoginWaitTime;
	
	UPROPERTY(EditAnywhere);
	float LobbyWaitTime = 20;

	UPROPERTY()
	class UMultiplayerSessionSubsystem* MultiplayerSubsystem;	

	UFUNCTION()
	void StartMatch();
};
