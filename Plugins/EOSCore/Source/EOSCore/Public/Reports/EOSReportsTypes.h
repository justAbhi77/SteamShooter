/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "eos_reports_types.h"
#include "Core/EOSHelpers.h"
#include "EOSReportsTypes.generated.h"

class UCoreReports;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		ENUMS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
/**
* An enumeration of the different player behavior categories that can be reported.
*/
UENUM(BlueprintType)
enum class EEOSEPlayerReportsCategory : uint8
{
	/** Not used */
	EOS_PRC_Invalid = 0,
	/** The reported player is cheating */
	EOS_PRC_Cheating = 1,
	/** The reported player is exploiting the game */
	EOS_PRC_Exploiting = 2,
	/** The reported player has an offensive profile, name, etc */
	EOS_PRC_OffensiveProfile = 3,
	/** The reported player is being abusive in chat */
	EOS_PRC_VerbalAbuse = 4,
	/** The reported player is scamming other players */
	EOS_PRC_Scamming = 5,
	/** The reported player is spamming chat */
	EOS_PRC_Spamming = 6,
	/** The player is being reported for something else */
	EOS_PRC_Other = 7
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		STRUCTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
/**
* Input parameters for the EOS_Reports_SendPlayerBehaviorReport function.
*/
USTRUCT(BlueprintType)
struct FEOSReportsSendPlayerBehaviorReportOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Product User ID of the reporting player */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Reports")
	FEOSProductUserId ReporterUserId;
	/** Product User ID of the reported player. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Reports")
	FEOSProductUserId ReportedUserId;
	/** Category for the player report. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Reports")
	EEOSEPlayerReportsCategory ReportCategory;
	/**
	* Optional plain text string associated with the report as UTF-8 encoded null-terminated string.
	*
	* The length of the message can be at maximum up to EOS_REPORTS_REPORTMESSAGE_MAX_LENGTH bytes
	* and any excess characters will be truncated upon sending the report.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Reports")
	FString Message;
	/**
	* Optional JSON string associated with the report as UTF-8 encoded null-terminated string.
	* This is intended as a way to associate arbitrary structured context information with a report.
	*
	* This string needs to be valid JSON, report will fail otherwise.
	* The length of the context can be at maximum up to EOS_REPORTS_REPORTCONTEXT_MAX_LENGTH bytes, not including the null terminator, report will fail otherwise.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Reports")
	FString Context;
public:
	explicit FEOSReportsSendPlayerBehaviorReportOptions()
		: ApiVersion(EOS_REPORTS_SENDPLAYERBEHAVIORREPORT_API_LATEST)
		, ReportCategory(EEOSEPlayerReportsCategory::EOS_PRC_Other)
	{
	}
};

/**
* Output parameters for the EOS_Reports_SendPlayerBehaviorReport function.
*/
USTRUCT(BlueprintType)
struct FEOSReportsSendPlayerBehaviorReportCompleteCallbackInfo
{
	GENERATED_BODY()
public:
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	EOSResult ResultCode;
	/** Context that was passed into EOS_Reports_SendPlayerBehaviorReport. */
	void* ClientData;
public:
	FEOSReportsSendPlayerBehaviorReportCompleteCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		, ClientData(nullptr)
	{
	}

	FEOSReportsSendPlayerBehaviorReportCompleteCallbackInfo(const EOS_Reports_SendPlayerBehaviorReportCompleteCallbackInfo& Data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(Data.ResultCode))
		, ClientData(Data.ClientData)
	{
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		DELEGATES
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSendPlayerBehaviorReportCompleteCallback, const FEOSReportsSendPlayerBehaviorReportCompleteCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSendPlayerBehaviorReportCompleteDelegate, const FEOSReportsSendPlayerBehaviorReportCompleteCallbackInfo&, Data);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		CALLBACK OBJECTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

struct FSendPlayerBehaviorReportCallback
{
public:
	UCoreReports* ReportsObject;
	FOnSendPlayerBehaviorReportCompleteCallback Callback;
public:
	FSendPlayerBehaviorReportCallback(UCoreReports* obj, const FOnSendPlayerBehaviorReportCompleteCallback& callback) : ReportsObject(obj), Callback(callback)
	{
	}
};