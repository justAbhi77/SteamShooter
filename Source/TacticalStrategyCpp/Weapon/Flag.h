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

	virtual void Dropped() override;

	void ResetFlag();

protected:
	virtual void BeginPlay() override;

	virtual void OnEquipped() override;
	
	virtual void OnDropped() override;

private:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* FlagMesh;

	FTransform InitialTransform;

public:
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE FTransform GetInitialTransform() const { return InitialTransform; }

	virtual UStaticMeshComponent* GetStaticWeaponMesh() const override { return FlagMesh; }
};
