// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ConsumableBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MyHUD.h"

UConsumableBase::UConsumableBase()
{
	ItemType = EItemType::Consumable;
}

void UConsumableBase::ApplyConsumableEffect(APlayerCharacter* InPlayerCharacter)
{
	AMyHUD* MyHUD = Cast<AMyHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (MyHUD == nullptr) return;

	MyHUD->UpdateMainUI();
}
