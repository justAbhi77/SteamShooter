// Fill out your copyright notice in the Description page of Project Settings.

#include "Flag.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"
#include "TacticalStrategyCpp/PlayerController/BlasterPlayerController.h"


AFlag::AFlag():
	FlagMesh(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
	WeaponType = EWeaponType::EWT_Flag;

	FlagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlagMesh"));
	SetRootComponent(FlagMesh);
	FlagMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	FlagMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlagMesh->SetSimulatePhysics(false);
	FlagMesh->SetEnableGravity(false);
	
	AreaSphere->SetupAttachment(FlagMesh);
	PickupWidget->SetupAttachment(FlagMesh);
}

void AFlag::Dropped()
{
	//SetWeaponState(EWeaponState::EWS_Dropped);

	const FDetachmentTransformRules DetachRule(EDetachmentRule::KeepWorld, true);
	FlagMesh->DetachFromComponent(DetachRule);
	SetOwner(nullptr);
	BlasterOwnerCharacter = nullptr;
	BlasterOwnerController = nullptr;	
}

void AFlag::ResetFlag()
{	
	if(ABlasterCharacter* FlagBearer = Cast<ABlasterCharacter>(GetOwner()))
	{
		FlagBearer->SetHoldingFlag(false);
		FlagBearer->SetOverlappingWeapon(nullptr);
		FlagBearer->UnCrouch();
	}

	if(!HasAuthority()) return;
	Dropped();
	SetActorTransform(InitialTransform);
	
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AreaSphere->SetCollisionResponseToChannel (ECC_Pawn, ECR_Overlap);
	SetWeaponState(EWeaponState::EWS_Initial);
}

void AFlag::BeginPlay()
{
	Super::BeginPlay();

	InitialTransform = GetActorTransform();
	EnableCustomDepth(false);
}

void AFlag::OnEquipped()
{
	ShowPickupWidget(false);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlagMesh->SetSimulatePhysics(false);
	FlagMesh->SetEnableGravity(false);
	FlagMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	FlagMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
}

void AFlag::OnDropped()
{
	if(HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	FlagMesh->SetSimulatePhysics(true);
	FlagMesh->SetEnableGravity(true);
	FlagMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);		
	FlagMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	FlagMesh->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
}

void AFlag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

