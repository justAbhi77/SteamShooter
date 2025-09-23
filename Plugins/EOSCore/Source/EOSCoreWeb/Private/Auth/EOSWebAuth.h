/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "Shared/EOSWebTypes.h"
#include "EOSWebAuth.generated.h"

UCLASS()
class UEOSWebAuthLibrary : public UObject
{
	GENERATED_BODY()
public:
	/*
	* To request an access token, the client must make an HTTP request to the Epic token endpoint, passing the client credentials (Client ID and Secret) and user credentials.
	*
	* @param	GrantType		The grant type being used: exchange_code, password, refresh_token, or client_credentials.
	* @param	DeploymentId	The deployment ID that the client is trying to authenticate with. This will impact interactions with other services that require a deployment. If the deployment is not public, only users who have been entitled will be able to log in.
	* @param	Username 		The username (email address) for the account; only used with the password grant type.
	* @param	Password 		The password for the account; only used with the password grant type.
	* @param	ExchangeCode 	The exchange code passed by Launcher to the game client; only used with the exchange_code grant type.
	* @param	Code		 	The authorization code received from the authorization server.
	* @param	ClientId	 	The OAuth Client ID for your application.
	* @param	ClientSecret 	The OAuth Client Secret for your application.
	*
	* See the official documentation for more information:
	* https://dev.epicgames.com/docs/services/en-US/WebAPIRef/AuthWebAPI/index.html
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCoreWeb|Auth")
	static void RequestAuthAccessToken(const FAuthAccessTokenCallbackDelegate& Callback, FRequestAuthAccessTokenRequest Request);

	/*
	 * Initiate a Epic Account Authentication
	 * This function will open a web browser to authenticate using a Epic Account
	 * Once the user has logged in to their Epic Account in their web browser, the user will be redirected to the URI that you specify in the Request RedirectURI parameter.
	 *
	 * The plugin will then attempt to find the Authentication Code returned by Epic that you can then use to authenticate the user with.
	 *
	 * See the official documentation for more information:
	 * https://dev.epicgames.com/docs/services/en-US/WebAPIRef/AuthWebAPI/index.html
	 *
	 * Related Function: RequestAuthAccessToken
	 *
	 * Note: This feature is not available on Mobile.
	 * 
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCoreWeb|Auth")
	static void InitiateEpicAuthentication(const FInitiateEpicAuthenticationCallbackDelegate& Callback, FInitiateEpicAuthenticationRequest Request);
};
