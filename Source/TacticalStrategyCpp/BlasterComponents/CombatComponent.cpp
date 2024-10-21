
#include "CombatComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundCue.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"
#include "TacticalStrategyCpp/Hud/BlasterHud.h"
#include "TacticalStrategyCpp/PlayerController/BlasterPlayerController.h"
#include "TacticalStrategyCpp/Weapon/Projectile.h"
#include "TacticalStrategyCpp/Weapon/Shotgun.h"
#include "TacticalStrategyCpp/Weapon/Weapon.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent():
	RightHandSocketName(FString("RightHandSocket")),
	LeftHandSocketName(FString("LeftHandSocket")),BackpackSocketName(FString("BackpackSocket")),
	ShotgunReloadEndSectionName("ShotgunEnd"),
	Grenades(4),
	MaxGrenades(4),
	Character(nullptr),
	Controller(nullptr),
	Hud(nullptr),
	EquippedWeapon(nullptr), SecondaryWeapon(nullptr),
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
	HUDPackage(), bCanFire(true), CarriedAmmo(0), StartingArCarriedAmmo(30),
	StartingRocketAmmo(4), StartingSmgAmmo(60), StartingShotgunAmmo(10),
	StartingPistolAmmo(45),
	StartingSniperAmmo(15),
	StartingGrenadeAmmo(5),
	CombatState(ECombatState::ECS_Unoccupied)
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxCarriedAmmoMap.Emplace(EWeaponType::EWT_AssaultRifle, 99);
	MaxCarriedAmmoMap.Emplace(EWeaponType::EWT_RocketLauncher, 9);
	MaxCarriedAmmoMap.Emplace(EWeaponType::EWT_SMG, 149);
	MaxCarriedAmmoMap.Emplace(EWeaponType::EWT_Shotgun, 25);
	MaxCarriedAmmoMap.Emplace(EWeaponType::EWT_Pistol, 50);
	MaxCarriedAmmoMap.Emplace(EWeaponType::EWT_SniperRifle, 14);
	MaxCarriedAmmoMap.Emplace(EWeaponType::EWT_GrenadeLauncher, 12);

	BaseWalkSpeed = 600.f;
	AimWalkSpeed = 450.f;
}

bool UCombatComponent::PickupAmmo(EWeaponType AmmoType, const int32 AmmoAmount)
{
	bool Ret = false;
	if(AmmoType == EWeaponType::EWT_MAX)
	{
		if(EquippedWeapon)
			AmmoType = EquippedWeapon->GetWeaponType();
		else
		{
			if(Grenades != MaxGrenades)
			{
				Grenades = FMath::Clamp(Grenades + 1, 0, MaxGrenades);
				UpdateHudGrenades();
				Ret = true;
			}
		}
	}
	if(CarriedAmmoMap.Contains(AmmoType))
	{
		if(CarriedAmmoMap[AmmoType] != MaxCarriedAmmoMap[AmmoType])
		CarriedAmmoMap[AmmoType] = FMath::Clamp(CarriedAmmoMap[AmmoType] + AmmoAmount, 0,
			MaxCarriedAmmoMap[AmmoType]);

		UpdateCarriedAmmo();

		Ret = true;
	}
	if(EquippedWeapon && EquippedWeapon->IsEmpty() && EquippedWeapon->GetWeaponType() == AmmoType)
		Reload();

	return Ret;
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
		if(Character->HasAuthority())
			InitializeCarriedAmmo();
	}
}

void UCombatComponent::SetAiming(const bool bIsAiming)
{
	if(Character == nullptr || EquippedWeapon == nullptr) return;
	bAiming = bIsAiming;

	if(Character->IsLocallyControlled())
		bAimButtonPressed = bIsAiming;

	if(Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming? AimWalkSpeed : BaseWalkSpeed;
		
		if(!Character->HasAuthority())
			ServerSetAiming(bIsAiming);
	}
	if(Character->IsLocallyControlled() && EquippedWeapon->GetWeaponType() == EWeaponType::EWT_SniperRifle)
		Character->ShowSniperScopeWidget(bIsAiming);
}

