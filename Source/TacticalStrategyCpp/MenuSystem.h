// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MenuSystem.generated.h"

UCLASS()
class TACTICALSTRATEGYCPP_API AMenuSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	AMenuSystem();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void CreateGameSession() const;
	
	UFUNCTION(BlueprintCallable)
	void JoinGameSession();

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	void OnFindSessionComplete(bool bWasSuccessful) const;

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

private:
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;

	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;

	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

public:	
	virtual void Tick(float DeltaTime) override;

	IOnlineSessionPtr OnlineSessionInterface;
};
