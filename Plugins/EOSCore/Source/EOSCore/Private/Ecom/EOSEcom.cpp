/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "Ecom/EOSEcom.h"

#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLogging.h"

UCoreEcom* UCoreEcom::GetEcom(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (GetEcomHandle(WorldContextObject))
		{
			if (UWorld* World = WorldContextObject->GetWorld())
			{
				if (UGameInstance* GameInstance = World->GetGameInstance())
				{
					return GameInstance->GetSubsystem<UCoreEcom>();
				}
			}
		}
	}
	return nullptr;
}

void UCoreEcom::EOSEcomQueryOwnership(UObject* WorldContextObject, FEOSEcomQueryOwnershipOptions Options, const FOnEcomQueryOwnershipCallback& Callback)
{
	if (GetEcomHandle(WorldContextObject))
	{
		FQueryOwnershipOptions EcomOptions(Options.CatalogNamespace, Options.CatalogItemIds.Num());
		EcomOptions.LocalUserId = Options.LocalUserId;

		for (int32 i = 0; i < Options.CatalogItemIds.Num(); i++)
		{
			FCStringAnsi::Strncpy(EcomOptions.PointerArray[i], TCHAR_TO_UTF8(*Options.CatalogItemIds[i]), 256);
		}

		FEcomQueryOwnershipCallback* CallbackObj = new FEcomQueryOwnershipCallback(Callback);
		EOS_Ecom_QueryOwnership(GetEcomHandle(WorldContextObject), &EcomOptions, CallbackObj, Internal_OnQueryOwnershipCallback);
	}
}

void UCoreEcom::EOSEcomQueryOwnershipBySandboxIds(UObject* WorldContextObject, FEOSEcomQueryOwnershipBySandboxIdsOptions Options, const FOnEcomQueryOwnershipBySandboxIds Callback)
{
	if (GetEcomHandle(WorldContextObject))
	{
		FQueryOwnershipBySandboxIdsOptions EcomOptions(Options.SandboxIds.Num());
		EcomOptions.LocalUserId = Options.LocalUserId;

		for (int32 i = 0; i < Options.SandboxIds.Num(); i++)
		{
			FCStringAnsi::Strncpy(EcomOptions.PointerArray[i], TCHAR_TO_UTF8(*Options.SandboxIds[i].CatalogItemId), 256);
		}

		FEcomQueryOwnershipBySandboxIdsCallback* CallbackObj = new FEcomQueryOwnershipBySandboxIdsCallback(Callback);
		EOS_Ecom_QueryOwnershipBySandboxIds(GetEcomHandle(WorldContextObject), &EcomOptions, CallbackObj, Internal_OnQueryOwnershipBySandboxIdsCallback);
	}
}

void UCoreEcom::EOSEcomQueryOwnershipToken(UObject* WorldContextObject, FEOSEcomQueryOwnershipTokenOptions Options, const FOnEcomQueryOwnershipTokenCallback& Callback)
{
	if (GetEcomHandle(WorldContextObject))
	{
		FQueryOwnershipTokenOptions EcomOptions(Options.CatalogNamespace, Options.CatalogItemIds.Num());
		EcomOptions.LocalUserId = Options.LocalUserId;

		for (int32 i = 0; i < Options.CatalogItemIds.Num(); i++)
		{
			FCStringAnsi::Strncpy(EcomOptions.PointerArray[i], TCHAR_TO_UTF8(*Options.CatalogItemIds[i]), 256);
		}

		FEcomQueryOwnershipTokenCallback* CallbackObj = new FEcomQueryOwnershipTokenCallback(Callback);
		EOS_Ecom_QueryOwnershipToken(GetEcomHandle(WorldContextObject), &EcomOptions, CallbackObj, Internal_OnQueryOwnershipTokenCallback);
	}
}

void UCoreEcom::EOSEcomQueryEntitlements(UObject* WorldContextObject, FEOSEcomQueryEntitlementsOptions Options, const FOnEcomQueryEntitlementsCallback& Callback)
{
	if (GetEcomHandle(WorldContextObject))
	{
		FQueryEntitlementsOptions EcomOptions(Options.EntitlementNames.Num());
		EcomOptions.LocalUserId = Options.LocalUserId;
		EcomOptions.bIncludeRedeemed = Options.bIncludeRedeemed;

		for (int32 i = 0; i < Options.EntitlementNames.Num(); i++)
		{
			FCStringAnsi::Strncpy(EcomOptions.PointerArray[i], TCHAR_TO_UTF8(*Options.EntitlementNames[i].Name), 256);
		}

		FEcomQueryEntitlementsCallback* CallbackObj = new FEcomQueryEntitlementsCallback(Callback);
		EOS_Ecom_QueryEntitlements(GetEcomHandle(WorldContextObject), &EcomOptions, CallbackObj, Internal_OnQueryEntitlementsCallback);
	}
}

