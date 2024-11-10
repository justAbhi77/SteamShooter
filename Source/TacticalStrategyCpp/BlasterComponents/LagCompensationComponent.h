// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LagCompensationComponent.generated.h"

// Struct to hold hitbox position, rotation, and size for a single frame
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

	FBoxInformation() : Location(FVector::ZeroVector), Rotation(FRotator::ZeroRotator), BoxExtent(FVector::ZeroVector)
	{}

	FBoxInformation(const FVector& IniLocation, const FRotator& IniRotation, const FVector& IniBoxExtent) :
		Location(IniLocation), Rotation(IniRotation), BoxExtent(IniBoxExtent)
	{}
};

// Struct to store snapshot data for a single frame, including hitbox data and character reference
USTRUCT(BlueprintType)
struct FFramePackage
{
	GENERATED_BODY()

	UPROPERTY()
	float Time;

	UPROPERTY()
	TMap<FName, FBoxInformation> HitBoxInfo; // Map of hitbox data by hitbox name

	UPROPERTY()
	class ABlasterCharacter* Character;

	FFramePackage() : Time(0), Character(nullptr)
	{}
};

// Struct for the result of a server-side rewind, indicating hit success and headshot status
USTRUCT()
struct FServerSideRewindResult
{
	GENERATED_BODY()

	UPROPERTY()
	bool bHitConfirmed;

	UPROPERTY()
	bool bHeadshot;

	FServerSideRewindResult() : bHitConfirmed(false), bHeadshot(false)
	{}

	FServerSideRewindResult(bool bHitConfirm, bool bIniHeadShot) :
		bHitConfirmed(bHitConfirm), bHeadshot(bIniHeadShot)
	{}
};

// Struct for storing shotgun-specific rewind results, with hit counts for each character
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

	// Allows ABlasterCharacter to access private members
	friend class ABlasterCharacter;

	// Debug function to visually display hitboxes from a frame package
	void ShowFramePackage(const FFramePackage& Package, const FColor& Color) const;

	// Rewinds time on the server to confirm a hit based on past character positions
	FServerSideRewindResult ServerSideRewind(ABlasterCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation, float HitTime);

	// Rewinds time for projectile hits
	FServerSideRewindResult ProjectileServerSideRewind(ABlasterCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime);

	// Requests a server-side score update for hits
	UFUNCTION(Server, Reliable)
	void ServerScoreRequest(ABlasterCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation, float HitTime);

	// Rewinds time to calculate hits for a shotgun spread
	FShotgunServerSideRewindResult ShotgunServerSideRewind(const TArray<ABlasterCharacter*>& HitCharacters, const FVector_NetQuantize& TraceStart, const TArray<FVector_NetQuantize>& HitLocations, float HitTime);

	// Requests a server-side score update for shotgun hits
	UFUNCTION(Server, Reliable)
	void ShotgunServerScoreRequest(const TArray<ABlasterCharacter*>& HitCharacters, const FVector_NetQuantize& TraceStart, const TArray<FVector_NetQuantize>& HitLocations, float HitTime);

	// Requests a server-side score update for projectile hits
	UFUNCTION(Server, Reliable)
	void ProjectileServerScoreRequest(ABlasterCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime);
protected:

	// Saves the current state of hitboxes into a frame package
	void SaveFramePackage(FFramePackage& Package);

	// Interpolates between two frames to get the hitbox positions at a specific time
	FFramePackage InterpBetweenFrames(const FFramePackage& OlderFrame, const FFramePackage& YoungerFrame, float HitTime);

	// Caches hitbox positions for the character into the provided frame package
	void CacheBoxPosition(ABlasterCharacter* HitCharacter, FFramePackage& OutFramePackage);

	// Moves character hitboxes to positions from a frame package
	void MoveBoxes(ABlasterCharacter* HitCharacter, const FFramePackage& Package);

	// Resets hitboxes back to the character’s current position
	void ResetBoxes(ABlasterCharacter* HitCharacter, const FFramePackage& Package);

	// Enables or disables collision for the character’s mesh
	void EnableCharacterMeshCollision(const ABlasterCharacter* HitCharacter, ECollisionEnabled::Type CollisionEnabled);

	// Saves the frame package to the history for later rewinding
	void SaveFramePackage();

	// Retrieves a frame package for hit detection based on the hit time
	FFramePackage GetFrameToCheck(ABlasterCharacter* HitCharacter, float HitTime);

	// Confirms a hit using rewind data, checking if the trace hits any hitbox
	FServerSideRewindResult ConfirmHit(const FFramePackage& Package, ABlasterCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation);

	// Confirms a hit for projectile weapons using rewind data
	FServerSideRewindResult ProjectileConfirmHit(const FFramePackage& Package, ABlasterCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime);

	// Confirms hits for shotgun spread using rewind data from multiple frames
	FShotgunServerSideRewindResult ShotgunConfirmHit(const TArray<FFramePackage>& FramePackages, const FVector_NetQuantize& TraceStart, const TArray<FVector_NetQuantize>& HitLocations);
private:
	UPROPERTY()
	ABlasterCharacter* Character;

	UPROPERTY()
	class ABlasterPlayerController* Controller;

	// Stores historical frame packages for rewind
	TDoubleLinkedList<FFramePackage> FrameHistory;

	// Max time in seconds for frame storage
	UPROPERTY(EditAnywhere)
	float MaxRecordTime = 4;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
