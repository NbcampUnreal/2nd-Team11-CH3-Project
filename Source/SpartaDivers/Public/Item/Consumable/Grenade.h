// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ConsumableBase.h"
#include "Grenade.generated.h"

class APlayerProjectile;
/**
 * 
 */
UCLASS()
class SPARTADIVERS_API UGrenade : public UConsumableBase
{
	GENERATED_BODY()

public:
	virtual void ApplyConsumableEffect(APlayerCharacter* InPlayerCharacter) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APlayerProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	float VelocityOfProjectile = 100.f;

	UPROPERTY(EditDefaultsOnly)
	FName FireSocketName;
};
