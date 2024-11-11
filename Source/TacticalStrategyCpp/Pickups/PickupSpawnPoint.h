
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupSpawnPoint.generated.h"

UCLASS()
class TACTICALSTRATEGYCPP_API APickupSpawnPoint : public AActor
{
	GENERATED_BODY()

public:
	APickupSpawnPoint();

protected:
	virtual void BeginPlay() override;

	// Array of Pickup classes that this spawn point can randomly choose from to spawn
	UPROPERTY(EditAnywhere, Category = "Pickup Settings")
	TArray<TSubclassOf<class APickup>> PickupClasses;

	// Spawns a pickup from the available PickupClasses
	void SpawnPickup();

	// Initiates a timer to spawn a new pickup after a delay when the current one is destroyed
	UFUNCTION()
	void StartSpawnPickupTimer(AActor* DestroyedActor);

	// Callback for when the spawn timer completes, triggering a new pickup spawn
	void SpawnPickupTimerFinished();

	// Reference to the currently spawned pickup
	UPROPERTY()
	APickup* SpawnedPickup;

private:
	FTimerHandle SpawnTimerHandle;

	// Minimum and maximum time delay before a new pickup spawns
	UPROPERTY(EditAnywhere, Category = "Spawn Timer Settings")
	float SpawnPickupTimeMin = 5.0f;
	UPROPERTY(EditAnywhere, Category = "Spawn Timer Settings")
	float SpawnPickupTimeMax = 15.0f;
};
