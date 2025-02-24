// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SDEnemyBase.h"
#include "RangedMinion.generated.h"

class AEnemyProjectile;

/**
 * 
 */
UCLASS()
class SPARTADIVERS_API ARangedMinion : public ASDEnemyBase
{
	GENERATED_BODY()

protected:
	virtual void Attack() override;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemyProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	float VelocityOfProjectile = 100.f;

	UPROPERTY(EditDefaultsOnly)
	FName FireSocketName;

public:
	virtual void ApplyAttackEffect(int32 EffectIndex) override;

	UFUNCTION(BlueprintCallable)
	void Fire();
};
