// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TacticalStrategyCpp/Enums/CombatState.h"
#include "TacticalStrategyCpp/Hud/BlasterHud.h"
#include "TacticalStrategyCpp/Weapon/WeaponTypes.h"
#include "CombatComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TACTICALSTRATEGYCPP_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();
	
	friend class ABlasterCharacter;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * Socket for weapon to attach to
	 */
	UPROPERTY(EditAnywhere)
	FString RightHandSocketName;
	
	UPROPERTY(EditAnywhere)
	FString LeftHandSocketName;
	
	UPROPERTY(EditAnywhere)
	FString BackpackSocketName;

	void EquipWeapon(class AWeapon* WeaponToEquip);
	void SwapWeapons();

	void EquipPrimaryWeapon(AWeapon* WeaponToEquip);

	void EquipSecondaryWeapon(AWeapon* WeaponToEquip);

	void Reload();
	
	UFUNCTION(BlueprintCallable)
	void FinishReloading();

	void FireButtonPressed(bool bPressed);

	UFUNCTION(BlueprintCallable)
	void ShotgunShellReload();

	void JumpToShotgunEnd() const;

	UFUNCTION(BlueprintCallable)
	void ThrowGrenadeFinished();
	
	UFUNCTION(BlueprintCallable)
	void LaunchGrenade();

	UFUNCTION(Server, Reliable)
	void Server_LaunchGrenade(const FVector_NetQuantize& Target);

	bool PickupAmmo(EWeaponType AmmoType, int32 AmmoAmount);

protected:
	virtual void BeginPlay() override;

	void SetAiming(bool bIsAiming);

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);

	UFUNCTION()
	void OnRep_EquippedWeapon();
	
	UFUNCTION()
	void OnRep_SecondaryWeapon() const;
	
	void Fire();

	UFUNCTION(Server, Reliable)
	void Server_Fire(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Fire(const FVector_NetQuantize& TraceHitTarget);

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHudCrosshairs(float DeltaTime);

	UFUNCTION(Server, Reliable)
	void Server_Reload();

	void HandleReload() const;

	UFUNCTION()
	void OnRep_CombatState();

	int32 AmountToReload();	
	
	void UpdateAmmoValues();

	void UpdateShotgunAmmoValues();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ShotgunReloadEndSectionName;

	void ThrowGrenade();

	UFUNCTION(Server, Reliable)
	void Server_ThrowGrenade();

	void DropEquippedWeapon() const;

	void AttachActorToRightHand(AActor* ActorToAttach) const;
	void AttachActorToLeftHand(AActor* ActorToAttach) const;
	void AttachActorToBackPack(AActor* ActorToAttach) const;

	void UpdateCarriedAmmo();

	void PlayEquipWeaponSound(const AWeapon* WeaponToEquip) const;

	void ReloadEmptyWeapon();

	void ShowAttachedGrenade(bool bShowGrenade) const;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> GrenadeClass;

	UPROPERTY(ReplicatedUsing=OnRep_Grenades)
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

	UPROPERTY(Replicated)
	bool bAiming;
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	float BaseWalkSpeed;
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	float AimWalkSpeed;

	bool bFireButtonPressed;

	/**
	 * Base spread(Spray pattern range for weapons) for crosshairs
	 */
	UPROPERTY(EditAnywhere)
	float CrosshairBaseLineSpread;

	// Hud and Crosshairs	
	float CrosshairVelocityFactor, CrosshairInAirFactor, CrosshairAimFactor, CrosshairShootingFactor;

	FVector HitTarget;

	/**
	 * Default FOV for Character
	 */
	float DefaultFov;

	/**
	 * Field of view when zooming with a weapon
	 */
	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomedFov;

	/**
	 * Delta(RoC) for Field of view when zooming the weapon
	 */
	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomInterpSpeed;

	float CurrentFov;

	void InterFov(const float DeltaTime);
	
	FHUDPackage HUDPackage;

	/**
	 * Automatic Fire
	 */
	FTimerHandle FireTimer;

	bool bCanFire; // To stop spamming button while timer is running
	
	void StartFireTimer();
	void FireTimerFinished();

	bool CanFire() const;

	/*
	 * Carried Ammo for current Weapon
	 */
	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo)
	int32 CarriedAmmo;

	UFUNCTION()
	void OnRep_CarriedAmmo();

	TMap<EWeaponType, int32> CarriedAmmoMap;
		
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	TMap<EWeaponType, int32> MaxCarriedAmmoMap;

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

	UPROPERTY(ReplicatedUsing = OnRep_CombatState)
	ECombatState CombatState;
	
public:
	FORCEINLINE int32 GetGrenades() const { return Grenades; }

	bool ShouldSwapWeapons() const;
};
