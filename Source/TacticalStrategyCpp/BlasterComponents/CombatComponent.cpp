
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

UCombatComponent::UCombatComponent():
	RightHandSocketName("RightHandSocket"),
	LeftHandSocketName("LeftHandSocket"),BackpackSocketName("BackpackSocket"),
	LeftHandFlagSocketName("LeftHandFlagSocket"), ShotgunReloadEndSectionName("ShotgunEnd"),
	Grenades(4),
	MaxGrenades(4),
	Character(nullptr),
	Controller(nullptr),
	Hud(nullptr),
	EquippedWeapon(nullptr), SecondaryWeapon(nullptr),
	bFireButtonPressed(false),
	bCanFire(true),
	CrosshairBaseLineSpread(0.5f),
	CrosshairVelocityFactor(0),
	CrosshairInAirFactor(0),
	CrosshairAimFactor(0),
	CrosshairShootingFactor(0),
	DefaultFov(0),
	CurrentFov(0),
	ZoomedFov(30.f),
	ZoomInterpSpeed(20.f), HUDPackage(), CarriedAmmo(0), StartingArCarriedAmmo(30),
	StartingRocketAmmo(4), StartingSmgAmmo(60), StartingShotgunAmmo(10),
	StartingPistolAmmo(45),
	StartingSniperAmmo(15),
	StartingGrenadeAmmo(5),
	CombatState(ECombatState::ECS_Unoccupied),
	TheFlag(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;

	// Initialize max ammo counts
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

// Enables or disables aiming state and updates movement speed accordingly
void UCombatComponent::SetAiming(const bool bIsAiming)
{
	if(Character == nullptr || EquippedWeapon == nullptr) return;

	bAiming = bIsAiming;
	if(Character->IsLocallyControlled())
		bAimButtonPressed = bIsAiming;

	// Update movement speed based on aiming state
	Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;

	// Ensure aiming state is replicated to server
	if(!Character->HasAuthority())
		ServerSetAiming(bIsAiming);

	// Display sniper scope widget if weapon type is sniper
	if(Character->IsLocallyControlled() && EquippedWeapon->GetWeaponType() == EWeaponType::EWT_SniperRifle)
		Character->ShowSniperScopeWidget(bIsAiming);
}

// Replicated function to handle aiming state on server
void UCombatComponent::ServerSetAiming_Implementation(const bool bIsAiming)
{
	SetAiming(bIsAiming);
}

// Replicates equipped weapon properties and sets attachment points
void UCombatComponent::OnRep_EquippedWeapon()
{
	if(EquippedWeapon && Character)
	{
		EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped, true);
		AttachActorToRightHand(EquippedWeapon);

		// Adjust rotation handling based on weapon type
		if(EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Flag)
		{
			Character->GetCharacterMovement()->bOrientRotationToMovement = true;
			Character->bUseControllerRotationYaw = false;
		}
		else
		{
			Character->GetCharacterMovement()->bOrientRotationToMovement = false;
			Character->bUseControllerRotationYaw = true;
		}

		// Update UI with carried ammo count and weapon state
		UpdateCarriedAmmo();
		PlayEquipWeaponSound(EquippedWeapon);
		EquippedWeapon->SetHudAmmo();
		ReloadEmptyWeapon();
	}
}

// Handles weapon firing logic and checks the weapon fire type (projectile, hitscan, or shotgun)
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

// Fire logic for projectile-based weapons
void UCombatComponent::FireProjectileWeapon()
{
	if(EquippedWeapon && Character)
	{
		HitTarget = EquippedWeapon->bUseScatter ? EquippedWeapon->TraceEndWithScatter(HitTarget) : HitTarget;
		Server_Fire(HitTarget, EquippedWeapon->FireDelay);

		// Local fire for client-side prediction
		if(!Character->HasAuthority()) LocalFire(HitTarget);
	}
}

