// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget_PlayerConsumableSlot.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Components/InventoryComponent.h"
#include "Item/ConsumableBase.h"
#include "Components/Image.h"
#include "UI/MyHUD.h"

void UUserWidget_PlayerConsumableSlot::ApplyUIToModel(UConsumableBase* InConsumable)
{
	if (InConsumable == nullptr) return;
	if (GetOwningItem() != nullptr) return;

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Player))
	{
		switch (SlotNum)
		{
		case 0:
			PlayerCharacter->FirstConsumable = InConsumable;
			break;
		case 1:
			PlayerCharacter->SecondConsumable = InConsumable;
			break;
		case 2:
			PlayerCharacter->ThirdConsumable = InConsumable;
			break;
		default:
			break;
		}

		PlayerCharacter->InventoryComponent->RemoveItem(InConsumable);
	}
	UpdateUI();
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (AMyHUD* MyHUD = Cast<AMyHUD>(PlayerController->GetHUD()))
		{
			MyHUD->UpdateMainUI();
		}
	}
}

void UUserWidget_PlayerConsumableSlot::UpdateUI()
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	UConsumableBase* OwningConsumable = GetOwningItem();

	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Player))
	{
		if (ItemIcon)
		{
			if (OwningConsumable == nullptr)
			{
				ItemIcon->SetVisibility(ESlateVisibility::Hidden);
			}
			else
			{
				ItemIcon->SetVisibility(ESlateVisibility::Visible);
				ItemIcon->SetBrushFromTexture(OwningConsumable->GetIconImage());

				ItemIcon->SetToolTipText(FText::Format(FText::FromString(TEXT("{0}: {1}")), FText::FromString(OwningConsumable->GetItemName().ToString()), OwningConsumable->GetItemDescription()));
			}
		}
	}
}

UImage* UUserWidget_PlayerConsumableSlot::GetItemIcon()
{
	return ItemIcon;
}

UConsumableBase* UUserWidget_PlayerConsumableSlot::GetOwningItem()
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	UConsumableBase* OwningConsumable = nullptr;

	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Player))
	{
		switch (SlotNum)
		{
		case 0:
			OwningConsumable = PlayerCharacter->FirstConsumable;
			break;
		case 1:
			OwningConsumable = PlayerCharacter->SecondConsumable;
			break;
		case 2:
			OwningConsumable = PlayerCharacter->ThirdConsumable;
			break;
		default:
			break;
		}
	}

	return OwningConsumable;
}

int32 UUserWidget_PlayerConsumableSlot::GetSlotNum()
{
	return SlotNum;
}
