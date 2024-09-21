
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
	
	virtual void Fire(const FVector& HitTarget) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	uint32 NumberOfPellets;

public:
	virtual void Tick(float DeltaTime) override;
};
