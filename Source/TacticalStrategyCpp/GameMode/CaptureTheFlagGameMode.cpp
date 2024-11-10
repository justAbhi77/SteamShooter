// Fill out your copyright notice in the Description page of Project Settings.

#include "CaptureTheFlagGameMode.h"
#include "TacticalStrategyCpp/CaptureTheFlag/FlagZone.h"
#include "TacticalStrategyCpp/GameState/BlasterGameState.h"
#include "TacticalStrategyCpp/Weapon/Flag.h"

void ACaptureTheFlagGameMode::FlagCaptured(const class AFlag* Flag, const class AFlagZone* Zone) const
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
