/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "Stats/EOSStatsTypes.h"
#include "Core/EOSCoreAsync.h"
#include "EOSStatsAsyncActions.generated.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreStatsIngestStat
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreStatsIngestStat : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
        FOnStatsIngestStatCompleteCallbackDelegate OnCallback;
private:
    FOnStatsIngestStatCompleteCallback m_Callback;
public:
    UEOSCoreStatsIngestStat() { m_Callback.BindUFunction(this, "HandleCallback"); }
    ~UEOSCoreStatsIngestStat() { m_Callback.Unbind(); }
public:
    /**
    * Ingest a stat by the amount specified in Options.
    * When the operation is complete and the delegate is triggered the stat will be uploaded to the backend to be processed.
    * The stat may not be updated immediately and an achievement using the stat may take a while to be unlocked once the stat has been uploaded.
    *
    * @param Options Structure containing information about the stat we're ingesting.
    *
    * @return EOS_Success if the operation completes successfully
    *         EOS_InvalidParameters if any of the options are incorrect
    *         EOS_InvalidUser if target user ID is missing or incorrect
    */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Stats|Async", DisplayName = "EOS_Stats_IngestStat"))
		static UEOSCoreStatsIngestStat* EOSStatsIngestStatAsync(UObject* WorldContextObject, FEOSStatsIngestStatOptions Options);
protected:
    FEOSStatsIngestStatOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
        void HandleCallback(const FEOSStatsIngestStatCompleteCallbackInfo& Data, bool bWasSuccessful)
    {
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
    }
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreStatsQueryStats
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreStatsQueryStats : public UEOSCoreAsyncAction
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintAssignable)
        FOnStatsQueryStatsCompleteCallbackDelegate OnCallback;
private:
    FOnStatsQueryStatsCompleteCallback m_Callback;
public:
    UEOSCoreStatsQueryStats() { m_Callback.BindUFunction(this, "HandleCallback"); }
    ~UEOSCoreStatsQueryStats() { m_Callback.Unbind(); }
public:
    /**
    * Query for a list of stats for a specific player.
    *
    * @param Options Structure containing information about the player whose stats we're retrieving.
    *
    * @return EOS_Success if the operation completes successfully
    *         EOS_InvalidParameters if any of the options are incorrect
    *         EOS_InvalidUser if target user ID is missing or incorrect
    */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Stats|Async", DisplayName = "EOS_Stats_QueryStats"))
        static UEOSCoreStatsQueryStats* EOSStatsQueryStatsAsync(UObject* WorldContextObject, FEOSStatsQueryStatsOptions Options);
protected:
    FEOSStatsQueryStatsOptions m_Options;
public:
	virtual void Activate() override;
private:
    UFUNCTION()
        void HandleCallback(const FEOSStatsOnQueryStatsCompleteCallbackInfo& Data, bool bWasSuccessful)
    {
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
    }
};
