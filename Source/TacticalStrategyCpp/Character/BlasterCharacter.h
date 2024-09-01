// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TacticalStrategyCpp/Enums/TurninginPlace.h"
#include "TacticalStrategyCpp/Interfaces/InteractWithCrosshairsInterface.h"
#include "BlasterCharacter.generated.h"

UCLASS()
class TACTICALSTRATEGYCPP_API ABlasterCharacter : public ACharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	ABlasterCharacter();
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostInitializeComponents() override;

	void PlayFireMontage(const bool bAiming) const;

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastHit();

	virtual void OnRep_ReplicatedMovement() override;

protected:
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void Turn(float Value);

	void LookUp(float Value);

	void EquipButtonPressed();

	void CrouchButtonPressed();

	void CrouchButtonReleased();

	void AimButtonPressed();

	void AimButtonReleased();
	void CalculateAoPitch();
	float CalculateSpeed() const;

	void AimOffset(float DeltaTime);

	/**
	 * Used for player characters on the connected machine (Simulated Proxies)
	 */
	void SimProxiesTurn();

	virtual void Jump() override;

	void FireButtonPressed();
	void FireButtonReleased();
	
	void PlayHitReactMontage() const;

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	class UWidgetComponent* OverheadWidget;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;

	UFUNCTION()
	void OnRep_OverlappingWeapon(const AWeapon* LastWeapon) const;

	UPROPERTY(VisibleAnywhere)
	class UCombatComponent* Combat;

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

	UPROPERTY(EditAnywhere)
	bool bIsCrouchButtonToggle;

	float AoYaw, AoPitch, InterpAoYaw;

	FRotator StartingAimRotation;

	ETurningInPlace TurningInPlace;

	void TurnInPlace(float DeltaTime);

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* FireWeaponMontage;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* HitReactMontage;

	void SetAimingSharpness(bool bIsAiming) const;

	/**
	 * distance at which character mesh becomes invisible if camera is too close
	 */
	UPROPERTY(EditAnywhere)
	float CameraThreshold;

	void HideCameraOnCharacterClose() const;

	bool bRotateRootBone;

	float TurnThreshold;

	FRotator ProxyRotation, ProxyRotationLastFrame;

	float ProxyYaw, TimeSinceLastMovementReplication;
	
public:
	void SetOverlappingWeapon(AWeapon* Weapon);

	bool IsWeaponEquipped() const;

	bool IsAiming() const;

	FORCEINLINE float GetAoYaw() const { return AoYaw; }

	FORCEINLINE float GetAoPitch() const { return AoPitch; }

	AWeapon* GetEquippedWeapon() const;
	
	/**
	 * Reference socket (0,0) for IK in the animation graph for the left hand
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	FString WeaponGrabbingHandSocket;
	
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
	
	/**
	 * Fire Montage Hip Section Names
	 */
	FString FireMontage_Hip;
	
	/**
	 * Fire Montage Aim Section Names
	 */
	FString FireMontage_Aim;

	FVector GetHitTarget() const;

	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }
};