void UCombatComponent::OnRep_EquippedWeapon()
{
	if(EquippedWeapon && Character)
	{
		EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped, true);

		AttachActorToRightHand(EquippedWeapon);
		
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		Character->bUseControllerRotationYaw = true;

		UpdateCarriedAmmo();

		ReloadEmptyWeapon();

		PlayEquipWeaponSound(EquippedWeapon);
		
		EquippedWeapon->SetHudAmmo();
	}
}

void UCombatComponent::OnRep_SecondaryWeapon() const
{
	if(SecondaryWeapon && Character)
	{
		SecondaryWeapon->SetWeaponState(EWeaponState::EWS_EquippedSecondary, true);
		AttachActorToBackPack(SecondaryWeapon);
		PlayEquipWeaponSound(SecondaryWeapon);
	}
}

void UCombatComponent::Fire()
{
	if(CanFire())
	{
		bCanFire = false;
		
		if(EquippedWeapon)
		{
			CrosshairShootingFactor = 0.75f;

			switch (EquippedWeapon->FireType)
			{
			case EFireType::EFT_Projectile:
				FireProjectileWeapon();
				break;
			case EFireType::EFT_HitScan:
				FireHitScanWeapon();
				break;
			case EFireType::EFT_Shotgun:
				FireShotgun();
				break;
			case EFireType::EFT_MAX:
				break;
			default: ;
			}
		}
	
		StartFireTimer();
	}
}

void UCombatComponent::FireProjectileWeapon()
{
	if(EquippedWeapon && Character)
	{
		HitTarget = EquippedWeapon->bUseScatter ? EquippedWeapon->TraceEndWithScatter(HitTarget) : HitTarget;
		Server_Fire(HitTarget, EquippedWeapon->FireDelay);
		if(!Character->HasAuthority()) LocalFire(HitTarget);
	}
}

void UCombatComponent::FireHitScanWeapon()
{
	if(EquippedWeapon && Character)
	{
		HitTarget = EquippedWeapon->bUseScatter ? EquippedWeapon->TraceEndWithScatter(HitTarget) : HitTarget;
		Server_Fire(HitTarget, EquippedWeapon->FireDelay);
		if(!Character->HasAuthority()) LocalFire(HitTarget);
	}
}

void UCombatComponent::FireShotgun()
{
	if(AShotgun* Shotgun = Cast<AShotgun>(EquippedWeapon); Shotgun && Character)
	{
		TArray<FVector_NetQuantize> HitTargets;
		Shotgun->ShotgunTraceEndWithScatter(HitTarget, HitTargets);
		if(!Character->HasAuthority()) LocalShotgunFire(HitTargets);
		ServerShotgunFire(HitTargets, EquippedWeapon->FireDelay);
	}
}

bool UCombatComponent::Server_Fire_Validate(const FVector_NetQuantize& TraceHitTarget, float FireDelay)
{
	if(EquippedWeapon)
	{
		const bool bNearlyEqual = FMath::IsNearlyEqual(EquippedWeapon->FireDelay, FireDelay, .01);
		return bNearlyEqual;
	}
	return true;
}

bool UCombatComponent::ServerShotgunFire_Validate(const TArray<FVector_NetQuantize>& TraceHitTargets, float FireDelay)
{
	if(EquippedWeapon)
	{
		const bool bNearlyEqual = FMath::IsNearlyEqual(EquippedWeapon->FireDelay, FireDelay, .01);
		return bNearlyEqual;
	}
	return true;
}

void UCombatComponent::ServerShotgunFire_Implementation(const TArray<FVector_NetQuantize>& TraceHitTargets, float FireDelay)
{
	MulticastShotgunFire(TraceHitTargets);
}

