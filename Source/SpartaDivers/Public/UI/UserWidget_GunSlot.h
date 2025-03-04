// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_GunSlot.generated.h"

class UGunBase;
class UImage;

/**
 * 
 */
UCLASS()
class SPARTADIVERS_API UUserWidget_GunSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ApplyUIToModel(UGunBase* InGun);

	UFUNCTION(BlueprintCallable)
	void UpdateUI();

	UPROPERTY(EditAnywhere)
	bool bIsMainGun;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemIcon;

	UFUNCTION(BlueprintPure)
	UImage* GetItemIcon();

	UFUNCTION(BlueprintPure)
	UGunBase* GetOwningItem();
};
