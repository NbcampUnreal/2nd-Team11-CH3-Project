// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget_Inventory.h"
#include "UI/UserWidget_ItemSlot.h"
#include "Components/UniformGridPanel.h"
#include "Component/InventoryComponent.h"
#include "Item/GunBase.h"
#include "Item/AttachmentBase.h"
#include "Item/ConsumableBase.h"

void UUserWidget_Inventory::InitUI(const TArray<UItemBase*>& InItemList)
{
	ItemList = InItemList;
	UpdateUI();
}

void UUserWidget_Inventory::UpdateUI()
{
	const TArray<UWidget*>& ItemSlots = Inventory->GetAllChildren();
	for (int i = 0; i < ItemSlots.Num(); i++)
	{
		UUserWidget_ItemSlot* ItemSlot = Cast<UUserWidget_ItemSlot>(ItemSlots[i]);
		if (ItemSlot == nullptr) return;

		if (ItemList.Num() > i)
		{
			ItemSlot->InitUI(ItemList[i]);
		}
		else
		{
			ItemSlot->InitUI(nullptr);
		}
	}
}
