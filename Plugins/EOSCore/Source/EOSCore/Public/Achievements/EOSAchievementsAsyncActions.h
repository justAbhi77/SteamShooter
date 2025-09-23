/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "Achievements/EOSAchievementsTypes.h"
#include "Core/EOSCoreAsync.h"
#include "EOSAchievementsAsyncActions.generated.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreAchievementsQueryDefinitions
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreAchievementsQueryDefinitions : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnAchievementsQueryDefinitionsCallbackDelegate OnCallback;
private:
	FOnAchievementsQueryDefinitionsCallback m_Callback;
public:
	UEOSCoreAchievementsQueryDefinitions() { m_Callback.BindUFunction(this, "HandleCallback"); }
public:
	/**
	* Query for a list of definitions for all existing achievements, including localized text, icon IDs and whether an achievement is hidden.
	* @note When the Social Overlay is enabled then this will be called automatically.  The Social Overlay is enabled by default (see EOS_PF_DISABLE_SOCIAL_OVERLAY).
	* @param Options Structure containing information about the application whose achievement definitions we're retrieving.
	* @return EOS_Success if the operation completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Achievements|Async", DisplayName = "EOS_Achievements_QueryDefinitions"))
	static UEOSCoreAchievementsQueryDefinitions* EOSAchievementsQueryDefinitionsAsync(UObject* WorldContextObject, FEOSAchievementsQueryDefinitionsOptions Options);
protected:
	FEOSAchievementsQueryDefinitionsOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSAchievementsOnQueryDefinitionsCompleteCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);

		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreAchievementsQueryPlayerAchievements
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreAchievementsQueryPlayerAchievements : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnAchievementsQueryPlayerAchievementsCallbackDelegate OnCallback;
private:
	FOnAchievementsQueryPlayerAchievementsCallback m_Callback;
public:
	UEOSCoreAchievementsQueryPlayerAchievements() { m_Callback.BindUFunction(this, "HandleCallback"); }
public:
	/**
	* Query for a list of achievements for a specific player, including progress towards completion for each achievement.
	*
	* @note When the Social Overlay is enabled then this will be called automatically.  The Social Overlay is enabled by default (see EOS_PF_DISABLE_SOCIAL_OVERLAY).
	*
	* @param Options Structure containing information about the player whose achievements we're retrieving.
	*
	* @return EOS_Success if the operation completes successfully
	*         EOS_Invalid_ProductUserID if any of the userid options are incorrect
	*         EOS_InvalidParameters if any of the other options are incorrect
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Achievements|Async", DisplayName = "EOS_Achievements_QueryPlayerAchievements"))
	static UEOSCoreAchievementsQueryPlayerAchievements* EOSAchievementsQueryPlayerAchievementsAsync(UObject* WorldContextObject, FEOSAchievementsQueryPlayerAchievementsOptions Options);
protected:
	FEOSAchievementsQueryPlayerAchievementsOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSAchievementsOnQueryPlayerAchievementsCompleteCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);

		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreAchievementsUnlockAchievements
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreAchievementsUnlockAchievements : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnAchievementsUnlockAchievementsCallbackDelegate OnCallback;
private:
	FOnAchievementsUnlockAchievementsCallback m_Callback;
public:
	UEOSCoreAchievementsUnlockAchievements() { m_Callback.BindUFunction(this, "HandleCallback"); }
public:
	/**
	* Unlocks a number of achievements for a specific player.
	*
	* @param Options Structure containing information about the achievements and the player whose achievements we're unlocking.
	*
	* @return EOS_Success if the operation completes successfully
	*         EOS_InvalidParameters if any of the options are incorrect
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Achievements|Async", DisplayName = "EOS_Achievements_UnlockAchievements"))
	static UEOSCoreAchievementsUnlockAchievements* EOSAchievementsUnlockAchievements(UObject* WorldContextObject, FEOSAchievementsUnlockAchievementsOptions Options);
protected:
	FEOSAchievementsUnlockAchievementsOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSAchievementsOnUnlockAchievementsCompleteCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);

		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};
