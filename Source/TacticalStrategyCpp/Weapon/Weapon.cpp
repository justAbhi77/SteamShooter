﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Casing.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "TacticalStrategyCpp/BlasterComponents/CombatComponent.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"
#include "TacticalStrategyCpp/PlayerController/BlasterPlayerController.h"

AWeapon::AWeapon() :
	bIsAutomatic(true),
	FireDelay(0.15f),
	MagCapacity(30),
	MuzzleFlashSocketName("MuzzleFlash"),
	ZoomedFov(30.f),
	ZoomedInterpSpeed(20.f),
	DistanceToSphere(800),
	SphereRadius(75),
	WeaponState(EWeaponState::EWS_Initial),
	Ammo(30),
	LeftHandSocketName("LeftHandSocket"),
	AmmoEjectFlashSocketName("AmmoEject")
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	AActor::SetReplicateMovement(true);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_PURPLE);
	EnableCustomDepth(true);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToChannels(ECR_Ignore);
	AreaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);
	PickupWidget->SetVisibility(false);
}

void AWeapon::SetHudAmmo()
{
	BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(GetOwner()) : BlasterOwnerCharacter;
	if(BlasterOwnerCharacter)
	{
		BlasterOwnerController = BlasterOwnerController == nullptr ? Cast<ABlasterPlayerController>(BlasterOwnerCharacter->Controller) : BlasterOwnerController;
		if(BlasterOwnerController)
			BlasterOwnerController->SetHudWeaponAmmo(Ammo);
	}
}

void AWeapon::OnRep_Owner()
{
	Super::OnRep_Owner();
	if(Owner == nullptr)
	{
		BlasterOwnerCharacter = nullptr;
		BlasterOwnerController = nullptr;
	}
	else
	{
		BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(GetOwner()) : BlasterOwnerCharacter;
		if(BlasterOwnerCharacter && BlasterOwnerCharacter->GetEquippedWeapon() == this)
			SetHudAmmo();
	}
}

// Add ammo, up to MagCapacity, and update UI
void AWeapon::AddAmmo(const int32 AmmoToAdd)
{
	Ammo = FMath::Clamp(Ammo + AmmoToAdd, 0, MagCapacity);
	SetHudAmmo();
	ClientAddAmmo(AmmoToAdd);
}

void AWeapon::EnableCustomDepth(const bool bEnable) const
{
	if(WeaponMesh)
		WeaponMesh->SetRenderCustomDepth(bEnable);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap);
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, WeaponState);
	DOREPLIFETIME_CONDITION(AWeapon, bUseServerSideRewind, COND_OwnerOnly);
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor))
	{
		if(WeaponType == EWeaponType::EWT_Flag && BlasterCharacter->GetTeam() == Team) return;
		if(BlasterCharacter->IsHoldingFlag()) return;

		BlasterCharacter->SetOverlappingWeapon(this);
	}
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor))
	{
		if(WeaponType == EWeaponType::EWT_Flag && BlasterCharacter->GetTeam() == Team) return;
		if(BlasterCharacter->IsHoldingFlag()) return;

		BlasterCharacter->SetOverlappingWeapon(nullptr);
	}
}

void AWeapon::SpendRound()
{
    // Decrement ammo and update UI
	Ammo = FMath::Clamp(Ammo - 1, 0, MagCapacity);
	SetHudAmmo();
	if(HasAuthority())
		ClientUpdateAmmo(Ammo);
	else
		AmmoSequence++;
}

void AWeapon::ClientUpdateAmmo_Implementation(int32 ServerAmmo)
{
    // Client-side ammo correction in case of desync
	if(HasAuthority()) return;
	
	Ammo = ServerAmmo;
	--AmmoSequence;
	Ammo -= AmmoSequence;
	SetHudAmmo();
}

void AWeapon::ClientAddAmmo_Implementation(int32 AmmoToAdd)
{
	if(HasAuthority()) return;
	Ammo = FMath::Clamp(Ammo + AmmoToAdd, 0, MagCapacity);
	BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(GetOwner()) : BlasterOwnerCharacter;
	if(BlasterOwnerCharacter && BlasterOwnerCharacter->GetCombatComponent() && IsFull())
		BlasterOwnerCharacter->GetCombatComponent()->JumpToShotgunEnd();
	SetHudAmmo();
}

void AWeapon::OnPingTooHigh(const bool bPingTooHigh)
{
	bUseServerSideRewind = !bPingTooHigh;
}

void AWeapon::OnRep_WeaponState()
{
	switch(WeaponState)
	{
	case EWeaponState::EWS_Initial: break;
	case EWeaponState::EWS_Equipped: OnEquipped(); break;
	case EWeaponState::EWS_EquippedSecondary: OnEquippedSecondary(); break;
	case EWeaponState::EWS_Dropped: OnDropped(); break;
	case EWeaponState::EWS_MAX: break;
	default: ;
	}
}

