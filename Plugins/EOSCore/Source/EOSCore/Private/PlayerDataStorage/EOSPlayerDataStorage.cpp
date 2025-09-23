/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "PlayerDataStorage/EOSPlayerDataStorage.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLibrary.h"
#include "Core/EOSCoreLogging.h"

static size_t MaxChunkSize = 4096;

FEOSHPlayerDataStorageFileTransferRequest UCorePlayerDataStorage::s_FileTransferHandleRead = nullptr;
FEOSHPlayerDataStorageFileTransferRequest UCorePlayerDataStorage::s_FileTransferHandleWrite = nullptr;
FTransferInProgress UCorePlayerDataStorage::s_CurrentRead;
FTransferInProgress UCorePlayerDataStorage::s_CurrentWrite;

void UCorePlayerDataStorage::Deinitialize()
{
	LogVerbose("");
}

UCorePlayerDataStorage* UCorePlayerDataStorage::GetPlayerDataStorage(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (GetPlayerDataStorageHandle(WorldContextObject))
		{
			if (UWorld* World = WorldContextObject->GetWorld())
			{
				if (UGameInstance* GameInstance = World->GetGameInstance())
				{
					return GameInstance->GetSubsystem<UCorePlayerDataStorage>();
				}
			}	
		}
	}
	return nullptr;
}

void UCorePlayerDataStorage::EOSPlayerDataStorageQueryFile(UObject* WorldContextObject, FEOSPlayerDataStorageQueryFileOptions QueryFileOptions, const FOnQueryFileCompleteCallback& Callback)
{
	LogVerbose("");

	if (GetPlayerDataStorageHandle(WorldContextObject))
	{
		const FTCHARToUTF8 FileNameAnsi(*QueryFileOptions.Filename);

		EOS_PlayerDataStorage_QueryFileOptions PlayerDataStorageOptions = { };
		PlayerDataStorageOptions.ApiVersion = QueryFileOptions.ApiVersion;
		PlayerDataStorageOptions.LocalUserId = QueryFileOptions.LocalUserId;
		PlayerDataStorageOptions.Filename = FileNameAnsi.Get();

		FFileCompleteCallback* CallbackObj = new FFileCompleteCallback(Callback);
		EOS_PlayerDataStorage_QueryFile(GetPlayerDataStorageHandle(WorldContextObject), &PlayerDataStorageOptions, CallbackObj, Internal_OnQueryFileCallback);
	}
}

void UCorePlayerDataStorage::EOSPlayerDataStorageQueryFileList(UObject* WorldContextObject, FEOSPlayerDataStorageQueryFileListOptions QueryFileListOptions, const FOnQueryFileListCompleteCallback& Callback)
{
	LogVerbose("");

	if (GetPlayerDataStorageHandle(WorldContextObject))
	{
		EOS_PlayerDataStorage_QueryFileListOptions PlayerDataStorageOptions = { };
		PlayerDataStorageOptions.ApiVersion = QueryFileListOptions.ApiVersion;
		PlayerDataStorageOptions.LocalUserId = QueryFileListOptions.LocalUserId;

		FFileListCompleteCallback* CallbackObj = new FFileListCompleteCallback(Callback);
		EOS_PlayerDataStorage_QueryFileList(GetPlayerDataStorageHandle(WorldContextObject), &PlayerDataStorageOptions, CallbackObj, Internal_OnQueryFileListCallback);
	}
}

EOSResult UCorePlayerDataStorage::EOSPlayerDataStorageCopyFileMetadataByFilename(UObject* WorldContextObject, FEOSPlayerDataStorageCopyFileMetadataByFilenameOptions CopyFileMetadataOptions, FEOSPlayerDataStorageFileMetadata& OutMetadata)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetPlayerDataStorageHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedFileName(*CopyFileMetadataOptions.Filename);
		
		EOS_PlayerDataStorage_CopyFileMetadataByFilenameOptions PlayerDataStorageOptions = { };
		PlayerDataStorageOptions.ApiVersion = CopyFileMetadataOptions.ApiVersion;
		PlayerDataStorageOptions.LocalUserId = CopyFileMetadataOptions.LocalUserId;
		PlayerDataStorageOptions.Filename = ConvertedFileName.Get();

		EOS_PlayerDataStorage_FileMetadata* FileMetaData = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_PlayerDataStorage_CopyFileMetadataByFilename(GetPlayerDataStorageHandle(WorldContextObject), &PlayerDataStorageOptions, &FileMetaData));

		if (Result == EOSResult::EOS_Success)
		{
			OutMetadata = *FileMetaData;
		}
	}

	return Result;
}

