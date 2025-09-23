/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "PlayerDataStorage/EOSPlayerDataStorageAsyncActions.h"
#include "PlayerDataStorage/EOSPlayerDataStorage.h"
#include "Core/EOSCorePluginPrivatePCH.h" 
#include "Core/EOSCoreLibrary.h"
#include "Core/EOSCoreLogging.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCorePlayerDataStorageQueryFile
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCorePlayerDataStorageQueryFile* UEOSCorePlayerDataStorageQueryFile::EOSPlayerDataStorageQueryFileAsync(UObject* WorldContextObject, FEOSPlayerDataStorageQueryFileOptions QueryFileOptions)
{
	if (UEOSCoreSubsystem::GetPlayerDataStorageHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCorePlayerDataStorageQueryFile>();
		AsyncObject->RegisterWithGameInstance(GameInstance);
		AsyncObject->m_Options = QueryFileOptions;
		AsyncObject->m_WorldContextObject = WorldContextObject;

		return AsyncObject;
	}
	else
	{
		LogError("Plugin not initialized!");
	}

	return nullptr;
}

void UEOSCorePlayerDataStorageQueryFile::Activate()
{
	Super::Activate();
	UCorePlayerDataStorage::GetPlayerDataStorage(m_WorldContextObject)->EOSPlayerDataStorageQueryFile(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCorePlayerDataStorageQueryFileList
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCorePlayerDataStorageQueryFileList* UEOSCorePlayerDataStorageQueryFileList::EOSPlayerDataStorageQueryFileListAsync(UObject* WorldContextObject, FEOSPlayerDataStorageQueryFileListOptions QueryFileListOptions)
{
	if (UEOSCoreSubsystem::GetPlayerDataStorageHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCorePlayerDataStorageQueryFileList>();
		AsyncObject->RegisterWithGameInstance(GameInstance);
		AsyncObject->m_Options = QueryFileListOptions;
		AsyncObject->m_WorldContextObject = WorldContextObject;

		return AsyncObject;
	}
	else
	{
		LogError("Plugin not initialized!");
	}

	return nullptr;
}

void UEOSCorePlayerDataStorageQueryFileList::Activate()
{
	Super::Activate();
	UCorePlayerDataStorage::GetPlayerDataStorage(m_WorldContextObject)->EOSPlayerDataStorageQueryFileList(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCorePlayerDataStorageDuplicateFile
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCorePlayerDataStorageDuplicateFile* UEOSCorePlayerDataStorageDuplicateFile::EOSPlayerDataStorageDuplicateFileAsync(UObject* WorldContextObject, FEOSPlayerDataStorageDuplicateFileOptions DuplicateOptions)
{
	if (UEOSCoreSubsystem::GetPlayerDataStorageHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCorePlayerDataStorageDuplicateFile>();
		AsyncObject->RegisterWithGameInstance(GameInstance);
		AsyncObject->m_Options = DuplicateOptions;
		AsyncObject->m_WorldContextObject = WorldContextObject;

		return AsyncObject;
	}
	else
	{
		LogError("Plugin not initialized!");
	}

	return nullptr;
}

void UEOSCorePlayerDataStorageDuplicateFile::Activate()
{
	Super::Activate();
	UCorePlayerDataStorage::GetPlayerDataStorage(m_WorldContextObject)->EOSPlayerDataStorageDuplicateFile(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCorePlayerDataStorageDeleteFile
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCorePlayerDataStorageDeleteFile* UEOSCorePlayerDataStorageDeleteFile::EOSPlayerDataStorageDeleteFileAsync(UObject* WorldContextObject, FEOSPlayerDataStorageDeleteFileOptions DeleteOptions)
{
	if (UEOSCoreSubsystem::GetPlayerDataStorageHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCorePlayerDataStorageDeleteFile>();
		AsyncObject->RegisterWithGameInstance(GameInstance);
		AsyncObject->m_Options = DeleteOptions;
		AsyncObject->m_WorldContextObject = WorldContextObject;

		return AsyncObject;
	}
	else
	{
		LogError("Plugin not initialized!");
	}

	return nullptr;
}

void UEOSCorePlayerDataStorageDeleteFile::Activate()
{
	Super::Activate();
	UCorePlayerDataStorage::GetPlayerDataStorage(m_WorldContextObject)->EOSPlayerDataStorageDeleteFile(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCorePlayerDataStorageReadFile
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCorePlayerDataStorageReadFile* UEOSCorePlayerDataStorageReadFile::EOSPlayerDataStorageReadFileAsync(UObject* WorldContextObject, FEOSPlayerDataStorageReadFileOptions m_ReadOptions)
{
	if (UEOSCoreSubsystem::GetPlayerDataStorageHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCorePlayerDataStorageReadFile>();
		AsyncObject->RegisterWithGameInstance(GameInstance);
		AsyncObject->m_Options = m_ReadOptions;
		AsyncObject->m_WorldContextObject = WorldContextObject;

		return AsyncObject;
	}
	else
	{
		LogError("Plugin not initialized!");
	}

	return nullptr;
}

void UEOSCorePlayerDataStorageReadFile::Activate()
{
	Super::Activate();
	UCorePlayerDataStorage::GetPlayerDataStorage(m_WorldContextObject)->EOSPlayerDataStorageReadFile(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCorePlayerDataStorageWriteFile
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCorePlayerDataStorageWriteFile* UEOSCorePlayerDataStorageWriteFile::EOSPlayerDataStorageWriteFileAsync(UObject* WorldContextObject, FEOSPlayerDataStorageWriteFileOptions WriteOptions)
{
	if (UEOSCoreSubsystem::GetPlayerDataStorageHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCorePlayerDataStorageWriteFile>();
		AsyncObject->RegisterWithGameInstance(GameInstance);
		AsyncObject->m_Options = WriteOptions;
		AsyncObject->m_WorldContextObject = WorldContextObject;

		return AsyncObject;
	}
	else
	{
		LogError("Plugin not initialized!");
	}

	return nullptr;
}

void UEOSCorePlayerDataStorageWriteFile::Activate()
{
	Super::Activate();
	UCorePlayerDataStorage::GetPlayerDataStorage(m_WorldContextObject)->EOSPlayerDataStorageWriteFile(m_WorldContextObject, m_Options, m_Callback);
}
