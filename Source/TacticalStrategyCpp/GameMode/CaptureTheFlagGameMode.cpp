// Fill out your copyright notice in the Description page of Project Settings.

#include "CaptureTheFlagGameMode.h"
#include "TacticalStrategyCpp/CaptureTheFlag/FlagZone.h"
#include "TacticalStrategyCpp/GameState/BlasterGameState.h"
#include "TacticalStrategyCpp/Weapon/Flag.h"

void ACaptureTheFlagGameMode::PlayerEliminated(class ABlasterCharacter* ElimmedCharacter, ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	// we do not want to score a team point when player is eliminated
	// Super::PlayerEliminated(ElimmedCharacter, VictimController, AttackerController);
	
	ABlasterGameMode::PlayerEliminated(ElimmedCharacter, VictimController, AttackerController);
}

void ACaptureTheFlagGameMode::FlagCaptured(const AFlag* Flag, const AFlagZone* Zone) const
{
	bool bValidCapture = Flag->GetTeam() != Zone->Team;

	ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(GameState);
	if(BlasterGameState && bValidCapture)
	{
		if(Zone->Team == ETeam::ET_Blue)
			BlasterGameState->BlueTeamScores();
		else if(Zone->Team == ETeam::ET_Red)
			BlasterGameState->RedTeamScores();
	}
}
