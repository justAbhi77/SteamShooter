// Fill out your copyright notice in the Description page of Project Settings.

#include "BlasterCharacter.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "TacticalStrategyCpp/TacticalStrategyCpp.h"
#include "TacticalStrategyCpp/BlasterComponents/BuffComponent.h"
#include "TacticalStrategyCpp/BlasterComponents/CombatComponent.h"
#include "TacticalStrategyCpp/BlasterComponents/LagCompensationComponent.h"
#include "TacticalStrategyCpp/GameMode/BlasterGameMode.h"
#include "TacticalStrategyCpp/PlayerController/BlasterPlayerController.h"
#include "TacticalStrategyCpp/GameState/BlasterGameState.h"
#include "TacticalStrategyCpp/PlayerStart/TeamsPlayerStart.h"
#include "TacticalStrategyCpp/PlayerState/BlasterPlayerState.h"
#include "TacticalStrategyCpp/Weapon/Weapon.h"

ABlasterCharacter::ABlasterCharacter():
	bDisableGameplay(false),
	CameraBoom(nullptr),
	FollowCamera(nullptr),
	bIsCrouchButtonToggle(true),
	TurningInPlace(ETurningInPlace::ETIP_NotTurning),
	TurnThreshold(0.5f),
	FireMontage_Hip("RifleHip"),
	FireMontage_Aim("RifleAim"),
	CameraThreshold(200.f),
	MaxHealth(100.f),
	Health(100.f),
	bElimmed(false),
	ElimDelay(3.f),
	DissolveMaterialParam("Dissolve"),
	WeaponGrabbingHandSocket("hand_r"),
	GrenadeSocket("GrenadeSocket")
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

	// Network settings for optimized update rates
	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;
	
	// Ensure character spawns even if spawn locations are occupied
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// Setup Camera Boom for smoother camera movement
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 350.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0, 75.f, 75.f);

	// Attach Follow Camera to Camera Boom for player viewpoint
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Setup overhead widget to display relevant UI (e.g., player name)
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	// Initialize and replicate Combat Component
	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	Combat->SetIsReplicated(true);

	// Initialize and replicate Buff Component
	Buff = CreateDefaultSubobject<UBuffComponent>(TEXT("BuffComponent"));
	Buff->SetIsReplicated(true);

	// Initialize Lag Compensation Component for server-side rewind
	LagCompensation = CreateDefaultSubobject<ULagCompensationComponent>(TEXT("LagCompensationComp"));

	// Setup Timeline for dissolve effect when character is eliminated
	DissolveTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("DissolveTimelineComp"));

	// Spawn location for elimination bot effect (appears when character is eliminated)
	ElimBotSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ElimBotSpawnPoint"));
	ElimBotSpawnLocation->SetupAttachment(RootComponent);
	ElimBotSpawnLocation->SetRelativeLocation(FVector(0, 0, 200));

	// Configure Character Movement for crouching, jumping, and rotation
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->SetCrouchedHalfHeight(60.f);
	CharacterMovementComponent->MaxWalkSpeedCrouched = 450.f;
	CharacterMovementComponent->bOrientRotationToMovement = true;
	CharacterMovementComponent->NavAgentProps.bCanCrouch = true;
	CharacterMovementComponent->GravityScale = 1.7f;
	CharacterMovementComponent->JumpZVelocity = 850.f;
	CharacterMovementComponent->RotationRate = FRotator(0.f, 850.f, 0.f);

	// Configure collision responses to avoid unnecessary interactions with camera
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	// Attach grenade mesh to character with predefined socket
	AttachedGrenade = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AttachedGrenade"));
	AttachedGrenade->SetupAttachment(GetMesh(), FName(GrenadeSocket));
	AttachedGrenade->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttachedGrenade->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Setup hitboxes for server-side rewind
	SetupBoxComponent(HeadBox, "HeadBox", HeadBoxBone);	
	SetupBoxComponent(PelvisBox, "PelvisBox", PelvisBoxBone);
	SetupBoxComponent(Spine_02Box, "Spine_02Box", Spine_02BoxBone);
	SetupBoxComponent(Spine_03Box, "Spine_03Box", Spine_03BoxBone);
	SetupBoxComponent(UpperArm_LBox, "UpperArm_LBox", UpperArm_LBoxBone);
	SetupBoxComponent(UpperArm_RBox, "UpperArm_RBox", UpperArm_RBoxBone);
	SetupBoxComponent(LowerArm_LBox, "LowerArm_LBox", LowerArm_LBoxBone);
	SetupBoxComponent(LowerArm_RBox, "LowerArm_RBox", LowerArm_RBoxBone);
	SetupBoxComponent(Hand_LBox, "Hand_LBox", Hand_LBoxBone);
	SetupBoxComponent(Hand_RBox, "Hand_RBox", Hand_RBoxBone);
	SetupBoxComponent(BackPackBox, "BackPackBox", BackPackBoxBone);
	SetupBoxComponent(BlanketBox, "BlanketBox", BlanketBoxBone);
	SetupBoxComponent(Thigh_LBox, "Thigh_LBox", Thigh_LBoxBone);
	SetupBoxComponent(Thigh_RBox, "Thigh_RBox", Thigh_RBoxBone);
	SetupBoxComponent(Calf_LBox, "Calf_LBox", Calf_LBoxBone);
	SetupBoxComponent(Calf_RBox, "Calf_RBox", Calf_RBoxBone);
	SetupBoxComponent(Foot_LBox, "Foot_LBox", Foot_LBoxBone);
	SetupBoxComponent(Foot_RBox, "Foot_RBox", Foot_RBoxBone);	
}

