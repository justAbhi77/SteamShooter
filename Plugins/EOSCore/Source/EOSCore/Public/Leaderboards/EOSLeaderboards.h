/**
* Copyright (C) 2017-2022 | eelDev AB
*
 official EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "EOSCoreModule.h"
#include "eos_leaderboards.h"
#include "Leaderboards/EOSLeaderboardsTypes.h"
#include "EOSLeaderboards.generated.h"

/**
 * The following EOSLeaderboards* functions allow you to query existing leaderboard definitions that have been defined for your application.
 * You can retrieve a list of scores for the top users for each Leaderboard.
 * You can also query scores for one or more users.
 */

UCLASS()
class EOSCORE_API UCoreLeaderboards : public UEOSCoreSubsystem
{
	GENERATED_BODY()
public:
	virtual void Deinitialize() override;
public:
	/**
	 * The following EOSLeaderboards* functions allow you to query existing leaderboard definitions that have been defined for your application.
	 * You can retrieve a list of scores for the top users for each Leaderboard.
	 * You can also query scores for one or more users.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Interfaces", meta = (WorldContext = "WorldContextObject"))
	static UCoreLeaderboards* GetLeaderboards(UObject* WorldContextObject);

public:
	/**
	* Query for a list of existing leaderboards definitions including their attributes.
	*
	* @param Options Structure containing information about the application whose leaderboard definitions we're retrieving.
	* @param Callback This function is called when the query operation completes.
	*
	* @return EOS_Success if the operation completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Leaderboards", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Leaderboards_QueryLeaderboardDefinitions"))
	void EOSLeaderboardsQueryLeaderboardDefinitions(UObject* WorldContextObject, FEOSLeaderboardsQueryLeaderboardDefinitionsOptions Options, const FOnQueryLeaderboardDefinitionsCompleteCallback& Callback);

	/**
	* Fetch the number of leaderboards definitions that are cached locally.
	*
	* @param Options The Options associated with retrieving the leaderboard count.
	*
	* @see EOS_Leaderboards_CopyLeaderboardDefinitionByIndex
	* @see EOS_Leaderboards_CopyLeaderboardDefinitionByLeaderboardId
	*
	* @return Number of leaderboards or 0 if there is an error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Leaderboards", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Leaderboards_GetLeaderboardDefinitionCount"))
	static int32 EOSLeaderboardsGetLeaderboardDefinitionCount(UObject* WorldContextObject, FEOSLeaderboardsGetLeaderboardDefinitionCountOptions Options);

	/**
	* Fetches a leaderboard definition from the cache using an index.
	*
	* @param Options Structure containing the index being accessed.
	* @param OutLeaderboardDefinition The leaderboard data for the given index, if it exists and is valid, use EOS_Leaderboards_Definition_Release when finished.
	*
	* @see EOS_Leaderboards_Definition_Release
	*
	* @return EOS_Success if the information is available and passed out in OutLeaderboardDefinition
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_NotFound if the leaderboard is not found
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Leaderboards", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Leaderboards_CopyLeaderboardDefinitionByIndex"))
	static EOSResult EOSLeaderboardsCopyLeaderboardDefinitionByIndex(UObject* WorldContextObject, FEOSLeaderboardsCopyLeaderboardDefinitionByIndexOptions Options, FEOSLeaderboardsDefinition& OutLeaderboardDefinition);

	/**
	* Fetches a leaderboard definition from the cache using a leaderboard ID.
	*
	* @param Options Structure containing the leaderboard ID being accessed.
	* @param OutLeaderboardDefinition The leaderboard definition for the given leaderboard ID, if it exists and is valid, use EOS_Leaderboards_Definition_Release when finished.
	*
	* @see EOS_Leaderboards_Definition_Release
	*
	* @return EOS_Success if the information is available and passed out in OutLeaderboardDefinition
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_NotFound if the leaderboard data is not found
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Leaderboards", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Leaderboards_CopyLeaderboardDefinitionByLeaderboardId"))
	static EOSResult EOSLeaderboardsCopyLeaderboardDefinitionByLeaderboardId(UObject* WorldContextObject, FEOSLeaderboardsCopyLeaderboardDefinitionByLeaderboardIdOptions Options, FEOSLeaderboardsDefinition& OutLeaderboardDefinition);

	/**
	* Retrieves top leaderboard records by rank in the leaderboard matching the given leaderboard ID.
	*
	* @param Options Structure containing information about the leaderboard records we're retrieving.
	* @param Callback This function is called when the query operation completes.
	*
	* @return EOS_Success if the operation completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Leaderboards", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Leaderboards_QueryLeaderboardRanks"))
	void EOSLeaderboardsQueryLeaderboardRanks(UObject* WorldContextObject, FEOSLeaderboardsQueryLeaderboardRanksOptions Options, const FOnQueryLeaderboardRanksCompleteCallback& Callback);

	/**
	* Fetch the number of leaderboard records that are cached locally.
	*
	* @param Options The Options associated with retrieving the leaderboard record count.
	*
	* @see EOS_Leaderboards_CopyLeaderboardRecordByIndex
	* @see EOS_Leaderboards_CopyLeaderboardRecordByUserId
	*
	* @return Number of leaderboard records or 0 if there is an error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Leaderboards", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Leaderboards_GetLeaderboardRecordCount"))
	static int32 EOSLeaderboardsGetLeaderboardRecordCount(UObject* WorldContextObject, FEOSLeaderboardsGetLeaderboardRecordCountOptions Options);

	/**
	* Fetches a leaderboard record from a given index.
	*
	* @param Options Structure containing the index being accessed.
	* @param OutLeaderboardRecord The leaderboard record for the given index, if it exists and is valid, use EOS_Leaderboards_LeaderboardRecord_Release when finished.
	*
	* @see EOS_Leaderboards_LeaderboardRecord_Release
	*
	* @return EOS_Success if the leaderboard record is available and passed out in OutLeaderboardRecord
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_NotFound if the leaderboard is not found
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Leaderboards", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Leaderboards_CopyLeaderboardRecordByIndex"))
	static EOSResult EOSLeaderboardsCopyLeaderboardRecordByIndex(UObject* WorldContextObject, FEOSLeaderboardsCopyLeaderboardRecordByIndexOptions Options, FEOSLeaderboardsLeaderboardRecord& OutLeaderboardRecord);

	/**
	* Fetches a leaderboard record from a given user ID.
	*
	* @param Options Structure containing the user ID being accessed.
	* @param OutLeaderboardRecord The leaderboard record for the given user ID, if it exists and is valid, use EOS_Leaderboards_LeaderboardRecord_Release when finished.
	*
	* @see EOS_Leaderboards_LeaderboardRecord_Release
	*
	* @return EOS_Success if the leaderboard record is available and passed out in OutLeaderboardRecord
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_NotFound if the leaderboard data is not found
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Leaderboards", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Leaderboards_CopyLeaderboardRecordByUserId"))
	static EOSResult EOSLeaderboardsCopyLeaderboardRecordByUserId(UObject* WorldContextObject, FEOSLeaderboardsCopyLeaderboardRecordByUserIdOptions Options, FEOSLeaderboardsLeaderboardRecord& OutLeaderboardRecord);

	/**
	* Query for a list of scores for a given list of users.
	*
	* @param Options Structure containing information about the users whose scores we're retrieving.
	* @param Callback This function is called when the query operation completes.
	*
	* @return EOS_Success if the operation completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Leaderboards", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Leaderboards_QueryLeaderboardUserScores"))
	void EOSLeaderboardsQueryLeaderboardUserScores(UObject* WorldContextObject, FEOSLeaderboardsQueryLeaderboardUserScoresOptions Options, const FOnQueryLeaderboardUserScoresCompleteCallback& Callback);

	/**
	* Fetch the number of leaderboard user scores that are cached locally.
	*
	* @param Options The Options associated with retrieving the leaderboard user scores count.
	*
	* @see EOS_Leaderboards_CopyLeaderboardUserScoreByIndex
	* @see EOS_Leaderboards_CopyLeaderboardUserScoreByUserId
	*
	* @return Number of leaderboard records or 0 if there is an error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Leaderboards", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Leaderboards_GetLeaderboardUserScoreCount"))
	static int32 EOSLeaderboardsGetLeaderboardUserScoreCount(UObject* WorldContextObject, FEOSLeaderboardsGetLeaderboardUserScoreCountOptions Options);

	/**
	* Fetches leaderboard user score from a given index.
	*
	* @param Options Structure containing the index being accessed.
	* @param OutLeaderboardUserScore The leaderboard user score for the given index, if it exists and is valid, use EOS_Leaderboards_LeaderboardUserScore_Release when finished.
	*
	* @see EOS_Leaderboards_LeaderboardUserScore_Release
	*
	* @return EOS_Success if the leaderboard scores are available and passed out in OutLeaderboardUserScore
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_NotFound if the leaderboard user scores are not found
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Leaderboards", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Leaderboards_CopyLeaderboardUserScoreByIndex"))
	static EOSResult EOSLeaderboardsCopyLeaderboardUserScoreByIndex(UObject* WorldContextObject, FEOSLeaderboardsCopyLeaderboardUserScoreByIndexOptions Options, FEOSLeaderboardsLeaderboardUserScore& OutLeaderboardUserScore);

	/**
	* Fetches leaderboard user score from a given user ID.
	*
	* @param Options Structure containing the user ID being accessed.
	* @param OutLeaderboardUserScore The leaderboard user score for the given user ID, if it exists and is valid, use EOS_Leaderboards_LeaderboardUserScore_Release when finished.
	*
	* @see EOS_Leaderboards_LeaderboardUserScore_Release
	*
	* @return EOS_Success if the leaderboard scores are available and passed out in OutLeaderboardUserScore
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_NotFound if the leaderboard user scores are not found
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Leaderboards", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Leaderboards_CopyLeaderboardUserScoreByUserId"))
	static EOSResult EOSLeaderboardsCopyLeaderboardUserScoreByUserId(UObject* WorldContextObject, FEOSLeaderboardsCopyLeaderboardUserScoreByUserIdOptions Options, FEOSLeaderboardsLeaderboardUserScore& OutLeaderboardUserScore);
private:
	static void Internal_OnQueryLeaderboardDefinitionsCallback(const EOS_Leaderboards_OnQueryLeaderboardDefinitionsCompleteCallbackInfo* Data);
	static void Internal_OnQueryLeaderboardRanksCallback(const EOS_Leaderboards_OnQueryLeaderboardRanksCompleteCallbackInfo* Data);
	static void Internal_OnQueryLeaderboardUserScoresCallback(const EOS_Leaderboards_OnQueryLeaderboardUserScoresCompleteCallbackInfo* Data);
};
