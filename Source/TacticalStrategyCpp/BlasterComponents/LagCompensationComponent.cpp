// Fill out your copyright notice in the Description page of Project Settings.

#include "LagCompensationComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TacticalStrategyCpp/TacticalStrategyCpp.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"
#include "TacticalStrategyCpp/Weapon/Weapon.h"


ULagCompensationComponent::ULagCompensationComponent() :
	Character(nullptr), Controller(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Debug function to visualize a frame's hitbox data in the world
void ULagCompensationComponent::ShowFramePackage(const FFramePackage& Package, const FColor& Color) const
{
	for(const auto& BoxInfo : Package.HitBoxInfo)
		DrawDebugBox(GetWorld(), BoxInfo.Value.Location, BoxInfo.Value.BoxExtent, FQuat(BoxInfo.Value.Rotation), Color, false, 4.f);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// Performs lag compensation for a hitscan weapon based on server-side rewind
FServerSideRewindResult ULagCompensationComponent::ServerSideRewind(ABlasterCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation, float HitTime)
{
	const FFramePackage FrameToCheck = GetFrameToCheck(HitCharacter, HitTime);
	return ConfirmHit(FrameToCheck, HitCharacter, TraceStart, HitLocation);
}

// Performs lag compensation for a projectile weapon
FServerSideRewindResult ULagCompensationComponent::ProjectileServerSideRewind(ABlasterCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime)
{
	const FFramePackage FrameToCheck = GetFrameToCheck(HitCharacter, HitTime);
	return ProjectileConfirmHit(FrameToCheck, HitCharacter, TraceStart, InitialVelocity, HitTime);
}

// Rewind and confirm hit for multiple targets in a shotgun spread
FShotgunServerSideRewindResult ULagCompensationComponent::ShotgunServerSideRewind(
	const TArray<ABlasterCharacter*>& HitCharacters, const FVector_NetQuantize& TraceStart,
	const TArray<FVector_NetQuantize>& HitLocations, const float HitTime)
{
	TArray<FFramePackage> FramesToCheck;
	for(ABlasterCharacter* HitCharacter : HitCharacters)
		FramesToCheck.Add(GetFrameToCheck(HitCharacter, HitTime));
	
	return ShotgunConfirmHit(FramesToCheck, TraceStart, HitLocations);
}

// Server request to confirm a shotgun hit and apply damage
void ULagCompensationComponent::ShotgunServerScoreRequest_Implementation(
	const TArray<ABlasterCharacter*>& HitCharacters, const FVector_NetQuantize& TraceStart,
	const TArray<FVector_NetQuantize>& HitLocations, float HitTime)
{
	FShotgunServerSideRewindResult Confirm = ShotgunServerSideRewind(HitCharacters, TraceStart, HitLocations, HitTime);

	for(auto& HitCharacter : HitCharacters)
	{
		if(HitCharacter == nullptr || HitCharacter->GetEquippedWeapon() == nullptr || Character == nullptr) continue;
		float TotalDamage = 0;

		// Calculate headshot and body shot damage
		if(Confirm.HeadShots.Contains(HitCharacter))
			TotalDamage += Confirm.HeadShots[HitCharacter] * HitCharacter->GetEquippedWeapon()->GetHeadshotDamage();
		if(Confirm.BodyShots.Contains(HitCharacter))
			TotalDamage += Confirm.BodyShots[HitCharacter] * HitCharacter->GetEquippedWeapon()->GetDamage();

		UGameplayStatics::ApplyDamage(HitCharacter, TotalDamage, Character->Controller, HitCharacter->GetEquippedWeapon(), UDamageType::StaticClass());
	}
}

// Retrieves the frame closest to the specified hit time for rewind
FFramePackage ULagCompensationComponent::GetFrameToCheck(ABlasterCharacter* HitCharacter, const float HitTime)
{
	FFramePackage FrameToCheck;
	if(HitCharacter == nullptr) return FrameToCheck;

	const ULagCompensationComponent* LagCompensationComponent = HitCharacter->GetLagCompensation();
	if(LagCompensationComponent == nullptr ||
		LagCompensationComponent->FrameHistory.GetHead() == nullptr ||
		LagCompensationComponent->FrameHistory.GetTail() == nullptr)
		return FrameToCheck;

	const TDoubleLinkedList<FFramePackage>& History = LagCompensationComponent->FrameHistory;

	const float OldestHistoryTime = History.GetTail()->GetValue().Time;

	// too far back in time to rewind to
	if(OldestHistoryTime > HitTime) return FrameToCheck;

	bool bShouldInterpolate = true;
	if(OldestHistoryTime == HitTime)
	{
		FrameToCheck = History.GetTail()->GetValue();
		bShouldInterpolate = false;
	}

	const float NewestHistoryTime = History.GetHead()->GetValue().Time;
	if(NewestHistoryTime <= HitTime)
	{
		FrameToCheck = History.GetHead()->GetValue();
		bShouldInterpolate = false;
	}

	auto Younger = History.GetHead();
	auto Older = Younger;

	while (Older->GetValue().Time > HitTime)
	{
		if(Older->GetNextNode() == nullptr) break;
		Older = Older->GetNextNode();
		if(Older->GetValue().Time > HitTime)
			Younger = Older;
	}
	if(Older->GetValue().Time == HitTime)
	{
		FrameToCheck = Older->GetValue();
		bShouldInterpolate = false;
	}

	if(bShouldInterpolate)
		FrameToCheck = InterpBetweenFrames(Older->GetValue(), Younger->GetValue(), HitTime);
	FrameToCheck.Character = HitCharacter;
	return FrameToCheck;
}

// Helper function to save a frame package of hitbox data
void ULagCompensationComponent::SaveFramePackage(FFramePackage& Package)
{
	Character = Character == nullptr ? Cast<ABlasterCharacter>(GetOwner()) : Character;
	if(Character)
	{
		Package.Time = GetWorld()->GetTimeSeconds();
		Package.Character = Character;

		for(auto& BoxPair : Character->HitCollisionBoxes)
		{
			FBoxInformation BoxInformation{};
			const UBoxComponent* BoxComponent = BoxPair.Value;
			BoxInformation.Location = BoxComponent->GetComponentLocation();
			BoxInformation.Rotation = BoxComponent->GetComponentRotation();
			BoxInformation.BoxExtent = BoxComponent->GetScaledBoxExtent();

			Package.HitBoxInfo.Add(BoxPair.Key, BoxInformation);
		}
	}
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// Linearly interpolates between two frames to get the hitbox positions at a specific time
FFramePackage ULagCompensationComponent::InterpBetweenFrames(const FFramePackage& OlderFrame, const FFramePackage& YoungerFrame, float HitTime)
{
	const float Distance = YoungerFrame.Time - OlderFrame.Time;
	const float InterpFraction = FMath::Clamp((HitTime - OlderFrame.Time) / Distance, 0.f, 1.f);

	FFramePackage InterpFramePackage;
	InterpFramePackage.Time = HitTime;

	for (const auto& YoungerPair : YoungerFrame.HitBoxInfo)
	{
		const FName& BoxInfoName = YoungerPair.Key;
		const FBoxInformation& OlderBox = OlderFrame.HitBoxInfo[BoxInfoName];
		const FBoxInformation& YoungerBox = YoungerPair.Value;

		FBoxInformation InterpBoxInfo;
		InterpBoxInfo.Location = FMath::VInterpTo(OlderBox.Location, YoungerBox.Location, 1, InterpFraction);
		InterpBoxInfo.Rotation = FMath::RInterpTo(OlderBox.Rotation, YoungerBox.Rotation, 1, InterpFraction);
		InterpBoxInfo.BoxExtent = OlderBox.BoxExtent;

		InterpFramePackage.HitBoxInfo.Add(BoxInfoName, InterpBoxInfo);
	}
	return InterpFramePackage;
}

// Performs hit confirmation by tracing to determine headshot or body shot
FServerSideRewindResult ULagCompensationComponent::ConfirmHit(const FFramePackage& Package, ABlasterCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation)
{
	if(HitCharacter == nullptr) return FServerSideRewindResult();

	FFramePackage CurrentFrame;
	CacheBoxPosition(HitCharacter, CurrentFrame);
	MoveBoxes(HitCharacter, Package);

	EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::NoCollision);

	// Headshot check
	if(UBoxComponent* HeadBox = HitCharacter->HitCollisionBoxes[FName(HitCharacter->HeadBoxBone)])
	{
		HeadBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		HeadBox->SetCollisionResponseToChannel(ECC_HitBox, ECR_Block);
	}
	const FVector TraceEnd = TraceStart + (HitLocation - TraceStart) * 1.25f;

	if(UWorld* World = GetWorld())
	{
		FHitResult ConfirmHitResult;
		World->LineTraceSingleByChannel(ConfirmHitResult, TraceStart, TraceEnd, ECC_HitBox);

		// Headshot
		if(ConfirmHitResult.bBlockingHit)
		{
			ResetBoxes(HitCharacter, CurrentFrame);
			EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::QueryAndPhysics);
			return FServerSideRewindResult{ true, true };
		}
		// Body shot
		for(auto& HitBoxPair : HitCharacter->HitCollisionBoxes)
		{
			if(HitBoxPair.Value)
			{
				HitBoxPair.Value->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				HitBoxPair.Value->SetCollisionResponseToChannel(ECC_HitBox, ECR_Block);
			}
		}
		World->LineTraceSingleByChannel(ConfirmHitResult, TraceStart, TraceEnd, ECC_HitBox);

		if(ConfirmHitResult.bBlockingHit)
		{
			ResetBoxes(HitCharacter, CurrentFrame);
			EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::QueryAndPhysics);
			return FServerSideRewindResult{ true, false };
		}
	}

	// No hit
	ResetBoxes(HitCharacter, CurrentFrame);
	EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::QueryAndPhysics);
	return FServerSideRewindResult{ false, false };
}

