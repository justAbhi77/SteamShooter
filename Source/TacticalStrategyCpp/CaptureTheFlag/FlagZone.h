// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TacticalStrategyCpp/Enums/Team.h"
#include "FlagZone.generated.h"

/**
 * Responsible for defining a zone that captures flags when other team members enter it.
 * This class uses a spherical collision component to detect overlapping actors.
 */
UCLASS()
class TACTICALSTRATEGYCPP_API AFlagZone : public AActor
{
	GENERATED_BODY()

public:
	AFlagZone();

	// Specifies the team associated with this flag zone
	UPROPERTY(EditAnywhere)
	ETeam Team;

protected:
	virtual void BeginPlay() override;

	// Collision component to detect when actors overlap the flag zone
	UPROPERTY(EditAnywhere)
	class USphereComponent* ZoneSphere;

	// Function triggered when another actor overlaps the flag zone
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp,	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
