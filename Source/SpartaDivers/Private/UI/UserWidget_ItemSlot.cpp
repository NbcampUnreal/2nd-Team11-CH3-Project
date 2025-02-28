// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget_ItemSlot.h"
#include "Components/Image.h"
#include "Item/ItemBase.h"

void UUserWidget_ItemSlot::UpdateItemSlot(UItemBase* InItem)
{
	if (ItemIcon)
	{
		if (InItem == nullptr)
		{
			ItemIcon->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			ItemIcon->SetVisibility(ESlateVisibility::Visible);
			ItemIcon->SetBrushFromTexture(InItem->GetIconImage());
		}
	}
}