void UCombatComponent::FireButtonPressed(const bool bPressed)
{
	bFireButtonPressed = bPressed;

	if(bFireButtonPressed)
	{
		Fire();
	}
}

void UCombatComponent::ShotgunShellReload()
{
	if(Character && Character->HasAuthority())
		UpdateShotgunAmmoValues();
}

void UCombatComponent::JumpToShotgunEnd() const
{	
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if(AnimInstance && Character->GetReloadMontage())
	{			
		AnimInstance->Montage_JumpToSection(FName(ShotgunReloadEndSectionName));
	}
}

void UCombatComponent::ThrowGrenadeFinished()
{
	CombatState = ECombatState::ECS_Unoccupied;
	AttachActorToRightHand(EquippedWeapon);
}

void UCombatComponent::LaunchGrenade()
{
	ShowAttachedGrenade(false);
	if(Character && Character->IsLocallyControlled())
		Server_LaunchGrenade(HitTarget);
}

void UCombatComponent::Server_LaunchGrenade_Implementation(const FVector_NetQuantize& Target)
{	
	UStaticMeshComponent* StaticMeshComponent = Character->GetAttachedGrenade();
	if(Character && GrenadeClass && StaticMeshComponent)
	{
		const FVector StartingLocation = StaticMeshComponent->GetComponentLocation(),
			ToTarget = Target - StartingLocation;
		FActorSpawnParameters SpawnParam;
		SpawnParam.Owner = Character;
		SpawnParam.Instigator = Character;

		if(UWorld* World = GetWorld())
			World->SpawnActor<AProjectile>(GrenadeClass, StartingLocation, ToTarget.Rotation(), SpawnParam);
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

void UCombatComponent::HandleReload() const
{
	if(Character)
		Character->PlayReloadMontage();
}

void UCombatComponent::OnRep_CombatState()
{
	switch (CombatState)
	{
		case ECombatState::ECS_Unoccupied:
			if(bFireButtonPressed)
				Fire();
			break;
		case ECombatState::ECS_Reloading:
			if(Character && !Character->IsLocallyControlled()) HandleReload();
			break;
		case ECombatState::ECS_ThrowingGrenade:
			if(Character)
			{
				Character->PlayThrowGrenadeMontage();
				AttachActorToLeftHand(EquippedWeapon);
				ShowAttachedGrenade(true);
			}
			if(Character && Character->HasAuthority())
			{
				Grenades = FMath::Clamp(Grenades - 1, 0, MaxGrenades);
				UpdateHudGrenades();
			}
			break;
		
		case ECombatState::ECS_SwappingWeapons:
			if(Character && !Character->IsLocallyControlled())
				Character->PlayWeaponSwapMontage();
			break;
		
		case ECombatState::ECS_Max:
			break;
		default: ;
	}
}

int32 UCombatComponent::AmountToReload()
{
	if(EquippedWeapon == nullptr) return 0;
	
	const int32 RoomInMag = EquippedWeapon->GetMagCapacity() - EquippedWeapon->GetAmmo();

	if(const EWeaponType Key = EquippedWeapon->GetWeaponType();CarriedAmmoMap.Contains(Key))
	{
		const int32 AmountCarried = CarriedAmmoMap[Key];
		const int32 Least = FMath::Min(RoomInMag, AmountCarried);		
		return FMath::Clamp(RoomInMag, 0, Least);
	}
	return 0;
}

void UCombatComponent::Server_Reload_Implementation()
{
	if(Character == nullptr || EquippedWeapon == nullptr || AmountToReload() == 0) return;

	CombatState = ECombatState::ECS_Reloading;

	if(!Character->IsLocallyControlled()) HandleReload();
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
	ReloadEmptyWeapon();
	if(bFireButtonPressed && EquippedWeapon->bIsAutomatic)
	{
		Fire();
	}
}

bool UCombatComponent::CanFire() const
{
	if(EquippedWeapon == nullptr) return false;

	if(!EquippedWeapon->IsEmpty() && bCanFire && CombatState == ECombatState::ECS_Reloading &&
		EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Shotgun)
		return true;

	if(bLocallyReloading) return false;
	
	return !EquippedWeapon->IsEmpty() && bCanFire && (CombatState == ECombatState::ECS_Unoccupied);
}

void UCombatComponent::OnRep_CarriedAmmo()
{	
	Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) :
		Controller;
	if(Controller)
		Controller->SetHudCarriedAmmo(CarriedAmmo);
	if(CombatState == ECombatState::ECS_Reloading && EquippedWeapon != nullptr &&
		EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Shotgun && CarriedAmmo == 0)
		JumpToShotgunEnd();
}

void UCombatComponent::InitializeCarriedAmmo()
{
	CarriedAmmoMap.Emplace(EWeaponType::EWT_AssaultRifle, StartingArCarriedAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_RocketLauncher, StartingRocketAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_SMG, StartingSmgAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_Shotgun, StartingShotgunAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_Pistol, StartingPistolAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_SniperRifle, StartingSniperAmmo);
	CarriedAmmoMap.Emplace(EWeaponType::EWT_GrenadeLauncher, StartingGrenadeAmmo);
}

bool UCombatComponent::ShouldSwapWeapons() const
{
	return (EquippedWeapon != nullptr) && (SecondaryWeapon != nullptr);
}

void UCombatComponent::Server_Fire_Implementation(const FVector_NetQuantize& TraceHitTarget, float FireDelay)
{
	Multicast_Fire(TraceHitTarget);
}

void UCombatComponent::Multicast_Fire_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	if(Character && Character->IsLocallyControlled() && !Character->HasAuthority()) return;

	LocalFire(TraceHitTarget);
}