EOSResult UCorePlayerDataStorage::EOSPlayerDataStorageGetFileMetadataCount(UObject* WorldContextObject, FEOSPlayerDataStorageGetFileMetadataCountOptions GetFileMetadataCountOptions, int32& OutFileMetadataCount)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetPlayerDataStorageHandle(WorldContextObject))
	{
		EOS_PlayerDataStorage_GetFileMetadataCountOptions PlayerDataStorageOptions = { };
		PlayerDataStorageOptions.ApiVersion = GetFileMetadataCountOptions.ApiVersion;
		PlayerDataStorageOptions.LocalUserId = GetFileMetadataCountOptions.LocalUserId;

		int32_t OutFileMetaDataCount = 0;

		Result = EOSHelpers::ToEOSCoreResult(EOS_PlayerDataStorage_GetFileMetadataCount(GetPlayerDataStorageHandle(WorldContextObject), &PlayerDataStorageOptions, &OutFileMetaDataCount));

		if (Result == EOSResult::EOS_Success)
		{
			OutFileMetadataCount = OutFileMetaDataCount;
		}
	}

	return Result;
}

EOSResult UCorePlayerDataStorage::EOSPlayerDataStorageCopyFileMetadataAtIndex(UObject* WorldContextObject, FEOSPlayerDataStorageCopyFileMetadataAtIndexOptions CopyFileMetadataOptions, FEOSPlayerDataStorageFileMetadata& OutMetadata)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetPlayerDataStorageHandle(WorldContextObject))
	{
		EOS_PlayerDataStorage_CopyFileMetadataAtIndexOptions PlayerDataStorageOptions = { };
		PlayerDataStorageOptions.ApiVersion = CopyFileMetadataOptions.ApiVersion;
		PlayerDataStorageOptions.LocalUserId = CopyFileMetadataOptions.LocalUserId;
		PlayerDataStorageOptions.Index = static_cast<uint32_t>(CopyFileMetadataOptions.Index);

		EOS_PlayerDataStorage_FileMetadata* FileMetaData = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_PlayerDataStorage_CopyFileMetadataAtIndex(GetPlayerDataStorageHandle(WorldContextObject), &PlayerDataStorageOptions, &FileMetaData));

		if (Result == EOSResult::EOS_Success)
		{
			OutMetadata = *FileMetaData;
		}
	}

	return Result;
}

void UCorePlayerDataStorage::EOSPlayerDataStorageDuplicateFile(UObject* WorldContextObject, FEOSPlayerDataStorageDuplicateFileOptions DuplicateOptions, const FOnDuplicateFileCompleteCallback& Callback)
{
	LogVerbose("");

	if (GetPlayerDataStorageHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedSource(*DuplicateOptions.SourceFilename);
		const FTCHARToUTF8 ConvertedDestination(*DuplicateOptions.DestinationFilename);

		EOS_PlayerDataStorage_DuplicateFileOptions PlayerDataStorageOptions = { };
		PlayerDataStorageOptions.ApiVersion = DuplicateOptions.ApiVersion;
		PlayerDataStorageOptions.LocalUserId = DuplicateOptions.LocalUserId;
		PlayerDataStorageOptions.DestinationFilename = ConvertedDestination.Get();
		PlayerDataStorageOptions.SourceFilename = ConvertedSource.Get();

		FDuplicateFileCompleteCallback* CallbackObj = new FDuplicateFileCompleteCallback(Callback);
		EOS_PlayerDataStorage_DuplicateFile(GetPlayerDataStorageHandle(WorldContextObject), &PlayerDataStorageOptions, CallbackObj, Internal_OnDuplicateFileCallback);
	}
}

void UCorePlayerDataStorage::EOSPlayerDataStorageDeleteFile(UObject* WorldContextObject, FEOSPlayerDataStorageDeleteFileOptions DeleteOptions, const FOnDeleteFileCompleteCallback& Callback)
{
	LogVerbose("");

	if (GetPlayerDataStorageHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedFileName(*DeleteOptions.Filename);

		EOS_PlayerDataStorage_DeleteFileOptions PlayerDataStorageOptions = { };
		PlayerDataStorageOptions.ApiVersion = DeleteOptions.ApiVersion;
		PlayerDataStorageOptions.LocalUserId = DeleteOptions.LocalUserId;
		PlayerDataStorageOptions.Filename = ConvertedFileName.Get();

		FDeleteFileCompleteCallback* CallbackObj = new FDeleteFileCompleteCallback(Callback);
		EOS_PlayerDataStorage_DeleteFile(GetPlayerDataStorageHandle(WorldContextObject), &PlayerDataStorageOptions, CallbackObj, Internal_OnDeleteFileCallback);
	}
}

