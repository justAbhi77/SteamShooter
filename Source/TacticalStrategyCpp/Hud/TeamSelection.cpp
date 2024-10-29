// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamSelection.h"
#include "Components/Button.h"
#include "TacticalStrategyCpp/Enums/Team.h"
#include "TacticalStrategyCpp/PlayerState/BlasterPlayerState.h"

void UTeamSelection::MenuSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	SetIsFocusable(true);

	if(const UWorld* World = GetWorld())
	{
		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
		if(PlayerController)
		{
			FInputModeGameAndUI InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}
	
	if(RedTeam && !RedTeam->OnClicked.IsBound())
		RedTeam->OnClicked.AddDynamic(this, &UTeamSelection::OnRedButtonClicked);

	if(BlueTeam && !BlueTeam->OnClicked.IsBound())
		BlueTeam->OnClicked.AddDynamic(this, &UTeamSelection::OnBlueButtonClicked);
}

void UTeamSelection::MenuTearDown()
{
	RemoveFromParent();	

	if(UWorld* World = GetWorld())
	{
		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
		if(PlayerController)
		{
			const FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
	if(RedTeam && RedTeam->OnClicked.IsBound())
		RedTeam->OnClicked.RemoveDynamic(this, &UTeamSelection::OnRedButtonClicked);

	if(BlueTeam && BlueTeam->OnClicked.IsBound())
		BlueTeam->OnClicked.RemoveDynamic(this, &UTeamSelection::OnBlueButtonClicked);
}

void UTeamSelection::TeamButtonClicked(const ETeam NewTeam) const
{
	if(RedTeam)
		RedTeam->SetIsEnabled(false);
	if(BlueTeam)
		BlueTeam->SetIsEnabled(false);
	if(ABlasterPlayerState* BlasterPlayerState = PlayerController->GetPlayerState<ABlasterPlayerState>())
	{
		BlasterPlayerState->SetTeam(NewTeam);
		TeamSelectionChanged.Broadcast(NewTeam);
	}
}

void UTeamSelection::OnRedButtonClicked()
{
	TeamButtonClicked(ETeam::ET_Red);
}

void UTeamSelection::OnBlueButtonClicked()
{
	TeamButtonClicked(ETeam::ET_Blue);
}

