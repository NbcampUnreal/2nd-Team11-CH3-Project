// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SDEnemyBase.h"
#include "MeleeMinion.generated.h"

UCLASS()
class SPARTADIVERS_API AMeleeMinion : public ASDEnemyBase
{
	GENERATED_BODY()
	
protected:
	AMeleeMinion();

	virtual void Attack(int32 SkillIndex) override;

	int32 CurCombo = 0;

	UPROPERTY(EditDefaultsOnly)
	TArray<UAnimMontage*> AttackMontages;

	virtual void ApplyAttackEffect(int32 EffectIndex) override;

	UPROPERTY(EditDefaultsOnly)
	float AttackRange = 200.f;
};