// Save frames over time for lag compensation
void ULagCompensationComponent::SaveFramePackage()
{
	if(Character == nullptr || !Character->HasAuthority()) return;

	if(FrameHistory.Num() <= 1)
	{
		FFramePackage FramePackage;
		SaveFramePackage(FramePackage);
		FrameHistory.AddHead(FramePackage);
	}
	else
	{
		float HistoryLength = FrameHistory.GetHead()->GetValue().Time - FrameHistory.GetTail()->GetValue().Time;
		while(HistoryLength > MaxRecordTime)
		{
			FrameHistory.RemoveNode(FrameHistory.GetTail());
			HistoryLength = FrameHistory.GetHead()->GetValue().Time - FrameHistory.GetTail()->GetValue().Time;
		}
		FFramePackage ThisFrame;
		SaveFramePackage(ThisFrame);
		FrameHistory.AddHead(ThisFrame);
	}
}

void ULagCompensationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SaveFramePackage();
}

// Sends score request to server, performs a server-side rewind to verify hitscan hit accuracy
void ULagCompensationComponent::ServerScoreRequest_Implementation(ABlasterCharacter* HitCharacter,
	const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation, float HitTime)
{
	FServerSideRewindResult Confirm = ServerSideRewind(HitCharacter, TraceStart, HitLocation, HitTime);

	if(Character && HitCharacter && Character->GetEquippedWeapon() && Confirm.bHitConfirmed)
	{
		const float Damage = Confirm.bHeadshot ? Character->GetEquippedWeapon()->GetHeadshotDamage() :
			Character->GetEquippedWeapon()->GetDamage();
		
		UGameplayStatics::ApplyDamage(HitCharacter, Damage,
			Character->Controller, Character->GetEquippedWeapon(), UDamageType::StaticClass());
	}
}

