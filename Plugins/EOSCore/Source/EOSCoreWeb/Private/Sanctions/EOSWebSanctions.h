/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "Shared/EOSWebTypes.h"
#include "EOSWebSanctions.generated.h"

UCLASS()
class UEOSWebSanctionsLibrary : public UObject
{
	GENERATED_BODY()
public:
	/*
	* The client policy used must have the anticheat:authenticateForAnyUser or anticheat:authenticateForLocalUser actions allowed.
	*
	* This call requires Bearer Token authorization with an EOS Client Auth access token, obtained from the Connect interface.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCoreWeb|Sanctions")
	static void QueryActiveSanctionsForSpecificPlayer(FString AccessToken, FString ProductUserId, FString QueryParameters, const FSanctionsQueryActiveSanctionsForSpecificPlayerCallbackDelegate& Callback);

	/*
	* The client policy used must have the Sanctions:syncSanctionEvents action allowed.
	*
	* This call requires Bearer Token authorization with an EOS Client Auth access token, obtained from the Connect interface.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCoreWeb|Sanctions")
	static void SynchronizeSanctionsToAnExternalService(FString AccessToken, FString LastLogId, const FSynchronizeSanctionsToAnExternalServiceCallbackDelegate& Callback);

	/*
	 * The client policy used must have the sanctions:createSanction action allowed.
	 *
	 * This call requires Bearer Token authorization with an EOS Client Auth access token, obtained from the Connect interface.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCoreWeb|Sanctions")
	static void CreateSanction(FString AccessToken, FString DeploymentId, FSanctionPostPayload SanctionPostPayload, const FCreateSanctionsCallbackDelegate& Callback);

	/*
	* The client policy used must have one of the following actions allowed:
	*
	* sanctions:findSanctionsForAnyUser
	* sanctions:findAllSanctions
	* sanctions:syncSanctionEvents
	*
	* This call requires Bearer Token authorization with an EOS Client Auth access token, obtained from the Connect interface.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCoreWeb|Sanctions")
	static void QuerySanctions(FString AccessToken, FString DeploymentId, int32 Limit, int32 Offset, const FQuerySanctionsCallbackDelegate& Callback);

	/*
	* The client policy used must have one of the following actions allowed:
	* 
	* sanctions:findSanctionsForAnyUser
	* sanctions:findSanctionsForLocalUser
	* sanctions:findAllSanctions
	* sanctions:syncSanctionEvents
	* 
	* This call requires Bearer Token authorization with an EOS Client Auth access token, obtained from the Connect interface.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCoreWeb|Sanctions")
	static void QuerySanctionsForPlayer(FString AccessToken, FString ProductUserId, FString DeploymentId, int32 Limit, int32 Offset, const FQuerySanctionsForPlayerCallbackDelegate& Callback);

	/*
	 * The client policy used must have the sanctions:deleteSanction action allowed.
	 *
	 * This call requires Bearer Token authorization with an EOS Client Auth access token, obtained from the Connect interface.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCoreWeb|Sanctions")
	static void RemoveSanction(FString AccessToken, FString DeploymentId, FString ReferenceId, const FRemoveSanctionCallbackDelegate& Callback);

	/*
	 * The client policy used must have the sanctions:createAppealsForAnyUser action allowed.
	 *
	 * This call requires Bearer Token authorization with an EOS Client Auth access token.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCoreWeb|Sanctions")
	static void CreateSanctionAppeal(FString AccessToken, FString DeploymentId, FAppealPostPayload Request, const FCreateSanctionAppealCallbackDelegate& Callback);

	/*
	 * The client policy used must have the sanctions:findAppealsForAnyUser action allowed.
	 *
	 * This call requires Bearer Token authorization with an EOS Client Auth access token.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCoreWeb|Sanctions")
	static void QuerySanctionAppeals(FString AccessToken, FString DeploymentId, FQuerySanctionAppeals Request, const FQuerySanctionAppealsCallbackDelegate& Callback);
};
