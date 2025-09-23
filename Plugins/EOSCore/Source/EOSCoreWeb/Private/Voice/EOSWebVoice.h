/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "Shared/EOSWebTypes.h"
#include "EOSWebVoice.generated.h"

UCLASS()
class UEOSWebVoiceLibrary : public UObject
{
	GENERATED_BODY()
public:
	/*
	* Create a Room Token that the player will use to join a specific room.
	* 
	* @param AccessToken The AccessToken returned from RequestAccessToken function
	* @param ProductUserId The ProductUserId of the user who wishes to join the chat room
	* @param DeploymentId Deployment Id of your application
	* @param RoomName The name of the room the user wishes to join
	* @param ClientIp will be used to determine the best possible location for the client to connect to
	* 
	* You can use GetPublicIp function to get the clients Ip
	* 
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCoreWeb|Voice")
	static void CreateRoomToken(FString AccessToken, TArray<FWebRequestParticipantData> Participants, FString DeploymentId, FString RoomName, const FCreateRoomTokenCallbackDelegate& Callback);

	/*
	* The removeParticipant request will remove a player from the voice room and revoke their room token, preventing them from reusing their existing token to join the same room again.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCoreWeb|Voice")
	static void RemoveParticipant(FString AccessToken, FString ProductUserId, FString DeploymentId, FString RoomName, const FRemoveParticipantCallbackDelegate& Callback);

	UFUNCTION(BlueprintCallable, Category = "EOSCoreWeb|Voice")
	static void ModifyParticipant(FString AccessToken, FString ProductUserId, FString DeploymentId, FString RoomName, bool bHardMuted, const FModifyParticipantCallbackDelegate& Callback);
};
