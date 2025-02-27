// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "EnemyProjectile.generated.h"

UCLASS()
class SPARTADIVERS_API AEnemyProjectile : public AProjectileBase
{
	GENERATED_BODY()

public:
	AEnemyProjectile();

	FTimerHandle EnemyProjectileTimerHandle;

	UFUNCTION()
	void OnProjectileOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult);

	void Explode() override;

	void InitProjectile(float InitVelocity) override;
};