void UCorePlayerDataStorage::EOSPlayerDataStorageReadFile(UObject* WorldContextObject, FEOSPlayerDataStorageReadFileOptions ReadOptions, const FOnReadFileCompleteCallback& Callback)
{
	LogVerbose("");

	if (GetPlayerDataStorageHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedRemoteFile(*ReadOptions.RemoteFile);

		EOS_PlayerDataStorage_ReadFileOptions PlayerDataStorageOptions = { };
		PlayerDataStorageOptions.ApiVersion = ReadOptions.ApiVersion;
		PlayerDataStorageOptions.LocalUserId = ReadOptions.LocalUserId;
		PlayerDataStorageOptions.Filename = ConvertedRemoteFile.Get();
		PlayerDataStorageOptions.ReadChunkLengthBytes = static_cast<uint32_t>(MaxChunkSize);
		PlayerDataStorageOptions.ReadFileDataCallback = OnFileDataReceived;
		PlayerDataStorageOptions.FileTransferProgressCallback = OnFileTransferProgressUpdated;

		s_CurrentRead.LocalFile = ReadOptions.LocalFile;
		FReadFileCompleteCallback* CallbackObj = new FReadFileCompleteCallback(this, Callback);
		s_FileTransferHandleRead = EOS_PlayerDataStorage_ReadFile(GetPlayerDataStorageHandle(WorldContextObject), &PlayerDataStorageOptions, CallbackObj, Internal_OnReadFileCallback);
	}
}

void UCorePlayerDataStorage::EOSPlayerDataStorageWriteFile(UObject* WorldContextObject, FEOSPlayerDataStorageWriteFileOptions WriteOptions, const FOnWriteFileCompleteCallback& Callback)
{
	LogVerbose("");

	if (GetPlayerDataStorageHandle(WorldContextObject))
	{
		const FTCHARToUTF8 ConvertedRemoteFile(*WriteOptions.RemoteFile);
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

		EOS_PlayerDataStorage_WriteFileOptions PlayerDataStorageOptions = { };
		PlayerDataStorageOptions.ApiVersion = WriteOptions.ApiVersion;
		PlayerDataStorageOptions.LocalUserId = WriteOptions.LocalUserId;
		PlayerDataStorageOptions.ChunkLengthBytes = static_cast<uint32_t>(MaxChunkSize);
		PlayerDataStorageOptions.WriteFileDataCallback = OnFileDataSend;
		PlayerDataStorageOptions.FileTransferProgressCallback = OnFileTransferProgressUpdated;
		
		if (!PlatformFile.FileExists(*WriteOptions.LocalFile))
		{
			LogError("File does not exist!");
		}
		else
		{
			// only set the remote file if local file exists to produce an error
			PlayerDataStorageOptions.Filename = ConvertedRemoteFile.Get();
			
			IFileHandle* Handle = PlatformFile.OpenRead(*WriteOptions.LocalFile);

			if (Handle && Handle->Size() > 0)
			{
				s_CurrentWrite.CurrentIndex = 0;

				TArray<uint8> Data;
				Data.SetNum(Handle->Size());

				s_CurrentWrite.Data.SetNum(Handle->Size());
				s_CurrentWrite.TotalSize = Handle->Size();
				Handle->Read(Data.GetData(), Handle->Size());

				memcpy(static_cast<void*>(&s_CurrentWrite.Data[0]), static_cast<const void*>(&Data[0]), Data.Num());
			}
			if (Handle)
			{
				delete Handle;
			}
		}

		FWriteFileCompleteCallback* Data = new FWriteFileCompleteCallback(this, Callback);
		s_FileTransferHandleWrite = EOS_PlayerDataStorage_WriteFile(GetPlayerDataStorageHandle(WorldContextObject), &PlayerDataStorageOptions, Data, Internal_OnWriteFileCallback);
	}
}

EOSResult UCorePlayerDataStorage::EOSPlayerDataStorageDeleteCache(UObject* WorldContextObject, FEOSPlayerDataStorageDeleteCacheOptions Options, const FOnDeleteCacheCompleteCallback& Callback)
{
	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetPlayerDataStorageHandle(WorldContextObject))
	{
		EOS_PlayerDataStorage_DeleteCacheOptions DeleteCacheOptions;
		DeleteCacheOptions.ApiVersion = Options.ApiVersion;
		DeleteCacheOptions.LocalUserId = Options.LocalUserId;

		FDeleteCacheCompleteCallback* CallbackObj = new FDeleteCacheCompleteCallback(Callback);

		Result = EOSHelpers::ToEOSCoreResult(EOS_PlayerDataStorage_DeleteCache(GetPlayerDataStorageHandle(WorldContextObject), &DeleteCacheOptions, nullptr, Internal_OnDeleteCacheCallback));
	}

	return Result;
}

