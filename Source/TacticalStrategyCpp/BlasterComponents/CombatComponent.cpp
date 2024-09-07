
#include "CombatComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"
#include "TacticalStrategyCpp/Hud/BlasterHud.h"
#include "TacticalStrategyCpp/PlayerController/BlasterPlayerController.h"
#include "TacticalStrategyCpp/Weapon/Weapon.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent():
	RightHandSocketName(FString("RightHandSocket")),
	Character(nullptr),
	Controller(nullptr),
	Hud(nullptr),
	EquippedWeapon(nullptr),
	bAiming(false),
	bFireButtonPressed(false),
	CrosshairBaseLineSpread(0.5f),
	CrosshairVelocityFactor(0),
	CrosshairInAirFactor(0),
	CrosshairAimFactor(0),
	CrosshairShootingFactor(0),
	DefaultFov(0),
	ZoomedFov(30.f),
	ZoomInterpSpeed(20.f),
	CurrentFov(0),
	HUDPackage(),
	bCanFire(true),
	CarriedAmmo(0)
{
	PrimaryComponentTick.bCanEverTick = true;

	BaseWalkSpeed = 600.f;
	AimWalkSpeed = 450.f;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if(Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

		if(const UCameraComponent* CameraComponent = Character->GetFollowCamera())
		{
			DefaultFov = CameraComponent->FieldOfView;
			CurrentFov = DefaultFov;
		}
	}

}

void UCombatComponent::SetAiming(const bool bIsAiming)
{
	bAiming = bIsAiming;

	if(Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming? AimWalkSpeed : BaseWalkSpeed;
		
		if(!Character->HasAuthority())
			ServerSetAiming(bIsAiming);
	}
}

void UCombatComponent::OnRep_EquippedWeapon() const
{
	if(EquippedWeapon && Character)
	{		
		EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped, true);

		USkeletalMeshComponent* SkeletalMeshComponent = Character->GetMesh();
		if(const USkeletalMeshSocket* HandSocket = SkeletalMeshComponent->GetSocketByName(FName(RightHandSocketName)))
		{
			HandSocket->AttachActor(EquippedWeapon, SkeletalMeshComponent);
		}		
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		Character->bUseControllerRotationYaw = true;		
	}
}

void UCombatComponent::Fire()
{
	if(CanFire())
	{
		Server_Fire(HitTarget);

		if(EquippedWeapon)
		{
			CrosshairShootingFactor = 0.75f;
		}
	
		StartFireTimer();
	}
}

void UCombatComponent::FireButtonPressed(const bool bPressed)
{
	bFireButtonPressed = bPressed;

	if(bFireButtonPressed)
	{
		Fire();
	}
}

void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	if(GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	const FVector2D CrosshairLocation = FVector2D(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition, CrosshairWorldDirection;
	
	// ReSharper disable once CppTooWideScope
	const bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),CrosshairLocation,
		CrosshairWorldPosition, CrosshairWorldDirection);

	if(bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;

		if(Character)
		{
			const float DistanceToCharacter = (Character->GetActorLocation() - Start).Size();
			Start += CrosshairWorldDirection * (DistanceToCharacter + 100.f);			
		}
		
		const FVector End = Start + CrosshairWorldDirection * TRACE_LENGTH;

		// ReSharper disable once CppTooWideScopeInitStatement
		const bool bLineTraceSingleByChannel = GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End,
			ECC_Visibility);
		
		if(!bLineTraceSingleByChannel)
			TraceHitResult.ImpactPoint = End;

		if(const AActor* Actor = TraceHitResult.GetActor(); Actor && Actor->Implements<UInteractWithCrosshairsInterface>())
		{
			HUDPackage.CrosshairsColor = FLinearColor::Red;
		}
		else
		{
			HUDPackage.CrosshairsColor = FLinearColor::White;			
		}
	}
}

