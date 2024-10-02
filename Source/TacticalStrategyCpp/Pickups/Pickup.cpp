
#include "Pickup.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


APickup::APickup(): OverlapSphere(nullptr), PickupSound(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	OverlapSphere->SetupAttachment(RootComponent);
	OverlapSphere->SetSphereRadius(150);
	OverlapSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetupAttachment(OverlapSphere);
	PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void APickup::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
		OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnSphereOverlap);	
}

void APickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickup::Destroyed()
{
	Super::Destroyed();
	
	if(PickupSound)
		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
}

