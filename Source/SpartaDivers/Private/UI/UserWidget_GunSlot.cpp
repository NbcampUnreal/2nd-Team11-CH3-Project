// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget_GunSlot.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Item/GunBase.h"
#include "Components/InventoryComponent.h"
#include "Components/Image.h"
#include "UI/MyHUD.h"

void UUserWidget_GunSlot::ApplyUIToModel(UGunBase* InGun)
{
	if (InGun == nullptr) return;
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Player))
	{
		if (bIsMainGun)
		{
			if (PlayerCharacter->GetEquippedGun() == nullptr)
			{
				PlayerCharacter->SetEquippedGun(InGun);
				PlayerCharacter->InventoryComponent->RemoveItem(InGun);
			}
		}
		else
		{
			if (PlayerCharacter->GetSubGun() == nullptr)
			{
				PlayerCharacter->SetSubGun(InGun);
				PlayerCharacter->InventoryComponent->RemoveItem(InGun);
			}
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

void UUserWidget_GunSlot::UpdateUI()
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	UGunBase* OwningGun = GetOwningItem();

	if (ItemIcon)
	{
		if (OwningGun == nullptr)
		{
			ItemIcon->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			ItemIcon->SetVisibility(ESlateVisibility::Visible);
			ItemIcon->SetBrushFromTexture(OwningGun->GetIconImage());

			ItemIcon->SetToolTipText(FText::Format(FText::FromString(TEXT("{0}: {1}")), FText::FromString(OwningGun->GetItemName().ToString()), OwningGun->GetItemDescription()));
		}
	}
}

UImage* UUserWidget_GunSlot::GetItemIcon()
{
	return ItemIcon;
}

UGunBase* UUserWidget_GunSlot::GetOwningItem()
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Player))
	{
		if (bIsMainGun)
		{
			return PlayerCharacter->GetEquippedGun();
		}
		else
		{
			return PlayerCharacter->GetSubGun();
		}
	}

	return nullptr;
}
