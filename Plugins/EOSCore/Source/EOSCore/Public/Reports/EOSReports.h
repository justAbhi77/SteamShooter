/**
* Copyright (C) 2017-2022 | eelDev AB
*
 official EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "EOSCoreModule.h"
#include "eos_reports_types.h"
#include "Reports/EOSReportsTypes.h"
#include "EOSReports.generated.h"

/**
* The Reports Interface stores in-game user behavior reports.
* You can query these user reports from your Developer Portal to view the details of reported user behaviors, for example, cheating, verbal abuse, offensive profiles, and so on.
* This interface provides information that you can then use with the Sanctions interface to conduct any punitive actions.
*/

UCLASS()
class EOSCORE_API UCoreReports : public UEOSCoreSubsystem
{
	GENERATED_BODY()
public:
	/**
	* The following EOS_Reports_* functions allow you to send reports directly from the players
	* to the back-end services without having to route these through a game server.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Interfaces", meta = (WorldContext = "WorldContextObject"))
	static UCoreReports* GetReports(UObject* WorldContextObject);

public:
	/**
	* Sends the provided report directly to the Epic Online Services back-end.
	*
	* @param Options Structure containing the player report information.
	* @param Callback This function is called when the send operation completes.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Reports", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Reports_SendPlayerBehaviorReport"))
	void EOSReportsSendPlayerBehaviorReport(UObject* WorldContextObject, FEOSReportsSendPlayerBehaviorReportOptions Options, const FOnSendPlayerBehaviorReportCompleteCallback& Callback);
private:
	static void Internal_OnSendPlayerBehaviorReportCallback(const EOS_Reports_SendPlayerBehaviorReportCompleteCallbackInfo* Data);
};
