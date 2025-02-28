// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_PlayerConsumableSlot.generated.h"

class UConsumableBase;
class UImage;

/**
 * 
 */
UCLASS()
class SPARTADIVERS_API UUserWidget_PlayerConsumableSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ApplyUIToModel(UConsumableBase* InConsumable);

	UFUNCTION(BlueprintCallable)
	void UpdateUI();

	UPROPERTY(EditAnywhere)
	int32 SlotNum;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemIcon;

	UFUNCTION(BlueprintPure)
	UImage* GetItemIcon();

	UFUNCTION(BlueprintPure)
	UConsumableBase* GetOwningItem();
};
