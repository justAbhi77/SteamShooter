
#include "BlasterPlayerController.h"
#include "TacticalStrategyCpp/Enums/Announcement.h"
#include "Components/HorizontalBox.h"
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
#include "TacticalStrategyCpp/Hud/ReturnToMainMenu.h"
#include "TacticalStrategyCpp/Hud/TeamSelection.h"
#include "TacticalStrategyCpp/Hud/WifiStrength.h"
#include "TacticalStrategyCpp/PlayerState/BlasterPlayerState.h"

ABlasterPlayerController::ABlasterPlayerController():
	BlasterHud(nullptr),
	BlasterGameMode(nullptr),
	PcCharacterOverlay(nullptr),
	HudHealth(0),
	HudMaxHealth(0),
	HudScore(0),
	HudShield(0),
	HudMaxShield(0),
	HudCarriedAmmo(0),
	HudWeaponAmmo(0),
	HudDefeats(0),
	HudGrenades(0),
	MatchTime(0), WarmUpTime(0), LevelStartingTime(0), CooldownTime(0), CountDownInt(0),
	TimeSyncFrequency(5), ClientServerDelta(0), WbpReturnToMenu(nullptr), WbpTeamSelection(nullptr)
{
}

void ABlasterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if(InputComponent == nullptr) return;
	
	InputComponent->BindAction("Quit", IE_Pressed, this, &ABlasterPlayerController::ShowReturnToMenu);
}

// Optimized helper to get Blaster HUD
void ABlasterPlayerController::GetBlasterHud()
{
	if(BlasterHud == nullptr)
		BlasterHud = Cast<ABlasterHud>(GetHUD());
	if((BlasterHud == nullptr) && !BlasterHudCacheTimer.IsValid())
		GetWorldTimerManager().SetTimer(BlasterHudCacheTimer, this, &ABlasterPlayerController::CacheBlasterHud, 0.2f, true);
}

void ABlasterPlayerController::CacheBlasterHud()
{
	if(BlasterHud == nullptr)
		BlasterHud = Cast<ABlasterHud>(GetHUD());
	if (BlasterHud != nullptr)
		BlasterHudCacheTimer.Invalidate();
}