void AWeapon::OnEquipped()
{
    // Configures the weapon for being equipped by the player
	ShowPickupWidget(false);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetEnableGravity(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if(WeaponType == EWeaponType::EWT_SMG)
	{
		WeaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponMesh->SetEnableGravity(true);
	}
	EnableCustomDepth(false);
	BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(GetOwner()) : BlasterOwnerCharacter;
	if(BlasterOwnerCharacter && bUseServerSideRewind)
	{
		BlasterOwnerController = BlasterOwnerController == nullptr ? Cast<ABlasterPlayerController>(BlasterOwnerCharacter->Controller) : BlasterOwnerController;
		if(BlasterOwnerController && HasAuthority() && !BlasterOwnerController->HighPingDelegate.IsBound())
		{
			BlasterOwnerController->HighPingDelegate.AddDynamic(this, &AWeapon::OnPingTooHigh);
		}
	}
}

void AWeapon::OnEquippedSecondary()
{
	ShowPickupWidget(false);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetEnableGravity(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if(WeaponType == EWeaponType::EWT_SMG)
	{
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponMesh->SetEnableGravity(true);
		WeaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	}
	GetSkeletalWeaponMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_TAN);
	GetSkeletalWeaponMesh()->MarkRenderStateDirty();
	BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(GetOwner()) :
		BlasterOwnerCharacter;
	if(BlasterOwnerCharacter && bUseServerSideRewind)
	{
		BlasterOwnerController = BlasterOwnerController == nullptr
				? Cast<ABlasterPlayerController>(BlasterOwnerCharacter->Controller) : BlasterOwnerController;
		if(BlasterOwnerController && HasAuthority() && BlasterOwnerController->HighPingDelegate.IsBound())
		{
			BlasterOwnerController->HighPingDelegate.RemoveDynamic(this, &AWeapon::OnPingTooHigh);
		}
	}
}

void AWeapon::OnDropped()
{
	if(HasAuthority())
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->SetEnableGravity(true);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	WeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_PURPLE);
	WeaponMesh->MarkRenderStateDirty();
	EnableCustomDepth(true);
	BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(GetOwner()) :
		BlasterOwnerCharacter;
	if(BlasterOwnerCharacter && bUseServerSideRewind)
	{
		BlasterOwnerController = BlasterOwnerController == nullptr
				? Cast<ABlasterPlayerController>(BlasterOwnerCharacter->Controller) : BlasterOwnerController;
		if(BlasterOwnerController && HasAuthority() && BlasterOwnerController->HighPingDelegate.IsBound())
		{
			BlasterOwnerController->HighPingDelegate.RemoveDynamic(this, &AWeapon::OnPingTooHigh);
		}
	}
}

void AWeapon::SetWeaponState(const EWeaponState State, const bool bUpdateLocally)
{
	WeaponState = State;

	if(HasAuthority() || bUpdateLocally)
		OnRep_WeaponState();
}

bool AWeapon::IsEmpty() const
{
	return Ammo <= 0;
}

bool AWeapon::IsFull() const
{
	return Ammo == MagCapacity;
}

void AWeapon::ShowPickupWidget(const bool bShowWidget) const
{
	if(PickupWidget)
		PickupWidget->SetVisibility(bShowWidget);
}

FTransform AWeapon::GetWeaponSocketLeftHand() const
{
	if(WeaponMesh)
		return WeaponMesh->GetSocketTransform(FName(LeftHandSocketName), RTS_World);
	return FTransform();
}

void AWeapon::Fire(const FVector& HitTarget)
{
    // Plays fire animation, spawns casing, and calls SpendRound to decrement ammo
	if(FireAnimation)
		WeaponMesh->PlayAnimation(FireAnimation, false);
	if(CasingClass)
		if(const USkeletalMeshSocket* AmmoEjectSocket = GetSkeletalWeaponMesh()->GetSocketByName(FName(AmmoEjectFlashSocketName)))
		{
			const FTransform SocketTransform = AmmoEjectSocket->GetSocketTransform(GetSkeletalWeaponMesh());
			// from muzzle flash socket(gun) to world hit location (blocking mesh) 
			// const FVector ToTarget = HitTarget - SocketTransform.GetLocation();		
			
			if(UWorld* World = GetWorld())
			{
				World->SpawnActor<ACasing>(CasingClass, SocketTransform.GetLocation(),
					SocketTransform.GetRotation().Rotator());
			}
		}
	SpendRound();
}

void AWeapon::Dropped()
{
    // Prepares weapon for being dropped, detaching from owner and enabling physics
	SetWeaponState(EWeaponState::EWS_Dropped);
	const FDetachmentTransformRules DetachRule(EDetachmentRule::KeepWorld, true);
	WeaponMesh->DetachFromComponent(DetachRule);
	SetOwner(nullptr);
	BlasterOwnerCharacter = nullptr;
	BlasterOwnerController = nullptr;
}

FVector AWeapon::TraceEndWithScatter(const FVector& HitTarget) const
{
	const FVector TraceStart = GetSkeletalWeaponMesh()->GetSocketLocation(FName(MuzzleFlashSocketName));

	const FVector ToTargetNormalized = (HitTarget - TraceStart).GetSafeNormal(),
		SphereCenter = TraceStart + ToTargetNormalized * DistanceToSphere,
		RandVec = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, SphereRadius),
		EndLoc = SphereCenter + RandVec, ToEndLoc = EndLoc - TraceStart;

	// DrawDebugSphere(GetWorld(), SphereCenter, SphereRadius, 12, FColor::Red, false, 1.5f);

	// DrawDebugSphere(GetWorld(), EndLoc, 4.f, 12, FColor::Orange, false, 1.5f);

	const FVector Return = TraceStart + ToEndLoc * TRACE_LENGTH / ToEndLoc.Size();
	// DrawDebugLine(GetWorld(), TraceStart, Return, FColor::Cyan, false, 1.5f);
	return Return;
}
