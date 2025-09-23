/**
* Copyright (C) 2017-2022 | eelDev AB
*
 official EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "EOSCoreModule.h"
#include "eos_playerdatastorage_types.h"
#include "PlayerDataStorage/EOSPlayerDataStorageTypes.h"
#include "EOSPlayerDataStorage.generated.h"

/**
 * The following EOS_PlayerDataStorage_* functions allow you to query file metadata; create/upload files; and duplicate, read, and delete existing files
 */

UCLASS()
class EOSCORE_API UCorePlayerDataStorage : public UEOSCoreSubsystem
{
	GENERATED_BODY()
public:
	virtual void Deinitialize() override;
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Interfaces", meta = (WorldContext = "WorldContextObject"))
	static UCorePlayerDataStorage* GetPlayerDataStorage(UObject* WorldContextObject);

public:
	/**
	* Query a specific file's metadata, such as file names, size, and a MD5 hash of the data. This is not required before a file may be opened, saved, copied, or deleted. Once a file has
	* been queried, its metadata will be available by the EOS_PlayerDataStorage_CopyFileMetadataAtIndex and EOS_PlayerDataStorage_CopyFileMetadataByFilename functions.
	*
	* @param QueryFileOptions Object containing properties related to which user is querying files, and what file is being queried
	* @param Callback This function is called when the query operation completes
	*
	* @see EOS_PlayerDataStorage_GetFileMetadataCount
	* @see EOS_PlayerDataStorage_CopyFileMetadataAtIndex
	* @see EOS_PlayerDataStorage_CopyFileMetadataByFilename
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|PlayerDataStorage", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_PlayerDataStorage_QueryFile"))
	void EOSPlayerDataStorageQueryFile(UObject* WorldContextObject, FEOSPlayerDataStorageQueryFileOptions QueryFileOptions, const FOnQueryFileCompleteCallback& Callback);

	/**
	* Query the file metadata, such as file names, size, and a MD5 hash of the data, for all files owned by this user for this application. This is not required before a file may be opened,
	* saved, copied, or deleted.
	*
	* @param QueryFileListOptions Object containing properties related to which user is querying files
	* @param Callback This function is called when the query operation completes
	*
	* @see EOS_PlayerDataStorage_GetFileMetadataCount
	* @see EOS_PlayerDataStorage_CopyFileMetadataAtIndex
	* @see EOS_PlayerDataStorage_CopyFileMetadataByFilename
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|PlayerDataStorage", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_PlayerDataStorage_QueryFileList"))
	void EOSPlayerDataStorageQueryFileList(UObject* WorldContextObject, FEOSPlayerDataStorageQueryFileListOptions QueryFileListOptions, const FOnQueryFileListCompleteCallback& Callback);

	/**
	* Create the cached copy of a file's metadata by filename. The metadata will be for the last retrieved or successfully saved version, and will not include any changes that have not
	* completed writing. The returned pointer must be released by the user when no longer needed.
	*
	* @param CopyFileMetadataOptions Object containing properties related to which user is requesting metadata, and for which filename
	* @param OutMetadata A copy of the FileMetadata structure will be set if successful.  This data must be released by calling EOS_PlayerDataStorage_FileMetadata_Release.
	* @return EOS_EResult::EOS_Success if the metadata is currently cached, otherwise an error result explaining what went wrong
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|PlayerDataStorage", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_PlayerDataStorage_CopyFileMetadataByFilename"))
	static EOSResult EOSPlayerDataStorageCopyFileMetadataByFilename(UObject* WorldContextObject, FEOSPlayerDataStorageCopyFileMetadataByFilenameOptions CopyFileMetadataOptions, FEOSPlayerDataStorageFileMetadata& OutMetadata);

	/**
	* Get the count of files we have previously queried information for and files we have previously read from / written to.
	*
	* @param GetFileMetadataCountOptions Object containing properties related to which user is requesting the metadata count
	* @param OutFileMetadataCount If successful, the count of metadata currently cached
	* @return EOS_EResult::EOS_Success if the input was valid, otherwise an error result explaining what went wrong
	*
	* @see EOS_PlayerDataStorage_CopyFileMetadataAtIndex
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|PlayerDataStorage", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_PlayerDataStorage_GetFileMetadataCount"))
	static EOSResult EOSPlayerDataStorageGetFileMetadataCount(UObject* WorldContextObject, FEOSPlayerDataStorageGetFileMetadataCountOptions GetFileMetadataCountOptions, int32& OutFileMetadataCount);

	/**
	* Get the cached copy of a file's metadata by index. The metadata will be for the last retrieved or successfully saved version, and will not include any local changes that have not been
	* committed by calling SaveFile. The returned pointer must be released by the user when no longer needed.
	*
	* @param CopyFileMetadataOptions Object containing properties related to which user is requesting metadata, and at what index
	* @param OutMetadata A copy of the FileMetadata structure will be set if successful.  This data must be released by calling EOS_PlayerDataStorage_FileMetadata_Release.
	* @return EOS_EResult::EOS_Success if the requested metadata is currently cached, otherwise an error result explaining what went wrong
	*
	* @see EOS_PlayerDataStorage_GetFileMetadataCount
	* @see EOS_PlayerDataStorage_FileMetadata_Release
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|PlayerDataStorage", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_PlayerDataStorage_CopyFileMetadataAtIndex"))
	static EOSResult EOSPlayerDataStorageCopyFileMetadataAtIndex(UObject* WorldContextObject, FEOSPlayerDataStorageCopyFileMetadataAtIndexOptions CopyFileMetadataOptions, FEOSPlayerDataStorageFileMetadata& OutMetadata);

	/**
	* Copies the data of an existing file to a new filename. This action happens entirely on the server and will not upload the contents of the source destination file from the host. This
	* function paired with a subsequent EOS_PlayerDataStorage_DeleteFile can be used to rename a file. If successful, the destination file's metadata will be updated in our local cache.
	*
	* @param DuplicateOptions Object containing properties related to which user is duplicating the file, and what the source and destination file names are
	* @param Callback This function is called when the duplicate operation completes
	*
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|PlayerDataStorage", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_PlayerDataStorage_DuplicateFile"))
	void EOSPlayerDataStorageDuplicateFile(UObject* WorldContextObject, FEOSPlayerDataStorageDuplicateFileOptions DuplicateOptions, const FOnDuplicateFileCompleteCallback& Callback);

	/**
	* Deletes an existing file in the cloud. If successful, the file's data will be removed from our local cache.
	*
	* @param DeleteOptions Object containing properties related to which user is deleting the file, and what file name is
	* @param Callback This function is called when the delete operation completes
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|PlayerDataStorage", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_PlayerDataStorage_DeleteFile"))
	void EOSPlayerDataStorageDeleteFile(UObject* WorldContextObject, FEOSPlayerDataStorageDeleteFileOptions DeleteOptions, const FOnDeleteFileCompleteCallback& Callback);

	/**
	* Retrieve the contents of a specific file, potentially downloading the contents if we do not have a local copy, from the cloud. This request will occur asynchronously, potentially over
	* multiple frames. All callbacks for this function will come from the same thread that the SDK is ticked from. If specified, the FileTransferProgressCallback will always be called at
	* least once if the request is started successfully.
	*
	* @param ReadOptions Object containing properties related to which user is opening the file, what the file's name is, and related mechanisms for copying the data
	* @param Callback This function is called when the read operation completes
	* @return A valid Player Data Storage File Request handle if successful, or NULL otherwise. Data contained in the completion callback will have more detailed information about issues with the request in failure cases. This handle must be released when it is no longer needed
	*
	* @see EOS_PlayerDataStorageFileTransferRequest_Release
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|PlayerDataStorage", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_PlayerDataStorage_ReadFile"))
	void EOSPlayerDataStorageReadFile(UObject* WorldContextObject, FEOSPlayerDataStorageReadFileOptions ReadOptions, const FOnReadFileCompleteCallback& Callback);

	/**
	* Write new data to a specific file, potentially overwriting any existing file by the same name, to the cloud. This request will occur asynchronously, potentially over multiple frames.
	* All callbacks for this function will come from the same thread that the SDK is ticked from. If specified, the FileTransferProgressCallback will always be called at least once if the
	* request is started successfully.
	*
	* @param WriteOptions Object containing properties related to which user is writing the file, what the file's name is, and related mechanisms for writing the data
	* @param Callback This function is called when the write operation completes
	* @return A valid Player Data Storage File Request handle if successful, or NULL otherwise. Data contained in the completion callback will have more detailed information about issues with the request in failure cases. This handle must be released when it is no longer needed
	*
	* @see EOS_PlayerDataStorageFileTransferRequest_Release
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|PlayerDataStorage", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_PlayerDataStorage_WriteFile"))
	void EOSPlayerDataStorageWriteFile(UObject* WorldContextObject, FEOSPlayerDataStorageWriteFileOptions WriteOptions, const FOnWriteFileCompleteCallback& Callback);

	/**
	* Clear previously cached file data. This operation will be done asynchronously. All cached files except those corresponding to the transfers in progress will be removed.
	* Warning: Use this with care. Cache system generally tries to clear old and unused cached files from time to time. Unnecessarily clearing cache can degrade performance as SDK will have to re-download data.
	*
	* @param Options Object containing properties related to which user is deleting cache
	* @param Callback This function is called when the delete cache operation completes
	* @return EOS_Success if the operation was started correctly, otherwise an error result explaining what went wrong
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|PlayerDataStorage", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_PlayerDataStorage_DeleteCache"))
	EOSResult EOSPlayerDataStorageDeleteCache(UObject* WorldContextObject, FEOSPlayerDataStorageDeleteCacheOptions Options, const FOnDeleteCacheCompleteCallback& Callback);

	/**
	 * Below are helper functions to retrieve information about a file request handle, or to attempt to cancel a request in progress.
	 */