// Processes a score request for projectile weapons with server-side rewind for hit accuracy
void ULagCompensationComponent::ProjectileServerScoreRequest_Implementation(ABlasterCharacter* HitCharacter,
	const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime)
{
	FServerSideRewindResult Confirm = ProjectileServerSideRewind(HitCharacter, TraceStart, InitialVelocity, HitTime);

	if(Character && Character->GetEquippedWeapon() && HitCharacter && Confirm.bHitConfirmed)
	{
		const float Damage = Confirm.bHeadshot ? Character->GetEquippedWeapon()->GetHeadshotDamage() :
			Character->GetEquippedWeapon()->GetDamage();
		
		UGameplayStatics::ApplyDamage(HitCharacter, Damage,
			Character->Controller, Character->GetEquippedWeapon(), UDamageType::StaticClass());
	}
}

// Predicts and confirms a projectile hit by rewinding to the target frame
FServerSideRewindResult ULagCompensationComponent::ProjectileConfirmHit(const FFramePackage& Package,
	ABlasterCharacter* HitCharacter, const FVector_NetQuantize& TraceStart,
	const FVector_NetQuantize100& InitialVelocity, float HitTime)
{
	if(HitCharacter == nullptr) return FServerSideRewindResult();

	FFramePackage CurrentFrame;
	CacheBoxPosition(HitCharacter, CurrentFrame);
	MoveBoxes(HitCharacter, Package);

	EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::NoCollision);

	// Perform headshot check first
	if(UBoxComponent* HeadBox = HitCharacter->HitCollisionBoxes[FName(HitCharacter->HeadBoxBone)])
	{
		HeadBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		HeadBox->SetCollisionResponseToChannel(ECC_HitBox, ECR_Block);
	}

	// Set up projectile path prediction
	FPredictProjectilePathParams PathParams;
	PathParams.bTraceWithCollision = true;
	PathParams.MaxSimTime = MaxRecordTime;
	PathParams.LaunchVelocity = InitialVelocity;
	PathParams.StartLocation = TraceStart;
	PathParams.SimFrequency = 15.f;
	PathParams.ProjectileRadius = 5;
	PathParams.TraceChannel = ECC_HitBox;
	PathParams.ActorsToIgnore.Add(GetOwner());
	PathParams.DrawDebugTime = 5;
	PathParams.DrawDebugType = EDrawDebugTrace::ForDuration;

	FPredictProjectilePathResult PathResult;
	UGameplayStatics::PredictProjectilePath(this, PathParams, PathResult);

	// Headshot confirmation
	if(PathResult.HitResult.bBlockingHit)
	{
		ResetBoxes(HitCharacter, CurrentFrame);
		EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::QueryAndPhysics);
		return FServerSideRewindResult{ true, true };
	}
	// Check for body shot
	for(auto& HitBoxPair : HitCharacter->HitCollisionBoxes)
	{
		if(HitBoxPair.Value)
		{
			HitBoxPair.Value->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			HitBoxPair.Value->SetCollisionResponseToChannel(ECC_HitBox, ECR_Block);
		}
	}
	UGameplayStatics::PredictProjectilePath(this, PathParams, PathResult);

	// Body shot confirmation
	if(PathResult.HitResult.bBlockingHit)
	{
		ResetBoxes(HitCharacter, CurrentFrame);
		EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::QueryAndPhysics);
		return FServerSideRewindResult{ true, false };
	}

	// No hit confirmed
	ResetBoxes(HitCharacter, CurrentFrame);
	EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::QueryAndPhysics);
	return FServerSideRewindResult{ false, false };
}

