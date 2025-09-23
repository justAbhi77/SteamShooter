/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "Metrics/EOSMetrics.h"
#include "Core/EOSCorePluginPrivatePCH.h"

UCoreMetrics* UCoreMetrics::GetMetrics(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (GetMetricsHandle(WorldContextObject))
		{
			if (const UWorld* World = WorldContextObject->GetWorld())
			{
				if (const UGameInstance* GameInstance = World->GetGameInstance())
				{
					return GameInstance->GetSubsystem<UCoreMetrics>();
				}
			}	
		}
	}
	return nullptr;
}

EOSResult UCoreMetrics::EOSMetricsBeginPlayerSession(UObject* WorldContextObject, FEOSMetricsBeginPlayerSessionOptions Options)
{
	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (const EOS_HMetrics Metrics = GetMetricsHandle(WorldContextObject))
	{
		EOS_Metrics_BeginPlayerSessionOptions MetricsOptions = { };
		MetricsOptions.ApiVersion = Options.ApiVersion;
		MetricsOptions.AccountIdType = static_cast<EOS_EMetricsAccountIdType>(Options.AccountIdType);
		MetricsOptions.AccountId.External = nullptr;
		MetricsOptions.GameSessionId = nullptr;
		MetricsOptions.ServerIp = nullptr;

		const FTCHARToUTF8 ConvertedExternal(*Options.External);
		const FTCHARToUTF8 ConvertedDisplayName(*Options.DisplayName);
		const FTCHARToUTF8 ConvertedGameSessionId(*Options.GameSessionId);
		const FTCHARToUTF8 ConvertedServerIp(*Options.ServerIp);

		switch (Options.AccountIdType)
		{
		case EEOSEMetricsAccountIdType::EOS_MAIT_Epic:
			MetricsOptions.AccountId.Epic = Options.Epic;
			break;
		case EEOSEMetricsAccountIdType::EOS_MAIT_External:
			MetricsOptions.AccountId.External = Options.External.Len() > 0 ? ConvertedExternal.Get() : nullptr;
			break;
		}

		MetricsOptions.ControllerType = static_cast<EOS_EUserControllerType>(Options.ControllerType);
		MetricsOptions.DisplayName = ConvertedDisplayName.Get();
		MetricsOptions.GameSessionId = Options.GameSessionId.Len() > 0 ? ConvertedGameSessionId.Get() : nullptr;
		MetricsOptions.ServerIp = Options.ServerIp.Len() > 0 ? ConvertedServerIp.Get() : nullptr;
		
		Result = EOSHelpers::ToEOSCoreResult(EOS_Metrics_BeginPlayerSession(Metrics, &MetricsOptions));
	}

	return Result;
}

EOSResult UCoreMetrics::EOSMetricsEndPlayerSession(UObject* WorldContextObject, FEOSMetricsEndPlayerSessionOptions Options)
{
	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (const EOS_HMetrics Metrics = GetMetricsHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedExternal(*Options.External);

		EOS_Metrics_EndPlayerSessionOptions MetricsOptions = { };
		MetricsOptions.ApiVersion = Options.ApiVersion;
		MetricsOptions.AccountIdType = static_cast<EOS_EMetricsAccountIdType>(Options.AccountIdType);
		MetricsOptions.AccountId.External = nullptr;

		switch (Options.AccountIdType)
		{
		case EEOSEMetricsAccountIdType::EOS_MAIT_Epic:
			MetricsOptions.AccountId.Epic = Options.Epic;
			break;
		case EEOSEMetricsAccountIdType::EOS_MAIT_External:
			MetricsOptions.AccountId.External = Options.External.Len() > 0 ? ConvertedExternal.Get() : nullptr;
			break;
		}

		Result = EOSHelpers::ToEOSCoreResult(EOS_Metrics_EndPlayerSession(Metrics, &MetricsOptions));
	}

	return Result;
}
