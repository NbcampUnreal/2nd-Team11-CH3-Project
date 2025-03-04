// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/GunBase.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class SPARTADIVERS_API UShotgun : public UGunBase
{
	GENERATED_BODY()
	
public:
	UShotgun();

	virtual void Fire() override;
	virtual void ResetFireCooldown() override;
	virtual void Reload() override;
	virtual void ApplyRecoil() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shotgun|Stats")
	int32 ShotgunPellets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shotgun|Stats")
	float SpreadAngle;

	UFUNCTION(BlueprintCallable, Category = "Shotgun|Actions")
	void FireShotgun();
};
