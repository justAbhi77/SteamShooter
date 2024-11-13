// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "RocketMovementComponent.generated.h"

/**
 * Movement component for rocket-type projectiles.
 * Extends ProjectileMovementComponent to handle specialized impact behavior and movement updates.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TACTICALSTRATEGYCPP_API URocketMovementComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()

public:
	URocketMovementComponent();

protected:

	virtual EHandleBlockingHitResult HandleBlockingHit(const FHitResult& Hit, float TimeTick, const FVector& MoveDelta, float& SubTickTimeRemaining) override;

	// Rockets should not stop only explode when collision is detected 
	virtual void HandleImpact(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta) override;
};
