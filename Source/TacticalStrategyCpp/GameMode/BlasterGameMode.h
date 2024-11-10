#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BlasterGameMode.generated.h"

// Custom match states for team selection and cooldown
namespace MatchState
{
	extern TACTICALSTRATEGYCPP_API const FName WaitingTeamSelection; // Waiting for players to select a team (team modes)
	extern TACTICALSTRATEGYCPP_API const FName MatchInCooldown; // Match cooldown after end to display winner/start new match
}

/**
 * Handles player elimination, respawn, and match states.
 */
UCLASS()
class TACTICALSTRATEGYCPP_API ABlasterGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABlasterGameMode();

	virtual void Tick(float DeltaSeconds) override;

	// Handles player elimination logic including updating score and respawn management
	virtual void PlayerEliminated(class ABlasterCharacter* ElimmedCharacter,
		class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController);

	// Request to respawn a player after elimination
	virtual void RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController);

	// Handles player disconnection during the match, updating score/state
	void PlayerLeftGame(class ABlasterPlayerState* PlayerLeaving) const;

	// Total match time in seconds
	UPROPERTY(EditDefaultsOnly)
	float MatchTime;

	// Time for players to prepare before match starts
	UPROPERTY(EditDefaultsOnly)
	float WarmUpTime;

	// Cooldown period after match end to display winner/start new match
	UPROPERTY(EditDefaultsOnly)
	float CooldownTime;

	// Starting time of the level, used for calculating match timing
	float LevelStartingTime;

	// Calculate damage dealt between players, modified by attacker/victim roles
	virtual float CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage);

	// Flag for team-based match
	bool bTeamsMatch = false;

protected:
	virtual void BeginPlay() override;

	// Logic executed after warmup when waiting for players to start
	virtual void AfterWaitingToStart();

	// Override to set custom match states
	virtual void OnMatchStateSet() override;

	// Current countdown timer, updated per tick
	float CountDownTime;

public:
	FORCEINLINE float GetCountdownTime() const { return CountDownTime; }
};
