// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Consumable/HealthPotion.h"
#include "PlayerCharacter.h"
#include "Components/StatusContainerComponent.h"

void UHealthPotion::ApplyConsumableEffect(APlayerCharacter* InPlayerCharacter)
{
	if (InPlayerCharacter == nullptr) return;

	UStatusContainerComponent* StatusContainerComponent = InPlayerCharacter->GetStatusContainerComponent();
	if (StatusContainerComponent == nullptr) return;

	float curHealth = StatusContainerComponent->GetCurHealth();
	curHealth += HealAmount;
	StatusContainerComponent->SetCurHealth(HealAmount);
}
