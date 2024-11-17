
#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "HitScanWeapon.generated.h"

/**
 * Represents a weapon that performs hitscan logic, where traces are used to determine hits.
 */
UCLASS()
class TACTICALSTRATEGYCPP_API AHitScanWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	AHitScanWeapon();

	/** Overrides the Fire function to implement hitscan logic */
	virtual void Fire(const FVector& HitTarget) override;

protected:
	// Particle and sound effects for various events
	UPROPERTY(EditAnywhere, Category = "VFX")
	UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, Category = "VFX")
	UParticleSystem* BeamParticles;

	UPROPERTY(EditAnywhere, Category = "VFX")
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundCue* FireSound;

	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundCue* HitSound;

	/**
	* Traces a line from the weapon's muzzle to the target to detect hits.
	*/
	void WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit) const;
};