	/**
	* Get the current state of a file request.
	*
	* @return EOS_Success if complete and successful, EOS_PlayerDataStorage_RequestInProgress if the request is still in progress, or another state for failure.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|PlayerDataStorage", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_PlayerDataStorageFileTransferRequest_GetFileRequestState"))
	static EOSResult EOSPlayerDataStorageFileTransferRequestGetFileRequestState(UObject* WorldContextObject, const FEOSHPlayerDataStorageFileTransferRequest& Handle);

	/**
	* Get the file name of the file this request is for. OutStringLength will always be set to the string length of the file name if it is not NULL.
	*
	* @param FilenameStringBufferSizeBytes The maximum number of bytes that can be written to OutStringBuffer
	* @param OutStringBuffer The buffer to write the NULL-terminated utf8 file name into, if successful
	* @return EOS_Success if the file name was successfully written to OutFilenameBuffer, a failure result otherwise
	*
	* @see EOS_PLAYERDATASTORAGE_FILENAME_MAX_LENGTH_BYTES
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|PlayerDataStorage", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_PlayerDataStorageFileTransferRequest_GetFilename"))
	static EOSResult EOSPlayerDataStorageFileTransferRequestGetFilename(UObject* WorldContextObject, const FEOSHPlayerDataStorageFileTransferRequest& Handle, int32 FilenameStringBufferSizeBytes, FString& OutStringBuffer);

	/**
	* Attempt to cancel this file request in progress. This is a best-effort command and is not guaranteed to be successful if the request has completed before this function is called.
	*
	* @return EOS_Success if cancel is successful, EOS_NoChange if request had already completed (can't be canceled), EOS_AlreadyPending if it's already been canceled before (this is a final state for canceled request and won't change over time).
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|PlayerDataStorage", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_PlayerDataStorageFileTransferRequest_CancelRequest"))
	static EOSResult EOSPlayerDataStorageFileTransferRequestCancelRequest(UObject* WorldContextObject, const FEOSHPlayerDataStorageFileTransferRequest& Handle);
public:
	/**
	* Free the memory used by a cloud-storage file request handle. This will not cancel a request in progress.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|PlayerDataStorage", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_PlayerDataStorageFileTransferRequest_Release"))
	static void EOSPlayerDataStorageFileTransferRequestRelease(UObject* WorldContextObject, const FEOSHPlayerDataStorageFileTransferRequest& Handle);
private:
	EOS_PlayerDataStorage_EReadResult ReceiveData(const FString& Filename, const void* Data, size_t NumBytes, size_t TotalSize);
	EOS_PlayerDataStorage_EWriteResult SendData(const FString& Filename, void* Data, uint32_t* BytesWritten);
	static EOS_PlayerDataStorage_EReadResult OnFileDataReceived(const EOS_PlayerDataStorage_ReadFileDataCallbackInfo* Data);
	static EOS_PlayerDataStorage_EWriteResult OnFileDataSend(const EOS_PlayerDataStorage_WriteFileDataCallbackInfo* Data, void* OutDataBuffer, uint32_t* OutDataWritten);
	static void OnFileTransferProgressUpdated(const EOS_PlayerDataStorage_FileTransferProgressCallbackInfo* Data);
private:
	static void Internal_OnQueryFileCallback(const EOS_PlayerDataStorage_QueryFileCallbackInfo* Data);
	static void Internal_OnQueryFileListCallback(const EOS_PlayerDataStorage_QueryFileListCallbackInfo* Data);
	static void Internal_OnDuplicateFileCallback(const EOS_PlayerDataStorage_DuplicateFileCallbackInfo* Data);
	static void Internal_OnDeleteFileCallback(const EOS_PlayerDataStorage_DeleteFileCallbackInfo* Data);
	static void Internal_OnReadFileCallback(const EOS_PlayerDataStorage_ReadFileCallbackInfo* Data);
	static void Internal_OnWriteFileCallback(const EOS_PlayerDataStorage_WriteFileCallbackInfo* Data);
	static void Internal_OnDeleteCacheCallback(const EOS_PlayerDataStorage_DeleteCacheCallbackInfo* Data);
private:
	static FEOSHPlayerDataStorageFileTransferRequest s_FileTransferHandleWrite;
	static FEOSHPlayerDataStorageFileTransferRequest s_FileTransferHandleRead;
	static FTransferInProgress s_CurrentRead;
	static FTransferInProgress s_CurrentWrite;
};
