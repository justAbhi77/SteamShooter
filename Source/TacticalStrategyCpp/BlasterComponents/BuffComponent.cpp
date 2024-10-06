
#include "BuffComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"

UBuffComponent::UBuffComponent():
	Character(nullptr),
	InitialBaseSpeed(0),
	InitialCrouchSpeed(0), InitialJumpVelocity(0)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBuffComponent::Heal(float HealAmount, float HealingTime)
{
	HealingRate = HealAmount / HealingTime;
	AmountToHeal += HealAmount;
	bHealing = true;
}

void UBuffComponent::ReplenishShield(float ShieldAmount, float ReplenishTime)
{
	ShieldRepRate = ShieldAmount / ReplenishTime;
	ShieldAmountToRep += ShieldAmount;
	bShieldReplenishing = true;
}

void UBuffComponent::BuffSpeed(float BuffBaseSpeed, float BuffCrouchSpeed, float BuffTime)
{
	if(Character == nullptr) return;

	Character->GetWorldTimerManager().SetTimer(SpeedBuffTimer, this, &UBuffComponent::ResetSpeedBuff,
		BuffTime);
	if(UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement())
	{
		CharacterMovementComponent->MaxWalkSpeed = BuffBaseSpeed;
		CharacterMovementComponent->MaxWalkSpeedCrouched = BuffCrouchSpeed;
	}
	MulticastSpeedBuff(BuffBaseSpeed, BuffCrouchSpeed);
}

void UBuffComponent::SetInitialSpeed(float BaseSpeed, float CrouchSpeed)
{
	InitialBaseSpeed = BaseSpeed;
	InitialCrouchSpeed = CrouchSpeed;
}

void UBuffComponent::SetInitialJumpVelocity(float Velocity)
{
	InitialJumpVelocity = Velocity;
}

void UBuffComponent::BuffJump(float BuffJumpVelocity, float BuffTime)
{
	if(Character == nullptr) return;

	Character->GetWorldTimerManager().SetTimer(JumpBuffTimer, this, &UBuffComponent::ResetJump,
		BuffTime);
	if(UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement())
	{
		CharacterMovementComponent->JumpZVelocity = BuffJumpVelocity;
	}
	MulticastJumpBuff(BuffJumpVelocity);
}

void UBuffComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UBuffComponent::HealRampUp(float DeltaTime)
{
	if(!bHealing || Character == nullptr || Character->IsElimmed()) return;

	const float HealThisFrame = HealingRate * DeltaTime;
	
	Character->SetHealth(FMath::Clamp(Character->GetHealth() + HealThisFrame, 0,
		Character->GetMaxHealth()));
	Character->UpdateHudHealth();
	
	AmountToHeal -= HealThisFrame;
	if(AmountToHeal <= 0.f || Character->GetHealth() >= Character->GetMaxHealth())
	{
		bHealing = false;
		AmountToHeal = 0;
	}
}

void UBuffComponent::ShieldRampUp(float DeltaTime)
{
	if(!bShieldReplenishing || Character == nullptr || Character->IsElimmed()) return;

	const float ShieldRepThisFrame = ShieldRepRate * DeltaTime;
	
	Character->SetShield(FMath::Clamp(Character->GetShield() + ShieldRepThisFrame, 0,
		Character->GetMaxShield()));
	Character->UpdateHudShield();
	
	ShieldAmountToRep -= ShieldRepThisFrame;
	if(ShieldAmountToRep <= 0.f || Character->GetShield() >= Character->GetMaxShield())
	{
		bShieldReplenishing = false;
		ShieldAmountToRep = 0;
	}
}

void UBuffComponent::ResetSpeedBuff()
{
	if(Character == nullptr) return;
	
	UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement();
	if(CharacterMovementComponent == nullptr) return;
	
	CharacterMovementComponent->MaxWalkSpeed = InitialBaseSpeed;
	CharacterMovementComponent->MaxWalkSpeedCrouched = InitialCrouchSpeed;
	
	MulticastSpeedBuff(InitialBaseSpeed, InitialCrouchSpeed);
}

void UBuffComponent::MulticastSpeedBuff_Implementation(float BaseSpeed, float CrouchSpeed)
{
	UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement();
	if(Character == nullptr || CharacterMovementComponent) return;
	
	CharacterMovementComponent->MaxWalkSpeed = BaseSpeed;
	CharacterMovementComponent->MaxWalkSpeedCrouched = CrouchSpeed;
}

void UBuffComponent::ResetJump()
{
	if(UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement())
	{
		CharacterMovementComponent->JumpZVelocity = InitialJumpVelocity;
	}
	MulticastJumpBuff(InitialJumpVelocity);
}

void UBuffComponent::MulticastJumpBuff_Implementation(float JumpVelocity)
{
	if(UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement())
	{
		CharacterMovementComponent->JumpZVelocity = JumpVelocity;
	}	
}

void UBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HealRampUp(DeltaTime);
	ShieldRampUp(DeltaTime);
}

