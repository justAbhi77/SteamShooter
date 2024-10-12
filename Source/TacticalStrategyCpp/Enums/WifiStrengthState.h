
#pragma once

UENUM(BlueprintType)
enum class EWifiStrengthState : uint8
{
	EWS_NoStrength UMETA(DisplayName = "No Strength"),
	EWS_OneThirdsStrength UMETA(DisplayName = "One Thirds Strength"),
	EWS_TwoThirdsStrength UMETA(DisplayName = "Two Thirds Strength"),
	EWS_FullStrength UMETA(DisplayName = "Full Strength"),
	EWS_Max UMETA(DisplayName = "Max For Animation")
};