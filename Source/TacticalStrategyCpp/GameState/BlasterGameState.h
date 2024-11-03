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
	
	TArray<ABlasterPlayerState*> RedTeam;
	TArray<ABlasterPlayerState*> BlueTeam;

	UFUNCTION()
	void OnRep_RedTeamScore();
	
	UFUNCTION()
	void OnRep_BlueTeamScore();
	
	UPROPERTY(ReplicatedUsing=OnRep_RedTeamScore)
	float RedTeamScore;
	
	UPROPERTY(ReplicatedUsing=OnRep_BlueTeamScore)
	float BlueTeamScore;

	void RedTeamScores();
	void BlueTeamScores();

private:
	float TopScore;
};
