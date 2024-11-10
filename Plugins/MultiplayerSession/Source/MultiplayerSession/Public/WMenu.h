// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerModes.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "WMenu.generated.h"

/**
 * Provides controls for hosting and joining multiplayer sessions.
 */
UCLASS()
class MULTIPLAYERSESSION_API UWMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/**
	 * Sets up the menu with default parameters or user-defined settings
	 * @param NumberPublicConnections Number of connections to wait for before starting match.
	 * @param TypeOfMatch The type of match to start(See EMultiplayerModes enum for possible types of match).
	 * @param LobbyPath The lobby to host while waiting for players to join.
	 */
	UFUNCTION(BlueprintCallable)
	void MenuSetup(int32 NumberPublicConnections = 4, EMultiplayerModes TypeOfMatch = EMultiplayerModes::Emm_Teams,
		FString LobbyPath = FString(TEXT("/Game/Maps/Lobby")));

protected:
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;

	// Delegate callbacks for handling session events
	UFUNCTION()
	void OnCreateSession(bool bWasSuccessful);
	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);
	UFUNCTION()
	void OnStartSession(bool bWasSuccessful);

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	// Functions triggered by button clicks
	UFUNCTION() void HostButtonClicked();
	UFUNCTION() void JoinButtonClicked();

	// Clean up UI
	void MenuTearDown();

	// Subsystem to manage multiplayer sessions
	UPROPERTY()
	class UMultiplayerSessionSubsystem* MultiplayerSessionSubsystem;

	/** Number of connections */
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 NumPublicConnections{4};

	/** Match type settings */
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EMultiplayerModes MatchType = EMultiplayerModes::Emm_FreeForAll;

	/** Path to the multiplayer lobby */
	FString PathToLobby{TEXT("")};
};
