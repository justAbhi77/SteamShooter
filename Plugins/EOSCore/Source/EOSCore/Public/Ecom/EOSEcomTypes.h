/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "eos_ecom_types.h"
#include "Core/EOSHelpers.h"
#include "Core/EOSCoreLogging.h"
#include "EOSEcomTypes.generated.h"

class UCoreEcom;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		ENUMS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

/**
 * An enumeration of the different ownership statuses.
 */
UENUM(BlueprintType)
enum class EEOSEOwnershipStatus : uint8
{
	/** The catalog item is not owned by the local user */
	EOS_OS_NotOwned = 0,
	/** The catalog item is owned by the local user */
	EOS_OS_Owned = 1
};

/**
 * An enumeration defining the type of catalog item.  The primary use is to identify how the item is expended.
 */
UENUM(BlueprintType)
enum class EEOSEEcomItemType : uint8
{
	/** This entitlement is intended to persist. */
	EOS_EIT_Durable = 0,
	/**
	 * This entitlement is intended to be transient and redeemed.
	 *
	 * @see EOS_Ecom_RedeemEntitlements
	 */
	EOS_EIT_Consumable = 1,
	/** This entitlement has a type that is not currently intneded for an in-game store. */
	EOS_EIT_Other = 2
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		STRUCTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
USTRUCT(BlueprintType)
struct FEOSEcomHTransaction
{
	GENERATED_BODY()
public:
	EOS_Ecom_TransactionHandle* Handle;
public:
	FEOSEcomHTransaction()
	{
	}

	FEOSEcomHTransaction(const EOS_Ecom_HTransaction& Data)
	{
		Handle = Data;
	}

public:
	operator EOS_Ecom_HTransaction() { return Handle; }
	operator EOS_Ecom_HTransaction() const { return Handle; }
};

/**
 * An identifier which is defined on a catalog item and stored with the backend catalog service.
 * The entitlement name may not be unique.  A catalog may be configured with multiple items with the
 * same entitlement name in order to define a logical grouping of entitlements.  This is used to
 * retrieve all entitlements granted to an account grouped in this way.
 *
 * @see EOS_Ecom_QueryEntitlements
 */
USTRUCT(BlueprintType)
struct FEOSEcomEntitlementName
{
	GENERATED_BODY()
public:
	FEOSEcomEntitlementName()
	{
	}

	FEOSEcomEntitlementName(const EOS_Ecom_EntitlementName& Data)
	{
		const FUTF8ToTCHAR NameChar(Data);
		Name = NameChar.Get();
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString Name;
};

/**
 * A unique identifier for an entitlement owned by an account.  An entitlement is always associated
 * with a single account.  The entitlement id is provided to allow redeeming the entitlement as
 * well as identify individual entitlement grants.
 *
 * @see EOS_Ecom_QueryEntitlements
 * @see EOS_Ecom_RedeemEntitlements
 */
USTRUCT(BlueprintType)
struct FEOSEcomEntitlementId
{
	GENERATED_BODY()
public:
	FEOSEcomEntitlementId()
	{
	}

	FEOSEcomEntitlementId(const EOS_Ecom_EntitlementId& Data)
	{
		const FUTF8ToTCHAR EntitlementIdChar(Data);
		EntitlementId = EntitlementIdChar.Get();
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	FString EntitlementId;
};

/**
 * Contains information about a single entitlement associated with an account. Instances of this structure are
 * created by EOS_Ecom_CopyEntitlementByIndex, EOS_Ecom_CopyEntitlementByNameAndIndex, or EOS_Ecom_CopyEntitlementById.
 * They must be passed to EOS_Ecom_Entitlement_Release.
 */
USTRUCT(BlueprintType)
struct FEOSEcomEntitlement
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Name of the entitlement */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEcomEntitlementName EntitlementName;
	/** Id of the entitlement owned by an account */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEcomEntitlementId EntitlementId;
	/** Id of the item associated with the offer which granted this entitlement */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString CatalogItemId;
	/**
	 * If queried using pagination then ServerIndex represents the index of the entitlement as it
	 * exists on the server.  If not queried using pagination then ServerIndex will be -1.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	int32 ServerIndex;
	/** If true then the catalog has this entitlement marked as redeemed */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	bool bRedeemed;
	/** If not -1 then this is a POSIX timestamp that this entitlement will end */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString EndTimestamp;
public:
	explicit FEOSEcomEntitlement()
		: ApiVersion(EOS_ECOM_ENTITLEMENT_API_LATEST)
		, ServerIndex(0)
		, bRedeemed(false)
	{
	}

	FEOSEcomEntitlement(const EOS_Ecom_Entitlement& Data)
		: ApiVersion(EOS_ECOM_ENTITLEMENT_API_LATEST)
		, EntitlementName(Data.EntitlementName)
		, EntitlementId(Data.EntitlementId)
		, ServerIndex(Data.ServerIndex)
		, bRedeemed((Data.bRedeemed > 0))
		, EndTimestamp(LexToString(Data.EndTimestamp))
	{
		const FUTF8ToTCHAR CatalogItemIdChar(Data.CatalogItemId);
		CatalogItemId = CatalogItemIdChar.Get();
	}
};

/**
 * Contains information about a single item ownership associated with an account. This structure is
 * returned as part of the EOS_Ecom_QueryOwnershipCallbackInfo structure.
 */
USTRUCT(BlueprintType)
struct FEOSEcomItemOwnership
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Id of the catalog item */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString Id;
	/** Is this catalog item owned by the local user */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	EEOSEOwnershipStatus OwnershipStatus;
public:
	explicit FEOSEcomItemOwnership()
		: ApiVersion(EOS_ECOM_ITEMOWNERSHIP_API_LATEST)
		, OwnershipStatus(EEOSEOwnershipStatus::EOS_OS_NotOwned)
	{
	}

	FEOSEcomItemOwnership(const EOS_Ecom_ItemOwnership& Data)
		: ApiVersion(EOS_ECOM_ITEMOWNERSHIP_API_LATEST)
		, OwnershipStatus(static_cast<EEOSEOwnershipStatus>(Data.OwnershipStatus))
	{
		const FUTF8ToTCHAR IdChar(Data.Id);
		Id = IdChar.Get();
	}
};

/**
 * Contains information about a single item within the catalog. Instances of this structure are created
 * by EOS_Ecom_CopyOfferItemByIndex. They must be passed to EOS_Ecom_CatalogItem_Release.
 */
USTRUCT(BlueprintType)
struct FEOSEcomCatalogItem
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Product namespace in which this item exists */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString CatalogNamespace;
	/** The ID of this item */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString Id;
	/** The entitlement name associated with this item */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEcomEntitlementName EntitlementName;
	/** Localized UTF-8 title of this item */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString TitleText;
	/** Localized UTF-8 description of this item */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString DescriptionText;
	/** Localized UTF-8 long description of this item */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString LongDescriptionText;
	/** Localized UTF-8 technical details of this item */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString TechnicalDetailsText;
	/** Localized UTF-8 developer of this item */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString DeveloperText;
	/** The type of item as defined in the catalog */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	EEOSEEcomItemType ItemType;
	/** If not -1 then this is the POSIX timestamp that the entitlement will end */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString EntitlementEndTimestamp;
public:
	explicit FEOSEcomCatalogItem()
		: ApiVersion(EOS_ECOM_CATALOGITEM_API_LATEST)
		, ItemType(EEOSEEcomItemType::EOS_EIT_Other)
	{
	}

