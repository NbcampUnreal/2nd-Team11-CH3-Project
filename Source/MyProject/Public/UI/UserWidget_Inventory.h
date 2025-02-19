// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_Inventory.generated.h"

class UItemBase;
class UUniformGridPanel;

/**
 * 
 */
UCLASS()
class MYPROJECT_API UUserWidget_Inventory : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<UItemBase*> ItemList;

	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* Inventory;

	UFUNCTION(BlueprintCallable)
	void InitUI(const TArray<UItemBase*>& InItemList);

	UFUNCTION(BlueprintCallable)
	void UpdateUI();
};
