
#include "HitScanWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "TacticalStrategyCpp/BlasterComponents/LagCompensationComponent.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"
#include "TacticalStrategyCpp/PlayerController/BlasterPlayerController.h"

AHitScanWeapon::AHitScanWeapon():
	ImpactParticles(nullptr), BeamParticles(nullptr), MuzzleFlash(nullptr), FireSound(nullptr),
	HitSound(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
	Damage = 20;
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
		if(Character && InstigatorController)
		{
			const bool bCauseServerDamage = !bUseServerSideRewind || OwnerPawn->IsLocallyControlled();
			if(HasAuthority() && bCauseServerDamage)
				UGameplayStatics::ApplyDamage(Character, Damage, InstigatorController,
						this, UDamageType::StaticClass());
			if(!HasAuthority() && bUseServerSideRewind)
			{
				BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ?
					Cast<ABlasterCharacter>(OwnerPawn) : BlasterOwnerCharacter;
				BlasterOwnerController = BlasterOwnerController == nullptr ?
					Cast<ABlasterPlayerController>(InstigatorController) : BlasterOwnerController;
				if(BlasterOwnerController && BlasterOwnerCharacter && BlasterOwnerCharacter->GetLagCompensation() && BlasterOwnerCharacter->IsLocallyControlled())
					BlasterOwnerCharacter->GetLagCompensation()->ServerScoreRequest(Character, Start, HitTarget,
						BlasterOwnerController->GetServerTime() - BlasterOwnerController->SingleTripTime, this);
			}
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

void AHitScanWeapon::WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit) const
{
	if(const UWorld* World = GetWorld())
	{
		// bUseScatter ? TraceEndWithScatter(TraceStart, HitTarget) :
		FVector End = TraceStart + (HitTarget - TraceStart) * 1.25;

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