// Helper to show/hide team scores and set initial values
void ABlasterPlayerController::SetTeamScoreVisibility(bool bIsVisible)
{
	GetBlasterHud();
	if(BlasterHud && BlasterHud->CharacterOverlay && BlasterHud->CharacterOverlay->TeamScoreContainer)
	{
		BlasterHud->CharacterOverlay->TeamScoreContainer->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void ABlasterPlayerController::HideTeamScores()
{
	SetTeamScoreVisibility(false);
}

void ABlasterPlayerController::InitTeamScores()
{
	SetTeamScoreVisibility(true);
	if(BlasterHud && BlasterHud->CharacterOverlay && BlasterHud->CharacterOverlay->RedTeamScore)
		BlasterHud->CharacterOverlay->RedTeamScore->SetText(FText::FromString("0"));	
	if(BlasterHud && BlasterHud->CharacterOverlay && BlasterHud->CharacterOverlay->BlueTeamScore)
		BlasterHud->CharacterOverlay->BlueTeamScore->SetText(FText::FromString("0"));
}

void ABlasterPlayerController::SetHudRedTeamScore(const int32 RedScore)
{
    SetTeamScoreVisibility(true);
	if(BlasterHud && BlasterHud->CharacterOverlay && BlasterHud->CharacterOverlay->RedTeamScore)
		BlasterHud->CharacterOverlay->RedTeamScore->SetText(FText::AsNumber(RedScore));
}

void ABlasterPlayerController::SetHudBlueTeamScore(const int32 BlueScore)
{
    SetTeamScoreVisibility(true);
	if(BlasterHud && BlasterHud->CharacterOverlay && BlasterHud->CharacterOverlay->BlueTeamScore)
		BlasterHud->CharacterOverlay->BlueTeamScore->SetText(FText::AsNumber(BlueScore));
}

void ABlasterPlayerController::BroadcastElim(APlayerState* Attacker, APlayerState* Victim)
{
	Client_ElimAnnouncement(Attacker, Victim);
}

// Display elimination announcements on the HUD
void ABlasterPlayerController::Client_ElimAnnouncement_Implementation(APlayerState* Attacker, APlayerState* Victim)
{
    GetBlasterHud();
	APlayerState* Self = GetPlayerState<APlayerState>();
	if(Self && Attacker && Victim && BlasterHud)
	{
		FString AttackerName = (Attacker == Self) ? "You" : Attacker->GetPlayerName();
		FString VictimName = (Victim == Self) ? "You" : Victim->GetPlayerName();

		// Check if the Attacker eliminated themselves
		if(Attacker == Victim)
			if (BlasterHud)
				BlasterHud->AddElimAnnouncement(AttackerName, (Attacker == Self) ? "Yourself" : "themselves");
		// Normal elimination cases
		else
			if (BlasterHud)
			BlasterHud->AddElimAnnouncement(AttackerName, VictimName);
	}
}

void ABlasterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	GetBlasterHud();
	ServerCheckMatchState();
}

// Sync the HUD countdown timer with the server
void ABlasterPlayerController::SetHudTime()
{
	const double MilliSecondsLeft = MatchTime - GetServerTime();
	float TimeLeft = 0;

	if(MatchState == MatchState::WaitingToStart)
		TimeLeft = WarmUpTime - GetServerTime() + LevelStartingTime;
	else if(MatchState == MatchState::InProgress)
		TimeLeft = WarmUpTime + MilliSecondsLeft + LevelStartingTime;
	else if(MatchState == MatchState::MatchInCooldown)
		TimeLeft = CooldownTime + WarmUpTime + MilliSecondsLeft + LevelStartingTime;

	uint32 SecondsLeft = FMath::CeilToInt(TimeLeft);
	if(HasAuthority())
	{
		BlasterGameMode = BlasterGameMode == nullptr ?
			Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this)): BlasterGameMode;

		if(BlasterGameMode)
		{
			LevelStartingTime = BlasterGameMode->LevelStartingTime;
			SecondsLeft = FMath::CeilToInt(BlasterGameMode->GetCountdownTime() + LevelStartingTime);
		}
	}
	if(CountDownInt != SecondsLeft)
	{
		if(MatchState == MatchState::WaitingToStart || MatchState == MatchState::MatchInCooldown)
			SetHudAnnouncementCountDown(TimeLeft);
		if(MatchState == MatchState::InProgress)
			SetHudMatchCountDown(TimeLeft);
	}
	CountDownInt = SecondsLeft;
}

// Initialize HUD elements when character overlay is available
void ABlasterPlayerController::PollInit()
{
	if(PcCharacterOverlay == nullptr)
	{
		if(BlasterHud)
		{
			if(BlasterHud->CharacterOverlay)
			{
				PcCharacterOverlay = BlasterHud->CharacterOverlay;
				if(PcCharacterOverlay)
				{
					// Set initial HUD values if they are flagged to initialize
					if(bInitializeHealth) SetHudHealth(HudHealth, HudMaxHealth);
					if(bInitializeShields) SetHudShield(HudShield, HudMaxShield);
					if(bInitializeScore) SetHudScore(HudScore);
					if(bInitializeDefeats) SetHudDefeats(HudDefeats);
					if(bInitializeCarriedAmmo) SetHudCarriedAmmo(HudCarriedAmmo);
					if(bInitializeWeaponAmmo) SetHudWeaponAmmo(HudWeaponAmmo);

					ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(GetPawn());
					if(BlasterCharacter && BlasterCharacter->GetCombatComponent() && bInitializeGrenades)
						SetHudGrenades(BlasterCharacter->GetCombatComponent()->GetGrenades());				

					if(bTeamsMatch) InitTeamScores();
					else HideTeamScores();
				}
			}
			
			if(MatchState == MatchState::WaitingToStart)
				BlasterHud->AddAnnouncement();
			if(MatchState == MatchState::InProgress)
				BlasterHud->AddCharacterOverlay();
		}
	}
}

