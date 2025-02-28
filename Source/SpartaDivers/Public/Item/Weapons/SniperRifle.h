// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/GunBase.h"
#include "SniperRifle.generated.h"

/**
 * 
 */
UCLASS()
class SPARTADIVERS_API USniperRifle : public UGunBase
{
	GENERATED_BODY()
	
public:
	USniperRifle();

	virtual void Fire() override;
	virtual void ResetFireCooldown() override;
	virtual void Reload() override;
	virtual void ApplyRecoil() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sniper|Stats")
	int32 Penetration;

	UFUNCTION(BlueprintCallable, Category = "Sniper|Actions")
	void PerformHitScanWithPenetration();
};
