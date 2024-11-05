// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerModes.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "WMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSION_API UWMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup(int32 NumberPublicConnections = 4, EMultiplayerModes TypeOfMatch = EMultiplayerModes::EMM_Teams,
		FString LobbyPath = FString(TEXT("/Game/Maps/Lobby")));

protected:
	virtual bool Initialize() override;
	virtual void NativeDestruct() override; // used instead of OnLevelRemoveFromWorld

	
	/**
	 * callbacks for delegate in the Multiplayer Subsystem
	 */
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

	UFUNCTION()
	void HostButtonClicked();

	UFUNCTION()
	void JoinButtonClicked();

	void MenuTearDown();

	
	/**
	 * SubSystem
	 */
	UPROPERTY()
	class UMultiplayerSessionSubsystem* MultiplayerSessionSubsystem;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 NumPublicConnections{4};

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EMultiplayerModes MatchType = EMultiplayerModes::EMM_FreeForAll;

	FString PathToLobby{TEXT("")};
};
