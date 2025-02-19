// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget_ItemSlot.h"
#include "Components/Image.h"
#include "Item/ItemBase.h"

void UUserWidget_ItemSlot::InitUI(UItemBase* InItem)
{
	ItemBase = InItem;
	UpdateUI();
}

void UUserWidget_ItemSlot::UpdateUI()
{
	if (ItemBase)
	{
		IconImage->SetBrushFromTexture(ItemBase->Icon);
		UE_LOG(LogTemp, Warning, TEXT("A"));
	}
	else
	{
		IconImage->SetBrushFromTexture(nullptr);
		UE_LOG(LogTemp, Warning, TEXT("B"));
	}
}
