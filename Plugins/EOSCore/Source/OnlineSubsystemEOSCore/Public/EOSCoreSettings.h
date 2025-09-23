/**
* Copyright (C) 2017-2022 | eelDev AB
*
* Official SteamCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include <Engine/DataAsset.h>
#include "EOSCoreSettings.generated.h"

#define INI_SECTION TEXT("/Script/OnlineSubsystemEOSCore.EOSCoreSettings")

inline FString StripQuotes(FString& Source)
{
	if (Source.StartsWith(TEXT("\"")))
	{
		return Source.Mid(1, Source.Len() - 2);
	}
	return Source;
}

inline bool ContainsWhitespace(const FString& Source)
{
	for (const TCHAR& IterChar : Source)
	{
		if (FChar::IsWhitespace(IterChar))
		{
			return true;
		}
	}
	return false;
}

inline bool IsAnsi(const FString& Source)
{
	for (const TCHAR& IterChar : Source)
	{
		if (!FChar::IsPrint(IterChar))
		{
			return false;
		}
	}
	return true;
}

inline bool IsHex(const FString& Source)
{
	for (const TCHAR& IterChar : Source)
	{
		if (!FChar::IsHexDigit(IterChar))
		{
			return false;
		}
	}
	return true;
}

USTRUCT(BlueprintType)
struct ONLINESUBSYSTEMEOSCORE_API FEOSConfig
{
	GENERATED_BODY()
public:
	FEOSConfig()
		: ConfigName(FString("DefaultConfig"))
		, EncryptionKey(FString("1111111111111111111111111111111111111111111111111111111111111111"))
		, bEnableRTC(true)
	{
	}

public:
	// Name of the Configuration
	UPROPERTY(config, EditAnywhere, Category = "EOSCore")
	FString ConfigName;
	UPROPERTY(config, EditAnywhere, Category = "EOSCore")
	FString ProductId;
	// Your SandboxId found in the EOS Developer Portal
	UPROPERTY(config, EditAnywhere, Category = "EOSCore")
	FString SandboxId;
	// Your DeploymentId found in the EOS Developer Portal
	UPROPERTY(config, EditAnywhere, Category = "EOSCore")
	FString DeploymentId;
	// Your ClientId found in the EOS Developer Portal
	UPROPERTY(config, EditAnywhere, Category = "EOSCore")
	FString ClientId;
	// Your ClientSecret found in the EOS Developer Portal
	UPROPERTY(config, EditAnywhere, Category = "EOSCore")
	FString ClientSecret;
	// Your EncryptionKey, must be of length 64
	UPROPERTY(config, EditAnywhere, Category = "EOSCore")
	FString EncryptionKey;
	/** RTC options. Setting to FALSE will disable RTC features (e.g. voice) */
	UPROPERTY(config, EditAnywhere, Category = "EOSCore")
	bool bEnableRTC;
public:
	void ParseRawArrayEntry(FString& RawLine);
};

UCLASS(config = Engine, defaultconfig, meta = (DisplayName = "EOSCore Plugin"))
class ONLINESUBSYSTEMEOSCORE_API UEOSCoreSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UEOSCoreSettings();
public:
	// Default EOS Configuration Name
	UPROPERTY(config, EditAnywhere, Category = "EOSCore")
	FString DefaultConfigurationName;
	// Used to throttle EOS services tick rate
	UPROPERTY(config, EditAnywhere, Category = "EOSCore")
	int32 TickBudgetInMilliseconds;
	/** Auth scope flags are permissions to request from the user while they are logging in. This is used by the Online Subsystem when utilizing the Login node */
	UPROPERTY(Config, EditAnywhere, Category = "EOSCore", meta = (Bitmask, BitmaskEnum = "/Script/EOSCoreShared.EOSEAuthScopeFlags"))
	int32 ScopeFlags;
	// Client Id used on IOS devices
	UPROPERTY(config, EditAnywhere, Category = "EOSCore", meta = (DisplayName = "IOS Client Id"))
	FString IOSClientId;
	// Client Id used on Android devices
	UPROPERTY(config, EditAnywhere, Category = "EOSCore", meta = (DisplayName = "Android Client Id"))
	FString AndroidClientId;
	UPROPERTY(config, EditAnywhere, Category = "EOSCore", meta = (DisplayName = "Disable EOS Overlay"))
	bool bDisableEOSOverlay;
	UPROPERTY(config, EditAnywhere, Category = "EOSCore")
	TArray<FEOSConfig> EOSConfigurations;
public:
	static bool GetEOSConfig(const FString& ConfigName, FEOSConfig& OutSettings);
	static int32 GetAuthScopeFlags();
private:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual FName GetCategoryName() const override;
#endif
};
