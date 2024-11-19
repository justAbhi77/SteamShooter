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
	// Initialize session settings
	NumPublicConnections = NumberPublicConnections;
	MatchType = TypeOfMatch;
	// Set up the lobby path to load as a listen server
	PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);

	// Show the menu widget
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	// Set it as focusable
	SetIsFocusable(true);

	if(const UWorld* World = GetWorld())
	{
		if(APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			// Configure input to UI-only mode
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	// Get the MultiplayerSessionSubsystem from the GameInstance
	if(const UGameInstance* GameInstance = GetGameInstance())
	{
		MultiplayerSessionSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();
	}

	// Bind session events to this widget's callbacks
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
	if(!Super::Initialize()) return false;

	// Bind button click events to their respective handlers
	if(HostButton) HostButton->OnClicked.AddDynamic(this, &UWMenu::HostButtonClicked);
	if(JoinButton) JoinButton->OnClicked.AddDynamic(this, &UWMenu::JoinButtonClicked);
	
	// UEnum* EnumClass = StaticEnum<EMultiplayerModes>();
	// FString string1 = EnumClass->GetDisplayNameTextByIndex(0).ToString();
	// UE_LOG(LogTemp, Warning, TEXT("enum is converted to %s, %s, %s"), *string1, *string2, *string3);
	return true;
}

void UWMenu::NativeDestruct()
{
	Super::NativeDestruct();
	// Clean up the menu UI
	MenuTearDown();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UWMenu::OnCreateSession(const bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		// If session creation was successful, travel to the lobby
		if(UWorld* World = GetWorld())
			World->ServerTravel(PathToLobby);
	}
	else // Enable the host button if session creation failed
		HostButton->SetIsEnabled(true);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UWMenu::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, const bool bWasSuccessful)
{
	if(MultiplayerSessionSubsystem == nullptr) return;
	
	// Iterate through session results to find a match with the desired settings
	for(auto Result : SessionResults)
	{
		int32 ValueType = -1;
		Result.Session.SessionSettings.Get(FName("MatchType"), ValueType);
		if(ValueType == -1) continue;

		if(static_cast<EMultiplayerModes>(ValueType) == MatchType)
		{
			// Join the matching session
			MultiplayerSessionSubsystem->JoinSession(Result);
			return;
		}
	}

	// Enable join button if no sessions were found or search was unsuccessful
	if(!bWasSuccessful || SessionResults.Num() == 0)
		JoinButton->SetIsEnabled(true);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UWMenu::OnJoinSession(const EOnJoinSessionCompleteResult::Type Result)
{
	// Handle join session success
	if(const IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld()))
		if(IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface())
		{
			FString Address;
			if(SessionInterface->GetResolvedConnectString(NAME_GameSession, Address))
				if(APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController())
					// Travel to the session address
					PlayerController->ClientTravel(Address, TRAVEL_Absolute);
		}

	// Enable join button if session joining failed
	if(Result != EOnJoinSessionCompleteResult::Success)
		JoinButton->SetIsEnabled(true);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void UWMenu::OnDestroySession(bool bWasSuccessful)
{
	// Placeholder for additional logic after session destruction if needed
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void UWMenu::OnStartSession(bool bWasSuccessful)
{
	// Placeholder for additional logic after starting session if needed
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UWMenu::HostButtonClicked()
{
	// Disable host button and create a session
	HostButton->SetIsEnabled(false);
	if(MultiplayerSessionSubsystem)
		MultiplayerSessionSubsystem->CreateSession(NumPublicConnections, MatchType);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UWMenu::JoinButtonClicked()
{
	// Disable join button and initiate session search
	JoinButton->SetIsEnabled(false);
	if(MultiplayerSessionSubsystem)
		MultiplayerSessionSubsystem->FindSessions(1000);
}

void UWMenu::MenuTearDown()
{
	// Remove widget from viewport and reset input to game-only mode
	RemoveFromParent();
	if(const UWorld* World = GetWorld())
	{
		if(APlayerController* PlayerController = World->GetFirstPlayerController())
		{			
			PlayerController->SetInputMode(FInputModeGameOnly());
			PlayerController->SetShowMouseCursor(false);
		}
	}
}
