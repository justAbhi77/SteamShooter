#pragma once

UENUM(BlueprintType)
enum class EMultiplayerModes : uint8
{
	EMM_Teams UMETA (DisplayName = "Teams"),
	EMM_CaptureFlag UMETA (DisplayName = "Capture the Flag"),
	EMM_FreeForAll UMETA (DisplayName = "Free For All"),	
	
	EMM_Max UMETA (DisplayName = "DefaultMAX", Hidden)
};
