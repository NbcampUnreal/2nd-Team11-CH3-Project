// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/ItemBase.h"
#include "UserWidget_ItemSlot.generated.h"


class UImage;
class UInventoryComponent;

/**
 * 
 */
UCLASS()
class SPARTADIVERS_API UUserWidget_ItemSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void UpdateItemSlot();

	UFUNCTION(BlueprintPure)
	UImage* GetItemIcon();

	UFUNCTION(BlueprintPure)
	UItemBase* GetOwningItem();

	UFUNCTION(BlueprintCallable)
	virtual void ApplyUIToModel(UItemBase* InItem);

	UPROPERTY(EditAnywhere)
	EItemType ItemSlotType;

	UPROPERTY(EditAnywhere)
	int32 ItemSlotIndex;

	UFUNCTION(BlueprintCallable)
	void SetItemSlotIndex(int32 InIndex);

	UFUNCTION(BlueprintCallable)
	void SetItemSlotType(EItemType InType);

private:
	UPROPERTY(meta = (BindWidget))
	UImage* ItemIcon;

	TWeakObjectPtr<UInventoryComponent> OwningUInventoryComponent;

	void UpdateOwningUInventoryComponent();
};
