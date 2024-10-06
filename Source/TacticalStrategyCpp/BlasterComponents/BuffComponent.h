
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuffComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TACTICALSTRATEGYCPP_API UBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBuffComponent();

	friend class ABlasterCharacter;

	void Heal(float HealAmount, float HealingTime);
	
	void ReplenishShield(float ShieldAmount, float ReplenishTime);

	void BuffSpeed(float BuffBaseSpeed, float BuffCrouchSpeed, float BuffTime);

	void SetInitialSpeed(float BaseSpeed, float CrouchSpeed);

	void SetInitialJumpVelocity(float Velocity);

	void BuffJump(float BuffJumpVelocity, float BuffTime);

protected:
	virtual void BeginPlay() override;

	void HealRampUp(float DeltaTime);

	void ShieldRampUp(float DeltaTime);

private:	
	UPROPERTY()
	ABlasterCharacter* Character;

	bool bHealing = false, bShieldReplenishing = false;

	float HealingRate = 0, AmountToHeal = 0, ShieldRepRate = 0, ShieldAmountToRep = 0;
	
	FTimerHandle SpeedBuffTimer;
	void ResetSpeedBuff();
	float InitialBaseSpeed, InitialCrouchSpeed;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpeedBuff(float BaseSpeed, float CrouchSpeed);

	FTimerHandle JumpBuffTimer;
	void ResetJump();
	float InitialJumpVelocity;
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastJumpBuff(float JumpVelocity);

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
