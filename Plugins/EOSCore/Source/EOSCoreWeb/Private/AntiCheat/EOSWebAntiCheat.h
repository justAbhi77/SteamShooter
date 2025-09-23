/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "Shared/EOSWebTypes.h"
#include "EOSWebAntiCheat.generated.h"

UCLASS()
class UEOSWebAntiCheatLibrary : public UObject
{
	GENERATED_BODY()
public:
	/*
	* The client policy used must have the anticheat:authenticateForAnyUser or anticheat:authenticateForLocalUser actions allowed.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCoreWeb|AntiCheat")
	static void QueryAntiCheatServiceStatusByDeployment(FString AccessToken, FString DeploymentId, const FAntiCheatStatusCallbackDelegate& Callback);
};
