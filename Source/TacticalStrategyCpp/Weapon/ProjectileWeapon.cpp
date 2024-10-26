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

	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	UWorld* World = GetWorld();
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName(MuzzleFlashSocketName));
	if(World && MuzzleFlashSocket)
	{
		const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		// from muzzle flash socket(gun) to world hit location (blocking mesh) 
		const FVector ToTarget = HitTarget - SocketTransform.GetLocation();
		const FRotator TargetRotation = ToTarget.Rotation();
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = InstigatorPawn;

		AProjectile* SpawnedProjectile;
		if(bUseServerSideRewind)
		{
			if(InstigatorPawn->HasAuthority())
			{
				if(InstigatorPawn->IsLocallyControlled())
				{
					// server, host will use replicated projectile, no SSR	
					SpawnedProjectile = World->SpawnActor<AProjectile>(ProjectileClass, SocketTransform.GetLocation(),
											TargetRotation, SpawnParams);
					SpawnedProjectile->bUseServerSideRewind = false;
					SpawnedProjectile->Damage = Damage;
					SpawnedProjectile->HeadShotDamage = HeadShotDamage;
				}
				else
				{
					// server, not local will use non-replicated projectile, SSR		
					SpawnedProjectile = World->SpawnActor<AProjectile>(ServerSideRewindProjectileClass,
											SocketTransform.GetLocation(), TargetRotation, SpawnParams);
					SpawnedProjectile->bUseServerSideRewind = true;
				}
			}
			else
			{
				// Not server 
				if(InstigatorPawn->IsLocallyControlled())
				{
					// Client, will use non-replicated projectile, SSR
					SpawnedProjectile = World->SpawnActor<AProjectile>(ServerSideRewindProjectileClass,
						SocketTransform.GetLocation(), TargetRotation, SpawnParams);
					SpawnedProjectile->bUseServerSideRewind = true;
					SpawnedProjectile->TraceStart = SocketTransform.GetLocation();
					SpawnedProjectile->InitializeVelocity =
						SpawnedProjectile->GetActorForwardVector() * SpawnedProjectile->InitialSpeed;
				}
				else
				{
					// Client, not local will use non-replicated projectile, no SSR
					SpawnedProjectile = World->SpawnActor<AProjectile>(ServerSideRewindProjectileClass,
											SocketTransform.GetLocation(), TargetRotation, SpawnParams);
					SpawnedProjectile->bUseServerSideRewind = false;
				}
			}
		}
		else // no SSR
		{
			if(InstigatorPawn->HasAuthority())
			{				
				SpawnedProjectile = World->SpawnActor<AProjectile>(ProjectileClass, SocketTransform.GetLocation(),
										TargetRotation, SpawnParams);
				SpawnedProjectile->bUseServerSideRewind = false;
				SpawnedProjectile->Damage = Damage;
				SpawnedProjectile->HeadShotDamage = HeadShotDamage;
			}
		}		
	}
}