EOSResult UCorePlayerDataStorage::EOSPlayerDataStorageFileTransferRequestGetFileRequestState(UObject* WorldContextObject, const FEOSHPlayerDataStorageFileTransferRequest& Handle)
{
	LogVerbose("");
	
	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetPlayerDataStorageHandle(WorldContextObject))
	{
		Result = EOSHelpers::ToEOSCoreResult(EOS_PlayerDataStorageFileTransferRequest_GetFileRequestState(Handle));
	}

	return Result;
}

EOSResult UCorePlayerDataStorage::EOSPlayerDataStorageFileTransferRequestGetFilename(UObject* WorldContextObject, const FEOSHPlayerDataStorageFileTransferRequest& Handle, int32 FilenameStringBufferSizeBytes, FString& OutStringBuffer)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetPlayerDataStorageHandle(WorldContextObject))
	{
		char Buffer[1024];
		int32 Size = 0;

		Result = EOSHelpers::ToEOSCoreResult(EOS_PlayerDataStorageFileTransferRequest_GetFilename(Handle, FilenameStringBufferSizeBytes, Buffer, &Size));

		if (Result == EOSResult::EOS_Success)
		{
			OutStringBuffer = Buffer;
		}
	}

	return Result;
}

EOSResult UCorePlayerDataStorage::EOSPlayerDataStorageFileTransferRequestCancelRequest(UObject* WorldContextObject, const FEOSHPlayerDataStorageFileTransferRequest& Handle)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetPlayerDataStorageHandle(WorldContextObject))
	{
		Result = EOSHelpers::ToEOSCoreResult(EOS_PlayerDataStorageFileTransferRequest_CancelRequest(Handle));
	}

	return Result;
}

void UCorePlayerDataStorage::EOSPlayerDataStorageFileTransferRequestRelease(UObject* WorldContextObject, const FEOSHPlayerDataStorageFileTransferRequest& Handle)
{
	LogVerbose("");

	if (GetPlayerDataStorageHandle(WorldContextObject))
	{
		EOS_PlayerDataStorageFileTransferRequest_Release(Handle);
	}
}

EOS_PlayerDataStorage_EReadResult UCorePlayerDataStorage::OnFileDataReceived(const EOS_PlayerDataStorage_ReadFileDataCallbackInfo* Data)
{
	LogVerbose("");

	const FReadFileCompleteCallback* CallbackObj = static_cast<FReadFileCompleteCallback*>(Data->ClientData);
	check(CallbackObj);
	
	if (Data && CallbackObj)
	{
		return CallbackObj->m_PlayerDataStorage->ReceiveData(Data->Filename, Data->DataChunk, Data->DataChunkLengthBytes, Data->TotalFileSizeBytes);
	}
	else
	{
		return EOS_PlayerDataStorage_EReadResult::EOS_RR_FailRequest;
	}
}

EOS_PlayerDataStorage_EReadResult UCorePlayerDataStorage::ReceiveData(const FString& Filename, const void* Data, size_t NumBytes, size_t TotalSize)
{
	LogVerbose("");

	if (Data)
	{
		// initialize struct
		if (s_CurrentRead.CurrentIndex == 0 && s_CurrentRead.TotalSize == 0)
		{
			s_CurrentRead.TotalSize = TotalSize;

			if (s_CurrentRead.TotalSize == 0)
			{
				return EOS_PlayerDataStorage_EReadResult::EOS_RR_ContinueReading;
			}

			s_CurrentRead.Data.SetNum(TotalSize);
		}

		if (s_CurrentRead.TotalSize - s_CurrentRead.CurrentIndex >= NumBytes)
		{
			memcpy(static_cast<void*>(&s_CurrentRead.Data[s_CurrentRead.CurrentIndex]), Data, NumBytes);
			s_CurrentRead.CurrentIndex += NumBytes;

			return EOS_PlayerDataStorage_EReadResult::EOS_RR_ContinueReading;
		}
		else
		{
			LogWarning("Could not receive data: too much of it!");

			return EOS_PlayerDataStorage_EReadResult::EOS_RR_FailRequest;
		}
	}
	else
	{
		LogWarning("Could not receive data: Data pointer is null!");

		return EOS_PlayerDataStorage_EReadResult::EOS_RR_FailRequest;
	}

	return EOS_PlayerDataStorage_EReadResult::EOS_RR_CancelRequest;
}

