
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
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class APickup>> PickupClasses;

	void SpawnPickup();

	UFUNCTION()
	void StartSpawnPickupTimer(AActor* DestroyedActor);
	void SpawnPickupTimerFinished();

	UPROPERTY()
	APickup* SpawnedPickup;

private:
	FTimerHandle SpawnTimerHandle;

	UPROPERTY(EditAnywhere)
	float SpawnPickupTimeMin;
	
	UPROPERTY(EditAnywhere)
	float SpawnPickupTimeMax;
};
