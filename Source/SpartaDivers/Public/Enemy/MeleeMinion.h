// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SDEnemyBase.h"
#include "MeleeMinion.generated.h"

/**
 * 
 */
UCLASS()
class SPARTADIVERS_API AMeleeMinion : public ASDEnemyBase
{
	GENERATED_BODY()
	
protected:
	virtual void Attack() override;

	int32 CurCombo = 0;

	UPROPERTY(EditDefaultsOnly)
	TArray<UAnimMontage*> AttackMontages;
};
