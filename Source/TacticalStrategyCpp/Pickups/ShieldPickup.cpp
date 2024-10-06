
#include "ShieldPickup.h"
#include "TacticalStrategyCpp/BlasterComponents/BuffComponent.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"


AShieldPickup::AShieldPickup()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AShieldPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShieldPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	if(ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor))
	{
		OverlappedActor = OtherActor;
		if(UBuffComponent* BuffComponent = BlasterCharacter->GetBuffComponent())
		{
			BuffComponent->ReplenishShield(ShieldReplenishAmount, ShieldReplenishTime);
		}
	}
	Destroy();
}

void AShieldPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