// Fire logic for hitscan weapons
void UCombatComponent::FireHitScanWeapon()
{
	if(EquippedWeapon && Character)
	{
		HitTarget = EquippedWeapon->bUseScatter ? EquippedWeapon->TraceEndWithScatter(HitTarget) : HitTarget;
		Server_Fire(HitTarget, EquippedWeapon->FireDelay);
		
		if(!Character->HasAuthority()) LocalFire(HitTarget);
	}
}

// Fire logic for shotgun-type weapons, uses multiple hit targets
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

// Determines whether the character can fire the equipped weapon
bool UCombatComponent::CanFire() const
{
	if(EquippedWeapon == nullptr)
		return false;
 
	// If the weapon is a shotgun, it's currently reloading, and the player can fire, allow firing
	if(EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Shotgun && CombatState == ECombatState::ECS_Reloading && !EquippedWeapon->IsEmpty() && bCanFire)
		return true;
 
	// If the player is locally reloading, they cannot fire
	if(bLocallyReloading)
		return false;
 
	// Check if the weapon is not empty, the player is allowed to fire, and the combat state allows it
	return !EquippedWeapon->IsEmpty() && bCanFire && CombatState == ECombatState::ECS_Unoccupied;
}

// Starts a timer to prevent immediate re-firing, based on weapon fire delay
void UCombatComponent::StartFireTimer()
{
	if(EquippedWeapon == nullptr || Character == nullptr) return;
	Character->GetWorldTimerManager().SetTimer(FireTimer, this, &UCombatComponent::FireTimerFinished, EquippedWeapon->FireDelay);
}

// Resets fire control after the fire timer completes
void UCombatComponent::FireTimerFinished()
{
	if(EquippedWeapon == nullptr) return;
	ReloadEmptyWeapon();

	// Handles automatic fire if fire button is held down
	if(bFireButtonPressed && EquippedWeapon->bIsAutomatic)
		Fire();
}

// Updates FOV during aiming, transitioning between default and zoomed FOV
void UCombatComponent::InterpFov(const float DeltaTime)
{
	if(EquippedWeapon == nullptr) return;

	const float TargetFov = bAiming ? EquippedWeapon->GetZoomedFov() : DefaultFov;
	const float TargetZoomSpeed = bAiming ? EquippedWeapon->GetZoomInterpSpeed() : ZoomInterpSpeed;
	CurrentFov = FMath::FInterpTo(CurrentFov, TargetFov, DeltaTime, TargetZoomSpeed);

	if(Character)
		if(UCameraComponent* CameraComponent = Character->GetFollowCamera())
			CameraComponent->SetFieldOfView(CurrentFov);
}

// Handles reload logic and updates ammo count in UI
void UCombatComponent::Reload()
{
	if(CarriedAmmo > 0 && CombatState == ECombatState::ECS_Unoccupied && EquippedWeapon && !EquippedWeapon->IsFull() && !bLocallyReloading)
	{
		bLocallyReloading = true;
		Server_Reload();
		HandleReload();
	}
}

// Equips primary weapon by detaching the current weapon and setting the new weapon as equipped
void UCombatComponent::EquipPrimaryWeapon(AWeapon* WeaponToEquip)
{
	DropEquippedWeapon();

	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetOwner(Character);

	if(Character->HasAuthority())
	{
		OnRep_EquippedWeapon();
		EquippedWeapon->OnRep_Owner();
	}
}

// Equips secondary weapon and attaches it to the backpack socket
void UCombatComponent::EquipSecondaryWeapon(AWeapon* WeaponToEquip)
{
	SecondaryWeapon = WeaponToEquip;
	SecondaryWeapon->SetOwner(Character);

	if(Character->HasAuthority())
	{
		OnRep_SecondaryWeapon();
		SecondaryWeapon->OnRep_Owner();
	}
}

