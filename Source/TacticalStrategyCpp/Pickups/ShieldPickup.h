
#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "ShieldPickup.generated.h"

/**
 * Represents a Shield pickup item in the game world.
 * Inherits from APickup and adds specific functionality to add an extra health slot(shield) for a player.
 */
UCLASS()
class TACTICALSTRATEGYCPP_API AShieldPickup : public APickup
{
	GENERATED_BODY()

public:
	AShieldPickup();

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:
	// Amount of shielding health this pickup provides
	UPROPERTY(EditAnywhere)
	float ShieldReplenishAmount = 100.f;

	// Amount of time in which to provide the specified health
	UPROPERTY(EditAnywhere)
	float ShieldReplenishTime = 5.f;
};
