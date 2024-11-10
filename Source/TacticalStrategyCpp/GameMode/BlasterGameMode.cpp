#include "BlasterGameMode.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"
#include "TacticalStrategyCpp/GameState/BlasterGameState.h"
#include "TacticalStrategyCpp/PlayerController/BlasterPlayerController.h"
#include "TacticalStrategyCpp/PlayerState/BlasterPlayerState.h"

namespace MatchState
{
	const FName WaitingTeamSelection = FName("WaitingTeamSelection");
	const FName MatchInCooldown = FName("MatchInCooldown");
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

// Called when match is ready to start (after waiting for player teams)
void ABlasterGameMode::AfterWaitingToStart()
{
	StartMatch();
}

// Game mode tick: Updates match state and countdown based on the time
void ABlasterGameMode::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(MatchState == MatchState::WaitingToStart)
	{
		CountDownTime = WarmUpTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if(CountDownTime <= 0.f)
			AfterWaitingToStart();
	}
	else if(MatchState == MatchState::InProgress)
	{
		CountDownTime = WarmUpTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if(CountDownTime <= 0.f)
			SetMatchState(MatchState::MatchInCooldown);
	}
	else if (MatchState == MatchState::MatchInCooldown)
	{
		CountDownTime = CooldownTime + WarmUpTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if(CountDownTime <= 0.f)
			RestartGame();
	}
}

// Handles player elimination (score updates, broadcasting to other players)
void ABlasterGameMode::PlayerEliminated(ABlasterCharacter* ElimmedCharacter, ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	if(AttackerController == nullptr || AttackerController->PlayerState == nullptr) return;
	if(VictimController == nullptr || VictimController->PlayerState == nullptr) return;

	ABlasterPlayerState* AttackerPlayerState = Cast<ABlasterPlayerState>(AttackerController->PlayerState);
	ABlasterPlayerState* VictimPlayerState = Cast<ABlasterPlayerState>(VictimController->PlayerState);
	ABlasterGameState* BlasterGameState = GetGameState<ABlasterGameState>();

	// Ensure valid player states and game state are available
	if(AttackerPlayerState && AttackerPlayerState != VictimPlayerState && BlasterGameState)
	{
		// Update the top scoring players based on attacker’s score
		TArray<ABlasterPlayerState*> PlayersInLead = BlasterGameState->TopScoringPlayers;

		AttackerPlayerState->AddToScore(1.0); // Add score for the attacker
		BlasterGameState->UpdateTopScore(AttackerPlayerState);

		// Notify lead changes to all players
		for(auto& CurrentPlayerState : PlayersInLead)
		{
			if(!BlasterGameState->TopScoringPlayers.Contains(CurrentPlayerState))
				if(ABlasterCharacter* Loser = Cast<ABlasterCharacter>(CurrentPlayerState->GetPawn()))
					Loser->Multicast_LostLead(); // Notify loser of lead loss
		}

		// Notify attacker of new lead
		if(BlasterGameState->TopScoringPlayers.Contains(AttackerPlayerState))
		{
			if(ABlasterCharacter* Leader = Cast<ABlasterCharacter>(AttackerPlayerState->GetPawn()))
				Leader->Multicast_GainedLead();
		}
	}

	// Track the victim’s defeat count and eliminate the character
	if(VictimPlayerState)
		VictimPlayerState->AddToDefeats(1);
	if(ElimmedCharacter)
		ElimmedCharacter->Elim(false);

	// Broadcast the elimination to all players
	for(FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ABlasterPlayerController* BlasterPlayer = Cast<ABlasterPlayerController>(*It);
		if(BlasterPlayer && AttackerPlayerState && VictimController)
			BlasterPlayer->BroadcastElim(AttackerPlayerState, VictimPlayerState);
	}
}

// Handles the respawn request of a player after elimination
void ABlasterGameMode::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	if(ElimmedCharacter)
	{
		ElimmedCharacter->Reset(); // Reset character to initial state
		ElimmedCharacter->Destroy(); // Destroy the eliminated character
	}
	if(ElimmedController)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		const int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1); // Randomly choose a spawn point
		RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[Selection]); // Respawn the player at selected start
	}
}

// Handles player leaving the game (removes from top scoring players)
void ABlasterGameMode::PlayerLeftGame(ABlasterPlayerState* PlayerLeaving) const
{
	if(!PlayerLeaving) return; // If no player state, do nothing

	ABlasterGameState* BlasterGameState = GetGameState<ABlasterGameState>();
	if(BlasterGameState && BlasterGameState->TopScoringPlayers.Contains(PlayerLeaving))
		BlasterGameState->TopScoringPlayers.Remove(PlayerLeaving); // Remove player from top scorers if they leave

	// Eliminate character of the player who left
	if(ABlasterCharacter* CharacterLeaving = Cast<ABlasterCharacter>(PlayerLeaving->GetPawn()))
		CharacterLeaving->Elim(true);
}

// Calculates the damage dealt between two players (customizable for more complex formulas)
float ABlasterGameMode::CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage)
{
	return BaseDamage;
}

void ABlasterGameMode::BeginPlay()
{
	Super::BeginPlay();
	LevelStartingTime = GetWorld()->GetTimeSeconds(); // Record the starting time of the level
}

// Called when match state changes (broadcasts to all players)
void ABlasterGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	for(FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if(ABlasterPlayerController* BlasterPlayer = Cast<ABlasterPlayerController>(*It))
			BlasterPlayer->OnMatchStateSet(MatchState, bTeamsMatch);
	}
}
