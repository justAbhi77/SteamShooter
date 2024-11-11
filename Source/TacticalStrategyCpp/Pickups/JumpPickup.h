
#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "JumpPickup.generated.h"

/**
 * Represents a jump buff pickup item in the game world.
 * Inherits from APickup and adds specific functionality to buff jumping for a player.
 */
UCLASS()
class TACTICALSTRATEGYCPP_API AJumpPickup : public APickup
{
	GENERATED_BODY()

public:
	AJumpPickup();

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:
	// Amount of jump buff this pickup provides
	UPROPERTY(EditAnywhere)
	float JumpVelocityBuff = 4000;

	// Amount of time to provide the buff for
	UPROPERTY(EditAnywhere)
	float JumpBuffTime = 30;
};