void UCombatComponent::AttachActorToRightHand(AActor* ActorToAttach) const
{
	if(Character == nullptr || ActorToAttach == nullptr) return;

	if(USkeletalMeshComponent* SkeletalMeshComponent = Character->GetMesh())
		if(const USkeletalMeshSocket* HandSocket = SkeletalMeshComponent->GetSocketByName(FName(RightHandSocketName)))
			HandSocket->AttachActor(ActorToAttach, SkeletalMeshComponent);
}

void UCombatComponent::AttachActorToLeftHand(AActor* ActorToAttach) const
{
	if(Character == nullptr || ActorToAttach == nullptr) return;

	if(USkeletalMeshComponent* SkeletalMeshComponent = Character->GetMesh())
		if(const USkeletalMeshSocket* HandSocket = SkeletalMeshComponent->GetSocketByName(FName(LeftHandSocketName)))
			HandSocket->AttachActor(ActorToAttach, SkeletalMeshComponent);
}

// Drops the currently equipped weapon
void UCombatComponent::DropEquippedWeapon() const
{
	if(EquippedWeapon)
		EquippedWeapon->Dropped();
}

// Validates that the weapon's firing delay matches the given FireDelay (for server security)
bool UCombatComponent::Server_Fire_Validate(const FVector_NetQuantize& TraceHitTarget, float FireDelay)
{
	return EquippedWeapon ? FMath::IsNearlyEqual(EquippedWeapon->FireDelay, FireDelay, 0.01f) : true;
}

// Validates that the shotgun weapon's firing delay matches the given FireDelay
bool UCombatComponent::ServerShotgunFire_Validate(const TArray<FVector_NetQuantize>& TraceHitTargets, float FireDelay)
{
	return EquippedWeapon ? FMath::IsNearlyEqual(EquippedWeapon->FireDelay, FireDelay, 0.01f) : true;
}

// Executes shotgun firing on all clients
void UCombatComponent::ServerShotgunFire_Implementation(const TArray<FVector_NetQuantize>& TraceHitTargets, float FireDelay)
{
	MulticastShotgunFire(TraceHitTargets);
}

// Toggles the firing state when the fire button is pressed/released
void UCombatComponent::FireButtonPressed(const bool bPressed)
{
	bFireButtonPressed = bPressed;

	// Initiates firing if the button is pressed
	if(bFireButtonPressed)
		Fire();
}

// Adds a single shotgun shell to the weapon's magazine (for shotguns)
void UCombatComponent::ShotgunShellReload()
{
	if(Character && Character->HasAuthority())
		UpdateShotgunAmmoValues();
}

// Jumps animation to the end of the shotgun reload section
void UCombatComponent::JumpToShotgunEnd() const
{
	if(UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
		if(Character->GetReloadMontage())
			AnimInstance->Montage_JumpToSection(FName(ShotgunReloadEndSectionName));
}

// Completes the grenade throw and re-equips the weapon in the right hand
void UCombatComponent::ThrowGrenadeFinished()
{
	CombatState = ECombatState::ECS_Unoccupied;
	AttachActorToRightHand(EquippedWeapon);
}

// Launches a grenade towards the specified target
void UCombatComponent::LaunchGrenade()
{
	ShowAttachedGrenade(false);
	if(Character && Character->IsLocallyControlled())
		Server_LaunchGrenade(HitTarget);
}

// Server function to spawn a grenade projectile
void UCombatComponent::Server_LaunchGrenade_Implementation(const FVector_NetQuantize& Target)
{
	if(Character && GrenadeClass && Character->GetAttachedGrenade())
	{
		const FVector StartLocation = Character->GetAttachedGrenade()->GetComponentLocation(), Direction = Target - StartLocation;

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Character;
		SpawnParams.Instigator = Character;

		if(UWorld* World = GetWorld())
			World->SpawnActor<AProjectile>(GrenadeClass, StartLocation, Direction.Rotation(), SpawnParams);
	}
}

// Traces a line from the crosshair to find hit results and sets crosshair color based on hit type
void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	if(GEngine && GEngine->GameViewport)
		GEngine->GameViewport->GetViewportSize(ViewportSize);

	const FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition, CrosshairWorldDirection;

	// ReSharper disable once CppTooWideScope
	const bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection);

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
		const bool bHit = GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECC_Visibility);

		// Update crosshair color based on hit result
		if(bHit && TraceHitResult.GetActor()->Implements<UInteractWithCrosshairsInterface>())
			HUDPackage.CrosshairsColor = FLinearColor::Red;
		else
			HUDPackage.CrosshairsColor = FLinearColor::White;

		// Set default impact point if no hit
		if(!bHit)
			TraceHitResult.ImpactPoint = End;
	}
}

