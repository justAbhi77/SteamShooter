// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptureTheFlagGameMode.h"
#include "TacticalStrategyCpp/CaptureTheFlag/FlagZone.h"
#include "TacticalStrategyCpp/GameState/BlasterGameState.h"
#include "TacticalStrategyCpp/Weapon/Flag.h"

void ACaptureTheFlagGameMode::PlayerEliminated(class ABlasterCharacter* ElimmedCharacter,
                                               class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	ABlasterGameMode::PlayerEliminated(ElimmedCharacter, VictimController, AttackerController);
}

void ACaptureTheFlagGameMode::FlagCaptured(const class AFlag* Flag, const class AFlagZone* Zone)
{
	bool bValidCapture = Flag->GetTeam() != Zone->Team;	
	if(ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(GameState))
	{
		if(Zone->Team == ETeam::ET_Blue)
			BlasterGameState->BlueTeamScores();
		else if(Zone->Team == ETeam::ET_Red)
			BlasterGameState->RedTeamScores();
	}
}
