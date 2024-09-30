
#include "Shotgun.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
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

void AShotgun::Fire(const FVector& HitTarget)
{
	AWeapon::Fire(HitTarget); // we do not want to fire a hitscan(single raycast) weapon 
	
	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	if(OwnerPawn == nullptr) return;

	AController* InstigatorController = OwnerPawn->GetController();

	if(const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName(MuzzleFlashSocketName)))
	{		
		const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());

		const FVector Start = SocketTransform.GetLocation();
		TMap<ABlasterCharacter*, uint32> HitMap; 
		
		for(uint32 i=0; i<NumberOfPellets; i++)
		{
			FHitResult FireHit;
			WeaponTraceHit(Start, HitTarget, FireHit);
			
			// ReSharper disable once CppTooWideScopeInitStatement
			ABlasterCharacter* Character = Cast<ABlasterCharacter>(FireHit.GetActor());
			if(Character && InstigatorController && HasAuthority())
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

void AShotgun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

