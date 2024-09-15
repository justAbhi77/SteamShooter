
#include "ProjectileRocket.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystemInstanceController.h"
#include "RocketMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

AProjectileRocket::AProjectileRocket():
	RocketMesh(nullptr),
	DestroyTime(3),
	InnerRadius(200),
	OuterRadius(500), // linear
	DamageFallOf(1)
{
	PrimaryActorTick.bCanEverTick = true;

	RocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RocketMesh"));
	RocketMesh->SetupAttachment(RootComponent);

	RocketMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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

	if(TrailSystem)
	{
		TrailSystemComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(TrailSystem, GetRootComponent(), FName(),
			GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition,
			false);
	}

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
	
	if(const APawn* FiringPawn = GetInstigator(); FiringPawn && HasAuthority())
	{
		if(AController* FiringController = FiringPawn->GetController())
			UGameplayStatics::ApplyRadialDamageWithFalloff(this, Damage, 10.f,
				GetActorLocation(), InnerRadius, OuterRadius, DamageFallOf,
				UDamageType::StaticClass(), TArray<AActor*>(), this, FiringController);
	}

	// Parent Destroys actor so we call it at the end of the function 
	// Wait for trail dissipates
	// Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);

	DestroyedCosmetics();

	if(RocketMesh)
		RocketMesh->SetVisibility(false);
	
	if(CollisionBox)
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if(TrailSystemComponent && TrailSystemComponent->GetSystemInstanceController())
		TrailSystemComponent->GetSystemInstanceController()->Deactivate();

	if(ProjectileLoopComponent && ProjectileLoopComponent->IsPlaying())
		ProjectileLoopComponent->Stop();
	
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &ThisClass::DestroyTimerFinished, DestroyTime);
}

void AProjectileRocket::DestroyTimerFinished()
{
	Destroy();
}

void AProjectileRocket::Destroyed()
{
}

void AProjectileRocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

