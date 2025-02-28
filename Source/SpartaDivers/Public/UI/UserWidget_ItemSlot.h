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
class SPARTADIVERS_API UUserWidget_ItemSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void UpdateItemSlot(UItemBase* InItem);

private:
	UPROPERTY(meta = (BindWidget))
	UImage* ItemIcon;
};
