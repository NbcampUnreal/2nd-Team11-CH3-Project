// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemBase.generated.h"

class UTexture2D;

UENUM()
enum class EItemType : uint8
{
	Gun,
	Attachment,
	Consumable
};

/**
 * 
 */
UCLASS()
class SPARTADIVERS_API UItemBase : public UObject
{
	GENERATED_BODY()

public:
	UItemBase();

	FORCEINLINE const UTexture2D* GetIconImage() const { return IconImage; }
	FORCEINLINE const EItemType GetItemType() const { return ItemType; }
	
protected:
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* IconImage;

	UPROPERTY(VisibleAnywhere)
	EItemType ItemType;
};
