/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "PlayerDataStorage/EOSPlayerDataStorageTypes.h"
#include "Core/EOSCoreAsync.h"
#include "EOSPlayerDataStorageAsyncActions.generated.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCorePlayerDataStorageQueryFile
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCorePlayerDataStorageQueryFile : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnQueryFileCompleteCallbackDelegate OnCallback;
private:
	FOnQueryFileCompleteCallback m_Callback;
public:
	UEOSCorePlayerDataStorageQueryFile() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCorePlayerDataStorageQueryFile() { m_Callback.Unbind(); }
public:
	/**
	* Query a specific file's metadata, such as file names, size, and a MD5 hash of the data. This is not required before a file may be opened, saved, copied, or deleted. Once a file has
	* been queried, its metadata will be available by the EOS_PlayerDataStorage_CopyFileMetadataAtIndex and EOS_PlayerDataStorage_CopyFileMetadataByFilename functions.
	*
	* @param QueryFileOptions Object containing properties related to which user is querying files, and what file is being queried
	*
	* @see EOS_PlayerDataStorage_GetFileMetadataCount
	* @see EOS_PlayerDataStorage_CopyFileMetadataAtIndex
	* @see EOS_PlayerDataStorage_CopyFileMetadataByFilename
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|PlayerDataStorage|Async", DisplayName = "EOS_PlayerDataStorage_QueryFile"))
	static UEOSCorePlayerDataStorageQueryFile* EOSPlayerDataStorageQueryFileAsync(UObject* WorldContextObject, FEOSPlayerDataStorageQueryFileOptions QueryFileOptions);
protected:
	FEOSPlayerDataStorageQueryFileOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSPlayerDataStorageQueryFileCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCorePlayerDataStorageQueryFileList
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCorePlayerDataStorageQueryFileList : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnQueryFileListCompleteCallbackDelegate OnCallback;
private:
	FOnQueryFileListCompleteCallback m_Callback;
public:
	UEOSCorePlayerDataStorageQueryFileList() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCorePlayerDataStorageQueryFileList() { m_Callback.Unbind(); }
public:
	/**
	* Query the file metadata, such as file names, size, and a MD5 hash of the data, for all files owned by this user for this application. This is not required before a file may be opened,
	* saved, copied, or deleted.
	*
	* @param QueryFileListOptions Object containing properties related to which user is querying files
	*
	* @see EOS_PlayerDataStorage_GetFileMetadataCount
	* @see EOS_PlayerDataStorage_CopyFileMetadataAtIndex
	* @see EOS_PlayerDataStorage_CopyFileMetadataByFilename
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|PlayerDataStorage|Async", DisplayName = "EOS_PlayerDataStorage_QueryFileList"))
	static UEOSCorePlayerDataStorageQueryFileList* EOSPlayerDataStorageQueryFileListAsync(UObject* WorldContextObject, FEOSPlayerDataStorageQueryFileListOptions QueryFileListOptions);
protected:
	FEOSPlayerDataStorageQueryFileListOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSPlayerDataStorageQueryFileListCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCorePlayerDataStorageDuplicateFile
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCorePlayerDataStorageDuplicateFile : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDuplicateFileCompleteCallbackDelegate OnCallback;
private:
	FOnDuplicateFileCompleteCallback m_Callback;
public:
	UEOSCorePlayerDataStorageDuplicateFile() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCorePlayerDataStorageDuplicateFile() { m_Callback.Unbind(); }
public:
	/**
	* Copies the data of an existing file to a new filename. This action happens entirely on the server and will not upload the contents of the source destination file from the host. This
	* function paired with a subsequent EOS_PlayerDataStorage_DeleteFile can be used to rename a file. If successful, the destination file's metadata will be updated in our local cache.
	*
	* @param DuplicateOptions Object containing properties related to which user is duplicating the file, and what the source and destination file names are
	*
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|PlayerDataStorage|Async", DisplayName = "EOS_PlayerDataStorage_DuplicateFile"))
	static UEOSCorePlayerDataStorageDuplicateFile* EOSPlayerDataStorageDuplicateFileAsync(UObject* WorldContextObject, FEOSPlayerDataStorageDuplicateFileOptions DuplicateOptions);
protected:
	FEOSPlayerDataStorageDuplicateFileOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSPlayerDataStorageDuplicateFileCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCorePlayerDataStorageDeleteFile
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCorePlayerDataStorageDeleteFile : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnDeleteFileCompleteCallbackDelegate OnCallback;
private:
	FOnDeleteFileCompleteCallback m_Callback;
public:
	UEOSCorePlayerDataStorageDeleteFile() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCorePlayerDataStorageDeleteFile() { m_Callback.Unbind(); }
public:
	/**
	* Deletes an existing file in the cloud. If successful, the file's data will be removed from our local cache.
	*
	* @param DeleteOptions Object containing properties related to which user is deleting the file, and what file name is
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|PlayerDataStorage|Async", DisplayName = "EOS_PlayerDataStorage_DeleteFile"))
	static UEOSCorePlayerDataStorageDeleteFile* EOSPlayerDataStorageDeleteFileAsync(UObject* WorldContextObject, FEOSPlayerDataStorageDeleteFileOptions DeleteOptions);
protected:
	FEOSPlayerDataStorageDeleteFileOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSPlayerDataStorageDeleteFileCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCorePlayerDataStorageReadFile
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCorePlayerDataStorageReadFile : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnReadFileCompleteCallbackDelegate OnCallback;
private:
	FOnReadFileCompleteCallback m_Callback;
	FOnFileTransferProgressUpdated m_ProgressCallback;
public:
	UEOSCorePlayerDataStorageReadFile() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCorePlayerDataStorageReadFile() { m_Callback.Unbind(); }
public:
	/**
	* Retrieve the contents of a specific file, potentially downloading the contents if we do not have a local copy, from the cloud. This request will occur asynchronously, potentially over
	* multiple frames. All callbacks for this function will come from the same thread that the SDK is ticked from. If specified, the FileTransferProgressCallback will always be called at
	* least once if the request is started successfully.
	*
	* @param ReadOptions Object containing properties related to which user is opening the file, what the file's name is, and related mechanisms for copying the data
	* @return A valid Player Data Storage File Request handle if successful, or NULL otherwise. Data contained in the completion callback will have more detailed information about issues with the request in failure cases. This handle must be released when it is no longer needed
	*
	* @see EOS_PlayerDataStorageFileTransferRequest_Release
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|PlayerDataStorage|Async", DisplayName = "EOS_PlayerDataStorage_ReadFile"))
	static UEOSCorePlayerDataStorageReadFile* EOSPlayerDataStorageReadFileAsync(UObject* WorldContextObject, FEOSPlayerDataStorageReadFileOptions ReadOptions);
protected:
	FEOSPlayerDataStorageReadFileOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSPlayerDataStorageReadFileCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCorePlayerDataStorageWriteFile
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UCLASS()
class EOSCORE_API UEOSCorePlayerDataStorageWriteFile : public UEOSCoreAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnWriteFileCompleteCallbackDelegate OnCallback;
private:
	FOnWriteFileCompleteCallback m_Callback;
	FOnFileTransferProgressUpdated m_ProgressCallback;
public:
	UEOSCorePlayerDataStorageWriteFile() { m_Callback.BindUFunction(this, "HandleCallback"); }
	~UEOSCorePlayerDataStorageWriteFile() { m_Callback.Unbind(); }
public:
	/**
	* Write new data to a specific file, potentially overwriting any existing file by the same name, to the cloud. This request will occur asynchronously, potentially over multiple frames.
	* All callbacks for this function will come from the same thread that the SDK is ticked from. If specified, the FileTransferProgressCallback will always be called at least once if the
	* request is started successfully.
	*
	* @param WriteOptions Object containing properties related to which user is writing the file, what the file's name is, and related mechanisms for writing the data
	* @return A valid Player Data Storage File Request handle if successful, or NULL otherwise. Data contained in the completion callback will have more detailed information about issues with the request in failure cases. This handle must be released when it is no longer needed
	*
	* @see EOS_PlayerDataStorageFileTransferRequest_Release
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", Category = "EOSCore|PlayerDataStorage|Async", DisplayName = "EOS_PlayerDataStorage_WriteFile"))
	static UEOSCorePlayerDataStorageWriteFile* EOSPlayerDataStorageWriteFileAsync(UObject* WorldContextObject, FEOSPlayerDataStorageWriteFileOptions WriteOptions);
protected:
	FEOSPlayerDataStorageWriteFileOptions m_Options;
public:
	virtual void Activate() override;
private:
	UFUNCTION()
	void HandleCallback(const FEOSPlayerDataStorageWriteFileCallbackInfo& Data, bool bWasSuccessful)
	{
		OnCallback.Broadcast(Data);
		m_Callback.Unbind();
		SetReadyToDestroy();
	}
};
