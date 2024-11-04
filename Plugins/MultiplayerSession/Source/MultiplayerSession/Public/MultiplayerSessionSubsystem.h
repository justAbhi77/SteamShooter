// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerSessionSubsystem.generated.h"

// Multicast Delegate (Event Dispatchers)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnCreateSessionComplete, bool, bWasSuccessful);

// Not compatible with blueprints
DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiplayerOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);

// Not compatible with blueprints
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnDestroySessionComplete, bool, bWasSuccessful);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnStartSessionComplete, bool, bWasSuccessful);

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSION_API UMultiplayerSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UMultiplayerSessionSubsystem();

	// Exposed Functions for Menu

	void CreateSession(int32 NumPublicConnections, const FString& MatchType);

	void FindSessions(int32 MaxSearchResults);

	void JoinSession(const FOnlineSessionSearchResult& SessionResult);

	void DestroySession();

	void StartSession();

	/**
	 * Custom Delegates for menu class (Event Dispatchers)
	 */
	FMultiplayerOnCreateSessionComplete MultiplayerOnCreateSessionComplete;

	FMultiplayerOnFindSessionsComplete MultiplayerOnFindSessionsComplete;

	FMultiplayerOnJoinSessionComplete MultiplayerOnJoinSessionComplete;

	FMultiplayerOnDestroySessionComplete MultiplayerOnDestroySessionComplete;

	FMultiplayerOnStartSessionComplete MultiplayerOnStartSessionComplete;	

	int32 DesiredNumPublicConnections{0};
	FString DesiredMatchType{};
	
protected:
	void OnCreateSessionComplete (FName SessionName, bool bWasSuccessful);
	
	void OnFindSessionsComplete(bool bWasSuccessful);
	
	void OnJoinSessionComplete (FName SessionName, EOnJoinSessionCompleteResult:: Type Result);
	
	void OnDestroySessionComplete (FName SessionName, bool bWasSuccessful);
	
	void OnStartSessionComplete (FName SessionName, bool bWasSuccessful);
	
private:
	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;

	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
	
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;
	
	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;

	bool bCreateSessionOnDestroy{ false };
	int32 LastNumPublicConnections;
	FString LastMatchType;
};
