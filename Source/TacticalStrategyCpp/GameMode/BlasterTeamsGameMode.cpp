// Fill out your copyright notice in the Description page of Project Settings.

#include "BlasterTeamsGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "TacticalStrategyCpp/GameState/BlasterGameState.h"
#include "TacticalStrategyCpp/PlayerController/BlasterPlayerController.h"
#include "TacticalStrategyCpp/PlayerState/BlasterPlayerState.h"

ABlasterTeamsGameMode::ABlasterTeamsGameMode()
{
	bTeamsMatch = true;
}

void ABlasterTeamsGameMode::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// if(MatchState == MatchState::WaitingTeamSelection)
}

void ABlasterTeamsGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	/*
	Automatically assign a team to the new player if they are not already assigned	
	if(ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this)))
	{
		ABlasterPlayerState* BlasterPlayerState = NewPlayer->GetPlayerState<ABlasterPlayerState>();
		if(BlasterPlayerState && BlasterPlayerState->GetTeam() == ETeam::ET_NoTeam)
		{
			// Assign the new player to the team with fewer members
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
	*/
}

void ABlasterTeamsGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	
	// Remove the player from the respective team when they exit the game
	if(ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this)))
	{
		ABlasterPlayerState* BlasterPlayerState = Exiting->GetPlayerState<ABlasterPlayerState>();
		if(BlasterPlayerState && BlasterPlayerState->GetTeam() == ETeam::ET_NoTeam)
		{
			// Remove the player from the team (if assigned) upon logout
			if(BlasterGameState->RedTeam.Contains(BlasterPlayerState))
				BlasterGameState->RedTeam.Remove(BlasterPlayerState);
			
			if(BlasterGameState->BlueTeam.Contains(BlasterPlayerState))
				BlasterGameState->BlueTeam.Remove(BlasterPlayerState);
		}
	}
}

bool ABlasterTeamsGameMode::HasMatchStarted() const
{
	// Prevent match start if still in team selection phase
	if (GetMatchState() == MatchState::WaitingTeamSelection)
		return false;
	return Super::HasMatchStarted();
}

float ABlasterTeamsGameMode::CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage)
{
	const ABlasterPlayerState* AttackerPlayerState = Attacker->GetPlayerState<ABlasterPlayerState>();
	const ABlasterPlayerState* VictimPlayerState = Victim->GetPlayerState<ABlasterPlayerState>();
	
	if(AttackerPlayerState == nullptr || VictimPlayerState == nullptr) return 0;

	// No damage if either player is not on a team
	if(VictimPlayerState->GetTeam() == ETeam::ET_NoTeam || AttackerPlayerState->GetTeam() == ETeam::ET_NoTeam)
		return 0;

	// No damage if the attacker and victim are on the same team
	if(AttackerPlayerState->GetTeam() == VictimPlayerState->GetTeam()) return 0;

	// Prevent self-damage in team mode
	if(VictimPlayerState == AttackerPlayerState) return 0;

	return BaseDamage;
}

void ABlasterTeamsGameMode::PlayerEliminated(ABlasterCharacter* ElimmedCharacter,
	class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	Super::PlayerEliminated(ElimmedCharacter, VictimController, AttackerController);

	// If a valid attacker player state exists, increment the corresponding team's score
	if(ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this)))
	{
		if(const ABlasterPlayerState* AttackerPlayerState = AttackerController ? Cast<ABlasterPlayerState>(AttackerController->PlayerState) : nullptr)
		{
			// Increment team scores based on attacker team
			if(AttackerPlayerState->GetTeam() == ETeam::ET_Blue)
				BlasterGameState->BlueTeamScores();
			
			if(AttackerPlayerState->GetTeam() == ETeam::ET_Red)
				BlasterGameState->RedTeamScores();
		}
	}
}

void ABlasterTeamsGameMode::AfterWaitingToStart()
{
	// Set the match state to WaitingTeamSelection when the waiting period is over
	SetMatchState(MatchState::WaitingTeamSelection);
}

void ABlasterTeamsGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	// logic to ensure all players are assigned to teams before match starts
	/*
	if(ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this)))
	{
		// Iterate through all players and assign them to teams if they are not yet assigned
		for(auto PlayerState : BlasterGameState->PlayerArray)
		{
			ABlasterPlayerState* BlasterPlayerState = Cast<ABlasterPlayerState>(PlayerState.Get());
			if(BlasterPlayerState && BlasterPlayerState->GetTeam() == ETeam::ET_NoTeam)
			{
				// Assign the player to the team with fewer members
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
	*/
}
