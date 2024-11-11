
#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "HealthPickup.generated.h"

/**
 * Represents a health buff pickup item in the game world.
 * Inherits from APickup and adds specific functionality to replenish health for a player.
 */
UCLASS()
class TACTICALSTRATEGYCPP_API AHealthPickup : public APickup
{
	GENERATED_BODY()

public:
	AHealthPickup();

protected:

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:
	// Amount of health this pickup provides
	UPROPERTY(EditAnywhere)
	float HealAmount = 100.f;

	// Amount of time in which to provide the specified health
	UPROPERTY(EditAnywhere)
	float HealingTime = 5.f;
};
