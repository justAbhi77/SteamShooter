// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if(PostLoginWaitTime.IsValid())
		PostLoginWaitTime.Invalidate();
	GetWorldTimerManager().SetTimer(PostLoginWaitTime, [&]
	{		
		if(UWorld* World = GetWorld())
		{
			bUseSeamlessTravel = true;
			MainGameMapPath = FString::Printf(TEXT("%s?listen"), *MainGameMapPath);
			World->ServerTravel(FString(MainGameMapPath));
		}
	}, 30, false, 10);

	if(GameState)
	{
		const int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();

		/*
		if(NumberOfPlayers == 2)
		{
		}
		*/
		
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1,60.f,FColor::Yellow,
			FString::Printf(TEXT("Players in Game %d"), NumberOfPlayers));

			if(const APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>())
			{
				const FString PlayerName = PlayerState->GetPlayerName();
				GEngine->AddOnScreenDebugMessage(-1,60.f,FColor::Cyan,
					FString::Printf(TEXT("%s has joined the game"), *PlayerName));
				
				if(NewPlayer->GetPawn())
				{
					if(ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(NewPlayer->GetPawn()))
					{
						BlasterCharacter->SetName(PlayerName);
					}
				}
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
