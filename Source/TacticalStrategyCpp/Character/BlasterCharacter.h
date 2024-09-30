// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "TacticalStrategyCpp/Enums/CombatState.h"
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

	void PlayReloadMontage() const;
	
	void PlayElimMontage() const;

	void PlayThrowGrenadeMontage() const;

	virtual void OnRep_ReplicatedMovement() override;

	void Elim();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Elim();

	virtual void Destroyed() override;

	UPROPERTY()
	class ABlasterPlayerState* BlasterPlayerState;

	UPROPERTY(Replicated)
	bool bDisableGameplay;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(const bool bShowScope);
protected:
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void Turn(float Value);

	void LookUp(float Value);

	void EquipButtonPressed();

	void CrouchButtonPressed();

	void CrouchButtonReleased();

	void ReloadButtonPressed();

	void AimButtonPressed();

	void AimButtonReleased();

	void GrenadeButtonPressed();
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
	
	void UpdateHudHealth();

	UFUNCTION()
	void OnRep_Health();

	UPROPERTY()
	class ABlasterPlayerController* BlasterPlayerController;

	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

	// For values that are not yet valid on begin play
	void PollInit();
	
	void RotateInPlace(float DeltaTime);

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
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
	UAnimMontage* ReloadMontage;	
	
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ElimMontage;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ThrowGrenadeMontage;

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

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxHealth;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
	float Health;

	bool bElimmed;

	FTimerHandle ElimTimer;

	/**
	 * Delay so spawn player after death
	 */
	UPROPERTY(EditDefaultsOnly)
	float ElimDelay; // EditDefault makes it so that the value is only editable on the default character
	
	void ElimTimerFinished();

	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* DissolveTimeLine; // timeline cpp declaration
	
	FOnTimelineFloat DissolveTrack; // Event for timeline update

	/*
	/**
	 * Dissolve Timeline curve asset 
	 */
	UPROPERTY(EditAnywhere)
	UCurveFloat* DissolveCurve;
	
	/**
	 * Dissolve Timeline curve asset 
	 */
	UPROPERTY(EditAnywhere)
	FString DissolveMaterialParam;

	/**
	 * Dissolve Timeline update event callback
	 */
	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);

	void StartDissolve();

	/*
	/**
	 * Dynamic instance spawned at runtime
	 #1#
	UPROPERTY(VisibleAnywhere, Category = Elim)
	UMaterialInstanceDynamic* DynamicDissolveMaterialInstance;

	/**
	 * Actual material used for elim effect
	 #1#
	UPROPERTY(EditAnywhere, Category = Elim)
	UMaterialInstance* DissolveMaterialInstance;
	*/

	/**
	 * Elim bot to spawn when player eliminated
	 */
	UPROPERTY(EditAnywhere)
	UParticleSystem* ElimBotEffect;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ElimBotComp;

	UPROPERTY(EditAnywhere)
	USceneComponent* ElimBotSpawnLocation;

	UPROPERTY(EditAnywhere)
	class USoundCue* ElimBotSound;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* AttachedGrenade;
	
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
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	FString GrenadeSocket;
	
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

	FORCEINLINE bool IsElimmed() const { return bElimmed; }
	
	FORCEINLINE float GetHealth() const { return Health; }
	
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	ECombatState GetCombatState() const;

	FORCEINLINE UCombatComponent* GetCombatComponent() const { return Combat; }

	FORCEINLINE bool GetDisableGameplay() const { return bDisableGameplay; }

	FORCEINLINE UAnimMontage* GetReloadMontage() const { return ReloadMontage; }

	FORCEINLINE UStaticMeshComponent* GetAttachedGrenade() const { return AttachedGrenade; } 
};