/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "TitleStorage/EOSTitleStorageAsyncActions.h"
#include "Core/EOSCorePluginPrivatePCH.h" 
#include "Core/EOSCoreLibrary.h"
#include "Core/EOSCoreLogging.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreTitleStorageQueryFile
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreTitleStorageQueryFile* UEOSCoreTitleStorageQueryFile::EOSTitleStorageQueryFileAsync(UObject* WorldContextObject, FEOSTitleStorageQueryFileOptions Options)
{
	if (UEOSCoreSubsystem::GetTitleStorageHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreTitleStorageQueryFile>();
		AsyncObject->RegisterWithGameInstance(GameInstance);
		AsyncObject->m_Options = Options;
		AsyncObject->m_WorldContextObject = WorldContextObject;

		return AsyncObject;
	}
	else
	{
		LogError("Plugin not initialized!");
	}

	return nullptr;
}

void UEOSCoreTitleStorageQueryFile::Activate()
{
	Super::Activate();
	UCoreTitleStorage::GetTitleStorage(m_WorldContextObject)->EOSTitleStorageQueryFile(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreTitleStorageQueryFileList
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreTitleStorageQueryFileList* UEOSCoreTitleStorageQueryFileList::EOSTitleStorageQueryFileListAsync(UObject* WorldContextObject, FEOSTitleStorageQueryFileListOptions Options)
{
	if (UEOSCoreSubsystem::GetTitleStorageHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreTitleStorageQueryFileList>();
		AsyncObject->RegisterWithGameInstance(GameInstance);
		AsyncObject->m_Options = Options;
		AsyncObject->m_WorldContextObject = WorldContextObject;

		return AsyncObject;
	}
	else
	{
		LogError("Plugin not initialized!");
	}

	return nullptr;
}

void UEOSCoreTitleStorageQueryFileList::Activate()
{
	Super::Activate();
	UCoreTitleStorage::GetTitleStorage(m_WorldContextObject)->EOSTitleStorageQueryFileList(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreTitleStorageReadFile
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreTitleStorageReadFile* UEOSCoreTitleStorageReadFile::EOSTitleStorageReadFileAsync(UObject* WorldContextObject, FEOSTitleStorageReadFileOptions Options)
{
	if (UEOSCoreSubsystem::GetTitleStorageHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreTitleStorageReadFile>();
		AsyncObject->RegisterWithGameInstance(GameInstance);
		AsyncObject->m_Options = Options;
		AsyncObject->m_WorldContextObject = WorldContextObject;

		return AsyncObject;
	}
	else
	{
		LogError("Plugin not initialized!");
	}

	return nullptr;
}

void UEOSCoreTitleStorageReadFile::Activate()
{
	Super::Activate();
	UCoreTitleStorage::GetTitleStorage(m_WorldContextObject)->EOSTitleStorageReadFile(m_WorldContextObject, m_Options, m_Callback);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		UEOSCoreTitleStorageDeleteCache
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
UEOSCoreTitleStorageDeleteCache* UEOSCoreTitleStorageDeleteCache::EOSTitleStorageDeleteCacheAsync(UObject* WorldContextObject, FEOSTitleStorageDeleteCacheOptions Options)
{
	if (UEOSCoreSubsystem::GetTitleStorageHandle(WorldContextObject))
	{
		auto* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
		auto* AsyncObject = NewObject<UEOSCoreTitleStorageDeleteCache>();
		AsyncObject->RegisterWithGameInstance(GameInstance);
		AsyncObject->m_Options = Options;
		AsyncObject->m_WorldContextObject = WorldContextObject;

		return AsyncObject;
	}
	else
	{
		LogError("Plugin not initialized!");
	}

	return nullptr;
}

void UEOSCoreTitleStorageDeleteCache::Activate()
{
	Super::Activate();
	UCoreTitleStorage::GetTitleStorage(m_WorldContextObject)->EOSTitleStorageDeleteCache(m_WorldContextObject, m_Options, m_Callback);
}