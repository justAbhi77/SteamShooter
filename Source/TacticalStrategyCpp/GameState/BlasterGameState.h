// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BlasterGameState.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALSTRATEGYCPP_API ABlasterGameState : public AGameState
{
	GENERATED_BODY()

public:
	ABlasterGameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(Replicated)
	TArray<class ABlasterPlayerState*> TopScoringPlayers;

	void UpdateTopScore(ABlasterPlayerState* ScoringPlayer);

private:
	float TopScore;
};
