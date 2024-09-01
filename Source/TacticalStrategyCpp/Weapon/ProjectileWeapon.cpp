// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"

#include "Projectile.h"
#include "Engine/SkeletalMeshSocket.h"


AProjectileWeapon::AProjectileWeapon():
	MuzzleFlashSocketName("MuzzleFlash")
{
	PrimaryActorTick.bCanEverTick = true;

	// Projectile Weapon default zoom values
	ZoomedFov = 45.f;
	ZoomedInterpSpeed = 25.f;
}

void AProjectileWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectileWeapon::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	if(!HasAuthority()) return;

	APawn* InstigatorPawn = Cast<APawn>(GetOwner());

	if(const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName(MuzzleFlashSocketName)))
	{
		const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		// from muzzle flash socket(gun) to world hit location (blocking mesh) 
		const FVector ToTarget = HitTarget - SocketTransform.GetLocation();
		const FRotator TargetRotation = ToTarget.Rotation();
		
		if(ProjectileClass && InstigatorPawn)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			SpawnParams.Instigator = InstigatorPawn;
			
			if(UWorld* World = GetWorld())
			{
				World->SpawnActor<AProjectile>(ProjectileClass, SocketTransform.GetLocation(),
					TargetRotation, SpawnParams);
			}
		}
	}
}

