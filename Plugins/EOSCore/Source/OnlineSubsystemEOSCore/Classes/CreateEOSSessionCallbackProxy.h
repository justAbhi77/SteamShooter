/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Misc/TVariant.h"
#include "CreateEOSSessionCallbackProxy.generated.h"

class APlayerController;

USTRUCT(BlueprintType)
struct ONLINESUBSYSTEMEOSCORE_API FEOSSessionSetting
{
	GENERATED_BODY()
public:
	FString Key;
	TVariant<int32, bool, FString> Data;

public:
	FEOSSessionSetting() {};
	FEOSSessionSetting(bool value)
	{ 
		Data.Set<bool>(value);
	}
	FEOSSessionSetting(int32 value)
	{
		Data.Set<int32>(value);
	}
	FEOSSessionSetting(FString value)
	{
		Data.Set<FString>(value);
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UCreateEOSSessionCallbackProxy
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class ONLINESUBSYSTEMEOSCORE_API UCreateEOSSessionCallbackProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FEmptyOnlineDelegate OnSuccess;
	UPROPERTY(BlueprintAssignable)
		FEmptyOnlineDelegate OnFailure;
public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "worldContextObject", AutoCreateRefTerm = "sessionSettings"), Category = "Online|Session")
		static UCreateEOSSessionCallbackProxy* Create_EOS_Session(UObject* WorldContextObject, TMap<FString, FEOSSessionSetting> SessionSettings, FString SessionName = "EOSCore Session", int32 PublicConnections = 4, bool bIsLanMatch = false);
public:
	// UOnlineBlueprintCallProxyBase interface
	virtual void Activate() override;
	// End of UOnlineBlueprintCallProxyBase interface
private:
	void OnCreateCompleted(FName SessionName, bool bWasSuccessful);
	void OnStartCompleted(FName SessionName, bool bWasSuccessful);
private:
	FOnCreateSessionCompleteDelegate m_CreateCompleteDelegate;
	FOnStartSessionCompleteDelegate m_StartCompleteDelegate;
	FDelegateHandle m_CreateCompleteDelegateHandle;
	FDelegateHandle m_StartCompleteDelegateHandle;
private:
	int m_NumPublicConnections;
	bool m_bIsLanMatch;
	UObject* m_WorldContextObject;
	FString m_SessionName;
	TMap<FString, FEOSSessionSetting> m_EOSSessionSettings;
};
