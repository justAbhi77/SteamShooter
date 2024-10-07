
#include "BlasterPlayerController.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "TacticalStrategyCpp/BlasterComponents/CombatComponent.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"
#include "TacticalStrategyCpp/GameMode/BlasterGameMode.h"
#include "TacticalStrategyCpp/GameState/BlasterGameState.h"
#include "TacticalStrategyCpp/Hud/Announcement.h"
#include "TacticalStrategyCpp/Hud/BlasterHud.h"
#include "TacticalStrategyCpp/Hud/CharacterOverlay.h"
#include "TacticalStrategyCpp/PlayerState/BlasterPlayerState.h"

ABlasterPlayerController::ABlasterPlayerController():
	ClientServerDelta(0),
	TimeSyncFrequency(5),
	BlasterHud(nullptr),
	BlasterGameMode(nullptr),
	MatchTime(0),
	WarmUpTime(0),
	LevelStartingTime(0),
	CooldownTime(0),
	CountDownInt(0),
	CharacterOverlay(nullptr),
	HudHealth(0),
	HudMaxHealth(0),
	HudScore(0), HudShield(0), HudMaxShield(0), HudCarriedAmmo(0), HudWeaponAmmo(0),
	HudDefeats(0), HudGrenades(0)
{
}

void ABlasterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	BlasterHud = Cast<ABlasterHud>(GetHUD());

	ServerCheckMatchState();
}

void ABlasterPlayerController::SetHudTime()
{
	const double MiliSecondsLeft = MatchTime - GetServerTime();

	float TimeLeft = 0;

	if(MatchState == MatchState::WaitingToStart)
		TimeLeft = WarmUpTime - GetServerTime() + LevelStartingTime;
	else if(MatchState == MatchState::InProgress)
		TimeLeft = WarmUpTime + MiliSecondsLeft + LevelStartingTime;
	else if(MatchState == MatchState::Cooldown)
		TimeLeft = CooldownTime + WarmUpTime + MiliSecondsLeft + LevelStartingTime;

	uint32 SecondsLeft = FMath::CeilToInt(TimeLeft);
	if(HasAuthority())
	{
		BlasterGameMode = BlasterGameMode == nullptr ?
			Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this)): BlasterGameMode;

		if(BlasterGameMode)
		{
			SecondsLeft = FMath::CeilToInt(BlasterGameMode->GetCountdownTime() + LevelStartingTime);
		}
	}
	if(CountDownInt != SecondsLeft)
	{
		if(MatchState == MatchState::WaitingToStart || MatchState == MatchState::Cooldown)
			SetHudAnnouncementCountDown(TimeLeft);
		if(MatchState == MatchState::InProgress)
			SetHudMatchCountDown(TimeLeft);
	}
	CountDownInt = SecondsLeft;
}

void ABlasterPlayerController::PollInit()
{
	if(CharacterOverlay == nullptr)
	{
		if(BlasterHud && BlasterHud->CharacterOverlay)
		{
			CharacterOverlay = BlasterHud->CharacterOverlay;
			if(CharacterOverlay)
			{
				if(bInitializeHealth) SetHudHealth(HudHealth, HudMaxHealth);
				if(bInitializeShields) SetHudShield(HudShield, HudMaxShield);
				if(bInitializeScore) SetHudScore(HudScore);
				if(bInitializeScore) SetHudDefeats(HudDefeats);
				if(bInitializeCarriedAmmo) SetHudCarriedAmmo(HudCarriedAmmo);
				if(bInitializeWeaponAmmo) SetHudWeaponAmmo(HudWeaponAmmo);
				
				ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(GetPawn());
				if(BlasterCharacter && BlasterCharacter->GetCombatComponent())			
					if(bInitializeGrenades) SetHudGrenades(BlasterCharacter->GetCombatComponent()->GetGrenades());
			}
		}
	}
}

