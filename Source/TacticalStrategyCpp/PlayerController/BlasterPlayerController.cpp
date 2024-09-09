
#include "BlasterPlayerController.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"
#include "TacticalStrategyCpp/GameMode/BlasterGameMode.h"
#include "TacticalStrategyCpp/Hud/Announcement.h"
#include "TacticalStrategyCpp/Hud/BlasterHud.h"
#include "TacticalStrategyCpp/Hud/CharacterOverlay.h"

ABlasterPlayerController::ABlasterPlayerController():
	ClientServerDelta(0),
	TimeSyncFrequency(5),
	BlasterHud(nullptr),
	MatchTime(0),
	WarmUpTime(0),
	LevelStartingTime(0),
	CountDownInt(0),
	CharacterOverlay(nullptr),
	bInitializeCharacterOverlay(false),
	HudHealth(0),
	HudMaxHealth(0),
	HudScore(0),
	HudDefeats(0)
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
	const uint32 SecondsLeft = FMath::CeilToInt(MiliSecondsLeft);

	float TimeLeft = 0;

	if(MatchState == MatchState::WaitingToStart)
		TimeLeft = WarmUpTime - GetServerTime() + LevelStartingTime;
	else if(MatchState == MatchState::InProgress)
		TimeLeft = WarmUpTime + MiliSecondsLeft + LevelStartingTime;

	if(CountDownInt != SecondsLeft)
	{
		if(MatchState == MatchState::WaitingToStart)
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
				SetHudHealth(HudHealth, HudMaxHealth);
				SetHudScore(HudScore);
				SetHudDefeats(HudDefeats);
			}
		}
	}
}

void ABlasterPlayerController::ClientJoinMidGame_Implementation(const FName StateOfMatch, const float WarmUp, const float Match, const float StartingTime)
{
	WarmUpTime = WarmUp;
	MatchTime = Match;
	LevelStartingTime = StartingTime;
	MatchState = StateOfMatch;
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
		ClientJoinMidGame(MatchState, WarmUpTime, MatchTime, LevelStartingTime);
		
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
	if(BlasterHud == nullptr) return;
			
	if(CharacterOverlay && CharacterOverlay->HealthBar && CharacterOverlay->HealthText)
	{		
		const float HealthPercent = Health/MaxHealth;
		CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		const FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
	else
	{
		bInitializeCharacterOverlay = true;
		HudHealth = Health;
		HudMaxHealth = MaxHealth;
	}
}

void ABlasterPlayerController::SetHudScore(const float Score)
{
	BlasterHud = BlasterHud == nullptr ? Cast<ABlasterHud>(GetHUD()) : BlasterHud;
	if(BlasterHud == nullptr) return;
		
	if(CharacterOverlay && CharacterOverlay->ScoreText)
	{
		const FString Text = FString::Printf(TEXT("%d"), FMath::CeilToInt(Score) );
		CharacterOverlay->ScoreText->SetText(FText::FromString(Text));
	}
	else
	{
		bInitializeCharacterOverlay = true;
		HudScore = Score;
	}
}

void ABlasterPlayerController::SetHudDefeats(int32 Defeats)
{
	BlasterHud = BlasterHud == nullptr ? Cast<ABlasterHud>(GetHUD()) : BlasterHud;
	if(BlasterHud == nullptr) return;
	
	if(CharacterOverlay && CharacterOverlay->DefeatText)
	{
		const FString Text = FString::Printf(TEXT("%d"), Defeats);
		CharacterOverlay->DefeatText->SetText(FText::FromString(Text));
	}	
	else
	{
		bInitializeCharacterOverlay = true;
		HudDefeats = Defeats;
	}
}

void ABlasterPlayerController::SetHudWeaponAmmo(const int32 Ammo)
{
	BlasterHud = BlasterHud == nullptr ? Cast<ABlasterHud>(GetHUD()) : BlasterHud;
	if(BlasterHud == nullptr) return;
		
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
		
	if(CharacterOverlay && CharacterOverlay->CarriedAmmoText)
	{
		const FString Text = FString::Printf(TEXT("%d"), Ammo);
		CharacterOverlay->CarriedAmmoText->SetText(FText::FromString(Text));
	}	
}

void ABlasterPlayerController::SetHudMatchCountDown(const float CountDownTime)
{
	BlasterHud = BlasterHud == nullptr ? Cast<ABlasterHud>(GetHUD()) : BlasterHud;
	if(BlasterHud == nullptr) return;
		
	if(CharacterOverlay && CharacterOverlay->MatchCountDownText)
	{
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
		const int32 Min = FMath::FloorToInt(CountdownTime / 60);
		const int32 Seconds = CountdownTime - Min * 60.f;
		const FString Text = FString::Printf(TEXT("%02d:%02d"), Min, Seconds);
		Announcement->WarmUpTime->SetText(FText::FromString(Text));
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

void ABlasterPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABlasterPlayerController, MatchState);
}
