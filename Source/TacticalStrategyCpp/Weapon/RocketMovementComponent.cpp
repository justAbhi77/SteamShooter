
#include "RocketMovementComponent.h"


URocketMovementComponent::URocketMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Rocket should not stop only explode when collision is detected
UProjectileMovementComponent::EHandleBlockingHitResult URocketMovementComponent::HandleBlockingHit(
	const FHitResult& Hit, float TimeTick, const FVector& MoveDelta, float& SubTickTimeRemaining)
{
	Super::HandleBlockingHit(Hit, TimeTick, MoveDelta, SubTickTimeRemaining);

	return EHandleBlockingHitResult::AdvanceNextSubstep;
}

// Rocket should not stop only explode when collision is detected
void URocketMovementComponent::HandleImpact(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta)
{
	//Super::HandleImpact(Hit, TimeSlice, MoveDelta);
}