void UCombatComponent::LocalFire(const FVector_NetQuantize& TraceHitTarget) const
{
	if(EquippedWeapon == nullptr) return;
	
	if(Character && CombatState == ECombatState::ECS_Unoccupied)
	{
		Character->PlayFireMontage(bAiming);
		EquippedWeapon->Fire(TraceHitTarget);
	}
}

void UCombatComponent::LocalShotgunFire(const TArray<FVector_NetQuantize>& TraceHitTargets)
{
	// ReSharper disable once CppTooWideScopeInitStatement
	AShotgun* Shotgun = Cast<AShotgun>(EquippedWeapon);
	if(Shotgun == nullptr || Character == nullptr) return;
	if(CombatState == ECombatState::ECS_Reloading || CombatState == ECombatState::ECS_Unoccupied)
	{
		Character->PlayFireMontage(bAiming);
		Shotgun->FireShotgun(TraceHitTargets);
		CombatState = ECombatState::ECS_Unoccupied;		
	}		
}

void UCombatComponent::MulticastShotgunFire_Implementation(const TArray<FVector_NetQuantize>& TraceHitTargets)
{
	if(Character && Character->IsLocallyControlled() && !Character->HasAuthority()) return;
	LocalShotgunFire(TraceHitTargets);
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
	
	DOREPLIFETIME(UCombatComponent, SecondaryWeapon);
	
	DOREPLIFETIME(UCombatComponent, bAiming);
	
	DOREPLIFETIME_CONDITION(UCombatComponent, CarriedAmmo, COND_OwnerOnly);
	
	DOREPLIFETIME(UCombatComponent, CombatState);
	
	DOREPLIFETIME(UCombatComponent, Grenades);
}

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if(Character == nullptr || WeaponToEquip == nullptr) return;

	if(CombatState != ECombatState::ECS_Unoccupied) return;

	if(EquippedWeapon != nullptr && SecondaryWeapon == nullptr)
		EquipSecondaryWeapon(WeaponToEquip);
	else
		EquipPrimaryWeapon(WeaponToEquip);
}

