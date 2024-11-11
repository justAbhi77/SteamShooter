
#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "TacticalStrategyCpp/Weapon/WeaponTypes.h"
#include "AmmoPickup.generated.h"

/**
 * Represents an ammo pickup item in the game world.
 * Inherits from APickup and adds specific functionality to replenish ammo for a player.
 */
UCLASS()
class TACTICALSTRATEGYCPP_API AAmmoPickup : public APickup
{
	GENERATED_BODY()

public:
	AAmmoPickup();

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	// Component for continuous rotation of the pickup item for visual effect
	UPROPERTY(VisibleAnywhere, Category = Components)
	class URotatingMovementComponent* Rotation;

	// The specific type of ammo this pickup item supplies (e.g., for rifles, pistols, etc.)
	UPROPERTY(EditAnywhere, Category="Pickup Properties")
	EWeaponType AmmoType;
	
	// Amount of ammo this pickup provides
	UPROPERTY(EditAnywhere, Category="Pickup Properties")
	int32 AmmoAmount = 0;
};
