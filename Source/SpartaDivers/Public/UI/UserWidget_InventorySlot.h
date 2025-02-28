// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/ItemBase.h"
#include "UserWidget_InventorySlot.generated.h"

class UUniformGridPanel;
class UInventoryComponent;

/**
 * 
 */
UCLASS()
class SPARTADIVERS_API UUserWidget_InventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void UpdateInventorySlot(UInventoryComponent* InventoryComponent);
	
protected:
	UPROPERTY(EditAnywhere)
	EItemType InventoryType;

	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* InventorySlot;
};
