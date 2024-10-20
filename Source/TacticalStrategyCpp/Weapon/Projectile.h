// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TACTICALSTRATEGYCPP_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	AProjectile();
	
	virtual void Tick(float DeltaTime) override;

	virtual void Destroyed() override;

	bool bUseServerSideRewind = false;

	FVector_NetQuantize TraceStart;

	FVector_NetQuantize100 InitializeVelocity;

	UPROPERTY(EditAnywhere)
	float InitialSpeed = 15000;	

	float Damage;

protected:
	
	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;
	
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);
	
	void DestroyedCosmetics() const;
	
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;
	
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* TrailSystem;	

	UPROPERTY()
	class UNiagaraComponent* TrailSystemComponent;

	void SpawnTrailSystem();
	
	FTimerHandle DestroyTimer;

	/**
	 * Tie after which to destroy rocket so that trail dissipates
	 */
	UPROPERTY(EditAnywhere)
	float DestroyTime;	

	void StartDestroyTimer();
	virtual void DestroyTimerFinished();
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ProjectileMesh;

	void ExplodeDamage();	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Rocket, meta=(AllowPrivateAccess= "true"))
	float InnerRadius;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Rocket, meta=(AllowPrivateAccess= "true"))
	float OuterRadius;

	/*
	 * The fallOf for the damage applied in-between the inner and outer radius  
	 */
	UPROPERTY(EditAnywhere, Category=Rocket, meta=(AllowPrivateAccess= "true"))
	float DamageFallOf;

private:
	UPROPERTY(EditAnywhere)
	UParticleSystem* Tracer;

	UPROPERTY()
	class UParticleSystemComponent* TracerComponent;
	
	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactParticles;
	
	UPROPERTY(EditAnywhere)
	class USoundCue* ImpactSound;

	void DestroyAfterTime();
public:
};
