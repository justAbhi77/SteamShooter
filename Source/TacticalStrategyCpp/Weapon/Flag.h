// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Flag.generated.h"

UCLASS()
class TACTICALSTRATEGYCPP_API AFlag : public AWeapon
{
	GENERATED_BODY()

public:
	AFlag();

	// Function to handle dropping the flag
	virtual void Dropped() override;

	// Resets the flag to its initial state
	void ResetFlag();

protected:
	virtual void BeginPlay() override;

	// Function to handle actions when flag is equipped
	virtual void OnEquipped() override;

	// Function to handle actions when flag is dropped
	virtual void OnDropped() override;

private:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* FlagMesh;

	// Initial transform to reset flag position
	FTransform InitialTransform;

public:
	FORCEINLINE FTransform GetInitialTransform() const { return InitialTransform; }

	// Returns the static mesh for the flag (override from Weapon)
	virtual UStaticMeshComponent* GetStaticWeaponMesh() const override { return FlagMesh; }
};
