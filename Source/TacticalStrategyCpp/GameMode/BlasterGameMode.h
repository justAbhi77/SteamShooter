#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BlasterGameMode.generated.h"

namespace MatchState
{
	extern TACTICALSTRATEGYCPP_API const FName Cooldown; // match has ended display winner and start new match
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

	UPROPERTY(EditDefaultsOnly)
	float MatchTime;
	
	UPROPERTY(EditDefaultsOnly)
	float WarmUpTime;
	
	UPROPERTY(EditDefaultsOnly)
	float CooldownTime;

	float LevelStartingTime;

protected:
	virtual void BeginPlay() override;

	virtual void OnMatchStateSet() override;
	
private:
	float CountDownTime;
};