// Updates the carried ammo and UI after picking up ammo
bool UCombatComponent::PickupAmmo(EWeaponType AmmoType, const int32 AmmoAmount)
{
	bool bPickedUpAmmo = false;

	if(AmmoType == EWeaponType::EWT_MAX)
	{
		// For grenades, cap at max limit
		if(Grenades < MaxGrenades)
		{
			Grenades = FMath::Clamp(Grenades + 1, 0, MaxGrenades);
			UpdateHudGrenades();
			bPickedUpAmmo = true;
		}
		if(EquippedWeapon)
			AmmoType = EquippedWeapon->GetWeaponType();
	}
	if(CarriedAmmoMap.Contains(AmmoType))
	{
		// Update carried ammo up to the maximum allowed
		CarriedAmmoMap[AmmoType] = FMath::Clamp(CarriedAmmoMap[AmmoType] + AmmoAmount, 0, MaxCarriedAmmoMap[AmmoType]);
		UpdateCarriedAmmo();
		bPickedUpAmmo = true;
	}

	// Reload if equipped weapon is empty and matches the picked-up ammo type
	if(EquippedWeapon && EquippedWeapon->IsEmpty() && EquippedWeapon->GetWeaponType() == AmmoType)
		Reload();

	return bPickedUpAmmo;
}

// Replicates the flag attachment to the left hand for all clients
void UCombatComponent::OnRep_TheFlag() const
{
	if(TheFlag && Character)
	{
		TheFlag->SetWeaponState(EWeaponState::EWS_Equipped, true);
		AttachFlagToLeftHand(TheFlag);

		Character->GetCharacterMovement()->bOrientRotationToMovement = true;
		Character->bUseControllerRotationYaw = false;

		PlayEquipWeaponSound(TheFlag);
	}
}

// Replicates the secondary weapon attachment to the backpack for all clients
void UCombatComponent::OnRep_SecondaryWeapon() const
{
	if(SecondaryWeapon && Character)
	{
		SecondaryWeapon->SetWeaponState(EWeaponState::EWS_EquippedSecondary, true);
		AttachActorToBackPack(SecondaryWeapon);
		PlayEquipWeaponSound(SecondaryWeapon);
	}
}

// Updates HUD crosshair spread based on character movement, aiming, and firing state
void UCombatComponent::SetHudCrosshairs(const float DeltaTime)
{
	if(Character == nullptr || Character->Controller == nullptr) return;

	Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;

	if(Controller)
	{
		Hud = Hud == nullptr ? Cast<ABlasterHud>(Controller->GetHUD()) : Hud;
		if(Hud)
		{
			// Set crosshair textures from equipped weapon
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

			// Calculate crosshair spread factors
			const FVector Velocity = Character->GetVelocity();
			const float VelocitySize = FVector(Velocity.X, Velocity.Y, 0.f).Size();
			const float MaxWalkSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;
			CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(FVector2D{0.f, MaxWalkSpeed}, {0.f, 1.f}, VelocitySize);

			const FVector2D WalkSpeedRange(0.f, Character->GetCharacterMovement()->MaxWalkSpeed), VelocityMultiplierRange(0.f, 1.f);

			CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());

			if(Character->GetCharacterMovement()->IsFalling())
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 2.25f, DeltaTime, 2.25);
			else
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.f, DeltaTime, 30);
			
			// for aiming to be in sync we can use the equipped weapon zoom interp speed
			/*
			if(EquippedWeapon)
				EquippedWeapon->GetZoomInterpSpeed()
			*/
			CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, bAiming ? 0.58f : 0.f, DeltaTime, 30.f);
			
			CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.f, DeltaTime, 40.f);

			// Update final crosshair spread and UI
			HUDPackage.CrosshairSpread = CrosshairBaseLineSpread + CrosshairVelocityFactor + CrosshairInAirFactor + CrosshairShootingFactor - CrosshairAimFactor;
			Hud->SetHudPackage(HUDPackage);
		}
	}
}

