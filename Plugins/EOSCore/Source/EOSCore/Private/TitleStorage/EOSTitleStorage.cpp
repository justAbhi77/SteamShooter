/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "TitleStorage/EOSTitleStorage.h"
#include "eos_titlestorage.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLogging.h"

static size_t MaxChunkSizeTitleStorage = 4 * 4 * 4096;

FTitleStorageTransferInProgress UCoreTitleStorage::m_CurrentRead;

UCoreTitleStorage* UCoreTitleStorage::GetTitleStorage(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (GetTitleStorageHandle(WorldContextObject))
		{
			if (UWorld* World = WorldContextObject->GetWorld())
			{
				if (UGameInstance* GameInstance = World->GetGameInstance())
				{
					return GameInstance->GetSubsystem<UCoreTitleStorage>();
				}
			}
		}
	}
	return nullptr;
}

void UCoreTitleStorage::EOSTitleStorageQueryFile(UObject* WorldContextObject, const FEOSTitleStorageQueryFileOptions Options, const FOnTitleStorageQueryFileCompleteCallback& Callback)
{
	LogVerbose("");

	if (GetTitleStorageHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedFilename(*Options.Filename);

		EOS_TitleStorage_QueryFileOptions TitleStorageOptions = {};
		TitleStorageOptions.ApiVersion = Options.ApiVersion;
		TitleStorageOptions.LocalUserId = Options.LocalUserId;
		TitleStorageOptions.Filename = ConvertedFilename.Get();

		FQueryFileOptionsCallback* CallbackObj = new FQueryFileOptionsCallback(Callback);
		EOS_TitleStorage_QueryFile(GetTitleStorageHandle(WorldContextObject), &TitleStorageOptions, CallbackObj, Internal_OnTitleStorageQueryFileCallback);
	}
}

void UCoreTitleStorage::EOSTitleStorageQueryFileList(UObject* WorldContextObject, const FEOSTitleStorageQueryFileListOptions options, const FOnTitleStorageQueryFileListCompleteCallback& Callback)
{
	LogVerbose("");

	if (GetTitleStorageHandle(WorldContextObject))
	{
		EOS_TitleStorage_QueryFileListOptions TitleStorageOptions = {};
		TitleStorageOptions.ApiVersion = options.ApiVersion;
		TitleStorageOptions.LocalUserId = options.LocalUserId;

		TArray<const char*> DataArray;

		for (int32 i = 0; i < options.ListOfTags.Num(); i++)
		{
			DataArray.Add(TCHAR_TO_UTF8(*options.ListOfTags[i]));
		}

		TitleStorageOptions.ListOfTagsCount = options.ListOfTags.Num();

		TitleStorageOptions.ListOfTags = DataArray.GetData();

		FQueryFileListOptionsCallback* CallbackObj = new FQueryFileListOptionsCallback(Callback);
		EOS_TitleStorage_QueryFileList(GetTitleStorageHandle(WorldContextObject), &TitleStorageOptions, CallbackObj, Internal_OnTitleStorageQueryFileListCallback);
	}
}

EOSResult UCoreTitleStorage::EOSTitleStorageCopyFileMetadataByFilename(UObject* WorldContextObject, const FEOSTitleStorageCopyFileMetadataByFilenameOptions Options, FEOSTitleStorageFileMetadata& OutMetadata)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetTitleStorageHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedFilename(*Options.Filename);

		EOS_TitleStorage_CopyFileMetadataByFilenameOptions TitleStorageOptions = {};
		TitleStorageOptions.ApiVersion = Options.ApiVersion;
		TitleStorageOptions.LocalUserId = Options.LocalUserId;
		TitleStorageOptions.Filename = ConvertedFilename.Get();

		EOS_TitleStorage_FileMetadata* MetaData = NULL;
		Result = EOSHelpers::ToEOSCoreResult(EOS_TitleStorage_CopyFileMetadataByFilename(GetTitleStorageHandle(WorldContextObject), &TitleStorageOptions, &MetaData));

		if (Result == EOSResult::EOS_Success)
		{
			OutMetadata = *MetaData;
			EOS_TitleStorage_FileMetadata_Release(MetaData);
		}
	}

	return Result;
}

int32 UCoreTitleStorage::EOSTitleStorageGetFileMetadataCount(UObject* WorldContextObject, const FEOSTitleStorageGetFileMetadataCountOptions Options)
{
	LogVerbose("");

	int32 Result = 0;

	if (GetTitleStorageHandle(WorldContextObject))
	{
		EOS_TitleStorage_GetFileMetadataCountOptions TitleStorageOptions = {};
		TitleStorageOptions.ApiVersion = Options.ApiVersion;
		TitleStorageOptions.LocalUserId = Options.LocalUserId;

		Result = EOS_TitleStorage_GetFileMetadataCount(GetTitleStorageHandle(WorldContextObject), &TitleStorageOptions);
	}

	return Result;
}

