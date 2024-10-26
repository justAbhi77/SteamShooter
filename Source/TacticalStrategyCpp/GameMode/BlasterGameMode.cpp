
#include "BlasterGameMode.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"
#include "TacticalStrategyCpp/GameState/BlasterGameState.h"
#include "TacticalStrategyCpp/PlayerController/BlasterPlayerController.h"
#include "TacticalStrategyCpp/PlayerState/BlasterPlayerState.h"

namespace MatchState
{
	const FName Cooldown = FName("Cooldown");
}

ABlasterGameMode::ABlasterGameMode():
	MatchTime(120),
	WarmUpTime(10),
	CooldownTime(10),
	LevelStartingTime(0),
	CountDownTime(0)
{
	bDelayedStart = true;
}

void ABlasterGameMode::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(MatchState == MatchState::WaitingToStart)
	{
		CountDownTime = WarmUpTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if(CountDownTime <= 0.f)
			StartMatch();
	}
	else if(MatchState == MatchState::InProgress)
	{
		 CountDownTime = WarmUpTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if(CountDownTime <= 0.f)
			SetMatchState(MatchState::Cooldown);
	}
	else if (MatchState == MatchState::Cooldown)
	{
		CountDownTime = CooldownTime + WarmUpTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if(CountDownTime <= 0.f)
			RestartGame();			
	}
}

void ABlasterGameMode::PlayerEliminated(ABlasterCharacter* ElimmedCharacter, ABlasterPlayerController* VictimController,
                                        ABlasterPlayerController* AttackerController)
{
	if(AttackerController == nullptr || AttackerController->PlayerState == nullptr) return;
	if(VictimController == nullptr || VictimController->PlayerState == nullptr) return;
	
	ABlasterPlayerState* AttackerPlayerState = AttackerController
			? Cast<ABlasterPlayerState>(AttackerController->PlayerState) : nullptr;
	
	ABlasterPlayerState* VictimPlayerState = VictimController
			? Cast<ABlasterPlayerState>(VictimController->PlayerState) : nullptr;

	ABlasterGameState* BlasterGameState =  GetGameState<ABlasterGameState>();

	if(AttackerPlayerState && AttackerPlayerState != VictimPlayerState && BlasterGameState)
	{		
		TArray<ABlasterPlayerState*> PlayersInLead;
		for(auto LeadPlayer : BlasterGameState->TopScoringPlayers)
			PlayersInLead.Add(LeadPlayer);
		
		AttackerPlayerState->AddToScore(1.0);
		BlasterGameState->UpdateTopScore(AttackerPlayerState);

		for(int32 i=0; i<PlayersInLead.Num(); i++)
		{
			ABlasterPlayerState* CurrentPlayerState = PlayersInLead[i];
			if(!BlasterGameState->TopScoringPlayers.Contains(CurrentPlayerState))
			{
				if(ABlasterCharacter* Loser = Cast<ABlasterCharacter>(CurrentPlayerState->GetPawn()))
					Loser->Multicast_LostLead();
			}
		}
		if(BlasterGameState->TopScoringPlayers.Contains(AttackerPlayerState))
		{
			if(ABlasterCharacter* Leader = Cast<ABlasterCharacter>(AttackerPlayerState->GetPawn()))
				Leader->Multicast_GainedLead();
		}
	}
	if(VictimPlayerState)
		VictimPlayerState->AddToDefeats(1);
	if(ElimmedCharacter)
		ElimmedCharacter->Elim(false);

	for(FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ABlasterPlayerController* BlasterPlayer = Cast<ABlasterPlayerController>(*It);
		if(BlasterPlayer && AttackerPlayerState && VictimController)
			BlasterPlayer->BroadcastElim(AttackerPlayerState, VictimPlayerState);
	}
}

void ABlasterGameMode::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	if(ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	if(ElimmedController)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		const int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[Selection]);
	}
}

void ABlasterGameMode::PlayerLeftGame(ABlasterPlayerState* PlayerLeaving)
{
	if(PlayerLeaving) return;
	ABlasterGameState* BlasterGameState =  GetGameState<ABlasterGameState>();
	if(BlasterGameState && BlasterGameState->TopScoringPlayers.Contains(PlayerLeaving))
	{
		BlasterGameState->TopScoringPlayers.Remove(PlayerLeaving);
	}
	if(ABlasterCharacter* CharacterLeaving = Cast<ABlasterCharacter>(PlayerLeaving->GetPawn()))
		CharacterLeaving->Elim(true);
}

void ABlasterGameMode::BeginPlay()
{
	Super::BeginPlay();
	LevelStartingTime = GetWorld()->GetTimeSeconds();
}

void ABlasterGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	for(FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if(ABlasterPlayerController* BlasterPlayer = Cast<ABlasterPlayerController>(*It))
		{
			BlasterPlayer->OnMatchStateSet(MatchState);
		}
	}
}
