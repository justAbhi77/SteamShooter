
#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "HitScanWeapon.generated.h"

UCLASS()
class TACTICALSTRATEGYCPP_API AHitScanWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	AHitScanWeapon();

	virtual void Fire(const FVector& HitTarget) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float Damage;
	
	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere)
	UParticleSystem* BeamParticles;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USoundCue* FireSound;
	
	UPROPERTY(EditAnywhere)
	USoundCue* HitSound;

	FVector TraceEndWithScatter(const FVector& TraceStart, const FVector& HitTarget) const;

	UPROPERTY(EditAnywhere, Category="Weapon Scatter")
	bool bUseScatter;
	
	UPROPERTY(EditAnywhere, Category="Weapon Scatter")
	float DistanceToSphere;
	
	UPROPERTY(EditAnywhere, Category="Weapon Scatter")
	float SphereRadius;

	void WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit) const;
	
public:
	virtual void Tick(float DeltaTime) override;
};
