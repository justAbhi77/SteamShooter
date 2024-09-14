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

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* RocketMesh;
	
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
