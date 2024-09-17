
#include "HitScanWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"

AHitScanWeapon::AHitScanWeapon():
	Damage(20), ImpactParticles(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHitScanWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	if(OwnerPawn == nullptr) return;

	AController* InstigatorController = OwnerPawn->GetController();
	
	if(InstigatorController == nullptr) return;

	if(const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName(MuzzleFlashSocketName)))
	{		
		const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());

		const FVector Start = SocketTransform.GetLocation(), End = Start + (HitTarget - Start) * 1.25;

		if(UWorld* World = GetWorld())
		{
			FHitResult FireHit;
			World->LineTraceSingleByChannel(FireHit, Start, End, ECC_Visibility);

			if(FireHit.bBlockingHit)
			{
				if(ABlasterCharacter* Character = Cast<ABlasterCharacter>(FireHit.GetActor()))
				{
					if(HasAuthority())
						UGameplayStatics::ApplyDamage(Character, Damage, InstigatorController,
							this, UDamageType::StaticClass());
				}

				if(ImpactParticles)
					UGameplayStatics::SpawnEmitterAtLocation(World, ImpactParticles, FireHit.ImpactPoint,
						FireHit.ImpactNormal.Rotation());
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

