// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "ProjectileWeapon.generated.h"

/**
 * Weapon class that fires projectiles.
 */
UCLASS()
class TACTICALSTRATEGYCPP_API AProjectileWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	AProjectileWeapon();

	/**
	 * Overrides the Fire function to spawn and launch projectiles toward the target.
	 * 
	 * @param HitTarget - Location to fire the projectile toward.
	 */
	virtual void Fire(const FVector& HitTarget) override;

private:
	// Class of the projectile to spawn when firing.
	UPROPERTY(EditAnywhere, Category = "Projectile Settings")
	TSubclassOf<class AProjectile> ProjectileClass;

	// Projectile class used when server-side rewind is enabled.
	UPROPERTY(EditAnywhere, Category = "Projectile Settings")
	TSubclassOf<AProjectile> ServerSideRewindProjectileClass;
};
