
#include "HitScanWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"

AHitScanWeapon::AHitScanWeapon():
	Damage(20), ImpactParticles(nullptr), BeamParticles(nullptr), MuzzleFlash(nullptr), FireSound(nullptr),
	HitSound(nullptr), bUseScatter(false), DistanceToSphere(800), SphereRadius(75)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHitScanWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	if(OwnerPawn == nullptr) return;

	AController* InstigatorController = OwnerPawn->GetController();

	if(const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName(MuzzleFlashSocketName)))
	{
		const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());

		const FVector Start = SocketTransform.GetLocation();
		
		FHitResult FireHit;

		WeaponTraceHit(Start, HitTarget, FireHit);
		
		UWorld* World = GetWorld();		
		// ReSharper disable once CppTooWideScopeInitStatement
		ABlasterCharacter* Character = Cast<ABlasterCharacter>(FireHit.GetActor());
		if(Character && InstigatorController && HasAuthority())
		{
			UGameplayStatics::ApplyDamage(Character, Damage, InstigatorController,
					this, UDamageType::StaticClass());
		}

		if(FireHit.bBlockingHit && ImpactParticles)
			UGameplayStatics::SpawnEmitterAtLocation(World, ImpactParticles, FireHit.ImpactPoint,
				FireHit.ImpactNormal.Rotation());
		if(HitSound)
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, FireHit.ImpactPoint);
		
		if(MuzzleFlash)
			UGameplayStatics::SpawnEmitterAtLocation(World, MuzzleFlash, SocketTransform);
		if(FireSound)
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
}

void AHitScanWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector AHitScanWeapon::TraceEndWithScatter(const FVector& TraceStart, const FVector& HitTarget) const
{
	const FVector ToTargetNormalized = (HitTarget - TraceStart).GetSafeNormal(),
		SphereCenter = TraceStart + ToTargetNormalized * DistanceToSphere,
		RandVec = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, SphereRadius),
		EndLoc = SphereCenter + RandVec, ToEndLoc = EndLoc - TraceStart;

	// DrawDebugSphere(GetWorld(), SphereCenter, SphereRadius, 12, FColor::Red, false, 1.5f);

	// DrawDebugSphere(GetWorld(), EndLoc, 4.f, 12, FColor::Orange, false, 1.5f);

	const FVector Return = TraceStart + ToEndLoc * TRACE_LENGTH / ToEndLoc.Size();
	// DrawDebugLine(GetWorld(), TraceStart, Return, FColor::Cyan, false, 1.5f);
	return Return;
}

void AHitScanWeapon::WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit) const
{
	if(const UWorld* World = GetWorld())
	{
		FVector End = bUseScatter ? TraceEndWithScatter(TraceStart, HitTarget) :
			TraceStart + (HitTarget - TraceStart) * 1.25;

		World->LineTraceSingleByChannel(OutHit, TraceStart, End, ECC_Visibility);	

		if(OutHit.bBlockingHit)
		{
			End = OutHit.ImpactPoint;
		}
		else
		{
			End.Normalize();
			End *= 2500;
		}
		if(BeamParticles)
		{
			// ReSharper disable once CppTooWideScope
			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(World, BeamParticles,
				TraceStart, FRotator::ZeroRotator,true);
			if(Beam)
				Beam->SetVectorParameter(FName("Target"), End);
		}
	}
}

void AHitScanWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

