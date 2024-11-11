
#include "AmmoPickup.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "TacticalStrategyCpp/BlasterComponents/CombatComponent.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"

AAmmoPickup::AAmmoPickup() :
	Rotation(nullptr), AmmoAmount(30), AmmoType(EWeaponType::EWT_MAX)
{
	PrimaryActorTick.bCanEverTick = false;

	// Initialize and configure the rotating movement component for a spinning effect
	Rotation = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotation"));
	Rotation->RotationRate = FRotator(0, 10, 0); // Rotate around the Y-axis
	Rotation->bRotationInLocalSpace = false;
	Rotation->bUpdateOnlyIfRendered = true; // Only update if the item is visible
}

void AAmmoPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// Check if the overlapping actor is a valid BlasterCharacter
	if(ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor))
		// Retrieve the character's combat component and add ammo
		if(UCombatComponent* Combat = BlasterCharacter->GetCombatComponent())
			Combat->PickupAmmo(AmmoType, AmmoAmount);

	// Destroy the pickup after use
	Destroy();
}
