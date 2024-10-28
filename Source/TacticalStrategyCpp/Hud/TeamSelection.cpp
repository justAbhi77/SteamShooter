// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamSelection.h"
#include "Components/Button.h"

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
		RedTeam->OnClicked.AddDynamic(&UReturnToMainMenu::ReturnButtonClicked);

	if(BlueTeam && !BlueTeam->OnClicked.IsBound())
		BlueTeam->OnClicked.AddDynamic(this, &UReturnToMainMenu::BackButtonClicked);
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
		RedTeam->OnClicked.RemoveDynamic(this, &UReturnToMainMenu::ReturnButtonClicked);

	if(BlueTeam && BlueTeam->OnClicked.IsBound())
		BlueTeam->OnClicked.RemoveDynamic(this, &UReturnToMainMenu::BackButtonClicked);
}

void UTeamSelection::TeamButtonClicked(ETeam NewTeam)
{
}

