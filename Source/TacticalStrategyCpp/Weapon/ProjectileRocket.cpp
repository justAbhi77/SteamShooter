
#include "ProjectileRocket.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystemInstanceController.h"
#include "RocketMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

AProjectileRocket::AProjectileRocket()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RocketMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RocketMovementComponent = CreateDefaultSubobject<URocketMovementComponent>(TEXT("RocketMovementComp"));
	RocketMovementComponent->bRotationFollowsVelocity = true;
	RocketMovementComponent->SetIsReplicated(true);
	RocketMovementComponent->InitialSpeed = 1500;
	RocketMovementComponent->MaxSpeed = 1500;
}

void AProjectileRocket::BeginPlay()
{
	Super::BeginPlay();
	
	if(!HasAuthority())
	{
		// ReSharper disable once CppBoundToDelegateMethodIsNotMarkedAsUFunction
		CollisionBox->OnComponentHit.AddDynamic(this, &AProjectileRocket::OnHit);
	}
	
	SpawnTrailSystem();

	if(ProjectileLoop && LoopingSoundAttenuation)
		ProjectileLoopComponent = UGameplayStatics::SpawnSoundAttached(ProjectileLoop, GetRootComponent(),
			FName(), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition,
			false, 1, 1, 0, LoopingSoundAttenuation,
			(USoundConcurrency*)nullptr, false);
}

void AProjectileRocket::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor == GetOwner()) return;

	ExplodeDamage();
	
	// Parent Destroys actor so we call it at the end of the function 
	// Wait for trail dissipates
	// Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);

	DestroyedCosmetics();

	if(ProjectileMesh)
		ProjectileMesh->SetVisibility(false);
	
	if(CollisionBox)
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if(TrailSystemComponent && TrailSystemComponent->GetSystemInstanceController())
		TrailSystemComponent->GetSystemInstanceController()->Deactivate();

	if(ProjectileLoopComponent && ProjectileLoopComponent->IsPlaying())
		ProjectileLoopComponent->Stop();

	StartDestroyTimer();
}

void AProjectileRocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

