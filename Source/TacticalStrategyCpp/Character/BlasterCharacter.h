// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "TacticalStrategyCpp/Enums/CombatState.h"
#include "TacticalStrategyCpp/Enums/Team.h"
#include "TacticalStrategyCpp/Enums/TurninginPlace.h"
#include "TacticalStrategyCpp/Interfaces/InteractWithCrosshairsInterface.h"
#include "BlasterCharacter.generated.h"

// Delegate to signal when a player leaves the game
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftGame);

/**
 * Handles core functionality like
 * health, shield, weapons, combat actions, animations, and team settings.
 */
UCLASS()
class TACTICALSTRATEGYCPP_API ABlasterCharacter : public ACharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	ABlasterCharacter();

	// Core functions for character operation
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;

	// Animation functions
	void PlayFireMontage(bool bAiming) const;
	void PlayReloadMontage() const;
	void PlayElimMontage() const;
	void PlayThrowGrenadeMontage() const;
	void PlayWeaponSwapMontage() const;
	virtual void OnRep_ReplicatedMovement() override;

	// Functions for handling elimination and leaving game
	void Elim(bool bPlayerLeftGame);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Elim(bool bPlayerLeftGame);
	virtual void Destroyed() override;

	// Player state and gameplay control
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ABlasterPlayerState* BlasterPlayerState;

	UPROPERTY(Replicated)
	bool bDisableGameplay;

	// HUD updates
	void UpdateHudHealth();
	void UpdateHudShield();
	void UpdateHudGrenade();
	void UpdateHudAmmo();

	// Weapon and attachment functions
	void SpawnDefaultWeapon();
	void SetOverlappingWeapon(class AWeapon* Weapon);
	bool IsWeaponEquipped() const;
	bool IsAiming() const;
	AWeapon* GetEquippedWeapon() const;

	// Sniper scope widget
	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(const bool bShowScope);

	// Event dispatchers for gameplay state
	FOnLeftGame OnLeftGame;
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_GainedLead();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_LostLead();
	UFUNCTION(Server, Reliable)
	void Server_LeaveGame();

	// Initialization and team settings
	void OnPollInit();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName="OnPollInit", ScriptName="OnPollInit"))
	void K2_OnPollInit();
	void SetTeamColor(ETeam Team) const;

protected:
	virtual void BeginPlay() override;

	// Movement and input handling
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	// Gameplay actions
	void EquipButtonPressed();
	void CrouchButtonPressed();
	void CrouchButtonReleased();
	void ReloadButtonPressed();
	void AimButtonPressed();
	void AimButtonReleased();
	void GrenadeButtonPressed();
	void FireButtonPressed();
	void FireButtonReleased();

	// Player aim and movement calculations
	void CalculateAoPitch();
	float CalculateSpeed() const;
	void AimOffset(float DeltaTime);
	/**
	 * Used for player characters on the connected machine (Simulated Proxies)
	 */
	void SimProxiesTurn();

	// Jump
	virtual void Jump() override;

	// Hit reaction
	void PlayHitReactMontage() const;

	// Damage and health management
	UFUNCTION()
	void OnRep_Health(float LastHealth);
	UFUNCTION()
	void OnRep_Shield(float LastShield);
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

	// Combat and weapon management
	void PollInit(); // For values that are not yet valid on begin play
	void RotateInPlace(float DeltaTime);
	void DropOrDestroyWeapon(AWeapon* Weapon);
	void SetupBoxComponent(class UBoxComponent*& CompToSetup, const FString& Name, const FString& BoneToAttachTo);
	void SetSpawnPoint();

private:
	// Camera components
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

	// UI components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* OverheadWidget;

	// Weapon handling
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;
	bool bFinishedSwapping = false;
	UFUNCTION()
	void OnRep_OverlappingWeapon(const AWeapon* LastWeapon) const;

	// Combat-related components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCombatComponent* Combat;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBuffComponent* Buff;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ULagCompensationComponent* LagCompensation;

	// Server and network functions
	UFUNCTION(Server, Reliable)
	void Server_EquipButtonPressed();

	// Aiming and character rotation properties
	UPROPERTY(EditAnywhere)
	bool bIsCrouchButtonToggle;
	float AoYaw, AoPitch, InterpAoYaw;
	FRotator StartingAimRotation;
	ETurningInPlace TurningInPlace;
	bool bRotateRootBone;
	float TurnThreshold, ProxyYaw, TimeSinceLastMovementReplication;
	FRotator ProxyRotation, ProxyRotationLastFrame;
	void TurnInPlace(float DeltaTime);

	// Animation properties
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
	/**
	 * Fire Montage Hip Section Names
	 */
	FString FireMontage_Hip;	
	/**
	 * Fire Montage Aim Section Names
	 */
	FString FireMontage_Aim;

	// Camera adjustments
	void SetAimingSharpness(bool bIsAiming) const;
	/**
	 * distance at which character mesh becomes invisible if camera is too close
	 */
	UPROPERTY(EditAnywhere)
	float CameraThreshold;
	void HideCameraOnCharacterClose() const;

	// Character stats
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxHealth;
	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
	float Health;
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxShield = 100;
	UPROPERTY(ReplicatedUsing = OnRep_Shield, EditAnywhere, Category = "Player Stats")
	float Shield = 0;
	bool bElimmed;

	// Elimination and respawn timer
	FTimerHandle ElimTimer;
	/**
	 * Delay so spawn player after death
	 */
	UPROPERTY(EditDefaultsOnly)
	float ElimDelay;
	void ElimTimerFinished();
	bool bLeftGame = false;

	// Dissolve effect properties
	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* DissolveTimeLine;
	UPROPERTY(EditAnywhere)
	UCurveFloat* DissolveCurve;
	UPROPERTY(EditAnywhere)
	FString DissolveMaterialParam;
	FOnTimelineFloat DissolveTrack;
	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);
	void StartDissolve();

	// Team color materials
	UPROPERTY(EditAnywhere, Category = Elim, BlueprintReadWrite, meta=(allowPrivateAccess = "true"))
	UMaterialInterface* RedDissolveMaterial;
	UPROPERTY(EditAnywhere, Category = Elim, BlueprintReadWrite, meta=(allowPrivateAccess = "true"))
	UMaterialInterface* BlueDissolveMaterial;

	// Elimination visual effects
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

	// Attached components and weapons
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* AttachedGrenade;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> DefaultWeaponClass;

	// Game mode reference
	UPROPERTY()
	class ABlasterGameMode* BlasterGameMode;

	// Player Controller reference
	UPROPERTY()
	class ABlasterPlayerController* BlasterPlayerController;

public:
	// Getters and utility functions for gameplay
	FORCEINLINE float GetAoYaw() const { return AoYaw; }
	FORCEINLINE float GetAoPitch() const { return AoPitch; }
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
	FORCEINLINE bool IsHoldingFlag() const;
	ETeam GetTeam();
	void SetHoldingFlag(bool bHolding) const;
	FORCEINLINE bool GetFinishedSwapping() const { return bFinishedSwapping; }
	FORCEINLINE void SetFinishedSwapping(const bool bNewSwapping) { bFinishedSwapping = bNewSwapping; }
	/**
	 * Reference socket (0,0) for IK in the animation graph for the left hand
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	FString WeaponGrabbingHandSocket;	
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	FString GrenadeSocket;
	
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }

	FVector GetHitTarget() const;

	// hit boxes for server side Rewind				
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	UBoxComponent* HeadBox;

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
};