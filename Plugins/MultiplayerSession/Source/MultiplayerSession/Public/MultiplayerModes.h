#pragma once

UENUM(BlueprintType)
enum class EMultiplayerModes : uint8
{
	EMM_Teams = 0 UMETA (DisplayName = "Teams"),
	EMM_CaptureFlag = 1 UMETA (DisplayName = "Capture the Flag"),
	EMM_FreeForAll = 2 UMETA (DisplayName = "Free For All"),	
	
	EMM_MAX = 3 UMETA (DisplayName = "DefaultMAX")
};
