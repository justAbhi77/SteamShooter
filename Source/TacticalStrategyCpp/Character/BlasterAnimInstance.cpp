// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterAnimInstance.h"
#include "BlasterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TacticalStrategyCpp/Weapon/Weapon.h"

void UBlasterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
}

void UBlasterAnimInstance::NativeUpdateAnimation(const float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// Recheck character reference in case it wasn't set initially
	if(BlasterCharacter == nullptr)
		BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
	if(BlasterCharacter == nullptr) return;

	// Update character movement properties
	FVector Velocity = BlasterCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	// Set movement and state booleans from character and combat state
	const UCharacterMovementComponent* CharacterMovementComponent = BlasterCharacter->GetCharacterMovement();
	bIsInAir = CharacterMovementComponent->IsFalling();
	bIsAccelerating = CharacterMovementComponent->GetCurrentAcceleration().Size() > 0.f;
	bWeaponEquipped = BlasterCharacter->IsWeaponEquipped();
	bIsCrouched = BlasterCharacter->bIsCrouched;
	bIsAiming = BlasterCharacter->IsAiming();
	bElimmed = BlasterCharacter->IsElimmed();
	bHoldingFlag = BlasterCharacter->IsHoldingFlag();

	// Cache equipped weapon reference
	EquippedWeapon = BlasterCharacter->GetEquippedWeapon();

	// Set rotation and aiming properties
	TurningInPlace = BlasterCharacter->GetTurningInPlace();
	bRotateRootBone = BlasterCharacter->ShouldRotateRootBone();
	AoYaw = BlasterCharacter->GetAoYaw();
	AoPitch = BlasterCharacter->GetAoPitch();

	// Calculate rotation offsets
	UpdateRotationOffsets(DeltaSeconds);

	// Calculate hand transformations and look-at rotations
	UpdateHandTransforms(DeltaSeconds);

	// Set IK and aim offsets based on character's combat state
	UpdateIKAndAimOffsets();
}

void UBlasterAnimInstance::UpdateRotationOffsets(float DeltaSeconds)
{
	// Calculate YawOffset using the difference between character movement and aiming directions
	const FRotator BaseAimRotation = BlasterCharacter->GetBaseAimRotation();
	const FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(BlasterCharacter->GetVelocity());
	const FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, BaseAimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaSeconds, 6.f);
	YawOffset = DeltaRotation.Yaw;

	// Calculate lean amount based on change in character rotation
	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = BlasterCharacter->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float TargetLean = Delta.Yaw / DeltaSeconds;
	const float Interp = FMath::FInterpTo(Lean, TargetLean, DeltaSeconds, 6.f);
	Lean = FMath::Clamp(Interp, -90.f, 90.f);
}

void UBlasterAnimInstance::UpdateHandTransforms(float DeltaSeconds)
{
	if(bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetSkeletalWeaponMesh() && BlasterCharacter->GetMesh())
	{
		// Set the left-hand transform based on the weapon’s socket
		LeftHandTransform = EquippedWeapon->GetWeaponSocketLeftHand();

		// Adjust left-hand position to match character's bone space
		const FName WeaponGrabbingBoneName(BlasterCharacter->WeaponGrabbingHandSocket);
		FVector OutPosition;
		FRotator OutRotation;
		BlasterCharacter->GetMesh()->TransformToBoneSpace(WeaponGrabbingBoneName, LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotation));

		// Right-hand rotation adjustments for aiming towards target
		if(BlasterCharacter->IsLocallyControlled())
		{
			bIsLocallyControlled = true;
			const FTransform RightHandTransform = EquippedWeapon->GetSkeletalWeaponMesh()->GetSocketTransform(WeaponGrabbingBoneName, RTS_World);
			const FVector RightHandWorldLocation = RightHandTransform.GetLocation();
			const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(RightHandWorldLocation, RightHandWorldLocation + (RightHandWorldLocation - BlasterCharacter->GetHitTarget()));
			RightHandRotation = FMath::RInterpTo(RightHandRotation, LookAtRotation, DeltaSeconds, 30.f);
		}
	}
}

void UBlasterAnimInstance::UpdateIKAndAimOffsets()
{
	// Determine conditions for using IK and aim offsets based on combat state
	bUseLeftHandIk = BlasterCharacter->GetCombatState() == ECombatState::ECS_Unoccupied;
	bUseRightHandIk = bUseLeftHandIk && !BlasterCharacter->GetDisableGameplay();
	bUseAimOffsets = bUseRightHandIk;

	// Disable left-hand IK when locally reloading, or during grenade throwing
	if (BlasterCharacter->IsLocallyControlled() && BlasterCharacter->GetCombatState() != ECombatState::ECS_ThrowingGrenade && BlasterCharacter->GetFinishedSwapping())
		bUseLeftHandIk = !BlasterCharacter->IsLocallyReloading();
}