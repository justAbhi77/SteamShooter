// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "TacticalStrategyCpp/Enums/CombatState.h"
#include "TacticalStrategyCpp/Enums/TurninginPlace.h"
#include "TacticalStrategyCpp/Interfaces/InteractWithCrosshairsInterface.h"
#include "BlasterCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftGame);

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
	
	void PlayWeaponSwapMontage() const;

	virtual void OnRep_ReplicatedMovement() override;

	void Elim(bool bPlayerLeftGame);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Elim(bool bPlayerLeftGame);

	virtual void Destroyed() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ABlasterPlayerState* BlasterPlayerState;

	UPROPERTY(Replicated)
	bool bDisableGameplay;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(const bool bShowScope);	
	
	void UpdateHudHealth();
	
	void UpdateHudShield();
	
	void UpdateHudGrenade();

	void UpdateHudAmmo();

	void SpawnDefaultWeapon() const;
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	class UBoxComponent* HeadBox;

	UPROPERTY(EditAnywhere, Category = "ServerRewind")
	FString HeadBoxBone = "head";
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	UBoxComponent* PelvisBox;
	
	UPROPERTY(EditAnywhere, Category = "ServerRewind")
	FString PelvisBoxBone = "pelvis";
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	UBoxComponent* Spine_02Box;
	
	UPROPERTY(EditAnywhere, Category = "ServerRewind")
	FString Spine_02BoxBone = "spine_02";
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UBoxComponent* Spine_03Box;
	
	UPROPERTY(EditAnywhere, Category = "ServerRewind")
	FString Spine_03BoxBone = "spine_03";
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UBoxComponent* UpperArm_LBox;
	
	UPROPERTY(EditAnywhere, Category = "ServerRewind")
	FString UpperArm_LBoxBone = "upperarm_l";
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UBoxComponent* UpperArm_RBox;
	
	UPROPERTY(EditAnywhere, Category = "ServerRewind")
	FString UpperArm_RBoxBone = "upperarm_r";
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UBoxComponent* LowerArm_LBox;
	
	UPROPERTY(EditAnywhere, Category = "ServerRewind")
	FString LowerArm_LBoxBone = "lowerarm_l";
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UBoxComponent* LowerArm_RBox;
	
	UPROPERTY(EditAnywhere, Category = "ServerRewind")
	FString LowerArm_RBoxBone = "lowerarm_r";
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UBoxComponent* Hand_LBox;
	
	UPROPERTY(EditAnywhere, Category = "ServerRewind")
	FString Hand_LBoxBone = "hand_l";
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UBoxComponent* Hand_RBox;
	
	UPROPERTY(EditAnywhere, Category = "ServerRewind")
	FString Hand_RBoxBone = "hand_r";
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UBoxComponent* BackPackBox;
	
	UPROPERTY(EditAnywhere, Category = "ServerRewind")
	FString BackPackBoxBone = "backpack";
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UBoxComponent* BlanketBox;
	
	UPROPERTY(EditAnywhere, Category = "ServerRewind")
	FString BlanketBoxBone = "blanket_l";
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UBoxComponent* Thigh_LBox;
	
	UPROPERTY(EditAnywhere, Category = "ServerRewind")
	FString Thigh_LBoxBone = "thigh_l";
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UBoxComponent* Thigh_RBox;
	
	UPROPERTY(EditAnywhere, Category = "ServerRewind")
	FString Thigh_RBoxBone = "thigh_r";
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UBoxComponent* Calf_LBox;
	
	UPROPERTY(EditAnywhere, Category = "ServerRewind")
	FString Calf_LBoxBone = "calf_l";
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UBoxComponent* Calf_RBox;
	
	UPROPERTY(EditAnywhere, Category = "ServerRewind")
	FString Calf_RBoxBone = "calf_r";
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UBoxComponent* Foot_LBox;
	
	UPROPERTY(EditAnywhere, Category = "ServerRewind")
	FString Foot_LBoxBone = "foot_l";
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UBoxComponent* Foot_RBox;
	
	UPROPERTY(EditAnywhere, Category = "ServerRewind")
	FString Foot_RBoxBone = "foot_r";

	UPROPERTY()
	TMap<FName, UBoxComponent*> HitCollisionBoxes;

	bool bFinishedSwapping = false;

	FOnLeftGame OnLeftGame;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_GainedLead();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_LostLead();
	
	UFUNCTION(Server, Reliable)
	void Server_LeaveGame();

	/**
	 * Called after PollInit when player state is valid
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnPollInit();
	
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

	UFUNCTION()
	void OnRep_Health(float LastHealth);
	
	UFUNCTION()
	void OnRep_Shield(float LastShield);

	UPROPERTY()
	class ABlasterPlayerController* BlasterPlayerController;

	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

	// For values that are not yet valid on begin play
	void PollInit();
	
	void RotateInPlace(float DeltaTime);

	void DropOrDestroyWeapon(class AWeapon* Weapon);

	// hit boxes for server side Rewind

	void SetupBoxComponent(UBoxComponent*& CompToSetup, const FString& Name, const FString& BoneToAttachTo);

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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	class UBuffComponent* Buff;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	class ULagCompensationComponent* LagCompensation;

	UFUNCTION(Server, Reliable)
	void Server_EquipButtonPressed();

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
	
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* WeaponSwapMontage;

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

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxShield = 100;

	UPROPERTY(ReplicatedUsing = OnRep_Shield, EditAnywhere, Category = "Player Stats")
	float Shield = 0;
	
	bool bElimmed;

	FTimerHandle ElimTimer;

	/**
	 * Delay so spawn player after death
	 */
	UPROPERTY(EditDefaultsOnly)
	float ElimDelay; // EditDefault makes it so that the value is only editable on the default character
	
	void ElimTimerFinished();

	bool bLeftGame = false;

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

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* CrownSystem;

	UPROPERTY()
	class UNiagaraComponent* CrownComponent;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* AttachedGrenade;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> DefaultWeaponClass;
	
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
	
	FORCEINLINE float GetShield() const { return Shield; }

	FORCEINLINE void SetHealth(const float Amount) { Health = Amount; }
	
	FORCEINLINE void SetShield(const float Amount) { Shield = Amount; }
	
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	
	FORCEINLINE float GetMaxShield() const { return MaxShield; }

	ECombatState GetCombatState() const;

	FORCEINLINE UCombatComponent* GetCombatComponent() const { return Combat; }

	FORCEINLINE UBuffComponent* GetBuffComponent() const { return Buff; }

	FORCEINLINE bool GetDisableGameplay() const { return bDisableGameplay; }

	FORCEINLINE UAnimMontage* GetReloadMontage() const { return ReloadMontage; }

	FORCEINLINE UStaticMeshComponent* GetAttachedGrenade() const { return AttachedGrenade; }

	bool IsLocallyReloading() const;

	FORCEINLINE ULagCompensationComponent* GetLagCompensation() const { return LagCompensation; }
};