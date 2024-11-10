
#include "Shotgun.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "TacticalStrategyCpp/BlasterComponents/LagCompensationComponent.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"
#include "TacticalStrategyCpp/PlayerController/BlasterPlayerController.h"

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
	if(const USkeletalMeshSocket* MuzzleFlashSocket = GetSkeletalWeaponMesh()->GetSocketByName(FName(MuzzleFlashSocketName)))
	{
		const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetSkeletalWeaponMesh());

		const FVector Start = SocketTransform.GetLocation();
		
		TMap<ABlasterCharacter*, uint32> HitMap;
		TMap<ABlasterCharacter*, uint32> HeadShotHitMap;
		for(int i=0; i<HitTargets.Num(); i++)
		{
			FHitResult FireHit;
			FVector HitTarget = HitTargets[i];
			WeaponTraceHit(Start, HitTarget, FireHit);
			
			if(ABlasterCharacter* Character = Cast<ABlasterCharacter>(FireHit.GetActor()))
			{
				if(FireHit.BoneName.ToString() == Character->HeadBoxBone)
				{
					if(HeadShotHitMap.Contains(Character)) HeadShotHitMap[Character]++;
					else HeadShotHitMap.Emplace(Character, 1);				
				}
				else
				{
					if(HitMap.Contains(Character)) HitMap[Character]++;
					else HitMap.Emplace(Character, 1);
				}
			}
			
			if(ImpactParticles)
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, FireHit.ImpactPoint,
					FireHit.ImpactNormal.Rotation());
			if(HitSound)
				UGameplayStatics::PlaySoundAtLocation(this, HitSound, FireHit.ImpactPoint,
					.5f, FMath::FRandRange(-.5f,.5f));
		}

		TArray<ABlasterCharacter*> HitCharacters;
		TMap<ABlasterCharacter*, float> DamageMap;
		for(auto HitPair : HitMap)
			if(HitPair.Key)
			{
				DamageMap.Emplace(HitPair.Key, HitPair.Value * Damage);
				HitCharacters.AddUnique(HitPair.Key);
			}		

		for(auto HeadshotHitPair : HitMap)
			if(HeadshotHitPair.Key)
			{
				if(DamageMap.Contains(HeadshotHitPair.Key)) DamageMap[HeadshotHitPair.Key]+= HeadshotHitPair.Value * HeadShotDamage;
				else DamageMap.Emplace(HeadshotHitPair.Key, HeadshotHitPair.Value * HeadShotDamage);
				HitCharacters.AddUnique(HeadshotHitPair.Key);
			}

		for(auto DamagePair : DamageMap)
			if(DamagePair.Key && InstigatorController)
			{
				const bool bCauseServerDamage = !bUseServerSideRewind || OwnerPawn->IsLocallyControlled();
				if(HasAuthority() && bCauseServerDamage)
					UGameplayStatics::ApplyDamage(DamagePair.Key, Damage * DamagePair.Value,
						InstigatorController, this, UDamageType::StaticClass());			
			}

		
		if(!HasAuthority() && bUseServerSideRewind)
		{					
			BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ?
				Cast<ABlasterCharacter>(OwnerPawn) : BlasterOwnerCharacter;
			BlasterOwnerController = BlasterOwnerController == nullptr ?
				Cast<ABlasterPlayerController>(InstigatorController) : BlasterOwnerController;
			if(BlasterOwnerController && BlasterOwnerCharacter && BlasterOwnerCharacter->GetLagCompensation() && BlasterOwnerCharacter->IsLocallyControlled())
				BlasterOwnerCharacter->GetLagCompensation()->ShotgunServerScoreRequest(HitCharacters, Start, HitTargets,
					BlasterOwnerController->GetServerTime() - BlasterOwnerController->SingleTripTime);				
		}
	}
}

void AShotgun::ShotgunTraceEndWithScatter(const FVector& HitTarget, TArray<FVector_NetQuantize>& HitTargets) const
{
	// same as trace end with scatter for normal weapons since there are multiple pellets in a shotgun
	const USkeletalMeshSocket* MuzzleFlashSocket = GetSkeletalWeaponMesh()->GetSocketByName(FName(MuzzleFlashSocketName));
	if(MuzzleFlashSocket == nullptr) return;
	
	const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetSkeletalWeaponMesh());
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

