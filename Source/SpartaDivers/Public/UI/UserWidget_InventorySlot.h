// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_InventorySlot.generated.h"

class UUniformGridPanel;

/**
 * 
 */
UCLASS()
class SPARTADIVERS_API UUserWidget_InventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void UpdateInventorySlot();

	UFUNCTION(BlueprintPure)
	UUniformGridPanel* GetInventorySlot();
	
protected:
	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* InventorySlot;
};
