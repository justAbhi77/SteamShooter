
#include "MenuSystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Online/OnlineSessionNames.h"

AMenuSystem::AMenuSystem():
	CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this,&ThisClass::OnCreateSessionComplete)),
	FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this,&ThisClass::OnFindSessionComplete)),
	JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this,&AMenuSystem::OnJoinSessionComplete))
{
	if(const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();

		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1,15.f,FColor::Blue,
			FString::Printf(TEXT("Found Subsystem %s"),*OnlineSubsystem->GetSubsystemName().ToString()));
		}
	}
}

void AMenuSystem::BeginPlay()
{
	Super::BeginPlay();
}

void AMenuSystem::CreateGameSession() const
{
	// Called When Pressing 1 Key
	if(!OnlineSessionInterface.IsValid())
		return;

	const auto ExistingSession = OnlineSessionInterface->GetNamedSession(NAME_GameSession);

	if(ExistingSession != nullptr)
	{
		OnlineSessionInterface->DestroySession(NAME_GameSession);
	}

	OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	const TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
	
	SessionSettings->bIsLANMatch = false;
	SessionSettings->NumPublicConnections = 4;
	SessionSettings->bAllowJoinInProgress = true;
	SessionSettings->bAllowJoinViaPresence = true;
	SessionSettings->bShouldAdvertise = true;
	SessionSettings->bUsesPresence = true;
	SessionSettings->bUseLobbiesIfAvailable = true;

	SessionSettings->Set(FName("MatchType"), FString("FreeForAll"),EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();	
	OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings);	
}

void AMenuSystem::JoinGameSession()
{
	//Find Game Sessions
	
	if(!OnlineSessionInterface.IsValid())
		return;

	OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);
	
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->bIsLanQuery = false;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	OnlineSessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());

}

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
void AMenuSystem::OnCreateSessionComplete(const FName SessionName, const bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1,15.f,FColor::Blue,FString::Printf(TEXT("Create Session: %s"), *SessionName.ToString()));
		}
		if(UWorld* World = GetWorld())
		{
			World->ServerTravel(FString("/Game/Maps/Lobby?listen"));
		}
	}
	else
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1,15.f,FColor::Red,FString(TEXT("Failed to create session")));
		}
	}
}

void AMenuSystem::OnFindSessionComplete(bool bWasSuccessful) const
{
	if(!OnlineSessionInterface.IsValid())
		return;
	
	for(auto Result : SessionSearch->SearchResults)
	{
		FString ID = Result.GetSessionIdStr();
		FString User = Result.Session.OwningUserName;

		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1,15.f,FColor::Emerald, FString::Printf(TEXT("Username: %s, Id: %s"), *User, *ID));

		FString MatchType;
		Result.Session.SessionSettings.Get(FName("MatchType"), MatchType);
		if(MatchType == FString("FreeForAll"))
		{			
			if(GEngine)
				GEngine->AddOnScreenDebugMessage(-1,15.f,FColor::Emerald, FString::Printf(TEXT("Joining Matchtype: %s"), *MatchType));

			OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);
			
			const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
			OnlineSessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, Result);
		}
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AMenuSystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if(!OnlineSessionInterface.IsValid())
		return;

	if(FString Address; OnlineSessionInterface->GetResolvedConnectString(NAME_GameSession, Address))
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1,15.f,FColor::Yellow, FString::Printf(TEXT("Connected to: %s"), *Address));

		if(APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController())
		{
			PlayerController->ClientTravel(Address, TRAVEL_Absolute);
		}
	}
}

void AMenuSystem::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}
