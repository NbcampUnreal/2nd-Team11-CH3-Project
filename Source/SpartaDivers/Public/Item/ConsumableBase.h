// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "ConsumableBase.generated.h"

class APlayerCharacter;

/**
 * 
 */
UCLASS()
class SPARTADIVERS_API UConsumableBase : public UItemBase
{
	GENERATED_BODY()
	
public:
	UConsumableBase();

	virtual void ApplyConsumableEffect(APlayerCharacter* InPlayerCharacter);
};