EOSResult UCoreTitleStorage::EOSTitleStorageCopyFileMetadataAtIndex(UObject* WorldContextObject, FEOSTitleStorageCopyFileMetadataAtIndexOptions Options, FEOSTitleStorageFileMetadata& OutMetadata)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetTitleStorageHandle(WorldContextObject))
	{
		EOS_TitleStorage_CopyFileMetadataAtIndexOptions TitleStorageOptions = {};
		TitleStorageOptions.ApiVersion = Options.ApiVersion;
		TitleStorageOptions.LocalUserId = Options.LocalUserId;
		TitleStorageOptions.Index = static_cast<uint32_t>(Options.Index);

		EOS_TitleStorage_FileMetadata* MetaData = NULL;
		Result = EOSHelpers::ToEOSCoreResult(EOS_TitleStorage_CopyFileMetadataAtIndex(GetTitleStorageHandle(WorldContextObject), &TitleStorageOptions, &MetaData));

		if (Result == EOSResult::EOS_Success)
		{
			OutMetadata = *MetaData;
			EOS_TitleStorage_FileMetadata_Release(MetaData);
		}
	}

	return Result;
}

FEOSTitleStorageFileTransferRequestHandle UCoreTitleStorage::EOSTitleStorageReadFile(UObject* WorldContextObject, FEOSTitleStorageReadFileOptions Options, const FOnTitleStorageReadFileCompleteCallback& Callback)
{
	LogVerbose("");

	FEOSTitleStorageFileTransferRequestHandle Handle;

	if (GetTitleStorageHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedFilename(*Options.Filename);

		EOS_TitleStorage_ReadFileOptions TitleStorageOptions = {};
		TitleStorageOptions.ApiVersion = Options.ApiVersion;
		TitleStorageOptions.LocalUserId = Options.LocalUserId;
		TitleStorageOptions.Filename = ConvertedFilename.Get();
		TitleStorageOptions.ReadChunkLengthBytes = static_cast<uint32_t>(MaxChunkSizeTitleStorage);
		TitleStorageOptions.ReadFileDataCallback = OnReadFileDataCallback;
		TitleStorageOptions.FileTransferProgressCallback = OnFileTransferProgressCallback;

		FReadFileCallback* CallbackObj = new FReadFileCallback(this, Callback);
		Handle = EOS_TitleStorage_ReadFile(GetTitleStorageHandle(WorldContextObject), &TitleStorageOptions, CallbackObj, Internal_OnTitleStorageReadFileCallback);

		m_CurrentRead.LocalFile = Options.SaveFileName;
		m_CurrentRead.CurrentTransferHandle = Handle;
	}

	return Handle;
}

EOSResult UCoreTitleStorage::EOSTitleStorageDeleteCache(UObject* WorldContextObject, FEOSTitleStorageDeleteCacheOptions Options, const FOnTitleStorageDeleteCacheCompleteCallback& Callback)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetTitleStorageHandle(WorldContextObject))
	{
		EOS_TitleStorage_DeleteCacheOptions TitleStorageOptions = {Options.ApiVersion, Options.LocalUserId};
		TitleStorageOptions.ApiVersion = Options.ApiVersion;
		TitleStorageOptions.LocalUserId = Options.LocalUserId;

		FDeleteCacheCallback* CallbackObj = new FDeleteCacheCallback(Callback);
		Result = EOSHelpers::ToEOSCoreResult(EOS_TitleStorage_DeleteCache(GetTitleStorageHandle(WorldContextObject), &TitleStorageOptions, CallbackObj, Internal_OnTitleStorageDeleteCacheCallback));
	}

	return Result;
}

EOSResult UCoreTitleStorage::EOSTitleStorageFileTransferRequestGetFileRequestState(UObject* WorldContextObject, const FEOSTitleStorageFileTransferRequestHandle& Handle)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetTitleStorageHandle(WorldContextObject))
	{
		Result = EOSHelpers::ToEOSCoreResult(EOS_TitleStorageFileTransferRequest_GetFileRequestState(Handle));
	}

	return Result;
}

EOSResult UCoreTitleStorage::EOSTitleStorageFileTransferRequestGetFilename(UObject* WorldContextObject, const FEOSTitleStorageFileTransferRequestHandle& Handle, int32 FilenameStringBufferSizeBytes, FString& OutStringBuffer)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetTitleStorageHandle(WorldContextObject))
	{
		char DataArray[EOS_TITLESTORAGE_FILENAME_MAX_LENGTH_BYTES + 1];
		int32_t* Size = nullptr;
		Result = EOSHelpers::ToEOSCoreResult(EOS_TitleStorageFileTransferRequest_GetFilename(Handle, (EOS_TITLESTORAGE_FILENAME_MAX_LENGTH_BYTES + 1), DataArray, Size));

		if (Result == EOSResult::EOS_Success)
		{
			OutStringBuffer = UTF8_TO_TCHAR(DataArray);
		}
	}

	return Result;
}

