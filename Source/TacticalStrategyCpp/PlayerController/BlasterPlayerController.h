#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TacticalStrategyCpp/Enums/Team.h"
#include "BlasterPlayerController.generated.h"

// Delegate for notifying when the player's ping is too high
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHighPingDelegate, bool, bPingTooHigh);

/**
 * Responsible for managing HUD updates, server time synchronization,
 * team selection, match state handling, and handling high ping warnings.
 */
UCLASS()
class TACTICALSTRATEGYCPP_API ABlasterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ABlasterPlayerController();

	// HUD Management functions
	void GetBlasterHud();
	void SetHudHealth(float Health, float MaxHealth);
	void SetHudShield(float Shield, float MaxShield);
	void SetHudScore(float Score);
	void SetHudDefeats(int32 Defeats);
	void SetHudWeaponAmmo(int32 Ammo);
	void SetHudCarriedAmmo(int32 Ammo);
	void SetHudMatchCountDown(const float CountDownTime);
	void SetHudAnnouncementCountDown(const float CountdownTime);
	void SetHudGrenades(int32 Grenades);
	void ShowReturnToMenu();

	// Player event functions
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;
	void PollInit();
	virtual void ReceivedPlayer() override;

	// Server time syncing functions
	virtual float GetServerTime();
	void SendReqSyncServerTime();

	// Match state and team management
	void OnMatchStateSet(FName State, bool bisTeamsMatch = false);
	void DisablePlayerMechanics() const;
	void HandleCooldown();
	void HandleTeamSelection();

	// Ping handling functions
	void CheckPing(float DeltaSeconds);
	void HighPingWarning();
	void StopHighPingWarning();
	float SingleTripTime = 0;

	// Game setup functions
	void BroadcastElim(APlayerState* Attacker, APlayerState* Victim);
	void SetTeamScoreVisibility(bool bIsVisible);
	void HideTeamScores();
	void InitTeamScores();
	void SetHudRedTeamScore(int32 RedScore);
	void SetHudBlueTeamScore(int32 BlueScore);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Delegate for high ping warnings
	FHighPingDelegate HighPingDelegate;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void SetHudTime();

	// Server-to-client communication for time synchronization
	UFUNCTION(Server, Reliable)
	void Server_RequestServerTime(float TimeOfClientRequest);
	UFUNCTION(Client, Reliable)
	void Client_ReportServerTime(float TimeOfClientRequest, float TimeOfServerReceivedClientRequest);

	// Match state and team setup functions
	UFUNCTION(Server, Reliable)
	void ServerCheckMatchState();
	UFUNCTION(Client, Reliable)
	void ClientJoinMidGame(const FName StateOfMatch, const float WarmUp, const float Match,
		const float StartingTime, const float Cooldown);

	// Ping reporting to the server
	UFUNCTION(Server, Reliable)
	void ServerReportPingStatus(bool bHighPing);

	// Functions to handle team selection changes
	UFUNCTION()
	void OnTeamSelectionChanged(ETeam NewTeam);
	UFUNCTION(Server, Reliable)
	void Server_OnTeamSelectionChanged(const ETeam NewTeam);

	// Function to handle match state replication
	UFUNCTION()
	void OnRep_MatchState();

	// Event to handle HUD announcements for eliminations
	UFUNCTION(Client, Reliable)
	void Client_ElimAnnouncement(APlayerState* Attacker, APlayerState* Victim);

	// Event handler for team match state replication
	UFUNCTION()
	void OnRep_TeamsMatch();

private:
	// Pointers to other game elements (HUD and Game Mode)
	UPROPERTY()
	class ABlasterHud* BlasterHud;
	UPROPERTY()
	class ABlasterGameMode* BlasterGameMode;
	UPROPERTY()
	class UCharacterOverlay* PcCharacterOverlay;
	
	FTimerHandle BlasterHudCacheTimer;
	UFUNCTION()
	void CacheBlasterHud();

	// HUD values (for polling data until hud is initialized)
	float HudHealth, HudMaxHealth, HudScore, HudShield, HudMaxShield, HudCarriedAmmo, HudWeaponAmmo;
	int32 HudDefeats, HudGrenades;

	// Values for time management
	float MatchTime, WarmUpTime, LevelStartingTime, CooldownTime;
	uint32 CountDownInt;

	// Player ping handling variables
	UPROPERTY(EditAnywhere)
	float HighPingDuration = 5;
	UPROPERTY(EditAnywhere)
	float CheckPingFrequency = 20;
	UPROPERTY(EditAnywhere)
	float HighPingThreshold = 50;
	float HighPingRunningTime = 0, PingAnimationRunningTime = 0;

	// Time synchronization between client and server
	UPROPERTY(EditAnywhere, Category = Time)
	float TimeSyncFrequency;
	float ClientServerDelta;

	// State management for HUD initialization
	bool bInitializeHealth = false, bInitializeScore = false, bInitializeDefeats = false, bInitializeGrenades = false, bInitializeShields = false, bInitializeCarriedAmmo = false, bInitializeWeaponAmmo = false;

	// Match and team management variables
	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName MatchState;
	UPROPERTY(ReplicatedUsing = OnRep_TeamsMatch)
	bool bTeamsMatch = false;

	// UI Widgets for in-game menus and team selection
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> WReturnToMenu;
	UPROPERTY()
	class UReturnToMainMenu* WbpReturnToMenu;
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> WTeamSelection;
	UPROPERTY()
	class UTeamSelection* WbpTeamSelection;

	// Player state management
	bool bReturnToMenuOpen = false, bHasSelectedTeam = false;

	// Utility functions for displaying team and player info on the HUD
	FString GetInfoText(const TArray<class ABlasterPlayerState*>& Players, const ABlasterPlayerState* BlasterPlayerState);
	FString GetTeamsInfoText(const class ABlasterGameState* BlasterGameState);
};