// Join mid-game with synchronized match timing
void ABlasterPlayerController::ClientJoinMidGame_Implementation(const FName StateOfMatch, const float WarmUp, const float Match, const float StartingTime, const float Cooldown)
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

// Set match state and handle team scores/selection visibility based on state
void ABlasterPlayerController::OnRep_MatchState()
{
	if(MatchState == MatchState::InProgress)
	{
		HandleTeamSelection();
		GetBlasterHud();
		if(BlasterHud == nullptr) return;

		BlasterHud->AddCharacterOverlay();
		if(BlasterHud->Announcement)
			BlasterHud->Announcement->SetVisibility(ESlateVisibility::Collapsed);

		if(bTeamsMatch) InitTeamScores();
		else HideTeamScores();
	}
	else if(MatchState == MatchState::MatchInCooldown)
		HandleCooldown();
	else if(MatchState == MatchState::WaitingTeamSelection)
		HandleTeamSelection();
}

void ABlasterPlayerController::SetHudHealth(const float Health, const float MaxHealth)
{
	GetBlasterHud();
	if(BlasterHud && BlasterHud->CharacterOverlay && BlasterHud->CharacterOverlay->HealthBar && BlasterHud->CharacterOverlay->HealthText)
	{
		const float HealthPercent = Health / MaxHealth;
		BlasterHud->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		const FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		BlasterHud->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
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
	GetBlasterHud();
	if(BlasterHud && BlasterHud->CharacterOverlay && BlasterHud->CharacterOverlay->ShieldBar && BlasterHud->CharacterOverlay->ShieldText)
	{
		const float ShieldPercent = Shield / MaxShield;
		BlasterHud->CharacterOverlay->ShieldBar->SetPercent(ShieldPercent);
		const FString ShieldText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Shield), FMath::CeilToInt(MaxShield));
		BlasterHud->CharacterOverlay->ShieldText->SetText(FText::FromString(ShieldText));
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
	GetBlasterHud();
	if(BlasterHud && BlasterHud->CharacterOverlay && BlasterHud->CharacterOverlay->ScoreText)
	{
		BlasterHud->CharacterOverlay->ScoreText->SetText(FText::AsNumber(FMath::CeilToInt(Score)));
	}
	else
	{
		HudScore = Score;
		bInitializeScore = true;
	}
}

void ABlasterPlayerController::SetHudDefeats(int32 Defeats)
{
	GetBlasterHud();
	if(BlasterHud && BlasterHud->CharacterOverlay && BlasterHud->CharacterOverlay->DefeatText)
	{
		BlasterHud->CharacterOverlay->DefeatText->SetText(FText::AsNumber(Defeats));
	}
	else
	{
		HudDefeats = Defeats;
		bInitializeDefeats = true;
	}
}

void ABlasterPlayerController::SetHudWeaponAmmo(const int32 Ammo)
{
	GetBlasterHud();
	if(BlasterHud && BlasterHud->CharacterOverlay && BlasterHud->CharacterOverlay->WeaponAmmoText)
	{
		BlasterHud->CharacterOverlay->WeaponAmmoText->SetText(FText::AsNumber(Ammo));
	}
	else
	{
		HudWeaponAmmo = Ammo;
		bInitializeWeaponAmmo = true;
	}
}

