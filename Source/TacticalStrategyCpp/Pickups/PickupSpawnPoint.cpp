
#include "PickupSpawnPoint.h"
#include "Pickup.h"


APickupSpawnPoint::APickupSpawnPoint() :
	SpawnedPickup(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void APickupSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	// Begin the initial timer to spawn a pickup
	StartSpawnPickupTimer(nullptr);
}

void APickupSpawnPoint::SpawnPickup()
{
	// Ensure there are available Pickup classes to spawn
	const int32 NumPickupClasses = PickupClasses.Num();
	if(NumPickupClasses > 0)
	{
		// Select a random pickup class from the list
		const int32 Selection = FMath::RandRange(0, NumPickupClasses - 1);

		SpawnedPickup = GetWorld()->SpawnActor<APickup>(PickupClasses[Selection], GetActorTransform());

		// If the pickup was successfully spawned and we have authority, bind to its OnDestroyed event
		if(HasAuthority() && SpawnedPickup)
			SpawnedPickup->OnDestroyed.AddDynamic(this, &APickupSpawnPoint::StartSpawnPickupTimer);
	}
}

// Starts a timer to respawn a pickup after the previous one is destroyed
void APickupSpawnPoint::StartSpawnPickupTimer(AActor* DestroyedActor)
{
	// Determine a random delay within the specified range for the spawn timer
	const float SpawnTime = FMath::FRandRange(SpawnPickupTimeMin, SpawnPickupTimeMax);

	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &APickupSpawnPoint::SpawnPickupTimerFinished, SpawnTime);
}

void APickupSpawnPoint::SpawnPickupTimerFinished()
{
	if(HasAuthority())
		SpawnPickup();
}
