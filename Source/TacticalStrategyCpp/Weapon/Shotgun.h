
#pragma once

#include "CoreMinimal.h"
#include "HitScanWeapon.h"
#include "Shotgun.generated.h"

/**
 * Shotgun class that extends a hitscan weapon to implement shotgun-specific functionality,
 * such as firing multiple pellets with scatter and processing headshot and body damage separately.
 */
UCLASS()
class TACTICALSTRATEGYCPP_API AShotgun : public AHitScanWeapon
{
	GENERATED_BODY()

public:
	AShotgun();

	/**
	 * Fires the shotgun with multiple pellets and calculates hits on the provided targets.
	 * @param HitTargets - Array of calculated hit locations for each pellet.
	 */
	virtual void FireShotgun(const TArray<FVector_NetQuantize>& HitTargets);

	/**
	 * Calculates end locations for pellets with scatter for a shotgun.
	 * @param HitTarget - Central aim point to scatter around.
	 * @param HitTargets - Array to populate with scattered end locations.
	 */
	void ShotgunTraceEndWithScatter(const FVector& HitTarget, TArray<FVector_NetQuantize>& HitTargets) const;

private:
	// Number of pellets fired per shotgun shot
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	uint32 NumberOfPellets;
};