void UCoreEcom::EOSEcomQueryEntitlementToken(UObject* WorldContextObject, FEOSEcomQueryEntitlementTokenOptions Options, const FOnEcomQueryEntitlementTokenCallback& Callback)
{
	if (GetEcomHandle(WorldContextObject))
	{
		FQueryEntitlementTokenOptions EcomOptions(Options.EntitlementNames.Num());
		EcomOptions.LocalUserId = Options.LocalUserId;
		
		for (int32 i = 0; i < Options.EntitlementNames.Num(); i++)
		{
			FCStringAnsi::Strncpy(EcomOptions.PointerArray[i], TCHAR_TO_UTF8(*Options.EntitlementNames[i].Name), 256);
		}

		FEcomQueryEntitlementTokenCallback* CallbackObj = new FEcomQueryEntitlementTokenCallback(Callback);
		EOS_Ecom_QueryEntitlementToken(GetEcomHandle(WorldContextObject), &EcomOptions, CallbackObj, Internal_OnQueryEntitlementTokenOptions);
	}
}

void UCoreEcom::EOSEcomQueryOffers(UObject* WorldContextObject, FEOSEcomQueryOffersOptions Options, const FOnEcomQueryQueryOffersCallback& Callback)
{
	if (GetEcomHandle(WorldContextObject))
	{
		FQueryOffersOptions EcomOptions(Options.OverrideCatalogNamespace);
		EcomOptions.LocalUserId = Options.LocalUserId;
		
		FEcomQueryOffersCallback* CallbackObj = new FEcomQueryOffersCallback(Callback);
		EOS_Ecom_QueryOffers(GetEcomHandle(WorldContextObject), &EcomOptions, CallbackObj, Internal_OnQueryOffersCallback);
	}
}

void UCoreEcom::EOSEcomCheckout(UObject* WorldContextObject, FEOSEcomCheckoutOptions Options, const FOnEcomQueryCheckoutCallback& Callback)
{
	if (GetEcomHandle(WorldContextObject))
	{
		FCheckoutOptions EcomOptions(Options.OverrideCatalogNamespace, Options.Entries.Num());
		EcomOptions.LocalUserId = Options.LocalUserId;

		for (int32 i = 0; i < Options.Entries.Num(); i++)
		{
			FCStringAnsi::Strncpy(const_cast<char*>(EcomOptions.Entries[i].OfferId), TCHAR_TO_UTF8(*Options.Entries[i].OfferId), 256);
		}

		FEcomCheckoutCallback* CallbackObj = new FEcomCheckoutCallback(Callback);
		EOS_Ecom_Checkout(GetEcomHandle(WorldContextObject), &EcomOptions, CallbackObj, Internal_OnCheckoutCallback);
	}
}

void UCoreEcom::EOSEcomRedeemEntitlements(UObject* WorldContextObject, FEOSEcomRedeemEntitlementsOptions Options, const FOnEcomQueryRedeemEntitlementsCallback& Callback)
{
	if (GetEcomHandle(WorldContextObject))
	{
		FRedeemEntitlementsOptions EcomOptions(Options.EntitlementIds.Num());
		EcomOptions.LocalUserId = Options.LocalUserId;

		for (int32 i = 0; i < Options.EntitlementIds.Num(); i++)
		{
			FCStringAnsi::Strncpy(EcomOptions.PointerArray[i], TCHAR_TO_UTF8(*Options.EntitlementIds[i].EntitlementId), 256);
		}

		FEcomRedeemEntitlementsCallback* CallbackObj = new FEcomRedeemEntitlementsCallback(Callback);
		EOS_Ecom_RedeemEntitlements(GetEcomHandle(WorldContextObject), &EcomOptions, CallbackObj, Internal_OnRedeemEntitlementsCallback);
	}
}

