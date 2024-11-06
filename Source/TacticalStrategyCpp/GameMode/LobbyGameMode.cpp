// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "MultiplayerSessionSubsystem.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if(PostLoginWaitTime.IsValid())
		PostLoginWaitTime.Invalidate();

	UMultiplayerSessionSubsystem* MultiplayerSubsystem;	
	
	// if the user wants to have control over the number of players in the match.
	if(UGameInstance* GameInstance = GetGameInstance())
	{
		MultiplayerSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();
		check(MultiplayerSubsystem);

		MultiplayerSubsystem->DesiredNumPublicConnections;
	}
	
	
	GetWorldTimerManager().SetTimer(PostLoginWaitTime, [&]
	{
		if(UWorld* World = GetWorld())
		{
			bUseSeamlessTravel = true;
			FString MainGameMapPath{};
			EMultiplayerModes MatchType = EMultiplayerModes::EMM_Teams;

			if(MultiplayerSubsystem)
				MatchType = MultiplayerSubsystem->DesiredMatchType;
			
			if(MatchType == EMultiplayerModes::EMM_FreeForAll)
				MainGameMapPath = FreeForAllMapPath;
			else if (MatchType == EMultiplayerModes::EMM_Teams)
				MainGameMapPath = TeamsMapPath;
			else if (MatchType == EMultiplayerModes::EMM_CaptureFlag)
				MainGameMapPath = CaptureFlagMapPath;
			else
				MainGameMapPath = FreeForAllMapPath;
			
			MainGameMapPath = FString::Printf(TEXT("%s?listen"), *MainGameMapPath);

			if(GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,
					FString::Printf(TEXT("loading level %s"), *MainGameMapPath));
			
			World->ServerTravel(FString(MainGameMapPath));
		}
	}, LobbyWaitTime, false);

	if(GameState)
	{
		const int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
		
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1,60.f,FColor::Yellow,
			FString::Printf(TEXT("Players in Game %d"), NumberOfPlayers));

			if(const APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>())
			{
				const FString PlayerName = PlayerState->GetPlayerName();
				GEngine->AddOnScreenDebugMessage(-1,60.f,FColor::Cyan,
					FString::Printf(TEXT("%s has joined the game"), *PlayerName));
			}
		}
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	if(GEngine)
	{
		if(const APlayerState* PlayerState = Exiting->GetPlayerState<APlayerState>())
		{
			const FString PlayerName = PlayerState->GetPlayerName();
			GEngine->AddOnScreenDebugMessage(-1,60.f,FColor::Cyan,
					FString::Printf(TEXT("%s has exited the game"), *PlayerName));
		}
	}
}