void ABlasterCharacter::SetupBoxComponent(UBoxComponent*& CompToSetup, const FString& Name, const FString& BoneToAttachTo)
{
	// Initialize and configure hitbox component with specified name and bone attachment
	CompToSetup = CreateDefaultSubobject<UBoxComponent>(FName(Name));
	CompToSetup->SetupAttachment(GetMesh(), FName(BoneToAttachTo));
	CompToSetup->SetCollisionObjectType(ECC_HitBox);
	CompToSetup->SetCollisionResponseToChannels(ECR_Ignore);
	CompToSetup->SetCollisionResponseToChannel(ECC_HitBox, ECR_Block);
	CompToSetup->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Add hitbox component to the hit collision map for easy access
	HitCollisionBoxes.Add(FName(BoneToAttachTo), CompToSetup);
}

void ABlasterCharacter::SetSpawnPoint()
{
	// Set character spawn point based on team affiliation (if any)
	if(HasAuthority() && BlasterPlayerState && BlasterPlayerState->GetTeam() != ETeam::ET_NoTeam)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, ATeamsPlayerStart::StaticClass(), PlayerStarts);

		// Filter player start points based on team
		TArray<ATeamsPlayerStart*> TeamPlayerStarts;
		for (const auto Start : PlayerStarts)
		{
			ATeamsPlayerStart* TeamStart = Cast<ATeamsPlayerStart>(Start);
			if(TeamStart && TeamStart->Team == BlasterPlayerState->GetTeam())
				TeamPlayerStarts.Add(TeamStart);
		}

		// Randomly select a spawn point within the team’s available points
		if(TeamPlayerStarts.Num() > 0)
		{
			const int32 Selection = FMath::RandRange(0, TeamPlayerStarts.Num() - 1);
			ATeamsPlayerStart* ChosenStart = TeamPlayerStarts[Selection];
			SetActorLocationAndRotation(ChosenStart->GetActorLocation(), ChosenStart->GetActorRotation());
		}
	}
}

