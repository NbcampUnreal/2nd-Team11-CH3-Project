// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget_MainMenu.h"
#include "UI/UserWidget_InventorySlot.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"

void UUserWidget_MainMenu::UpdateInventorySlot()
{
	if (!OwnerCharacter.IsValid())
	{
		OwnerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
		if (!OwnerCharacter.IsValid()) return;
	}

	GunInventorySlot->UpdateInventorySlot(OwnerCharacter->InventoryComponent);
	AttachmentInventorySlot->UpdateInventorySlot(OwnerCharacter->InventoryComponent);
	ConsumableInventorySlot->UpdateInventorySlot(OwnerCharacter->InventoryComponent);
}