int32 UCoreEcom::EOSEcomGetLastRedeemedEntitlementsCount(UObject* WorldContextObject, FEOSEcomGetLastRedeemedEntitlementsCountOptions Options)
{
	int32 Result = 0;

	if (GetEcomHandle(WorldContextObject))
	{
		EOS_Ecom_GetLastRedeemedEntitlementsCountOptions EcomOptions = {};
		EcomOptions.ApiVersion = EOS_ECOM_GETLASTREDEEMEDENTITLEMENTSCOUNT_API_LATEST;
		EcomOptions.LocalUserId = Options.LocalUserId;

		Result = EOS_Ecom_GetLastRedeemedEntitlementsCount(GetEcomHandle(WorldContextObject), &EcomOptions);
	}

	return Result;
}

EOSResult UCoreEcom::EOSEcomCopyLastRedeemedEntitlementByIndex(UObject* WorldContextObject, FEOSEcomCopyLastRedeemedEntitlementByIndexOptions Options, FString& OutRedeemedEntitlementId)
{
	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetEcomHandle(WorldContextObject))
	{
		EOS_Ecom_CopyLastRedeemedEntitlementByIndexOptions EcomOptions = {};
		EcomOptions.ApiVersion = EOS_ECOM_COPYLASTREDEEMEDENTITLEMENTBYINDEX_API_LATEST;
		EcomOptions.LocalUserId = Options.LocalUserId;
		EcomOptions.RedeemedEntitlementIndex = Options.RedeemedEntitlementIndex;

		TArray<char> CharArray;
		CharArray.AddUninitialized(1024);

		int32_t InOutRedeemedEntitlementIdLength = 1024;
		
		Result = EOSHelpers::ToEOSCoreResult(EOS_Ecom_CopyLastRedeemedEntitlementByIndex(GetEcomHandle(WorldContextObject), &EcomOptions, CharArray.GetData(), &InOutRedeemedEntitlementIdLength));

		if (Result == EOSResult::EOS_Success)
		{
			CharArray.SetNum(InOutRedeemedEntitlementIdLength);
			OutRedeemedEntitlementId = UTF8_TO_TCHAR(CharArray.GetData());
		}
	}

	return Result;
}

int32 UCoreEcom::EOSEcomGetEntitlementsCount(UObject* WorldContextObject, FEOSEcomGetEntitlementsCountOptions Options)
{
	int32 Result = 0;

	if (GetEcomHandle(WorldContextObject))
	{
		EOS_Ecom_GetEntitlementsCountOptions EcomOptions = { };
		EcomOptions.ApiVersion = Options.ApiVersion;
		EcomOptions.LocalUserId = Options.LocalUserId;

		Result = EOS_Ecom_GetEntitlementsCount(GetEcomHandle(WorldContextObject), &EcomOptions);
	}

	return Result;
}

int32 UCoreEcom::EOSEcomGetEntitlementsByNameCount(UObject* WorldContextObject, FEOSEcomGetEntitlementsByNameCountOptions Options)
{
	int32 Result = 0;

	if (GetEcomHandle(WorldContextObject))
	{
		FGetEntitlementsByNameCountOptions EcomOptions(Options.EntitlementName.Name);
		EcomOptions.LocalUserId = Options.LocalUserId;

		Result = EOS_Ecom_GetEntitlementsByNameCount(GetEcomHandle(WorldContextObject), &EcomOptions);
	}

	return Result;
}

EOSResult UCoreEcom::EOSEcomCopyEntitlementByIndex(UObject* WorldContextObject, FEOSEcomCopyEntitlementByIndexOptions Options, FEOSEcomEntitlement& OutEntitlement)
{
	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetEcomHandle(WorldContextObject))
	{
		EOS_Ecom_CopyEntitlementByIndexOptions EcomOptions = { };
		EcomOptions.ApiVersion = Options.ApiVersion;
		EcomOptions.LocalUserId = Options.LocalUserId;
		EcomOptions.EntitlementIndex = static_cast<uint32_t>(Options.EntitlementIndex);

		EOS_Ecom_Entitlement* Entitlement = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Ecom_CopyEntitlementByIndex(GetEcomHandle(WorldContextObject), &EcomOptions, &Entitlement));

		if (Result == EOSResult::EOS_Success)
		{
			OutEntitlement = *Entitlement;
			EOS_Ecom_Entitlement_Release(Entitlement);
		}
	}

	return Result;
}

