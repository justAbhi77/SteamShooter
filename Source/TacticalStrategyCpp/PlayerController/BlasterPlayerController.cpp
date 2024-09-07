
#include "BlasterPlayerController.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"
#include "TacticalStrategyCpp/Hud/BlasterHud.h"
#include "TacticalStrategyCpp/Hud/CharacterOverlay.h"

void ABlasterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	BlasterHud = Cast<ABlasterHud>(GetHUD());
}

void ABlasterPlayerController::SetHudHealth(const float Health, const float MaxHealth)
{
	BlasterHud = BlasterHud == nullptr ? Cast<ABlasterHud>(GetHUD()) : BlasterHud;
	if(BlasterHud == nullptr) return;
	
	// ReSharper disable once CppTooWideScopeInitStatement
	const UCharacterOverlay* CharacterOverlay = BlasterHud->CharacterOverlay;
		
	if(CharacterOverlay && CharacterOverlay->HealthBar && CharacterOverlay->HealthText)
	{		
		const float HealthPercent = Health/MaxHealth;
		CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		const FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}	
}

void ABlasterPlayerController::SetHudScore(float Score)
{
	BlasterHud = BlasterHud == nullptr ? Cast<ABlasterHud>(GetHUD()) : BlasterHud;
	if(BlasterHud == nullptr) return;
	
	// ReSharper disable once CppTooWideScopeInitStatement
	const UCharacterOverlay* CharacterOverlay = BlasterHud->CharacterOverlay;
		
	if(CharacterOverlay && CharacterOverlay->ScoreText)
	{
		const FString Text = FString::Printf(TEXT("%d"), FMath::CeilToInt(Score) );
		CharacterOverlay->ScoreText->SetText(FText::FromString(Text));
	}	
}

void ABlasterPlayerController::SetHudDefeats(int32 Defeats)
{
	BlasterHud = BlasterHud == nullptr ? Cast<ABlasterHud>(GetHUD()) : BlasterHud;
	if(BlasterHud == nullptr) return;
	
	// ReSharper disable once CppTooWideScopeInitStatement
	const UCharacterOverlay* CharacterOverlay = BlasterHud->CharacterOverlay;
		
	if(CharacterOverlay && CharacterOverlay->DefeatText)
	{
		const FString Text = FString::Printf(TEXT("%d"), Defeats);
		CharacterOverlay->DefeatText->SetText(FText::FromString(Text));
	}	
}

void ABlasterPlayerController::SetHudWeaponAmmo(const int32 Ammo)
{
	BlasterHud = BlasterHud == nullptr ? Cast<ABlasterHud>(GetHUD()) : BlasterHud;
	if(BlasterHud == nullptr) return;
	
	// ReSharper disable once CppTooWideScopeInitStatement
	const UCharacterOverlay* CharacterOverlay = BlasterHud->CharacterOverlay;
		
	if(CharacterOverlay && CharacterOverlay->WeaponAmmoText)
	{
		const FString Text = FString::Printf(TEXT("%d"), Ammo);
		CharacterOverlay->WeaponAmmoText->SetText(FText::FromString(Text));
	}	
}

void ABlasterPlayerController::SetHudCarriedAmmo(int32 Ammo)
{
	BlasterHud = BlasterHud == nullptr ? Cast<ABlasterHud>(GetHUD()) : BlasterHud;
	if(BlasterHud == nullptr) return;
	
	// ReSharper disable once CppTooWideScopeInitStatement
	const UCharacterOverlay* CharacterOverlay = BlasterHud->CharacterOverlay;
		
	if(CharacterOverlay && CharacterOverlay->CarriedAmmoText)
	{
		const FString Text = FString::Printf(TEXT("%d"), Ammo);
		CharacterOverlay->CarriedAmmoText->SetText(FText::FromString(Text));
	}	
}

void ABlasterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(InPawn))
	{
		SetHudHealth(BlasterCharacter->GetHealth(), BlasterCharacter->GetMaxHealth());
	}
}
