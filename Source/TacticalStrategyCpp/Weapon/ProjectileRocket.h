// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileRocket.generated.h"

UCLASS()
class TACTICALSTRATEGYCPP_API AProjectileRocket : public AProjectile
{
	GENERATED_BODY()

public:
	AProjectileRocket();

protected:
	virtual void BeginPlay() override;

	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit) override;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* TrailSystem;

	void DestroyTimerFinished();

	virtual void Destroyed() override;

	UPROPERTY()
	class UNiagaraComponent* TrailSystemComponent;

	UPROPERTY(EditAnywhere)
	USoundCue* ProjectileLoop;

	UPROPERTY(EditAnywhere)
	USoundAttenuation* LoopingSoundAttenuation;
	
	UPROPERTY()
	UAudioComponent* ProjectileLoopComponent;

	UPROPERTY(VisibleAnywhere)
	class URocketMovementComponent* RocketMovementComponent;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* RocketMesh;

	FTimerHandle DestroyTimer;

	/**
	 * Tie after which to destroy rocket so that trail dissipates
	 */
	UPROPERTY(EditAnywhere)
	float DestroyTime;
	
public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category=Rocket, meta=(AllowPrivateAccess= "true"))
	float InnerRadius;
	
	UPROPERTY(EditAnywhere, Category=Rocket, meta=(AllowPrivateAccess= "true"))
	float OuterRadius;

	/*
	 * The fallOf for the damage applied in-between the inner and outer radius  
	 */
	UPROPERTY(EditAnywhere, Category=Rocket, meta=(AllowPrivateAccess= "true"))
	float DamageFallOf;
};