void ABlasterPlayerController::ClientJoinMidGame_Implementation(const FName StateOfMatch, const float WarmUp,
	const float Match, const float StartingTime, const float Cooldown)
{
	WarmUpTime = WarmUp;
	MatchTime = Match;
	LevelStartingTime = StartingTime;
	MatchState = StateOfMatch;
	CooldownTime = Cooldown;
	OnMatchStateSet(MatchState);
	
	if(BlasterHud && MatchState == MatchState::WaitingToStart)
		BlasterHud->AddAnnouncement();
}

void ABlasterPlayerController::ServerCheckMatchState_Implementation()
{
	if(const ABlasterGameMode* GameMode = Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		WarmUpTime = GameMode->WarmUpTime;
		MatchTime = GameMode->MatchTime;
		LevelStartingTime = GameMode->LevelStartingTime;
		MatchState = GameMode->GetMatchState();
		CooldownTime = GameMode->CooldownTime;
		ClientJoinMidGame(MatchState, WarmUpTime, MatchTime, LevelStartingTime, CooldownTime);
		
		/*
		if(BlasterHud && MatchState == MatchState::WaitingToStart && IsLocalController())
			BlasterHud->AddAnnouncement();
			*/		
	}
}

void ABlasterPlayerController::OnRep_MatchState()
{
	if(MatchState == MatchState::InProgress)
	{		
		BlasterHud = BlasterHud == nullptr ? Cast<ABlasterHud>(GetHUD()) : BlasterHud;
		if(BlasterHud == nullptr) return;

		BlasterHud->AddCharacterOverlay();
		if(BlasterHud->Announcement)
		{
			BlasterHud->Announcement->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else if(MatchState == MatchState::Cooldown)
		HandleCooldown();
}

void ABlasterPlayerController::Server_RequestServerTime_Implementation(float TimeOfClientRequest)
{
	const float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds();
	Client_ReportServerTime(TimeOfClientRequest, ServerTimeOfReceipt);
}

void ABlasterPlayerController::Client_ReportServerTime_Implementation(const float TimeOfClientRequest,
                                                                      float TimeOfServerReceivedClientRequest)
{
	const float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	const float CurrentServerTime = TimeOfServerReceivedClientRequest + (0.5f * RoundTripTime);

	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}

void ABlasterPlayerController::SetHudHealth(const float Health, const float MaxHealth)
{
	BlasterHud = BlasterHud == nullptr ? Cast<ABlasterHud>(GetHUD()) : BlasterHud;
			
	if(BlasterHud && CharacterOverlay && CharacterOverlay->HealthBar && CharacterOverlay->HealthText)
	{		
		const float HealthPercent = Health/MaxHealth;
		CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		const FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
	else
	{
		HudHealth = Health;
		HudMaxHealth = MaxHealth;
		bInitializeHealth = true;
	}
}

void ABlasterPlayerController::SetHudShield(float Shield, float MaxShield)
{
	BlasterHud = BlasterHud == nullptr ? Cast<ABlasterHud>(GetHUD()) : BlasterHud;
			
	if(BlasterHud && CharacterOverlay && CharacterOverlay->ShieldBar && CharacterOverlay->ShieldText)
	{		
		const float ShieldPercent = Shield/MaxShield;
		CharacterOverlay->ShieldBar->SetPercent(ShieldPercent);
		const FString ShieldText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Shield), FMath::CeilToInt(MaxShield));
		CharacterOverlay->ShieldText->SetText(FText::FromString(ShieldText));
	}
	else
	{
		HudShield = Shield;
		HudMaxShield = MaxShield;
		bInitializeShields = true;
	}
}

void ABlasterPlayerController::SetHudScore(const float Score)
{
	BlasterHud = BlasterHud == nullptr ? Cast<ABlasterHud>(GetHUD()) : BlasterHud;
		
	if(BlasterHud && CharacterOverlay && CharacterOverlay->ScoreText)
	{
		const FString Text = FString::Printf(TEXT("%d"), FMath::CeilToInt(Score) );
		CharacterOverlay->ScoreText->SetText(FText::FromString(Text));
	}
	else
	{
		HudScore = Score;
		bInitializeScore = true;
	}
}

void ABlasterPlayerController::SetHudDefeats(int32 Defeats)
{
	BlasterHud = BlasterHud == nullptr ? Cast<ABlasterHud>(GetHUD()) : BlasterHud;
	
	if(BlasterHud && CharacterOverlay && CharacterOverlay->DefeatText)
	{
		const FString Text = FString::Printf(TEXT("%d"), Defeats);
		CharacterOverlay->DefeatText->SetText(FText::FromString(Text));
	}	
	else
	{
		HudDefeats = Defeats;
		bInitializeDefeats = true;
	}
}

void ABlasterPlayerController::SetHudWeaponAmmo(const int32 Ammo)
{
	BlasterHud = BlasterHud == nullptr ? Cast<ABlasterHud>(GetHUD()) : BlasterHud;
		
	if(BlasterHud && CharacterOverlay && CharacterOverlay->WeaponAmmoText)
	{
		const FString Text = FString::Printf(TEXT("%d"), Ammo);
		CharacterOverlay->WeaponAmmoText->SetText(FText::FromString(Text));
	}
	else
	{
		HudWeaponAmmo = Ammo;
		bInitializeWeaponAmmo = true;
	}
}

void ABlasterPlayerController::SetHudCarriedAmmo(int32 Ammo)
{
	BlasterHud = BlasterHud == nullptr ? Cast<ABlasterHud>(GetHUD()) : BlasterHud;
		
	if(BlasterHud && CharacterOverlay && CharacterOverlay->CarriedAmmoText)
	{
		const FString Text = FString::Printf(TEXT("%d"), Ammo);
		CharacterOverlay->CarriedAmmoText->SetText(FText::FromString(Text));
	}
	else
	{
		HudCarriedAmmo = Ammo;
		bInitializeCarriedAmmo = true;
	}
}

void ABlasterPlayerController::SetHudMatchCountDown(const float CountDownTime)
{
	BlasterHud = BlasterHud == nullptr ? Cast<ABlasterHud>(GetHUD()) : BlasterHud;
		
	if(BlasterHud && CharacterOverlay && CharacterOverlay->MatchCountDownText)
	{		
		if(CountDownTime < 0.f)
		{
			CharacterOverlay->MatchCountDownText->SetText(FText());
			return;
		}
		const int32 Min = FMath::FloorToInt(CountDownTime / 60);
		const int32 Seconds = CountDownTime - Min * 60.f;
		const FString Text = FString::Printf(TEXT("%02d:%02d"), Min, Seconds);
		CharacterOverlay->MatchCountDownText->SetText(FText::FromString(Text));
	}
}

void ABlasterPlayerController::SetHudAnnouncementCountDown(const float CountdownTime)
{
	BlasterHud = BlasterHud == nullptr ? Cast<ABlasterHud>(GetHUD()) : BlasterHud;
	if(BlasterHud == nullptr) return;
	
	// ReSharper disable once CppTooWideScopeInitStatement
	const UAnnouncement* Announcement = BlasterHud->Announcement;	
	if(Announcement && Announcement->WarmUpTime)
	{
		if(CountdownTime < 0.f)
		{
			Announcement->WarmUpTime->SetText(FText());
			return;
		}
		const int32 Min = FMath::FloorToInt(CountdownTime / 60);
		const int32 Seconds = CountdownTime - Min * 60.f;
		const FString Text = FString::Printf(TEXT("%02d:%02d"), Min, Seconds);
		Announcement->WarmUpTime->SetText(FText::FromString(Text));
	}
}

void ABlasterPlayerController::SetHudGrenades(int32 Grenades)
{
	BlasterHud = BlasterHud == nullptr ? Cast<ABlasterHud>(GetHUD()) : BlasterHud;
		
	if(BlasterHud != nullptr && CharacterOverlay != nullptr && CharacterOverlay->GrenadesText != nullptr)
	{
		const FString Text = FString::Printf(TEXT("%d"), Grenades);
		CharacterOverlay->GrenadesText->SetText(FText::FromString(Text));
	}
	else
	{
		HudGrenades = Grenades;
		bInitializeGrenades = true;
	}
}

void ABlasterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(InPawn))
	{
		SetHudHealth(BlasterCharacter->GetHealth(), BlasterCharacter->GetMaxHealth());
		SetHudShield(BlasterCharacter->GetShield(), BlasterCharacter->GetMaxShield());
		SetHudGrenades(BlasterCharacter->GetCombatComponent()->GetGrenades());
		BlasterCharacter->UpdateHudAmmo();
	}
}