void UCombatComponent::SwapWeapons()
{
	if(CombatState != ECombatState::ECS_Unoccupied || Character == nullptr) return;

	Character->PlayWeaponSwapMontage();
	CombatState = ECombatState::ECS_SwappingWeapons;
	Character->bFinishedSwapping = false;

	if(SecondaryWeapon)
		SecondaryWeapon->EnableCustomDepth(false);
}

void UCombatComponent::EquipPrimaryWeapon(AWeapon* WeaponToEquip)
{
	DropEquippedWeapon();

	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetOwner(Character);
	
	if(Character->HasAuthority())
	{
		OnRep_EquippedWeapon();
		if(EquippedWeapon)
			EquippedWeapon->OnRep_Owner();
	}
}

void UCombatComponent::EquipSecondaryWeapon(AWeapon* WeaponToEquip)
{
	SecondaryWeapon = WeaponToEquip;
	SecondaryWeapon->SetOwner(Character);

	if(Character->HasAuthority())
	{
		OnRep_SecondaryWeapon();
		if(SecondaryWeapon)
			SecondaryWeapon->OnRep_Owner();
	}
}

void UCombatComponent::Reload()
{
	if(CarriedAmmo > 0 && CombatState == ECombatState::ECS_Unoccupied && EquippedWeapon && !EquippedWeapon->IsFull() &&
		!bLocallyReloading )
	{
		bLocallyReloading = true;
		Server_Reload();
		HandleReload();
	}
}

void UCombatComponent::UpdateAmmoValues()
{
	const int32 ReloadAmount = AmountToReload();
	if(const EWeaponType Key = EquippedWeapon->GetWeaponType();CarriedAmmoMap.Contains(Key))
	{
		CarriedAmmoMap[Key] -= ReloadAmount;
		CarriedAmmo = CarriedAmmoMap[Key];
	}

	Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) :
		             Controller;
	if(Controller)
		Controller->SetHudCarriedAmmo(CarriedAmmo);
	
	EquippedWeapon->AddAmmo(ReloadAmount);
}

void UCombatComponent::UpdateShotgunAmmoValues()
{
	if(const EWeaponType Key = EquippedWeapon->GetWeaponType();CarriedAmmoMap.Contains(Key))
	{
		CarriedAmmoMap[Key] -= 1;
		CarriedAmmo = CarriedAmmoMap[Key];
	}
	Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) :
					 Controller;
	if(Controller)
		Controller->SetHudCarriedAmmo(CarriedAmmo);
	
	EquippedWeapon->AddAmmo(1);
	bCanFire = true;
	if(EquippedWeapon->IsFull() || CarriedAmmo == 0) // if shotgun is full stop reload animation
		JumpToShotgunEnd();
}

void UCombatComponent::ThrowGrenade()
{
	if(Grenades == 0) return;
	// || EquippedWeapon == nullptr
	if(CombatState != ECombatState::ECS_Unoccupied) return;
	
	CombatState = ECombatState::ECS_ThrowingGrenade;
	OnRep_CombatState();
	
	if(Character && !Character->HasAuthority())
		Server_ThrowGrenade();
}

void UCombatComponent::DropEquippedWeapon() const
{	
	if(EquippedWeapon)
		EquippedWeapon->Dropped();
}

void UCombatComponent::AttachActorToRightHand(AActor* ActorToAttach) const
{
	if(Character == nullptr || ActorToAttach == nullptr) return;
	USkeletalMeshComponent* SkeletalMeshComponent = Character->GetMesh();
	if(SkeletalMeshComponent == nullptr) return;
	if(const USkeletalMeshSocket* HandSocket = SkeletalMeshComponent->GetSocketByName(FName(RightHandSocketName)))
	{
		HandSocket->AttachActor(ActorToAttach, SkeletalMeshComponent);
	}
}

void UCombatComponent::AttachActorToLeftHand(AActor* ActorToAttach) const
{
	if(Character == nullptr || ActorToAttach == nullptr) return;
	USkeletalMeshComponent* SkeletalMeshComponent = Character->GetMesh();
	if(SkeletalMeshComponent == nullptr) return;
	if(const USkeletalMeshSocket* HandSocket = SkeletalMeshComponent->GetSocketByName(FName(LeftHandSocketName)))
	{
		HandSocket->AttachActor(ActorToAttach, SkeletalMeshComponent);
	}
}

