
#include "AmmoPickup.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "TacticalStrategyCpp/BlasterComponents/CombatComponent.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"


AAmmoPickup::AAmmoPickup():
	Rotation(),
	AmmoAmount(30),
	AmmoType(EWeaponType::EWT_MAX)
{
	PrimaryActorTick.bCanEverTick = true;

	Rotation = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotation"));
	Rotation->RotationRate = FRotator(0, 10, 0);
	Rotation->bRotationInLocalSpace = false;
	Rotation->bUpdateOnlyIfRendered = true;
}

void AAmmoPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAmmoPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if(ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor))
	{
		if(UCombatComponent* Combat = BlasterCharacter->GetCombatComponent())
		{
			Combat->PickupAmmo(AmmoType, AmmoAmount);
		}
	}
	Destroy();
}

void AAmmoPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