void ABlasterCharacter::OnRep_ReplicatedMovement()
{
	Super::OnRep_ReplicatedMovement();

	// Handle rotation for simulated proxies
	SimProxiesTurn();

	// Reset time since last replication
	TimeSinceLastMovementReplication = 0;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void ABlasterCharacter::DropOrDestroyWeapon(AWeapon* Weapon)
{
	if(Weapon == nullptr) return;

	// Check if the weapon should be destroyed or simply dropped
	if(Weapon->bDestroyWeapon)
		Weapon->Destroy();
	else
		Weapon->Dropped();
}

void ABlasterCharacter::Elim(bool bPlayerLeftGame)
{
	// If there are equipped items, drop or destroy them upon elimination
	if(Combat)
	{
		if(Combat->EquippedWeapon)
			DropOrDestroyWeapon(Combat->EquippedWeapon);
		if(Combat->SecondaryWeapon)
			DropOrDestroyWeapon(Combat->SecondaryWeapon);
		if(Combat->TheFlag)
			DropOrDestroyWeapon(Combat->TheFlag);
	}

	// Notify all clients about the elimination
	Multicast_Elim(bPlayerLeftGame);
}

void ABlasterCharacter::Multicast_Elim_Implementation(bool bPlayerLeftGame)
{
	bLeftGame = bPlayerLeftGame;

	// Update HUD to reflect zero ammo upon elimination
	if(BlasterPlayerController)
		BlasterPlayerController->SetHudWeaponAmmo(0);

	bElimmed = true; // Mark character as eliminated
	PlayElimMontage(); // Play elimination animation

	StartDissolve(); // Begin dissolve effect

	// Disable character movement
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->DisableMovement();
	CharacterMovementComponent->StopMovementImmediately();

	// Disable certain gameplay elements without fully disabling input
	bDisableGameplay = true;
	if(Combat)
		Combat->FireButtonPressed(false);

	// Disable collision for eliminated character
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttachedGrenade->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Spawn visual effects (ElimBot) at predefined location
	const FVector ElimBotSpawnPoint = ElimBotSpawnLocation->GetComponentLocation();
	if(ElimBotEffect)
	{
		const FRotator ElimBotSpawnRotation = ElimBotSpawnLocation->GetComponentRotation();
		ElimBotComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ElimBotEffect, ElimBotSpawnPoint, ElimBotSpawnRotation);
	}

	// Play elimination sound at the character's location
	if(ElimBotSound)
		UGameplayStatics::PlaySoundAtLocation(this, ElimBotSound, ElimBotSpawnPoint);

	// Hide sniper scope widget if the character was aiming with a sniper rifle
	if(IsLocallyControlled() && Combat && Combat->bAiming && Combat->EquippedWeapon && Combat->EquippedWeapon->GetWeaponType() == EWeaponType::EWT_SniperRifle)
		ShowSniperScopeWidget(false);

	// Destroy crown component if it exists
	if(CrownComponent) CrownComponent->DestroyComponent();

	// Set a timer to handle additional elimination steps (e.g., respawning) after a delay
	GetWorldTimerManager().SetTimer(ElimTimer, this, &ABlasterCharacter::ElimTimerFinished, ElimDelay);
}

void ABlasterCharacter::Destroyed()
{
	// Clean up components and references when character is destroyed
	Super::Destroyed();

	if(ElimBotComp)
		ElimBotComp->DestroyComponent();

	// Check if server
	BlasterGameMode = BlasterGameMode == nullptr ? GetWorld()->GetAuthGameMode<ABlasterGameMode>() : BlasterGameMode;
	if(BlasterGameMode == nullptr) return;

	// Destroy equipped weapon if the match is not in progress
	const bool bMatchNotInProgress = BlasterGameMode->GetMatchState() != MatchState::InProgress;
	if(bMatchNotInProgress && Combat && Combat->EquippedWeapon)
		Combat->EquippedWeapon->Destroy();
}

void ABlasterCharacter::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if(World == nullptr) return;

	// Check if server
	BlasterGameMode = BlasterGameMode == nullptr ? GetWorld()->GetAuthGameMode<ABlasterGameMode>() : BlasterGameMode;
	if(BlasterGameMode && !bElimmed && DefaultWeaponClass)
	{
		// Spawn the default weapon for the character at the start of the game
		AWeapon* StartingWeapon = World->SpawnActor<AWeapon>(DefaultWeaponClass);
		StartingWeapon->bDestroyWeapon = true; // Set to destroy weapon on drop

		// Equip the starting weapon
		if(Combat)
			Combat->EquipWeapon(StartingWeapon);
	}
}
// Handles gaining the lead, spawning the crown effect
void ABlasterCharacter::Multicast_GainedLead_Implementation()
{
	if(CrownSystem == nullptr) return;

	// Spawn the crown if it does not already exist
	if(CrownComponent == nullptr)
		CrownComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(CrownSystem, GetMesh(), FName(),
		GetActorLocation() + FVector(0.f, 0.f, 110.f), 
			GetActorRotation(), EAttachLocation::KeepWorldPosition, false);

	if(CrownComponent)
		CrownComponent->Activate();
}

// Handles losing the lead by removing the crown effect
void ABlasterCharacter::Multicast_LostLead_Implementation()
{
	if(CrownComponent)
		CrownComponent->DestroyComponent();
}

// Sets the team color for the character based on selection
void ABlasterCharacter::SetTeamColor(const ETeam Team) const
{
	if(GetMesh() == nullptr) return;

	switch (Team)
	{
	case ETeam::ET_NoTeam:
		break;
	case ETeam::ET_Red:
		if(RedDissolveMaterial)
			GetMesh()->SetMaterial(0, RedDissolveMaterial);
		break;
	case ETeam::ET_Blue:
		if(BlueDissolveMaterial)
			GetMesh()->SetMaterial(0, BlueDissolveMaterial);
		break;
	default: ;
	}
}

