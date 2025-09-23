/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "Ecom/EOSEcomTypes.h"
#include "Core/EOSCoreAsync.h"
#include "EOSEcomAsyncActions.generated.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreEcomQueryOwnership
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreEcomQueryOwnership : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnEcomQueryOwnershipCallbackDelegate OnCallback;
private:
	FOnEcomQueryOwnershipCallback m_Callback;
public:
	UEOSCoreEcomQueryOwnership() { m_Callback.BindUFunction(this, "HandleCallback"); }
public:
	/**
	* Query the ownership status for a given list of catalog item IDs defined with Epic Online Services.
	* This data will be cached for a limited time and retrieved again from the backend when necessary
	*
	* @param Options structure containing the account and catalog item IDs to retrieve
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Ecom|Async", DisplayName = "EOS_Ecom_QueryOwnership"))
	static UEOSCoreEcomQueryOwnership* EOSEcomQueryOwnershipAsync(UObject* WorldContextObject, FEOSEcomQueryOwnershipOptions Options);
protected:
	FEOSEcomQueryOwnershipOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSEcomQueryOwnershipCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreEcomQueryOwnershipToken
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreEcomQueryOwnershipToken : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnEcomQueryOwnershipTokenCallbackDelegate OnCallback;
private:
	FOnEcomQueryOwnershipTokenCallback m_Callback;
public:
	UEOSCoreEcomQueryOwnershipToken() { m_Callback.BindUFunction(this, "HandleCallback"); }
public:
	/**
	* Query the ownership status for a given list of catalog item IDs defined with Epic Online Services.
	* The data is return via the callback in the form of a signed JWT that should be verified by an external backend server using a public key for authenticity.
	*
	* @param Options structure containing the account and catalog item IDs to retrieve in token form
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Ecom|Async", DisplayName = "EOS_Ecom_QueryOwnershipToken"))
	static UEOSCoreEcomQueryOwnershipToken* EOSEcomQueryOwnershipTokenAsync(UObject* WorldContextObject, FEOSEcomQueryOwnershipTokenOptions Options);
protected:
	FEOSEcomQueryOwnershipTokenOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSEcomQueryOwnershipTokenCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreEcomQueryEntitlements
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreEcomQueryEntitlements : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnEcomQueryEntitlementsCallbackDelegate OnCallback;
private:
	FOnEcomQueryEntitlementsCallback m_Callback;
public:
	UEOSCoreEcomQueryEntitlements() { m_Callback.BindUFunction(this, "HandleCallback"); }
public:
	/**
	* Query the entitlement information defined with Epic Online Services.
	* A set of entitlement names can be provided to filter the set of entitlements associated with the account.
	* This data will be cached for a limited time and retrieved again from the backend when necessary.
	* Use EOS_Ecom_CopyEntitlementByIndex, EOS_Ecom_CopyEntitlementByNameAndIndex, and EOS_Ecom_CopyEntitlementById to get the entitlement details.
	* Use EOS_Ecom_GetEntitlementsByNameCount to retrieve the number of entitlements with a specific entitlement name.
	*
	* @param Options structure containing the account and entitlement names to retrieve
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Ecom|Async", DisplayName = "EOS_Ecom_QueryEntitlements"))
	static UEOSCoreEcomQueryEntitlements* EOSEcomQueryEntitlementsAsync(UObject* WorldContextObject, FEOSEcomQueryEntitlementsOptions Options);
protected:
	FEOSEcomQueryEntitlementsOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSEcomQueryEntitlementsCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreEcomQueryOffers
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreEcomQueryOffers : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnEcomQueryQueryOffersCallbackDelegate OnCallback;
private:
	FOnEcomQueryQueryOffersCallback m_Callback;
public:
	UEOSCoreEcomQueryOffers() { m_Callback.BindUFunction(this, "HandleCallback"); }
public:
	/**
	* Query for a list of catalog offers defined with Epic Online Services.
	* This data will be cached for a limited time and retrieved again from the backend when necessary.
	*
	* @param Options structure containing filter criteria
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Ecom|Async", DisplayName = "EOS_Ecom_QueryOffers"))
	static UEOSCoreEcomQueryOffers* EOSEcomQueryOffersAsync(UObject* WorldContextObject, FEOSEcomQueryOffersOptions Options);
protected:
	FEOSEcomQueryOffersOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSEcomQueryOffersCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreEcomCheckout
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreEcomCheckout : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnEcomQueryCheckoutCallbackDelegate OnCallback;
private:
	FOnEcomQueryCheckoutCallback m_Callback;
public:
	UEOSCoreEcomCheckout() { m_Callback.BindUFunction(this, "HandleCallback"); }
public:
	/**
	* Initiates the purchase flow for a set of offers.  The callback is triggered after the purchase flow.
	* On success, the set of entitlements that were unlocked will be cached.
	* On success, a Transaction ID will be returned. The Transaction ID can be used to obtain an
	* EOS_Ecom_HTransaction handle. The handle can then be used to retrieve the entitlements rewarded by the purchase.
	*
	* @see EOS_Ecom_Transaction_Release
	*
	* @param Options structure containing filter criteria
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Ecom|Async", DisplayName = "EOS_Ecom_Checkout"))
	static UEOSCoreEcomCheckout* EOSEcomCheckoutAsync(UObject* WorldContextObject, FEOSEcomCheckoutOptions Options);
protected:
	FEOSEcomCheckoutOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSEcomCheckoutCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreEcomRedeemEntitlements
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreEcomRedeemEntitlements : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnEcomQueryRedeemEntitlementsCallbackDelegate OnCallback;
private:
	FOnEcomQueryRedeemEntitlementsCallback m_Callback;
public:
	UEOSCoreEcomRedeemEntitlements() { m_Callback.BindUFunction(this, "HandleCallback"); }
public:
	/**
	* Requests that the provided entitlement be marked redeemed.  This will cause that entitlement
	* to no longer be returned from QueryEntitlements unless the include redeemed request flag is set true.
	*
	* @param Options structure containing entitlement to redeem
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|Ecom|Async", DisplayName = "EOS_Ecom_RedeemEntitlements"))
	static UEOSCoreEcomRedeemEntitlements* EOSEcomRedeemEntitlementsAsync(UObject* WorldContextObject, FEOSEcomRedeemEntitlementsOptions Options);
protected:
	FEOSEcomRedeemEntitlementsOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSEcomRedeemEntitlementsCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};
