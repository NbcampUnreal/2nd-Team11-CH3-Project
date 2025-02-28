// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ConsumableBase.h"

UConsumableBase::UConsumableBase()
{
	ItemType = EItemType::Consumable;
}

void UConsumableBase::ApplyConsumableEffect(APlayerCharacter* InPlayerCharacter)
{
}
