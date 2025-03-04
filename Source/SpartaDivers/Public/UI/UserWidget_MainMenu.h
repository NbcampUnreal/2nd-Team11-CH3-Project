// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_MainMenu.generated.h"

class UUserWidget_InventorySlot;
class APlayerCharacter;

/**
 * 
 */
UCLASS()
class SPARTADIVERS_API UUserWidget_MainMenu : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UUserWidget_InventorySlot* GunInventorySlot;

	UPROPERTY(meta = (BindWidget))
	UUserWidget_InventorySlot* AttachmentInventorySlot;

	UPROPERTY(meta = (BindWidget))
	UUserWidget_InventorySlot* ConsumableInventorySlot;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateInventorySlot();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMainMenu();

private:
	TWeakObjectPtr<APlayerCharacter> OwnerCharacter;
};
