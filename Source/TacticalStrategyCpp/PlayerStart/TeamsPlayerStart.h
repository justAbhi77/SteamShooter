// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "TacticalStrategyCpp/Enums/Team.h"
#include "TeamsPlayerStart.generated.h"

UCLASS()
class TACTICALSTRATEGYCPP_API ATeamsPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	ETeam Team = ETeam::ET_NoTeam;
};
