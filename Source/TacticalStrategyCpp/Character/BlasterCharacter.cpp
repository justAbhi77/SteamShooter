// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "TacticalStrategyCpp/TacticalStrategyCpp.h"
#include "TacticalStrategyCpp/BlasterComponents/BuffComponent.h"
#include "TacticalStrategyCpp/BlasterComponents/CombatComponent.h"
#include "TacticalStrategyCpp/GameMode/BlasterGameMode.h"
#include "TacticalStrategyCpp/PlayerController/BlasterPlayerController.h"
#include "TacticalStrategyCpp/PlayerState/BlasterPlayerState.h"
#include "TacticalStrategyCpp/Weapon/Weapon.h"

ABlasterCharacter::ABlasterCharacter():
	bDisableGameplay(false),
	CameraBoom(nullptr),
	FollowCamera(nullptr),
	bIsCrouchButtonToggle(true),
	TurningInPlace(ETurningInPlace::ETIP_NotTurning),
	CameraThreshold(200.f),
	TurnThreshold(0.5f),
	MaxHealth(100.f),
	Health(100.f),
	bElimmed(false),
	ElimDelay(3.f),
	DissolveMaterialParam("Dissolve"),
	WeaponGrabbingHandSocket("hand_r"),
	GrenadeSocket("GrenadeSocket"),
	FireMontage_Hip("RifleHip"),
	FireMontage_Aim("RifleAim")
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Always spawn even if players are at player spawn (number os player spawns < players in game)
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 350.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0, 75.f, 75.f);
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	Combat->SetIsReplicated(true);

	Buff = CreateDefaultSubobject<UBuffComponent>(TEXT("BuffComponent"));
	Buff->SetIsReplicated(true);

	DissolveTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("DissolveTimelineComp"));

	ElimBotSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ElimBotSpawnPoint"));
	ElimBotSpawnLocation->SetupAttachment(RootComponent);
	ElimBotSpawnLocation->SetRelativeLocation(FVector(0,0,200));

	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->SetCrouchedHalfHeight(60.f);
	CharacterMovementComponent->MaxWalkSpeedCrouched = 450.f;
	CharacterMovementComponent->bOrientRotationToMovement = true;
	CharacterMovementComponent->NavAgentProps.bCanCrouch = true;
	CharacterMovementComponent->GravityScale = 1.7f;
	CharacterMovementComponent->JumpZVelocity = 850.f;
	CharacterMovementComponent->RotationRate = FRotator(0.f, 850.f, 0.f);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;

	AttachedGrenade = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AttachedGrenade"));
	AttachedGrenade->SetupAttachment(GetMesh(), FName(GrenadeSocket));
	AttachedGrenade->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABlasterCharacter::OnRep_ReplicatedMovement()
{
	Super::OnRep_ReplicatedMovement();

	SimProxiesTurn();
	TimeSinceLastMovementReplication = 0;
}

void ABlasterCharacter::Elim()
{
	if(Combat && Combat->EquippedWeapon)
	{
		Combat->EquippedWeapon->Dropped();
	}
	Multicast_Elim();
	GetWorldTimerManager().SetTimer(ElimTimer, this, &ABlasterCharacter::ElimTimerFinished, ElimDelay);
}

