// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget_InventorySlot.h"
#include "Components/UniformGridPanel.h"
#include "UI/UserWidget_ItemSlot.h"

void UUserWidget_InventorySlot::UpdateInventorySlot()
{
	int32 ChildrenWidgetNum = InventorySlot->GetAllChildren().Num();
	for (int i = 0; i < ChildrenWidgetNum; i++)
	{
		UUserWidget_ItemSlot* ItemSlot = Cast<UUserWidget_ItemSlot>(InventorySlot->GetAllChildren()[i]);
		ItemSlot->UpdateItemSlot();
	}
}

UUniformGridPanel* UUserWidget_InventorySlot::GetInventorySlot()
{
	return InventorySlot;
}
