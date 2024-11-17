
#include "ProjectileGrenade.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


AProjectileGrenade::AProjectileGrenade()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrenadeMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->SetIsReplicated(true);
	ProjectileMovementComponent->InitialSpeed = 15000.f;
	ProjectileMovementComponent->MaxSpeed = 15000.f;
	ProjectileMovementComponent->bShouldBounce = true; // Enable bouncing
}

void AProjectileGrenade::BeginPlay()
{
	// Skip the parent `AProjectile` BeginPlay logic (use `AActor::BeginPlay` instead)
	AActor::BeginPlay();

	// Spawn visual trail system
	SpawnTrailSystem();
	// Start the destroy timer for auto destruct
	StartDestroyTimer();

	// Bind the bounce event to OnBounce handler
	ProjectileMovementComponent->OnProjectileBounce.AddDynamic(this, &AProjectileGrenade::OnBounce);
}

void AProjectileGrenade::Destroyed()
{
	ExplodeDamage();

	Super::Destroyed();
}

// Called when the grenade bounces off a surface.Plays a bounce sound at the grenade's location.
// ReSharper disable once CppMemberFunctionMayBeConst
void AProjectileGrenade::OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	// Play the bounce sound at the grenade's current location
	if(BounceSound)
		UGameplayStatics::PlaySoundAtLocation(this, BounceSound, GetActorLocation());
}
