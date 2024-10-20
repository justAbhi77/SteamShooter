// Fill out your copyright notice in the Description page of Project Settings.

#include "LagCompensationComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TacticalStrategyCpp/TacticalStrategyCpp.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"
#include "TacticalStrategyCpp/Weapon/Weapon.h"


ULagCompensationComponent::ULagCompensationComponent():
	Character(nullptr), Controller(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULagCompensationComponent::ShowFramePackage(const FFramePackage& Package, const FColor& Color) const
{
	for(auto& BoxInfo: Package.HitBoxInfo)
	{
		DrawDebugBox(GetWorld(), BoxInfo.Value.Location, BoxInfo.Value.BoxExtent,
			FQuat(BoxInfo.Value.Rotation), Color, false, 4.f);
	}
}

// ReSharper disable once CppMemberFunctionMayBeStatic
FServerSideRewindResult ULagCompensationComponent::ServerSideRewind(ABlasterCharacter* HitCharacter, const FVector_NetQuantize& TraceStart,
                                                 const FVector_NetQuantize& HitLocation, float HitTime)
{
	const FFramePackage FrameToCheck = GetFrameToCheck(HitCharacter, HitTime);
	return ConfirmHit(FrameToCheck, HitCharacter, TraceStart, HitLocation);
}

FServerSideRewindResult ULagCompensationComponent::ProjectileServerSideRewind(ABlasterCharacter* HitCharacter,
	const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime)
{
	const FFramePackage FrameToCheck = GetFrameToCheck(HitCharacter, HitTime);
	return ProjectileConfirmHit(FrameToCheck, HitCharacter, TraceStart, InitialVelocity, HitTime);
}

FShotgunServerSideRewindResult ULagCompensationComponent::ShotgunServerSideRewind(
	const TArray<ABlasterCharacter*>& HitCharacters, const FVector_NetQuantize& TraceStart,
	const TArray<FVector_NetQuantize>& HitLocations, const float HitTime)
{
	TArray<FFramePackage> FramesToCheck;
	for(ABlasterCharacter* HitCharacter : HitCharacters)
	{
		FramesToCheck.Add(GetFrameToCheck(HitCharacter, HitTime));
	}
	return ShotgunConfirmHit(FramesToCheck, TraceStart, HitLocations);
}

void ULagCompensationComponent::ShotgunServerScoreRequest_Implementation(
	const TArray<ABlasterCharacter*>& HitCharacters, const FVector_NetQuantize& TraceStart,
	const TArray<FVector_NetQuantize>& HitLocations, float HitTime)
{
	FShotgunServerSideRewindResult Confirm = ShotgunServerSideRewind(HitCharacters, TraceStart, HitLocations, HitTime);
	for(auto& HitCharacter : HitCharacters)
	{
		if(HitCharacter == nullptr || HitCharacter->GetEquippedWeapon() == nullptr || Character == nullptr) continue;
		float TotalDamage = 0;
		if(Confirm.HeadShots.Contains(HitCharacter))
			TotalDamage += Confirm.HeadShots[HitCharacter] * HitCharacter->GetEquippedWeapon()->GetDamage();
		
		if(Confirm.BodyShots.Contains(HitCharacter))
			TotalDamage += Confirm.HeadShots[HitCharacter] * HitCharacter->GetEquippedWeapon()->GetDamage();

		UGameplayStatics::ApplyDamage(HitCharacter, TotalDamage, Character->Controller,
			HitCharacter->GetEquippedWeapon(), UDamageType::StaticClass());
	}
}

FFramePackage ULagCompensationComponent::GetFrameToCheck(ABlasterCharacter* HitCharacter, const float HitTime)
{	
	FFramePackage FrameToCheck{}; // frame to check for rewind checking
	if(HitCharacter == nullptr) return FrameToCheck;

	const ULagCompensationComponent* LagCompensationComponent = HitCharacter->GetLagCompensation();
	if(LagCompensationComponent == nullptr ||
		LagCompensationComponent->FrameHistory.GetHead() == nullptr ||
		LagCompensationComponent->FrameHistory.GetTail() == nullptr)
		return FrameToCheck;
	// History of hit character 
	const TDoubleLinkedList<FFramePackage>& History = LagCompensationComponent->FrameHistory;

	const float OldestHistoryTime = History.GetTail()->GetValue().Time;

	if(OldestHistoryTime > HitTime) // too far back in time to rewind to
		return FrameToCheck;
	
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

	while (Older->GetValue().Time > HitTime) // is older still younger than hittime
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
	{
		FrameToCheck = InterpBetweenFrames(Older->GetValue(), Younger->GetValue(), HitTime);
	}
	
	FrameToCheck.Character = HitCharacter;
	return FrameToCheck;
}

void ULagCompensationComponent::ServerScoreRequest_Implementation(ABlasterCharacter* HitCharacter,
                                                                  const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation, float HitTime, AWeapon* DamageCauser)
{
	FServerSideRewindResult Confirm = ServerSideRewind(HitCharacter, TraceStart, HitLocation, HitTime);

	if(Character && HitCharacter && DamageCauser && Confirm.bHitConfirmed)
	{
		UGameplayStatics::ApplyDamage(HitCharacter, DamageCauser->GetDamage(),
			Character->Controller, DamageCauser, UDamageType::StaticClass());
	}
}

void ULagCompensationComponent::ProjectileServerScoreRequest_Implementation(ABlasterCharacter* HitCharacter,
	const FVector_NetQuantize& TraceStart, const FVector_NetQuantize100& InitialVelocity, float HitTime)
{
	FServerSideRewindResult Confirm = ProjectileServerSideRewind(HitCharacter, TraceStart, InitialVelocity, HitTime);

	if(Character && HitCharacter && Confirm.bHitConfirmed)
	{
		UGameplayStatics::ApplyDamage(HitCharacter, Character->GetEquippedWeapon()->GetDamage(),
			Character->Controller, Character->GetEquippedWeapon(), UDamageType::StaticClass());
	}
}

void ULagCompensationComponent::BeginPlay()
{
	Super::BeginPlay();
}

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
FFramePackage ULagCompensationComponent::InterpBetweenFrames(const FFramePackage& OlderFrame,
                                                             const FFramePackage& YoungerFrame, float HitTime)
{
	const float Distance = YoungerFrame.Time - OlderFrame.Time;
	const float InterpFraction = FMath::Clamp((HitTime - OlderFrame.Time) / Distance, 0.f, 1.f);

	FFramePackage InterpFramePackage{};
	InterpFramePackage.Time = HitTime;

	for (auto& YoungerPair: YoungerFrame.HitBoxInfo)
	{
		const FName& BoxInfoName = YoungerPair.Key;
		const FBoxInformation& OlderBox = OlderFrame.HitBoxInfo[BoxInfoName];
		const FBoxInformation& YoungerBox = YoungerPair.Value;

		FBoxInformation InterpBoxInfo{};
		InterpBoxInfo.Location = FMath::VInterpTo(OlderBox.Location, YoungerBox.Location, 1, InterpFraction);
		InterpBoxInfo.Rotation = FMath::RInterpTo(OlderBox.Rotation, YoungerBox.Rotation, 1, InterpFraction);
		InterpBoxInfo.BoxExtent = OlderBox.BoxExtent;

		InterpFramePackage.HitBoxInfo.Add(BoxInfoName, InterpBoxInfo);
	}
	
	return InterpFramePackage;
}

FServerSideRewindResult ULagCompensationComponent::ConfirmHit(const FFramePackage& Package,
	ABlasterCharacter* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation)
{
	if(HitCharacter == nullptr) return FServerSideRewindResult();

	FFramePackage CurrentFrame;
	CacheBoxPosition(HitCharacter, CurrentFrame);
	MoveBoxes(HitCharacter, Package);

	EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::NoCollision);

	// check for headshot first
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

		if(ConfirmHitResult.bBlockingHit) // headshot confirmed
		{
			ResetBoxes(HitCharacter, CurrentFrame);
			EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::QueryAndPhysics);
			return FServerSideRewindResult{ true, true };
		}
		// not a headshot
		for(auto& HitBoxPair : HitCharacter->HitCollisionBoxes)
		{
			if(HitBoxPair.Value != nullptr)
			{
				HitBoxPair.Value->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				HitBoxPair.Value->SetCollisionResponseToChannel(ECC_HitBox, ECR_Block);
			}
		}
		World->LineTraceSingleByChannel(ConfirmHitResult, TraceStart, TraceEnd, ECC_HitBox);
		if(ConfirmHitResult.bBlockingHit) // bodyshot confirmed
		{
			ResetBoxes(HitCharacter, CurrentFrame);
			EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::QueryAndPhysics);
			return FServerSideRewindResult{ true, false };
		}
	}
	
	ResetBoxes(HitCharacter, CurrentFrame);
	EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::QueryAndPhysics);
	return FServerSideRewindResult{ false, false };
}

