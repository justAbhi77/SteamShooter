
#include "Pickup.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

APickup::APickup() :
	OverlappedActor(nullptr), OverlapSphere(nullptr), PickupSound(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Initialize and configure overlap sphere for detecting pickup events
	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	OverlapSphere->SetupAttachment(RootComponent);
	OverlapSphere->SetSphereRadius(150.f); // Defines the collision radius
	OverlapSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Initially disabled to avoid premature collisions
	OverlapSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Only react to pawns

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetupAttachment(OverlapSphere);
	PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickupEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PickupEffectComp"));
	PickupEffectComponent->SetupAttachment(RootComponent);
}

void APickup::BeginPlay()
{
	Super::BeginPlay();

	// Enable overlap after a delay if the actor has authority
	if(HasAuthority())
		GetWorldTimerManager().SetTimer(BindOverlapTimer, this, &APickup::BindOverlapTimerFinished, BindOverlapTime);
}

void APickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Store the overlapped actor for use during the destruction process
	OverlappedActor = OtherActor;
}

void APickup::BindOverlapTimerFinished()
{
	// Enable collision and bind overlap event to respond to pickups
	OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnSphereOverlap);
	OverlapSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void APickup::Destroyed()
{
	// Spawn visual and audio effects when the pickup is destroyed
	if(PickupEffect && OverlappedActor)
		UNiagaraFunctionLibrary::SpawnSystemAttached(PickupEffect, OverlappedActor->GetRootComponent(), FName(),
			OverlappedActor->GetActorLocation(), OverlappedActor->GetActorRotation(), EAttachLocation::KeepWorldPosition, true);

	// Play pickup sound at this actor's location
	if(PickupSound)
		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());

	Super::Destroyed();
}
