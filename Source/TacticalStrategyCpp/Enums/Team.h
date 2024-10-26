#pragma once

/*
 * https://www.youtube.com/watch?v=XnsRdaZTMas
 */

UENUM(BlueprintType)
enum class ETeam : uint8
{
	ET_Red UMETA (DisplayName = "Red Team"),
	ET_Blue UMETA (DisplayName = "Blue Team"),
	ET_NoTeam UMETA (DisplayName = "Not in a Team"),	
	
	ET_MAX UMETA (DisplayName = "DefaultMAX")
};