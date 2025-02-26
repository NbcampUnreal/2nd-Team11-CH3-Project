// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UItemBase;
class UGunBase;
class UConsumableBase;
class UAttachmentBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPARTADIVERS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

public:
	void AddItem(UItemBase* InItem);

	void RemoveItem(UItemBase* InItem);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UGunBase*> GunInventroy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UAttachmentBase*> AttachmentInventroy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UConsumableBase*> ConsumableInventroy;
};
