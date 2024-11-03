// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TacticalStrategyCpp/Enums/Team.h"
#include "BlasterPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALSTRATEGYCPP_API ABlasterPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void OnRep_Score() override;

	void AddToScore(const float ScoreAmount, const bool bUpdateLocally = false);
	
	void AddToDefeats(const int32 DefeatsAmount, const bool bUpdateLocally = false);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	virtual void OnRep_Defeats();
	
private:
	UPROPERTY()
	class ABlasterCharacter* Character;

	UPROPERTY()
	class ABlasterPlayerController* Controller;

	UPROPERTY(ReplicatedUsing = OnRep_Defeats)
	int32 Defeats;

	UFUNCTION()
	void OnRep_Team();
	
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_Team , meta=(AllowPrivateAccess = "true"))
	ETeam Team = ETeam::ET_NoTeam;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ETeam GetTeam() const { return Team; }
	void SetTeam(const ETeam TeamToSet);
};
