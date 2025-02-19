// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_Main.generated.h"

class UUserWidget_Inventory;
class UInventoryComponent;

/**
 * 
 */
UCLASS()
class MYPROJECT_API UUserWidget_Main : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UUserWidget_Inventory* GunInventory;

	UPROPERTY(meta = (BindWidget))
	UUserWidget_Inventory* AttachmentInventory;

	UPROPERTY(meta = (BindWidget))
	UUserWidget_Inventory* ConsumableInventory;

	UFUNCTION(BlueprintCallable)
	void InitUI(UInventoryComponent* InventoryComponent);

	UFUNCTION(BlueprintCallable)
	void UpdateUI();
};
