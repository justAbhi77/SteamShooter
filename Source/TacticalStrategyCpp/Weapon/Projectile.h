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

protected:
	
	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;
	
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	float Damage;
	
	void DestroyedCosmetics() const;
	
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

	void DestroyAfterTime();
public:
};
