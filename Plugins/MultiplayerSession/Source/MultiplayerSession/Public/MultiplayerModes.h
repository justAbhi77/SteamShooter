#pragma once

UENUM(BlueprintType)
enum class EMultiplayerModes : uint8
{
	Emm_Teams = 0 UMETA (DisplayName = "Teams"),
	Emm_CaptureFlag = 1 UMETA (DisplayName = "Capture the Flag"),
	Emm_FreeForAll = 2 UMETA (DisplayName = "Free For All"),	
	
	Emm_Max = 3 UMETA (DisplayName = "DefaultMAX")
};
