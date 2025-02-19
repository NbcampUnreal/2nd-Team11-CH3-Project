// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/InventoryComponent.h"
#include "Item/ItemBase.h"
#include "Item/GunBase.h"
#include "Item/AttachmentBase.h"
#include "Item/ConsumableBase.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UInventoryComponent::AddItem(UItemBase* InItem)
{
	EItemType InItemType = InItem->ItemType;

	if (InItemType == EItemType::Gun)
	{
		GunItems.Add(InItem);
	}
	else if (InItemType == EItemType::Attachment)
	{
		AttachmentItems.Add(InItem);
	}
	else if (InItemType == EItemType::Consumable)
	{
		ConsumableItems.Add(InItem);
	}
}

void UInventoryComponent::RemoveItem(UItemBase* InItem)
{
	EItemType InItemType = InItem->ItemType;

	if (InItemType == EItemType::Gun)
	{
		GunItems.Remove(InItem);
	}
	else if (InItemType == EItemType::Attachment)
	{
		AttachmentItems.Remove(InItem);
	}
	else if (InItemType == EItemType::Consumable)
	{
		ConsumableItems.Remove(InItem);
	}
}

