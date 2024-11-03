// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTypes.h"
#include "GameFramework/Actor.h"
#include "TacticalStrategyCpp/Enums/Team.h"
#include "Weapon.generated.h"

UENUM (BlueprintType)
enum class EWeaponState: uint8
{
	EWS_Initial UMETA (DisplayName = "Initial State"),
	EWS_Equipped UMETA (DisplayName = "Equipped"),
	EWS_EquippedSecondary UMETA (DisplayName = "Equipped Secondary"),
	EWS_Dropped UMETA (DisplayName = "Dropped"),
	
	EWS_MAX UMETA (DisplayName = "DefaultMAX")
};

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

	virtual void Tick(float DeltaTime) override;

	void ShowPickupWidget(bool bShowWidget) const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Fire(const FVector& HitTarget);

	virtual void Dropped();
	
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
	FString MuzzleFlashSocketName;

	/**
	 * Is this weapon automatic
	 */
	UPROPERTY(EditAnywhere, Category = Combat)
	bool bIsAutomatic;
	
	/**
	 * Automatic weapon delay
	 */
	UPROPERTY(EditAnywhere, Category = Combat)
	float FireDelay;

	void SetHudAmmo();
	virtual void OnRep_Owner() override;

	void AddAmmo(int32 AmmoToAdd);

	UPROPERTY(EditAnywhere)
	class USoundCue* EquipSound;

	/**
	 * Enable/Disable Custom depth for outline of mesh 
	 * @param bEnable Should we enable the Custom depth
	 */
	void EnableCustomDepth(bool bEnable) const;

	bool bDestroyWeapon = false;

	UPROPERTY(EditAnywhere)
	EFireType FireType;	

	UPROPERTY(EditAnywhere, Category="Weapon Scatter")
	bool bUseScatter;	

	FVector TraceEndWithScatter(const FVector& HitTarget) const;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp,	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap( UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);	

	/**
	 * Field of view when zooming with this weapon
	 */
	UPROPERTY(EditAnywhere)
	float ZoomedFov;
	
	/**
	 * Delta(RoC) for Field of view when zooming with this weapon
	 */
	UPROPERTY(EditAnywhere)
	float ZoomedInterpSpeed;
	
	UPROPERTY(EditAnywhere)
	int32 Ammo;

	/**
	 * Number of unprocessed server requests for ammo
	 * updated in spend round and client update ammo
	 * client side prediction for ammo
	 */
	int32 AmmoSequence = 0; 

	UFUNCTION(Client, Reliable)
	void ClientUpdateAmmo(int32 ServerAmmo);
	
	UFUNCTION(Client, Reliable)
	void ClientAddAmmo(int32 AmmoToAdd);
	
	UPROPERTY(EditAnywhere)
	int32 MagCapacity;

	void SpendRound();
	
	UPROPERTY(EditAnywhere, Category="Weapon Scatter")
	float DistanceToSphere;
	
	UPROPERTY(EditAnywhere, Category="Weapon Scatter")
	float SphereRadius;	

	UPROPERTY(EditAnywhere)
	float Damage = 20;
	
	UPROPERTY(EditAnywhere)
	float HeadShotDamage = 40;

	UPROPERTY(EditAnywhere, Replicated)
	bool bUseServerSideRewind = false;	

	UPROPERTY()
	class ABlasterCharacter* BlasterOwnerCharacter;

	UPROPERTY()
	class ABlasterPlayerController* BlasterOwnerController;

	UFUNCTION()
	void OnPingTooHigh(bool bPingTooHigh);

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere)
	ETeam Team;
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properies")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properies")
	class USphereComponent* AreaSphere;

	UPROPERTY(EditAnywhere, Category = "Weapon Properies")
	class UWidgetComponent* PickupWidget;	
	
	UFUNCTION()
	void OnRep_WeaponState();

	virtual void OnEquipped();

	virtual void OnDropped(); 
	
private:
	
	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Weapon Properies")
	EWeaponState WeaponState;

	/**
	 * Socket for IK in the animation graph for the left hand
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	FString LeftHandSocketName;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UAnimationAsset* FireAnimation;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACasing> CasingClass;
	
	UPROPERTY(EditAnywhere)
	FString AmmoEjectFlashSocketName;

public:
	void SetWeaponState(const EWeaponState State, const bool bUpdateLocally = false);

	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }

	FTransform GetWeaponSocketLeftHand() const;

	FORCEINLINE float GetZoomedFov() const { return ZoomedFov; }
	
	FORCEINLINE float GetZoomInterpSpeed() const { return ZoomedInterpSpeed; }

	bool IsEmpty() const;
	bool IsFull() const;

	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }

	FORCEINLINE int32 GetAmmo() const { return Ammo; }
	
	FORCEINLINE int32 GetMagCapacity() const { return MagCapacity; }

	FORCEINLINE float GetDamage() const { return Damage; }
	FORCEINLINE float GetHeadshotDamage() const { return HeadShotDamage; }

	FORCEINLINE ETeam GetTeam() const { return Team; }
	
};
