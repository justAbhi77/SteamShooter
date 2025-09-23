/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "Sanctions/EOSSanctionsTypes.h"
#include "Core/EOSCoreAsync.h"
#include "EOSSanctionsAsyncActions.generated.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSSanctionsQueryActivePlayerSanctions
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSSanctionsQueryActivePlayerSanctions : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnQueryActivePlayerSanctionsCallbackDelegate OnCallback;
private:
	FOnQueryActivePlayerSanctionsCallback Callback;
public:
	UEOSSanctionsQueryActivePlayerSanctions() { Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSSanctionsQueryActivePlayerSanctions() { Callback.Unbind(); }
public:
	/**
	* Start an asynchronous query to retrieve any active sanctions for a specified user.
	* Call EOS_Sanctions_GetPlayerSanctionCount and EOS_Sanctions_CopyPlayerSanctionByIndex to retrieve the data.
	*
	* @param Options Structure containing the input parameters
	*
	* @see EOS_Sanctions_GetPlayerSanctionCount
	* @see EOS_Sanctions_CopyPlayerSanctionByIndex
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Sanctions|Async", DisplayName = "EOS_Sanctions_QueryActivePlayerSanctions"))
        static UEOSSanctionsQueryActivePlayerSanctions* EOSSanctionsQueryActivePlayerSanctionsAsync(UObject* WorldContextObject, FEOSSanctionsQueryActivePlayerSanctionsOptions Options);
protected:
	FEOSSanctionsQueryActivePlayerSanctionsOptions Options;
public:
	virtual void Activate() override; 
private:
	UFUNCTION()
        void HandleCallback(const FEOSSanctionsQueryActivePlayerSanctionsCallbackInfo& data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(data);
		Callback.Unbind();
		SetReadyToDestroy();
	}
};

