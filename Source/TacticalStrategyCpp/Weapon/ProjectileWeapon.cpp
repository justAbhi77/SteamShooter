// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"
#include "Projectile.h"
#include "Engine/SkeletalMeshSocket.h"


AProjectileWeapon::AProjectileWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	// Projectile Weapon default zoom values
	ZoomedFov = 45.f;
	ZoomedInterpSpeed = 25.f;
}

void AProjectileWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	UWorld* World = GetWorld();
	const USkeletalMeshSocket* MuzzleFlashSocket = GetSkeletalWeaponMesh()->GetSocketByName(FName(MuzzleFlashSocketName));
	if(World && MuzzleFlashSocket && InstigatorPawn)
	{
		const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetSkeletalWeaponMesh());
		// from muzzle flash socket(gun) to world hit location (blocking mesh)
		// Calculate the rotation from the muzzle to the hit target
		const FVector ToTarget = HitTarget - SocketTransform.GetLocation();
		const FRotator TargetRotation = ToTarget.Rotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = InstigatorPawn;

		const bool bServer = InstigatorPawn->HasAuthority(), bLocallyControlled = InstigatorPawn->IsLocallyControlled();
		TSubclassOf<AProjectile> ChosenProjectile = nullptr;
		bool bChosenServerSideRewind = false;

		if(bUseServerSideRewind)
		{
		    if((bServer && !bLocallyControlled) || (!bServer && bLocallyControlled))
		    {
		        ChosenProjectile = ServerSideRewindProjectileClass;
		        bChosenServerSideRewind = true;
		    }
		    else
		    {
		        ChosenProjectile = (bServer ? ProjectileClass : ServerSideRewindProjectileClass);
		        bChosenServerSideRewind = false;
		    }
		}
		else if(bServer)
		{
		    ChosenProjectile = ProjectileClass;
		    bChosenServerSideRewind = false;
		}		
		
		AProjectile* SpawnedProjectile = World->SpawnActor<AProjectile>(ChosenProjectile, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
		SpawnedProjectile->bUseServerSideRewind = bChosenServerSideRewind;
		SpawnedProjectile->Damage = Damage;
		SpawnedProjectile->HeadShotDamage = HeadShotDamage;
		
		if(bUseServerSideRewind && !bServer && !bLocallyControlled)
		{
			SpawnedProjectile->TraceStart = SocketTransform.GetLocation();
			SpawnedProjectile->InitialVelocity = SpawnedProjectile->GetActorForwardVector() * SpawnedProjectile->InitialSpeed;
		}
	}
}
