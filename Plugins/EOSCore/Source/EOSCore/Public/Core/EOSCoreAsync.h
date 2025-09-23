/**
* Copyright (C) 2017-2022 | eelDev AB
*
* Official SteamCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "EOSCoreAsync.generated.h"

UCLASS()
class EOSCORE_API UEOSCoreAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UObject* m_WorldContextObject;
public:
	virtual void Activate() override;
	virtual void SetReadyToDestroy() override; 
};