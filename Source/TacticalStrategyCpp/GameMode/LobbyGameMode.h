// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * Responsible for managing the game lobby, where players are waiting to join a match.
 * It handles player logins, logouts, and manages the transition from the lobby to the game modes.
 */
UCLASS()
class TACTICALSTRATEGYCPP_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	// Path for the Free-for-All map.
	UPROPERTY(EditAnywhere, Category = "Map Paths")
	FString FreeForAllMapPath;

	// Path for the Teams map.
	UPROPERTY(EditAnywhere, Category = "Map Paths")
	FString TeamsMapPath;

	// Path for the Capture-the-Flag map.
	UPROPERTY(EditAnywhere, Category = "Map Paths")
	FString CaptureFlagMapPath;

	// Timer handle to wait after a player has logged in before starting the match.
	FTimerHandle PostLoginWaitTime;

	// Duration (in seconds) for how long the lobby waits before starting the match.
	UPROPERTY(EditAnywhere, Category = "Lobby Settings")
	float LobbyWaitTime = 20;

	UPROPERTY()
	class UMultiplayerSessionSubsystem* MultiplayerSubsystem;

	/**
	 * Starts the match by transitioning from the lobby to the selected game mode.
	 * This function can trigger the appropriate map change depending on the selection in menu.
	 */
	UFUNCTION()
	void StartMatch();
};
