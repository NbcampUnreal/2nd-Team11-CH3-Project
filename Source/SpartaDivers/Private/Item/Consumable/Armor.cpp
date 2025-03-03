// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Consumable/Armor.h"
#include "PlayerCharacter.h"
#include "Components/StatusContainerComponent.h"
#include "Components/InventoryComponent.h"

void UArmor::ApplyConsumableEffect(APlayerCharacter* InPlayerCharacter)
{
	Super::ApplyConsumableEffect(InPlayerCharacter);

	/*UStatusContainerComponent* StatusContainerComponent = InPlayerCharacter->GetStatusContainerComponent();
	float curArmor = StatusContainerComponent->GetCurArmor();

	StatusContainerComponent->SetCurHealth(curArmor + ArmorAmount);

	InPlayerCharacter->InventoryComponent->RemoveItem(this);

	UE_LOG(LogTemp, Warning, TEXT("Armor : +%f"), ArmorAmount);*/
}
