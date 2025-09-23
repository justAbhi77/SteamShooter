/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "EOSCoreModule.h"
#include "Containers/Ticker.h"
#include "Core/EOSTypes.h"
#include "P2P/EOSP2PTypes.h"
#include <Misc/EngineVersionComparison.h>
#include "EOSChat.generated.h"

/**
 * The Chat Interface is a exlusive interface to EOSCore::
 * It's purpose is to simplify chat messages
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnChatMessageReceived, const FEOSProductUserId&, sender, const FEOSProductUserId&, receiver, const FString&, message);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnChatMessageReceivedCallback, const FEOSProductUserId&, sender, const FEOSProductUserId&, receiver, const FString&, message);

UCLASS()
class EOSCORE_API UCoreChat : public UEOSCoreSubsystem
{
	GENERATED_BODY()
public:
	UCoreChat();
public:
	UPROPERTY(BlueprintAssignable)
	FOnChatMessageReceived OnChatMessageReceived;
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool Tick(float deltaTime);
public:
	/**
	 * The Chat Interface is a exlusive interface to EOSCore::
	 * It's purpose is to simplify chat messages
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Interfaces", meta = (WorldContext = "WorldContextObject"))
	static UCoreChat* GetChat() { return m_ChatObject; }

	UFUNCTION(BlueprintCallable, Category = "EOSCore|Chat", meta = (WorldContext = "WorldContextObject"))
	static void SendChatMessage(UObject* WorldContextObject, FEOSProductUserId localUserId, FEOSProductUserId target, FString message, EEOSEPacketReliability reliability = EEOSEPacketReliability::EOS_PR_UnreliableUnordered);
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Chat", meta = (WorldContext = "WorldContextObject"))
	static void ListenForChatMessages(UObject* WorldContextObject, FEOSProductUserId localUserId, const FOnChatMessageReceivedCallback& callback);
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Chat")
	static void ClearAllChatListeners();
private:
#if UE_VERSION_NEWER_THAN(4,27,2)
	FTSTicker::FDelegateHandle m_Ticker;
#else
	FDelegateHandle m_Ticker;
#endif
	static UCoreChat* m_ChatObject;
};
