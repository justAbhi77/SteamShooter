
#include "HealthPickup.h"
#include "TacticalStrategyCpp/BlasterComponents/BuffComponent.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"


AHealthPickup::AHealthPickup()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AHealthPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	// Check if the overlapping actor is a valid BlasterCharacter
	if(ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor))
		// Retrieve the character's buff component and add health
		if(UBuffComponent* BuffComponent = BlasterCharacter->GetBuffComponent())
			BuffComponent->Heal(HealAmount, HealingTime);
	
	// Destroy the pickup after use
	Destroy();
}

