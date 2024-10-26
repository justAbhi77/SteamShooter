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

	FBoxInformation():
		Location(FVector::ZeroVector),
		Rotation(FRotator::ZeroRotator),
		BoxExtent(FVector::ZeroVector)
	{
	}
	FBoxInformation(const FVector& IniLocation, const FRotator& IniRotation, const FVector& IniBoxExtent)
	{
		Location = IniLocation;
		Rotation = IniRotation;
		BoxExtent = IniBoxExtent;
	}
};

USTRUCT(BlueprintType)
struct FFramePackage
{
	GENERATED_BODY()

	UPROPERTY()
	float Time;

	UPROPERTY()
	TMap<FName, FBoxInformation> HitBoxInfo;

	UPROPERTY()
	class ABlasterCharacter* Character;

	FFramePackage(): 
		Time(0),
		Character(nullptr)
	{
		HitBoxInfo = TMap<FName, FBoxInformation>();
	}
};

USTRUCT()
struct FServerSideRewindResult
{
	GENERATED_BODY()

	UPROPERTY()
	bool bHitConfirmed;

	UPROPERTY()
	bool bHeadshot;

	FServerSideRewindResult():
		bHitConfirmed(false),
		bHeadshot(false)
	{
	}
	
	FServerSideRewindResult(bool bHitConfirm, bool bIniHeadShot)
	{		
		bHitConfirmed = bHitConfirm;
		bHeadshot = bIniHeadShot;
	}
};

USTRUCT()
struct FShotgunServerSideRewindResult
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<ABlasterCharacter*, uint32> HeadShots;
	
	UPROPERTY()
	TMap<ABlasterCharacter*, uint32> BodyShots;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TACTICALSTRATEGYCPP_API ULagCompensationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULagCompensationComponent();

	friend class ABlasterCharacter;

	void ShowFramePackage(const FFramePackage& Package, const FColor& Color) const;

	FServerSideRewindResult ServerSideRewind(ABlasterCharacter* HitCharacter, const FVector_NetQuantize& TraceStart,
	                      const FVector_NetQuantize& HitLocation, float HitTime);

	FServerSideRewindResult ProjectileServerSideRewind(ABlasterCharacter* HitCharacter, const FVector_NetQuantize& TraceStart,
						  const FVector_NetQuantize100& InitialVelocity, float HitTime);
	
	UFUNCTION(Server, Reliable)
	void ServerScoreRequest(ABlasterCharacter* HitCharacter, const FVector_NetQuantize& TraceStart,
						  const FVector_NetQuantize& HitLocation, float HitTime);

	FShotgunServerSideRewindResult ShotgunServerSideRewind(const TArray<ABlasterCharacter*>& HitCharacters,
		const FVector_NetQuantize& TraceStart, const TArray<FVector_NetQuantize>& HitLocations, float HitTime);
	
	UFUNCTION(Server, Reliable)
	void ShotgunServerScoreRequest(const TArray<ABlasterCharacter*>& HitCharacters,
		const FVector_NetQuantize& TraceStart, const TArray<FVector_NetQuantize>& HitLocations, float HitTime);

	UFUNCTION(Server, Reliable)
	void ProjectileServerScoreRequest(ABlasterCharacter* HitCharacter, const FVector_NetQuantize& TraceStart,
						  const FVector_NetQuantize100& InitialVelocity, float HitTime);
protected:
	virtual void BeginPlay() override;

	void SaveFramePackage(FFramePackage& Package);

	FFramePackage InterpBetweenFrames(const FFramePackage& OlderFrame, const FFramePackage& YoungerFrame, float HitTime);

	void CacheBoxPosition(ABlasterCharacter* HitCharacter, FFramePackage& OutFramePackage);

	void MoveBoxes(ABlasterCharacter* HitCharacter, const FFramePackage& Package);
	void ResetBoxes(ABlasterCharacter* HitCharacter, const FFramePackage& Package);

	void EnableCharacterMeshCollision(const ABlasterCharacter* HitCharacter, ECollisionEnabled::Type CollisionEnabled);
	void SaveFramePackage();

	FFramePackage GetFrameToCheck(ABlasterCharacter* HitCharacter, float HitTime);

	FServerSideRewindResult ConfirmHit(const FFramePackage& Package, ABlasterCharacter* HitCharacter,
		const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation);
	
	FServerSideRewindResult ProjectileConfirmHit(const FFramePackage& Package, ABlasterCharacter* HitCharacter,
		const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime);
	
	FShotgunServerSideRewindResult ShotgunConfirmHit(const TArray<FFramePackage>& FramePackages,
		const FVector_NetQuantize& TraceStart, const TArray<FVector_NetQuantize>& HitLocations);
private:
	UPROPERTY()
	ABlasterCharacter* Character;

	UPROPERTY()
	class ABlasterPlayerController* Controller;

	TDoubleLinkedList<FFramePackage> FrameHistory;

	UPROPERTY(EditAnywhere)
	float MaxRecordTime = 4;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
