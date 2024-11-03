#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BlasterGameMode.generated.h"

namespace MatchState
{
	extern TACTICALSTRATEGYCPP_API const FName WaitingTeamSelection; // waiting for the player to select a team used in team game mode
	extern TACTICALSTRATEGYCPP_API const FName MatchInCooldown; // match has ended display winner and start new match
}

/**
 * 
 */
UCLASS()
class TACTICALSTRATEGYCPP_API ABlasterGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ABlasterGameMode();

	virtual void Tick(float DeltaSeconds) override;
	
	virtual void PlayerEliminated(class ABlasterCharacter* ElimmedCharacter,
		class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController);

	virtual void RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController);

	void PlayerLeftGame(class ABlasterPlayerState* PlayerLeaving);

	UPROPERTY(EditDefaultsOnly)
	float MatchTime;
	
	UPROPERTY(EditDefaultsOnly)
	float WarmUpTime;
	
	UPROPERTY(EditDefaultsOnly)
	float CooldownTime;

	float LevelStartingTime;

	virtual float CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage);

	bool bTeamsMatch = false;

protected:
	virtual void BeginPlay() override;
	
	virtual void AfterWaitingToStart();

	virtual void OnMatchStateSet() override;
	
	float CountDownTime;

public:
	FORCEINLINE float GetCountdownTime() const { return CountDownTime; }
};
