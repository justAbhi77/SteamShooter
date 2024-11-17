// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlasterTeamsGameMode.h"
#include "CaptureTheFlagGameMode.generated.h"

/**
 * Handles the game mode specific to Capture The Flag (CTF).
 * This mode adds functionality for flag capturing and eliminates players as in a team-based game mode.
 */
UCLASS()
class TACTICALSTRATEGYCPP_API ACaptureTheFlagGameMode : public ABlasterTeamsGameMode
{
	GENERATED_BODY()
	
public:
	virtual void PlayerEliminated(class ABlasterCharacter* ElimmedCharacter, class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController) override;

	void FlagCaptured(const class AFlag* Flag, const class AFlagZone* Zone) const;
};