// Sets up the character when the game begins
void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Spawns the default weapon for the character
	SpawnDefaultWeapon();

	// Bind to events only on server
	if(HasAuthority())
		OnTakeAnyDamage.AddDynamic(this, &ABlasterCharacter::ReceiveDamage);

	// Hide the grenade initially
	if(AttachedGrenade) AttachedGrenade->SetVisibility(false);

	// Initialize HUD components
	UpdateHudHealth();
	UpdateHudShield();
	UpdateHudGrenade();
	UpdateHudAmmo();
}

// Bind input actions and axes for the character
void ABlasterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABlasterCharacter::Jump);
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ABlasterCharacter::EquipButtonPressed);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ABlasterCharacter::CrouchButtonPressed);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ABlasterCharacter::CrouchButtonReleased);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ABlasterCharacter::AimButtonPressed);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ABlasterCharacter::AimButtonReleased);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABlasterCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABlasterCharacter::FireButtonReleased);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ABlasterCharacter::ReloadButtonPressed);
	PlayerInputComponent->BindAction("ThrowGrenade", IE_Pressed, this, &ABlasterCharacter::GrenadeButtonPressed);
	PlayerInputComponent->BindAxis("MoveForward", this, &ABlasterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABlasterCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ABlasterCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ABlasterCharacter::LookUp);
}

void ABlasterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Only replicate overlapping weapon to the owning player
	DOREPLIFETIME_CONDITION(ABlasterCharacter, OverlappingWeapon, COND_OwnerOnly);

	DOREPLIFETIME(ABlasterCharacter, Health);
	DOREPLIFETIME(ABlasterCharacter, Shield);
	DOREPLIFETIME(ABlasterCharacter, bDisableGameplay);
}

// Initializes components after they are created
void ABlasterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(Combat) Combat->Character = this;

	// Setup Buff component properties
	if(Buff)
	{
		Buff->Character = this;
		Buff->SetInitialSpeed(GetCharacterMovement()->MaxWalkSpeed, GetCharacterMovement()->MaxWalkSpeedCrouched);
		Buff->SetInitialJumpVelocity(GetCharacterMovement()->JumpZVelocity);
	}

	// Setup Lag Compensation component properties
	if(LagCompensation)
	{
		LagCompensation->Character = this;
		if(Controller) LagCompensation->Controller = Cast<ABlasterPlayerController>(Controller);
	}
}