void ABlasterPlayerController::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	SetHudTime();

	PollInit();
}

float ABlasterPlayerController::GetServerTime()
{
	if(HasAuthority()) return GetWorld()->GetTimeSeconds();
	
	return GetWorld()->GetTimeSeconds() + ClientServerDelta;
}

void ABlasterPlayerController::SendReqSyncServerTime()
{
	Server_RequestServerTime(GetWorld()->GetTimeSeconds());
}

void ABlasterPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if(IsLocalController())
	{
		SendReqSyncServerTime();
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ABlasterPlayerController::SendReqSyncServerTime,
			TimeSyncFrequency, true);
	}
}

void ABlasterPlayerController::OnMatchStateSet(const FName State)
{
	MatchState = State;
	if(HasAuthority())
		OnRep_MatchState();
}

void ABlasterPlayerController::HandleCooldown()
{
	if(ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(GetPawn()))
	{
		BlasterCharacter->bDisableGameplay = true;
		if(UCombatComponent* CombatComponent = BlasterCharacter->GetCombatComponent())
		{
			CombatComponent->FireButtonPressed(false);
		}
	}
	
	BlasterHud = BlasterHud == nullptr ? Cast<ABlasterHud>(GetHUD()) : BlasterHud;	
	if(BlasterHud == nullptr) return;

	BlasterHud->CharacterOverlay->RemoveFromParent();
	
	// ReSharper disable once CppTooWideScopeInitStatement
	UAnnouncement* Announcement = BlasterHud->Announcement;
	if(Announcement && Announcement->AnnouncementText && Announcement->InfoText)
	{
		Announcement->SetVisibility(ESlateVisibility::Visible);

		const FString AnnouncementText{"New Match Starts in: "};
		Announcement->AnnouncementText->SetText(FText::FromString(AnnouncementText));

		FString WinnerText;

		const ABlasterPlayerState* BlasterPlayerState = GetPlayerState<ABlasterPlayerState>();
		// ReSharper disable once CppTooWideScopeInitStatement
		const ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(
					UGameplayStatics::GetGameState(this));
		if(BlasterGameState && BlasterPlayerState)
		{
			TArray<ABlasterPlayerState*> TopPlayers = BlasterGameState->TopScoringPlayers;
			if(TopPlayers.Num() == 0)
				WinnerText = FString("No Winner");
			else if(TopPlayers.Num() == 1 && TopPlayers[0] == BlasterPlayerState)
				WinnerText = FString("You are the Winner! Yay!");
			else if(TopPlayers.Num() == 1)
				WinnerText = FString::Printf(TEXT("Winner: \n%s"), *TopPlayers[0]->GetPlayerName());
			else if(TopPlayers.Num() > 1)
			{
				WinnerText = FString("Players Tied for win: \n");
				for(const auto TiedPlayer : TopPlayers)
				{
					WinnerText.Append(FString::Printf(TEXT("%s\n"), *TiedPlayer->GetPlayerName()));
				}
			}
		}
		
		Announcement->InfoText->SetText(FText::FromString(WinnerText));
	}
}

void ABlasterPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABlasterPlayerController, MatchState);
}
