// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget_GunAttachmentSlot.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Item/GunBase.h"
#include "Components/InventoryComponent.h"
#include "Item/AttachmentBase.h"
#include "Components/Image.h"
#include "UI/MyHUD.h"

void UUserWidget_GunAttachmentSlot::ApplyUIToModel(UAttachmentBase* InAttachment)
{
	if (InAttachment == nullptr) return;
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	UGunBase* OwningGun = nullptr;

	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Player))
	{
		if (bIsMainGun)
		{
			OwningGun = PlayerCharacter->GetEquippedGun();
		}
		else
		{
			OwningGun = PlayerCharacter->GetSubGun();
		}
		if (OwningGun == nullptr) return;

		switch (SlotNum)
		{
		case 0:
			if (OwningGun->FirstAttachment == nullptr)
			{
				OwningGun->SetAttachment(0, InAttachment);
				PlayerCharacter->InventoryComponent->RemoveItem(InAttachment);
			}
			break;
		case 1:
			if (OwningGun->SecondAttachment == nullptr)
			{
				OwningGun->SetAttachment(1, InAttachment);
				PlayerCharacter->InventoryComponent->RemoveItem(InAttachment);
			}
			break;
		case 2:
			if (OwningGun->ThirdAttachment == nullptr)
			{
				OwningGun->SetAttachment(2, InAttachment);
				PlayerCharacter->InventoryComponent->RemoveItem(InAttachment);
			}
			break;
		default:
			break;
		}
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

void UUserWidget_GunAttachmentSlot::UpdateUI()
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	UAttachmentBase* OwningAttachment = GetOwningItem();

	if (ItemIcon)
	{
		if (OwningAttachment == nullptr)
		{
			ItemIcon->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			ItemIcon->SetVisibility(ESlateVisibility::Visible);
			ItemIcon->SetBrushFromTexture(OwningAttachment->GetIconImage());

			ItemIcon->SetToolTipText(OwningAttachment->GetItemDescription());
		}
	}
}

UImage* UUserWidget_GunAttachmentSlot::GetItemIcon()
{
	return ItemIcon;
}

UAttachmentBase* UUserWidget_GunAttachmentSlot::GetOwningItem()
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	UGunBase* OwningGun = nullptr;
	UAttachmentBase* OwningAttachment = nullptr;

	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Player))
	{
		if (bIsMainGun)
		{
			OwningGun = PlayerCharacter->GetEquippedGun();
		}
		else
		{
			OwningGun = PlayerCharacter->GetSubGun();
		}
		if (OwningGun == nullptr) return nullptr;

		switch (SlotNum)
		{
		case 0:
			OwningAttachment = OwningGun->FirstAttachment;
			break;
		case 1:
			OwningAttachment = OwningGun->SecondAttachment;
			break;
		case 2:
			OwningAttachment = OwningGun->ThirdAttachment;
			break;
		default:
			break;
		}
	}

	return OwningAttachment;
}
