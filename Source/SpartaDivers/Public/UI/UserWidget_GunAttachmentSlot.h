// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_GunAttachmentSlot.generated.h"

class UAttachmentBase;
class UImage;

/**
 * 
 */
UCLASS()
class SPARTADIVERS_API UUserWidget_GunAttachmentSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ApplyUIToModel(UAttachmentBase* InAttachment);

	UFUNCTION(BlueprintCallable)
	void UpdateUI();

	UPROPERTY(EditAnywhere)
	int32 SlotNum;

	UPROPERTY(EditAnywhere)
	bool bIsMainGun;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemIcon;

	UFUNCTION(BlueprintPure)
	UImage* GetItemIcon();

	UFUNCTION(BlueprintPure)
	UAttachmentBase* GetOwningItem();
};
