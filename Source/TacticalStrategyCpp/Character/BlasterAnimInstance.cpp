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

	if(BlasterCharacter == nullptr)
	{
		BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());		
	}

	if(BlasterCharacter == nullptr) return;

	FVector Velocity = BlasterCharacter->GetVelocity();
	Velocity.Z = 0.f;
	
	Speed = Velocity.Size();

	const UCharacterMovementComponent* CharacterMovementComponent = BlasterCharacter->GetCharacterMovement();
	
	bIsInAir = CharacterMovementComponent->IsFalling();

	bIsAccelerating = CharacterMovementComponent->GetCurrentAcceleration().Size() > 0.f;

	bWeaponEquipped = BlasterCharacter->IsWeaponEquipped();

	EquippedWeapon = BlasterCharacter->GetEquippedWeapon();

	bIsCrouched = BlasterCharacter->bIsCrouched;

	bIsAiming = BlasterCharacter->IsAiming();

	TurningInPlace = BlasterCharacter->GetTurningInPlace();

	bRotateRootBone = BlasterCharacter->ShouldRotateRootBone();

	bElimmed = BlasterCharacter->IsElimmed();

	const FRotator BaseAimRotation = BlasterCharacter->GetBaseAimRotation();
	const FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(BlasterCharacter->GetVelocity());

	const FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, BaseAimRotation);

	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaSeconds, 6.f);
	YawOffset = DeltaRotation.Yaw;

	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = BlasterCharacter->GetActorRotation();

	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = (Delta.Yaw / DeltaSeconds);
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaSeconds, 6.f);
	Lean = FMath::Clamp(Interp, -90.f, 90.f);

	AoYaw = BlasterCharacter->GetAoYaw();
	AoPitch = BlasterCharacter->GetAoPitch();

	if(bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && BlasterCharacter->GetMesh())
	{
		//LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName(""), RTS_World);
		// Extra function for ease of use with different weapons
		LeftHandTransform = EquippedWeapon->GetWeaponSocketLeftHand();

		const FString WeaponGrabbingHandSocket = BlasterCharacter->WeaponGrabbingHandSocket;
		const FName WeaponGrabbingBoneName(WeaponGrabbingHandSocket);
		FVector OutPosition;
		FRotator OutRotation;
		BlasterCharacter->GetMesh()->TransformToBoneSpace(WeaponGrabbingBoneName, LeftHandTransform.GetLocation(),
		                                                  FRotator::ZeroRotator, OutPosition, OutRotation);

		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotation));

		if(BlasterCharacter->IsLocallyControlled())
		{
			bIsLocallyControlled = true;
			const FTransform RightHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(
				WeaponGrabbingBoneName, RTS_World);
			const FVector RightHandWorldLocation = RightHandTransform.GetLocation();

			const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(RightHandWorldLocation,
				RightHandWorldLocation + (RightHandWorldLocation - BlasterCharacter->GetHitTarget()));
			RightHandRotation = FMath::RInterpTo(RightHandRotation, LookAtRotation, DeltaSeconds, 30.f);
			
		// const FTransform MuzzleTipTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("MuzzleFlash"), RTS_World);		
		// const FVector MuzzleX(FRotationMatrix(MuzzleTipTransform.GetRotation().Rotator()).GetUnitAxis(EAxis::X));
		// DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), MuzzleTipTransform.GetLocation() + MuzzleX * 1000, FColor::Red);
		// DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), BlasterCharacter->GetHitTarget(), FColor::Orange);
		}
	}

	bUseLeftHandIk = BlasterCharacter->GetCombatState() == ECombatState::ECS_Unoccupied;
	
	bUseRightHandIk = BlasterCharacter->GetCombatState() == ECombatState::ECS_Unoccupied &&
		!BlasterCharacter->GetDisableGameplay();

	bUseAimOffsets = BlasterCharacter->GetCombatState() == ECombatState::ECS_Unoccupied &&
		!BlasterCharacter->GetDisableGameplay();;
}
