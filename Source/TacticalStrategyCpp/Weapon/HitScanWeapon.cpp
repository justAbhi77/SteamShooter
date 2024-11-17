
#include "HitScanWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "TacticalStrategyCpp/BlasterComponents/LagCompensationComponent.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"
#include "TacticalStrategyCpp/PlayerController/BlasterPlayerController.h"

AHitScanWeapon::AHitScanWeapon() :
	ImpactParticles(nullptr), BeamParticles(nullptr), MuzzleFlash(nullptr), FireSound(nullptr), HitSound(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;
	Damage = 20;
}

// Handles firing logic for the weapon, including damage, effects, and server-side rewind.
void AHitScanWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn == nullptr) return;

	AController* InstigatorController = OwnerPawn->GetController();
	const USkeletalMeshSocket* MuzzleFlashSocket = GetSkeletalWeaponMesh()->GetSocketByName(FName(MuzzleFlashSocketName));
	if (!MuzzleFlashSocket) return;

	const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetSkeletalWeaponMesh());
	const FVector Start = SocketTransform.GetLocation();

	// Perform line trace to detect hits
	FHitResult FireHit;
	WeaponTraceHit(Start, HitTarget, FireHit);

	UWorld* World = GetWorld();
	if (!World) return;

	// ReSharper disable once CppTooWideScopeInitStatement
	ABlasterCharacter* Character = Cast<ABlasterCharacter>(FireHit.GetActor());
	if(Character && InstigatorController)
	{
		const bool bCauseServerDamage = !bUseServerSideRewind || OwnerPawn->IsLocallyControlled();
		if(HasAuthority() && bCauseServerDamage)
		{
			const float DamageToCause = (FireHit.BoneName.ToString() == Character->HeadBoxBone) ? HeadShotDamage : Damage;
			UGameplayStatics::ApplyDamage(Character, DamageToCause, InstigatorController, this, UDamageType::StaticClass());
		}

		if(!HasAuthority() && bUseServerSideRewind)
		{
			BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(OwnerPawn) : BlasterOwnerCharacter;
			BlasterOwnerController = BlasterOwnerController == nullptr ? Cast<ABlasterPlayerController>(InstigatorController) : BlasterOwnerController;
			if(BlasterOwnerController && BlasterOwnerCharacter && BlasterOwnerCharacter->GetLagCompensation() && BlasterOwnerCharacter->IsLocallyControlled())
				BlasterOwnerCharacter->GetLagCompensation()->ServerScoreRequest(Character, Start, HitTarget, BlasterOwnerController->GetServerTime() - BlasterOwnerController->SingleTripTime);
		}
	}

	// Spawn effects for impact and muzzle flash
	if(FireHit.bBlockingHit && ImpactParticles)
		UGameplayStatics::SpawnEmitterAtLocation(World, ImpactParticles, FireHit.ImpactPoint, FireHit.ImpactNormal.Rotation());
	if(HitSound)
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, FireHit.ImpactPoint);
	if(MuzzleFlash)
		UGameplayStatics::SpawnEmitterAtLocation(World, MuzzleFlash, SocketTransform);
	if(FireSound)
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
}

void AHitScanWeapon::WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit) const
{
	if(const UWorld* World = GetWorld())
	{
		// bUseScatter ? TraceEndWithScatter(TraceStart, HitTarget) :
		FVector TraceEnd = TraceStart + (HitTarget - TraceStart) * 1.25f;

		// Perform the line trace
		World->LineTraceSingleByChannel(OutHit, TraceStart, TraceEnd, ECC_Visibility);

		// Adjust the trace endpoint if a blocking hit occurs
		if(OutHit.bBlockingHit)
			TraceEnd = OutHit.ImpactPoint;
		else
			TraceEnd = TraceStart + (HitTarget - TraceStart).GetSafeNormal() * 2500.f;

		// Spawn beam effect
		if(BeamParticles)
		{
			// ReSharper disable once CppTooWideScope
			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(World, BeamParticles, TraceStart, FRotator::ZeroRotator,true);
			if(Beam)
				Beam->SetVectorParameter(FName("Target"), TraceEnd);
		}
	}
}