	FEOSEcomCatalogItem(const EOS_Ecom_CatalogItem& Data)
		: ApiVersion(EOS_ECOM_CATALOGITEM_API_LATEST)
		, CatalogNamespace(Data.CatalogNamespace)
		, Id(UTF8_TO_TCHAR(Data.Id))
		, EntitlementName(Data.EntitlementName)
		, TitleText(Data.TitleText)
		, DescriptionText(Data.DescriptionText)
		, LongDescriptionText(Data.LongDescriptionText)
		, TechnicalDetailsText(Data.TechnicalDetailsText)
		, DeveloperText(Data.DeveloperText)
		, ItemType(static_cast<EEOSEEcomItemType>(Data.ItemType))
		, EntitlementEndTimestamp(LexToString(Data.EntitlementEndTimestamp))
	{
	}
};

/**
 * Contains information about a single offer within the catalog. Instances of this structure are
 * created by EOS_Ecom_CopyOfferByIndex. They must be passed to EOS_Ecom_CatalogOffer_Release.
 * Prices are stored in the lowest denomination for the associated currency.  If CurrencyCode is
 * "USD" then a price of 299 represents "$2.99".
 */
USTRUCT(BlueprintType)
struct FEOSEcomCatalogOffer
{
	GENERATED_BODY()
public:
	/**
	 * The index of this offer as it exists on the server.
	 * This is useful for understanding pagination Data.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	int32 ServerIndex;
	/** Product namespace in which this offer exists */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString CatalogNamespace;
	/** The ID of this offer */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString Id;
	/** Localized UTF-8 title of this offer */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString TitleText;
	/** Localized UTF-8 description of this offer */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString DescriptionText;
	/** Localized UTF-8 long description of this offer */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString LongDescriptionText;
	/**
	 * Deprecated.
	 * EOS_Ecom_CatalogOffer::TechnicalDetailsText has been deprecated.
	 * EOS_Ecom_CatalogItem::TechnicalDetailsText is still valid.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString TechnicalDetailsText;
	/** The Currency Code for this offer */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString CurrencyCode;
	/**
	 * If this value is EOS_Success then OriginalPrice, CurrentPrice, and DiscountPercentage contain valid Data.
	 * Otherwise this value represents the error that occurred on the price query.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	EOSResult PriceResult;
	/** The original price of this offer. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	int64 OriginalPrice;
	/** The current price including discounts of this offer. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	int64 CurrentPrice;
	/** A value from 0 to 100 define the percentage of the OrignalPrice that the CurrentPrice represents */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	int32 DiscountPercentage;
	/** Contains the POSIX timestamp that the offer expires or -1 if it does not expire */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString ExpirationTimestamp;
	/**
	 * The maximum number of times that the offer can be purchased.
	 * A negative value implies there is no limit.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	int32 PurchaseLimit;
	/** True if the user can purchase this offer. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	bool bAvailableForPurchase;
	/** The original price of this offer as a 64-bit number. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	int64 OriginalPrice64;
	/** The current price including discounts of this offer as a 64-bit number. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	int64 CurrentPrice64;
	/** The decimal point for the provided price.  For example, DecimalPoint '2' and CurrentPrice64 '12345' would be '123.45'. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	int32 DecimalPoint;
	/** Timestamp indicating when the time when the offer was released. Can be ignored if set to -1. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	int64 ReleaseDateTimestamp;
	/** Timestamp indicating the effective date of the offer. Can be ignored if set to -1. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	int64 EffectiveDateTimestamp;
public:
	explicit FEOSEcomCatalogOffer()
		: ServerIndex(0)
		, PriceResult(EOSResult::EOS_ServiceFailure)
		, OriginalPrice(0)
		, CurrentPrice(0)
		, DiscountPercentage(0)
		, PurchaseLimit(0)
		, bAvailableForPurchase(false)
		, OriginalPrice64(0)
		, CurrentPrice64(0)
		, DecimalPoint(0)
		, ReleaseDateTimestamp(0)
		, EffectiveDateTimestamp(0)
	{
	}

	FEOSEcomCatalogOffer(const EOS_Ecom_CatalogOffer& Data)
		: ServerIndex(Data.ServerIndex)
		, CatalogNamespace(Data.CatalogNamespace)
		, Id(Data.Id)
		, TitleText(Data.TitleText)
		, DescriptionText(Data.DescriptionText)
		, LongDescriptionText(Data.LongDescriptionText)
		, TechnicalDetailsText(Data.TechnicalDetailsText_DEPRECATED)
		, CurrencyCode(Data.CurrencyCode)
		, PriceResult(EOSHelpers::ToEOSCoreResult(Data.PriceResult))
		, OriginalPrice(Data.OriginalPrice64)
		, CurrentPrice(Data.CurrentPrice64)
		, DiscountPercentage(Data.DiscountPercentage)
		, ExpirationTimestamp(LexToString(Data.ExpirationTimestamp))
		, PurchaseLimit(Data.PurchaseLimit)
		, bAvailableForPurchase((Data.bAvailableForPurchase > 0))
		, OriginalPrice64(Data.OriginalPrice64)
		, CurrentPrice64(Data.CurrentPrice64)
		, DecimalPoint(Data.DecimalPoint)
		, ReleaseDateTimestamp(Data.ReleaseDateTimestamp)
		, EffectiveDateTimestamp(Data.EffectiveDateTimestamp)
	{
	}
};

/**
 * Contains information about a key image used by the catalog.  Instances of this structure are
 * created by EOS_Ecom_CopyItemImageInfoByIndex.  They must be passed to EOS_Ecom_KeyImageInfo_Release.
 * A Key Image is defined within Dev Portal and is associated with a Catalog Item.  A Key Image is
 * intended to be used to provide imagery for an in-game store.
 *
 * @see EOS_Ecom_CopyItemImageInfoByIndex
 * @see EOS_Ecom_KeyImageInfo_Release
 */
USTRUCT(BlueprintType)
struct FEOSEcomKeyImageInfo
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Describes the usage of the image (ex: home_thumbnail) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString Type;
	/** The URL of the image */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString Url;
	/** The expected width in pixels of the image */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	int32 Width;
	/** The expected height in pixels of the image */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	int32 Height;
public:
	explicit FEOSEcomKeyImageInfo()
		: ApiVersion(EOS_ECOM_KEYIMAGEINFO_API_LATEST)
		, Width(0)
		, Height(0)
	{
	}

	FEOSEcomKeyImageInfo(const EOS_Ecom_KeyImageInfo& Data)
		: ApiVersion(EOS_ECOM_KEYIMAGEINFO_API_LATEST)
		, Type(Data.Type)
		, Url(Data.Url)
		, Width(Data.Width)
		, Height(Data.Height)
	{
	}
};

/**
 * Contains information about a single release within the catalog. Instances of this structure are
 * created by EOS_Ecom_CopyItemReleaseByIndex. They must be passed to EOS_Ecom_CatalogRelease_Release.
 */
USTRUCT(BlueprintType)
struct FEOSEcomCatalogRelease
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** The number of APP IDs */
	int32 CompatibleAppIdCount;
	/** A list of compatible APP IDs */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	TArray<FString> CompatibleAppIds;
	/** The number of platforms */
	int32 CompatiblePlatformCount;
	/** A list of compatible Platforms */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	TArray<FString> CompatiblePlatforms;
	/** Release note for compatible versions */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString ReleaseNote;