// Plays reload animation for the character
void UCombatComponent::HandleReload() const
{
	if(Character)
		Character->PlayReloadMontage();
}

// Handles changes in combat state, including reloading, throwing grenades, and swapping weapons
void UCombatComponent::OnRep_CombatState()
{
	switch (CombatState)
	{
		case ECombatState::ECS_Unoccupied:
			if(bFireButtonPressed) Fire();
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

// Calculates the amount of ammo needed to fully reload the weapon's magazine
int32 UCombatComponent::AmountToReload()
{
	if(EquippedWeapon == nullptr) return 0;

	const int32 RoomInMag = EquippedWeapon->GetMagCapacity() - EquippedWeapon->GetAmmo();
	const EWeaponType AmmoType = EquippedWeapon->GetWeaponType();
	const int32 AmmoCarried = CarriedAmmoMap.Contains(AmmoType) ? CarriedAmmoMap[AmmoType] : 0;
	const int32 Least = FMath::Min(RoomInMag, AmmoCarried);

	return FMath::Clamp(RoomInMag, 0, Least);
}

// Server function to initiate reloading if there is ammo available
void UCombatComponent::Server_Reload_Implementation()
{
	if(Character && EquippedWeapon && AmountToReload() > 0)
	{
		CombatState = ECombatState::ECS_Reloading;
		if(!Character->IsLocallyControlled()) HandleReload();
	}
}

// Updates HUD for carried ammo when replicated
void UCombatComponent::OnRep_CarriedAmmo()
{
	Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) :
		Controller;
	if(Controller)
		Controller->SetHudCarriedAmmo(CarriedAmmo);
	if(CombatState == ECombatState::ECS_Reloading && EquippedWeapon && EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Shotgun && CarriedAmmo == 0)
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

// Crouches/Uncrouches character based on flag-holding state
void UCombatComponent::OnRep_HoldingFlag() const
{
	if(Character && Character->IsLocallyControlled())
	{
		if(bHoldingFlag)
			Character->Crouch();
		else
			Character->UnCrouch();
	}
}

// Returns true if both a primary and secondary weapon are equipped
bool UCombatComponent::ShouldSwapWeapons() const
{
	return EquippedWeapon && SecondaryWeapon;
}

// Executes firing on server and replicates to clients
void UCombatComponent::Server_Fire_Implementation(const FVector_NetQuantize& TraceHitTarget, float FireDelay)
{
	Multicast_Fire(TraceHitTarget);
}

void UCombatComponent::Multicast_Fire_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	if(Character && Character->IsLocallyControlled() && !Character->HasAuthority()) return;

	LocalFire(TraceHitTarget);
}

// Locally fires the weapon by playing animation and triggering weapon fire
void UCombatComponent::LocalFire(const FVector_NetQuantize& TraceHitTarget) const
{
	if(EquippedWeapon == nullptr || CombatState != ECombatState::ECS_Unoccupied) return;

	if(Character)
		Character->PlayFireMontage(bAiming);

	if(EquippedWeapon)
		EquippedWeapon->Fire(TraceHitTarget);
}

// Handles shotgun firing logic on client
void UCombatComponent::LocalShotgunFire(const TArray<FVector_NetQuantize>& TraceHitTargets)
{
	AShotgun* Shotgun = Cast<AShotgun>(EquippedWeapon);
	if(Shotgun == nullptr || Character == nullptr) return;

	if(CombatState == ECombatState::ECS_Reloading || CombatState == ECombatState::ECS_Unoccupied)
	{
		bLocallyReloading = false;
		Character->PlayFireMontage(bAiming);
		Shotgun->FireShotgun(TraceHitTargets);
		CombatState = ECombatState::ECS_Unoccupied;
	}
}

// Replicates shotgun firing on all clients
void UCombatComponent::MulticastShotgunFire_Implementation(const TArray<FVector_NetQuantize>& TraceHitTargets)
{
	if(Character && Character->IsLocallyControlled() && !Character->HasAuthority()) return;
	LocalShotgunFire(TraceHitTargets);
}

// Per-frame updates for crosshair, aiming, and FOV
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(Character && Character->IsLocallyControlled())
	{
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		HitTarget = HitResult.ImpactPoint;

		SetHudCrosshairs(DeltaTime);

		InterpFov(DeltaTime);
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
	DOREPLIFETIME(UCombatComponent, bHoldingFlag);
	DOREPLIFETIME(UCombatComponent, TheFlag);
}

// Equips the specified weapon, handling primary, secondary, and flag weapon types
void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if(Character == nullptr || WeaponToEquip == nullptr || CombatState != ECombatState::ECS_Unoccupied) return;

	if(WeaponToEquip->GetWeaponType() == EWeaponType::EWT_Flag)
	{
		bHoldingFlag = true;
		if(Character->HasAuthority())
			OnRep_HoldingFlag();

		WeaponToEquip->SetWeaponState(EWeaponState::EWS_Equipped);
		TheFlag = WeaponToEquip;
		AttachFlagToLeftHand(WeaponToEquip);
		WeaponToEquip->SetOwner(Character);

		if(Character->HasAuthority())
			WeaponToEquip->OnRep_Owner();
	}
	else
	{
		if(EquippedWeapon != nullptr && SecondaryWeapon == nullptr)
			EquipSecondaryWeapon(WeaponToEquip);
		else
			EquipPrimaryWeapon(WeaponToEquip);
	}
}

