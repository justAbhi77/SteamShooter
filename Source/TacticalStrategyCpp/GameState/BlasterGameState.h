// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BlasterGameState.generated.h"

/**
 * Game state class that tracks team scores, top scoring players, and team membership.
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

	// Adds a player to the top-scoring list if their score is the highest
	void UpdateTopScore(ABlasterPlayerState* ScoringPlayer);

	// Arrays to store references to players on each team
	TArray<ABlasterPlayerState*> RedTeam, BlueTeam;

	UFUNCTION()
	void OnRep_RedTeamScore();
	
	UFUNCTION()
	void OnRep_BlueTeamScore();

	UPROPERTY(ReplicatedUsing = OnRep_RedTeamScore)
	float RedTeamScore = 0.0f;

	UPROPERTY(ReplicatedUsing = OnRep_BlueTeamScore)
	float BlueTeamScore = 0.0f;

	// Increments the Red Team score
	void RedTeamScores();

	// Increments the Blue Team score and triggers replication
	void BlueTeamScores();

private:
	// Stores the highest score achieved by the top-scoring player(s)
	float TopScore = 0.0f;

	UPROPERTY()
	class ABlasterPlayerController* PlayerController = nullptr;
};