public:
	explicit FEOSEcomCatalogRelease()
		: ApiVersion(EOS_ECOM_CATALOGRELEASE_API_LATEST)
		, CompatibleAppIdCount(0)
		, CompatiblePlatformCount(0)
	{
	}

	FEOSEcomCatalogRelease(const EOS_Ecom_CatalogRelease& Data)
		: ApiVersion(EOS_ECOM_CATALOGRELEASE_API_LATEST)
	{
		for (int32 i = 0; i < (int32)Data.CompatibleAppIdCount; i++)
		{
			CompatibleAppIds.Add(Data.CompatibleAppIds[i]);
		}
		CompatibleAppIdCount = CompatibleAppIds.Num();

		for (int32 i = 0; i < (int32)Data.CompatiblePlatformCount; i++)
		{
			CompatiblePlatforms.Add(Data.CompatiblePlatforms[i]);
		}
		CompatiblePlatformCount = CompatiblePlatforms.Num();

		ReleaseNote = Data.ReleaseNote;
	}
};

/**
 * Contains information about a request to purchase a single offer.  This structure is set as part
 * of the EOS_Ecom_CheckoutOptions structure.
 */
USTRUCT(BlueprintType)
struct FEOSEcomCheckoutEntry
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** The ID of the offer to purchase */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString OfferId;
public:
	explicit FEOSEcomCheckoutEntry()
		: ApiVersion(EOS_ECOM_CHECKOUTENTRY_API_LATEST)
	{
	}
};

/**
 * Input parameters for the EOS_Ecom_QueryOwnership Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomQueryOwnershipOptions
{
	GENERATED_BODY()
public:
	/** Local user whose ownership to query */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** List of catalog item ids to check for ownership, matching in number to the CatalogItemIdCount */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	TArray<FString> CatalogItemIds;
	/** Optional product namespace, if not the one specified during initialization */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString CatalogNamespace;
public:
	FEOSEcomQueryOwnershipOptions() = default;
};

/**
 * Output parameters for the EOS_Ecom_QueryOwnership Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomQueryOwnershipCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful query, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Ecom_QueryOwnership */
	void* ClientData;
	/** Local user whose ownership was queried */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** List of catalog items and their ownership status */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	TArray<FEOSEcomItemOwnership> ItemOwnership;
	/** Number of ownership results are included in this callback */
	int32 ItemOwnershipCount;
public:
	explicit FEOSEcomQueryOwnershipCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		, ClientData(nullptr)
		, ItemOwnershipCount(0)
	{
	}

	FEOSEcomQueryOwnershipCallbackInfo(const EOS_Ecom_QueryOwnershipCallbackInfo& Data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(Data.ResultCode))
		, ClientData(Data.ClientData)
		, LocalUserId(Data.LocalUserId)
		, ItemOwnershipCount(Data.ItemOwnershipCount)
	{
		LogVerbose("ItemOwnershipCount: %i", Data.ItemOwnershipCount);

		for (int32 i = 0; i < static_cast<int32>(Data.ItemOwnershipCount); i++)
		{
			ItemOwnership.Add(Data.ItemOwnership[i]);
		}
	}
};


/**
 * A unique identifier for a catalog item defined and stored with the backend catalog service.
 * A catalog item represents a distinct object within the catalog.  When acquired by an account, an
 * entitlement is granted that references a specific catalog item.
 */
USTRUCT(BlueprintType)
struct FEOSEcomCatalogOfferId
{
	GENERATED_BODY()
public:
	/** SandboxId */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString CatalogItemId;

	FEOSEcomCatalogOfferId() = default;
	FEOSEcomCatalogOfferId(const EOS_Ecom_CatalogOfferId& Data)
	{
		const FUTF8ToTCHAR CatalogOfferIdChar(Data);
		CatalogItemId = CatalogOfferIdChar.Get();
	}
};

/**
 * A unique identifier for the Sandbox.
 * 
 * @see EOS_Ecom_QueryOwnershipBySandboxIds
 */
USTRUCT(BlueprintType)
struct FEOSEcomSandboxId
{
	GENERATED_BODY()
public:
	/** SandboxId */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString CatalogItemId;

	FEOSEcomSandboxId() = default;
	FEOSEcomSandboxId(const EOS_Ecom_SandboxId& Data)
	{
		const FUTF8ToTCHAR SandboxIdChar(Data);
		CatalogItemId = SandboxIdChar.Get();
	}
};

/**
 * A unique identifier for a catalog item defined and stored with the backend catalog service.
 * A catalog item represents a distinct object within the catalog.  When acquired by an account, an
 * entitlement is granted that references a specific catalog item.
 */
USTRUCT(BlueprintType)
struct FEOSEcomCatalogItemId
{
	GENERATED_BODY()
public:
	/** SandboxId */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString CatalogItemId;

	FEOSEcomCatalogItemId() = default;
	FEOSEcomCatalogItemId(const EOS_Ecom_CatalogItemId& Data)
	{
		const FUTF8ToTCHAR CatalogItemIdChar(Data);
		CatalogItemId = CatalogItemIdChar.Get(); 
	}
};

/**
 * Contains all owned catalog items for a sandbox ID. This structure is
 * returned as part of the EOS_Ecom_QueryOwnershipBySandboxIdsCallbackInfo structure.
 * Note: The SandboxID and CatalogItemId strings will not remain valid after EOS_Ecom_OnQueryOwnershipBySandboxIdsCallback, so a copy should be as needed.
 */
USTRUCT(BlueprintType)
struct FEOSEcomSandboxIdItemOwnership
{
	GENERATED_BODY()
public:
	/** SandboxId */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEcomSandboxId SandboxId;
	/** List of all owned catalog items for this SandboxId */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	TArray<FEOSEcomCatalogItemId> OwnedCatalogItemIds;

	FEOSEcomSandboxIdItemOwnership() = default;

	FEOSEcomSandboxIdItemOwnership(const EOS_Ecom_SandboxIdItemOwnership& Data)
		: SandboxId(Data.SandboxId)
	{
		for (int32 i = 0; i < static_cast<int32>(Data.OwnedCatalogItemIdsCount); i++)
		{
			OwnedCatalogItemIds.Add(Data.OwnedCatalogItemIds[i]);
		}
	}
};

/**
 * Output parameters for the EOS_Ecom_QueryOwnershipBySandboxIds Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomQueryOwnershipBySandboxIdsCallbackInfo
{
	GENERATED_BODY()
public:
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Ecom_QueryOwnership */
	void* ClientData;
	/** The Epic Account ID of the local user whose ownership was queried */
	FEOSEpicAccountId LocalUserId;
	/** List of SandboxIds and their corresponding owned catalog item Ids. If there are no ownership items, the OwnedCatalogItemIdsCount is 0 and OwnedCatalogItemIds is null. */
	TArray<FEOSEcomSandboxIdItemOwnership> SandboxIdItemOwnerships;
public:
	explicit FEOSEcomQueryOwnershipBySandboxIdsCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		, ClientData(nullptr)
	{
	}

	FEOSEcomQueryOwnershipBySandboxIdsCallbackInfo(const EOS_Ecom_QueryOwnershipBySandboxIdsCallbackInfo& Data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(Data.ResultCode))
		, ClientData(Data.ClientData)
		, LocalUserId(Data.LocalUserId)
	{
		for (int32 i = 0; i < static_cast<int32>(Data.SandboxIdItemOwnershipsCount); i++)
		{
			SandboxIdItemOwnerships.Add(Data.SandboxIdItemOwnerships[i]);
		}
	}
};

