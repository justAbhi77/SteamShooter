// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterGameState.h"
#include "Net/UnrealNetwork.h"
#include "TacticalStrategyCpp/PlayerController/BlasterPlayerController.h"
#include "TacticalStrategyCpp/PlayerState/BlasterPlayerState.h"

ABlasterGameState::ABlasterGameState():
	RedTeamScore(0),
	BlueTeamScore(0),
	TopScore(0)
{
}

void ABlasterGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABlasterGameState, TopScoringPlayers);
	
	DOREPLIFETIME(ABlasterGameState, RedTeamScore);
	DOREPLIFETIME(ABlasterGameState, BlueTeamScore);
}

void ABlasterGameState::UpdateTopScore(ABlasterPlayerState* ScoringPlayer)
{
	if(TopScoringPlayers.Num() == 0)
	{
		TopScoringPlayers.Add(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();
	}
	else if(ScoringPlayer->GetScore() == TopScore)
		TopScoringPlayers.AddUnique(ScoringPlayer);
	else if(ScoringPlayer->GetScore() > TopScore)
	{
		TopScoringPlayers.Empty();
		TopScoringPlayers.Add(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();
	}
}

void ABlasterGameState::OnRep_RedTeamScore()
{
	if(ABlasterPlayerController* PlayerController = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController()))
		PlayerController->SetHudRedTeamScore(RedTeamScore);
}

void ABlasterGameState::OnRep_BlueTeamScore()
{
	if(ABlasterPlayerController* PlayerController = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController()))
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
