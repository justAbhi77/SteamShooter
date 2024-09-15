
#include "RocketMovementComponent.h"


URocketMovementComponent::URocketMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URocketMovementComponent::BeginPlay()
{
	Super::BeginPlay();	
}

UProjectileMovementComponent::EHandleBlockingHitResult URocketMovementComponent::HandleBlockingHit(
	const FHitResult& Hit, float TimeTick, const FVector& MoveDelta, float& SubTickTimeRemaining)
{
	Super::HandleBlockingHit(Hit, TimeTick, MoveDelta, SubTickTimeRemaining);

	return EHandleBlockingHitResult::AdvanceNextSubstep;
}

void URocketMovementComponent::HandleImpact(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta)
{
	// Rockets should not stop only explode when collision is detected 
	//Super::HandleImpact(Hit, TimeSlice, MoveDelta);
}

void URocketMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

