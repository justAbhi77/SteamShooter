// Fill out your copyright notice in the Description page of Project Settings.

#include "Flag.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"
#include "TacticalStrategyCpp/PlayerController/BlasterPlayerController.h"


AFlag::AFlag() :
	FlagMesh(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;

	FlagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlagMesh"));
	SetRootComponent(FlagMesh);

	// Set flag mesh collision properties
	FlagMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	FlagMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlagMesh->SetSimulatePhysics(false);
	FlagMesh->SetEnableGravity(false);

	// Attach collision sphere and widget to flag mesh
	AreaSphere->SetupAttachment(FlagMesh);
	PickupWidget->SetupAttachment(FlagMesh);
	
	WeaponType = EWeaponType::EWT_Flag;
}

void AFlag::BeginPlay()
{
	Super::BeginPlay();

	// Store initial transform for reset functionality
	InitialTransform = GetActorTransform();
	
	// Disable custom depth outline effect
	EnableCustomDepth(false);
}

void AFlag::OnEquipped()
{
	// Hide the pickup widget and disable collision
	ShowPickupWidget(false);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlagMesh->SetSimulatePhysics(false);
	FlagMesh->SetEnableGravity(false);
	FlagMesh->SetCollisionResponseToChannels(ECR_Ignore);

	// Set custom collision for interactions only
	FlagMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	FlagMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	EnableCustomDepth(false);
}

void AFlag::Dropped()
{
	// Reset weapon state to dropped
	SetWeaponState(EWeaponState::EWS_Dropped);

	// Detach the flag with world transform
	const FDetachmentTransformRules DetachRule(EDetachmentRule::KeepWorld, true);
	FlagMesh->DetachFromComponent(DetachRule);

	// Clear ownership data
	SetOwner(nullptr);
	BlasterOwnerCharacter = nullptr;
	BlasterOwnerController = nullptr;
}

void AFlag::ResetFlag()
{
	// Check for and reset flag bearer properties if the flag is held by a character
	if(ABlasterCharacter* FlagBearer = Cast<ABlasterCharacter>(GetOwner()))
	{
		FlagBearer->SetHoldingFlag(false);
		FlagBearer->SetOverlappingWeapon(nullptr);
		FlagBearer->UnCrouch();
	}

	// Return if not on server (authority)
	if(!HasAuthority()) return;

	// Detach and reset flag properties
	const FDetachmentTransformRules DetachRule(EDetachmentRule::KeepWorld, true);
	FlagMesh->DetachFromComponent(DetachRule);
	SetOwner(nullptr);
	BlasterOwnerCharacter = nullptr;
	BlasterOwnerController = nullptr;

	// Reset transform to initial position and enable collision for pickup
	SetActorTransform(InitialTransform);
	SetWeaponState(EWeaponState::EWS_Initial);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AreaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AFlag::OnDropped()
{
	// Enable collision sphere for detection when flag is dropped
	if(HasAuthority())
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// Enable collision and physics for realistic drop effect
	FlagMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	FlagMesh->SetCollisionResponseToAllChannels(ECR_Block);
	FlagMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	FlagMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	FlagMesh->SetSimulatePhysics(true);
	FlagMesh->SetEnableGravity(true);

	// Detach flag from character with world transform
	const FDetachmentTransformRules DetachRule(EDetachmentRule::KeepWorld, true);
	FlagMesh->DetachFromComponent(DetachRule);
}
