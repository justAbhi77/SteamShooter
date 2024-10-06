
#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "JumpPickup.generated.h"

UCLASS()
class TACTICALSTRATEGYCPP_API AJumpPickup : public APickup
{
	GENERATED_BODY()

public:
	AJumpPickup();

protected:
	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float JumpVelocityBuff = 4000;
	
	UPROPERTY(EditAnywhere)
	float JumpBuffTime = 30;
};
