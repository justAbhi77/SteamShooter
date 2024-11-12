// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTypes.h"
#include "GameFramework/Actor.h"
#include "TacticalStrategyCpp/Enums/Team.h"
#include "Weapon.generated.h"

// Enum for different weapon states
UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_EquippedSecondary UMETA(DisplayName = "Equipped Secondary"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),

	EWS_MAX UMETA(DisplayName = "DefaultMAX")
};

// Enum for different fire types
UENUM(BlueprintType)
enum class EFireType : uint8
{
	EFT_HitScan UMETA(DisplayName = "HitScan Weapon"),
	EFT_Projectile UMETA(DisplayName = "Projectile Weapon"),
	EFT_Shotgun UMETA(DisplayName = "Shotgun Weapon"),

	EFT_MAX UMETA(DisplayName = "Default Max")
};

UCLASS()
class TACTICALSTRATEGYCPP_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

	// Toggles the visibility of the pickup widget
	void ShowPickupWidget(bool bShowWidget) const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Fires the weapon towards the specified target
	virtual void Fire(const FVector& HitTarget);

	// Drops the weapon
	virtual void Dropped();

	void SetHudAmmo();

	virtual void OnRep_Owner() override;

	// Adds ammo to the weapon
	void AddAmmo(int32 AmmoToAdd);

	// Enables/disables custom depth for mesh outlining
	void EnableCustomDepth(bool bEnable) const;

	void SetWeaponState(const EWeaponState State, const bool bUpdateLocally = false);

	UPROPERTY(EditAnywhere)
	class USoundCue* EquipSound;

	bool IsEmpty() const;
	bool IsFull() const;

	FORCEINLINE virtual USkeletalMeshComponent* GetSkeletalWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE virtual UStaticMeshComponent* GetStaticWeaponMesh() const { return nullptr; }
	FORCEINLINE float GetZoomedFov() const { return ZoomedFov; }
	FORCEINLINE float GetZoomInterpSpeed() const { return ZoomedInterpSpeed; }
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE int32 GetAmmo() const { return Ammo; }
	FORCEINLINE int32 GetMagCapacity() const { return MagCapacity; }
	FORCEINLINE float GetDamage() const { return Damage; }
	FORCEINLINE float GetHeadshotDamage() const { return HeadShotDamage; }
	FORCEINLINE ETeam GetTeam() const { return Team; }
	FTransform GetWeaponSocketLeftHand() const;

	bool bDestroyWeapon = false;

	// The weapon's firing type
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	EFireType FireType;

	// Scatter settings 
	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	bool bUseScatter;

	// Traces the scatter pattern for weapons
	FVector TraceEndWithScatter(const FVector& HitTarget) const;

	// Is this weapon automatic
	UPROPERTY(EditAnywhere, Category = Combat)
	bool bIsAutomatic;

	// Delay between shots for automatic weapons
	UPROPERTY(EditAnywhere, Category = Combat)
	float FireDelay;

	// Textures for Weapon Crosshairs
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	class UTexture2D* CrosshairsCenter;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsLeft;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsRight;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsTop;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsBottom;

	UPROPERTY(EditAnywhere)
	int32 MagCapacity;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(Client, Reliable)
	void ClientUpdateAmmo(int32 ServerAmmo);

	UFUNCTION(Client, Reliable)
	void ClientAddAmmo(int32 AmmoToAdd);

	// Spends a single round of ammo
	void SpendRound();

	// Responds to high ping
	UFUNCTION()
	void OnPingTooHigh(bool bPingTooHigh);

	UFUNCTION()
	void OnRep_WeaponState();

	virtual void OnEquipped();

	virtual void OnEquippedSecondary();

	virtual void OnDropped();
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	EWeaponType WeaponType;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class USphereComponent* AreaSphere;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	class UWidgetComponent* PickupWidget;

	UPROPERTY()
	class ABlasterCharacter* BlasterOwnerCharacter;
	UPROPERTY()
	class ABlasterPlayerController* BlasterOwnerController;

	// Damage for a normal hit
	UPROPERTY(EditAnywhere)
	float Damage = 20;

	// Damage for a headshot
	UPROPERTY(EditAnywhere)
	float HeadShotDamage = 40;

	UPROPERTY(EditAnywhere)
	FString MuzzleFlashSocketName;

	UPROPERTY(EditAnywhere, Replicated)
	bool bUseServerSideRewind = false;

	// Field of view when zooming with this weapon
	UPROPERTY(EditAnywhere)
	float ZoomedFov;

	// Interpolation speed for zooming
	UPROPERTY(EditAnywhere)
	float ZoomedInterpSpeed;

	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	float DistanceToSphere;
	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	float SphereRadius;

private:
	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;

	// Current ammo count
	UPROPERTY(EditAnywhere)
	int32 Ammo;

	// Ammo sequence count for network sync
	int32 AmmoSequence = 0;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FString LeftHandSocketName;

	// Team designation for the weapon
	UPROPERTY(EditAnywhere)
	ETeam Team;

	UPROPERTY(EditAnywhere)
	FString AmmoEjectFlashSocketName;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UAnimationAsset* FireAnimation;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACasing> CasingClass;
};
