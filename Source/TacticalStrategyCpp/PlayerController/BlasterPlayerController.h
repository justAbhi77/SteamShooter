#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlasterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALSTRATEGYCPP_API ABlasterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ABlasterPlayerController();
	
	void SetHudHealth(float Health, float MaxHealth);
	void SetHudShield(float Shield, float MaxShield);
	
	void SetHudScore(float Score);

	void SetHudDefeats(int32 Defeats);
	
	void SetHudWeaponAmmo(int32 Ammo);
	
	void SetHudCarriedAmmo(int32 Ammo);

	void SetHudMatchCountDown(const float CountDownTime);

	void SetHudAnnouncementCountDown(const float CountdownTime);

	void SetHudGrenades(int32 Grenades);

	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaSeconds) override;

	/*
	 * Time on the Server. This is synced up with server
	 */
	virtual float GetServerTime();
	void SendReqSyncServerTime();

	virtual void ReceivedPlayer() override;

	void OnMatchStateSet(FName State);

	void HandleCooldown();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:	
	virtual void BeginPlay() override;

	void SetHudTime();

	// Time Syncing between Server and clients

	/*
	 * Requests the server for its time, passing the clients time
	 */
	UFUNCTION(Server, Reliable)
	void Server_RequestServerTime(float TimeOfClientRequest);

	/*
	 * Reply to the client with the time, passing the clients request time as well
	 */
	UFUNCTION(Client , Reliable)
	void Client_ReportServerTime(float TimeOfClientRequest, float TimeOfServerReceivedClientRequest);

	/*
	 * The time difference between server and client
	 */
	float ClientServerDelta;

	UPROPERTY(EditAnywhere, Category = Time)
	float TimeSyncFrequency;

	void PollInit();

	UFUNCTION(Server, Reliable)
	void ServerCheckMatchState();

	UFUNCTION(Client, Reliable)
	void ClientJoinMidGame(const FName StateOfMatch, const float WarmUp, const float Match,
		const float StartingTime, const float Cooldown);

	void CheckPing(float DeltaSeconds);
	void HighPingWarning();
	void StopHighPingWarning();
	
private:
	UPROPERTY()
	class ABlasterHud* BlasterHud;

	UPROPERTY()
	class ABlasterGameMode* BlasterGameMode;

	float MatchTime, WarmUpTime, LevelStartingTime, CooldownTime;

	uint32 CountDownInt;

	UFUNCTION()
	void OnRep_MatchState();
	
	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName MatchState;

	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;

	float HudHealth, HudMaxHealth, HudScore, HudShield, HudMaxShield, HudCarriedAmmo, HudWeaponAmmo,
		HighPingRunningTime = 0, PingAnimationRunningTime = 0;
	int32 HudDefeats, HudGrenades;

	bool bInitializeHealth = false, bInitializeScore = false, bInitializeDefeats = false, bInitializeGrenades = false,
		bInitializeShields = false, bInitializeCarriedAmmo = false, bInitializeWeaponAmmo = false;

	UPROPERTY(EditAnywhere)
	float HighPingDuration = 5;
	
	UPROPERTY(EditAnywhere)
	float CheckPingFrequency = 20;
	
	UPROPERTY(EditAnywhere)
	float HighPingThreshold = 50;
};