void ABlasterPlayerController::SetHudCarriedAmmo(int32 Ammo)
{
	GetBlasterHud();
	if(BlasterHud && BlasterHud->CharacterOverlay && BlasterHud->CharacterOverlay->CarriedAmmoText)
	{
		BlasterHud->CharacterOverlay->CarriedAmmoText->SetText(FText::AsNumber(Ammo));
	}
	else
	{
		HudCarriedAmmo = Ammo;
		bInitializeCarriedAmmo = true;
	}
}

// Requests server time and reports the server time back to the client
void ABlasterPlayerController::Server_RequestServerTime_Implementation(float TimeOfClientRequest)
{
	const float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds();
	Client_ReportServerTime(TimeOfClientRequest, ServerTimeOfReceipt);
}

// Client receives server time, calculates trip time, and adjusts for server-client time delta
void ABlasterPlayerController::Client_ReportServerTime_Implementation(const float TimeOfClientRequest, float TimeOfServerReceivedClientRequest)
{
	const float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	SingleTripTime = 0.5f * RoundTripTime;
	const float CurrentServerTime = TimeOfServerReceivedClientRequest + SingleTripTime;
	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}

// Retrieves match state from the server and updates HUD and relevant game parameters
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
	}
}

// Updates HUD countdown timer based on remaining match time
void ABlasterPlayerController::SetHudMatchCountDown(const float CountDownTime)
{
	GetBlasterHud();
	if(BlasterHud && BlasterHud->CharacterOverlay && BlasterHud->CharacterOverlay->MatchCountDownText)
	{		
		if(CountDownTime < 0.f)
		{
			BlasterHud->CharacterOverlay->MatchCountDownText->SetText(FText::FromString(TEXT("00::00")));
			return;
		}
		const int32 Min = FMath::FloorToInt(CountDownTime / 60);
		const int32 Seconds = CountDownTime - Min * 60;
		const FString Text = FString::Printf(TEXT("%02d:%02d"), Min, Seconds);
		BlasterHud->CharacterOverlay->MatchCountDownText->SetText(FText::FromString(Text));
	}
}

// Updates HUD announcement countdown timer for announcements such as WarmUp time
void ABlasterPlayerController::SetHudAnnouncementCountDown(const float CountdownTime)
{
	GetBlasterHud();
	if(BlasterHud == nullptr) return;

	// ReSharper disable once CppTooWideScopeInitStatement
	const UAnnouncement* Announcement = BlasterHud->Announcement;
	if(Announcement && Announcement->WarmUpTime)
	{
		if(CountdownTime < 0.f)
		{
			Announcement->WarmUpTime->SetText(FText::FromString(TEXT("00::00")));
			return;
		}
		const int32 Min = FMath::FloorToInt(CountdownTime / 60);
		const int32 Seconds = CountdownTime - Min * 60;
		const FString Text = FString::Printf(TEXT("%02d:%02d"), Min, Seconds);
		Announcement->WarmUpTime->SetText(FText::FromString(Text));
	}
}

void ABlasterPlayerController::SetHudGrenades(int32 Grenades)
{
	GetBlasterHud();
	if(BlasterHud && BlasterHud->CharacterOverlay && BlasterHud->CharacterOverlay->GrenadesText)
	{
		const FString Text = FString::Printf(TEXT("%d"), Grenades);
		BlasterHud->CharacterOverlay->GrenadesText->SetText(FText::FromString(Text));
	}
	else
	{
		HudGrenades = Grenades;
		bInitializeGrenades = true;
	}
}

// Called when the controller possesses a new character, updating HUD health, shield, and grenades
void ABlasterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(InPawn))
	{
		SetHudHealth(BlasterCharacter->GetHealth(), BlasterCharacter->GetMaxHealth());
		SetHudShield(BlasterCharacter->GetShield(), BlasterCharacter->GetMaxShield());
		SetHudGrenades(BlasterCharacter->GetCombatComponent()->GetGrenades());
		BlasterCharacter->UpdateHudAmmo();
		if(MatchState == MatchState::MatchInCooldown)
			DisablePlayerMechanics();
	}
}

