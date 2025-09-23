/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"

#include "FindEOSSessionsCallbackProxy.h"
#include "UObject/ObjectMacros.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "UpdateEOSSessionCallbackProxy.generated.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UUpdateEOSSessionCallbackProxy
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class ONLINESUBSYSTEMEOSCORE_API UUpdateEOSSessionCallbackProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnCallback;

public:
	UUpdateEOSSessionCallbackProxy();

	UFUNCTION(BlueprintCallable,
		meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "EOSCore|Utilities")
	static UUpdateEOSSessionCallbackProxy* Update_EOS_Session(UObject* WorldContextObject, TMap<FString, FEOSSessionSearchSetting> Settings, FString SessionName = "EOSCore Session", int32 PublicConnections = 4);

protected:
	TMap<FString, FEOSSessionSearchSetting> m_Settings;
	UObject* m_WorldContextObject;
	FString m_SessionName;
	int32 m_PublicConnections;
private:
	FOnUpdateSessionCompleteDelegate Delegate;
	FDelegateHandle DelegateHandle;
public:
	virtual void Activate() override;
public:
	UFUNCTION()
	void OnCompleted(FName SessionName, bool bWasSuccessful);
};
