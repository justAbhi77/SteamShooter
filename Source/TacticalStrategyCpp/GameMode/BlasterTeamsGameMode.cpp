// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterTeamsGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "TacticalStrategyCpp/GameState/BlasterGameState.h"
#include "TacticalStrategyCpp/PlayerState/BlasterPlayerState.h"

void ABlasterTeamsGameMode::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(MatchState == MatchState::WaitingTeamSelection)
	{
		
	}
}

void ABlasterTeamsGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	if(ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this)))
	{
		ABlasterPlayerState* BlasterPlayerState = NewPlayer->GetPlayerState<ABlasterPlayerState>();
		if(BlasterPlayerState && BlasterPlayerState->GetTeam() == ETeam::ET_NoTeam)
		{
			if(BlasterGameState->BlueTeam.Num() >= BlasterGameState->RedTeam.Num())
			{
				BlasterGameState->RedTeam.AddUnique(BlasterPlayerState);
				BlasterPlayerState->SetTeam(ETeam::ET_Red);
			}
			else
			{
				BlasterGameState->BlueTeam.AddUnique(BlasterPlayerState);
				BlasterPlayerState->SetTeam(ETeam::ET_Blue);				
			}
		}
	}
}

void ABlasterTeamsGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	
	if(ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this)))
	{
		ABlasterPlayerState* BlasterPlayerState = Exiting->GetPlayerState<ABlasterPlayerState>();
		if(BlasterPlayerState && BlasterPlayerState->GetTeam() == ETeam::ET_NoTeam)
		{
			if(BlasterGameState->RedTeam.Contains(BlasterPlayerState))
				BlasterGameState->RedTeam.Remove(BlasterPlayerState);
			
			if(BlasterGameState->BlueTeam.Contains(BlasterPlayerState))
				BlasterGameState->BlueTeam.Remove(BlasterPlayerState);
		}
	}
}

void ABlasterTeamsGameMode::AfterWaitingToStart()
{
	// if teams match let player choose team
	SetMatchState(MatchState::WaitingTeamSelection);
}

void ABlasterTeamsGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	if(ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this)))
	{
		for(auto PlayerState : BlasterGameState->PlayerArray)
		{
			ABlasterPlayerState* BlasterPlayerState = Cast<ABlasterPlayerState>(PlayerState.Get());
			if(BlasterPlayerState && BlasterPlayerState->GetTeam() == ETeam::ET_NoTeam)
			{
				if(BlasterGameState->BlueTeam.Num() >= BlasterGameState->RedTeam.Num())
				{
					BlasterGameState->RedTeam.AddUnique(BlasterPlayerState);
					BlasterPlayerState->SetTeam(ETeam::ET_Red);
				}
				else
				{
					BlasterGameState->BlueTeam.AddUnique(BlasterPlayerState);
					BlasterPlayerState->SetTeam(ETeam::ET_Blue);				
				}
			}
		}
	}
}