void ABlasterPlayerController::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CheckPing(DeltaSeconds);
	SetHudTime();
	PollInit();
}

// Checks player ping, triggering warning animations if above threshold
void ABlasterPlayerController::CheckPing(float DeltaSeconds)
{	
	HighPingRunningTime += DeltaSeconds;
	if(HighPingRunningTime >= CheckPingFrequency)
	{
		PlayerState = PlayerState == nullptr ? TObjectPtr<APlayerState>(GetPlayerState<APlayerState>()) : PlayerState;
		if(PlayerState && PlayerState->GetPingInMilliseconds() > HighPingThreshold)
		{
			PingAnimationRunningTime = 0;
			HighPingWarning();
			ServerReportPingStatus(true);
		}
		else
			ServerReportPingStatus(false);
		HighPingRunningTime = 0;
	}

	// Stops high ping animation after set duration
	if(BlasterHud && BlasterHud->CharacterOverlay && BlasterHud->CharacterOverlay->HighPingAnimation && BlasterHud->CharacterOverlay->IsAnimationPlaying(BlasterHud->CharacterOverlay->HighPingAnimation))
	{
		PingAnimationRunningTime += DeltaSeconds;
		if(PingAnimationRunningTime > HighPingDuration)
			StopHighPingWarning();
	}
}

// Server-side report on player's high ping status
void ABlasterPlayerController::ServerReportPingStatus_Implementation(bool bHighPing)
{
	HighPingDelegate.Broadcast(bHighPing);
}

// Returns current server time, adjusted by server-client delta if not server authority
float ABlasterPlayerController::GetServerTime()
{
	if(HasAuthority()) return GetWorld()->GetTimeSeconds();
	
	return GetWorld()->GetTimeSeconds() + ClientServerDelta;
}

// Sends sync request to the server for server time adjustment
void ABlasterPlayerController::SendReqSyncServerTime()
{
	Server_RequestServerTime(GetWorld()->GetTimeSeconds());
}

// Initializes player by syncing time with the server
void ABlasterPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	if(IsLocalController())
	{
		SendReqSyncServerTime();
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ABlasterPlayerController::SendReqSyncServerTime, TimeSyncFrequency, true);
	}
}

void ABlasterPlayerController::OnMatchStateSet(const FName State, bool bisTeamsMatch)
{
	MatchState = State;
	if(HasAuthority())
	{
		bTeamsMatch = bisTeamsMatch;
		OnRep_MatchState();
	}
}

void ABlasterPlayerController::DisablePlayerMechanics() const
{
	if(ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(GetPawn()))
	{
		BlasterCharacter->bDisableGameplay = true;
		if(UCombatComponent* CombatComponent = BlasterCharacter->GetCombatComponent())
			CombatComponent->FireButtonPressed(false);
	}
}

