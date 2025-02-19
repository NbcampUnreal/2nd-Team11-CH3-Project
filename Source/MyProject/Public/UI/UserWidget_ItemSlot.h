// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_ItemSlot.generated.h"

class UImage;
class UItemBase;

/**
 * 
 */
UCLASS()
class MYPROJECT_API UUserWidget_ItemSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UImage* IconImage;

	UPROPERTY()
	UItemBase* ItemBase;

	UFUNCTION(BlueprintCallable)
	void InitUI(UItemBase* InItem);

	UFUNCTION(BlueprintCallable)
	void UpdateUI();
};