void UCombatComponent::SetHudCrosshairs(const float DeltaTime)
{
	if(Character == nullptr || Character->Controller == nullptr) return;

	Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) :
		Controller;

	if(Controller)
	{
		Hud = Hud == nullptr ? Cast<ABlasterHud>(Controller->GetHUD()) : Hud;
		if(Hud)
		{
			if(EquippedWeapon)
			{
				HUDPackage.CrosshairsCenter = EquippedWeapon->CrosshairsCenter;
				HUDPackage.CrosshairsLeft = EquippedWeapon->CrosshairsLeft;
				HUDPackage.CrosshairsRight = EquippedWeapon->CrosshairsRight;
				HUDPackage.CrosshairsTop = EquippedWeapon->CrosshairsTop;
				HUDPackage.CrosshairsBottom = EquippedWeapon->CrosshairsBottom;
			}
			else
			{
				HUDPackage.CrosshairsCenter = nullptr;
				HUDPackage.CrosshairsLeft = nullptr;
				HUDPackage.CrosshairsRight = nullptr;
				HUDPackage.CrosshairsTop = nullptr;
				HUDPackage.CrosshairsBottom = nullptr;
			}

			// Calculate Crosshair spread

			// map speed from 0 to 600 to 0 to 1

			const FVector2D WalkSpeedRange(0.f, Character->GetCharacterMovement()->MaxWalkSpeed),
				VelocityMultiplierRange(0.f, 1.f);
			FVector Velocity = Character->GetVelocity();
			Velocity.Z = 0;

			CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange,
				VelocityMultiplierRange, Velocity.Size());

			if(Character->GetCharacterMovement()->IsFalling())
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 2.25f, DeltaTime, 2.25);
			}
			else
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.f, DeltaTime, 30);			
			}
			if(bAiming)
			{
				// for aiming to be in sync we can use the equipped weapon zoom interp speed
				/*
				 *if(EquippedWeapon)
					EquippedWeapon->GetZoomInterpSpeed()
				*/
				CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.58f, DeltaTime, 30); 
			}
			else
			{
				CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0, DeltaTime, 30);				
			}
			
			HUDPackage.CrosshairSpread = CrosshairBaseLineSpread +
				CrosshairVelocityFactor + CrosshairInAirFactor + CrosshairShootingFactor -
					CrosshairAimFactor;			

			CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0, DeltaTime, 40.f);

			Hud->SetHudPackage(HUDPackage);
		}
	}
}

void UCombatComponent::InterFov(const float DeltaTime)
{
	if(EquippedWeapon == nullptr) return;

	if(bAiming)
	{
		CurrentFov = FMath::FInterpTo(CurrentFov, EquippedWeapon->GetZoomedFov(), DeltaTime,
			EquippedWeapon->GetZoomInterpSpeed());		
	}
	else
	{
		CurrentFov = FMath::FInterpTo(CurrentFov, DefaultFov, DeltaTime,
			ZoomInterpSpeed);		
	}

	if(Character)
	{
		if(UCameraComponent* CameraComponent = Character->GetFollowCamera())
		{
			CameraComponent->SetFieldOfView(CurrentFov);
		}
	}
}

void UCombatComponent::StartFireTimer()
{
	if(EquippedWeapon == nullptr || Character == nullptr) return;
	Character->GetWorldTimerManager().SetTimer(FireTimer, this, &UCombatComponent::FireTimerFinished,
		EquippedWeapon->FireDelay);
}

void UCombatComponent::FireTimerFinished()
{
	if(EquippedWeapon == nullptr) return;
	bCanFire = true;
	if(bFireButtonPressed && EquippedWeapon->bIsAutomatic)
	{
		Fire();
	}
}

bool UCombatComponent::CanFire() const
{
	if(EquippedWeapon == nullptr) return false;
	return !EquippedWeapon->IsEmpty() || !bCanFire;
}

void UCombatComponent::OnRep_CarriedAmmo()
{
}

void UCombatComponent::Server_Fire_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	Multicast_Fire(TraceHitTarget);
}

void UCombatComponent::Multicast_Fire_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	if(EquippedWeapon == nullptr) return;
	
	if(Character)
	{
		Character->PlayFireMontage(bAiming);
		EquippedWeapon->Fire(TraceHitTarget);
	}
}

void UCombatComponent::ServerSetAiming_Implementation(const bool bIsAiming)
{
	SetAiming(bIsAiming);
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(Character && Character->IsLocallyControlled())
	{
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		HitTarget = HitResult.ImpactPoint;

		SetHudCrosshairs(DeltaTime);

		InterFov(DeltaTime);
	}
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, EquippedWeapon);
	
	DOREPLIFETIME(UCombatComponent, bAiming);
	
	DOREPLIFETIME_CONDITION(UCombatComponent, CarriedAmmo, COND_OwnerOnly);
}

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if(Character == nullptr || WeaponToEquip == nullptr) return;

	if(EquippedWeapon)
		EquippedWeapon->Dropped();

	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetOwner(Character);
	
	if(Character->HasAuthority())
	{
		OnRep_EquippedWeapon();
		if(EquippedWeapon)
			EquippedWeapon->OnRep_Owner();
	}
}

