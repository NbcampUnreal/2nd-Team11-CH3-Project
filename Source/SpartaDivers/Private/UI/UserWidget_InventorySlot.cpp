// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget_InventorySlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/InventoryComponent.h"
#include "UI/UserWidget_ItemSlot.h"

#include "Item/GunBase.h"
#include "Item/AttachmentBase.h"
#include "Item/ConsumableBase.h"

void UUserWidget_InventorySlot::UpdateInventorySlot(UInventoryComponent* InventoryComponent)
{
	if (InventorySlot)
	{
		int32 ItemNum = 0;

		switch (InventoryType)
		{
		case EItemType::Gun:
			ItemNum = InventoryComponent->GetGunInventory().Num();
			break;
		case EItemType::Attachment:
			ItemNum = InventoryComponent->GetAttachmentInventory().Num();
			break;
		case EItemType::Consumable:
			ItemNum = InventoryComponent->GetConsumableInventory().Num();
			break;
		default:
			break;
		}

		for (int i = 0; i < ItemNum; i++)
		{
			UUserWidget_ItemSlot* ItemSlot = Cast<UUserWidget_ItemSlot>(InventorySlot->GetAllChildren()[i]);
			if (ItemSlot)
			{
				if (InventoryComponent == nullptr)
				{
					ItemSlot->UpdateItemSlot(nullptr);
				}
				else
				{
					switch (InventoryType)
					{
					case EItemType::Gun:
						ItemSlot->UpdateItemSlot(Cast<UItemBase>(InventoryComponent->GetGunInventory()[i]));
						break;
					case EItemType::Attachment:
						ItemSlot->UpdateItemSlot(Cast<UItemBase>(InventoryComponent->GetAttachmentInventory()[i]));
						break;
					case EItemType::Consumable:
						ItemSlot->UpdateItemSlot(Cast<UItemBase>(InventoryComponent->GetConsumableInventory()[i]));
						break;
					default:
						break;
					}
				}
			}
		}

		int32 ChildrenWidgetNum = InventorySlot->GetAllChildren().Num();
		for (int i = ItemNum; i < ChildrenWidgetNum; i++)
		{
			UUserWidget_ItemSlot* ItemSlot = Cast<UUserWidget_ItemSlot>(InventorySlot->GetAllChildren()[i]);
			ItemSlot->UpdateItemSlot(nullptr);
		}
	}
}
