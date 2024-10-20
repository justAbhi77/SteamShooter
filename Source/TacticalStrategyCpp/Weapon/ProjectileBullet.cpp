
#include "ProjectileBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TacticalStrategyCpp/BlasterComponents/LagCompensationComponent.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"
#include "TacticalStrategyCpp/PlayerController/BlasterPlayerController.h"

AProjectileBullet::AProjectileBullet()
{
	PrimaryActorTick.bCanEverTick = true;	
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->SetIsReplicated(true);
	ProjectileMovementComponent->InitialSpeed = InitialSpeed;
	ProjectileMovementComponent->MaxSpeed = InitialSpeed;
}

#if WITH_EDITOR
void AProjectileBullet::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = PropertyChangedEvent.Property != nullptr ?
		PropertyChangedEvent.Property->GetFName() : NAME_None;

	if(PropertyName == GET_MEMBER_NAME_CHECKED(AProjectileBullet, InitialSpeed))
	{
		if(ProjectileMovementComponent)
		{			
			ProjectileMovementComponent->InitialSpeed = InitialSpeed;
			ProjectileMovementComponent->MaxSpeed = InitialSpeed;
		}
	}
}
#endif

void AProjectileBullet::BeginPlay()
{
	Super::BeginPlay();
	
	FPredictProjectilePathParams PathParams;
	PathParams.bTraceWithChannel = true;
	PathParams.bTraceWithChannel = true;
	PathParams.DrawDebugTime = 5;
	PathParams.DrawDebugType = EDrawDebugTrace::ForDuration;
	PathParams.LaunchVelocity = GetActorForwardVector() * InitialSpeed;
	PathParams.MaxSimTime = 4;
	PathParams.ProjectileRadius = 5.f;
	PathParams.StartLocation = GetActorLocation();
	PathParams.TraceChannel = ECC_Visibility;
	PathParams.ActorsToIgnore.Add(this);
		
	FPredictProjectilePathResult PathResult;
	UGameplayStatics::PredictProjectilePath(this, PathParams, PathResult);
}

void AProjectileBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(const ABlasterCharacter* OwnerCharacter = Cast<ABlasterCharacter>(GetOwner()))
	{
		if(ABlasterPlayerController* OwnerController = Cast<ABlasterPlayerController>(OwnerCharacter->Controller))
		{
			if(OwnerCharacter->HasAuthority() && !bUseServerSideRewind)
			{
				UGameplayStatics::ApplyDamage(OtherActor, Damage, OwnerController, this,
					UDamageType::StaticClass());
				
				Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
				return;
			}
			ABlasterCharacter* HitCharacter = Cast<ABlasterCharacter>(OtherActor);
			if(HitCharacter && bUseServerSideRewind && OwnerCharacter->GetLagCompensation() &&
				OwnerCharacter->IsLocallyControlled())
			{
				OwnerCharacter->GetLagCompensation()->ProjectileServerScoreRequest(HitCharacter, TraceStart,
					InitializeVelocity, OwnerController->GetServerTime() - OwnerController->SingleTripTime);
			}
		}
	}
	
	// Since projectile is destroyed as soon as it hits a surface 
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}

void AProjectileBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

