/**
* Copyright (C) 2017-2022 | eelDev AB
*
 official EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "EOSCoreModule.h"
#include "eos_ecom_types.h"
#include "Ecom/EOSEcomTypes.h"
#include "EOSEcom.generated.h"

/**
 * The Ecom Interface exposes all catalog, purchasing, and ownership entitlement features available with the Epic Games store
 * All Ecom Interface calls take a handle of type EOS_HEcom as the first parameter.
 * This handle can be retrieved from a EOS_HPlatform handle by using the EOS_Platform_GetEcomInterface function.
 *
 * NOTE: At this time, this feature is only available for products that are part of the Epic Games store.
 *
 * @see EOS_Platform_GetEcomInterface
 */

UCLASS()
class EOSCORE_API UCoreEcom : public UEOSCoreSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Interfaces", meta = (WorldContext = "WorldContextObject"))
	static UCoreEcom* GetEcom(UObject* WorldContextObject);

public:
	/**
	* Query the ownership status for a given list of catalog item IDs defined with Epic Online Services.
	* This data will be cached for a limited time and retrieved again from the backend when necessary
	*
	* @param Options structure containing the account and catalog item IDs to retrieve
	* @param Callback a callback that is fired when the async operation completes, either successfully or in error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_QueryOwnership"))
	void EOSEcomQueryOwnership(UObject* WorldContextObject, FEOSEcomQueryOwnershipOptions Options, const FOnEcomQueryOwnershipCallback& Callback);

	/**
	 * Query the ownership status of all catalog item IDs under the given list of Sandbox IDs defined with Epic Online Services.
	 * This data will be cached for a limited time and retrieved again from the backend when necessary.
	 *
	 * @param Options structure containing the account and Sandbox IDs to retrieve.
	 * @param Callback a callback that is fired when the async operation completes, either successfully or in error.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_QueryOwnershipBySandboxIds"))
	void EOSEcomQueryOwnershipBySandboxIds(UObject* WorldContextObject, FEOSEcomQueryOwnershipBySandboxIdsOptions Options, const FOnEcomQueryOwnershipBySandboxIds Callback);
	
	/** 
	* Query the ownership status for a given list of catalog item IDs defined with Epic Online Services.
	* The data is return via the callback in the form of a signed JWT that should be verified by an external backend server using a public key for authenticity.
	*
	* @param Options structure containing the account and catalog item IDs to retrieve in token form
	* @param Callback a callback that is fired when the async operation completes, either successfully or in error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_QueryOwnershipToken"))
	void EOSEcomQueryOwnershipToken(UObject* WorldContextObject, FEOSEcomQueryOwnershipTokenOptions Options, const FOnEcomQueryOwnershipTokenCallback& Callback);

	/**
	* Query the entitlement information defined with Epic Online Services.
	* A set of entitlement names can be provided to filter the set of entitlements associated with the account.
	* This data will be cached for a limited time and retrieved again from the backend when necessary.
	* Use EOS_Ecom_CopyEntitlementByIndex, EOS_Ecom_CopyEntitlementByNameAndIndex, and EOS_Ecom_CopyEntitlementById to get the entitlement details.
	* Use EOS_Ecom_GetEntitlementsByNameCount to retrieve the number of entitlements with a specific entitlement name.
	*
	* @param Options structure containing the account and entitlement names to retrieve
	* @param Callback a callback that is fired when the async operation completes, either successfully or in error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_QueryEntitlements"))
	void EOSEcomQueryEntitlements(UObject* WorldContextObject, FEOSEcomQueryEntitlementsOptions Options, const FOnEcomQueryEntitlementsCallback& Callback);

	/**
	* Query the entitlement information defined with Epic Online Services.
	* A set of entitlement names can be provided to filter the set of entitlements associated with the account.
	* This data will be cached for a limited time and retrieved again from the backend when necessary.
	* Use EOS_Ecom_CopyEntitlementByIndex, EOS_Ecom_CopyEntitlementByNameAndIndex, and EOS_Ecom_CopyEntitlementById to get the entitlement details.
	* Use EOS_Ecom_GetEntitlementsByNameCount to retrieve the number of entitlements with a specific entitlement name.
	*
	* @param Options structure containing the account and entitlement names to retrieve
	* @param Callback a callback that is fired when the async operation completes, either successfully or in error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_QueryEntitlementToken"))
	void EOSEcomQueryEntitlementToken(UObject* WorldContextObject, FEOSEcomQueryEntitlementTokenOptions Options, const FOnEcomQueryEntitlementTokenCallback& Callback);

	/**
	* Query for a list of catalog offers defined with Epic Online Services.
	* This data will be cached for a limited time and retrieved again from the backend when necessary.
	*
	* @param Options structure containing filter criteria
	* @param Callback a callback that is fired when the async operation completes, either successfully or in error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_QueryOffers"))
	void EOSEcomQueryOffers(UObject* WorldContextObject, FEOSEcomQueryOffersOptions Options, const FOnEcomQueryQueryOffersCallback& Callback);

	/**
	* Initiates the purchase flow for a set of offers.  The callback is triggered after the purchase flow.
	* On success, the set of entitlements that were unlocked will be cached.
	* On success, a Transaction ID will be returned. The Transaction ID can be used to obtain an
	* EOS_Ecom_HTransaction handle. The handle can then be used to retrieve the entitlements rewarded by the purchase.
	*
	* @see EOS_Ecom_Transaction_Release
	*
	* @param Options structure containing filter criteria
	* @param Callback a callback that is fired when the async operation completes, either successfully or in error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_Checkout"))
	void EOSEcomCheckout(UObject* WorldContextObject, FEOSEcomCheckoutOptions Options, const FOnEcomQueryCheckoutCallback& Callback);

	/**
	* Requests that the provided entitlement be marked redeemed.  This will cause that entitlement
	* to no longer be returned from QueryEntitlements unless the include redeemed request flag is set true.
	*
	* @param Options structure containing entitlement to redeem
	* @param Callback a callback that is fired when the async operation completes, either successfully or in error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_RedeemEntitlements"))
	void EOSEcomRedeemEntitlements(UObject* WorldContextObject, FEOSEcomRedeemEntitlementsOptions Options, const FOnEcomQueryRedeemEntitlementsCallback& Callback);

	/**
	 * Fetch the number of entitlements that were redeemed during the last EOS_Ecom_RedeemEntitlements call.
	 *
	 * @param Options structure containing the Epic Account ID
	 *
	 * @see EOS_Ecom_CopyLastRedeemedEntitlementByIndex
	 *
	 * @return the number of the redeemed entitlements.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_GetLastRedeemedEntitlementsCount"))
	int32 EOSEcomGetLastRedeemedEntitlementsCount(UObject* WorldContextObject, FEOSEcomGetLastRedeemedEntitlementsCountOptions Options);

	/**
	 * Fetches a redeemed entitlement id from a given index.
	 * Only entitlements that were redeemed during the last EOS_Ecom_RedeemEntitlements call can be copied.
	 *
	 * @param Options structure containing the Epic Account ID and index being accessed
	 * @param OutRedeemedEntitlementId The ID of the redeemed entitlement. Must be long enough to hold a string of EOS_ECOM_ENTITLEMENTID_MAX_LENGTH.
	 *
	 * @return EOS_Success if the information is available and passed out in OutRedeemedEntitlementId
	 *         EOS_InvalidParameters if you pass a null pointer for the out parameter
	 *         EOS_NotFound if the entitlement id is not found
	 *
	 * @see EOS_ECOM_ENTITLEMENTID_MAX_LENGTH
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_CopyLastRedeemedEntitlementByIndex"))
	EOSResult EOSEcomCopyLastRedeemedEntitlementByIndex(UObject* WorldContextObject, FEOSEcomCopyLastRedeemedEntitlementByIndexOptions Options, FString& OutRedeemedEntitlementId);

	/**
	* Fetch the number of entitlements that are cached for a given local user.
	*
	* @param Options structure containing the Epic Online Services Account ID being accessed
	*
	* @see EOS_Ecom_CopyEntitlementByIndex
	*
	* @return the number of entitlements found.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_GetEntitlementsCount"))
	static int32 EOSEcomGetEntitlementsCount(UObject* WorldContextObject, FEOSEcomGetEntitlementsCountOptions Options);

	/**
	* Fetch the number of entitlements with the given Entitlement Name that are cached for a given local user.
	*
	* @param Options structure containing the Epic Online Services Account ID and name being accessed
	*
	* @see EOS_Ecom_CopyEntitlementByNameAndIndex
	*
	* @return the number of entitlements found.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_GetEntitlementsByNameCount"))
	static int32 EOSEcomGetEntitlementsByNameCount(UObject* WorldContextObject, FEOSEcomGetEntitlementsByNameCountOptions Options);

	/**
	* Fetches an entitlement from a given index.
	*
	* @param Options structure containing the Epic Online Services Account ID and index being accessed
	* @param OutEntitlement the entitlement for the given index, if it exists and is valid, use EOS_Ecom_Entitlement_Release when finished
	*
	* @see EOS_Ecom_Entitlement_Release
	*
	* @return EOS_Success if the information is available and passed out in OutEntitlement
	*         EOS_Ecom_EntitlementStale if the entitlement information is stale and passed out in OutEntitlement
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_NotFound if the entitlement is not found
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_CopyEntitlementByIndex"))
	static EOSResult EOSEcomCopyEntitlementByIndex(UObject* WorldContextObject, FEOSEcomCopyEntitlementByIndexOptions Options, FEOSEcomEntitlement& OutEntitlement);

	/**
	* Fetches a single entitlement with a given Entitlement Name.  The Index is used to access individual
	* entitlements among those with the same Entitlement Name.  The Index can be a value from 0 to
	* one less than the result from EOS_Ecom_GetEntitlementsByNameCount.
	*
	* @param Options structure containing the Epic Online Services Account ID, entitlement name, and index being accessed
	* @param OutEntitlement the entitlement for the given name index pair, if it exists and is valid, use EOS_Ecom_Entitlement_Release when finished
	*
	* @see EOS_Ecom_Entitlement_Release
	*
	* @return EOS_Success if the information is available and passed out in OutEntitlement
	*         EOS_Ecom_EntitlementStale if the entitlement information is stale and passed out in OutEntitlement
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_NotFound if the entitlement is not found
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_CopyEntitlementByNameAndIndex"))
	static EOSResult EOSEcomCopyEntitlementByNameAndIndex(UObject* WorldContextObject, FEOSEcomCopyEntitlementByNameAndIndexOptions Options, FEOSEcomEntitlement& OutEntitlement);

	/**
	* Fetches the entitlement with the given ID.
	*
	* @param Options structure containing the Epic Online Services Account ID and entitlement ID being accessed
	* @param OutEntitlement the entitlement for the given ID, if it exists and is valid, use EOS_Ecom_Entitlement_Release when finished
	*
	* @see EOS_Ecom_CopyEntitlementByNameAndIndex
	* @see EOS_Ecom_Entitlement_Release
	*
	* @return EOS_Success if the information is available and passed out in OutEntitlement
	*         EOS_Ecom_EntitlementStale if the entitlement information is stale and passed out in OutEntitlement
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_NotFound if the entitlement is not found
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_CopyEntitlementById"))
	static EOSResult EOSEcomCopyEntitlementById(UObject* WorldContextObject, FEOSEcomCopyEntitlementByIdOptions Options, FEOSEcomEntitlement& OutEntitlement);

	/**
	* Fetch the number of offers that are cached for a given local user.
	*
	* @param Options structure containing the Epic Online Services Account ID being accessed
	*
	* @see EOS_Ecom_CopyOfferByIndex
	*
	* @return the number of offers found.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_GetOfferCount"))
	static int32 EOSEcomGetOfferCount(UObject* WorldContextObject, FEOSEcomGetOfferCountOptions Options);

	/**
	* Fetches an offer from a given index.  The pricing and text are localized to the provided account.
	*
	* @param Options structure containing the Epic Online Services Account ID and index being accessed
	* @param OutOffer the offer for the given index, if it exists and is valid, use EOS_Ecom_CatalogOffer_Release when finished
	*
	* @see EOS_Ecom_CatalogOffer_Release
	* @see EOS_Ecom_GetOfferItemCount
	*
	* @return EOS_Success if the information is available and passed out in OutOffer
	*         EOS_Ecom_CatalogOfferStale if the offer information is stale and passed out in OutOffer
	*         EOS_Ecom_CatalogOfferPriceInvalid if the offer information has an invalid price and passed out in OutOffer
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_NotFound if the offer is not found
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_CopyOfferByIndex"))
	static EOSResult EOSEcomCopyOfferByIndex(UObject* WorldContextObject, FEOSEcomCopyOfferByIndexOptions Options, FEOSEcomCatalogOffer& OutOffer);

	/**
	* Fetches an offer with a given ID.  The pricing and text are localized to the provided account.
	*
	* @param Options structure containing the Epic Online Services Account ID and offer ID being accessed
	* @param OutOffer the offer for the given index, if it exists and is valid, use EOS_Ecom_CatalogOffer_Release when finished
	*
	* @see EOS_Ecom_CatalogOffer_Release
	* @see EOS_Ecom_GetOfferItemCount
	*
	* @return EOS_Success if the information is available and passed out in OutOffer
	*         EOS_Ecom_CatalogOfferStale if the offer information is stale and passed out in OutOffer
	*         EOS_Ecom_CatalogOfferPriceInvalid if the offer information has an invalid price and passed out in OutOffer
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_NotFound if the offer is not found
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_CopyOfferById"))
	static EOSResult EOSEcomCopyOfferById(UObject* WorldContextObject, FEOSEcomCopyOfferByIdOptions Options, FEOSEcomCatalogOffer& OutOffer);

	/**
	* Fetch the number of items that are associated with a given cached offer for a local user.
	*
	* @return the number of items found.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_GetOfferItemCount"))
	static int32 EOSEcomGetOfferItemCount(UObject* WorldContextObject, FEOSEcomGetOfferItemCountOptions Options);

	/**
	* Fetches an item from a given index.
	*
	* @param Options structure containing the Epic Online Services Account ID and index being accessed
	* @param OutItem the item for the given index, if it exists and is valid, use EOS_Ecom_CatalogItem_Release when finished
	*
	* @see EOS_Ecom_CatalogItem_Release
	* @see EOS_Ecom_GetItemImageInfoCount
	* @see EOS_Ecom_GetItemReleaseCount
	*
	* @return EOS_Success if the information is available and passed out in OutItem
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_Ecom_CatalogItemStale if the item information is stale
	*         EOS_NotFound if the item is not found
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_CopyOfferItemByIndex"))
	static EOSResult EOSEcomCopyOfferItemByIndex(UObject* WorldContextObject, FEOSEcomCopyOfferItemByIndexOptions Options, FEOSEcomCatalogItem& OutItem);

	/**
	* Fetches an item with a given ID.
	*
	* @param Options structure containing the item ID being accessed
	* @param OutItem the item for the given index, if it exists and is valid, use EOS_Ecom_CatalogItem_Release when finished
	*
	* @see EOS_Ecom_CatalogItem_Release
	* @see EOS_Ecom_GetItemImageInfoCount
	* @see EOS_Ecom_GetItemReleaseCount
	*
	* @return EOS_Success if the information is available and passed out in OutItem
	*         EOS_Ecom_CatalogItemStale if the item information is stale and passed out in OutItem
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_NotFound if the offer is not found
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_CopyItemById"))
	static EOSResult EOSEcomCopyItemById(UObject* WorldContextObject, FEOSEcomCopyItemByIdOptions Options, FEOSEcomCatalogItem& OutItem);

	/**
	* Fetch the number of images that are associated with a given cached offer for a local user.
	*
	* @return the number of images found.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_GetOfferImageInfoCount"))
	static int32 EOSEcomGetOfferImageInfoCount(UObject* WorldContextObject, FEOSEcomGetOfferImageInfoCountOptions Options);

	/**
	* Fetches an image from a given index.
	*
	* @param Options structure containing the offer ID and index being accessed
	* @param OutImageInfo the image for the given index, if it exists and is valid, use EOS_Ecom_KeyImageInfo_Release when finished
	*
	* @see EOS_Ecom_KeyImageInfo_Release
	*
	* @return EOS_Success if the information is available and passed out in OutImageInfo
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_Ecom_CatalogOfferStale if the associated offer information is stale
	*         EOS_NotFound if the image is not found
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_CopyOfferImageInfoByIndex"))
	static EOSResult EOSEcomCopyOfferImageInfoByIndex(UObject* WorldContextObject, FEOSEcomCopyOfferImageInfoByIndexOptions Options, FEOSEcomKeyImageInfo& OutImageInfo);

	/**
	* Fetch the number of images that are associated with a given cached item for a local user.
	*
	* @return the number of images found.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_GetItemImageInfoCount"))
	static int32 EOSEcomGetItemImageInfoCount(UObject* WorldContextObject, FEOSEcomGetItemImageInfoCountOptions Options);

	/**
	* Fetches an image from a given index.
	*
	* @param Options structure containing the item ID and index being accessed
	* @param OutImageInfo the image for the given index, if it exists and is valid, use EOS_Ecom_KeyImageInfo_Release when finished
	*
	* @see EOS_Ecom_KeyImageInfo_Release
	*
	* @return EOS_Success if the information is available and passed out in OutImageInfo
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_Ecom_CatalogItemStale if the associated item information is stale
	*         EOS_NotFound if the image is not found
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_CopyItemImageInfoByIndex"))
	static EOSResult EOSEcomCopyItemImageInfoByIndex(UObject* WorldContextObject, FEOSEcomCopyItemImageInfoByIndexOptions Options, FEOSEcomKeyImageInfo& OutImageInfo);

	/**
	* Fetch the number of releases that are associated with a given cached item for a local user.
	*
	* @return the number of releases found.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_GetItemReleaseCount"))
	static int32 EOSEcomGetItemReleaseCount(UObject* WorldContextObject, FEOSEcomGetItemReleaseCountOptions Options);

	/**
	* Fetches a release from a given index.
	*
	* @param Options structure containing the item ID and index being accessed
	* @param OutRelease the release for the given index, if it exists and is valid, use EOS_Ecom_CatalogRelease_Release when finished
	*
	* @see EOS_Ecom_CatalogRelease_Release
	*
	* @return EOS_Success if the information is available and passed out in OutRelease
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_Ecom_CatalogItemStale if the associated item information is stale
	*         EOS_NotFound if the release is not found
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_CopyItemReleaseByIndex"))
	static EOSResult EOSEcomCopyItemReleaseByIndex(UObject* WorldContextObject, FEOSEcomCopyItemReleaseByIndexOptions Options, FEOSEcomCatalogRelease& OutRelease);

	/**
	* Fetch the number of transactions that are cached for a given local user.
	*
	* @see EOS_Ecom_CheckoutCallbackInfo
	* @see EOS_Ecom_CopyTransactionByIndex
	*
	* @return the number of transactions found.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_GetTransactionCount"))
	static int32 EOSEcomGetTransactionCount(UObject* WorldContextObject, FEOSEcomGetTransactionCountOptions Options);

	/**
	* Fetches the transaction handle at the given index.
	*
	* @param Options structure containing the Epic Online Services Account ID and index being accessed
	*
	* @see EOS_Ecom_CheckoutCallbackInfo
	* @see EOS_Ecom_Transaction_Release
	*
	* @return EOS_Success if the information is available and passed out in OutTransaction
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_NotFound if the transaction is not found
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_CopyTransactionByIndex"))
	static EOSResult EOSEcomCopyTransactionByIndex(UObject* WorldContextObject, FEOSEcomCopyTransactionByIndexOptions Options, FEOSEcomHTransaction& OutTransaction);

	/**
	* Fetches the transaction handle at the given index.
	*
	* @param Options structure containing the Epic Online Services Account ID and transaction ID being accessed
	*
	* @see EOS_Ecom_CheckoutCallbackInfo
	* @see EOS_Ecom_Transaction_Release
	*
	* @return EOS_Success if the information is available and passed out in OutTransaction
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_NotFound if the transaction is not found
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_CopyTransactionById"))
	static EOSResult EOSEcomCopyTransactionById(UObject* WorldContextObject, FEOSEcomCopyTransactionByIdOptions Options, FEOSEcomHTransaction& OutTransaction);

	/**
	* The Ecom Transaction Interface exposes getters for accessing information about a completed transaction.
	* All Ecom Transaction Interface calls take a handle of type EOS_Ecom_HTransaction as the first parameter.
	* An EOS_Ecom_HTransaction handle is originally returned as part of the EOS_Ecom_CheckoutCallbackInfo struct.
	* An EOS_Ecom_HTransaction handle can also be retrieved from an EOS_HEcom handle using EOS_Ecom_CopyTransactionByIndex.
	* It is expected that after a transaction that EOS_Ecom_Transaction_Release is called.
	* When EOS_Platform_Release is called any remaining transactions will also be released.
	*
	* @see EOS_Ecom_CheckoutCallbackInfo
	* @see EOS_Ecom_GetTransactionCount
	* @see EOS_Ecom_CopyTransactionByIndex
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_Transaction_GetTransactionId"))
	static EOSResult EOSEcomTransaction_GetTransactionId(UObject* WorldContextObject, const FEOSEcomHTransaction& Handle, FString& OutTransactionId);

	/**
	* Fetch the number of entitlements that are part of this transaction.
	*
	* @param Options structure containing the Epic Online Services Account ID being accessed
	*
	* @see EOS_Ecom_Transaction_CopyEntitlementByIndex
	*
	* @return the number of entitlements found.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_Transaction_GetEntitlementsCount"))
	static int32 EOSEcomTransaction_GetEntitlementsCount(UObject* WorldContextObject, const FEOSEcomHTransaction& Handle, FEOSEcomTransactionGetEntitlementsCountOptions Options);

	/**
	* Fetches an entitlement from a given index.
	*
	* @param Options structure containing the index being accessed
	* @param OutEntitlement the entitlement for the given index, if it exists and is valid, use EOS_Ecom_Entitlement_Release when finished
	*
	* @see EOS_Ecom_Entitlement_Release
	*
	* @return EOS_Success if the information is available and passed out in OutEntitlement
	*         EOS_Ecom_EntitlementStale if the entitlement information is stale and passed out in OutEntitlement
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_NotFound if the entitlement is not found
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|Ecom", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_Ecom_Transaction_CopyEntitlementByIndex"))
	static EOSResult EOSEcomTransaction_CopyEntitlementByIndex(UObject* WorldContextObject, const FEOSEcomHTransaction& Handle, FEOSEcomTransactionCopyEntitlementByIndexOptions Options, FEOSEcomEntitlement& OutEntitlement);
private:
	static void Internal_OnQueryOwnershipCallback(const EOS_Ecom_QueryOwnershipCallbackInfo* Data);
	static void Internal_OnQueryOwnershipBySandboxIdsCallback(const EOS_Ecom_QueryOwnershipBySandboxIdsCallbackInfo* Data);
	static void Internal_OnQueryOwnershipTokenCallback(const EOS_Ecom_QueryOwnershipTokenCallbackInfo* Data);
	static void Internal_OnQueryEntitlementsCallback(const EOS_Ecom_QueryEntitlementsCallbackInfo* Data);
	static void Internal_OnQueryEntitlementTokenOptions(const EOS_Ecom_QueryEntitlementTokenCallbackInfo* Data);
	static void Internal_OnQueryOffersCallback(const EOS_Ecom_QueryOffersCallbackInfo* Data);
	static void Internal_OnCheckoutCallback(const EOS_Ecom_CheckoutCallbackInfo* Data);
	static void Internal_OnRedeemEntitlementsCallback(const EOS_Ecom_RedeemEntitlementsCallbackInfo* Data);
};