void ABlasterCharacter::Multicast_Elim_Implementation()
{
	if(BlasterPlayerController)
	{
		BlasterPlayerController->SetHudWeaponAmmo(0);
	}
	bElimmed = true;
	PlayElimMontage();

	/*
	if(DissolveMaterialInstance)
	{
		DynamicDissolveMaterialInstance = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicDissolveMaterialInstance);
		DynamicDissolveMaterialInstance->SetScalarParameterValue(FName(DissolveMaterialParam), -0.6f); // Dissolve
		//DynamicDissolveMaterialInstance->SetScalarParameterValue(); // Glow
	}
	*/
	
	StartDissolve();

	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->DisableMovement();
	CharacterMovementComponent->StopMovementImmediately();
	/* Don't want to disable all input just disable certain gameplay elements 
	 if(BlasterPlayerController)
	{
		DisableInput(BlasterPlayerController);		
	}
	*/
	bDisableGameplay = true;
	if(Combat)
		Combat->FireButtonPressed(false);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	const FVector ElimBotSpawnPoint{ ElimBotSpawnLocation->GetComponentLocation() };
	// Elim Bot
	if(ElimBotEffect)
	{
		const FRotator ElimBotSpawnRotation{ ElimBotSpawnLocation->GetComponentRotation() };
		ElimBotComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ElimBotEffect, ElimBotSpawnPoint,
		                                                       ElimBotSpawnRotation);		
	}
	
	if(ElimBotSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ElimBotSound, ElimBotSpawnPoint);		
	}

	if(IsLocallyControlled() && Combat && Combat->bAiming &&  Combat->EquippedWeapon &&
		Combat->EquippedWeapon->GetWeaponType() == EWeaponType::EWT_SniperRifle)
		ShowSniperScopeWidget(false);
}

void ABlasterCharacter::Destroyed()
{
	Super::Destroyed();

	if(ElimBotComp)
	{
		ElimBotComp->DestroyComponent();
	}

	const ABlasterGameMode* BlasterGameMode = Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this));

	// ReSharper disable once CppTooWideScopeInitStatement
	const bool bMatchNotInProgress = BlasterGameMode && BlasterGameMode->GetMatchState() != MatchState::InProgress;

	if(bMatchNotInProgress && Combat && Combat->EquippedWeapon)
		Combat-> EquippedWeapon->Destroy();
}

void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();

	UpdateHudHealth();

	if(HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this, &ABlasterCharacter::ReceiveDamage);
	}
	if(AttachedGrenade)
		AttachedGrenade->SetVisibility(false);
}

void ABlasterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABlasterCharacter::Jump);
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ABlasterCharacter::EquipButtonPressed);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ABlasterCharacter::CrouchButtonPressed);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ABlasterCharacter::CrouchButtonReleased);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ABlasterCharacter::AimButtonPressed);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ABlasterCharacter::AimButtonReleased);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABlasterCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABlasterCharacter::FireButtonReleased);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ABlasterCharacter::ReloadButtonPressed);
	PlayerInputComponent->BindAction("ThrowGrenade", IE_Pressed, this, &ABlasterCharacter::GrenadeButtonPressed);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &ABlasterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABlasterCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ABlasterCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ABlasterCharacter::LookUp);
}

void ABlasterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABlasterCharacter, OverlappingWeapon, COND_OwnerOnly);

	DOREPLIFETIME(ABlasterCharacter, Health);

	DOREPLIFETIME(ABlasterCharacter, bDisableGameplay);
}

void ABlasterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(Combat)
		Combat->Character = this;
	if(Buff)
		Buff->Character = this;
}

