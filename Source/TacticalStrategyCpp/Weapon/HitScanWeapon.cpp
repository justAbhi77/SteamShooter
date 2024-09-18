
#include "HitScanWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"

AHitScanWeapon::AHitScanWeapon():
	Damage(20), ImpactParticles(nullptr), BeamParticles(nullptr)
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

		const FVector Start = SocketTransform.GetLocation(), End = Start + (HitTarget - Start) * 1.25;

		if(UWorld* World = GetWorld())
		{
			FHitResult FireHit;
			World->LineTraceSingleByChannel(FireHit, Start, End, ECC_Visibility);

			FVector BeamEnd = End;

			if(FireHit.bBlockingHit)
			{
				BeamEnd = FireHit.ImpactPoint;
				// ReSharper disable once CppTooWideScopeInitStatement
				ABlasterCharacter* Character = Cast<ABlasterCharacter>(FireHit.GetActor());
				if(Character && InstigatorController && HasAuthority())
				{
					UGameplayStatics::ApplyDamage(Character, Damage, InstigatorController,
							this, UDamageType::StaticClass());
				}

				if(ImpactParticles)
					UGameplayStatics::SpawnEmitterAtLocation(World, ImpactParticles, FireHit.ImpactPoint,
						FireHit.ImpactNormal.Rotation());			
			}
			if(BeamParticles)
			{
				UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(World, BeamParticles,
					SocketTransform);
				if(Beam)
					Beam->SetVectorParameter(FName("Target"), BeamEnd);
			}
		}
	}
}

void AHitScanWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHitScanWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