EOSResult UCoreEcom::EOSEcomCopyEntitlementByNameAndIndex(UObject* WorldContextObject, FEOSEcomCopyEntitlementByNameAndIndexOptions Options, FEOSEcomEntitlement& OutEntitlement)
{
	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetEcomHandle(WorldContextObject))
	{
		FCopyEntitlementByNameAndIndexOptions EcomOptions(Options.EntitlementName.Name);
		EcomOptions.LocalUserId = Options.LocalUserId;
		
		EcomOptions.Index = static_cast<uint32_t>(Options.Index);

		EOS_Ecom_Entitlement* Entitlement = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Ecom_CopyEntitlementByNameAndIndex(GetEcomHandle(WorldContextObject), &EcomOptions, &Entitlement));

		if (Result == EOSResult::EOS_Success)
		{
			OutEntitlement = *Entitlement;
		}
	}

	return Result;
}

EOSResult UCoreEcom::EOSEcomCopyEntitlementById(UObject* WorldContextObject, FEOSEcomCopyEntitlementByIdOptions Options, FEOSEcomEntitlement& OutEntitlement)
{
	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetEcomHandle(WorldContextObject))
	{
		FCopyEntitlementByIdOptions EcomOptions(Options.EntitlementId.EntitlementId);
		EcomOptions.LocalUserId = Options.LocalUserId;

		EOS_Ecom_Entitlement* Entitlement = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Ecom_CopyEntitlementById(GetEcomHandle(WorldContextObject), &EcomOptions, &Entitlement));

		if (Result == EOSResult::EOS_Success)
		{
			OutEntitlement = *Entitlement;
		}
	}

	return Result;
}

int32 UCoreEcom::EOSEcomGetOfferCount(UObject* WorldContextObject, FEOSEcomGetOfferCountOptions Options)
{
	int32 Result = 0;

	if (GetEcomHandle(WorldContextObject))
	{
		EOS_Ecom_GetOfferCountOptions EcomOptions = { };
		EcomOptions.ApiVersion = Options.ApiVersion;
		EcomOptions.LocalUserId = Options.LocalUserId;

		Result = EOS_Ecom_GetOfferCount(GetEcomHandle(WorldContextObject), &EcomOptions);
	}

	return Result;
}

EOSResult UCoreEcom::EOSEcomCopyOfferByIndex(UObject* WorldContextObject, FEOSEcomCopyOfferByIndexOptions Options, FEOSEcomCatalogOffer& OutOffer)
{
	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetEcomHandle(WorldContextObject))
	{
		EOS_Ecom_CopyOfferByIndexOptions EcomOptions = { };
		EcomOptions.ApiVersion = Options.ApiVersion;
		EcomOptions.LocalUserId = Options.LocalUserId;
		EcomOptions.OfferIndex = static_cast<uint32_t>(Options.OfferIndex);

		EOS_Ecom_CatalogOffer* CatalogOffer = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Ecom_CopyOfferByIndex(GetEcomHandle(WorldContextObject), &EcomOptions, &CatalogOffer));

		if (Result == EOSResult::EOS_Success)
		{
			OutOffer = *CatalogOffer;
			EOS_Ecom_CatalogOffer_Release(CatalogOffer);
		}
	}

	return Result;
}

EOSResult UCoreEcom::EOSEcomCopyOfferById(UObject* WorldContextObject, FEOSEcomCopyOfferByIdOptions Options, FEOSEcomCatalogOffer& OutOffer)
{
	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetEcomHandle(WorldContextObject))
	{
		FCopyOfferByIdOptions EcomOptions(Options.OfferId);
		EcomOptions.LocalUserId = Options.LocalUserId;

		EOS_Ecom_CatalogOffer* CatalogOffer = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Ecom_CopyOfferById(GetEcomHandle(WorldContextObject), &EcomOptions, &CatalogOffer));

		if (Result == EOSResult::EOS_Success)
		{
			OutOffer = *CatalogOffer;
		}
	}

	return Result;
}

int32 UCoreEcom::EOSEcomGetOfferItemCount(UObject* WorldContextObject, FEOSEcomGetOfferItemCountOptions Options)
{
	int32 Result = 0;

	if (GetEcomHandle(WorldContextObject))
	{
		FGetOfferItemCountOptions EcomOptions(Options.OfferId);
		EcomOptions.LocalUserId = Options.LocalUserId;

		Result = EOS_Ecom_GetOfferItemCount(GetEcomHandle(WorldContextObject), &EcomOptions);
	}

	return Result;
}

