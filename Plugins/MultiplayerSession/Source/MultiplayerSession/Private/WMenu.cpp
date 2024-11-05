// Fill out your copyright notice in the Description page of Project Settings.


#include "WMenu.h"
#include "Components/Button.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "MultiplayerSessionSubsystem.h"
#include "OnlineSubsystemUtils.h"

void UWMenu::MenuSetup(const int32 NumberPublicConnections, EMultiplayerModes TypeOfMatch, const FString LobbyPath)
{
	NumPublicConnections = NumberPublicConnections;
	MatchType = TypeOfMatch;
	PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);
	
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	SetIsFocusable(true);

	if(const UWorld* World = GetWorld())
	{
		if(APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	if(const UGameInstance* GameInstance = GetGameInstance())
	{
		MultiplayerSessionSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();
	}

	if(MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);

		MultiplayerSessionSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
		MultiplayerSessionSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
		
		MultiplayerSessionSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);

		MultiplayerSessionSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
	}
}

bool UWMenu::Initialize()
{
	if(!Super::Initialize())
		return false;

	if(HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &UWMenu::HostButtonClicked);
	}

	if(JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &UWMenu::JoinButtonClicked);
	}
	
	return true;
}

void UWMenu::NativeDestruct()
{
	Super::NativeDestruct();
	MenuTearDown();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UWMenu::OnCreateSession(const bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1,15.f,FColor::Yellow,
			FString::Printf(TEXT("Session Created Successfully! Yay")));
		}

		if(UWorld* World = GetWorld())
		{
			World->ServerTravel(PathToLobby);
		}
	}
	else
	{		
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1,15.f,FColor::Red,
			FString::Printf(TEXT("Failed to create Session")));
		}
		
		HostButton->SetIsEnabled(true);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UWMenu::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, const bool bWasSuccessful)
{
	if(MultiplayerSessionSubsystem == nullptr)
		return;
	
	for(auto Result : SessionResults)
	{
		int32 ValueType = -1;
		Result.Session.SessionSettings.Get(FName("MatchType"), ValueType);
		if(ValueType == -1) continue;

		const EMultiplayerModes SettingsValue = static_cast<EMultiplayerModes>(ValueType);
		if(SettingsValue == MatchType)
		{
			MultiplayerSessionSubsystem->JoinSession(Result);
			return;
		}
	}

	if(!bWasSuccessful || SessionResults.Num() == 0)
	{
		JoinButton->SetIsEnabled(true);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UWMenu::OnJoinSession(const EOnJoinSessionCompleteResult::Type Result)
{
	if(const IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld()))
	{
		if(IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface())
		{
			if(FString Address; SessionInterface->GetResolvedConnectString(NAME_GameSession, Address))
			{				
				if(APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController())
				{
					PlayerController->ClientTravel(Address, TRAVEL_Absolute);
				}
			}
		}
	}

	if(Result != EOnJoinSessionCompleteResult::Success)
	{
		JoinButton->SetIsEnabled(true);		
	}
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void UWMenu::OnDestroySession(bool bWasSuccessful)
{
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void UWMenu::OnStartSession(bool bWasSuccessful)
{
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UWMenu::HostButtonClicked()
{
	HostButton->SetIsEnabled(false);
	
	if(MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->CreateSession(NumPublicConnections, MatchType);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UWMenu::JoinButtonClicked()
{
	JoinButton->SetIsEnabled(false);
	
	if(MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->FindSessions(1000);
	}
}

void UWMenu::MenuTearDown()
{
	RemoveFromParent();
	if(const UWorld* World = GetWorld())
	{
		if(APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			const FInputModeGameOnly InputModeData;
			
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
}
