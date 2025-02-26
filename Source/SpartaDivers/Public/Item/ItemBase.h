// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemDataRow.h"
#include "ItemBase.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Gun             UMETA(DisplayName = "Gun"),
	Attachment      UMETA(DisplayName = "Attachment"),
	Consumable      UMETA(DisplayName = "Consumable"),
};

class UTexture2D;
class UDataTable;

UCLASS(Blueprintable, BlueprintType)
class SPARTADIVERS_API UItemBase : public UObject
{
	GENERATED_BODY()

public:
	UItemBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UDataTable* ItemDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemID;

	UFUNCTION(BlueprintPure)
	FORCEINLINE EItemType GetItemType() { return ItemType; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE UTexture2D* GetIconImage() { return IconImage;	}
	UFUNCTION(BlueprintPure)
	FORCEINLINE FName GetItemName() { return ItemName; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE FText GetItemDescription() { return ItemDescription; }

	virtual void InitializeItem(UItemBase* DefaultItem);
	
protected:
	UPROPERTY(VisibleAnywhere)
	EItemType ItemType;

private:
	UTexture2D* IconImage;

	FName ItemName;

	FText ItemDescription;
};