EOSResult UCoreEcom::EOSEcomCopyOfferItemByIndex(UObject* WorldContextObject, FEOSEcomCopyOfferItemByIndexOptions Options, FEOSEcomCatalogItem& OutItem)
{
	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetEcomHandle(WorldContextObject))
	{
		FCopyOfferItemByIndexOptions EcomOptions(Options.OfferId);
		EcomOptions.LocalUserId = Options.LocalUserId;

		EcomOptions.ItemIndex = static_cast<uint32_t>(Options.ItemIndex);
		EOS_Ecom_CatalogItem* CatalogItem = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Ecom_CopyOfferItemByIndex(GetEcomHandle(WorldContextObject), &EcomOptions, &CatalogItem));

		if (Result == EOSResult::EOS_Success)
		{
			OutItem = *CatalogItem;
			EOS_Ecom_CatalogItem_Release(CatalogItem);
		}
	}

	return Result;
}

EOSResult UCoreEcom::EOSEcomCopyItemById(UObject* WorldContextObject, FEOSEcomCopyItemByIdOptions Options, FEOSEcomCatalogItem& OutItem)
{
	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetEcomHandle(WorldContextObject))
	{
		FCopyItemByIdOptions EcomOptions(Options.ItemId);
		EcomOptions.LocalUserId = Options.LocalUserId;

		EOS_Ecom_CatalogItem* CatalogItem = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Ecom_CopyItemById(GetEcomHandle(WorldContextObject), &EcomOptions, &CatalogItem));

		if (Result == EOSResult::EOS_Success)
		{
			OutItem = *CatalogItem;
			EOS_Ecom_CatalogItem_Release(CatalogItem);
		}
	}

	return Result;
}

int32 UCoreEcom::EOSEcomGetOfferImageInfoCount(UObject* WorldContextObject, FEOSEcomGetOfferImageInfoCountOptions Options)
{
	int32 Result = 0;

	if (GetEcomHandle(WorldContextObject))
	{
		FGetOfferImageInfoCountOptions EcomOptions(Options.OfferId);
		EcomOptions.LocalUserId = Options.LocalUserId;

		Result = EOS_Ecom_GetOfferImageInfoCount(GetEcomHandle(WorldContextObject), &EcomOptions);
	}

	return Result;
}

EOSResult UCoreEcom::EOSEcomCopyOfferImageInfoByIndex(UObject* WorldContextObject, FEOSEcomCopyOfferImageInfoByIndexOptions Options, FEOSEcomKeyImageInfo& OutImageInfo)
{
	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetEcomHandle(WorldContextObject))
	{
		FCopyOfferImageInfoByIndexOptions EcomOptions(Options.OfferId);
		EcomOptions.LocalUserId = Options.LocalUserId;

		EcomOptions.ImageInfoIndex = static_cast<uint32_t>(Options.ImageInfoIndex);

		EOS_Ecom_KeyImageInfo* KeyImageInfo = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Ecom_CopyOfferImageInfoByIndex(GetEcomHandle(WorldContextObject), &EcomOptions, &KeyImageInfo));

		if (Result == EOSResult::EOS_Success)
		{
			OutImageInfo = *KeyImageInfo;
			EOS_Ecom_KeyImageInfo_Release(KeyImageInfo);
		}
	}

	return Result;
}

int32 UCoreEcom::EOSEcomGetItemImageInfoCount(UObject* WorldContextObject, FEOSEcomGetItemImageInfoCountOptions Options)
{
	int32 Result = 0;

	if (GetEcomHandle(WorldContextObject))
	{
		FGetItemImageInfoCountOptions EcomOptions(Options.ItemId);
		EcomOptions.LocalUserId = Options.LocalUserId;

		Result = EOS_Ecom_GetItemImageInfoCount(GetEcomHandle(WorldContextObject), &EcomOptions);
	}

	return Result;
}

