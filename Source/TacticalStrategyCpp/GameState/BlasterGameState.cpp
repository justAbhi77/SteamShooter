// Fill out your copyright notice in the Description page of Project Settings.

#include "BlasterGameState.h"
#include "Net/UnrealNetwork.h"
#include "TacticalStrategyCpp/PlayerController/BlasterPlayerController.h"
#include "TacticalStrategyCpp/PlayerState/BlasterPlayerState.h"

ABlasterGameState::ABlasterGameState()
{}

void ABlasterGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABlasterGameState, TopScoringPlayers);
	DOREPLIFETIME(ABlasterGameState, RedTeamScore);
	DOREPLIFETIME(ABlasterGameState, BlueTeamScore);
}

void ABlasterGameState::UpdateTopScore(ABlasterPlayerState* ScoringPlayer)
{
	// Add the scoring player as a top scorer if no top scorers exist yet
	if(TopScoringPlayers.Num() == 0)
	{
		TopScoringPlayers.Add(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();
	}
	// If player's score matches the top score, add them if they aren't already in the list
	else if(ScoringPlayer->GetScore() == TopScore)
		TopScoringPlayers.AddUnique(ScoringPlayer);
	// If player's score exceeds the current top score, update the top score and reset the list
	else if(ScoringPlayer->GetScore() > TopScore)
	{
		TopScoringPlayers.Empty();
		TopScoringPlayers.Add(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();
	}
}

void ABlasterGameState::OnRep_RedTeamScore()
{
	PlayerController = PlayerController == nullptr ? Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController()) : PlayerController;
	if(PlayerController)
		PlayerController->SetHudRedTeamScore(RedTeamScore);
}

void ABlasterGameState::OnRep_BlueTeamScore()
{
	PlayerController = PlayerController == nullptr ? Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController()) : PlayerController;
	if(PlayerController)
		PlayerController->SetHudBlueTeamScore(BlueTeamScore);
}

void ABlasterGameState::RedTeamScores()
{
	++RedTeamScore;
	if(HasAuthority())
		OnRep_RedTeamScore();
}

void ABlasterGameState::BlueTeamScores()
{
	++BlueTeamScore;
	if(HasAuthority())
		OnRep_BlueTeamScore();
}
