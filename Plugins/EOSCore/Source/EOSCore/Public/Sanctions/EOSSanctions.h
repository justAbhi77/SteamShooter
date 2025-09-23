/**
* Copyright (C) 2017-2022 | eelDev AB
*
 official EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "EOSCoreModule.h"
#include "eos_sanctions_types.h"
#include "Sanctions/EOSSanctionsTypes.h"
#include "EOSSanctions.generated.h"

/**
* The Sanctions Interface manages punitive actions taken against your users.
* Actions may include temporary or permanent bans from gameplay or communication bans that limit the social aspects of your for a particular user.
* You define the disciplinary actions for your product to handle negative behavior based on your use cases.
* The Sanctions Interface only tracks and coordinates punitive actions. You are responsible for defining and implementing your disciplinary system.
*/

UCLASS()
class EOSCORE_API UCoreSanctions : public UEOSCoreSubsystem
{
	GENERATED_BODY()
public:
	/**
	* The Sanctions Interface manages punitive actions taken against your users.
	* Actions may include temporary or permanent bans from gameplay or communication bans that limit the social aspects of your for a particular user.
	* You define the disciplinary actions for your product to handle negative behavior based on your use cases.
	* The Sanctions Interface only tracks and coordinates punitive actions. You are responsible for defining and implementing your disciplinary system.
	*/
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Interfaces", meta = (WorldContext = "WorldContextObject"))
		static UCoreSanctions* GetSanctions(UObject* WorldContextObject);
public:
	/**
	* Start an asynchronous query to retrieve any active sanctions for a specified user.
	* Call EOS_Sanctions_GetPlayerSanctionCount and EOS_Sanctions_CopyPlayerSanctionByIndex to retrieve the data.
	*
	* @param Options Structure containing the input parameters
	* @param Callback A callback that is fired when the async operation completes, either successfully or in error
	*
	* @see EOS_Sanctions_GetPlayerSanctionCount
	* @see EOS_Sanctions_CopyPlayerSanctionByIndex
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sanctions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sanctions_QueryActivePlayerSanctions"))
		void EOSSanctionsQueryActivePlayerSanctions(UObject* WorldContextObject, FEOSSanctionsQueryActivePlayerSanctionsOptions Options, const FOnQueryActivePlayerSanctionsCallback& Callback);

	/**
	* Fetch the number of player sanctions that have been retrieved for a given player.
	* You must call QueryActivePlayerSanctions first to retrieve the data from the service backend.
	*
	* @param Options Structure containing the input parameters
	*
	* @see EOS_Sanctions_QueryActivePlayerSanctions
	* @see EOS_Sanctions_CopyPlayerSanctionByIndex
	*
	* @return Number of available sanctions for this player.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sanctions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sanctions_GetPlayerSanctionCount"))
		static int32 EOSSanctionsGetPlayerSanctionCount(UObject* WorldContextObject, FEOSSanctionsGetPlayerSanctionCountOptions Options);

	/**
	* Copies an active player sanction.
	* You must call QueryActivePlayerSanctions first to retrieve the data from the service backend.
	* On success, EOS_Sanctions_PlayerSanction_Release must be called on OutSanction to free memory.
	*
	* @param Options Structure containing the input parameters
	* @param OutSanction The player sanction data for the given index, if it exists and is valid
	*
	* @see EOS_Sanctions_QueryActivePlayerSanctions
	* @see EOS_Sanctions_PlayerSanction_Release
	*
	* @return EOS_Success if the information is available and passed out in OutSanction
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_NotFound if the player achievement is not found
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Sanctions", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Sanctions_CopyPlayerSanctionByIndex"))
		static EOSResult EOSSanctionsCopyPlayerSanctionByIndex(UObject* WorldContextObject, FEOSSanctionsCopyPlayerSanctionByIndexOptions Options, FEOSSanctionsPlayerSanction& OutSanction);

private:
	static void Internal_OnQueryActivePlayerSanctionsCallback(const EOS_Sanctions_QueryActivePlayerSanctionsCallbackInfo* Data);
};
