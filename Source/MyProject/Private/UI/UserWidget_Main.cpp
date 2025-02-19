// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget_Main.h"
#include "UI/UserWidget_Inventory.h"
#include "Component/InventoryComponent.h"

void UUserWidget_Main::InitUI(UInventoryComponent* InventoryComponent)
{
	GunInventory->InitUI(InventoryComponent->GunItems);
	AttachmentInventory->InitUI(InventoryComponent->AttachmentItems);
	ConsumableInventory->InitUI(InventoryComponent->ConsumableItems);

	UpdateUI();
}

void UUserWidget_Main::UpdateUI()
{
	GunInventory->UpdateUI();
	AttachmentInventory->UpdateUI();
	ConsumableInventory->UpdateUI();
}
