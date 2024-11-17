
#include "Shotgun.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "TacticalStrategyCpp/BlasterComponents/LagCompensationComponent.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"
#include "TacticalStrategyCpp/PlayerController/BlasterPlayerController.h"

AShotgun::AShotgun() :
	NumberOfPellets(10)
{
	PrimaryActorTick.bCanEverTick = false;
	MagCapacity = 4;
}

void AShotgun::FireShotgun(const TArray<FVector_NetQuantize>& HitTargets)
{
	AWeapon::Fire(FVector()); // Prevent firing a single hitscan ray.

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn == nullptr) return;

	AController* InstigatorController = OwnerPawn->GetController();
	const USkeletalMeshSocket* MuzzleFlashSocket = GetSkeletalWeaponMesh()->GetSocketByName(FName(MuzzleFlashSocketName));
	if (!MuzzleFlashSocket) return;

	const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetSkeletalWeaponMesh());
	const FVector Start = SocketTransform.GetLocation();

	TMap<ABlasterCharacter*, uint32> HitMap;
	TMap<ABlasterCharacter*, uint32> HeadShotHitMap;

	// Process hits for each pellet.
	for(const FVector& HitTarget : HitTargets)
	{
		FHitResult FireHit;
		WeaponTraceHit(Start, HitTarget, FireHit);

		if(ABlasterCharacter* Character = Cast<ABlasterCharacter>(FireHit.GetActor()))
		{
			TMap<ABlasterCharacter*, uint32>& MapToUse = (FireHit.BoneName.ToString() == Character->HeadBoxBone) ? HeadShotHitMap : HitMap;
			MapToUse.FindOrAdd(Character)++;
		}

		// Spawn impact effects and sounds.
		if(ImpactParticles)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, FireHit.ImpactPoint, FireHit.ImpactNormal.Rotation());
		if(HitSound)
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, FireHit.ImpactPoint, 0.5f, FMath::FRandRange(-0.5f, 0.5f));
	}

	// Calculate total damage for each character.
	TMap<ABlasterCharacter*, float> DamageMap;
	TArray<ABlasterCharacter*> HitCharacters;
	for(const auto& HitPair : HitMap)
		if(HitPair.Key)
		{
			DamageMap.Emplace(HitPair.Key, HitPair.Value * Damage);
			HitCharacters.AddUnique(HitPair.Key);
		}

	for(const auto& HeadshotHitPair : HitMap)
		if(HeadshotHitPair.Key)
		{
			if(DamageMap.Contains(HeadshotHitPair.Key)) DamageMap[HeadshotHitPair.Key]+= HeadshotHitPair.Value * HeadShotDamage;
			else DamageMap.Emplace(HeadshotHitPair.Key, HeadshotHitPair.Value * HeadShotDamage);
			HitCharacters.AddUnique(HeadshotHitPair.Key);
		}

	// Apply damage or handle server-side rewind for each hit character.
	for(const auto& DamagePair : DamageMap)
		if(DamagePair.Key && InstigatorController)
		{
			const bool bCauseServerDamage = !bUseServerSideRewind || OwnerPawn->IsLocallyControlled();
			if(HasAuthority() && bCauseServerDamage)
				UGameplayStatics::ApplyDamage(DamagePair.Key, Damage * DamagePair.Value, InstigatorController, this, UDamageType::StaticClass());
		}

	// Handle server-side rewind for non-authority clients.
	if(!HasAuthority() && bUseServerSideRewind)
	{
		BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(OwnerPawn) : BlasterOwnerCharacter;
		BlasterOwnerController = BlasterOwnerController == nullptr ? Cast<ABlasterPlayerController>(InstigatorController) : BlasterOwnerController;
		if(BlasterOwnerCharacter && BlasterOwnerController && BlasterOwnerCharacter->GetLagCompensation() && BlasterOwnerCharacter->IsLocallyControlled())
			BlasterOwnerCharacter->GetLagCompensation()->ShotgunServerScoreRequest(HitCharacters, Start, HitTargets, BlasterOwnerController->GetServerTime() - BlasterOwnerController->SingleTripTime);
	}
}

void AShotgun::ShotgunTraceEndWithScatter(const FVector& HitTarget, TArray<FVector_NetQuantize>& HitTargets) const
{
	// same as trace end with scatter for normal weapons since there are multiple pellets in a shotgun
	const USkeletalMeshSocket* MuzzleFlashSocket = GetSkeletalWeaponMesh()->GetSocketByName(FName(MuzzleFlashSocketName));
	if(MuzzleFlashSocket == nullptr) return;
	
	const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetSkeletalWeaponMesh());
	const FVector TraceStart = SocketTransform.GetLocation();

	// Calculate a sphere for scatter around the HitTarget.
	const FVector ToTargetNormalized = (HitTarget - TraceStart).GetSafeNormal(), SphereCenter = TraceStart + ToTargetNormalized * DistanceToSphere;

	// Generate scattered end points for each pellet.
	for(uint32 i = 0; i < NumberOfPellets; i++)
	{
		const FVector RandVec = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, SphereRadius), EndLoc = SphereCenter + RandVec, ToEndLoc = EndLoc - TraceStart;
		HitTargets.Add(TraceStart + ToEndLoc * TRACE_LENGTH / ToEndLoc.Size());
	}
}
