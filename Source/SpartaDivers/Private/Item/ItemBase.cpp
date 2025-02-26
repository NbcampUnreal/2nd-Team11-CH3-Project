// Copyright Epic Games, Inc. All Rights Reserved.

#include "Item/ItemBase.h"

UItemBase::UItemBase()
{
}

void UItemBase::InitializeItem(UItemBase* DefaultItem)
{
	if (DefaultItem->ItemDataTable)
	{
		FItemDataRow* FoundRow = DefaultItem->ItemDataTable->FindRow<FItemDataRow>(DefaultItem->ItemID, FString(""));

		if (FoundRow)
		{
			IconImage = FoundRow->IconImage;
			ItemName = FoundRow->ItemName;
			ItemDescription = FoundRow->ItemDescription;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s : Error From ItemID"), *GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : Error From ItemDataTable"), *GetName());
	}
}
