// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UItemBase;
class UGunBase;
class UAttachmentBase;
class UConsumableBase;
class UUserWidget_Inventory;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

public:
	UFUNCTION(BlueprintCallable)
	void AddItem(UItemBase* InItem);

	UFUNCTION(BlueprintCallable)
	void RemoveItem(UItemBase* InItem);

	UPROPERTY()
	TArray<UItemBase*> GunItems;

	UPROPERTY()
	TArray<UItemBase*> AttachmentItems;

	UPROPERTY()
	TArray<UItemBase*> ConsumableItems;
};
