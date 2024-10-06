
#include "PickupSpawnPoint.h"
#include "Pickup.h"


APickupSpawnPoint::APickupSpawnPoint():
	SpawnedPickup(nullptr),
	SpawnPickupTimeMin(0),
	SpawnPickupTimeMax(0)
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void APickupSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	StartSpawnPickupTimer(nullptr);
}

void APickupSpawnPoint::SpawnPickup()
{
	const int32 NumPickupClasses = PickupClasses.Num();

	if(NumPickupClasses > 0)
	{
		int32 Selection = FMath::RandRange(0, NumPickupClasses - 1);
		SpawnedPickup = GetWorld()->SpawnActor<APickup>(PickupClasses[Selection], GetActorTransform());

		if(HasAuthority() && SpawnedPickup)
			SpawnedPickup->OnDestroyed.AddDynamic(this, &APickupSpawnPoint::StartSpawnPickupTimer);
	}
}

void APickupSpawnPoint::StartSpawnPickupTimer(AActor* DestroyedActor)
{
	const float SpawnTime = FMath::FRandRange(SpawnPickupTimeMin, SpawnPickupTimeMax);
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &APickupSpawnPoint::SpawnPickupTimerFinished,
		SpawnTime);
}

void APickupSpawnPoint::SpawnPickupTimerFinished()
{
	if(HasAuthority())
		SpawnPickup();
}

void APickupSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

