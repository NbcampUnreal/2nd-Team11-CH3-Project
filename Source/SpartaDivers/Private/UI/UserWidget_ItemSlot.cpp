// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget_ItemSlot.h"
#include "Components/Image.h"
#include "Item/ItemBase.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Components/InventoryComponent.h"
#include "UI/MyHUD.h"

#include "Item/GunBase.h"
#include "Item/AttachmentBase.h"
#include "Item/ConsumableBase.h"

void UUserWidget_ItemSlot::UpdateItemSlot()
{
	if (!OwningUInventoryComponent.IsValid()) UpdateOwningUInventoryComponent();

	UItemBase* InItem = GetOwningItem();

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

			ItemIcon->SetToolTipText(InItem->GetItemDescription());
		}
	}
}

UImage* UUserWidget_ItemSlot::GetItemIcon()
{
	return ItemIcon;
}

UItemBase* UUserWidget_ItemSlot::GetOwningItem()
{
	if (!OwningUInventoryComponent.IsValid()) UpdateOwningUInventoryComponent();

	UItemBase* output = nullptr;

	if (OwningUInventoryComponent.IsValid())
	{
		switch (ItemSlotType)
		{
		case EItemType::Gun:
			if (ItemSlotIndex < OwningUInventoryComponent->GetGunInventory().Num())
			{
				output = Cast<UItemBase>(OwningUInventoryComponent->GetGunInventory()[ItemSlotIndex]);
			}
			break;
		case EItemType::Attachment:
			if (ItemSlotIndex < OwningUInventoryComponent->GetAttachmentInventory().Num())
			{
				output = Cast<UItemBase>(OwningUInventoryComponent->GetAttachmentInventory()[ItemSlotIndex]);
			}
			break;
		case EItemType::Consumable:
			if (ItemSlotIndex < OwningUInventoryComponent->GetConsumableInventory().Num())
			{
				output = Cast<UItemBase>(OwningUInventoryComponent->GetConsumableInventory()[ItemSlotIndex]);
			}
			break;
		default:
			break;
		}
	}

	return output;
}

EItemType UUserWidget_ItemSlot::GetItemSlotType()
{
	return ItemSlotType;
}

int32 UUserWidget_ItemSlot::GetItemSlotIndex()
{
	return ItemSlotIndex;
}

void UUserWidget_ItemSlot::ApplyUIToModel(UItemBase* InItem)
{
	if (InItem == nullptr) return;
	if (!OwningUInventoryComponent.IsValid()) UpdateOwningUInventoryComponent();

	APlayerCharacter* PlayerCharacter = nullptr;
	if (ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		PlayerCharacter = Cast<APlayerCharacter>(Character);
	}
	if (PlayerCharacter == nullptr) return;

	if (OwningUInventoryComponent.IsValid())
	{
		if (ItemSlotType == EItemType::Gun)
		{
			UGunBase* InGun = Cast<UGunBase>(InItem);
			if (InGun) OwningUInventoryComponent->AddItem(InGun);

			if (PlayerCharacter->GetEquippedGun() == InGun) PlayerCharacter->SetEquippedGun(nullptr);
			if (PlayerCharacter->GetSubGun() == InGun) PlayerCharacter->SetSubGun(nullptr);
		}
		else if (ItemSlotType == EItemType::Attachment)
		{
			UAttachmentBase* InAttachment = Cast<UAttachmentBase>(InItem);
			if (InAttachment) OwningUInventoryComponent->AddItem(InAttachment);

			if (PlayerCharacter->GetEquippedGun())
			{
				if (PlayerCharacter->GetEquippedGun()->FirstAttachment == InAttachment) PlayerCharacter->GetEquippedGun()->SetAttachment(0, nullptr);
				if (PlayerCharacter->GetEquippedGun()->SecondAttachment == InAttachment) PlayerCharacter->GetEquippedGun()->SetAttachment(1, nullptr);
				if (PlayerCharacter->GetEquippedGun()->ThirdAttachment == InAttachment) PlayerCharacter->GetEquippedGun()->SetAttachment(2, nullptr);
			}
			if (PlayerCharacter->GetSubGun())
			{
				if (PlayerCharacter->GetSubGun()->FirstAttachment == InAttachment) PlayerCharacter->GetSubGun()->SetAttachment(0, nullptr);
				if (PlayerCharacter->GetSubGun()->SecondAttachment == InAttachment) PlayerCharacter->GetSubGun()->SetAttachment(1, nullptr);
				if (PlayerCharacter->GetSubGun()->ThirdAttachment == InAttachment) PlayerCharacter->GetSubGun()->SetAttachment(2, nullptr);
			}

		}
		else if (ItemSlotType == EItemType::Consumable)
		{
			UConsumableBase* InConsumable = Cast<UConsumableBase>(InItem);
			if (InConsumable) OwningUInventoryComponent->AddItem(InConsumable);

			if (PlayerCharacter->FirstConsumable == InConsumable) PlayerCharacter->FirstConsumable = nullptr;
			if (PlayerCharacter->SecondConsumable == InConsumable) PlayerCharacter->SecondConsumable = nullptr;
			if (PlayerCharacter->ThirdConsumable == InConsumable) PlayerCharacter->ThirdConsumable = nullptr;
		}
	}

	UpdateItemSlot();
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (AMyHUD* MyHUD = Cast<AMyHUD>(PlayerController->GetHUD()))
		{
			MyHUD->UpdateMainUI();
		}
	}
}

void UUserWidget_ItemSlot::SetItemSlotIndex(int32 InIndex)
{
	ItemSlotIndex = InIndex;
}

void UUserWidget_ItemSlot::SetItemSlotType(EItemType InType)
{
	ItemSlotType = InType;
}

void UUserWidget_ItemSlot::UpdateOwningUInventoryComponent()
{
	if (ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Character))
		{
			OwningUInventoryComponent = PlayerCharacter->InventoryComponent;
		}
	}
}
