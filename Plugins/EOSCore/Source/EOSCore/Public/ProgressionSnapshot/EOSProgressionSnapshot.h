/**
* Copyright (C) 2017-2022 | eelDev AB
*
 official EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "EOSCoreModule.h"
#include "eos_progressionsnapshot_types.h"
#include "ProgressionSnapshot/EOSProgressionSnapshotTypes.h"
#include "EOSProgressionSnapshot.generated.h"

/**
* Progression Snapshots allow you to store player specific game state.
*
* BeginSnapshot must be called to obtain a handle for a given player.
* AddProgression allows you to add Key/Value pairs that represent some state for that player in the game.
* SubmitSnapshot sends all the progression data you have added (via AddProgression) to the service.
* EndSnapshot cleans up internal resources allocated for that snapshot.
* DeleteSnapshot wipes out all data associated with a particular player.
*
* @see EOS_Platform_GetProgressionSnapshotInterface
*/

UCLASS()
class EOSCORE_API UCoreProgressionSnapshot : public UEOSCoreSubsystem
{
	GENERATED_BODY()
public:
	virtual void Deinitialize() override;
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Interfaces", meta = (WorldContext = "WorldContextObject"))
	static UCoreProgressionSnapshot* GetProgressionSnapshot(UObject* WorldContextObject);

public:
	/**
	* Creates a new progression-snapshot resource for a given user.
	*
	* @param Options Object containing properties that identifies the PUID this Snapshot will belong to.
	* @param OutSnapshotId A progression-snapshot identifier output parameter. Use that identifier to reference the snapshot in the other APIs.
	*
	* @return EOS_Success when successful.
	*          EOS_ProgressionSnapshot_SnapshotIdUnavailable when no IDs are available. This is irrecoverable state.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|ProgressionSnapshot", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_ProgressionSnapshot_BeginSnapshot"))
	static EOSResult EOSProgressionSnapshotBeginSnapshot(UObject* WorldContextObject, FEOSProgressionSnapshotBeginSnapshotOptions Options, int32& OutSnapshotId);

	/**
	 * Stores a Key/Value pair in memory for a given snapshot.
	 * If multiple calls happen with the same key, the last invocation wins, overwriting the previous value for that
	 * given key.
	 *
	 * The order in which the Key/Value pairs are added is stored as is for later retrieval/display.
	 * Ideally, you would make multiple calls to AddProgression() followed by a single call to SubmitSnapshot().
	 *
	 * @return EOS_Success when successful; otherwise, EOS_NotFound
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|ProgressionSnapshot", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_ProgressionSnapshot_AddProgression"))
	static EOSResult EOSProgressionSnapshotAddProgression(UObject* WorldContextObject, FEOSProgressionSnapshotAddProgressionOptions Options);

	/**
	 * Saves the previously added Key/Value pairs of a given Snapshot to the service.
	 *
	 * Note: This will overwrite any prior progression data stored with the service that's associated with the user.
	 **/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|ProgressionSnapshot", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_ProgressionSnapshot_SubmitSnapshot"))
	static void EOSProgressionSnapshotSubmitSnapshot(UObject* WorldContextObject, FEOSProgressionSnapshotSubmitSnapshotOptions Options, const FOnSubmitSnapshotCallback& Callback);

	/**
	 * Cleans up and releases resources associated with the given progression snapshot identifier.
	 *
	 * @return EOS_Success when successful; otherwise, EOS_NotFound
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|ProgressionSnapshot", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_ProgressionSnapshot_EndSnapshot"))
	static EOSResult EOSProgressionSnapshotEndSnapshot(UObject* WorldContextObject, FEOSProgressionSnapshotEndSnapshotOptions Options);

	/**
	 * Wipes out all progression data for the given user from the service. However, any previous progression data that haven't
	 * been submitted yet are retained.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|ProgressionSnapshot", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_ProgressionSnapshot_DeleteSnapshot"))
	static void EOSProgressionSnapshotDeleteSnapshot(UObject* WorldContextObject, FEOSProgressionSnapshotDeleteSnapshotOptions Options, const FOnDeleteSnapshotCallback& Callback);
private:
	static void Internal_OnSubmitSnapshotCallback(const EOS_ProgressionSnapshot_SubmitSnapshotCallbackInfo* Data);
	static void Internal_OnDeleteSnapshotCallback(const EOS_ProgressionSnapshot_DeleteSnapshotCallbackInfo* Data);
};