void ABlasterCharacter::PlayFireMontage(const bool bAiming) const
{
	if(Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	if(UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && FireWeaponMontage)
	{
		AnimInstance->Montage_Play(FireWeaponMontage);
		const FName SectionName = bAiming ? FName(FireMontage_Aim) : FName(FireMontage_Hip);
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void ABlasterCharacter::PlayReloadMontage() const
{
	if(Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	if(UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && ReloadMontage)
	{
		AnimInstance->Montage_Play(ReloadMontage);
		FName SectionName;
		switch (Combat->EquippedWeapon->GetWeaponType())
		{
			case EWeaponType::EWT_AssaultRifle:
				SectionName = FName("Rifle");
				break;
			case EWeaponType::EWT_RocketLauncher:
				SectionName = FName("RocketLauncher");
				break;
			case EWeaponType::EWT_Pistol:
				SectionName = FName("Pistol");		
				break;
			case EWeaponType::EWT_SMG:
				SectionName = FName("Pistol"); // Reusing pistol section :)
				break;
			case EWeaponType::EWT_Shotgun:
				SectionName = FName("Shotgun");
				break;
			case EWeaponType::EWT_SniperRifle:
				SectionName = FName("SniperRifle");
				break;
			case EWeaponType::EWT_GrenadeLauncher:
				SectionName = FName("Rifle"); // Reusing pistol section :)
				break;
			case EWeaponType::EWT_MAX:
				break;
			default: ;
		}
		AnimInstance->Montage_JumpToSection(SectionName);
	}	
}

void ABlasterCharacter::PlayElimMontage() const
{
	if(UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && ElimMontage)
	{
		AnimInstance->Montage_Play(ElimMontage);
	}
}

void ABlasterCharacter::PlayThrowGrenadeMontage() const
{
	if(UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && ThrowGrenadeMontage)
	{
		AnimInstance->Montage_Play(ThrowGrenadeMontage);
	}	
}

void ABlasterCharacter::PlayHitReactMontage() const
{
	if(Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	if(UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		const FName SectionName = FName("FromFront");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void ABlasterCharacter::UpdateHudHealth()
{
	BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(Controller) : BlasterPlayerController;
	
	if(BlasterPlayerController)
	{
		BlasterPlayerController->SetHudHealth(Health, MaxHealth);
	}
}

void ABlasterCharacter::OnRep_Health()
{
	UpdateHudHealth();
	PlayHitReactMontage();
}

void ABlasterCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatorController, AActor* DamageCauser)
{
	if(bElimmed) return;
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	OnRep_Health();
	if(Health == 0.f)
		if(ABlasterGameMode* BlasterGameMode = GetWorld()->GetAuthGameMode<ABlasterGameMode>())
		{
			BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(Controller) :
				BlasterPlayerController;
			ABlasterPlayerController* AttackerController = Cast<ABlasterPlayerController>(InstigatorController);
			BlasterGameMode->PlayerEliminated(this, BlasterPlayerController, AttackerController);
		}
}

void ABlasterCharacter::PollInit()
{
	if(BlasterPlayerState == nullptr)
	{
		BlasterPlayerState = GetPlayerState<ABlasterPlayerState>();
		if(BlasterPlayerState)
		{
			BlasterPlayerState->AddToScore(0, true);
			BlasterPlayerState->AddToDefeats(0, true);
		}
	}
}

void ABlasterCharacter::MoveForward(const float Value)
{
	if(bDisableGameplay) return;
	if(Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw ,0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));

		AddMovementInput(Direction, Value);
	}
}

void ABlasterCharacter::MoveRight(const float Value)
{
	if(bDisableGameplay) return;
	if(Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw ,0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));

		AddMovementInput(Direction, Value);
	}
}

void ABlasterCharacter::Turn(const float Value)
{
	AddControllerYawInput(Value);
}

void ABlasterCharacter::LookUp(const float Value)
{
	AddControllerPitchInput(Value);
}

void ABlasterCharacter::EquipButtonPressed()
{
	if(bDisableGameplay) return;
	if(Combat)
	{
		if(HasAuthority())
		{
			Combat->EquipWeapon(OverlappingWeapon);
		}
		else
		{
			ServerEquipButtonPressed();
		}
	}
}

void ABlasterCharacter::CrouchButtonPressed()
{
	if(bIsCrouchButtonToggle && bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();		
	}
}

void ABlasterCharacter::CrouchButtonReleased()
{
	if(!bIsCrouchButtonToggle)
	{
		UnCrouch();
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ABlasterCharacter::ReloadButtonPressed()
{
	if(bDisableGameplay) return;
	if(Combat)
		Combat->Reload();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ABlasterCharacter::AimButtonPressed()
{
	if(bDisableGameplay) return;
	if(Combat)
	{
		Combat->SetAiming(true);
		SetAimingSharpness(true);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ABlasterCharacter::AimButtonReleased()
{
	if(bDisableGameplay) return;
	if(Combat)
	{
		Combat->SetAiming(false);
		SetAimingSharpness(false);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ABlasterCharacter::GrenadeButtonPressed()
{
	if(Combat)
	{
		Combat->ThrowGrenade();
	}
}

void ABlasterCharacter::CalculateAoPitch()
{
	AoPitch = GetBaseAimRotation().Pitch;
	if(AoPitch > 90.f && !IsLocallyControlled())
	{
		// map from 270 to 360 to -90 to 0 because unreal compresses rotation data send across network
		const FVector2D InRange(270.f, 360.f), OutRange(-90.f, 0.f);
		AoPitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AoPitch);
	}
}

float ABlasterCharacter::CalculateSpeed() const
{
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	return Velocity.Size();
}

void ABlasterCharacter::AimOffset(const float DeltaTime)
{
	if(Combat && Combat->EquippedWeapon == nullptr) return;

	const float Speed = CalculateSpeed();

	const bool bIsInAir = GetCharacterMovement()->IsFalling();

	if(Speed == 0.f && !bIsInAir) // standing still, not jumping
	{
		bRotateRootBone = true;

		const FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);

		const FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation,
			StartingAimRotation);

		AoYaw = DeltaAimRotation.Yaw;

		if(TurningInPlace == ETurningInPlace::ETIP_NotTurning)
		{
			InterpAoYaw = AoYaw;
		}

		bUseControllerRotationYaw = true;

		TurnInPlace(DeltaTime);
	}
	if(Speed > 0.f || bIsInAir) // running or jumping
	{
		bRotateRootBone = false;
		
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);

		AoYaw = 0.f;
		
		bUseControllerRotationYaw = true;
		
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	}

	CalculateAoPitch();
}

void ABlasterCharacter::SimProxiesTurn()
{
	if(Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	bRotateRootBone = false;

	if(CalculateSpeed() > 0.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		return;
	}
	
	ProxyRotationLastFrame = ProxyRotation;

	ProxyRotation = GetActorRotation();

	ProxyYaw = UKismetMathLibrary::NormalizedDeltaRotator(ProxyRotation, ProxyRotationLastFrame).Yaw;

	if(FMath::Abs(ProxyYaw) > TurnThreshold)
	{
		if(ProxyYaw > TurnThreshold)
			TurningInPlace = ETurningInPlace::ETIP_Right;
		else if(ProxyYaw < -TurnThreshold)
			TurningInPlace = ETurningInPlace::ETIP_Left;
		else
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		return;
	}
	TurningInPlace = ETurningInPlace::ETIP_NotTurning;
}

void ABlasterCharacter::Jump()
{
	// if(bDisableGameplay) return;
	
	if(bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Super::Jump();
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ABlasterCharacter::FireButtonPressed()
{
	if(bDisableGameplay) return;
	if(Combat)
	{
		Combat->FireButtonPressed(true);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ABlasterCharacter::FireButtonReleased()
{
	if(bDisableGameplay) return;
	if(Combat)
	{
		Combat->FireButtonPressed(false);
	}
}

void ABlasterCharacter::OnRep_OverlappingWeapon(const AWeapon* LastWeapon) const
{
	if(LastWeapon)
	{
		LastWeapon->ShowPickupWidget(false);
	}
	if(OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(true);
	}
}

void ABlasterCharacter::ServerEquipButtonPressed_Implementation()
{
	EquipButtonPressed();
}

void ABlasterCharacter::TurnInPlace(float DeltaTime)
{
	if(AoYaw > 90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Right;
	}
	else if(AoYaw < -90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Left;		
	}
	if(TurningInPlace != ETurningInPlace::ETIP_NotTurning)
	{
		InterpAoYaw = FMath::FInterpTo(InterpAoYaw, 0, DeltaTime, 4.f);
		AoYaw = InterpAoYaw;
		if(FMath::Abs(AoYaw) < 15.f)
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
			StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
}

void ABlasterCharacter::SetAimingSharpness(const bool bIsAiming) const
{
	if(FollowCamera == nullptr) return;

	int FocalDistance = 0, ApertureFStop = 4; // Default Values

	if(bIsAiming)
	{
		FocalDistance = 10000; 
		ApertureFStop = 32;
	}
	
	FollowCamera->PostProcessSettings.DepthOfFieldFocalDistance = FocalDistance; // Focal Distance
	FollowCamera->PostProcessSettings.DepthOfFieldFstop = ApertureFStop; // Aperture F-Stop
}

void ABlasterCharacter::HideCameraOnCharacterClose() const
{
	if(!IsLocallyControlled()) return;

	if((FollowCamera->GetComponentLocation() - GetActorLocation()).Size() < CameraThreshold)
	{
		GetMesh()->SetVisibility(false);
		if(Combat && Combat->EquippedWeapon && Combat->EquippedWeapon->GetWeaponMesh())
		{
			Combat->EquippedWeapon->GetWeaponMesh()->bOwnerNoSee = true;
		}
	}
	else
	{
		GetMesh()->SetVisibility(true);
		if(Combat && Combat->EquippedWeapon && Combat->EquippedWeapon->GetWeaponMesh())
		{
			Combat->EquippedWeapon->GetWeaponMesh()->bOwnerNoSee = false;
		}		
	}
}

void ABlasterCharacter::ElimTimerFinished()
{
	if(ABlasterGameMode* BlasterGameMode = GetWorld()->GetAuthGameMode<ABlasterGameMode>())
	{
		BlasterGameMode->RequestRespawn(this, Controller);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ABlasterCharacter::UpdateDissolveMaterial(const float DissolveValue)
{
	
	/*
	if(DynamicDissolveMaterialInstance)
	{
		DynamicDissolveMaterialInstance->SetScalarParameterValue(FName(DissolveMaterialParam), DissolveValue);
	}
	*/
	GetMesh()->SetScalarParameterValueOnMaterials(FName(DissolveMaterialParam), DissolveValue);
}

void ABlasterCharacter::StartDissolve()
{
	DissolveTrack.BindDynamic(this, &ABlasterCharacter::UpdateDissolveMaterial);
	if(DissolveCurve && DissolveTimeLine)
	{
		DissolveTimeLine->AddInterpFloat(DissolveCurve, DissolveTrack);
		DissolveTimeLine->PlayFromStart();
	}
}

void ABlasterCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
	const AWeapon* LastWeapon = OverlappingWeapon;
	OverlappingWeapon = Weapon;
	
	if(IsLocallyControlled())
	{
		OnRep_OverlappingWeapon(LastWeapon);
	}
}

bool ABlasterCharacter::IsWeaponEquipped() const
{
	return (Combat && Combat->EquippedWeapon);
}

bool ABlasterCharacter::IsAiming() const
{
	return (Combat && Combat->bAiming);
}

AWeapon* ABlasterCharacter::GetEquippedWeapon() const
{
	if(Combat == nullptr) return nullptr;

	return Combat->EquippedWeapon;
}

FVector ABlasterCharacter::GetHitTarget() const
{
	if(Combat == nullptr) return FVector();

	return Combat->HitTarget;
}

ECombatState ABlasterCharacter::GetCombatState() const
{
	if(Combat == nullptr) return ECombatState::ECS_Max;

	return Combat->CombatState;
}

void ABlasterCharacter::RotateInPlace(const float DeltaTime)
{
	if(bDisableGameplay)
	{
		bUseControllerRotationYaw = false;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		return;
	}
	
	if(GetLocalRole() > ROLE_SimulatedProxy && IsLocallyControlled()) 
		AimOffset(DeltaTime);
	else
	{
		TimeSinceLastMovementReplication += DeltaTime;
		if(TimeSinceLastMovementReplication > 0.25f)
			OnRep_ReplicatedMovement();

		CalculateAoPitch();
	}
}

void ABlasterCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateInPlace(DeltaTime);	
	HideCameraOnCharacterClose();
	PollInit();
}
