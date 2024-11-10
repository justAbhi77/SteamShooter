
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuffComponent.generated.h"

/**
 * Manages buffs applied to a character, such as health, shield, speed and jump.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TACTICALSTRATEGYCPP_API UBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBuffComponent();

	// Allows ABlasterCharacter class to access private/protected members
	friend class ABlasterCharacter;

	// Starts a healing buff over a specified time
	void Heal(float HealAmount, float HealingTime);

	// Starts a shield buff over a specified time
	void ReplenishShield(float ShieldAmount, float ReplenishTime);

	// Temporarily increases speed for a specified duration
	void BuffSpeed(float BuffBaseSpeed, float BuffCrouchSpeed, float BuffTime);

	// Sets initial base and crouch speeds (called once on initialization)
	void SetInitialSpeed(float BaseSpeed, float CrouchSpeed);

	// Sets initial jump velocity (called once on initialization)
	void SetInitialJumpVelocity(float Velocity);

	// Temporarily increases jump height for a specified duration
	void BuffJump(float BuffJumpVelocity, float BuffTime);

protected:
	virtual void BeginPlay() override;

	// Manages gradual healing over time
	void HealRampUp(float DeltaTime);

	// Manages gradual shield replenishment over time
	void ShieldRampUp(float DeltaTime);

private:
	// Owner character
	UPROPERTY()
	ABlasterCharacter* Character;

	// Flags to control healing and shield states
	bool bHealing = false, bShieldReplenishing = false;

	// Healing and Shield rate and total amount to apply over time
	float HealingRate = 0, AmountToHeal = 0, ShieldRepRate = 0, ShieldAmountToRep = 0;

	// Settings for speed
	FTimerHandle SpeedBuffTimer; // Timer for the speed buff duration
	float InitialBaseSpeed, InitialCrouchSpeed;
	// Resets character's speed to initial values
	void ResetSpeedBuff();

	// Multicast to apply speed buff on all clients
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpeedBuff(float BaseSpeed, float CrouchSpeed);

	// Buff settings for jump height
	FTimerHandle JumpBuffTimer; // Timer for the jump buff duration
	float InitialJumpVelocity;
	// Resets character's jump height to initial value
	void ResetJump();

	// Multicast to apply jump buff on all clients
	UFUNCTION(NetMulticast, Reliable)
	void MulticastJumpBuff(float JumpVelocity);

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
