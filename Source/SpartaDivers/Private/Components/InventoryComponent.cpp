// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
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
	switch (InItem->GetItemType())
	{
	case EItemType::Gun:
		if (UGunBase* InGunBase = Cast<UGunBase>(InItem))
		{
			GunInventroy.Add(InGunBase);
		}
		break;
	case EItemType::Attachment:
		if (UAttachmentBase* InAttachmentBase = Cast<UAttachmentBase>(InItem))
		{
			AttachmentInventroy.Add(InAttachmentBase);
		}
		break;
	case EItemType::Consumable:
		if (UConsumableBase* InConsumableBase = Cast<UConsumableBase>(InItem))
		{
			ConsumableInventroy.Add(InConsumableBase);
		}
		break;
	default:
		break;
	}
}

void UInventoryComponent::RemoveItem(UItemBase* InItem)
{
	switch (InItem->GetItemType())
	{
	case EItemType::Gun:
		if (UGunBase* InGunBase = Cast<UGunBase>(InItem))
		{
			GunInventroy.Remove(InGunBase);
		}
		break;
	case EItemType::Attachment:
		if (UAttachmentBase* InAttachmentBase = Cast<UAttachmentBase>(InItem))
		{
			AttachmentInventroy.Remove(InAttachmentBase);
		}
		break;
	case EItemType::Consumable:
		if (UConsumableBase* InConsumableBase = Cast<UConsumableBase>(InItem))
		{
			ConsumableInventroy.Remove(InConsumableBase);
		}
		break;
	default:
		break;
	}
}

