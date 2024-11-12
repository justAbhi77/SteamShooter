
#include "Casing.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


ACasing::ACasing()
{
	PrimaryActorTick.bCanEverTick = false;

	CasingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CasingMesh"));
	SetRootComponent(CasingMesh);

	// Set collision responses to ignore interactions with the camera and pawn channels
	CasingMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	CasingMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	// Enable physics simulation with gravity and ensure hit events are generated
	CasingMesh->SetSimulatePhysics(true);
	CasingMesh->SetEnableGravity(true);
	// In Blueprints this is named as "Simulation Generates Hit Events"
	CasingMesh->SetNotifyRigidBodyCollision(true); // Generates hit events when colliding
}

void ACasing::BeginPlay()
{
	Super::BeginPlay();

	CasingMesh->OnComponentHit.AddDynamic(this, &ACasing::OnHit);

	// Add an initial ejection impulse to simulate casing being ejected from a firearm
	CasingMesh->AddImpulse(GetActorForwardVector() * ShellEjectionImpulse);
}

void ACasing::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(ShellSound)
		UGameplayStatics::PlaySoundAtLocation(this, ShellSound, GetActorLocation());

	// Ensure the destroy timer is set only once
	if(!AfterDropping.IsValid())
		if(const UWorld* World = GetWorld())
			World->GetTimerManager().SetTimer(AfterDropping, this, &ACasing::DestroyAfter, ShellDestroyTime);
}

void ACasing::DestroyAfter()
{
	// Destroy the actor after the timer expires
	Destroy();
}
