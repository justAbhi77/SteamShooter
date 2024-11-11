
#include "JumpPickup.h"
#include "TacticalStrategyCpp/BlasterComponents/BuffComponent.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"


AJumpPickup::AJumpPickup()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AJumpPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	// Check if the overlapping actor is a valid BlasterCharacter
	if(ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor))
		// Retrieve the character's buff component and buff jumping
		if(UBuffComponent* BuffComponent = BlasterCharacter->GetBuffComponent())
			BuffComponent->BuffJump(JumpVelocityBuff, JumpBuffTime);
			
	Destroy();
}

