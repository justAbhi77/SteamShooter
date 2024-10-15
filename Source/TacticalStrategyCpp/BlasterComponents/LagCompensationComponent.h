// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LagCompensationComponent.generated.h"

USTRUCT()
struct FBoxInformation
{
	GENERATED_BODY()

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FRotator Rotation;

	UPROPERTY()
	FVector BoxExtent;
};

USTRUCT(BlueprintType)
struct FFramePackage
{
	GENERATED_BODY()

	UPROPERTY()
	float Time;

	UPROPERTY()
	TMap<FName, FBoxInformation> HitBoxInfo;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TACTICALSTRATEGYCPP_API ULagCompensationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULagCompensationComponent();

	friend class ABlasterCharacter;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	ABlasterCharacter* Character;

	UPROPERTY()
	class ABlasterPlayerController* Controller;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
