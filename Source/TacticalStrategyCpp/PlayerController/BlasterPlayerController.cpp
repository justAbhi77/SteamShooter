
#include "BlasterPlayerController.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
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
