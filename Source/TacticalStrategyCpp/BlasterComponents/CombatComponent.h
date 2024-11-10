// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TacticalStrategyCpp/Enums/CombatState.h"
#include "TacticalStrategyCpp/Hud/BlasterHud.h"
#include "TacticalStrategyCpp/Weapon/WeaponTypes.h"
#include "CombatComponent.generated.h"

/**
 * Manages all combat-related functionality for the character, including weapon equipping, firing, reloading, and aiming.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TACTICALSTRATEGYCPP_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();

	// Allow ABlasterCharacter class to access private/protected members
	friend class ABlasterCharacter;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Weapon sockets for attaching weapons and flag
	UPROPERTY(EditAnywhere)
	FString RightHandSocketName;
	
	UPROPERTY(EditAnywhere)
	FString LeftHandSocketName;

	UPROPERTY(EditAnywhere)
	FString BackpackSocketName;

	UPROPERTY(EditAnywhere)
	FString LeftHandFlagSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ShotgunReloadEndSectionName;

	// Weapon management functions
	void EquipWeapon(class AWeapon* WeaponToEquip);
	void DropEquippedWeapon() const;
	void SwapWeapons();
	void EquipPrimaryWeapon(AWeapon* WeaponToEquip);
	void EquipSecondaryWeapon(AWeapon* WeaponToEquip);

	// Reload functionality
	bool bLocallyReloading = false;
	void Reload();
	UFUNCTION(BlueprintCallable)
	void ShotgunShellReload();

	void JumpToShotgunEnd() const;
	UFUNCTION(BlueprintCallable)
	void FinishReloading();

	// Swap
	UFUNCTION(BlueprintCallable)
	void FinishSwap();
	UFUNCTION(BlueprintCallable)
	void FinishSwapAttachWeapons();

	// Firing and grenade throwing functions
	void FireButtonPressed(bool bPressed);
	
	UFUNCTION(BlueprintCallable)
	void LaunchGrenade();
	UFUNCTION(BlueprintCallable)
	void ThrowGrenadeFinished();

	UFUNCTION(Server, Reliable)
	void Server_LaunchGrenade(const FVector_NetQuantize& Target);

	// Ammo pickup handling
	bool PickupAmmo(EWeaponType AmmoType, int32 AmmoAmount);

protected:
	virtual void BeginPlay() override;

	// Handles aiming
	void SetAiming(bool bIsAiming);
	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);

	UFUNCTION()
	void OnRep_EquippedWeapon();

	UFUNCTION()
	void OnRep_TheFlag() const;

	UFUNCTION()
	void OnRep_SecondaryWeapon() const;

	UFUNCTION()
	void OnRep_CombatState();

	// Functions for handling different weapon firing types
	void Fire();
	void FireProjectileWeapon();
	void FireHitScanWeapon();
	void FireShotgun();

	// Firing replicated to server and multicast
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Fire(const FVector_NetQuantize& TraceHitTarget, float FireDelay);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerShotgunFire(const TArray<FVector_NetQuantize>& TraceHitTargets, float FireDelay);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Fire(const FVector_NetQuantize& TraceHitTarget);

	void LocalFire(const FVector_NetQuantize& TraceHitTarget) const;
	void LocalShotgunFire(const TArray<FVector_NetQuantize>& TraceHitTargets);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastShotgunFire(const TArray<FVector_NetQuantize>& TraceHitTargets);

	// Crosshair management and UI updates
	void TraceUnderCrosshairs(FHitResult& TraceHitResult);
	void SetHudCrosshairs(float DeltaTime);

	// Reload functionality
	int32 AmountToReload();
	UFUNCTION(Server, Reliable)
	void Server_Reload();
	void HandleReload() const;

	// Grenade handling
	void ThrowGrenade();
	UFUNCTION(Server, Reliable)
	void Server_ThrowGrenade();

	// Attaching and detaching weapon/flag
	void AttachActorToRightHand(AActor* ActorToAttach) const;
	void AttachActorToLeftHand(AActor* ActorToAttach) const;
	void AttachActorToBackPack(AActor* ActorToAttach) const;
	void AttachFlagToLeftHand(AWeapon* Flag) const;

	// UI updates
	void UpdateAmmoValues();
	void UpdateShotgunAmmoValues();
	void UpdateCarriedAmmo();
	void PlayEquipWeaponSound(const AWeapon* WeaponToEquip) const;
	void ReloadEmptyWeapon();
	void ShowAttachedGrenade(bool bShowGrenade) const;

	// Grenade projectile class
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> GrenadeClass;

	// Grenade inventory and updates
	UPROPERTY(ReplicatedUsing = OnRep_Grenades)
	int32 Grenades;
	int32 MaxGrenades;
	UFUNCTION()
	void OnRep_Grenades();
	void UpdateHudGrenades();

private:
	UPROPERTY()
	ABlasterCharacter* Character;

	UPROPERTY()
	class ABlasterPlayerController* Controller;

	UPROPERTY()
	class ABlasterHud* Hud;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	AWeapon* EquippedWeapon;

	UPROPERTY(ReplicatedUsing = OnRep_SecondaryWeapon)
	AWeapon* SecondaryWeapon;

	UPROPERTY(ReplicatedUsing = OnRep_Aiming)
	bool bAiming = false;
	bool bAimButtonPressed = false;
	UFUNCTION()
	void OnRep_Aiming();

	// Movement speeds for walking and aiming
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float BaseWalkSpeed;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float AimWalkSpeed;

	// Fire control variables
	bool bFireButtonPressed;
	FTimerHandle FireTimer;
	bool bCanFire; // Prevents spamming fire button while timer is active
	void StartFireTimer();
	void FireTimerFinished();
	bool CanFire() const;
	
	// Crosshair properties
	UPROPERTY(EditAnywhere)
	float CrosshairBaseLineSpread;

	float CrosshairVelocityFactor, CrosshairInAirFactor, CrosshairAimFactor, CrosshairShootingFactor;

	// Target location for firing
	FVector HitTarget;

	// Field of View (FOV) settings for zooming
	float DefaultFov, CurrentFov;
	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomedFov;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomInterpSpeed;
	void InterpFov(const float DeltaTime);

	// HUD package for crosshair display
	FHUDPackage HUDPackage;

	// Ammo handling
	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo)
	int32 CarriedAmmo;
	UFUNCTION()
	void OnRep_CarriedAmmo();

	// Maps for tracking ammo per weapon type
	TMap<EWeaponType, int32> CarriedAmmoMap;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TMap<EWeaponType, int32> MaxCarriedAmmoMap;

	// Starting ammo for each weapon type
	UPROPERTY(EditAnywhere)
	int32 StartingArCarriedAmmo;
	
	UPROPERTY(EditAnywhere)
	int32 StartingRocketAmmo;
	
	UPROPERTY(EditAnywhere)
	int32 StartingSmgAmmo;
	
	UPROPERTY(EditAnywhere)
	int32 StartingShotgunAmmo;
	
	UPROPERTY(EditAnywhere)
	int32 StartingPistolAmmo;
	
	UPROPERTY(EditAnywhere)
	int32 StartingSniperAmmo;
	
	UPROPERTY(EditAnywhere)
	int32 StartingGrenadeAmmo;

	void InitializeCarriedAmmo();

	// Combat state and flag holding
	UPROPERTY(ReplicatedUsing = OnRep_CombatState)
	ECombatState CombatState;

	UFUNCTION()
	void OnRep_HoldingFlag() const;

	UPROPERTY(ReplicatedUsing = OnRep_HoldingFlag)
	bool bHoldingFlag = false;

	UPROPERTY(ReplicatedUsing = OnRep_TheFlag)
	AWeapon* TheFlag;

public:
	FORCEINLINE int32 GetGrenades() const { return Grenades; }
	bool ShouldSwapWeapons() const;	
};
