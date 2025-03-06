// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SDEnemyBase.h"
#include "RangedMinion.generated.h"

class AEnemyProjectile;

UCLASS()
class SPARTADIVERS_API ARangedMinion : public ASDEnemyBase
{
	GENERATED_BODY()

protected:
	ARangedMinion();


	virtual void Attack(int32 SkillIndex) override;

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
