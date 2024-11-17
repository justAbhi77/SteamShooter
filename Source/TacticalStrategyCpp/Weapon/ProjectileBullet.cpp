
#include "ProjectileBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TacticalStrategyCpp/BlasterComponents/LagCompensationComponent.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"
#include "TacticalStrategyCpp/PlayerController/BlasterPlayerController.h"

AProjectileBullet::AProjectileBullet()
{
	PrimaryActorTick.bCanEverTick = false;

	// Initialize and configure the projectile movement component
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->SetIsReplicated(true);
	ProjectileMovementComponent->InitialSpeed = InitialSpeed;
	ProjectileMovementComponent->MaxSpeed = InitialSpeed;
}

#if WITH_EDITOR
/**
 * Called when a property is edited in the editor.
 * Updates projectile speed values in the movement component.
 */
void AProjectileBullet::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = PropertyChangedEvent.Property != nullptr ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	// Update movement component speed when InitialSpeed changes
	if(PropertyName == GET_MEMBER_NAME_CHECKED(AProjectileBullet, InitialSpeed))
		if(ProjectileMovementComponent)
		{
			ProjectileMovementComponent->InitialSpeed = InitialSpeed;
			ProjectileMovementComponent->MaxSpeed = InitialSpeed;
		}
}
#endif

void AProjectileBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(const ABlasterCharacter* OwnerCharacter = Cast<ABlasterCharacter>(GetOwner()))
	{
		if(ABlasterPlayerController* OwnerController = Cast<ABlasterPlayerController>(OwnerCharacter->Controller))
		{
			ABlasterCharacter* HitCharacter = Cast<ABlasterCharacter>(OtherActor);

			if(OwnerCharacter->HasAuthority() && !bUseServerSideRewind)
			{
				float DamageToCause = Damage;

				// Check for headshot damage
				if(HitCharacter)
					DamageToCause = (Hit.BoneName.ToString() == HitCharacter->HeadBoxBone) ? HeadShotDamage : Damage;

				UGameplayStatics::ApplyDamage(OtherActor, DamageToCause, OwnerController, this, UDamageType::StaticClass());

				// Call base class implementation for default behavior
				Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
				return;
			}
			// Handle server-side rewind for lag compensation
			if(HitCharacter && bUseServerSideRewind && OwnerCharacter->GetLagCompensation() && OwnerCharacter->IsLocallyControlled())
				OwnerCharacter->GetLagCompensation()->ProjectileServerScoreRequest(HitCharacter, TraceStart, InitialVelocity, OwnerController->GetServerTime() - OwnerController->SingleTripTime);
		}
	}

	// Since projectile is destroyed as soon as it hits a surface
	// Destroy the projectile after it hits an object
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}