// Starts the weapon swap animation and sets combat state
void UCombatComponent::SwapWeapons()
{
	if(CombatState != ECombatState::ECS_Unoccupied || Character == nullptr) return;

	Character->PlayWeaponSwapMontage();
	CombatState = ECombatState::ECS_SwappingWeapons;
	Character->SetFinishedSwapping(false);

	if(SecondaryWeapon)
		SecondaryWeapon->EnableCustomDepth(false);
}

// Updates ammo values based on reload amount and updates UI
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

// Updates shotgun ammo values and HUD, stops reload if full or out of ammo
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

	// Stop reloading if shotgun is full or out of ammo
	if(EquippedWeapon->IsFull() || CarriedAmmo == 0)
		JumpToShotgunEnd();
}

// Initiates grenade throw animation and updates combat state
void UCombatComponent::ThrowGrenade()
{
	if(Grenades == 0 || CombatState != ECombatState::ECS_Unoccupied) return;

	CombatState = ECombatState::ECS_ThrowingGrenade;
	OnRep_CombatState();

	if(Character && !Character->HasAuthority())
		Server_ThrowGrenade();
}

// Attaches weapon to the backpack socket
void UCombatComponent::AttachActorToBackPack(AActor* ActorToAttach) const
{
	if(Character && Character->GetMesh() && ActorToAttach)
	{
		USkeletalMeshComponent* SkeletalMeshComponent = Character->GetMesh();
		if(const USkeletalMeshSocket* BackpackSocket = SkeletalMeshComponent->GetSocketByName(FName(BackpackSocketName)))
			BackpackSocket->AttachActor(ActorToAttach, SkeletalMeshComponent);
	}
}

