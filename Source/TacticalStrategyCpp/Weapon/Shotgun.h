
#pragma once

#include "CoreMinimal.h"
#include "HitScanWeapon.h"
#include "Shotgun.generated.h"

UCLASS()
class TACTICALSTRATEGYCPP_API AShotgun : public AHitScanWeapon
{
	GENERATED_BODY()

public:
	AShotgun();

	virtual void FireShotgun(const TArray<FVector_NetQuantize>& HitTargets);

	void ShotgunTraceEndWithScatter(const FVector& HitTarget, TArray<FVector_NetQuantize>& HitTargets) const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	uint32 NumberOfPellets;

public:
	virtual void Tick(float DeltaTime) override;
};
