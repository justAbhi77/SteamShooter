
#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "SpeedPickup.generated.h"

/**
 * Represents a speed buff pickup item in the game world.
 * Inherits from APickup and adds specific functionality to buff speeds for a player.
 */
UCLASS()
class TACTICALSTRATEGYCPP_API ASpeedPickup : public APickup
{
	GENERATED_BODY()

public:
	ASpeedPickup();

protected:

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:
	// Amount of speed buff while standing
	UPROPERTY(EditAnywhere)
	float BaseSpeedBuff = 1600.f;

	// Amount of speed buff while crouching
	UPROPERTY(EditAnywhere)
	float CrouchSpeedBuff = 850.f;

	// Amount of time to provide the buff for
	UPROPERTY(EditAnywhere)
	float SpeedBuffTime = 20.f;
};
