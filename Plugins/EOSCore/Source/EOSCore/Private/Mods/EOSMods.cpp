/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "Mods/EOSMods.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLogging.h"

UCoreMods* UCoreMods::GetMods(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (GetModsHandle(WorldContextObject))
		{
			if (UWorld* World = WorldContextObject->GetWorld())
			{
				if (UGameInstance* GameInstance = World->GetGameInstance())
				{
					return GameInstance->GetSubsystem<UCoreMods>();
				}
			}	
		}
	}
	return nullptr;
}

void UCoreMods::EOSModsInstallMod(UObject* WorldContextObject, FEOSModsInstallModOptions Options, const FOnInstallModCallback& Callback)
{
	LogVerbose("");

	if (GetModsHandle(WorldContextObject))
	{
		EOS_Mods_InstallModOptions ModsOptions = { };
		ModsOptions.ApiVersion = EOS_MODS_INSTALLMOD_API_LATEST;
		ModsOptions.LocalUserId = Options.LocalUserId;

		const FTCHARToUTF8 ConvertedNamespaceId(*Options.Mod.NamespaceId);
		const FTCHARToUTF8 ConvertedItemId(*Options.Mod.ItemId);
		const FTCHARToUTF8 ConvertedArtifactId(*Options.Mod.ArtifactId);
		const FTCHARToUTF8 ConvertedTitle(*Options.Mod.Title);
		const FTCHARToUTF8 ConvertedVersion(*Options.Mod.Version);
		
		EOS_Mod_Identifier Mod = {};
		Mod.ApiVersion = EOS_MOD_IDENTIFIER_API_LATEST;
		Mod.NamespaceId = ConvertedNamespaceId.Get();
		Mod.ItemId = ConvertedItemId.Get();
		Mod.ArtifactId = ConvertedArtifactId.Get();
		Mod.Title = ConvertedTitle.Get();
		Mod.Version = ConvertedVersion.Get();

		ModsOptions.Mod = &Mod;
		ModsOptions.bRemoveAfterExit = Options.bRemoveAfterExit;

		FInstallModCallback* CallbackObj = new FInstallModCallback(Callback);
		EOS_Mods_InstallMod(GetModsHandle(WorldContextObject), &ModsOptions, CallbackObj, Internal_OnInstallModCallback);
	}
}

void UCoreMods::EOSModsUninstallMod(UObject* WorldContextObject, FEOSModsUninstallModOptions Options, const FOnUninstallModCallback& Callback)
{
	LogVerbose("");

	if (GetModsHandle(WorldContextObject))
	{
		EOS_Mods_UninstallModOptions ModsOptions = { };
		ModsOptions.ApiVersion = EOS_MODS_UNINSTALLMOD_API_LATEST;
		ModsOptions.LocalUserId = Options.LocalUserId;

		EOS_Mod_Identifier Mod = { };
		Mod.ApiVersion = EOS_MOD_IDENTIFIER_API_LATEST;
		
		const FTCHARToUTF8 ConvertedNamespaceId(*Options.Mod.NamespaceId);
		const FTCHARToUTF8 ConvertedItemId(*Options.Mod.ItemId);
		const FTCHARToUTF8 ConvertedArtifactId(*Options.Mod.ArtifactId);
		const FTCHARToUTF8 ConvertedTitle(*Options.Mod.Title);
		const FTCHARToUTF8 ConvertedVersion(*Options.Mod.Version);

		Mod.NamespaceId = ConvertedNamespaceId.Get();
		Mod.ItemId = ConvertedItemId.Get();
		Mod.ArtifactId = ConvertedArtifactId.Get();
		Mod.Title = ConvertedTitle.Get();
		Mod.Version = ConvertedVersion.Get();

		ModsOptions.Mod = &Mod;

		FUninstallModCallback* CallbackObj = new FUninstallModCallback(Callback);
		EOS_Mods_UninstallMod(GetModsHandle(WorldContextObject), &ModsOptions, CallbackObj, Internal_OnUninstallModCallback);
	}
}

void UCoreMods::EOSModsEnumerateMods(UObject* WorldContextObject, FEOSModsEnumerateModsOptions Options, const FOnEnumerateModsCallback& Callback)
{
	LogVerbose("");

	if (GetModsHandle(WorldContextObject))
	{
		EOS_Mods_EnumerateModsOptions ModsOptions = { };
		ModsOptions.ApiVersion = EOS_MODS_ENUMERATEMODS_API_LATEST;
		ModsOptions.LocalUserId = Options.LocalUserId;
		ModsOptions.Type = static_cast<EOS_EModEnumerationType>(Options.Type);

		FEnumerateModsCallback* CallbackObj = new FEnumerateModsCallback(Callback);
		EOS_Mods_EnumerateMods(GetModsHandle(WorldContextObject), &ModsOptions, CallbackObj, Internal_OnEnumerateModsCallback);
	}
}

