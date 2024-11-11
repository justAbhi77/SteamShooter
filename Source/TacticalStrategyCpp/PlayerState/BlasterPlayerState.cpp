// Fill out your copyright notice in the Description page of Project Settings.

#include "BlasterPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"
#include "TacticalStrategyCpp/PlayerController/BlasterPlayerController.h"

void ABlasterPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	Character = Character ? Character : Cast<ABlasterCharacter>(GetPawn());
	if(Character)
	{
		Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
		if(Controller)
			Controller->SetHudScore(GetScore());
	}
}

void ABlasterPlayerState::AddToScore(const float ScoreAmount, const bool bUpdateLocally)
{
	// Add score and trigger replication event if needed
	SetScore(GetScore() + ScoreAmount);

	if(HasAuthority() || bUpdateLocally)
		OnRep_Score();
}

void ABlasterPlayerState::AddToDefeats(const int32 DefeatsAmount, const bool bUpdateLocally)
{
	// Increment defeats and trigger replication event if needed
	Defeats += DefeatsAmount;

	if(HasAuthority() || bUpdateLocally)
		OnRep_Defeats();
}

void ABlasterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABlasterPlayerState, Defeats);
	DOREPLIFETIME(ABlasterPlayerState, Team);
}

void ABlasterPlayerState::OnRep_Defeats()
{
	Character = Character == nullptr ? Cast<ABlasterCharacter>(GetPawn()) : Character;
	if(Character)
	{
		Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
		if(Controller)
			Controller->SetHudDefeats(Defeats);
	}
}

void ABlasterPlayerState::SetTeam(const ETeam TeamToSet)
{
	// Set the player's team
	Team = TeamToSet;
	if(HasAuthority())
		OnRep_Team();
}

void ABlasterPlayerState::OnRep_Team()
{
	Character = Character == nullptr ? Cast<ABlasterCharacter>(GetPawn()) : Character;
	if(Character)
	{
		// Set the player state on Character and initialize any required team-based functionality
		Character->BlasterPlayerState = this;
		Character->OnPollInit();
	}
}