// Processes shotgun hits across multiple targets and calculates head/body shot counts
FShotgunServerSideRewindResult ULagCompensationComponent::ShotgunConfirmHit(const TArray<FFramePackage>& FramePackages, const FVector_NetQuantize& TraceStart, const TArray<FVector_NetQuantize>& HitLocations)
{
	FShotgunServerSideRewindResult ShotgunResult;
	UWorld* World = GetWorld();
	TArray<FFramePackage> CurrentFrames;

	for(const auto& Frame : FramePackages)
	{
		if(Frame.Character == nullptr) return ShotgunResult;

		FFramePackage CurrentFrame;
		CurrentFrame.Character = Frame.Character;
		CacheBoxPosition(Frame.Character, CurrentFrame);
		MoveBoxes(Frame.Character, Frame);
		EnableCharacterMeshCollision(Frame.Character, ECollisionEnabled::NoCollision);

		CurrentFrames.Add(CurrentFrame);
		
		// Enable headshot hitbox for headshot detection
		if(UBoxComponent* HeadBox = Frame.Character->HitCollisionBoxes[FName(Frame.Character->HeadBoxBone)])
		{
			HeadBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			HeadBox->SetCollisionResponseToChannel(ECC_HitBox, ECR_Block);
		}
	}

	// Trace for each hit location to confirm headshots
	for(auto& HitLocation : HitLocations)
	{
		const FVector TraceEnd = TraceStart + (HitLocation - TraceStart) * 1.25f;

		if(World)
		{
			FHitResult ConfirmHitResult;
			World->LineTraceSingleByChannel(ConfirmHitResult, TraceStart, TraceEnd, ECC_HitBox);

			if(ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(ConfirmHitResult.GetActor()))
				ShotgunResult.HeadShots.FindOrAdd(BlasterCharacter)++;
		}
	}

	// Enable all hitboxes for body shot detection
	for(const auto& Frame : FramePackages)
	{
		for(auto& HitBoxPair : Frame.Character->HitCollisionBoxes)
		{
			if(HitBoxPair.Value)
			{
				HitBoxPair.Value->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				HitBoxPair.Value->SetCollisionResponseToChannel(ECC_HitBox, ECR_Block);
			}
		}
		if(UBoxComponent* HeadBox = Frame.Character->HitCollisionBoxes[FName(Frame.Character->HeadBoxBone)])
			HeadBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Trace again for each hit location to confirm body shots
	for(auto& HitLocation : HitLocations)
	{
		const FVector TraceEnd = TraceStart + (HitLocation - TraceStart) * 1.25f;

		if(World)
		{
			FHitResult ConfirmHitResult;
			World->LineTraceSingleByChannel(ConfirmHitResult, TraceStart, TraceEnd, ECC_HitBox);

			if(ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(ConfirmHitResult.GetActor()))
					ShotgunResult.BodyShots.FindOrAdd(BlasterCharacter)++;
		}
	}

	// Reset all boxes to their original state
	for(auto& CurrentFrame : CurrentFrames)
	{
		ResetBoxes(CurrentFrame.Character, CurrentFrame);
		EnableCharacterMeshCollision(CurrentFrame.Character, ECollisionEnabled::QueryAndPhysics);
	}

	return ShotgunResult;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// Caches current hitbox positions for the given character
void ULagCompensationComponent::CacheBoxPosition(ABlasterCharacter* HitCharacter, FFramePackage& OutFramePackage)
{
	if(HitCharacter == nullptr) return;

	for(const auto& HitBoxPair : HitCharacter->HitCollisionBoxes)
	{
		if(HitBoxPair.Value)
		{
			FBoxInformation BoxInfo;
			BoxInfo.Location = HitBoxPair.Value->GetComponentLocation();
			BoxInfo.Rotation = HitBoxPair.Value->GetComponentRotation();
			BoxInfo.BoxExtent = HitBoxPair.Value->GetScaledBoxExtent();
			OutFramePackage.HitBoxInfo.Add(HitBoxPair.Key, BoxInfo);
		}
	}
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// Moves hitboxes to recorded positions from a specific frame for hit detection
void ULagCompensationComponent::MoveBoxes(ABlasterCharacter* HitCharacter, const FFramePackage& Package)
{
	if(HitCharacter == nullptr) return;

	for(const auto& HitBoxPair : HitCharacter->HitCollisionBoxes)
	{
		if(HitBoxPair.Value)
		{
			const auto& BoxInformation = Package.HitBoxInfo[HitBoxPair.Key];
			HitBoxPair.Value->SetWorldLocation(BoxInformation.Location);
			HitBoxPair.Value->SetWorldRotation(BoxInformation.Rotation);
			HitBoxPair.Value->SetBoxExtent(BoxInformation.BoxExtent);
		}
	}
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// Resets hitboxes to their original positions after hit detection
void ULagCompensationComponent::ResetBoxes(ABlasterCharacter* HitCharacter, const FFramePackage& Package)
{	
	if(HitCharacter == nullptr) return;

	for(const auto& HitBoxPair : HitCharacter->HitCollisionBoxes)
	{
		if(HitBoxPair.Value)
		{
			const auto& BoxInformation = Package.HitBoxInfo[HitBoxPair.Key];
			HitBoxPair.Value->SetWorldLocation(BoxInformation.Location);
			HitBoxPair.Value->SetWorldRotation(BoxInformation.Rotation);
			HitBoxPair.Value->SetBoxExtent(BoxInformation.BoxExtent);
			HitBoxPair.Value->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// Enables or disables the collision for the character's mesh
void ULagCompensationComponent::EnableCharacterMeshCollision(const ABlasterCharacter* HitCharacter, ECollisionEnabled::Type CollisionEnabled)
{
	if(HitCharacter && HitCharacter->GetMesh())
		HitCharacter->GetMesh()->SetCollisionEnabled(CollisionEnabled);
}
