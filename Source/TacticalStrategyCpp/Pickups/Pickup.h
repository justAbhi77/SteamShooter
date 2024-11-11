
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class TACTICALSTRATEGYCPP_API APickup : public AActor
{
	GENERATED_BODY()

public:
	APickup();

	// Play sound and particle system when destroyed
	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp,	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Actor that overlaps with this pickup
	UPROPERTY()
	AActor* OverlappedActor;

private:
	UPROPERTY(EditAnywhere, Category="Components")
	class USphereComponent* OverlapSphere;

	// Sound to play when pickup is collected
	UPROPERTY(EditAnywhere, Category="Audio")
	class USoundCue* PickupSound;

	// Mesh for visual representation of the pickup
	UPROPERTY(EditAnywhere, Category="Components")
	class UStaticMeshComponent* PickupMesh;

	UPROPERTY(VisibleAnywhere, Category="Effects")
	class UNiagaraComponent* PickupEffectComponent;
	
	UPROPERTY(EditAnywhere, Category="Effects")
	class UNiagaraSystem* PickupEffect;

	// Timer to handle delay before enabling overlap events
	FTimerHandle BindOverlapTimer;
	// Delay time for enabling overlap after spawn
	float BindOverlapTime = 0.25f;
	// Called when BindOverlapTimer finishes to enable overlap binding
	void BindOverlapTimerFinished();
};
