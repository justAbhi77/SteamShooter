// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlasterGameMode.h"
#include "BlasterTeamsGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALSTRATEGYCPP_API ABlasterTeamsGameMode : public ABlasterGameMode
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaSeconds) override;
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	virtual bool HasMatchStarted() const override;
	
protected:
	virtual void AfterWaitingToStart() override;
	
	virtual void HandleMatchHasStarted() override;
};
