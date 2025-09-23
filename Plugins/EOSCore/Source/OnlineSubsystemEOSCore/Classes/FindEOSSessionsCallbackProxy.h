/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "CreateEOSSessionCallbackProxy.h"
#include "FindSessionsCallbackProxy.h"
#include "UObject/ObjectMacros.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "eos_common.h"
#include "FindEOSSessionsCallbackProxy.generated.h"

USTRUCT(BlueprintType)
struct FEOSSessionResult
{
	GENERATED_BODY()
public:
	FEOSSessionResult() = default;
	FEOSSessionResult(const FBlueprintSessionResult& result, const TArray<FEOSSessionSetting>& sessionSettings)
		: Result(result)
		, SessionSettings(sessionSettings)
	{}
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data")
		FBlueprintSessionResult Result;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data")
		TArray<FEOSSessionSetting> SessionSettings;
};

USTRUCT(BlueprintType)
struct FEOSSessionSearchSetting
{
	GENERATED_BODY()
public:
	EOS_EOnlineComparisonOp ComparisonOp;
	TVariant<int32, bool, FString> Data;
public:
	FEOSSessionSearchSetting() = default;
	FEOSSessionSearchSetting(bool value)
        : ComparisonOp(EOS_EOnlineComparisonOp::EOS_CO_EQUAL)
	{
		Data.Set<bool>(value);
	}
	FEOSSessionSearchSetting(int32 value)
        : ComparisonOp(EOS_EOnlineComparisonOp::EOS_CO_EQUAL)
	{
		Data.Set<int32>(value);
	}
	FEOSSessionSearchSetting(FString value)
        : ComparisonOp(EOS_EOnlineComparisonOp::EOS_CO_EQUAL)
	{
		Data.Set<FString>(value);
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFindSteamSessionsResultDelegate, const TArray<FEOSSessionResult>&, Results);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UFindEOSSessionsCallbackProxy
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class ONLINESUBSYSTEMEOSCORE_API UFindEOSSessionsCallbackProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FFindSteamSessionsResultDelegate OnCallback;

public:
	UFindEOSSessionsCallbackProxy();

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", AutoCreateRefTerm = "SearchSettings", DisplayName = "Find EOS Sessions"), Category = "EOSCore|Utilities")
		static UFindEOSSessionsCallbackProxy* FindEOSSessions(UObject* WorldContextObject, TMap<FString, FEOSSessionSearchSetting> SearchSettings, int32 MaxResults = 10, bool bIsLanQuery = false);

protected:
	TSharedPtr<class FOnlineSessionSearch> m_SearchSettings;
	TMap<FString, FEOSSessionSearchSetting> m_AdditionalSearchSettings;
	TArray<FBlueprintSessionResult> m_SearchResults;
	int m_MaxResults;
	bool m_bIsLanQuery;
	UObject* m_WorldContextObject;
private:
	FOnFindSessionsCompleteDelegate Delegate;
	FDelegateHandle DelegateHandle;
public:
	virtual void Activate() override;
public:
	UFUNCTION()
		void OnCompleted(bool bSuccessful);
};
