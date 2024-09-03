
#include "ProjectileBullet.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AProjectileBullet::AProjectileBullet()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AProjectileBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectileBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(const ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		if(AController* OwnerController = OwnerCharacter->Controller)
		{
			UGameplayStatics::ApplyDamage(OtherActor, Damage, OwnerController, this, UDamageType::StaticClass());
		}
	}
	
	// Since projectile is destroyed as soon as it hits a surface 
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);	
}

void AProjectileBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

