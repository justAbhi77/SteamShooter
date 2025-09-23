/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "Reports/EOSReportsTypes.h"
#include "Core/EOSCoreAsync.h"
#include "EOSReportsAsyncActions.generated.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSReportsSendPlayerBehaviorReport
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSReportsSendPlayerBehaviorReport : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnSendPlayerBehaviorReportCompleteDelegate OnCallback;
private:
	FOnSendPlayerBehaviorReportCompleteCallback Callback;
public:
	UEOSReportsSendPlayerBehaviorReport() { Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSReportsSendPlayerBehaviorReport() { Callback.Unbind(); }
public:
	/**
	* Sends the provided report directly to the Epic Online Services back-end.
	*
	* @param Options Structure containing the player report information.
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Reports|Async", DisplayName = "EOS_Reports_SendPlayerBehaviorReport"))
        static UEOSReportsSendPlayerBehaviorReport* EOSReportsSendPlayerBehaviorReportAsync(UObject* WorldContextObject, FEOSReportsSendPlayerBehaviorReportOptions Options);
protected:
	FEOSReportsSendPlayerBehaviorReportOptions Options;
public:
	virtual void Activate() override; 
private:
	UFUNCTION()
        void HandleCallback(const FEOSReportsSendPlayerBehaviorReportCompleteCallbackInfo& data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(data);
		Callback.Unbind();
		SetReadyToDestroy();
	}
};