/**
 * Input parameters for the EOS_Ecom_QueryOwnershipBySandboxIds function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomQueryOwnershipBySandboxIdsOptions
{
	GENERATED_BODY()
public:
	/** The Epic Account ID of the local user whose ownership to query. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** The array of Sandbox IDs to check for ownership. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	TArray<FEOSEcomSandboxId> SandboxIds;
public:
	FEOSEcomQueryOwnershipBySandboxIdsOptions() = default;
};

/**
 * Input parameters for the EOS_Ecom_QueryOwnershipToken Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomQueryOwnershipTokenOptions
{
	GENERATED_BODY()
public:
	/** Local user whose ownership token to query */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** List of catalog item ids to check for ownership, matching in number to the CatalogItemIdCount */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	TArray<FString> CatalogItemIds;
	/** Number of catalog item ids to query */
	int32 CatalogItemIdCount;
	/** Optional product namespace, if not the one specified during initialization */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString CatalogNamespace;
public:
	FEOSEcomQueryOwnershipTokenOptions() = default;
};

/**
 * Output parameters for the EOS_Ecom_QueryOwnershipToken Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomQueryOwnershipTokenCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful query, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Ecom_QueryOwnershipToken */
	void* ClientData;
	/** Local user whose ownership token was queried */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** Ownership token containing details about the catalog items queried */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString OwnershipToken;
public:
	explicit FEOSEcomQueryOwnershipTokenCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		, ClientData(nullptr)
	{
	}

	FEOSEcomQueryOwnershipTokenCallbackInfo(const EOS_Ecom_QueryOwnershipTokenCallbackInfo& Data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(Data.ResultCode))
		, ClientData(Data.ClientData)
		, LocalUserId(Data.LocalUserId)
		, OwnershipToken(Data.OwnershipToken)
	{
	}
};

/**
 * Input parameters for the EOS_Ecom_QueryEntitlements Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomQueryEntitlementsOptions
{
	GENERATED_BODY()
public:
	/** Local user whose entitlement to query */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** List of entitlement names, matching in number to the EntitlementNameCount */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	TArray<FEOSEcomEntitlementName> EntitlementNames;
	/** Determines rater or not to include redeemed entitlements */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	bool bIncludeRedeemed;
public:
	FEOSEcomQueryEntitlementsOptions()
		: bIncludeRedeemed(false)
	{
	}
};

/**
 * Input parameters for the EOS_Ecom_QueryEntitlementToken function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomQueryEntitlementTokenOptions
{
	GENERATED_BODY()
public:
	/** The Epic Account ID of the local user whose Entitlements you want to retrieve */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** An array of Entitlement Names that you want to check */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	TArray<FEOSEcomEntitlementName> EntitlementNames;
};

/**
 * Output parameters for the EOS_Ecom_QueryEntitlements Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomQueryEntitlementsCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful query, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Ecom_QueryEntitlements */
	void* ClientData;
	/** Local user whose entitlement was queried */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
public:
	explicit FEOSEcomQueryEntitlementsCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		, ClientData(nullptr)
	{
	}

	FEOSEcomQueryEntitlementsCallbackInfo(const EOS_Ecom_QueryEntitlementsCallbackInfo& Data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(Data.ResultCode))
		, ClientData(Data.ClientData)
		, LocalUserId(Data.LocalUserId)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSEcomQueryEntitlementTokenCallbackInfo
{
	GENERATED_BODY()
public:
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	EOSResult ResultCode;
	/** The Epic Account ID of the local user whose entitlement was queried */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** Entitlements token containing details about the catalog items queried */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString EntitlementToken;
public:
	explicit FEOSEcomQueryEntitlementTokenCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
	{
	}

	FEOSEcomQueryEntitlementTokenCallbackInfo(const EOS_Ecom_QueryEntitlementTokenCallbackInfo& Data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(Data.ResultCode))
		, LocalUserId(Data.LocalUserId)
	{
		const FUTF8ToTCHAR EntitlementTokenChar(Data.EntitlementToken);
		EntitlementToken = EntitlementTokenChar.Get(); 
	}
};

/**
 * Input parameters for the EOS_Ecom_QueryOffers Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomQueryOffersOptions
{
	GENERATED_BODY()
public:
	/** Local user whose offer to query */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** If not provided then the SandboxId is used as the catalog namespace */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString OverrideCatalogNamespace;
public:
	explicit FEOSEcomQueryOffersOptions() = default;
};

/**
 * Output parameters for the EOS_Ecom_QueryOffers Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomQueryOffersCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful query, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Ecom_QueryOffers */
	void* ClientData;
	/** Local user whose offer was queried */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
public:
	explicit FEOSEcomQueryOffersCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		, ClientData(nullptr)
	{
	}

	FEOSEcomQueryOffersCallbackInfo(const EOS_Ecom_QueryOffersCallbackInfo& Data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(Data.ResultCode))
		, ClientData(Data.ClientData)
		, LocalUserId(Data.LocalUserId)
	{
	}
};

/**
 * Input parameters for the EOS_Ecom_Checkout Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomCheckoutOptions
{
	GENERATED_BODY()
public:
	/** The Account ID of the user which is initiating the purchase */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** If not provided then the SandboxId is used as the catalog namespace */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString OverrideCatalogNamespace;
	/** The items that the user intends to purchase */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	TArray<FEOSEcomCheckoutEntry> Entries;
public:
	FEOSEcomCheckoutOptions() = default;
};

/**
 * Output parameters for the EOS_Ecom_Checkout Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomCheckoutCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful query, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Ecom_Checkout */
	void* ClientData;
	/** The Account ID of the user which initiated the purchase */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** The transaction id which can be used to obtain an EOS_Ecom_HTransaction using EOS_Ecom_CopyTransactionById. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString TransactionId;
public:
	explicit FEOSEcomCheckoutCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		, ClientData(nullptr)
	{
	}

	FEOSEcomCheckoutCallbackInfo(const EOS_Ecom_CheckoutCallbackInfo& Data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(Data.ResultCode))
		, ClientData(Data.ClientData)
		, LocalUserId(Data.LocalUserId)
		, TransactionId(Data.TransactionId)
	{
	}
};

/**
 * Input parameters for the EOS_Ecom_RedeemEntitlements Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomRedeemEntitlementsOptions
{
	GENERATED_BODY()
public:
	/** The Account ID of the user which is redeeming entitlements */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** The entitlements to redeem. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	TArray<FEOSEcomEntitlementId> EntitlementIds;
public:
	explicit FEOSEcomRedeemEntitlementsOptions() = default;
};

/**
 * Output parameters for the EOS_Ecom_RedeemEntitlements Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomRedeemEntitlementsCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful query, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Ecom_RedeemEntitlements */
	void* ClientData;
	/** The Account ID of the user which has redeemed entitlements */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/* The number of redeemed Entitlements */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	int32 RedeemedEntitlementIdsCount;
public:
	explicit FEOSEcomRedeemEntitlementsCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		, ClientData(nullptr)
		, RedeemedEntitlementIdsCount(0)
	{
	}

	FEOSEcomRedeemEntitlementsCallbackInfo(const EOS_Ecom_RedeemEntitlementsCallbackInfo& Data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(Data.ResultCode))
		, ClientData(Data.ClientData)
		, LocalUserId(Data.LocalUserId)
		, RedeemedEntitlementIdsCount(Data.RedeemedEntitlementIdsCount)
	{
	}
};

/**
 * Input parameters for the EOS_Ecom_GetLastRedeemedEntitlementsCount function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomGetLastRedeemedEntitlementsCountOptions
{
	GENERATED_BODY()
public:
	/** The Epic Account ID of the local user for who to retrieve the last redeemed entitlements count */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
};

