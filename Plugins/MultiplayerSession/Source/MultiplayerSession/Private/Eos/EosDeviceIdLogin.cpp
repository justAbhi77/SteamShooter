

#include "Eos/EosDeviceIdLogin.h"
#include "ThirdParty/EOSSDK/SDK/Bin/Android/include/eos_init.h"
#include "ThirdParty/EOSSDK/SDK/Bin/Android/include/eos_sdk.h"

UEosDeviceIdLogin* UEosDeviceIdLogin::EosDeviceIdLogin()
{
	UEosDeviceIdLogin* EosDeviceIdLogin = NewObject<UEosDeviceIdLogin>();

	return EosDeviceIdLogin;
}

void UEosDeviceIdLogin::Activate()
{
	Super::Activate();

	CreateDeviceId();
}

void UEosDeviceIdLogin::EndTask()
{
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UEosDeviceIdLogin::Success(const EOS_Connect_CreateDeviceIdCallbackInfo* Data)
{
	if(Data->ResultCode == EOS_EResult::EOS_Success || Data->ResultCode == EOS_EResult::EOS_DuplicateNotAllowed)
	{
		OnSuccess.Broadcast();
		EndTask();
	}
	else
		Failure(EOS_EResult_ToString(Data->ResultCode));
}

void UEosDeviceIdLogin::Failure(const FString& ErrorTxt)
{
	UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorTxt);
	OnFailure.Broadcast();
	EndTask();
}

void UEosDeviceIdLogin::CreateDeviceId()
{
	EOS_Platform_ClientCredentials ClientCredentials;
	ClientCredentials.ClientId = "xyza7891gaDHorZ5zZvotKcvRAPrJNqT";
	ClientCredentials.ClientSecret = "9SXJLAM3q+WW7flJBs34mzRGwFjTtNG6Irc0KLGf3js";

	PlatformOptions.ClientCredentials = ClientCredentials;
	PlatformOptions.DeploymentId = "c6febd34407445aaa9152cf0142dd77d";
	PlatformOptions.SandboxId = "fc0b8443a9ed438dac28e0283d478fbe";
	PlatformOptions.EncryptionKey = "1111111111111111111111111111111111111111111111111111111111111111";
	PlatformOptions.bIsServer = EOS_FALSE;
	PlatformOptions.ProductId = "fc0b8443a9ed438dac28e0283d478fbe";

	PlatformHandle = EOS_Platform_Create(&PlatformOptions);
	if(!PlatformHandle)
		Failure("EOS_Platform_Create() failed");

	EOS_HConnect ConnectHandle = EOS_Platform_GetConnectInterface(PlatformHandle);
	if(!ConnectHandle)
		Failure("EOS_Platform_GetConnectInterface() failed");

	EOS_Connect_CreateDeviceIdOptions CreateDeviceIdOptions;
	CreateDeviceIdOptions.ApiVersion = EOS_CONNECT_CREATEDEVICEID_API_LATEST;
	CreateDeviceIdOptions.DeviceModel = "testing";

	EOS_Connect_OnCreateDeviceIdCallback CompleteDelegate = [](const EOS_Connect_CreateDeviceIdCallbackInfo* Data){
		if(UEosDeviceIdLogin* This = static_cast<UEosDeviceIdLogin*>(Data->ClientData))
			This->Success(Data);
	};

	EOS_Connect_CreateDeviceId(ConnectHandle, &CreateDeviceIdOptions, this, CompleteDelegate);
}
