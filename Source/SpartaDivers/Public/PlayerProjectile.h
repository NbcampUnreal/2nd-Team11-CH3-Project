// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "PlayerProjectile.generated.h"

UCLASS()
class SPARTADIVERS_API APlayerProjectile : public AProjectileBase
{
	GENERATED_BODY()
	
public:
	APlayerProjectile();

	FTimerHandle PlayerProjectileTimerHandle;

	UFUNCTION()
	void OnProjectileOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	void Explode() override;

	void SetVelocity(FVector InDirection, float Speed);
};
