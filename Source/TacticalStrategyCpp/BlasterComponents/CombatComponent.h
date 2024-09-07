// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TacticalStrategyCpp/Hud/BlasterHud.h"
#include "TacticalStrategyCpp/Weapon/WeaponTypes.h"
#include "CombatComponent.generated.h"

#define TRACE_LENGTH 9999.f

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

	void EquipWeapon(class AWeapon* WeaponToEquip);

protected:
	virtual void BeginPlay() override;

	void SetAiming(bool bIsAiming);

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);

	UFUNCTION()
	void OnRep_EquippedWeapon() const;
	void Fire();

	void FireButtonPressed(bool bPressed);

	UFUNCTION(Server, Reliable)
	void Server_Fire(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Fire(const FVector_NetQuantize& TraceHitTarget);

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHudCrosshairs(float DeltaTime);

private:
	UPROPERTY()
	ABlasterCharacter* Character;

	UPROPERTY()
	class ABlasterPlayerController* Controller;

	UPROPERTY()
	class ABlasterHud* Hud;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	AWeapon* EquippedWeapon;

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
	
public:
	
};
