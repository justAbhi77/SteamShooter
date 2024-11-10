#include "BuffComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"

UBuffComponent::UBuffComponent():
	Character(nullptr),
	InitialBaseSpeed(0),
	InitialCrouchSpeed(0),
	InitialJumpVelocity(0)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBuffComponent::Heal(float HealAmount, float HealingTime)
{
	// Initialize healing rate and total amount
	HealingRate = HealAmount / HealingTime;
	AmountToHeal += HealAmount;
	bHealing = true;
}

void UBuffComponent::ReplenishShield(float ShieldAmount, float ReplenishTime)
{
	// Initialize shield replenishment rate and total amount
	ShieldRepRate = ShieldAmount / ReplenishTime;
	ShieldAmountToRep += ShieldAmount;
	bShieldReplenishing = true;
}

void UBuffComponent::BuffSpeed(float BuffBaseSpeed, float BuffCrouchSpeed, float BuffTime)
{
	if(Character == nullptr) return;

	// Set timer to reset speed buff after BuffTime expires
	Character->GetWorldTimerManager().SetTimer(SpeedBuffTimer, this, &UBuffComponent::ResetSpeedBuff, BuffTime);

	// Apply speed buff to character's movement
	if(UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement())
	{
		MovementComponent->MaxWalkSpeed = BuffBaseSpeed;
		MovementComponent->MaxWalkSpeedCrouched = BuffCrouchSpeed;
	}

	// Update speed for all clients
	MulticastSpeedBuff(BuffBaseSpeed, BuffCrouchSpeed);
}

void UBuffComponent::SetInitialSpeed(float BaseSpeed, float CrouchSpeed)
{
	// Stores character's initial speed values for resetting later
	InitialBaseSpeed = BaseSpeed;
	InitialCrouchSpeed = CrouchSpeed;
}

void UBuffComponent::SetInitialJumpVelocity(float Velocity)
{
	// Stores character's initial jump velocity for resetting later
	InitialJumpVelocity = Velocity;
}

void UBuffComponent::BuffJump(float BuffJumpVelocity, float BuffTime)
{
	if(Character == nullptr) return;

	// Set timer to reset jump buff after BuffTime expires
	Character->GetWorldTimerManager().SetTimer(JumpBuffTimer, this, &UBuffComponent::ResetJump, BuffTime);

	// Apply jump buff to character movement component
	if(UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement())
		MovementComponent->JumpZVelocity = BuffJumpVelocity;

	// Update jump velocity for all clients
	MulticastJumpBuff(BuffJumpVelocity);
}

void UBuffComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBuffComponent::HealRampUp(float DeltaTime)
{
	// Apply healing over time if healing is active and character is alive
	if(!bHealing || Character == nullptr || Character->IsElimmed()) return;

	const float HealThisFrame = HealingRate * DeltaTime;
	Character->SetHealth(FMath::Clamp(Character->GetHealth() + HealThisFrame, 0.f, Character->GetMaxHealth()));
	Character->UpdateHudHealth();

	AmountToHeal -= HealThisFrame;
	if(AmountToHeal <= 0.f || Character->GetHealth() >= Character->GetMaxHealth())
	{
		// Stop healing if full health reached or amount to heal is depleted
		bHealing = false;
		AmountToHeal = 0.f;
	}
}

void UBuffComponent::ShieldRampUp(float DeltaTime)
{
	// Apply shield replenishment over time if replenishment is active and character is alive
	if(!bShieldReplenishing || Character == nullptr || Character->IsElimmed()) return;

	const float ShieldRepThisFrame = ShieldRepRate * DeltaTime;
	Character->SetShield(FMath::Clamp(Character->GetShield() + ShieldRepThisFrame, 0.f, Character->GetMaxShield()));
	Character->UpdateHudShield();
	ShieldAmountToRep -= ShieldRepThisFrame;
	if(ShieldAmountToRep <= 0.f || Character->GetShield() >= Character->GetMaxShield())
	{
		// Stop shield replenishment if full shield reached or amount to replenish is depleted
		bShieldReplenishing = false;
		ShieldAmountToRep = 0.f;
	}
}

void UBuffComponent::ResetSpeedBuff()
{
	// Resets character's speed to initial values after buff duration expires
	if(Character == nullptr) return;

	if(UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement())
	{
		MovementComponent->MaxWalkSpeed = InitialBaseSpeed;
		MovementComponent->MaxWalkSpeedCrouched = InitialCrouchSpeed;
	}

	// Update speed for all clients to initial values
	MulticastSpeedBuff(InitialBaseSpeed, InitialCrouchSpeed);
}

void UBuffComponent::MulticastSpeedBuff_Implementation(float BaseSpeed, float CrouchSpeed)
{
	// Applies speed buff to all clients
	if (Character == nullptr) return;

	if(UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement())
	{
		MovementComponent->MaxWalkSpeed = BaseSpeed;
		MovementComponent->MaxWalkSpeedCrouched = CrouchSpeed;
	}
}

void UBuffComponent::ResetJump()
{
	// Resets character's jump velocity to initial value after buff duration expires
	if(UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement())
		MovementComponent->JumpZVelocity = InitialJumpVelocity;

	// Update jump velocity for all clients to initial value
	MulticastJumpBuff(InitialJumpVelocity);
}

void UBuffComponent::MulticastJumpBuff_Implementation(float JumpVelocity)
{
	// Applies jump buff to all clients
	if(UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement())
		MovementComponent->JumpZVelocity = JumpVelocity;
}

void UBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Gradually apply healing and shield replenishment each frame
	HealRampUp(DeltaTime);
	ShieldRampUp(DeltaTime);
}
