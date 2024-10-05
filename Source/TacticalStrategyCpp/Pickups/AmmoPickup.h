
#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "TacticalStrategyCpp/Weapon/WeaponTypes.h"
#include "AmmoPickup.generated.h"

UCLASS()
class TACTICALSTRATEGYCPP_API AAmmoPickup : public APickup
{
	GENERATED_BODY()

public:
	AAmmoPickup();

protected:
	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(VisibleAnywhere)
	class URotatingMovementComponent* Rotation;
	
	UPROPERTY(EditAnywhere)
	int32 AmmoAmount;

	UPROPERTY(EditAnywhere)
	EWeaponType AmmoType;
public:
	virtual void Tick(float DeltaTime) override;
};
