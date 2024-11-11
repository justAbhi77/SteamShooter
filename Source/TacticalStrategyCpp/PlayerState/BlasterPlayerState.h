// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TacticalStrategyCpp/Enums/Team.h"
#include "BlasterPlayerState.generated.h"

/**
 * Player State class for managing player-specific data such as score, team, and defeats count.
 * This class handles replication to synchronize player data across the network.
 */
UCLASS()
class TACTICALSTRATEGYCPP_API ABlasterPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void OnRep_Score() override;

	/**
	 * Adds a specified amount to the player's score.
	 * @param ScoreAmount - Amount to add to the player's score.
	 */
	void AddToScore(const float ScoreAmount, const bool bUpdateLocally = false);
	
	/**
	 * Adds a specified amount to the player's defeats count.
	 * @param DefeatsAmount - Amount to add to the player's defeats.
	 */
	void AddToDefeats(const int32 DefeatsAmount, const bool bUpdateLocally = false);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	virtual void OnRep_Defeats();

private:
	// Reference to the Blaster character associated with this player state
	UPROPERTY()
	class ABlasterCharacter* Character;

	// Reference to the Blaster player controller associated with this player state
	UPROPERTY()
	class ABlasterPlayerController* Controller;

	UPROPERTY(ReplicatedUsing = OnRep_Defeats)
	int32 Defeats;

	UFUNCTION()
	void OnRep_Team();

	// Team assignment for the player, replicated
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_Team, meta=(AllowPrivateAccess = "true"))
	ETeam Team = ETeam::ET_NoTeam;

public:
	// Retrieves the team of the player
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ETeam GetTeam() const { return Team; }

	void SetTeam(const ETeam TeamToSet);
};
