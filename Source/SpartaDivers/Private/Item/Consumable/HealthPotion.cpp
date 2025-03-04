// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Consumable/HealthPotion.h"
#include "PlayerCharacter.h"
#include "Components/StatusContainerComponent.h"
#include "Components/InventoryComponent.h"

void UHealthPotion::ApplyConsumableEffect(APlayerCharacter* InPlayerCharacter)
{
	Super::ApplyConsumableEffect(InPlayerCharacter);

	UStatusContainerComponent* StatusContainerComponent = InPlayerCharacter->GetStatusContainerComponent();
	float curHealth = StatusContainerComponent->GetCurHealth();

	StatusContainerComponent->SetCurHealth(curHealth + HealAmount);

	InPlayerCharacter->InventoryComponent->RemoveItem(this);

	UE_LOG(LogTemp, Warning, TEXT("HealthPotion : +%f"), HealAmount);
}
