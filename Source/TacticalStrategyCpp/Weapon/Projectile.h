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

	// Special effects on destruction
	virtual void Destroyed() override;

	// Toggle for server-side rewind
	bool bUseServerSideRewind = false;

	// Start position for projectile tracing
	FVector_NetQuantize TraceStart;

	// Initial velocity for the projectile
	FVector_NetQuantize100 InitializeVelocity;

	UPROPERTY(EditAnywhere)
	float InitialSpeed = 15000;

	UPROPERTY(EditAnywhere)
	float Damage = 20;

	UPROPERTY(EditAnywhere)
	float HeadShotDamage = 40;

protected:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);

	// Audio Visual effects on projectile destruction
	void DestroyedCosmetics();

	// Optional trail particle system
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* TrailSystem;

	UPROPERTY()
	class UNiagaraComponent* TrailSystemComponent;

	// Spawns the trail particle effect
	void SpawnTrailSystem();

	FTimerHandle DestroyTimer;

	// Time after which to self destruct
	UPROPERTY(EditAnywhere)
	float DestroyTime;

	void StartDestroyTimer();
	virtual void DestroyTimerFinished();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ProjectileMesh;

	// Applies damage in an area around the projectile explosion
	void ExplodeDamage();

	// Inner radius for maximum damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Rocket, meta=(AllowPrivateAccess= "true"))
	float InnerRadius;

	// Outer radius for falloff damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Rocket, meta=(AllowPrivateAccess= "true"))
	float OuterRadius;

	// Damage falloff factor between inner and outer radius
	UPROPERTY(EditAnywhere, Category=Rocket, meta=(AllowPrivateAccess= "true"))
	float DamageFallOf;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

private:
	UPROPERTY(EditAnywhere)
	UParticleSystem* Tracer;

	UPROPERTY()
	class UParticleSystemComponent* TracerComponent;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere)
	class USoundCue* ImpactSound;

	// Whether already destroyed
	bool bHasBeenDestroyed = false;
};
