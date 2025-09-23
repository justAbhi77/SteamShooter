/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "TitleStorage/EOSTitleStorage.h"
#include "Core/EOSCoreAsync.h"
#include "EOSTitleStorageAsyncActions.generated.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreTitleStorageQueryFile
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreTitleStorageQueryFile : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FOnTitleStorageQueryFileCompleteCallbackDelegate OnCallback;
private:
	FOnTitleStorageQueryFileCompleteCallback m_Callback;
public:
	UEOSCoreTitleStorageQueryFile()
	{
		m_Callback.BindUFunction(this, "HandleCallback");
	}
	~UEOSCoreTitleStorageQueryFile()
	{
		m_Callback.Unbind();
	}
public:
	/**
	* Query a specific file's metadata, such as file names, size, and a MD5 hash of the data. This is not required before a file may be opened. Once a file has
	* been queried, its metadata will be available by the EOS_TitleStorage_CopyFileMetadataAtIndex and EOS_TitleStorage_CopyFileMetadataByFilename functions.
	*
	* @param Options Object containing properties related to which user is querying files, and what file is being queried
	*
	* @see EOS_TitleStorage_GetFileMetadataCount
	* @see EOS_TitleStorage_CopyFileMetadataAtIndex
	* @see EOS_TitleStorage_CopyFileMetadataByFilename
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|TitleStorage|Async", DisplayName = "EOS_TitleStorage_QueryFile"))
		static UEOSCoreTitleStorageQueryFile* EOSTitleStorageQueryFileAsync(UObject* WorldContextObject, FEOSTitleStorageQueryFileOptions Options);
protected:
	FEOSTitleStorageQueryFileOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
        void HandleCallback(const FEOSTitleStorageQueryFileCallbackInfo& Data, bool bWasSuccessful)
    {
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
    }
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreTitleStorageQueryFileList
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreTitleStorageQueryFileList : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FOnTitleStorageQueryFileListCompleteCallbackDelegate OnCallback;
private:
	FOnTitleStorageQueryFileListCompleteCallback m_Callback;
public:
	UEOSCoreTitleStorageQueryFileList()
	{
		m_Callback.BindUFunction(this, "HandleCallback");
	}
	~UEOSCoreTitleStorageQueryFileList()
	{
		m_Callback.Unbind();
	}
public:
	/**
	* Query the file metadata, such as file names, size, and a MD5 hash of the data, for all files available for current user based on their settings (such as game role) and tags provided.
	* This is not required before a file can be downloaded by name.
	*
	* @param Options Object containing properties related to which user is querying files and the list of tags
	*
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|TitleStorage|Async", DisplayName = "EOS_TitleStorage_QueryFileList"))
		static UEOSCoreTitleStorageQueryFileList* EOSTitleStorageQueryFileListAsync(UObject* WorldContextObject, FEOSTitleStorageQueryFileListOptions Options);
protected:
	FEOSTitleStorageQueryFileListOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
        void HandleCallback(const FEOSTitleStorageQueryFileListCallbackInfo& Data, bool bWasSuccessful)
    {
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
    }
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreTitleStorageReadFile
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreTitleStorageReadFile : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FOnTitleStorageReadFileCompleteCallbackDelegate OnCallback;
private:
	FOnTitleStorageReadFileCompleteCallback m_Callback;
public:
	UEOSCoreTitleStorageReadFile()
	{
		m_Callback.BindUFunction(this, "HandleCallback");
	}
	~UEOSCoreTitleStorageReadFile()
	{
		m_Callback.Unbind();
	}
public:
	/**
	* Retrieve the contents of a specific file, potentially downloading the contents if we do not have a local copy, from the cloud. This request will occur asynchronously, potentially over
	* multiple frames. All callbacks for this function will come from the same thread that the SDK is ticked from. If specified, the FileTransferProgressCallback will always be called at
	* least once if the request is started successfully.
	*
	* @param Options Object containing properties related to which user is opening the file, what the file's name is, and related mechanisms for copying the data
	* @param Callback This function is called when the read operation completes
	* @return A valid Title Storage File Request handle if successful, or NULL otherwise. Data contained in the completion callback will have more detailed information about issues with the request in failure cases. This handle must be released when it is no longer needed
	*
	* @see EOS_TitleStorageFileTransferRequest_Release
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|TitleStorage|Async", DisplayName = "EOS_TitleStorage_ReadFile"))
		static UEOSCoreTitleStorageReadFile* EOSTitleStorageReadFileAsync(UObject* WorldContextObject, FEOSTitleStorageReadFileOptions Options);
protected:
	FEOSTitleStorageReadFileOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
		void HandleCallback(const FEOSTitleStorageReadFileCallbackInfo& Data, bool bWasSuccessful)
    {
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
    }
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreTitleStorageDeleteCache
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCoreTitleStorageDeleteCache : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FOnTitleStorageDeleteCacheCompleteCallbackDelegate OnCallback;
private:
	FOnTitleStorageDeleteCacheCompleteCallback m_Callback;
public:
	UEOSCoreTitleStorageDeleteCache()
	{
		m_Callback.BindUFunction(this, "HandleCallback");
	}
	~UEOSCoreTitleStorageDeleteCache()
	{
		m_Callback.Unbind();
	}
public:
	/**
	* Clear previously cached file data. This operation will be done asynchronously. All cached files except those corresponding to the transfers in progress will be removed.
	* Warning: Use this with care. Cache system generally tries to clear old and unused cached files from time to time. Unnecessarily clearing cache can degrade performance as SDK will have to re-download data.
	*
	* @param Options Object containing properties related to which user is deleting cache
	* @return EOS_Success if the operation was started correctly, otherwise an error result explaining what went wrong
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|TitleStorage|Async", DisplayName = "EOS_TitleStorage_DeleteCache"))
		static UEOSCoreTitleStorageDeleteCache* EOSTitleStorageDeleteCacheAsync(UObject* WorldContextObject, FEOSTitleStorageDeleteCacheOptions Options);
protected:
	FEOSTitleStorageDeleteCacheOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
		void HandleCallback(const FEOSTitleStorageDeleteCacheCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};