// Plays the firing montage, with sections for aiming or hip-fire
void ABlasterCharacter::PlayFireMontage(const bool bAiming) const
{
	if(Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	if(UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && FireWeaponMontage)
	{
		AnimInstance->Montage_Play(FireWeaponMontage);
		const FName SectionName = bAiming ? FName(FireMontage_Aim) : FName(FireMontage_Hip);
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

// Plays the reload montage with weapon-specific animations
void ABlasterCharacter::PlayReloadMontage() const
{
	if(Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	if(UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && ReloadMontage)
	{
		AnimInstance->Montage_Play(ReloadMontage);
		FName SectionName;
		switch (Combat->EquippedWeapon->GetWeaponType())
		{
			case EWeaponType::EWT_AssaultRifle:
				SectionName = FName("Rifle");
				break;
			case EWeaponType::EWT_RocketLauncher:
				SectionName = FName("RocketLauncher");
				break;
			case EWeaponType::EWT_Pistol:
				SectionName = FName("Pistol");
				break;
			case EWeaponType::EWT_SMG:
				SectionName = FName("Pistol"); // Reusing pistol section :)
				break;
			case EWeaponType::EWT_Shotgun:
				SectionName = FName("Shotgun");
				break;
			case EWeaponType::EWT_SniperRifle:
				SectionName = FName("SniperRifle");
				break;
			case EWeaponType::EWT_GrenadeLauncher:
				SectionName = FName("Rifle"); // Reusing rifle section :)
				break;
			case EWeaponType::EWT_MAX:
				break;
			default: ;
		}
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

// Plays elimination montage when character is eliminated
void ABlasterCharacter::PlayElimMontage() const
{
	if(UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && ElimMontage)
		AnimInstance->Montage_Play(ElimMontage);
}

// Plays throwing grenade montage
void ABlasterCharacter::PlayThrowGrenadeMontage() const
{
	if(UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && ThrowGrenadeMontage)
		AnimInstance->Montage_Play(ThrowGrenadeMontage);
}

// Plays weapon swap montage
void ABlasterCharacter::PlayWeaponSwapMontage() const
{
	if(UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && WeaponSwapMontage)
		AnimInstance->Montage_Play(WeaponSwapMontage);
}

// Plays hit reaction montage when the character takes damage
void ABlasterCharacter::PlayHitReactMontage() const
{
	if(Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	if(UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		const FName SectionName = FName("FromFront");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

// Updates the health UI
void ABlasterCharacter::UpdateHudHealth()
{
	BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(Controller) : BlasterPlayerController;
	if(BlasterPlayerController)
		BlasterPlayerController->SetHudHealth(Health, MaxHealth);
}

// Updates the shield UI
void ABlasterCharacter::UpdateHudShield()
{
	BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(Controller) : BlasterPlayerController;
	if(BlasterPlayerController)
		BlasterPlayerController->SetHudShield(Shield, MaxShield);
}

// Updates the grenade UI
void ABlasterCharacter::UpdateHudGrenade()
{
	BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(Controller) : BlasterPlayerController;
	if(BlasterPlayerController && Combat)
		BlasterPlayerController->SetHudGrenades(Combat->Grenades);
}

// Updates the corresponding ammo UI
void ABlasterCharacter::UpdateHudAmmo()
{
	BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(Controller) : BlasterPlayerController;
	if(BlasterPlayerController && Combat && Combat->EquippedWeapon)
	{
		BlasterPlayerController->SetHudCarriedAmmo(Combat->CarriedAmmo);
		BlasterPlayerController->SetHudWeaponAmmo(Combat->EquippedWeapon->GetAmmo());
	}
}

// Called when health is replicated, updates UI and plays hit react if damaged
void ABlasterCharacter::OnRep_Health(float LastHealth)
{
	UpdateHudHealth();
	if(Health < LastHealth) PlayHitReactMontage();
}

// Called when shield is replicated, updates UI
void ABlasterCharacter::OnRep_Shield(float LastShield)
{
	UpdateHudShield();
}

// Called when character takes damage, manages health and shield adjustments
void ABlasterCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	BlasterGameMode = BlasterGameMode == nullptr ? GetWorld()->GetAuthGameMode<ABlasterGameMode>() : BlasterGameMode;
	if(bElimmed || BlasterGameMode == nullptr) return;

	// Calculate adjusted damage based on game mode settings(MatchType)
	Damage = BlasterGameMode->CalculateDamage(InstigatorController, Controller, Damage);

	// Handle damage absorption by shield if present
	float DamageToHealth = Damage;
	const float PrevShield = Shield;
	if(Shield > 0)
	{
		if(Shield >= Damage)
		{
			Shield = FMath::Clamp(Shield - Damage, 0, MaxShield);
			DamageToHealth = 0;
		}
		else
		{
			DamageToHealth = FMath::Clamp(Damage - Shield, 0, Damage);
			Shield = 0;
		}
	}
	OnRep_Shield(PrevShield);

	// Apply remaining damage to health
	const float PrevHealth = Health;
	Health = FMath::Clamp(Health - DamageToHealth, 0.f, MaxHealth);
	OnRep_Health(PrevHealth);

	// Handle player elimination if health reaches zero
	if(Health == 0.f && BlasterGameMode)
	{
		BlasterPlayerController = BlasterPlayerController == nullptr ? Cast<ABlasterPlayerController>(Controller) : BlasterPlayerController;
		ABlasterPlayerController* AttackerController = Cast<ABlasterPlayerController>(InstigatorController);
		BlasterGameMode->PlayerEliminated(this, BlasterPlayerController, AttackerController);
	}
}

// Checks if the character’s PlayerState has initialized, then sets team color and spawn point
void ABlasterCharacter::PollInit()
{
	if(BlasterPlayerState == nullptr)
	{
		BlasterPlayerState = GetPlayerState<ABlasterPlayerState>();
		if(BlasterPlayerState)
		{
			BlasterPlayerState->AddToScore(0, true);
			BlasterPlayerState->AddToDefeats(0, true);

			ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
			if(BlasterGameState && BlasterGameState->TopScoringPlayers.Contains(BlasterPlayerState))
				Multicast_GainedLead();

			OnPollInit();
		}
	}
}

// Sets team color and spawn point on initialization
void ABlasterCharacter::OnPollInit()
{
	if(BlasterPlayerState)
	{
		SetTeamColor(BlasterPlayerState->GetTeam());
		SetSpawnPoint();
	}
	K2_OnPollInit(); // Calls Blueprint event
}

// Calculates and applies forward movement based on controller yaw
void ABlasterCharacter::MoveForward(const float Value)
{
	if(bDisableGameplay || Controller == nullptr || Value == 0.f) return;

	const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
	const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));

	AddMovementInput(Direction, Value);
}

// Calculates and applies right movement based on controller yaw
void ABlasterCharacter::MoveRight(const float Value)
{
	if(bDisableGameplay || Controller == nullptr || Value == 0.f) return;

	const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
	const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));

	AddMovementInput(Direction, Value);
}

// Adds yaw input for turning
void ABlasterCharacter::Turn(const float Value)
{
	AddControllerYawInput(Value);
}

// Adds pitch input for looking up
void ABlasterCharacter::LookUp(const float Value)
{
	AddControllerPitchInput(Value);
}

// Handles equip action when the button is pressed
void ABlasterCharacter::EquipButtonPressed()
{
	if(bDisableGameplay || Combat == nullptr || Combat->bHoldingFlag) return;

	if(Combat->CombatState == ECombatState::ECS_Unoccupied) Server_EquipButtonPressed();
	if(Combat->ShouldSwapWeapons() && !HasAuthority() && Combat->CombatState == ECombatState::ECS_Unoccupied && OverlappingWeapon == nullptr)
	{
		Combat->CombatState = ECombatState::ECS_SwappingWeapons;
		PlayWeaponSwapMontage();
		bFinishedSwapping = false;
	}
}

// Handles crouch toggle when the button is pressed
void ABlasterCharacter::CrouchButtonPressed()
{
	if(Combat && Combat->bHoldingFlag) return;

	if(bIsCrouchButtonToggle && bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

// Handles uncrouch if crouch button is released and not in toggle mode
void ABlasterCharacter::CrouchButtonReleased()
{
	if(Combat && Combat->bHoldingFlag) return;
	if(!bIsCrouchButtonToggle)
		UnCrouch();
}

// ReSharper disable once CppMemberFunctionMayBeConst
// Handles reload action when the reload button is pressed
void ABlasterCharacter::ReloadButtonPressed()
{
	if(bDisableGameplay || Combat == nullptr || Combat->bHoldingFlag) return;
	Combat->Reload();
}

// ReSharper disable once CppMemberFunctionMayBeConst
// Handles aiming when aim button is pressed
void ABlasterCharacter::AimButtonPressed()
{
	if(bDisableGameplay || Combat == nullptr || Combat->bHoldingFlag) return;
	Combat->SetAiming(true);
	SetAimingSharpness(true);
}

// ReSharper disable once CppMemberFunctionMayBeConst
// Handles stopping aim when aim button is released
void ABlasterCharacter::AimButtonReleased()
{
	if(bDisableGameplay || Combat == nullptr || Combat->bHoldingFlag) return;
	Combat->SetAiming(false);
	SetAimingSharpness(false);
}

// ReSharper disable once CppMemberFunctionMayBeConst
// Handles grenade throwing when grenade button is pressed
void ABlasterCharacter::GrenadeButtonPressed()
{
	if(Combat && !Combat->bHoldingFlag)
		Combat->ThrowGrenade();
}

// Calculates the aim offset pitch
void ABlasterCharacter::CalculateAoPitch()
{
	AoPitch = GetBaseAimRotation().Pitch;
	if(AoPitch > 90.f && !IsLocallyControlled())
	{
		// Map pitch values from 270-360 to -90-0 for network compression compatibility
		const FVector2D InRange(270.f, 360.f), OutRange(-90.f, 0.f);
		AoPitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AoPitch);
	}
}

// Calculates and returns movement speed, ignoring vertical velocity
float ABlasterCharacter::CalculateSpeed() const
{
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	return Velocity.Size();
}

// Handles aim offset, controlling rotation and turn-in-place logic
void ABlasterCharacter::AimOffset(const float DeltaTime)
{
	if(Combat && Combat->EquippedWeapon == nullptr) return;

	const float Speed = CalculateSpeed();
	const bool bIsInAir = GetCharacterMovement()->IsFalling();

	// Handles aim offset when standing still on the ground
	if(Speed == 0.f && !bIsInAir)
	{
		bRotateRootBone = true;

		const FRotator CurrentAimRotation(0.f, GetBaseAimRotation().Yaw, 0.f);
		const FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);

		AoYaw = DeltaAimRotation.Yaw;

		// Set interpolation for yaw rotation when not turning in place
		if(TurningInPlace == ETurningInPlace::ETIP_NotTurning)
		{
			InterpAoYaw = AoYaw;
		}

		bUseControllerRotationYaw = true;
		TurnInPlace(DeltaTime);
	}
	// Reset rotation settings while moving or in air
	else if(Speed > 0.f || bIsInAir)
	{
		bRotateRootBone = false;
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AoYaw = 0.f;
		bUseControllerRotationYaw = true;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	}

	CalculateAoPitch();
}

// Handles simulated proxies rotation for smooth turn-in-place
void ABlasterCharacter::SimProxiesTurn()
{
	if(Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	bRotateRootBone = false;

	// Reset turning state if character is moving
	if(CalculateSpeed() > 0.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		return;
	}

	// Calculate the yaw difference to determine turn direction
	ProxyRotationLastFrame = ProxyRotation;
	ProxyRotation = GetActorRotation();
	ProxyYaw = UKismetMathLibrary::NormalizedDeltaRotator(ProxyRotation, ProxyRotationLastFrame).Yaw;

	// Set turn direction based on yaw difference
	if(FMath::Abs(ProxyYaw) > TurnThreshold)
		TurningInPlace = (ProxyYaw > TurnThreshold) ? ETurningInPlace::ETIP_Right : ETurningInPlace::ETIP_Left;
	else
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
}

// Overriding Jump function to handle crouch toggle
void ABlasterCharacter::Jump()
{
	// if(bDisableGameplay) return;
	if(Combat && Combat->bHoldingFlag) return;

	if(bIsCrouched)
		UnCrouch();
	else
		Super::Jump();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ABlasterCharacter::FireButtonPressed()
{
	if (bDisableGameplay || (Combat && Combat->bHoldingFlag)) return;

	if(Combat) Combat->FireButtonPressed(true);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ABlasterCharacter::FireButtonReleased()
{
	if(bDisableGameplay || (Combat && Combat->bHoldingFlag)) return;

	if(Combat) Combat->FireButtonPressed(false);
}

// Manages the display of weapon pickup widgets based on overlap status
void ABlasterCharacter::OnRep_OverlappingWeapon(const AWeapon* LastWeapon) const
{
	if(LastWeapon) LastWeapon->ShowPickupWidget(false);
	if(OverlappingWeapon) OverlappingWeapon->ShowPickupWidget(true);
}

// Handles server-side equip button functionality
void ABlasterCharacter::Server_EquipButtonPressed_Implementation()
{
	if(Combat)
	{
		if(OverlappingWeapon)
			Combat->EquipWeapon(OverlappingWeapon);
		else if(Combat->ShouldSwapWeapons())
			Combat->SwapWeapons();
	}
}

// Smoothly interpolates aim offset yaw when turning in place
void ABlasterCharacter::TurnInPlace(float DeltaTime)
{
	if(AoYaw > 90.f)
		TurningInPlace = ETurningInPlace::ETIP_Right;
	else if(AoYaw < -90.f)
		TurningInPlace = ETurningInPlace::ETIP_Left;

	// Interpolates yaw back to zero when turning in place
	if(TurningInPlace != ETurningInPlace::ETIP_NotTurning)
	{
		InterpAoYaw = FMath::FInterpTo(InterpAoYaw, 0.f, DeltaTime, 4.f);
		AoYaw = InterpAoYaw;
		if(FMath::Abs(AoYaw) < 15.f)
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
			StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
}

// Adjusts aiming sharpness (depth of field) for improved visual feedback
void ABlasterCharacter::SetAimingSharpness(const bool bIsAiming) const
{
	if(FollowCamera == nullptr) return;

	FollowCamera->PostProcessSettings.DepthOfFieldFocalDistance = bIsAiming ? 10000 : 0; // Focal Distance
	FollowCamera->PostProcessSettings.DepthOfFieldFstop = bIsAiming ? 32 : 4; // Aperture F-Stop
}

// Hides character and weapon meshes when the camera is too close
void ABlasterCharacter::HideCameraOnCharacterClose() const
{
	if(!IsLocallyControlled()) return;

	bool bHide = (FollowCamera->GetComponentLocation() - GetActorLocation()).Size() < CameraThreshold;
	GetMesh()->SetVisibility(!bHide);

	// Hide weapon meshes if within threshold distance
	if (Combat)
	{
		if(Combat->EquippedWeapon && Combat->EquippedWeapon->GetSkeletalWeaponMesh()) Combat->EquippedWeapon->GetSkeletalWeaponMesh()->bOwnerNoSee = bHide;
		if(Combat->SecondaryWeapon && Combat->SecondaryWeapon->GetSkeletalWeaponMesh()) Combat->SecondaryWeapon->GetSkeletalWeaponMesh()->bOwnerNoSee = bHide;
		if(Combat->TheFlag && Combat->TheFlag->GetStaticWeaponMesh()) Combat->TheFlag->GetStaticWeaponMesh()->bOwnerNoSee = bHide;
	}
}

// Called when elimination timer finishes, handles respawn or leave game actions
void ABlasterCharacter::ElimTimerFinished()
{
	BlasterGameMode = BlasterGameMode == nullptr ? GetWorld()->GetAuthGameMode<ABlasterGameMode>() : BlasterGameMode;
	if(BlasterGameMode && !bLeftGame)
		BlasterGameMode->RequestRespawn(this, Controller);
	if(bLeftGame && IsLocallyControlled())
		OnLeftGame.Broadcast();
}

// Handles server-side logic for leaving the game
void ABlasterCharacter::Server_LeaveGame_Implementation()
{
	BlasterGameMode = BlasterGameMode == nullptr ? GetWorld()->GetAuthGameMode<ABlasterGameMode>() : BlasterGameMode;
	BlasterPlayerState = BlasterPlayerState == nullptr ? GetPlayerState<ABlasterPlayerState>() : BlasterPlayerState;
	if(BlasterGameMode && BlasterPlayerState)
		BlasterGameMode->PlayerLeftGame(BlasterPlayerState);
}

// ReSharper disable once CppMemberFunctionMayBeConst
// Updates dissolve material effect based on dissolve progress
void ABlasterCharacter::UpdateDissolveMaterial(const float DissolveValue)
{
	GetMesh()->SetScalarParameterValueOnMaterials(FName(DissolveMaterialParam), DissolveValue);
}

// Starts the dissolve effect using a timeline
void ABlasterCharacter::StartDissolve()
{
	DissolveTrack.BindDynamic(this, &ABlasterCharacter::UpdateDissolveMaterial);
	if(DissolveCurve && DissolveTimeLine)
	{
		DissolveTimeLine->AddInterpFloat(DissolveCurve, DissolveTrack);
		DissolveTimeLine->PlayFromStart();
	}
}

// Sets the weapon that the character is overlapping, updating the UI as necessary
void ABlasterCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
	const AWeapon* LastWeapon = OverlappingWeapon;
	OverlappingWeapon = Weapon;

	if(IsLocallyControlled())
		OnRep_OverlappingWeapon(LastWeapon);
}

// Checks if the character currently has a weapon equipped
bool ABlasterCharacter::IsWeaponEquipped() const
{
	return Combat && Combat->EquippedWeapon;
}

// Checks if the character is currently aiming
bool ABlasterCharacter::IsAiming() const
{
	return Combat && Combat->bAiming;
}

// Gets the currently equipped weapon
AWeapon* ABlasterCharacter::GetEquippedWeapon() const
{
	return Combat ? Combat->EquippedWeapon : nullptr;
}

// Retrieves the character's hit target from the combat component
FVector ABlasterCharacter::GetHitTarget() const
{
	return Combat ? Combat->HitTarget : FVector();
}

// Retrieves the current combat state from the combat component
ECombatState ABlasterCharacter::GetCombatState() const
{
	return Combat ? Combat->CombatState : ECombatState::ECS_Max;
}

// Checks if the character is locally reloading
bool ABlasterCharacter::IsLocallyReloading() const
{
	return Combat && Combat->bLocallyReloading;
}

// Checks if the character is holding the flag
bool ABlasterCharacter::IsHoldingFlag() const
{
	return Combat && Combat->bHoldingFlag;
}

// Retrieves the character's team from the player state
ETeam ABlasterCharacter::GetTeam()
{
	BlasterPlayerState = BlasterPlayerState == nullptr ? GetPlayerState<ABlasterPlayerState>() : BlasterPlayerState;
	return BlasterPlayerState ? BlasterPlayerState->GetTeam() : ETeam::ET_NoTeam;
}

// Updates flag holding status
void ABlasterCharacter::SetHoldingFlag(const bool bHolding) const
{
	if(Combat == nullptr) return;

	Combat->bHoldingFlag = bHolding;
	Combat->TheFlag = nullptr;
	if(HasAuthority())
		Combat->OnRep_TheFlag();
}

// Controls root rotation based on movement state and aim offset
void ABlasterCharacter::RotateInPlace(const float DeltaTime)
{
	if(Combat && Combat->bHoldingFlag)
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		return;
	}

	// Enable root rotation when equipped with a weapon and gameplay is enabled
	if(Combat && Combat->EquippedWeapon)
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	if(bDisableGameplay)
	{
		bUseControllerRotationYaw = false;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		return;
	}

	// Determine aim offset for local or simulated players
	if(GetLocalRole() > ROLE_SimulatedProxy && IsLocallyControlled())
		AimOffset(DeltaTime);
	else
	{
		TimeSinceLastMovementReplication += DeltaTime;
		if(TimeSinceLastMovementReplication > 0.25f)
			OnRep_ReplicatedMovement();
		CalculateAoPitch();
	}
}

// Main Tick function, handles rotation and camera each frame
void ABlasterCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateInPlace(DeltaTime);
	HideCameraOnCharacterClose();
	PollInit();
}
