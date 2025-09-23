/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "EOSCoreModule.h"
#include "Achievements/EOSAchievementsTypes.h"
#include "EOSAchievements.generated.h"

/**
 * The following EOS_Achievements_* functions allow you to query existing achievement definitions that have been defined for your application.
 * You can also query achievement progress data for users.
 * In addition, you can also unlock one or more achievements directly.
 * You can also receive notifications when achievements are unlocked.
 */

UCLASS()
class EOSCORE_API UCoreAchievements : public UEOSCoreSubsystem
{
	GENERATED_BODY()
public:
	virtual void Deinitialize() override;
public:
	/**
	 * The following EOS_Achievements_* functions allow you to query existing achievement definitions that have been defined for your application.
	 * You can also query achievement progress data for users.
	 * In addition, you can also unlock one or more achievements directly.
	 * You can also receive notifications when achievements are unlocked.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Interfaces", meta = (WorldContext = "WorldContextObject"))
	static UCoreAchievements* GetAchievements(UObject* WorldContextObject);

public:
	/**
	* Query for a list of definitions for all existing achievements, including localized text, icon IDs and whether an achievement is hidden.
	*
	* @note When the Social Overlay is enabled then this will be called automatically.  The Social Overlay is enabled by default (see EOS_PF_DISABLE_SOCIAL_OVERLAY).
	*
	* @param Options Structure containing information about the application whose achievement definitions we're retrieving.
	* @param Callback This function is called when the query definitions operation completes.
	*
	* @return EOS_Success if the operation completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Achievements", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Achievements_QueryDefinitions"))
	void EOSAchievementsQueryDefinitions(UObject* WorldContextObject, FEOSAchievementsQueryDefinitionsOptions Options, const FOnAchievementsQueryDefinitionsCallback& Callback);

	/**
	* Fetch the number of achievement definitions that are cached locally.
	*
	* @param Options The Options associated with retrieving the achievement definition count
	*
	* @see EOS_Achievements_CopyAchievementDefinitionByIndex
	*
	* @return Number of achievement definitions or 0 if there is an error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Achievements", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Achievements_GetAchievementDefinitionCount"))
	static int32 EOSAchievementsGetAchievementDefinitionCount(UObject* WorldContextObject, FEOSAchievementsGetAchievementDefinitionCountOptions Options);

	/**
	* Fetches an achievement definition from a given index.
	*
	* @param Options Structure containing the index being accessed
	* @param OutDefinition The achievement definition for the given index, if it exists and is valid, use EOS_Achievements_Definition_Release when finished
	*
	* @see EOS_Achievements_DefinitionV2_Release
	*
	* @return EOS_Success if the information is available and passed out in OutDefinition
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_NotFound if the achievement definition is not found
	*         EOS_Invalid_ProductUserID if any of the userid options are incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Achievements", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Achievements_CopyAchievementDefinitionV2ByIndex"))
	static EOSResult EOSAchievementsCopyAchievementDefinitionV2ByIndex(UObject* WorldContextObject, FEOSAchievementsCopyAchievementDefinitionV2ByIndexOptions Options, FEOSAchievementsDefinitionV2& OutDefinition);

	/**
	* Fetches an achievement definition from a given achievement ID.
	*
	* @param Options Structure containing the achievement ID being accessed
	* @param OutDefinition The achievement definition for the given achievement ID, if it exists and is valid, use EOS_Achievements_Definition_Release when finished
	*
	* @see EOS_Achievements_DefinitionV2_Release
	*
	* @return EOS_Success if the information is available and passed out in OutDefinition
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_NotFound if the achievement definition is not found
	*         EOS_Invalid_ProductUserID if any of the userid options are incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Achievements", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Achievements_CopyAchievementDefinitionV2ByAchievementId"))
	static EOSResult EOSAchievementsCopyAchievementDefinitionV2ByAchievementId(UObject* WorldContextObject, FEOSAchievementsCopyAchievementDefinitionV2ByAchievementIdOptions Options, FEOSAchievementsDefinitionV2& OutDefinition);

	/**
	* Query for a list of achievements for a specific player, including progress towards completion for each achievement.
	*
	* @note When the Social Overlay is enabled then this will be called automatically.  The Social Overlay is enabled by default (see EOS_PF_DISABLE_SOCIAL_OVERLAY).
	*
	* @param Options Structure containing information about the player whose achievements we're retrieving.
	* @param Callback This function is called when the query player achievements operation completes.
	*
	* @return EOS_Success if the operation completes successfully
	*         EOS_Invalid_ProductUserID if any of the userid options are incorrect
	*         EOS_InvalidParameters if any of the other options are incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Achievements", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Achievements_QueryPlayerAchievements"))
	void EOSAchievementsQueryPlayerAchievements(UObject* WorldContextObject, FEOSAchievementsQueryPlayerAchievementsOptions Options, const FOnAchievementsQueryPlayerAchievementsCallback& Callback);

	/**
	* Fetch the number of player achievements that are cached locally.
	*
	* @param Options The Options associated with retrieving the player achievement count
	*
	* @see EOS_Achievements_CopyPlayerAchievementByIndex
	*
	* @return Number of player achievements or 0 if there is an error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Achievements", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Achievements_GetPlayerAchievementCount"))
	static int32 EOSAchievementsGetPlayerAchievementCount(UObject* WorldContextObject, FEOSAchievementsGetPlayerAchievementCountOptions Options);

	/**
	* Fetches a player achievement from a given index.
	*
	* @param Options Structure containing the Epic Online Services Account ID and index being accessed
	* @param OutAchievement The player achievement data for the given index, if it exists and is valid, use EOS_Achievements_PlayerAchievement_Release when finished
	*
	* @see EOS_Achievements_PlayerAchievement_Release
	*
	* @return EOS_Success if the information is available and passed out in OutAchievement
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_NotFound if the player achievement is not found
	*         EOS_Invalid_ProductUserID if you pass an invalid user ID
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Achievements", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Achievements_CopyPlayerAchievementByIndex"))
	static EOSResult EOSAchievementsCopyPlayerAchievementByIndex(UObject* WorldContextObject, FEOSAchievementsCopyPlayerAchievementByIndexOptions Options, FEOSAchievementsPlayerAchievement& OutAchievement);

	/**
	* Unlocks a number of achievements for a specific player.
	*
	* @param Options Structure containing information about the achievements and the player whose achievements we're unlocking.
	* @param Callback This function is called when the unlock achievements operation completes.
	*
	* @return EOS_Success if the operation completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Achievements", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Achievements_CopyPlayerAchievementByAchievementId"))
	void EOSAchievementsUnlockAchievements(UObject* WorldContextObject, FEOSAchievementsUnlockAchievementsOptions Options, const FOnAchievementsUnlockAchievementsCallback& Callback);

	/**
	* Register to receive achievement unlocked notifications.
	* @note must call EOS_Achievements_RemoveNotifyAchievementsUnlocked to remove the notification
	*
	* @see EOS_Achievements_RemoveNotifyAchievementsUnlocked
	*
	* @param Callback Arbitrary data that is passed back to you in the CompletionDelegate
	*
	* @return handle representing the registered callback
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Achievements", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Achievements_AddNotifyAchievementsUnlockedV2"))
	static FEOSNotificationId EOSAchievementsAddNotifyAchievementsUnlockedV2(UObject* WorldContextObject, const FOnAchievementsUnlockedV2Callback& Callback);

	/**
	* Unregister from receiving achievement unlocked notifications.
	*
	* @see EOS_Achievements_AddNotifyAchievementsUnlocked
	*
	* @param Id Handle representing the registered callback
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Achievements", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Achievements_RemoveNotifyAchievementsUnlocked"))
	static void EOSAchievementsRemoveNotifyAchievementsUnlocked(UObject* WorldContextObject, FEOSNotificationId Id);
private:
	static void Internal_OnQueryDefinitionsCallback(const EOS_Achievements_OnQueryDefinitionsCompleteCallbackInfo* Data);
	static void Internal_OnQueryPlayerAchievementsCallback(const EOS_Achievements_OnQueryPlayerAchievementsCompleteCallbackInfo* Data);
	static void Internal_OnUnlockAchievementsCallback(const EOS_Achievements_OnUnlockAchievementsCompleteCallbackInfo* Data);

protected:
	TMap<FEOSNotificationId, FAchievementsUnlockedV2Callback*> m_AchievementsUnlockedV2Callbacks;
};
