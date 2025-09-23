/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "eos_sanctions_types.h"
#include "Core/EOSHelpers.h"
#include "EOSSanctionsTypes.generated.h"

class UCoreSanctions;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		STRUCTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
/**
* Contains information about a single player sanction.
*/
USTRUCT(BlueprintType)
struct FEOSSanctionsPlayerSanction
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** The POSIX timestamp when the sanction was placed */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sanctions")
	FDateTime TimePlaced;
	/** The action associated with this sanction */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sanctions")
	FString Action;
public:
	explicit FEOSSanctionsPlayerSanction()
		: ApiVersion(EOS_SANCTIONS_PLAYERSANCTION_API_LATEST)
		, TimePlaced(0)
	{
	}

	FEOSSanctionsPlayerSanction(const EOS_Sanctions_PlayerSanction& data)
		: ApiVersion(EOS_SANCTIONS_PLAYERSANCTION_API_LATEST)
		, TimePlaced(data.TimePlaced)
	{
		const FUTF8ToTCHAR ActionChar(data.Action);
		Action = ActionChar.Get();
	}
};

/**
* Input parameters for the EOS_Sanctions_QueryActivePlayerSanctions API.
*/
USTRUCT(BlueprintType)
struct FEOSSanctionsQueryActivePlayerSanctionsOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Product User ID of the user whose active sanctions are to be retrieved. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sanctions")
	FEOSProductUserId TargetUserId;
	/** The Product User ID of the local user who initiated this request. Dedicated servers should set this to null. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sanctions")
	FEOSProductUserId LocalUserId;
public:
	explicit FEOSSanctionsQueryActivePlayerSanctionsOptions()
		: ApiVersion(EOS_SANCTIONS_QUERYACTIVEPLAYERSANCTIONS_API_LATEST)
	{
	}
};

/**
* Output parameters for the EOS_Sanctions_QueryActivePlayerSanctions function.
*/
USTRUCT(BlueprintType)
struct FEOSSanctionsQueryActivePlayerSanctionsCallbackInfo
{
	GENERATED_BODY()
public:
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Sanctions_QueryActivePlayerSanctions. */
	void* ClientData;
	/** Target Product User ID that was passed to EOS_Sanctions_QueryActivePlayerSanctions. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId TargetUserId;
	/** The Product User ID of the local user who initiated this request, if applicable. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FEOSProductUserId LocalUserId;
public:
	FEOSSanctionsQueryActivePlayerSanctionsCallbackInfo()
		: ResultCode(EOSResult::EOS_UnexpectedError)
		, ClientData(nullptr)
	{
	}

	FEOSSanctionsQueryActivePlayerSanctionsCallbackInfo(const EOS_Sanctions_QueryActivePlayerSanctionsCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		, ClientData(data.ClientData)
		, TargetUserId(data.TargetUserId)
		, LocalUserId(data.LocalUserId)
	{
	}
};

/**
* Input parameters for the EOS_Sanctions_GetPlayerSanctionCount function.
*/
USTRUCT(BlueprintType)
struct FEOSSanctionsGetPlayerSanctionCountOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Product User ID of the user whose sanction count should be returned */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sanctions")
	FEOSProductUserId TargetUserId;
public:
	explicit FEOSSanctionsGetPlayerSanctionCountOptions()
		: ApiVersion(EOS_SANCTIONS_GETPLAYERSANCTIONCOUNT_API_LATEST)
	{
	}
};

/**
* Input parameters for the EOS_Sanctions_CopyPlayerSanctionByIndex function
*/
USTRUCT(BlueprintType)
struct FEOSSanctionsCopyPlayerSanctionByIndexOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Product User ID of the user whose active sanctions are to be copied */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sanctions")
	FEOSProductUserId TargetUserId;
	/** Index of the sanction to retrieve from the cache */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sanctions")
	int32 SanctionIndex;
public:
	explicit FEOSSanctionsCopyPlayerSanctionByIndexOptions()
		: ApiVersion(EOS_SANCTIONS_COPYPLAYERSANCTIONBYINDEX_API_LATEST)
		, SanctionIndex(0)
	{
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		DELEGATES
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnQueryActivePlayerSanctionsCallback, const FEOSSanctionsQueryActivePlayerSanctionsCallbackInfo&, data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQueryActivePlayerSanctionsCallbackDelegate, const FEOSSanctionsQueryActivePlayerSanctionsCallbackInfo&, data);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		CALLBACK OBJECTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

struct FQueryActivePlayerSanctionsCallback
{
public:
	FOnQueryActivePlayerSanctionsCallback m_Callback;
public:
	FQueryActivePlayerSanctionsCallback(const FOnQueryActivePlayerSanctionsCallback& callback)
		: m_Callback(callback)
	{
	}
	virtual ~FQueryActivePlayerSanctionsCallback()
	{
		m_Callback.Unbind();
	}
};