USTRUCT(BlueprintType)
struct FEOSEcomCopyLastRedeemedEntitlementByIndexOptions
{
	GENERATED_BODY()
public:
	/** The Epic Account ID of the local user for who to retrieve the last redeemed entitlements count */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** Index of the last redeemed entitlement id to retrieve from the cache */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	int32 RedeemedEntitlementIndex;
public:
	FEOSEcomCopyLastRedeemedEntitlementByIndexOptions()
		: RedeemedEntitlementIndex(0)
	{
	}
};

/**
 * Input parameters for the EOS_Ecom_GetEntitlementsCount Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomGetEntitlementsCountOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Local user for which to retrieve the entitlement count */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
public:
	explicit FEOSEcomGetEntitlementsCountOptions() 
		: ApiVersion(EOS_ECOM_GETENTITLEMENTSCOUNT_API_LATEST)
	{
	}
};

/**
 * Input parameters for the EOS_Ecom_GetEntitlementsByNameCount Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomGetEntitlementsByNameCountOptions
{
	GENERATED_BODY()
public:
	/** Local user for which to retrieve the entitlement count */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** Name of the entitlement to count in the cache */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEcomEntitlementName EntitlementName;
public:
	explicit FEOSEcomGetEntitlementsByNameCountOptions() = default;
};

/**
 * Input parameters for the EOS_Ecom_CopyEntitlementByIndex Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomCopyEntitlementByIndexOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Local user whose entitlement is being copied */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** Index of the entitlement to retrieve from the cache */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	int32 EntitlementIndex;
public:
	explicit FEOSEcomCopyEntitlementByIndexOptions()
		: ApiVersion(EOS_ECOM_COPYENTITLEMENTBYINDEX_API_LATEST)
		, EntitlementIndex(0)
	{
	}
};

/**
 * Input parameters for the EOS_Ecom_CopyEntitlementByNameAndIndex Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomCopyEntitlementByNameAndIndexOptions
{
	GENERATED_BODY()
public:
	/** Local user whose entitlement is being copied */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** Name of the entitlement to retrieve from the cache */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEcomEntitlementName EntitlementName;
	/** Index of the entitlement within the named set to retrieve from the cache. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	int32 Index;
public:
	FEOSEcomCopyEntitlementByNameAndIndexOptions()
		: Index(0)
	{
	}
};

/**
 * Input parameters for the EOS_Ecom_CopyEntitlementById Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomCopyEntitlementByIdOptions
{
	GENERATED_BODY()
public:
	/** Local user whose entitlement is being copied */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** Id of the entitlement to retrieve from the cache */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEcomEntitlementId EntitlementId;
public:
	explicit FEOSEcomCopyEntitlementByIdOptions() = default;
};

/**
 * Input parameters for the EOS_Ecom_GetOfferCount Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomGetOfferCountOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Local user whose offers are being accessed */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
public:
	explicit FEOSEcomGetOfferCountOptions()
		: ApiVersion(EOS_ECOM_GETOFFERCOUNT_API_LATEST)
	{
	}
};

/**
 * Input parameters for the EOS_Ecom_CopyOfferByIndex Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomCopyOfferByIndexOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Local user whose offer is being copied */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** The index of the offer to get. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	int32 OfferIndex;
public:
	explicit FEOSEcomCopyOfferByIndexOptions()
		: ApiVersion(EOS_ECOM_COPYOFFERBYINDEX_API_LATEST)
		, OfferIndex(0)
	{
	}
};

/**
 * Input parameters for the EOS_Ecom_CopyOfferById Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomCopyOfferByIdOptions
{
	GENERATED_BODY()
public:
	/** Local user whose offer is being copied */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** The id of the offer to get. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString OfferId;
public:
	explicit FEOSEcomCopyOfferByIdOptions() = default;
};

/**
 * Input parameters for the EOS_Ecom_GetOfferItemCount Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomGetOfferItemCountOptions
{
	GENERATED_BODY()
public:
	/** Local user whose offer is being copied */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** The id of the offer to get. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString OfferId;
public:
	explicit FEOSEcomGetOfferItemCountOptions() = default;
};

/**
 * Input parameters for the EOS_Ecom_CopyOfferById Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomCopyOfferItemByIndexOptions
{
	GENERATED_BODY()
public:
	/** Local user whose item is being copied */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** The ID of the offer to get the items for. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString OfferId;
	/** The index of the item to get. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	int32 ItemIndex;
public:
	FEOSEcomCopyOfferItemByIndexOptions()
		: ItemIndex(0)
	{
	}
};

/**
 * Input parameters for the EOS_Ecom_CopyItemById Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomCopyItemByIdOptions
{
	GENERATED_BODY()
public:
	/** Local user whose item is being copied */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** The ID of the item to get. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString ItemId;
public:
	explicit FEOSEcomCopyItemByIdOptions() = default;
};

/**
 * Input parameters for the EOS_Ecom_GetOfferImageInfoCount Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomGetOfferImageInfoCountOptions
{
	GENERATED_BODY()
public:
	/** Local user whose offer image is being accessed. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** The ID of the offer to get the images for. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString OfferId;
public:
	explicit FEOSEcomGetOfferImageInfoCountOptions() = default;
};

/**
 * Input parameters for the EOS_Ecom_CopyOfferImageInfoByIndex Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomCopyOfferImageInfoByIndexOptions
{
	GENERATED_BODY()
public:
	/** Local user whose offer image is being copied. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** The ID of the offer to get the images for. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString OfferId;
	/** The index of the image to get. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	int32 ImageInfoIndex;
public:
	FEOSEcomCopyOfferImageInfoByIndexOptions()
		: ImageInfoIndex(0)
	{
	}
};

/**
 * Input parameters for the EOS_Ecom_GetItemImageInfoCount Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomGetItemImageInfoCountOptions
{
	GENERATED_BODY()
public:
	/** Local user whose item image is being accessed */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** The ID of the item to get the images for. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString ItemId;
public:
	explicit FEOSEcomGetItemImageInfoCountOptions() = default;
};

/**
 * Input parameters for the EOS_Ecom_CopyItemImageInfoByIndex Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomCopyItemImageInfoByIndexOptions
{
	GENERATED_BODY()
public:
	/** Local user whose item image is being copied */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** The ID of the item to get the images for. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString ItemId;
	/** The index of the image to get. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	int32 ImageInfoIndex;
public:
	FEOSEcomCopyItemImageInfoByIndexOptions()
		: ImageInfoIndex(0)
	{
	}
};

/**
 * Input parameters for the EOS_Ecom_GetItemReleaseCount Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomGetItemReleaseCountOptions
{
	GENERATED_BODY()
public:
	/** Local user whose item release is being accessed */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** The ID of the item to get the releases for. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString ItemId;
public:
	explicit FEOSEcomGetItemReleaseCountOptions() = default;
};

/**
 * Input parameters for the EOS_Ecom_CopyItemReleaseByIndex Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomCopyItemReleaseByIndexOptions
{
	GENERATED_BODY()
public:
	/** Local user whose item release is being copied */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** The ID of the item to get the releases for. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString ItemId;
	/** The index of the release to get. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	int32 ReleaseIndex;
public:
	FEOSEcomCopyItemReleaseByIndexOptions()
		: ReleaseIndex(0)
	{
	}
};

/**
 * Input parameters for the EOS_Ecom_GetTransactionCount Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomGetTransactionCountOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Local user whose transaction count to get */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
public:
	explicit FEOSEcomGetTransactionCountOptions()
		: ApiVersion(EOS_ECOM_GETTRANSACTIONCOUNT_API_LATEST)
	{
	}
};