EOSResult UCoreEcom::EOSEcomCopyItemImageInfoByIndex(UObject* WorldContextObject, FEOSEcomCopyItemImageInfoByIndexOptions Options, FEOSEcomKeyImageInfo& OutImageInfo)
{
	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetEcomHandle(WorldContextObject))
	{
		FCopyItemImageInfoByIndexOptions EcomOptions(Options.ItemId);
		EcomOptions.LocalUserId = Options.LocalUserId;
		EcomOptions.ImageInfoIndex = static_cast<uint32_t>(Options.ImageInfoIndex);

		EOS_Ecom_KeyImageInfo* KeyImageInfo = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Ecom_CopyItemImageInfoByIndex(GetEcomHandle(WorldContextObject), &EcomOptions, &KeyImageInfo));

		if (Result == EOSResult::EOS_Success)
		{
			OutImageInfo = *KeyImageInfo;
			EOS_Ecom_KeyImageInfo_Release(KeyImageInfo);
		}
	}

	return Result;
}

int32 UCoreEcom::EOSEcomGetItemReleaseCount(UObject* WorldContextObject, FEOSEcomGetItemReleaseCountOptions Options)
{
	int32 Result = 0;

	if (GetEcomHandle(WorldContextObject))
	{
		FGetItemReleaseCountOptions EcomOptions(Options.ItemId);
		EcomOptions.LocalUserId = Options.LocalUserId;

		Result = EOS_Ecom_GetItemReleaseCount(GetEcomHandle(WorldContextObject), &EcomOptions);
	}

	return Result;
}

EOSResult UCoreEcom::EOSEcomCopyItemReleaseByIndex(UObject* WorldContextObject, FEOSEcomCopyItemReleaseByIndexOptions Options, FEOSEcomCatalogRelease& OutRelease)
{
	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetEcomHandle(WorldContextObject))
	{
		FCopyItemReleaseByIndexOptions EcomOptions(Options.ItemId);
		EcomOptions.LocalUserId = Options.LocalUserId;
		
		EcomOptions.ReleaseIndex = static_cast<uint32_t>(Options.ReleaseIndex);

		EOS_Ecom_CatalogRelease* CatalogRelease = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Ecom_CopyItemReleaseByIndex(GetEcomHandle(WorldContextObject), &EcomOptions, &CatalogRelease));

		if (Result == EOSResult::EOS_Success)
		{
			OutRelease = *CatalogRelease;
			EOS_Ecom_CatalogRelease_Release(CatalogRelease);
		}
	}

	return Result;
}

int32 UCoreEcom::EOSEcomGetTransactionCount(UObject* WorldContextObject, FEOSEcomGetTransactionCountOptions Options)
{
	int32 Result = 0;

	if (GetEcomHandle(WorldContextObject))
	{
		EOS_Ecom_GetTransactionCountOptions EcomOptions = {  };
		EcomOptions.ApiVersion = Options.ApiVersion;
		EcomOptions.LocalUserId = Options.LocalUserId;

		Result = EOS_Ecom_GetTransactionCount(GetEcomHandle(WorldContextObject), &EcomOptions);
	}

	return Result;
}

EOSResult UCoreEcom::EOSEcomCopyTransactionByIndex(UObject* WorldContextObject, FEOSEcomCopyTransactionByIndexOptions Options, FEOSEcomHTransaction& OutTransaction)
{
	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetEcomHandle(WorldContextObject))
	{
		EOS_Ecom_CopyTransactionByIndexOptions EcomOptions = { };
		EcomOptions.ApiVersion = Options.ApiVersion;
		EcomOptions.LocalUserId = Options.LocalUserId;
		EcomOptions.TransactionIndex = static_cast<uint32_t>(Options.TransactionIndex);

		EOS_Ecom_HTransaction Transaction;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Ecom_CopyTransactionByIndex(GetEcomHandle(WorldContextObject), &EcomOptions, &Transaction));

		if (Result == EOSResult::EOS_Success)
		{
			OutTransaction = Transaction;
			EOS_Ecom_Transaction_Release(Transaction);
		}
	}

	return Result;
}

EOSResult UCoreEcom::EOSEcomCopyTransactionById(UObject* WorldContextObject, FEOSEcomCopyTransactionByIdOptions Options, FEOSEcomHTransaction& OutTransaction)
{
	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetEcomHandle(WorldContextObject))
	{
		FCopyTransactionByIdOptions EcomOptions(Options.TransactionId);
		EcomOptions.LocalUserId = Options.LocalUserId;

		EOS_Ecom_HTransaction Transaction;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Ecom_CopyTransactionById(GetEcomHandle(WorldContextObject), &EcomOptions, &Transaction));

		if (Result == EOSResult::EOS_Success)
		{
			OutTransaction = Transaction;
			EOS_Ecom_Transaction_Release(Transaction);
		}
	}

	return Result;
}

