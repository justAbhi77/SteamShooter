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

	if(MatchState == MatchState::WaitingTeamSelection)
	{
		
	}
}

void ABlasterTeamsGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	/*
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
	*/
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

bool ABlasterTeamsGameMode::HasMatchStarted() const
{
	if (GetMatchState() == MatchState::WaitingTeamSelection)
	{
		return false;
	}
	return Super::HasMatchStarted();
}

float ABlasterTeamsGameMode::CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage)
{
	const ABlasterPlayerState* AttackerPlayerState = Attacker->GetPlayerState<ABlasterPlayerState>();
	const ABlasterPlayerState* VictimPlayerState = Victim->GetPlayerState<ABlasterPlayerState>();
	
	if(AttackerPlayerState == nullptr || VictimPlayerState == nullptr) return 0;

	if(VictimPlayerState->GetTeam() == ETeam::ET_NoTeam || AttackerPlayerState->GetTeam() == ETeam::ET_NoTeam)
		return 0;

	if(AttackerPlayerState->GetTeam() == VictimPlayerState->GetTeam()) return 0;

	if(VictimPlayerState == AttackerPlayerState) return 0; // prevent self damage in teams mode

	return BaseDamage;
}

void ABlasterTeamsGameMode::PlayerEliminated(ABlasterCharacter* ElimmedCharacter,
	class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	Super::PlayerEliminated(ElimmedCharacter, VictimController, AttackerController);
	if(ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this)))
	{
		ABlasterPlayerState* AttackerPlayerState = AttackerController ?
			Cast<ABlasterPlayerState>(AttackerController->PlayerState) : nullptr;
		if(AttackerPlayerState)
		{
			if(AttackerPlayerState->GetTeam() == ETeam::ET_Blue)
				BlasterGameState->BlueTeamScores();
			
			if(AttackerPlayerState->GetTeam() == ETeam::ET_Red)
				BlasterGameState->RedTeamScores();
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

	/*
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
	*/	
}
