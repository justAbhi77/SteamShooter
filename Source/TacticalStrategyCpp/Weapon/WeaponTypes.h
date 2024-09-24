#pragma once

#define TRACE_LENGTH 9999.f

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_AssaultRifle UMETA(DisplayName = "Assault Rifle"),
	EWT_RocketLauncher UMETA(DisplayName = "Rocket Launcher"),
	EWT_SMG UMETA(DisplayName = "Sub Machine Gun"),	
	EWT_Shotgun UMETA(DisplayName = "Shotgun"),
	EWT_Pistol UMETA(DisplayName = "Pistol"),
	EWT_SniperRifle UMETA(DisplayName = "Sniper Rifle"),
	EWT_GrenadeLauncher UMETA(DisplayName = "Grenade Launcher"),
	
	EWT_MAX UMETA(DisplayName = "Default Max")
};