void UCombatComponent::AttachActorToBackPack(AActor* ActorToAttach) const
{
	if(Character == nullptr || Character->GetMesh() == nullptr || ActorToAttach == nullptr) return;
	
	USkeletalMeshComponent* SkeletalMeshComponent = Character->GetMesh();
	if(const USkeletalMeshSocket* BackpackSocket = SkeletalMeshComponent->GetSocketByName(FName(BackpackSocketName)))
	{
		BackpackSocket->AttachActor(ActorToAttach, SkeletalMeshComponent);
	}
}

void UCombatComponent::UpdateCarriedAmmo()
{
	if(EquippedWeapon == nullptr) return;
	if(const EWeaponType Key = EquippedWeapon->GetWeaponType(); CarriedAmmoMap.Contains(Key))
		CarriedAmmo = CarriedAmmoMap[Key];
	if(Character->HasAuthority())
		OnRep_CarriedAmmo();
}

void UCombatComponent::PlayEquipWeaponSound(const AWeapon* WeaponToEquip) const
{
	if(Character && WeaponToEquip)
	{
		if(USoundCue* Sound = WeaponToEquip->EquipSound)
			UGameplayStatics::PlaySoundAtLocation(this, Sound,
				Character->GetActorLocation());
	}
}

void UCombatComponent::ReloadEmptyWeapon()
{	
	if(EquippedWeapon->IsEmpty())
	{
		Reload();
	}
	bCanFire = true;
}

void UCombatComponent::ShowAttachedGrenade(const bool bShowGrenade) const
{
	UStaticMeshComponent* AttachedGrenade = Character->GetAttachedGrenade();
	if(Character && AttachedGrenade)
		AttachedGrenade->SetVisibility(bShowGrenade);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void UCombatComponent::OnRep_Grenades()
{	
	UpdateHudGrenades();
}

void UCombatComponent::UpdateHudGrenades()
{	
	Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) :
		Controller;

	if(Controller)
	{
		Controller->SetHudGrenades(Grenades);
	}
}

void UCombatComponent::OnRep_Aiming()
{
	if(Character && Character->IsLocallyControlled())
		bAiming = bAimButtonPressed;
}

void UCombatComponent::Server_ThrowGrenade_Implementation()
{
	if(Grenades == 0) return;
	CombatState = ECombatState::ECS_ThrowingGrenade;

	OnRep_CombatState();
}

void UCombatComponent::FinishReloading()
{ 
	if(Character == nullptr) return;
	bLocallyReloading = false;
	
	if(Character->HasAuthority())
	{
		CombatState = ECombatState::ECS_Unoccupied;
		UpdateAmmoValues();
	}
	
	if(bFireButtonPressed)
		Fire();
}

void UCombatComponent::FinishSwap()
{
	if(Character && Character->HasAuthority())
		CombatState = ECombatState::ECS_Unoccupied;
	
	if(Character) Character->bFinishedSwapping = true;
	
	if(SecondaryWeapon)
		SecondaryWeapon->EnableCustomDepth(true);
}

void UCombatComponent::FinishSwapAttachWeapons()
{
	if(Character == nullptr || !Character->HasAuthority()) return;
	
	AWeapon* TempWeapon = EquippedWeapon;
	EquippedWeapon = SecondaryWeapon;
	SecondaryWeapon = TempWeapon;
	
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	AttachActorToRightHand(EquippedWeapon);
	EquippedWeapon->SetHudAmmo();
	UpdateCarriedAmmo();
	PlayEquipWeaponSound(EquippedWeapon);

	SecondaryWeapon->SetWeaponState(EWeaponState::EWS_EquippedSecondary);
	AttachActorToBackPack(SecondaryWeapon);
}
