// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "TacticalStrategyCpp/TacticalStrategyCpp.h"
#include "TacticalStrategyCpp/BlasterComponents/CombatComponent.h"
#include "TacticalStrategyCpp/GameMode/BlasterGameMode.h"
#include "TacticalStrategyCpp/PlayerController/BlasterPlayerController.h"
#include "TacticalStrategyCpp/Weapon/Weapon.h"

ABlasterCharacter::ABlasterCharacter():
	CameraBoom(nullptr),
	FollowCamera(nullptr),
	bIsCrouchButtonToggle(true),
	TurningInPlace(ETurningInPlace::ETIP_NotTurning),
	CameraThreshold(200.f),
	TurnThreshold(0.5f),
	MaxHealth(100.f),
	Health(100.f),
	WeaponGrabbingHandSocket("hand_r"),
	FireMontage_Hip("RifleHip"),
	FireMontage_Aim("RifleAim")
{
	PrimaryActorTick.bCanEverTick = true;

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
}

void ABlasterCharacter::OnRep_ReplicatedMovement()
{
	Super::OnRep_ReplicatedMovement();

	SimProxiesTurn();
	TimeSinceLastMovementReplication = 0;
}

void ABlasterCharacter::Elim()
{
	
}

void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();

	UpdateHudHealth();

	if(HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this, &ABlasterCharacter::ReceiveDamage);
	}
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
}

void ABlasterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(Combat)
	{
		Combat->Character = this;
	}
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

void ABlasterCharacter::MoveForward(const float Value)
{
	if(Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw ,0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));

		AddMovementInput(Direction, Value);
	}
}

void ABlasterCharacter::MoveRight(const float Value)
{	
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
void ABlasterCharacter::AimButtonPressed()
{
	if(Combat)
	{
		Combat->SetAiming(true);
		SetAimingSharpness(true);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ABlasterCharacter::AimButtonReleased()
{
	if(Combat)
	{
		Combat->SetAiming(false);
		SetAimingSharpness(false);
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
	if(Combat)
	{
		Combat->FireButtonPressed(true);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ABlasterCharacter::FireButtonReleased()
{
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

void ABlasterCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(GetLocalRole() > ROLE_SimulatedProxy && IsLocallyControlled()) 
		AimOffset(DeltaTime);
	else
	{
		TimeSinceLastMovementReplication += DeltaTime;
		if(TimeSinceLastMovementReplication > 0.25f)
			OnRep_ReplicatedMovement();

		CalculateAoPitch();
	}
	HideCameraOnCharacterClose();
}
