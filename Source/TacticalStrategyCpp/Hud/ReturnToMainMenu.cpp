// Fill out your copyright notice in the Description page of Project Settings.


#include "ReturnToMainMenu.h"
#include "MultiplayerSessionSubsystem.h"
#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"

void UReturnToMainMenu::MenuSetup()
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
	
	if(ReturnButton && !ReturnButton->OnClicked.IsBound())
		ReturnButton->OnClicked.AddDynamic(this, &UReturnToMainMenu::ReturnButtonClicked);

	if(BackButton && !BackButton->OnClicked.IsBound())
	{
		BackButton->SetIsEnabled(true);
		BackButton->OnClicked.AddDynamic(this, &UReturnToMainMenu::BackButtonClicked);
	}

	if(UGameInstance* GameInstance = GetGameInstance())
	{
		MultiplayerSessionSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();
		if(MultiplayerSessionSubsystem)
			MultiplayerSessionSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this,
				&UReturnToMainMenu::OnDestroySession);
	}
}

void UReturnToMainMenu::MenuTearDown()
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
	if(ReturnButton && ReturnButton->OnClicked.IsBound())
		ReturnButton->OnClicked.RemoveDynamic(this, &UReturnToMainMenu::ReturnButtonClicked);

	if(BackButton && BackButton->OnClicked.IsBound())
	{
		BackButton->SetIsEnabled(true);
		BackButton->OnClicked.RemoveDynamic(this, &UReturnToMainMenu::BackButtonClicked);
	}
	
	if(MultiplayerSessionSubsystem && MultiplayerSessionSubsystem->MultiplayerOnDestroySessionComplete.IsBound())
		MultiplayerSessionSubsystem->MultiplayerOnDestroySessionComplete.RemoveDynamic(this,
			&UReturnToMainMenu::OnDestroySession);
}

bool UReturnToMainMenu::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}

	return true;
}

void UReturnToMainMenu::OnDestroySession(bool bWasSuccessful)
{
	if(!bWasSuccessful)
	{
		ReturnButton->SetIsEnabled(true);
		return;
	}
	
	if(const UWorld* World = GetWorld())
	{
		if(AGameModeBase* GameMode = World->GetAuthGameMode<AGameModeBase>())
			GameMode->ReturnToMainMenuHost();
		else
		{
			PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
			if(PlayerController)
				PlayerController->ClientReturnToMainMenuWithTextReason(FText());
		}
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UReturnToMainMenu::ReturnButtonClicked()
{
	ReturnButton->SetIsEnabled(false);
	
	if(MultiplayerSessionSubsystem)
		MultiplayerSessionSubsystem->DestroySession();
}

void UReturnToMainMenu::BackButtonClicked()
{
	BackButton->SetIsEnabled(false);
	OnMenuTornDown.Execute();
	MenuTearDown();
}