// Attaches the flag to the character's left hand
void UCombatComponent::AttachFlagToLeftHand(AWeapon* Flag) const
{
	if(Character && Character->GetMesh() && Flag)
	{
		USkeletalMeshComponent* SkeletalMeshComponent = Character->GetMesh();
		if(const USkeletalMeshSocket* HandSocket = SkeletalMeshComponent->GetSocketByName(FName(LeftHandFlagSocketName)))
			HandSocket->AttachActor(Flag, SkeletalMeshComponent);
	}
}

// Updates carried ammo based on the equipped weapon's type
void UCombatComponent::UpdateCarriedAmmo()
{
	if(EquippedWeapon == nullptr) return;
	if(CarriedAmmoMap.Contains(EquippedWeapon->GetWeaponType()))
	{
		CarriedAmmo = CarriedAmmoMap[EquippedWeapon->GetWeaponType()];
		if(Character->HasAuthority())
			OnRep_CarriedAmmo();
	}
}

// Plays the equipped weapon's sound effect
void UCombatComponent::PlayEquipWeaponSound(const AWeapon* WeaponToEquip) const
{
	if(Character && WeaponToEquip && WeaponToEquip->EquipSound)
		UGameplayStatics::PlaySoundAtLocation(this, WeaponToEquip->EquipSound, Character->GetActorLocation());
}

// Reloads if equipped weapon is empty, re-enables firing
void UCombatComponent::ReloadEmptyWeapon()
{
	if(EquippedWeapon && EquippedWeapon->IsEmpty())
		Reload();
	bCanFire = true;
}

// Toggles the visibility of an attached grenade
void UCombatComponent::ShowAttachedGrenade(const bool bShowGrenade) const
{
	if(Character && Character->GetAttachedGrenade())
		Character->GetAttachedGrenade()->SetVisibility(bShowGrenade);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// Updates grenade count on UI
void UCombatComponent::OnRep_Grenades()
{	
	UpdateHudGrenades();
}

void UCombatComponent::UpdateHudGrenades()
{
	Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) :
		Controller;
	if(Controller)
		Controller->SetHudGrenades(Grenades);
}

// Aiming replication to handle aiming state changes for networked gameplay
void UCombatComponent::OnRep_Aiming()
{
	if(Character && Character->IsLocallyControlled())
		bAiming = bAimButtonPressed;
}

// Handles grenade throw on server side
void UCombatComponent::Server_ThrowGrenade_Implementation()
{
	if(Grenades == 0) return;
	CombatState = ECombatState::ECS_ThrowingGrenade;
	OnRep_CombatState();
}

// Completes reloading and resets the combat state
void UCombatComponent::FinishReloading()
{
	bLocallyReloading = false;

	if(Character && Character->HasAuthority())
	{
		CombatState = ECombatState::ECS_Unoccupied;
		UpdateAmmoValues();
	}

	if(bFireButtonPressed)
		Fire();
}

// Completes weapon swap and resets combat state
void UCombatComponent::FinishSwap()
{
	if(Character && Character->HasAuthority())
	{
		CombatState = ECombatState::ECS_Unoccupied;
		OnRep_EquippedWeapon();
		OnRep_SecondaryWeapon();
	}

	if(Character) Character->SetFinishedSwapping(true);

	if(SecondaryWeapon) SecondaryWeapon->EnableCustomDepth(true);
}

// Swaps primary and secondary weapon references
void UCombatComponent::FinishSwapAttachWeapons()
{
	PlayEquipWeaponSound(SecondaryWeapon);

	if(Character && Character->HasAuthority())
	{
		AWeapon* TempWeapon = EquippedWeapon;
		EquippedWeapon = SecondaryWeapon;
		SecondaryWeapon = TempWeapon;
	}
}
