#include "Casing.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


ACasing::ACasing():
	ShellEjectionImpulse(7.f),
	ShellDestroyTime(3.f)
{
	PrimaryActorTick.bCanEverTick = false;

	CasingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CasingMesh"));
	SetRootComponent(CasingMesh);

	CasingMesh->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	CasingMesh->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	CasingMesh->SetSimulatePhysics(true);
	CasingMesh->SetEnableGravity(true);
	CasingMesh->SetNotifyRigidBodyCollision(true); // Make physics aware of generation of hit events
	// In Blueprints this is named as "Simulation Generates Hit Events"
}

void ACasing::BeginPlay()
{
	Super::BeginPlay();

	CasingMesh->OnComponentHit.AddDynamic(this, &ACasing::OnHit);

	CasingMesh->AddImpulse(GetActorForwardVector() * ShellEjectionImpulse);
}

void ACasing::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(ShellSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShellSound, GetActorLocation());
	}

	if(const UWorld* World = GetWorld())
	{
		if(!AfterDropping.IsValid())
		{
			World->GetTimerManager().SetTimer(AfterDropping, this, &ACasing::DestroyAfter, ShellDestroyTime);
		}
	}
}

void ACasing::DestroyAfter()
{
	Destroy();
}