/**
 * Input parameters for the EOS_Ecom_CopyTransactionByIndex Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomCopyTransactionByIndexOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Local user whose transaction to get */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** The index of the transaction to get */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	int32 TransactionIndex;
public:
	explicit FEOSEcomCopyTransactionByIndexOptions()
		: ApiVersion(EOS_ECOM_COPYTRANSACTIONBYINDEX_API_LATEST)
		, TransactionIndex(0)
	{
	}
};

/**
 * Input parameters for the EOS_Ecom_CopyTransactionById Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomCopyTransactionByIdOptions
{
	GENERATED_BODY()
public:
	/** Local user whose transaction to get */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
	/** The id of the transaction to get */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FString TransactionId;
public:
	explicit FEOSEcomCopyTransactionByIdOptions() = default;
};

/**
 * Input parameters for the EOS_Ecom_Transaction_GetEntitlementsCount Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomTransactionGetEntitlementsCountOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Local user for which to retrieve the entitlement count */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	FEOSEpicAccountId LocalUserId;
public:
	explicit FEOSEcomTransactionGetEntitlementsCountOptions()
		: ApiVersion(EOS_ECOM_COPYTRANSACTIONBYID_API_LATEST)
	{
	}
};

/**
 * Input parameters for the EOS_Ecom_Transaction_CopyEntitlementByIndex Function.
 */
USTRUCT(BlueprintType)
struct FEOSEcomTransactionCopyEntitlementByIndexOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** The index of the entitlement to get */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ecom")
	int32 EntitlementIndex;
public:
	explicit FEOSEcomTransactionCopyEntitlementByIndexOptions()
		: ApiVersion(EOS_ECOM_COPYTRANSACTIONBYID_API_LATEST)
		, EntitlementIndex(0)
	{
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		DELEGATES
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnEcomQueryOwnershipCallback, const FEOSEcomQueryOwnershipCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnEcomQueryOwnershipBySandboxIds, const FEOSEcomQueryOwnershipBySandboxIdsCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnEcomQueryOwnershipTokenCallback, const FEOSEcomQueryOwnershipTokenCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnEcomQueryEntitlementsCallback, const FEOSEcomQueryEntitlementsCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnEcomQueryEntitlementTokenCallback, const FEOSEcomQueryEntitlementTokenCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnEcomQueryQueryOffersCallback, const FEOSEcomQueryOffersCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnEcomQueryCheckoutCallback, const FEOSEcomCheckoutCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnEcomQueryRedeemEntitlementsCallback, const FEOSEcomRedeemEntitlementsCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEcomQueryOwnershipCallbackDelegate, const FEOSEcomQueryOwnershipCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEcomQueryOwnershipTokenCallbackDelegate, const FEOSEcomQueryOwnershipTokenCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEcomQueryEntitlementsCallbackDelegate, const FEOSEcomQueryEntitlementsCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEcomQueryQueryOffersCallbackDelegate, const FEOSEcomQueryOffersCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEcomQueryCheckoutCallbackDelegate, const FEOSEcomCheckoutCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEcomQueryRedeemEntitlementsCallbackDelegate, const FEOSEcomRedeemEntitlementsCallbackInfo&, Data);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		CALLBACK OBJECTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

struct FEcomQueryOwnershipBySandboxIdsCallback
{
public:
	FOnEcomQueryOwnershipBySandboxIds m_Callback;
public:
	FEcomQueryOwnershipBySandboxIdsCallback(const FOnEcomQueryOwnershipBySandboxIds& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FEcomQueryOwnershipBySandboxIdsCallback()
	{
		m_Callback.Unbind();
	}
};

struct FEcomQueryOwnershipCallback
{
public:
	FOnEcomQueryOwnershipCallback m_Callback;
public:
	FEcomQueryOwnershipCallback(const FOnEcomQueryOwnershipCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FEcomQueryOwnershipCallback()
	{
		m_Callback.Unbind();
	}
};

struct FEcomQueryOwnershipTokenCallback
{
public:
	FOnEcomQueryOwnershipTokenCallback m_Callback;
public:
	FEcomQueryOwnershipTokenCallback(const FOnEcomQueryOwnershipTokenCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FEcomQueryOwnershipTokenCallback()
	{
		m_Callback.Unbind();
	}
};

struct FEcomQueryEntitlementsCallback
{
public:
	FOnEcomQueryEntitlementsCallback m_Callback;
public:
	FEcomQueryEntitlementsCallback(const FOnEcomQueryEntitlementsCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FEcomQueryEntitlementsCallback()
	{
		m_Callback.Unbind();
	}
};

struct FEcomQueryEntitlementTokenCallback
{
public:
	FOnEcomQueryEntitlementTokenCallback m_Callback;
public:
	FEcomQueryEntitlementTokenCallback(const FOnEcomQueryEntitlementTokenCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FEcomQueryEntitlementTokenCallback()
	{
		m_Callback.Unbind();
	}
};

struct FEcomQueryOffersCallback
{
public:
	FOnEcomQueryQueryOffersCallback m_Callback;
public:
	FEcomQueryOffersCallback(const FOnEcomQueryQueryOffersCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FEcomQueryOffersCallback()
	{
		m_Callback.Unbind();
	}
};

struct FEcomCheckoutCallback
{
public:
	FOnEcomQueryCheckoutCallback m_Callback;
public:
	FEcomCheckoutCallback(const FOnEcomQueryCheckoutCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FEcomCheckoutCallback()
	{
		m_Callback.Unbind();
	}
};

struct FEcomRedeemEntitlementsCallback
{
public:
	FOnEcomQueryRedeemEntitlementsCallback m_Callback;
public:
	FEcomRedeemEntitlementsCallback(const FOnEcomQueryRedeemEntitlementsCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FEcomRedeemEntitlementsCallback()
	{
		m_Callback.Unbind();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		REQUEST OBJECTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

struct FQueryOwnershipBySandboxIdsOptions : public EOS_Ecom_QueryOwnershipBySandboxIdsOptions
{
	TArray<char*> PointerArray;

	FQueryOwnershipBySandboxIdsOptions(int32 NumCatalogItemIds)
		: EOS_Ecom_QueryOwnershipBySandboxIdsOptions()
	{
		ApiVersion = EOS_ECOM_QUERYOWNERSHIPBYSANDBOXIDSOPTIONS_API_LATEST;
		PointerArray.AddZeroed(NumCatalogItemIds);

		for (int32 i = 0; i < NumCatalogItemIds; i++)
		{
			PointerArray[i] = new char[256];
		}

		SandboxIds = const_cast<EOS_Ecom_SandboxId*>(PointerArray.GetData());
		SandboxIdsCount = PointerArray.Num();
	}

	~FQueryOwnershipBySandboxIdsOptions()
	{
		for (uint32_t i = 0; i < SandboxIdsCount; i++)
		{
			delete[] SandboxIds[i];
		}
	}
};

struct FQueryOwnershipOptions : public EOS_Ecom_QueryOwnershipOptions
{
	TArray<char*> PointerArray;

	FQueryOwnershipOptions(FString catalogNamespace, int32 NumCatalogItemIds)
		: EOS_Ecom_QueryOwnershipOptions()
	{
		ApiVersion = EOS_ECOM_QUERYOWNERSHIP_API_LATEST;

		if (!catalogNamespace.IsEmpty())
		{
			CatalogNamespace = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(CatalogNamespace), TCHAR_TO_UTF8(*catalogNamespace), 256);
		}

		PointerArray.AddZeroed(NumCatalogItemIds);

		for (int32 i = 0; i < NumCatalogItemIds; i++)
		{
			PointerArray[i] = new char[256];
		}

		CatalogItemIds = const_cast<EOS_Ecom_CatalogItemId*>(PointerArray.GetData());
		CatalogItemIdCount = PointerArray.Num();
	}

	~FQueryOwnershipOptions()
	{
		delete[] CatalogNamespace;

		for (uint32_t i = 0; i < CatalogItemIdCount; i++)
		{
			delete[] CatalogItemIds[i];
		}
	}
};

struct FQueryOwnershipTokenOptions : public EOS_Ecom_QueryOwnershipTokenOptions
{
	TArray<char*> PointerArray;

	FQueryOwnershipTokenOptions(FString catalogNamespace, int32 NumCatalogItemIds)
		: EOS_Ecom_QueryOwnershipTokenOptions()
	{
		ApiVersion = EOS_ECOM_QUERYOWNERSHIPTOKEN_API_LATEST;

		if (!catalogNamespace.IsEmpty())
		{
			CatalogNamespace = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(CatalogNamespace), TCHAR_TO_UTF8(*catalogNamespace), 256);
		}

		PointerArray.AddZeroed(NumCatalogItemIds);

		for (int32 i = 0; i < NumCatalogItemIds; i++)
		{
			PointerArray[i] = new char[256];
		}

		CatalogItemIds = const_cast<EOS_Ecom_CatalogItemId*>(PointerArray.GetData());
		CatalogItemIdCount = PointerArray.Num();
	}

	~FQueryOwnershipTokenOptions()
	{
		delete[] CatalogNamespace;

		for (uint32_t i = 0; i < CatalogItemIdCount; i++)
		{
			delete[] CatalogItemIds[i];
		}
	}
};

struct FQueryOffersOptions : public EOS_Ecom_QueryOffersOptions
{
	FQueryOffersOptions(FString overrideCatalogNamespace)
		: EOS_Ecom_QueryOffersOptions()
	{
		ApiVersion = EOS_ECOM_QUERYOFFERS_API_LATEST;

		if (!overrideCatalogNamespace.IsEmpty())
		{
			OverrideCatalogNamespace = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(OverrideCatalogNamespace), TCHAR_TO_UTF8(*overrideCatalogNamespace), 256);
		}
	}

