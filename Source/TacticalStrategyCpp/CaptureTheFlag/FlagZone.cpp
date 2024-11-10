// Fill out your copyright notice in the Description page of Project Settings.


#include "FlagZone.h"
#include "Components/SphereComponent.h"
#include "TacticalStrategyCpp/GameMode/CaptureTheFlagGameMode.h"
#include "TacticalStrategyCpp/Weapon/Flag.h"

AFlagZone::AFlagZone() :
	Team(ETeam::ET_NoTeam),
	ZoneSphere(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;

	ZoneSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ZoneSphere"));
	SetRootComponent(ZoneSphere);
	ZoneSphere->SetCollisionResponseToChannels(ECR_Overlap);
	ZoneSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AFlagZone::BeginPlay()
{
	Super::BeginPlay();

	ZoneSphere->OnComponentBeginOverlap.AddDynamic(this, &AFlagZone::OnSphereOverlap);
}

// Checks if the overlapping actor is a flag of the opposing team. If so, calls the game mode’s function to capture the flag.
void AFlagZone::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFlag* OverlappingFlag = Cast<AFlag>(OtherActor);
	if(OverlappingFlag && OverlappingFlag->GetTeam() != Team)
	{
		if(ACaptureTheFlagGameMode* GameMode = GetWorld()->GetAuthGameMode<ACaptureTheFlagGameMode>())
			GameMode->FlagCaptured(OverlappingFlag, this);

		// Reset the flag's state after capture
		OverlappingFlag->ResetFlag();
	}
}