EOSResult UCoreMods::EOSModsCopyModInfo(UObject* WorldContextObject, FEOSModsCopyModInfoOptions Options, FEOSModsModInfo& OutEnumeratedMods)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetModsHandle(WorldContextObject))
	{
		EOS_Mods_CopyModInfoOptions ModsOptions = { };
		ModsOptions.ApiVersion = EOS_MODS_COPYMODINFO_API_LATEST;
		ModsOptions.LocalUserId = Options.LocalUserId;
		ModsOptions.Type = static_cast<EOS_EModEnumerationType>(Options.Type);

		EOS_Mods_ModInfo* ModInfo = NULL;

		Result = EOSHelpers::ToEOSCoreResult(EOS_Mods_CopyModInfo(GetModsHandle(WorldContextObject), &ModsOptions, &ModInfo));

		if (Result == EOSResult::EOS_Success)
		{
			OutEnumeratedMods.ApiVersion = { EOS_MODS_COPYMODINFO_API_LATEST };

			for (int32 i = 0; i < ModInfo->ModsCount; i++)
			{
				OutEnumeratedMods.Mods.Add(ModInfo->Mods[i]);
			}

			OutEnumeratedMods.Type = static_cast<EEOSEModEnumerationType>(ModInfo->Type);

			EOS_Mods_ModInfo_Release(ModInfo);
		}
	}

	return Result;
}

void UCoreMods::EOSModsUpdateMod(UObject* WorldContextObject, FEOSModsUpdateModOptions Options, const FOnUpdateModCallback& Callback)
{
	LogVerbose("");

	if (GetModsHandle(WorldContextObject))
	{
		EOS_Mods_UpdateModOptions ModsOptions = { };
		ModsOptions.ApiVersion = EOS_MODS_UPDATEMOD_API_LATEST;
		ModsOptions.LocalUserId = Options.LocalUserId;

		EOS_Mod_Identifier Mod = { };
		Mod.ApiVersion = EOS_MOD_IDENTIFIER_API_LATEST;

		const FTCHARToUTF8 ConvertedNamespaceId(*Options.Mod.NamespaceId);
		const FTCHARToUTF8 ConvertedItemId(*Options.Mod.ItemId);
		const FTCHARToUTF8 ConvertedArtifactId(*Options.Mod.ArtifactId);
		const FTCHARToUTF8 ConvertedTitle(*Options.Mod.Title);
		const FTCHARToUTF8 ConvertedVersion(*Options.Mod.Version);

		Mod.NamespaceId = ConvertedNamespaceId.Get();
		Mod.ItemId = ConvertedItemId.Get();
		Mod.ArtifactId = ConvertedArtifactId.Get();
		Mod.Title = ConvertedTitle.Get();
		Mod.Version = ConvertedVersion.Get();

		ModsOptions.Mod = &Mod;

		FUpdateModCallback* CallbackObj = new FUpdateModCallback(Callback);
		EOS_Mods_UpdateMod(GetModsHandle(WorldContextObject), &ModsOptions, CallbackObj, Internal_OnUpdateModCallback);
	}
}

void UCoreMods::Internal_OnInstallModCallback(const EOS_Mods_InstallModCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FInstallModCallback* CallbackObj = static_cast<FInstallModCallback*>(Data->ClientData);
	check(CallbackObj);

	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreMods::Internal_OnUninstallModCallback(const EOS_Mods_UninstallModCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FUninstallModCallback* CallbackObj = static_cast<FUninstallModCallback*>(Data->ClientData);
	check(CallbackObj);

	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreMods::Internal_OnEnumerateModsCallback(const EOS_Mods_EnumerateModsCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FEnumerateModsCallback* CallbackObj = static_cast<FEnumerateModsCallback*>(Data->ClientData);
	check(CallbackObj);

	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreMods::Internal_OnUpdateModCallback(const EOS_Mods_UpdateModCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FUpdateModCallback* CallbackObj = static_cast<FUpdateModCallback*>(Data->ClientData);
	check(CallbackObj);

	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}
