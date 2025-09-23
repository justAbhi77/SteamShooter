/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "Leaderboards/EOSLeaderboardsTypes.h"
#include "Core/EOSCoreAsync.h"
#include "EOSLeaderboardsAsyncActions.generated.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreLeaderboardsQueryLeaderboardDefinitions
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreLeaderboardsQueryLeaderboardDefinitions : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
        FOnQueryLeaderboardDefinitionsCompleteCallbackDelegate OnCallback;
private:
    FOnQueryLeaderboardDefinitionsCompleteCallback m_Callback;
public:
    UEOSCoreLeaderboardsQueryLeaderboardDefinitions() { m_Callback.BindUFunction(this, "HandleCallback"); }
    ~UEOSCoreLeaderboardsQueryLeaderboardDefinitions() { m_Callback.Unbind(); }
public:
	/**
	* Query for a list of existing leaderboards definitions including their attributes.
	*
	* @param Options Structure containing information about the application whose leaderboard definitions we're retrieving.
	*
	* @return EOS_Success if the operation completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Leaderboards|Async", DisplayName = "EOS_Leaderboards_QueryLeaderboardDefinitions"))
		static UEOSCoreLeaderboardsQueryLeaderboardDefinitions* EOSLeaderboardsQueryLeaderboardDefinitionsAsync(UObject* WorldContextObject, FEOSLeaderboardsQueryLeaderboardDefinitionsOptions Options);
protected:
    FEOSLeaderboardsQueryLeaderboardDefinitionsOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
        void HandleCallback(const FEOSLeaderboardsOnQueryLeaderboardDefinitionsCompleteCallbackInfo& Data, bool bWasSuccessful)
    {
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
    }
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreLeaderboardsQueryLeaderboardRanks
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreLeaderboardsQueryLeaderboardRanks : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
        FOnQueryLeaderboardRanksCompleteCallbackDelegate OnCallback;
private:
    FOnQueryLeaderboardRanksCompleteCallback m_Callback;
public:
    UEOSCoreLeaderboardsQueryLeaderboardRanks() { m_Callback.BindUFunction(this, "HandleCallback"); }
    ~UEOSCoreLeaderboardsQueryLeaderboardRanks() { m_Callback.Unbind(); }
public:
	/**
	* Retrieves top leaderboard records by rank in the leaderboard matching the given leaderboard ID.
	*
	* @param Options Structure containing information about the leaderboard records we're retrieving.
	*
	* @return EOS_Success if the operation completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Leaderboards|Async", DisplayName = "EOS_Leaderboards_QueryLeaderboardRanks"))
		static UEOSCoreLeaderboardsQueryLeaderboardRanks* EOSLeaderboardsQueryLeaderboardRanksAsync(UObject* WorldContextObject, FEOSLeaderboardsQueryLeaderboardRanksOptions Options);
protected:
    FEOSLeaderboardsQueryLeaderboardRanksOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
        void HandleCallback(const FEOSLeaderboardsOnQueryLeaderboardRanksCompleteCallbackInfo& Data, bool bWasSuccessful)
    {
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
    }
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreLeaderboardsQueryLeaderboardUserScores
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreLeaderboardsQueryLeaderboardUserScores : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
        FOnQueryLeaderboardUserScoresCompleteCallbackDelegate OnCallback;
private:
    FOnQueryLeaderboardUserScoresCompleteCallback m_Callback;
public:
    UEOSCoreLeaderboardsQueryLeaderboardUserScores() { m_Callback.BindUFunction(this, "HandleCallback"); }
    ~UEOSCoreLeaderboardsQueryLeaderboardUserScores() { m_Callback.Unbind(); }
public:
	/**
	* Query for a list of scores for a given list of users.
	*
	* @param Options Structure containing information about the users whose scores we're retrieving.
	*
	* @return EOS_Success if the operation completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Leaderboards|Async", DisplayName = "EOS_Leaderboards_QueryLeaderboardUserScores"))
		static UEOSCoreLeaderboardsQueryLeaderboardUserScores* EOSLeaderboardsQueryLeaderboardUserScoresAsync(UObject* WorldContextObject, FEOSLeaderboardsQueryLeaderboardUserScoresOptions Options);
protected:
    FEOSLeaderboardsQueryLeaderboardUserScoresOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
        void HandleCallback(const FEOSLeaderboardsOnQueryLeaderboardUserScoresCompleteCallbackInfo& Data, bool bWasSuccessful)
    {
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
    }
};