// Disables gameplay controls during cooldown phase and updates HUD announcements
void ABlasterPlayerController::HandleCooldown()
{
	DisablePlayerMechanics();

	GetBlasterHud();
	if(BlasterHud == nullptr || BlasterHud->CharacterOverlay == nullptr) return;

	BlasterHud->CharacterOverlay->RemoveFromParent();
	
	// ReSharper disable once CppTooWideScopeInitStatement
	UAnnouncement* Announcement = BlasterHud->Announcement;
	if(Announcement && Announcement->AnnouncementText && Announcement->InfoText)
	{
		Announcement->SetVisibility(ESlateVisibility::Visible);
		Announcement->AnnouncementText->SetText(FText::FromString(Announcement::NewMatchStartsIn));

		FString WinnerText = "";
		const ABlasterPlayerState* BlasterPlayerState = GetPlayerState<ABlasterPlayerState>();
		// ReSharper disable once CppTooWideScopeInitStatement
		const ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
		if(BlasterGameState && BlasterPlayerState)
		{
			TArray<ABlasterPlayerState*> TopPlayers = BlasterGameState->TopScoringPlayers;
			WinnerText = bTeamsMatch ? GetTeamsInfoText(BlasterGameState) : GetInfoText(TopPlayers, BlasterPlayerState);
		}		
		Announcement->InfoText->SetText(FText::FromString(WinnerText));
	}
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// Utility function to generate text for top players
FString ABlasterPlayerController::GetInfoText(const TArray<ABlasterPlayerState*>& Players, const ABlasterPlayerState* BlasterPlayerState)
{
	FString WinnerText;
	if(!BlasterPlayerState) return WinnerText;
	if(Players.IsEmpty())
		WinnerText = Announcement::ThereIsNoWinner;
	else if(Players.Num() == 1 && Players[0] == BlasterPlayerState)
		WinnerText = Announcement::YouAreTheWinner;
	else if(Players.Num() == 1)
		WinnerText = FString::Printf(TEXT("Winner: \n%s"), *Players[0]->GetPlayerName());
	else
	{
		WinnerText = Announcement::PlayersTiedForTheWin + "\n";
		for(const auto* TiedPlayer : Players)
			WinnerText.Append(FString::Printf(TEXT("%s\n"), *TiedPlayer->GetPlayerName()));
	}
	return WinnerText;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// Generates winner text based on team scores and current game state
FString ABlasterPlayerController::GetTeamsInfoText(const ABlasterGameState* BlasterGameState)
{
	FString WinnerText("");
	if(BlasterGameState)
	{
		const int32 RedTeamScore = BlasterGameState->RedTeamScore;
		const int32 BlueTeamScore = BlasterGameState->BlueTeamScore;

		if(RedTeamScore == 0 && BlueTeamScore == 0)
			WinnerText = Announcement::ThereIsNoWinner;
		else if(RedTeamScore == BlueTeamScore)
		{
			WinnerText = FString::Printf(TEXT("%s\n"), *Announcement::ThereIsNoWinner);
			WinnerText.Append(Announcement::RedTeam);
			WinnerText.Append("\n");
			WinnerText.Append(Announcement::BlueTeam);
			WinnerText.Append("\n");
		}
		else if(RedTeamScore > BlueTeamScore)
		{
			WinnerText = FString::Printf(TEXT("%s wins!"), *Announcement::RedTeam);			
			WinnerText.Append("\n");
			WinnerText.Append(FString::Printf(TEXT("%s: %d"), *Announcement::RedTeam, RedTeamScore));			
			WinnerText.Append("\n");
			WinnerText.Append(FString::Printf(TEXT("%s: %d"), *Announcement::BlueTeam, BlueTeamScore));
		}
		else if(BlueTeamScore > RedTeamScore)
		{			
			WinnerText = FString::Printf(TEXT("%s wins!"), *Announcement::BlueTeam);			
			WinnerText.Append("\n");
			WinnerText.Append(FString::Printf(TEXT("%s: %d"), *Announcement::BlueTeam, BlueTeamScore));
			WinnerText.Append("\n");
			WinnerText.Append(FString::Printf(TEXT("%s: %d"), *Announcement::RedTeam, RedTeamScore));			
		}
	}
	return WinnerText;
}

// Handles team selection for the player, opening the team selection widget
void ABlasterPlayerController::HandleTeamSelection()
{
	if(!IsLocalPlayerController() || bHasSelectedTeam || WTeamSelection == nullptr) return;

	// Creates and sets up the team selection widget if it doesn't already exist
	if(WbpTeamSelection == nullptr)
	{
		WbpTeamSelection = CreateWidget<UTeamSelection>(this, WTeamSelection);
		if(WbpTeamSelection)
		{
			WbpTeamSelection->MenuSetup();
			WbpTeamSelection->OnTeamSelectionChanged.AddDynamic(this, &ABlasterPlayerController::OnTeamSelectionChanged);
		}
	}
}

void ABlasterPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABlasterPlayerController, MatchState);
	DOREPLIFETIME(ABlasterPlayerController, bTeamsMatch);
}

