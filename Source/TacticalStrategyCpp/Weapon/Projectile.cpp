// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "TacticalStrategyCpp/TacticalStrategyCpp.h"

AProjectile::AProjectile() :
	Damage(10.f),
	InnerRadius(200),
	OuterRadius(500),
	DamageFallOf(1.0f) // Linear falloff
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionResponseToChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_SkeletalMesh, ECR_Block);
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetBoxExtent(FVector(5, 2.5f, 2.5f), false);
	
	/* we are using different projectile movement handler based on ammo type so we don't need this here 
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->InitialSpeed = 15000;
	ProjectileMovementComponent->MaxSpeed = 15000;
	*/	
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		CollisionBox->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
		
		// Ignore collision with owner to prevent self-damage thanks @dottheeyes (discord)
		CollisionBox->IgnoreActorWhenMoving(GetOwner(), true);

		StartDestroyTimer();
	}

	if(Tracer)
		TracerComponent = UGameplayStatics::SpawnEmitterAttached(Tracer, CollisionBox, FName(), GetActorLocation(),
			GetActorRotation(), EAttachLocation::KeepWorldPosition);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}

void AProjectile::DestroyedCosmetics()
{
	// Ensure cosmetics only play once
	if(bHasBeenDestroyed) return;
	bHasBeenDestroyed = true;

	if(ImpactParticles)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());

	if(ImpactSound)
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
}

void AProjectile::SpawnTrailSystem()
{
	if(TrailSystem)
		TrailSystemComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(TrailSystem, GetRootComponent(), FName(),
			GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
}

void AProjectile::StartDestroyTimer()
{
	if(!DestroyTimer.IsValid())
		GetWorldTimerManager().SetTimer(DestroyTimer, this, &ThisClass::DestroyTimerFinished, DestroyTime);
}

void AProjectile::DestroyTimerFinished()
{
	Destroy();
}

void AProjectile::ExplodeDamage()
{
	// Applies radial damage with falloff, controlled by inner and outer radii
	if(const APawn* FiringPawn = GetInstigator(); FiringPawn && HasAuthority())
		if(AController* FiringController = FiringPawn->GetController())
			UGameplayStatics::ApplyRadialDamageWithFalloff(this, Damage, 10.f,
				GetActorLocation(), InnerRadius, OuterRadius, DamageFallOf,
				UDamageType::StaticClass(), TArray<AActor*>(), this, FiringController);
}

void AProjectile::Destroyed()
{
	DestroyedCosmetics();

	Super::Destroyed();
}
