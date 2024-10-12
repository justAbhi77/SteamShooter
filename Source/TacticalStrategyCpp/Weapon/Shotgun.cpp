
#include "Shotgun.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"

AShotgun::AShotgun():
	NumberOfPellets(10)
{
	PrimaryActorTick.bCanEverTick = true;
	MagCapacity = 4;
}

void AShotgun::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShotgun::FireShotgun(const TArray<FVector_NetQuantize>& HitTargets)
{
	AWeapon::Fire(FVector()); // we do not want to fire a hitscan(single raycast) weapon
	
	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	if(OwnerPawn == nullptr) return;

	AController* InstigatorController = OwnerPawn->GetController();
	if(const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName(MuzzleFlashSocketName)))
	{
		const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());

		const FVector Start = SocketTransform.GetLocation();
		
		TMap<ABlasterCharacter*, uint32> HitMap;
		for(int i=0; i<HitTargets.Num(); i++)
		{
			FHitResult FireHit;
			FVector HitTarget = HitTargets[i];
			WeaponTraceHit(Start, HitTarget, FireHit);
			
			if(ABlasterCharacter* Character = Cast<ABlasterCharacter>(FireHit.GetActor()))
			{
				if(HitMap.Contains(Character))
					HitMap[Character]++;
				else
					HitMap.Emplace(Character, 1);
			}
			
			if(ImpactParticles)
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, FireHit.ImpactPoint,
					FireHit.ImpactNormal.Rotation());
			if(HitSound)
				UGameplayStatics::PlaySoundAtLocation(this, HitSound, FireHit.ImpactPoint,
					.5f, FMath::FRandRange(-.5f,.5f));
		}

		for(auto HitPair : HitMap)
		{
			if(HitPair.Key && InstigatorController && HasAuthority())
			{
				UGameplayStatics::ApplyDamage(HitPair.Key, Damage * HitPair.Value,
					InstigatorController, this, UDamageType::StaticClass());
			}
		}
	}
}

void AShotgun::ShotgunTraceEndWithScatter(const FVector& HitTarget, TArray<FVector_NetQuantize>& HitTargets) const
{
	// same as trace end with scatter for normal weapons since there are multiple pellets in a shotgun
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName(MuzzleFlashSocketName));
	if(MuzzleFlashSocket == nullptr) return;
	
	const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
	const FVector TraceStart = SocketTransform.GetLocation();
	
	const FVector ToTargetNormalized = (HitTarget - TraceStart).GetSafeNormal(),
		SphereCenter = TraceStart + ToTargetNormalized * DistanceToSphere;
	for(uint32 i=0; i < NumberOfPellets; i++)
	{
		const FVector RandVec = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, SphereRadius),
			EndLoc = SphereCenter + RandVec, ToEndLoc = EndLoc - TraceStart;
		HitTargets.Add(TraceStart + ToEndLoc * TRACE_LENGTH / ToEndLoc.Size());
	}
}

void AShotgun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