// Shows a high ping warning animation on the HUD when player's ping is above a set threshold
void ABlasterPlayerController::HighPingWarning()
{
	GetBlasterHud();
	if(BlasterHud && BlasterHud->CharacterOverlay && BlasterHud->CharacterOverlay->WifiStrength && BlasterHud->CharacterOverlay->HighPingAnimation)
	{
		BlasterHud->CharacterOverlay->WifiStrength->SetVisibility(ESlateVisibility::Visible);
		BlasterHud->CharacterOverlay->PlayAnimation(BlasterHud->CharacterOverlay->HighPingAnimation, 0, 0);
	}
}

// Stops the high ping warning animation on the HUD
void ABlasterPlayerController::StopHighPingWarning()
{
	GetBlasterHud();
	if(BlasterHud && BlasterHud->CharacterOverlay && BlasterHud->CharacterOverlay->WifiStrength && BlasterHud->CharacterOverlay->HighPingAnimation)
	{
		BlasterHud->CharacterOverlay->WifiStrength->SetVisibility(ESlateVisibility::Collapsed);
		if(BlasterHud->CharacterOverlay->IsAnimationPlaying(BlasterHud->CharacterOverlay->HighPingAnimation))
			BlasterHud->CharacterOverlay->StopAnimation(BlasterHud->CharacterOverlay->HighPingAnimation);
	}
}

// Toggles the Return to Menu widget visibility, initializing it if not already created
void ABlasterPlayerController::ShowReturnToMenu()
{
	if(WReturnToMenu == nullptr) return;
	if(WbpReturnToMenu == nullptr)
	{
		WbpReturnToMenu = CreateWidget<UReturnToMainMenu>(this, WReturnToMenu);
		if(WbpReturnToMenu)
			WbpReturnToMenu->OnMenuTornDown.BindLambda([this](){ bReturnToMenuOpen = !bReturnToMenuOpen; });
	}
	if(WbpReturnToMenu)
	{
		bReturnToMenuOpen = !bReturnToMenuOpen;
		if(bReturnToMenuOpen)
			WbpReturnToMenu->MenuSetup();
		else
			WbpReturnToMenu->MenuTearDown();
	}
}

// Updates team selection and notifies the server of the selected team
void ABlasterPlayerController::OnTeamSelectionChanged(const ETeam NewTeam)
{
	bHasSelectedTeam = true;
	if(WbpTeamSelection)
		WbpTeamSelection->MenuTearDown();
	Server_OnTeamSelectionChanged(NewTeam);
}

void ABlasterPlayerController::OnRep_TeamsMatch()
{
	bTeamsMatch ? InitTeamScores() : HideTeamScores();
}

// Server function to handle team selection change and adjust the game state accordingly
void ABlasterPlayerController::Server_OnTeamSelectionChanged_Implementation(const ETeam NewTeam)
{
	if(ABlasterPlayerState* BlasterPlayerState = GetPlayerState<ABlasterPlayerState>())
	{
		if(ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this)))
		{
			// Remove player from any current team
			BlasterGameState->RedTeam.Remove(BlasterPlayerState);
			BlasterGameState->BlueTeam.Remove(BlasterPlayerState);

			// Add player to the selected team
			if(NewTeam == ETeam::ET_Red)
				BlasterGameState->RedTeam.AddUnique(BlasterPlayerState);
			else
				BlasterGameState->BlueTeam.AddUnique(BlasterPlayerState);
		}
		BlasterPlayerState->SetTeam(NewTeam);
	}

	// If game mode is not yet set, retrieve it
	BlasterGameMode = BlasterGameMode == nullptr ? Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this)): BlasterGameMode;
	if(BlasterGameMode)
		BlasterGameMode->StartMatch();
}
