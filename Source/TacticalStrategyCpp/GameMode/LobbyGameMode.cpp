// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if(GameState)
	{
		const int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();

		if(NumberOfPlayers == 2)
		{
			if(UWorld* World = GetWorld())
			{
				bUseSeamlessTravel = true;
				World->ServerTravel(FString("/Game/Maps/MainGame?listen"));
			}
		}
		
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
	
	if(GameState)
	{
		const int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
		
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1,60.f,FColor::Yellow,
			FString::Printf(TEXT("Players in Game %d"), NumberOfPlayers - 1));

			if(const APlayerState* PlayerState = Exiting->GetPlayerState<APlayerState>())
			{
				const FString PlayerName = PlayerState->GetPlayerName();
				GEngine->AddOnScreenDebugMessage(-1,60.f,FColor::Cyan,
												 FString::Printf(TEXT("%s has exited the game"), *PlayerName));
			}
		}
	}
}
