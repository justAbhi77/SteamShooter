
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
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RocketMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RocketMovementComponent = CreateDefaultSubobject<URocketMovementComponent>(TEXT("RocketMovementComp"));
	RocketMovementComponent->bRotationFollowsVelocity = true;
	RocketMovementComponent->SetIsReplicated(true);
	RocketMovementComponent->InitialSpeed = 1500.f;
	RocketMovementComponent->MaxSpeed = 1500.f;
}

void AProjectileRocket::BeginPlay()
{
	Super::BeginPlay();

	if(!HasAuthority())
		CollisionBox->OnComponentHit.AddDynamic(this, &AProjectileRocket::OnHit);

	SpawnTrailSystem();

	if(ProjectileLoop && LoopingSoundAttenuation)
		ProjectileLoopComponent = UGameplayStatics::SpawnSoundAttached(ProjectileLoop, GetRootComponent(), FName(), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, false, 1, 1, 0, LoopingSoundAttenuation, nullptr, false);
}

void AProjectileRocket::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	// Parent Destroys actor so we call it at the end of the function 
	// Wait for trail dissipates
	// Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);

	// Ignore collisions with the rocket's owner
	if(OtherActor == GetOwner()) return;

	// Apply explosion damage to nearby actors
	ExplodeDamage();

	DestroyedCosmetics();

	if(ProjectileMesh)
		ProjectileMesh->SetVisibility(false);

	if(CollisionBox)
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Deactivate the trail system
	if (TrailSystemComponent && TrailSystemComponent->GetSystemInstanceController())
		TrailSystemComponent->GetSystemInstanceController()->Deactivate();

	// Stop the looping sound
	if (ProjectileLoopComponent && ProjectileLoopComponent->IsPlaying())
		ProjectileLoopComponent->Stop();

	// Start the timer to destroy the rocket after cleanup
	StartDestroyTimer();
}