FServerSideRewindResult ULagCompensationComponent::ProjectileConfirmHit(const FFramePackage& Package,
	ABlasterCharacter* HitCharacter, const FVector_NetQuantize& TraceStart,
	const FVector_NetQuantize100& InitialVelocity, float HitTime)
{
	if(HitCharacter == nullptr) return FServerSideRewindResult();

	FFramePackage CurrentFrame;
	CacheBoxPosition(HitCharacter, CurrentFrame);
	MoveBoxes(HitCharacter, Package);

	EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::NoCollision);

	// check for headshot first
	if(UBoxComponent* HeadBox = HitCharacter->HitCollisionBoxes[FName(HitCharacter->HeadBoxBone)])
	{
		HeadBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		HeadBox->SetCollisionResponseToChannel(ECC_HitBox, ECR_Block);
	}

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

	if(PathResult.HitResult.bBlockingHit)
	{
		// headshot
		
		ResetBoxes(HitCharacter, CurrentFrame);
		EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::QueryAndPhysics);
		return FServerSideRewindResult{ true, true };
	}
	{
		// bodyshot
		for(auto& HitBoxPair : HitCharacter->HitCollisionBoxes)
		{
			if(HitBoxPair.Value != nullptr)
			{
				HitBoxPair.Value->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				HitBoxPair.Value->SetCollisionResponseToChannel(ECC_HitBox, ECR_Block);
			}
		}
		UGameplayStatics::PredictProjectilePath(this, PathParams, PathResult);
		if(PathResult.HitResult.bBlockingHit) // bodyshot confirmed
		{
			ResetBoxes(HitCharacter, CurrentFrame);
			EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::QueryAndPhysics);
			return FServerSideRewindResult{ true, false };
		}
	}
	
	ResetBoxes(HitCharacter, CurrentFrame);
	EnableCharacterMeshCollision(HitCharacter, ECollisionEnabled::QueryAndPhysics);
	return FServerSideRewindResult{ false, false };
}