EOS_PlayerDataStorage_EWriteResult UCorePlayerDataStorage::SendData(const FString& Filename, void* Data, uint32_t* BytesWritten)
{
	LogVerbose("");

	if (Data && BytesWritten)
	{
		if (s_CurrentWrite.Done())
		{
			*BytesWritten = 0;

			return EOS_PlayerDataStorage_EWriteResult::EOS_WR_CompleteRequest;
		}

		const size_t BytesToWrite = std::min(MaxChunkSize, s_CurrentWrite.TotalSize - s_CurrentWrite.CurrentIndex);

		if (BytesToWrite > 0)
		{
			memcpy(Data, static_cast<const void*>(&s_CurrentWrite.Data[s_CurrentWrite.CurrentIndex]), BytesToWrite);
		}

		*BytesWritten = static_cast<uint32_t>(BytesToWrite);

		s_CurrentWrite.CurrentIndex += BytesToWrite;

		if (s_CurrentWrite.Done())
		{
			return EOS_PlayerDataStorage_EWriteResult::EOS_WR_CompleteRequest;
		}
		else
		{
			return EOS_PlayerDataStorage_EWriteResult::EOS_WR_ContinueWriting;
		}
	}
	else
	{
		LogError("Could not send data: pointer is null!");

		return EOS_PlayerDataStorage_EWriteResult::EOS_WR_FailRequest;
	}
}

EOS_PlayerDataStorage_EWriteResult UCorePlayerDataStorage::OnFileDataSend(const EOS_PlayerDataStorage_WriteFileDataCallbackInfo* Data, void* OutDataBuffer, uint32_t* OutDataWritten)
{
	LogVerbose("");

	FWriteFileCompleteCallback* CallbackObj = static_cast<FWriteFileCompleteCallback*>(Data->ClientData);
	check(CallbackObj);
	
	if (Data)
	{
		if (CallbackObj)
		{
			return CallbackObj->m_PlayerDataStorage->SendData(Data->Filename, OutDataBuffer, OutDataWritten);
		}
	}

	return EOS_PlayerDataStorage_EWriteResult::EOS_WR_FailRequest;
}

void UCorePlayerDataStorage::OnFileTransferProgressUpdated(const EOS_PlayerDataStorage_FileTransferProgressCallbackInfo* Data)
{
	LogVerbose("");
}

void UCorePlayerDataStorage::Internal_OnQueryFileCallback(const EOS_PlayerDataStorage_QueryFileCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FFileCompleteCallback* CallbackObj = static_cast<FFileCompleteCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCorePlayerDataStorage::Internal_OnQueryFileListCallback(const EOS_PlayerDataStorage_QueryFileListCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FFileListCompleteCallback* CallbackObj = static_cast<FFileListCompleteCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCorePlayerDataStorage::Internal_OnDuplicateFileCallback(const EOS_PlayerDataStorage_DuplicateFileCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FDuplicateFileCompleteCallback* CallbackObj = static_cast<FDuplicateFileCompleteCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCorePlayerDataStorage::Internal_OnDeleteFileCallback(const EOS_PlayerDataStorage_DeleteFileCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FDeleteFileCompleteCallback* CallbackObj = static_cast<FDeleteFileCompleteCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCorePlayerDataStorage::Internal_OnReadFileCallback(const EOS_PlayerDataStorage_ReadFileCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	if (Data->ResultCode == EOS_EResult::EOS_Success)
	{
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		IFileHandle* Handle = PlatformFile.OpenWrite(*s_CurrentRead.LocalFile);

		if (Handle)
		{
			if (Handle->Write(s_CurrentRead.Data.GetData(), s_CurrentRead.Data.Num()))
			{
				LogVerbose("");
			}

			delete Handle;
		}
	}
	
	EOS_PlayerDataStorageFileTransferRequest_CancelRequest(s_FileTransferHandleRead);
	s_CurrentRead.Reset();

	const FReadFileCompleteCallback* CallbackObj = static_cast<FReadFileCompleteCallback*>(Data->ClientData);
	check(CallbackObj);

	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCorePlayerDataStorage::Internal_OnWriteFileCallback(const EOS_PlayerDataStorage_WriteFileCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FWriteFileCompleteCallback* CallbackObj = static_cast<FWriteFileCompleteCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		EOS_PlayerDataStorageFileTransferRequest_CancelRequest(s_FileTransferHandleWrite);
		s_CurrentWrite.Reset();

		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;

	}
}

void UCorePlayerDataStorage::Internal_OnDeleteCacheCallback(const EOS_PlayerDataStorage_DeleteCacheCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FDeleteCacheCompleteCallback* CallbackObj = static_cast<FDeleteCacheCompleteCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}
