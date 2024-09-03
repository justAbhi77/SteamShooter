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
	
protected:	
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	class ABlasterHud* BlasterHud;
};