EOSResult UCoreEcom::EOSEcomTransaction_GetTransactionId(UObject* WorldContextObject, const FEOSEcomHTransaction& Handle, FString& TransactionId)
{
	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetEcomHandle(WorldContextObject))
	{
		char Buffer[4096];
		int32_t Size = 0;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Ecom_Transaction_GetTransactionId(Handle, Buffer, &Size));

		if (Result == EOSResult::EOS_Success)
		{
			TransactionId = Buffer;
		}
	}

	return Result;
}

int32 UCoreEcom::EOSEcomTransaction_GetEntitlementsCount(UObject* WorldContextObject, const FEOSEcomHTransaction& Handle, FEOSEcomTransactionGetEntitlementsCountOptions Options)
{
	int32 Result = 0;

	if (GetEcomHandle(WorldContextObject))
	{
		EOS_Ecom_GetEntitlementsCountOptions EcomOptions = { };
		EcomOptions.ApiVersion = Options.ApiVersion;
		EcomOptions.LocalUserId = Options.LocalUserId;

		Result = EOS_Ecom_GetEntitlementsCount(GetEcomHandle(WorldContextObject), &EcomOptions);
	}

	return Result;
}

EOSResult UCoreEcom::EOSEcomTransaction_CopyEntitlementByIndex(UObject* WorldContextObject, const FEOSEcomHTransaction& Handle, FEOSEcomTransactionCopyEntitlementByIndexOptions Options, FEOSEcomEntitlement& OutEntitlement)
{
	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetEcomHandle(WorldContextObject))
	{
		EOS_Ecom_Transaction_CopyEntitlementByIndexOptions EcomOptions = { };
		EcomOptions.ApiVersion = Options.ApiVersion;
		EcomOptions.EntitlementIndex = static_cast<uint32_t>(Options.EntitlementIndex);

		EOS_Ecom_Entitlement* Entitlement = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Ecom_Transaction_CopyEntitlementByIndex(Handle, &EcomOptions, &Entitlement));

		if (Result == EOSResult::EOS_Success)
		{
			OutEntitlement = *Entitlement;
			EOS_Ecom_Entitlement_Release(Entitlement);
		}
	}

	return Result;
}


void UCoreEcom::Internal_OnQueryOwnershipCallback(const EOS_Ecom_QueryOwnershipCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FEcomQueryOwnershipCallback* CallbackObj = static_cast<FEcomQueryOwnershipCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreEcom::Internal_OnQueryOwnershipBySandboxIdsCallback(const EOS_Ecom_QueryOwnershipBySandboxIdsCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FEcomQueryOwnershipBySandboxIdsCallback* CallbackObj = static_cast<FEcomQueryOwnershipBySandboxIdsCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreEcom::Internal_OnQueryOwnershipTokenCallback(const EOS_Ecom_QueryOwnershipTokenCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FEcomQueryOwnershipTokenCallback* CallbackObj = static_cast<FEcomQueryOwnershipTokenCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreEcom::Internal_OnQueryEntitlementsCallback(const EOS_Ecom_QueryEntitlementsCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FEcomQueryEntitlementsCallback* CallbackObj = static_cast<FEcomQueryEntitlementsCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreEcom::Internal_OnQueryEntitlementTokenOptions(const EOS_Ecom_QueryEntitlementTokenCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FEcomQueryEntitlementTokenCallback* CallbackObj = static_cast<FEcomQueryEntitlementTokenCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreEcom::Internal_OnQueryOffersCallback(const EOS_Ecom_QueryOffersCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FEcomQueryOffersCallback* CallbackObj = static_cast<FEcomQueryOffersCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreEcom::Internal_OnCheckoutCallback(const EOS_Ecom_CheckoutCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FEcomCheckoutCallback* CallbackObj = static_cast<FEcomCheckoutCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreEcom::Internal_OnRedeemEntitlementsCallback(const EOS_Ecom_RedeemEntitlementsCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FEcomRedeemEntitlementsCallback* CallbackObj = static_cast<FEcomRedeemEntitlementsCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}