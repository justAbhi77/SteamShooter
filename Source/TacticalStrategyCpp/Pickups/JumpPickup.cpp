
#include "JumpPickup.h"
#include "TacticalStrategyCpp/BlasterComponents/BuffComponent.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"


AJumpPickup::AJumpPickup()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AJumpPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void AJumpPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	if(ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor))
	{
		OverlappedActor = OtherActor;
		if(UBuffComponent* BuffComponent = BlasterCharacter->GetBuffComponent())
		{
			BuffComponent->BuffJump(JumpVelocityBuff, JumpBuffTime);
		}
	}
	Destroy();
}

void AJumpPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

