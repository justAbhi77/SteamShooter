/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "Shared/EOSWebTypes.h"
#include "EOSWebPlayerTicket.generated.h"

UCLASS()
class UEOSWebPlayerTicketLibrary : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "EOSCoreWeb|TicketingSystem")
	static void SubmitTicket(FString WebApiKey, FSubmitTicketRequest Request, const FSubmitTicketCallbackDelegate& Callback);
};