FShotgunServerSideRewindResult ULagCompensationComponent::ShotgunConfirmHit(const TArray<FFramePackage>& FramePackages,
                                                                            const FVector_NetQuantize& TraceStart, const TArray<FVector_NetQuantize>& HitLocations)
{
	FShotgunServerSideRewindResult ShotgunResult;
	for(const auto& Frame : FramePackages)
		if(Frame.Character == nullptr) return ShotgunResult;
	
	TArray<FFramePackage> CurrentFrames;
	for(auto& Frame : FramePackages)
	{
		FFramePackage CurrentFrame;
		CurrentFrame.Character = Frame.Character;
		CacheBoxPosition(Frame.Character, CurrentFrame);
		MoveBoxes(Frame.Character, Frame);
		EnableCharacterMeshCollision(Frame.Character, ECollisionEnabled::NoCollision);

		CurrentFrames.Add(CurrentFrame);
	}
	for(auto& Frame : FramePackages)
	{
		// check for headshot first
		if(UBoxComponent* HeadBox = Frame.Character->HitCollisionBoxes[FName(Frame.Character->HeadBoxBone)])
		{
			HeadBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			HeadBox->SetCollisionResponseToChannel(ECC_HitBox, ECR_Block);
		}
	}
	UWorld* World = GetWorld();
	for(auto& HitLocation :HitLocations)
	{
		const FVector TraceEnd = TraceStart + (HitLocation - TraceStart) * 1.25f;

		if(World)
		{
			FHitResult ConfirmHitResult;
			World->LineTraceSingleByChannel(ConfirmHitResult, TraceStart, TraceEnd, ECC_HitBox);

			if(ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(ConfirmHitResult.GetActor()))
			{
				if(ShotgunResult.HeadShots.Contains(BlasterCharacter))
					ShotgunResult.HeadShots[BlasterCharacter]++;
				else
					ShotgunResult.HeadShots.Emplace(BlasterCharacter, 1);
			}
		}
	}

	// not a headshot
	for(auto& Frame : FramePackages)
	{
		for(auto& HitBoxPair : Frame.Character->HitCollisionBoxes)
		{
			if(HitBoxPair.Value != nullptr)
			{
				HitBoxPair.Value->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				HitBoxPair.Value->SetCollisionResponseToChannel(ECC_HitBox, ECR_Block);
			}
		}
		if(UBoxComponent* HeadBox = Frame.Character->HitCollisionBoxes[FName(Frame.Character->HeadBoxBone)])
			HeadBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	for(auto& HitLocation :HitLocations)
	{
		const FVector TraceEnd = TraceStart + (HitLocation - TraceStart) * 1.25f;

		if(World)
		{
			FHitResult ConfirmHitResult;
			World->LineTraceSingleByChannel(ConfirmHitResult, TraceStart, TraceEnd, ECC_HitBox);

			if(ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(ConfirmHitResult.GetActor()))
			{
				if(ShotgunResult.BodyShots.Contains(BlasterCharacter))
					ShotgunResult.BodyShots[BlasterCharacter]++;
				else
					ShotgunResult.BodyShots.Emplace(BlasterCharacter, 1);
			}
		}
	}

	for(auto& CurrentFrame : CurrentFrames)
	{
		ResetBoxes(CurrentFrame.Character, CurrentFrame);
		EnableCharacterMeshCollision(CurrentFrame.Character, ECollisionEnabled::QueryAndPhysics);
	}
	
	return ShotgunResult;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void ULagCompensationComponent::CacheBoxPosition(ABlasterCharacter* HitCharacter, FFramePackage& OutFramePackage)
{
	if(HitCharacter == nullptr) return;

	for(auto& HitBoxPair : HitCharacter->HitCollisionBoxes)
	{
		if(HitBoxPair.Value != nullptr)
		{
			FBoxInformation BoxInfo{};
			BoxInfo.Location = HitBoxPair.Value->GetComponentLocation();
			BoxInfo.Rotation = HitBoxPair.Value->GetComponentRotation();
			BoxInfo.BoxExtent = HitBoxPair.Value->GetScaledBoxExtent();
			OutFramePackage.HitBoxInfo.Add(HitBoxPair.Key, BoxInfo);
		}
	}
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void ULagCompensationComponent::MoveBoxes(ABlasterCharacter* HitCharacter, const FFramePackage& Package)
{
	if(HitCharacter == nullptr) return;

	for(auto& HitBoxPair : HitCharacter->HitCollisionBoxes)
	{
		if(HitBoxPair.Value != nullptr)
		{
			const auto BoxInformation = Package.HitBoxInfo[HitBoxPair.Key];
			HitBoxPair.Value->SetWorldLocation(BoxInformation.Location);
			HitBoxPair.Value->SetWorldRotation(BoxInformation.Rotation);
			HitBoxPair.Value->SetBoxExtent(BoxInformation.BoxExtent);
		}
	}
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void ULagCompensationComponent::ResetBoxes(ABlasterCharacter* HitCharacter, const FFramePackage& Package)
{	
	if(HitCharacter == nullptr) return;

	for(auto& HitBoxPair : HitCharacter->HitCollisionBoxes)
	{
		if(HitBoxPair.Value != nullptr)
		{
			const auto BoxInformation = Package.HitBoxInfo[HitBoxPair.Key];
			HitBoxPair.Value->SetWorldLocation(BoxInformation.Location);
			HitBoxPair.Value->SetWorldRotation(BoxInformation.Rotation);
			HitBoxPair.Value->SetBoxExtent(BoxInformation.BoxExtent);
			HitBoxPair.Value->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void ULagCompensationComponent::EnableCharacterMeshCollision(const ABlasterCharacter* HitCharacter,
                                                             ECollisionEnabled::Type CollisionEnabled)
{
	if(HitCharacter && HitCharacter->GetMesh())
		HitCharacter->GetMesh()->SetCollisionEnabled(CollisionEnabled);
}

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

		// ShowFramePackage(ThisFrame, FColor::Orange);
	}
}

void ULagCompensationComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SaveFramePackage();
}

