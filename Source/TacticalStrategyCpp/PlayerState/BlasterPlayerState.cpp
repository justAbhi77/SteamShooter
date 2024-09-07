// Fill out your copyright notice in the Description page of Project Settings.

#include "BlasterPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "TacticalStrategyCpp/Character/BlasterCharacter.h"
#include "TacticalStrategyCpp/PlayerController/BlasterPlayerController.h"

void ABlasterPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	Character = Character == nullptr ? Cast<ABlasterCharacter>(GetPawn()) : Character;
	if(Character)
	{
		Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
		if(Controller)
		{
			Controller->SetHudScore(GetScore());
		}
	}
}

void ABlasterPlayerState::AddToScore(const float ScoreAmount, const bool bUpdateLocally)
{
	SetScore( GetScore() + ScoreAmount);

	if(HasAuthority() || bUpdateLocally)
	{
		OnRep_Score();
	}
}

void ABlasterPlayerState::AddToDefeats(const int32 DefeatsAmount, const bool bUpdateLocally)
{
	Defeats += DefeatsAmount;

	if(HasAuthority() || bUpdateLocally)
	{
		OnRep_Defeats();
	}	
}

void ABlasterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABlasterPlayerState, Defeats);
}

void ABlasterPlayerState::OnRep_Defeats()
{
	Character = Character == nullptr ? Cast<ABlasterCharacter>(GetPawn()) : Character;
	if(Character)
	{
		Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
		if(Controller)
		{
			Controller->SetHudDefeats(Defeats);
		}
	}	
}
