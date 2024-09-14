
#include "ProjectileRocket.h"
#include "Kismet/GameplayStatics.h"

AProjectileRocket::AProjectileRocket():
	RocketMesh(nullptr),
	InnerRadius(200),
	OuterRadius(500),
	DamageFallOf(1) // linear
{
	PrimaryActorTick.bCanEverTick = true;

	RocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RocketMesh"));
	RocketMesh->SetupAttachment(RootComponent);

	RocketMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AProjectileRocket::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectileRocket::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red,
			FString::Printf(TEXT("Comp: %s"), *OtherComp->GetName()));
	if(const APawn* FiringPawn = GetInstigator())
	{
		if(AController* FiringController = FiringPawn->GetController())
			UGameplayStatics::ApplyRadialDamageWithFalloff(this, Damage, 10.f,
				GetActorLocation(), InnerRadius, OuterRadius, DamageFallOf,
				UDamageType::StaticClass(), TArray<AActor*>(), this, FiringController);
	}

	// Parent Destroys actor so we call it at the end of the function 
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);	
}

void AProjectileRocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

