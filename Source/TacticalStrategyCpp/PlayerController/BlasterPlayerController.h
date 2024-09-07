#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlasterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALSTRATEGYCPP_API ABlasterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void SetHudHealth(float Health, float MaxHealth);
	void SetHudScore(float Score);

	void SetHudDefeats(int32 Defeats);
	
	void SetHudWeaponAmmo(int32 Ammo);
	void SetHudCarriedAmmo(int32 Ammo);

	virtual void OnPossess(APawn* InPawn) override;
	
protected:	
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	class ABlasterHud* BlasterHud;
};
