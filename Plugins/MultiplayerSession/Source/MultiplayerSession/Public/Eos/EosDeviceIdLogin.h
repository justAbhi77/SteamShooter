

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "ThirdParty/EOSSDK/SDK/Bin/Android/include/eos_connect_types.h"
#include "ThirdParty/EOSSDK/SDK/Bin/Android/include/eos_types.h"
#include "EosDeviceIdLogin.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLoginStatus);

/**
 *
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = "AsyncTask"))
class MULTIPLAYERSESSION_API UEosDeviceIdLogin : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FLoginStatus OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FLoginStatus OnFailure;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UEosDeviceIdLogin* EosDeviceIdLogin();

	virtual void Activate() override;

	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:
	EOS_Platform_Options PlatformOptions =
	{
		EOS_PLATFORM_OPTIONS_API_LATEST,
		nullptr,
	};

	EOS_HPlatform PlatformHandle;

	void Success(const EOS_Connect_CreateDeviceIdCallbackInfo* Data);

	void Failure(const FString& ErrorTxt);

	void CreateDeviceId();
};
