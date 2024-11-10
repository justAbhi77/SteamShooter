// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include "MultiplayerSessionSubsystem.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// If there is an existing timer, invalidate it to reset the timer for the new login.
	if(PostLoginWaitTime.IsValid())
		PostLoginWaitTime.Invalidate();

	// Attempt to retrieve the MultiplayerSubsystem from the GameInstance.
	if(UGameInstance* GameInstance = GetGameInstance())
	{
		// Ensure the MultiplayerSubsystem is valid and ready to be used.
		MultiplayerSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();
		check(MultiplayerSubsystem); // assert

		// Uncomment and implement this line to set or use the desired number of connections for the match.
		// Example: Fetch the desired number of players.
		// MultiplayerSubsystem->GetDesiredNumPublicConnections();
	}

	// Set a timer for the match to start after the lobby wait time.
	GetWorldTimerManager().SetTimer(PostLoginWaitTime, this, &ALobbyGameMode::StartMatch, LobbyWaitTime);

	// Check and update the GameState with the number of players in the game.
	if(GameState)
	{
		const int32 NumberOfPlayers = GameState->PlayerArray.Num();
		
		// Display the current number of players in the game on the screen.
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Yellow, FString::Printf(TEXT("Players in Game: %d"), NumberOfPlayers));

			// Log the player who has joined the game.
			if(const APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>())
			{
				const FString PlayerName = PlayerState->GetPlayerName();
				GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Cyan, FString::Printf(TEXT("%s has joined the game"), *PlayerName));
			}
		}
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	// Display which player has exited the game.
	if(GEngine)
	{
		if(const APlayerState* PlayerState = Exiting->GetPlayerState<APlayerState>())
		{
			const FString PlayerName = PlayerState->GetPlayerName();
			GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Cyan, FString::Printf(TEXT("%s has exited the game"), *PlayerName));
		}
	}
}

void ALobbyGameMode::StartMatch()
{
	// Ensure the MultiplayerSubsystem is initialized before proceeding.
	if(MultiplayerSubsystem == nullptr)
		if(UGameInstance* GameInstance = GetGameInstance())
		{
			MultiplayerSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();
			check(MultiplayerSubsystem);
		}

	// Only proceed with level transition if the world is valid.
	if(UWorld* World = GetWorld())
	{
		// Enable seamless travel for better player experience during map change.
		bUseSeamlessTravel = true;

		// Default the map to the FreeForAll map if no match type is selected.
		EMultiplayerModes MatchType = EMultiplayerModes::Emm_Teams;
		FString MainGameMapPath = "";

		// Retrieve the match type from the MultiplayerSubsystem if available.
		if(MultiplayerSubsystem)
			MatchType = MultiplayerSubsystem->GetDesiredMatchType();

		// Determine which map to load based on the match type.
		switch (MatchType)
		{
		case EMultiplayerModes::Emm_Teams:
			MainGameMapPath = TeamsMapPath;
			break;
		case EMultiplayerModes::Emm_CaptureFlag:
			MainGameMapPath = CaptureFlagMapPath;
			break;
		case EMultiplayerModes::Emm_FreeForAll:
			MainGameMapPath = FreeForAllMapPath;
			break;
		case EMultiplayerModes::Emm_Max:
		default:
			// Default to FreeForAll if the match type is invalid.
			MainGameMapPath = FreeForAllMapPath;
		}

		// Append ?listen to allow the server to listen for connections from clients.
		MainGameMapPath = FString::Printf(TEXT("%s?listen"), *MainGameMapPath);

		// Log the map loading action for debugging purposes.
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("Loading level: %s"), *MainGameMapPath));

		// Transition to the selected game map.
		World->ServerTravel(MainGameMapPath);
	}
}