	~FQueryOffersOptions()
	{
		delete[] OverrideCatalogNamespace;
	}
};

struct FCheckoutOptions : public EOS_Ecom_CheckoutOptions
{
	TArray<EOS_Ecom_CheckoutEntry> PointerArray;

	FCheckoutOptions(FString overrideCatalogNamespace, int32 NumCatalogItemIds)
		: EOS_Ecom_CheckoutOptions()
	{
		ApiVersion = EOS_ECOM_CHECKOUT_API_LATEST;

		if (!overrideCatalogNamespace.IsEmpty())
		{
			OverrideCatalogNamespace = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(OverrideCatalogNamespace), TCHAR_TO_UTF8(*overrideCatalogNamespace), 256);
		}

		PointerArray.AddZeroed(NumCatalogItemIds);

		for (int32 i = 0; i < NumCatalogItemIds; i++)
		{
			PointerArray[i].ApiVersion = EOS_ECOM_CHECKOUTENTRY_API_LATEST;
			PointerArray[i].OfferId = new char[256];
		}

		Entries = PointerArray.GetData();
		EntryCount = PointerArray.Num();
	}

	~FCheckoutOptions()
	{
		delete[] OverrideCatalogNamespace;

		for (uint32_t i = 0; i < EntryCount; i++)
		{
			delete[] Entries[i].OfferId;
		}
	}
};

struct FRedeemEntitlementsOptions : public EOS_Ecom_RedeemEntitlementsOptions
{
	TArray<char*> PointerArray;

	FRedeemEntitlementsOptions(int32 NumCatalogItemIds)
		: EOS_Ecom_RedeemEntitlementsOptions()
	{
		ApiVersion = EOS_ECOM_REDEEMENTITLEMENTS_API_LATEST;

		PointerArray.AddZeroed(NumCatalogItemIds);
		for (int32 i = 0; i < NumCatalogItemIds; i++)
		{
			EntitlementIds[i] = new char[256];
		}

		EntitlementIds = (EOS_Ecom_EntitlementId*)PointerArray.GetData();
		EntitlementIdCount = PointerArray.Num();
	}

	~FRedeemEntitlementsOptions()
	{
		for (uint32_t i = 0; i < EntitlementIdCount; i++)
		{
			delete[] EntitlementIds[i];
		}
	}
};

struct FGetEntitlementsByNameCountOptions : public EOS_Ecom_GetEntitlementsByNameCountOptions
{
	FGetEntitlementsByNameCountOptions(FString entitlementName)
		: EOS_Ecom_GetEntitlementsByNameCountOptions()
	{
		ApiVersion = EOS_ECOM_GETENTITLEMENTSBYNAMECOUNT_API_LATEST;

		if (!entitlementName.IsEmpty())
		{
			EntitlementName = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(EntitlementName), TCHAR_TO_UTF8(*entitlementName), 256);
		}
	}

	~FGetEntitlementsByNameCountOptions()
	{
		delete[] EntitlementName;
	}
};

struct FCopyEntitlementByNameAndIndexOptions : public EOS_Ecom_CopyEntitlementByNameAndIndexOptions
{
	FCopyEntitlementByNameAndIndexOptions(FString entitlementName)
		: EOS_Ecom_CopyEntitlementByNameAndIndexOptions()
	{
		ApiVersion = EOS_ECOM_GETENTITLEMENTSBYNAMECOUNT_API_LATEST;

		if (!entitlementName.IsEmpty())
		{
			EntitlementName = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(EntitlementName), TCHAR_TO_UTF8(*entitlementName), 256);
		}
	}

	~FCopyEntitlementByNameAndIndexOptions()
	{
		delete[] EntitlementName;
	}
};

struct FCopyEntitlementByIdOptions : public EOS_Ecom_CopyEntitlementByIdOptions
{
	FCopyEntitlementByIdOptions(FString entitlementId)
		: EOS_Ecom_CopyEntitlementByIdOptions()
	{
		ApiVersion = EOS_ECOM_COPYENTITLEMENTBYID_API_LATEST;

		if (!entitlementId.IsEmpty())
		{
			EntitlementId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(EntitlementId), TCHAR_TO_UTF8(*entitlementId), 256);
		}
	}

	~FCopyEntitlementByIdOptions()
	{
		delete[] EntitlementId;
	}
};

struct FCopyOfferByIdOptions : public EOS_Ecom_CopyOfferByIdOptions
{
	FCopyOfferByIdOptions(FString offerId)
		: EOS_Ecom_CopyOfferByIdOptions()
	{
		ApiVersion = EOS_ECOM_COPYOFFERBYID_API_LATEST;

		if (!offerId.IsEmpty())
		{
			OfferId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(OfferId), TCHAR_TO_UTF8(*offerId), 256);
		}
	}

	~FCopyOfferByIdOptions()
	{
		delete[] OfferId;
	}
};

struct FGetOfferItemCountOptions : public EOS_Ecom_GetOfferItemCountOptions
{
	FGetOfferItemCountOptions(FString offerId)
		: EOS_Ecom_GetOfferItemCountOptions()
	{
		ApiVersion = EOS_ECOM_COPYOFFERBYINDEX_API_LATEST;

		if (!offerId.IsEmpty())
		{
			OfferId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(OfferId), TCHAR_TO_UTF8(*offerId), 256);
		}
	}

