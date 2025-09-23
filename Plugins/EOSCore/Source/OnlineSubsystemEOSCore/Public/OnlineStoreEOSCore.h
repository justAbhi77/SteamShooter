/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include <Misc/EngineVersionComparison.h>
#if UE_VERSION_NEWER_THAN(4,27,2)
#include <Online/CoreOnline.h>
#else
#include <UObject/CoreOnline.h>
#endif
#include "Interfaces/OnlineStoreInterfaceV2.h"
#include "Interfaces/OnlinePurchaseInterface.h"
#include "OnlineSubsystemPackageEOSCore.h"
#include "OnlineSubsystemTypesEOSCore.h"
#include "eos_ecom_types.h"

class UWorld;

class FOnlineStoreEOSCore : public IOnlineStoreV2, public IOnlinePurchase, public TSharedFromThis<FOnlineStoreEOSCore, ESPMode::ThreadSafe>
{
public:
	virtual ~FOnlineStoreEOSCore() = default;
private:
	FOnlineStoreEOSCore() = delete;

public:
	// Begin IOnlineStoreV2
	virtual void QueryCategories(const FUniqueNetId& UserId, const FOnQueryOnlineStoreCategoriesComplete& Delegate) override;
	virtual void GetCategories(TArray<FOnlineStoreCategory>& OutCategories) const override;
	virtual void QueryOffersByFilter(const FUniqueNetId& UserId, const FOnlineStoreFilter& Filter, const FOnQueryOnlineStoreOffersComplete& Delegate) override;
	virtual void QueryOffersById(const FUniqueNetId& UserId, const TArray<FUniqueOfferId>& OfferIds, const FOnQueryOnlineStoreOffersComplete& Delegate) override;
	virtual void GetOffers(TArray<FOnlineStoreOfferRef>& OutOffers) const override;
	virtual TSharedPtr<FOnlineStoreOffer> GetOffer(const FUniqueOfferId& OfferId) const override;
	// End IOnlineStoreV2

public:
	// Begin IOnlinePurchase
	virtual bool IsAllowedToPurchase(const FUniqueNetId& UserId) override { return true; }
	virtual void Checkout(const FUniqueNetId& UserId, const FPurchaseCheckoutRequest& CheckoutRequest, const FOnPurchaseCheckoutComplete& Delegate) override;
#if UE_VERSION_NEWER_THAN(5,0,3)
	virtual void Checkout(const FUniqueNetId& UserId, const FPurchaseCheckoutRequest& CheckoutRequest, const FOnPurchaseReceiptlessCheckoutComplete& Delegate) override;
#endif
	virtual void FinalizePurchase(const FUniqueNetId& UserId, const FString& ReceiptId) override;
	virtual void RedeemCode(const FUniqueNetId& UserId, const FRedeemCodeRequest& RedeemCodeRequest, const FOnPurchaseRedeemCodeComplete& Delegate) override;
	virtual void QueryReceipts(const FUniqueNetId& UserId, bool bRestoreReceipts, const FOnQueryReceiptsComplete& Delegate) override;
	virtual void GetReceipts(const FUniqueNetId& UserId, TArray<FPurchaseReceipt>& OutReceipts) const override;
	virtual void FinalizeReceiptValidationInfo(const FUniqueNetId& UserId, FString& InReceiptValidationInfo, const FOnFinalizeReceiptValidationInfoComplete& Delegate) override;
	// End IOnlinePurchase

PACKAGE_SCOPE:
	FOnlineStoreEOSCore(FOnlineSubsystemEOSCore* InSubsystem);
	bool HandleEcomExec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar);

private:
	void QueryOffers(const FUniqueNetId& UserId, const FOnQueryOnlineStoreOffersComplete& Delegate);

private:
	FOnlineSubsystemEOSCore* m_Subsystem;
	TArray<FOnlineStoreOfferRef> m_CachedOffers;
	TArray<FUniqueOfferId> m_CachedOfferIds;
	TArray<FPurchaseReceipt> m_CachedReceipts;
};

typedef TSharedPtr<FOnlineStoreEOSCore, ESPMode::ThreadSafe> FOnlineStoreEOSCorePtr;
