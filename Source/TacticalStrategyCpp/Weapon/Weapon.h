// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM (BlueprintType)
enum class EWeaponState: uint8
{
	EWS_Initial UMETA (DisplayName = "Initial State"),
	EWS_Equipped UMETA (DisplayName = "Equipped"),
	EWS_Dropped UMETA (DisplayName = "Dropped"),
	
	EWS_MAX UMETA (DisplayName = "DefaultMAX")
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
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properies")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properies")
	class USphereComponent* AreaSphere;
	
	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Weapon Properies")
	EWeaponState WeaponState;	
	
	UFUNCTION()
	void OnRep_WeaponState() const;

	UPROPERTY(EditAnywhere, Category = "Weapon Properies")
	class UWidgetComponent* PickupWidget;

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
	void SetWeaponState(const EWeaponState State);

	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }

	FTransform GetWeaponSocketLeftHand() const;
	
};
