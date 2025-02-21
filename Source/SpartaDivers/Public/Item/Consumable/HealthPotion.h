// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ConsumableBase.h"
#include "HealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class SPARTADIVERS_API UHealthPotion : public UConsumableBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly)
	float HealAmount;

	virtual void ApplyConsumableEffect(APlayerCharacter* InPlayerCharacter) override;
};
