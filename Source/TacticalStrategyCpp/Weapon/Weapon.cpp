// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Casing.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"
#include "TacticalStrategyCpp/PlayerController/BlasterPlayerController.h"

AWeapon::AWeapon():
	MuzzleFlashSocketName("MuzzleFlash"),
	bIsAutomatic(true),
	FireDelay(0.15f),
	ZoomedFov(30.f),
	ZoomedInterpSpeed(20.f),
	WeaponState(EWeaponState::EWS_Initial),
	LeftHandSocketName("LeftHandSocket"),
	AmmoEjectFlashSocketName("AmmoEject"),
	Ammo(30),
	MagCapacity(30)
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	AActor::SetReplicateMovement(true);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	SetRootComponent(WeaponMesh);

	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);
}

void AWeapon::SetHudAmmo()
{
	BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(GetOwner()) :
		BlasterOwnerCharacter;
	if(BlasterOwnerCharacter)
	{
		BlasterOwnerController = BlasterOwnerController == nullptr
				? Cast<ABlasterPlayerController>(BlasterOwnerCharacter->Controller) : BlasterOwnerController;
		if(BlasterOwnerController)
		{
			BlasterOwnerController->SetHudWeaponAmmo(Ammo);
		}
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
		SetHudAmmo();
	}
}

void AWeapon::AddAmmo(const int32 AmmoToAdd)
{
	Ammo = FMath::Clamp(Ammo - AmmoToAdd, 0, MagCapacity);
	SetHudAmmo();
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		AreaSphere->SetCollisionResponseToChannel (ECC_Pawn, ECR_Overlap);

		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap);
	}
	
	if(PickupWidget)
		PickupWidget->SetVisibility(false);
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor))
	{		
		BlasterCharacter->SetOverlappingWeapon(this);
	}
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor))
	{		
		BlasterCharacter->SetOverlappingWeapon(nullptr);
	}	
}

void AWeapon::OnRep_Ammo()
{
	SetHudAmmo();
}

void AWeapon::SpendRound()
{
	Ammo = FMath::Clamp(Ammo - 1, 0, MagCapacity);
	OnRep_Ammo();
}

void AWeapon::OnRep_WeaponState() const
{
	switch(WeaponState)
	{
	case EWeaponState::EWS_Initial:
		break;
	case EWeaponState::EWS_Equipped:
		ShowPickupWidget(false);
		if(HasAuthority())
		{
			AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		WeaponMesh->SetSimulatePhysics(false);
		WeaponMesh->SetEnableGravity(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);		
		break;
	case EWeaponState::EWS_Dropped:
		if(HasAuthority())
		{
			AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		WeaponMesh->SetSimulatePhysics(true);
		WeaponMesh->SetEnableGravity(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		break;
	case EWeaponState::EWS_MAX:
		break;
	default: ;
	}
}

void AWeapon::SetWeaponState(const EWeaponState State,const bool bUpdateLocally)
{
	WeaponState = State;

	if(HasAuthority() || bUpdateLocally)
	{
		OnRep_WeaponState();
	}
}

FTransform AWeapon::GetWeaponSocketLeftHand() const
{
	if(WeaponMesh)
	{
		return WeaponMesh->GetSocketTransform(FName(LeftHandSocketName), RTS_World);
	}
	return FTransform();
}

bool AWeapon::IsEmpty() const
{
	return Ammo <= 0;
}

void AWeapon::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::ShowPickupWidget(const bool bShowWidget) const
{
	if(PickupWidget)
		PickupWidget->SetVisibility(bShowWidget);
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, WeaponState);
	
	DOREPLIFETIME(AWeapon, Ammo);
}

void AWeapon::Fire(const FVector& HitTarget)
{
	if(FireAnimation)
	{
		WeaponMesh->PlayAnimation(FireAnimation, false);
	}
	if(CasingClass)
	{
		if(const USkeletalMeshSocket* AmmoEjectSocket = GetWeaponMesh()->GetSocketByName(FName(AmmoEjectFlashSocketName)))
		{
			const FTransform SocketTransform = AmmoEjectSocket->GetSocketTransform(GetWeaponMesh());
			// from muzzle flash socket(gun) to world hit location (blocking mesh) 
			// const FVector ToTarget = HitTarget - SocketTransform.GetLocation();		
			
			if(UWorld* World = GetWorld())
			{
				World->SpawnActor<ACasing>(CasingClass, SocketTransform.GetLocation(),
					SocketTransform.GetRotation().Rotator());
			}
		}
	}
	SpendRound();
}

void AWeapon::Dropped()
{
	SetWeaponState(EWeaponState::EWS_Dropped);

	const FDetachmentTransformRules DetachRule(EDetachmentRule::KeepWorld, true);
	WeaponMesh->DetachFromComponent(DetachRule);
	SetOwner(nullptr);
	BlasterOwnerCharacter = nullptr;
	BlasterOwnerController = nullptr;
	
}

