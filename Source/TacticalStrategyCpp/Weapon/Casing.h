// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Casing.generated.h"

UCLASS()
class TACTICALSTRATEGYCPP_API ACasing : public AActor
{
	GENERATED_BODY()

public:
	ACasing();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);

	// Sets a timer to destroy the casing after it drops
	void DestroyAfter();

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CasingMesh;

	// Impulse applied to the casing when ejected
	UPROPERTY(EditAnywhere, Category="Casing Properties", meta = (ClampMin = "0.0", ClampMax = "10.0"))
	float ShellEjectionImpulse = 4;

	// Delay time before the casing is destroyed after being ejected
	UPROPERTY(EditAnywhere, Category="Casing Properties", meta = (ClampMin = "0.0", ClampMax = "15.0"))
	float ShellDestroyTime = 3;

	// Timer handle for casing destruction
	FTimerHandle AfterDropping;

	// Sound played when the casing hits a surface
	UPROPERTY(EditAnywhere, Category="Sound")
	class USoundCue* ShellSound;
};
