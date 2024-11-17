// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileRocket.generated.h"

/**
 * Represents a rocket projectile with custom movement, sound, and explosion effects.
 */
UCLASS()
class TACTICALSTRATEGYCPP_API AProjectileRocket : public AProjectile
{
	GENERATED_BODY()

public:
	AProjectileRocket();

protected:
	virtual void BeginPlay() override;

	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit) override;

	// Continuous sound effect for the rocket's flight.
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* ProjectileLoop;

	// Controls how the looping sound attenuates with distance.
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundAttenuation* LoopingSoundAttenuation;

	// Audio component for managing the rocket's looping sound.
	UPROPERTY()
	UAudioComponent* ProjectileLoopComponent;

	// Custom movement component for the rocket's physics and trajectory.
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	class URocketMovementComponent* RocketMovementComponent;
};
