
#include "SpeedPickup.h"
#include "TacticalStrategyCpp/BlasterComponents/BuffComponent.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"


ASpeedPickup::ASpeedPickup()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASpeedPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASpeedPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	if(ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor))
	{
		OverlappedActor = OtherActor;
		if(UBuffComponent* BuffComponent = BlasterCharacter->GetBuffComponent())
		{
			BuffComponent->BuffSpeed(BaseSpeedBuff, CrouchSpeedBuff, SpeedBuffTime);
		}
	}
	Destroy();
}

void ASpeedPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