	~FGetOfferItemCountOptions()
	{
		delete[] OfferId;
	}
};

struct FCopyOfferItemByIndexOptions : public EOS_Ecom_CopyOfferItemByIndexOptions
{
	FCopyOfferItemByIndexOptions(FString offerId)
		: EOS_Ecom_CopyOfferItemByIndexOptions()
	{
		ApiVersion = EOS_ECOM_COPYOFFERITEMBYINDEX_API_LATEST;

		if (!offerId.IsEmpty())
		{
			OfferId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(OfferId), TCHAR_TO_UTF8(*offerId), 256);
		}
	}

	~FCopyOfferItemByIndexOptions()
	{
		delete[] OfferId;
	}
};

struct FCopyItemByIdOptions : public EOS_Ecom_CopyItemByIdOptions
{
	FCopyItemByIdOptions(FString itemId)
		: EOS_Ecom_CopyItemByIdOptions()
	{
		ApiVersion = EOS_ECOM_COPYITEMBYID_API_LATEST;

		if (!itemId.IsEmpty())
		{
			ItemId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(ItemId), TCHAR_TO_UTF8(*itemId), 256);
		}
	}

	~FCopyItemByIdOptions()
	{
		delete[] ItemId;
	}
};


struct FGetOfferImageInfoCountOptions : public EOS_Ecom_GetOfferImageInfoCountOptions
{
	FGetOfferImageInfoCountOptions(FString offerId)
		: EOS_Ecom_GetOfferImageInfoCountOptions()
	{
		ApiVersion = EOS_ECOM_GETOFFERIMAGEINFOCOUNT_API_LATEST;

		if (!offerId.IsEmpty())
		{
			OfferId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(OfferId), TCHAR_TO_UTF8(*offerId), 256);
		}
	}

	~FGetOfferImageInfoCountOptions()
	{
		delete[] OfferId;
	}
};

struct FCopyOfferImageInfoByIndexOptions : public EOS_Ecom_CopyOfferImageInfoByIndexOptions
{
	FCopyOfferImageInfoByIndexOptions(FString offerId)
		: EOS_Ecom_CopyOfferImageInfoByIndexOptions()
	{
		ApiVersion = EOS_ECOM_COPYOFFERIMAGEINFOBYINDEX_API_LATEST;

		if (!offerId.IsEmpty())
		{
			OfferId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(OfferId), TCHAR_TO_UTF8(*offerId), 256);
		}
	}

	~FCopyOfferImageInfoByIndexOptions()
	{
		delete[] OfferId;
	}
};

struct FGetItemImageInfoCountOptions : public EOS_Ecom_GetItemImageInfoCountOptions
{
	FGetItemImageInfoCountOptions(FString itemId)
		: EOS_Ecom_GetItemImageInfoCountOptions()
	{
		ApiVersion = EOS_ECOM_GETITEMIMAGEINFOCOUNT_API_LATEST;

		if (!itemId.IsEmpty())
		{
			ItemId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(ItemId), TCHAR_TO_UTF8(*itemId), 256);
		}
	}

	~FGetItemImageInfoCountOptions()
	{
		delete[] ItemId;
	}
};

struct FCopyItemImageInfoByIndexOptions : public EOS_Ecom_CopyItemImageInfoByIndexOptions
{
	FCopyItemImageInfoByIndexOptions(FString itemId)
		: EOS_Ecom_CopyItemImageInfoByIndexOptions()
	{
		ApiVersion = EOS_ECOM_COPYITEMIMAGEINFOBYINDEX_API_LATEST;

		if (!itemId.IsEmpty())
		{
			ItemId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(ItemId), TCHAR_TO_UTF8(*itemId), 256);
		}
	}

	~FCopyItemImageInfoByIndexOptions()
	{
		delete[] ItemId;
	}
};

struct FGetItemReleaseCountOptions : public EOS_Ecom_GetItemReleaseCountOptions
{
	FGetItemReleaseCountOptions(FString itemId)
		: EOS_Ecom_GetItemReleaseCountOptions()
	{
		ApiVersion = EOS_ECOM_GETITEMRELEASECOUNT_API_LATEST;

		if (!itemId.IsEmpty())
		{
			ItemId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(ItemId), TCHAR_TO_UTF8(*itemId), 256);
		}
	}

	~FGetItemReleaseCountOptions()
	{
		delete[] ItemId;
	}
};

struct FCopyItemReleaseByIndexOptions : public EOS_Ecom_CopyItemReleaseByIndexOptions
{
	FCopyItemReleaseByIndexOptions(FString itemId)
		: EOS_Ecom_CopyItemReleaseByIndexOptions()
	{
		ApiVersion = EOS_ECOM_COPYITEMRELEASEBYINDEX_API_LATEST;

		if (!itemId.IsEmpty())
		{
			ItemId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(ItemId), TCHAR_TO_UTF8(*itemId), 256);
		}
	}

	~FCopyItemReleaseByIndexOptions()
	{
		delete[] ItemId;
	}
};

struct FCopyTransactionByIdOptions : public EOS_Ecom_CopyTransactionByIdOptions
{
	FCopyTransactionByIdOptions(FString transactionId)
		: EOS_Ecom_CopyTransactionByIdOptions()
	{
		ApiVersion = EOS_ECOM_COPYTRANSACTIONBYID_API_LATEST;

		if (!transactionId.IsEmpty())
		{
			TransactionId = new char[256];
			FCStringAnsi::Strncpy(const_cast<char*>(TransactionId), TCHAR_TO_UTF8(*transactionId), 256);
		}
	}

	~FCopyTransactionByIdOptions()
	{
		delete[] TransactionId;
	}
};

struct FQueryEntitlementsOptions : public EOS_Ecom_QueryEntitlementsOptions
{
	TArray<char*> PointerArray;
	
	FQueryEntitlementsOptions(int32 NumCatalogItemIds)
		: EOS_Ecom_QueryEntitlementsOptions()
	{
		ApiVersion = EOS_ECOM_QUERYENTITLEMENTS_API_LATEST;

		PointerArray.AddZeroed(NumCatalogItemIds);

		for (int32 i=0; i<NumCatalogItemIds; i++)
		{
			PointerArray[i] = new char[256];
		}
		
		EntitlementNames = const_cast<EOS_Ecom_EntitlementName*>(PointerArray.GetData());
		EntitlementNameCount = PointerArray.Num();

	}
	~FQueryEntitlementsOptions()
	{
		for (uint32_t i=0; i<EntitlementNameCount; i++)
		{
			delete[] EntitlementNames[i];
		}
	}
};

struct FQueryEntitlementTokenOptions : public EOS_Ecom_QueryEntitlementTokenOptions
{
	TArray<char*> PointerArray;
	
	FQueryEntitlementTokenOptions(int32 EntitlementNameCount)
		: EOS_Ecom_QueryEntitlementTokenOptions()
	{
		ApiVersion = EOS_ECOM_QUERYENTITLEMENTTOKEN_API_LATEST;

		PointerArray.AddZeroed(EntitlementNameCount);

		for (int32 i=0; i<EntitlementNameCount; i++)
		{
			PointerArray[i] = new char[256];
		}
		
		EntitlementNames = const_cast<EOS_Ecom_EntitlementName*>(PointerArray.GetData());
		EntitlementNameCount = PointerArray.Num();

	}
	~FQueryEntitlementTokenOptions()
	{
		for (uint32_t i=0; i<EntitlementNameCount; i++)
		{
			delete[] EntitlementNames[i];
		}
	}
};