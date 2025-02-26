// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/GunBase.h"
#include "AssaultRifle.generated.h"

/**
 * 
 */
UCLASS()
class SPARTADIVERS_API UAssaultRifle : public UGunBase
{
	GENERATED_BODY()

public:
	UAssaultRifle();

	virtual void Fire() override;
	virtual void ResetFireCooldown() override;
	virtual void Reload() override;

	UFUNCTION(BlueprintCallable,Category = "Gun|Actions")
	void PerformHitScan();

	bool bOnInfiniteBullet;
};