EOSResult UCoreTitleStorage::EOSTitleStorageFileTransferRequestCancelRequest(UObject* WorldContextObject, const FEOSTitleStorageFileTransferRequestHandle& Handle)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetTitleStorageHandle(WorldContextObject))
	{
		Result = EOSHelpers::ToEOSCoreResult(EOS_TitleStorageFileTransferRequest_CancelRequest(Handle));
	}

	return Result;
}

void UCoreTitleStorage::Internal_OnTitleStorageQueryFileCallback(const EOS_TitleStorage_QueryFileCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FQueryFileOptionsCallback* CallbackObj = static_cast<FQueryFileOptionsCallback*>(Data->ClientData);
	check(CallbackObj);

	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreTitleStorage::Internal_OnTitleStorageQueryFileListCallback(const EOS_TitleStorage_QueryFileListCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FQueryFileListOptionsCallback* CallbackObj = static_cast<FQueryFileListOptionsCallback*>(Data->ClientData);
	check(CallbackObj);

	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreTitleStorage::Internal_OnTitleStorageDeleteCacheCallback(const EOS_TitleStorage_DeleteCacheCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FDeleteCacheCallback* CallbackObj = static_cast<FDeleteCacheCallback*>(Data->ClientData);
	check(CallbackObj);

	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreTitleStorage::Internal_OnTitleStorageReadFileCallback(const EOS_TitleStorage_ReadFileCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FReadFileCallback* CallbackObj = static_cast<FReadFileCallback*>(Data->ClientData);
	check(CallbackObj);

	if (Data->ResultCode == EOS_EResult::EOS_Success)
	{
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		IFileHandle* Handle = PlatformFile.OpenWrite(*m_CurrentRead.LocalFile);

		if (Handle)
		{
			if (Handle->Write(m_CurrentRead.Data.GetData(), m_CurrentRead.Data.Num()))
			{
				LogVerbose("");
			}

			delete Handle;
		}
	}

	m_CurrentRead.Reset();

	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

EOS_TitleStorage_EReadResult UCoreTitleStorage::OnReadFileDataCallback(const EOS_TitleStorage_ReadFileDataCallbackInfo* Data)
{
	LogVerbose("");

	const FReadFileCallback* CallbackObj = static_cast<FReadFileCallback*>(Data->ClientData);
	check(CallbackObj);

	if (Data && CallbackObj)
	{
		return CallbackObj->m_TitleStorage->ReceiveData(Data->Filename, Data->DataChunk, Data->DataChunkLengthBytes, Data->TotalFileSizeBytes);
	}
	else
	{
		return EOS_TitleStorage_EReadResult::EOS_TS_RR_FailRequest;
	}
}

void UCoreTitleStorage::OnFileTransferProgressCallback(const EOS_TitleStorage_FileTransferProgressCallbackInfo* Data)
{
	LogVerbose("Transfered: %d/%d", Data->BytesTransferred, Data->TotalFileSizeBytes);
}

EOS_TitleStorage_EReadResult UCoreTitleStorage::ReceiveData(const FString& Filename, const void* Data, size_t NumBytes, size_t TotalSize)
{
	LogVerbose("");

	if (Data)
	{
		if (m_CurrentRead.CurrentIndex == 0 && m_CurrentRead.TotalSize == 0)
		{
			m_CurrentRead.TotalSize = TotalSize;

			if (m_CurrentRead.TotalSize == 0)
			{
				return EOS_TitleStorage_EReadResult::EOS_TS_RR_ContinueReading;
			}

			m_CurrentRead.Data.SetNum(TotalSize);
		}

		if (m_CurrentRead.TotalSize - m_CurrentRead.CurrentIndex >= NumBytes)
		{
			memcpy(static_cast<void*>(&m_CurrentRead.Data[m_CurrentRead.CurrentIndex]), Data, NumBytes);
			m_CurrentRead.CurrentIndex += NumBytes;

			return EOS_TitleStorage_EReadResult::EOS_TS_RR_ContinueReading;
		}
		else
		{
			LogWarning("Could not receive data: too much of it!");

			return EOS_TitleStorage_EReadResult::EOS_TS_RR_FailRequest;
		}
	}
	else
	{
		LogWarning("Could not receive data: Data pointer is null!");

		return EOS_TitleStorage_EReadResult::EOS_TS_RR_FailRequest;
	}

	return EOS_TitleStorage_EReadResult::EOS_TS_RR_CancelRequest;
}
