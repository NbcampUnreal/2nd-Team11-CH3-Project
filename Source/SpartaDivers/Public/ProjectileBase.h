// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UStaticMeshComponent;
class UNiagaraSystem;

UCLASS()
class SPARTADIVERS_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectileBase();

protected:
	UPROPERTY(EditAnywhere)
	USphereComponent* Collision;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent;

	FTimerHandle ExplosionTimerHandle;

	UPROPERTY(EditAnywhere)
	USoundBase* ExplosionSound;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* ExplosionEffect;

	UPROPERTY(EditAnywhere, Category = "Explode")
	float ExplosionDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explode")
	float ExplosionRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explode")
	float ExplosionDamage;

	UFUNCTION()
	virtual void Explode();

public:
	virtual void InitProjectile(float InitVelocity);
	virtual void SetVelocity(FVector InDirection, float Speed);
	void SetExplosionDamage(float NewDamage);
	float GetExplosionDamage() const;


};
