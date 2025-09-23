/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Core/EOSHelpers.h"
#include "eos_progressionsnapshot_types.h"
#include "EOSProgressionSnapshotTypes.generated.h"

class UCoreProgressionSnapshot;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		ENUMS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		STRUCTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

USTRUCT(BlueprintType)
struct FEOSProgressionSnapshotBeginSnapshotOptions
{
	GENERATED_BODY()
public:
	/** The Product User ID of the local user to whom the key/value pair belong */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Progression Snapshot")
	FEOSProductUserId LocalUserId;
public:
	FEOSProgressionSnapshotBeginSnapshotOptions() = default;
};

USTRUCT(BlueprintType)
struct FEOSProgressionSnapshotAddProgressionOptions
{
	GENERATED_BODY()
public:
	/** The Snapshot Id received via a EOS_ProgressionSnapshot_BeginSnapshot function. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Progression Snapshot")
	int32 SnapshotId;
	/** The key in a key/value pair of progression entry */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Progression Snapshot")
	FString Key;
	/** The value in a key/value pair of progression entry */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Progression Snapshot")
	FString Value;
public:
	FEOSProgressionSnapshotAddProgressionOptions()
		: SnapshotId(0)
	{}
};

USTRUCT(BlueprintType)
struct FEOSProgressionSnapshotSubmitSnapshotOptions
{
	GENERATED_BODY()
public:
	/** The Snapshot Id received via a EOS_ProgressionSnapshot_BeginSnapshot function. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Progression Snapshot")
	int32 SnapshotId;
public:
	FEOSProgressionSnapshotSubmitSnapshotOptions()
		: SnapshotId(0)
	{}
};

USTRUCT(BlueprintType)
struct FEOSProgressionSnapshotEndSnapshotOptions
{
	GENERATED_BODY()
public:
	/** The Snapshot Id received via a EOS_ProgressionSnapshot_BeginSnapshot function. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Progression Snapshot")
	int32 SnapshotId;
public:
	FEOSProgressionSnapshotEndSnapshotOptions()
		: SnapshotId(0)
	{}
};

USTRUCT(BlueprintType)
struct FEOSProgressionSnapshotDeleteSnapshotOptions
{
	GENERATED_BODY()
public:
	/** The Product User ID of the local user to whom the key/value pair belong */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Progression Snapshot")
	FEOSProductUserId LocalUserId;
public:
	FEOSProgressionSnapshotDeleteSnapshotOptions() = default;
};

USTRUCT(BlueprintType)
struct FEOSProgressionSnapshotSubmitSnapshotCallbackInfo
{
	GENERATED_BODY()
public:
	/* The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Presence")
	EOSResult ResultCode;
	/** The Snapshot Id used in the Submit function. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Presence")
	int32 SnapshotId;
	/** Context that was passed into EOS_ProgressionSnapshot_SubmitSnapshot. */
	void* ClientData;
public:
	FEOSProgressionSnapshotSubmitSnapshotCallbackInfo()
		: ResultCode(EOSResult::EOS_UnexpectedError)
		, SnapshotId(0)
		, ClientData(nullptr)
	{}

	FEOSProgressionSnapshotSubmitSnapshotCallbackInfo(const EOS_ProgressionSnapshot_SubmitSnapshotCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		, SnapshotId(data.SnapshotId)
		, ClientData(data.ClientData)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSProgressionSnapshotDeleteSnapshotCallbackInfo
{
	GENERATED_BODY()
public:
	/* The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Presence")
	EOSResult ResultCode;
	/** The Product User ID of the local user to whom the key/value pair belong */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Presence")
	FEOSProductUserId LocalUserId;
	/** Context that was passed into EOS_ProgressionSnapshot_SubmitSnapshot. */
	void* ClientData;
public:
	FEOSProgressionSnapshotDeleteSnapshotCallbackInfo()
		: ResultCode(EOSResult::MAX)
		, ClientData(nullptr) {}

	FEOSProgressionSnapshotDeleteSnapshotCallbackInfo(const EOS_ProgressionSnapshot_DeleteSnapshotCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		, LocalUserId(data.LocalUserId)
		, ClientData(data.ClientData)
	{
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		DELEGATES
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSubmitSnapshotCallback, const FEOSProgressionSnapshotSubmitSnapshotCallbackInfo&, data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnDeleteSnapshotCallback, const FEOSProgressionSnapshotDeleteSnapshotCallbackInfo&, data);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		CALLBACK OBJECTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

struct FSubmitSnapshotCallback
{
public:
	FOnSubmitSnapshotCallback m_Callback;
public:
	FSubmitSnapshotCallback(const FOnSubmitSnapshotCallback& callback)
		: m_Callback(callback)
	{
	}
	virtual ~FSubmitSnapshotCallback()
	{
		m_Callback.Unbind();
	}
};

struct FDeleteSnapshotCallback
{
public:
	FOnDeleteSnapshotCallback m_Callback;
public:
	FDeleteSnapshotCallback(const FOnDeleteSnapshotCallback& callback)
		: m_Callback(callback)
	{
	}
	virtual ~FDeleteSnapshotCallback()
	{
		m_Callback.Unbind();
	}
};