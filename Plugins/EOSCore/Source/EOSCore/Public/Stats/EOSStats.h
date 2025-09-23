/**
* Copyright (C) 2017-2022 | eelDev AB
*
 official EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "EOSCoreModule.h"
#include "eos_stats_types.h"
#include "Stats/EOSStatsTypes.h"
#include "EOSStats.generated.h"

/**
* The Stats Interface manages user stats like number of items collected, fastest completion time for a level, number of wins/losses, number of times that a user has performed a certain action, and so on.
* You can use stats to determine when to unlock achievements and how to use rank users in leaderboards.
* All Stats Interface calls take a handle of type EOS_HStats as the first parameter.
* This handle can be retrieved from an EOS_HPlatform handle by using the EOS_Platform_GetStatsInterface function.
* 
* @see EOS_Platform_GetStatsInterface
*/

UCLASS()
class EOSCORE_API UCoreStats : public UEOSCoreSubsystem
{
	GENERATED_BODY()
public:
	/**
	* The Stats Interface manages user stats like number of items collected, fastest completion time for a level, number of wins/losses, number of times that a user has performed a certain action, and so on.
	* You can use stats to determine when to unlock achievements and how to use rank users in leaderboards.
	* All Stats Interface calls take a handle of type EOS_HStats as the first parameter.
	* This handle can be retrieved from an EOS_HPlatform handle by using the EOS_Platform_GetStatsInterface function.
	* 
	* @see EOS_Platform_GetStatsInterface
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Interfaces", meta = (WorldContext = "WorldContextObject"))
	static UCoreStats* GetStats(UObject* WorldContextObject);

public:
	/**
	* Ingest a stat by the amount specified in Options.
	* When the operation is complete and the delegate is triggered the stat will be uploaded to the backend to be processed.
	* The stat may not be updated immediately and an achievement using the stat may take a while to be unlocked once the stat has been uploaded.
	*
	* @param Options Structure containing information about the stat we're ingesting.
	* @param Callback This function is called when the ingest stat operation completes.
	*
	* @return EOS_Success if the operation completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_InvalidUser if target user ID is missing or incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Stats", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Stats_IngestStat"))
	void EOSStatsIngestStat(UObject* WorldContextObject, FEOSStatsIngestStatOptions Options, const FOnStatsIngestStatCompleteCallback& Callback);

	/**
	* Query for a list of stats for a specific player.
	*
	* @param Options Structure containing information about the player whose stats we're retrieving.
	* @param Callback This function is called when the query player stats operation completes.
	*
	* @return EOS_Success if the operation completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*         EOS_InvalidUser if target user ID is missing or incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Stats", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Stats_QueryStats"))
	void EOSStatsQueryStats(UObject* WorldContextObject, FEOSStatsQueryStatsOptions Options, const FOnStatsQueryStatsCompleteCallback& Callback);

	/**
	* Fetch the number of stats that are cached locally.
	*
	* @param Options The Options associated with retrieving the stat count
	*
	* @see EOS_Stats_CopyStatByIndex
	*
	* @return Number of stats or 0 if there is an error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Stats", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Stats_GetStatsCount"))
	static int32 EOSStatsGetStatsCount(UObject* WorldContextObject, FEOSStatsGetStatCountOptions Options);

	/**
	* Fetches a stat from a given index. Use EOS_Stats_Stat_Release when finished with the data.
	*
	* @param Options Structure containing the Epic Online Services Account ID and index being accessed
	* @param OutStat The stat data for the given index, if it exists and is valid
	*
	* @see EOS_Stats_Stat_Release
	*
	* @return EOS_Success if the information is available and passed out in OutStat
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_NotFound if the stat is not found
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Stats", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Stats_CopyStatByIndex"))
	static EOSResult EOSStatsCopyStatByIndex(UObject* WorldContextObject, FEOSStatsCopyStatByIndexOptions Options, FEOSStatsStat& OutStat);

	/**
	* Fetches a stat from cached stats by name. Use EOS_Stats_Stat_Release when finished with the data.
	*
	* @param Options Structure containing the Epic Online Services Account ID and name being accessed
	* @param OutStat The stat data for the given name, if it exists and is valid
	*
	* @see EOS_Stats_Stat_Release
	*
	* @return EOS_Success if the information is available and passed out in OutStat
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_NotFound if the stat is not found
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Stats", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Stats_CopyStatByName"))
	static EOSResult EOSStatsCopyStatByName(UObject* WorldContextObject, FEOSStatsCopyStatByNameOptions Options, FEOSStatsStat& OutStat);
private:
	static void Internal_OnIngestStatCallback(const EOS_Stats_IngestStatCompleteCallbackInfo* Data);
	static void Internal_OnQueryStatsCallback(const EOS_Stats_OnQueryStatsCompleteCallbackInfo* Data);
};
