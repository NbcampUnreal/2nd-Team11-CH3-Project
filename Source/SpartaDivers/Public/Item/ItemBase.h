// Copyright Epic Games, Inc. All Rights Reserved.

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

UCLASS()
class SPARTADIVERS_API UItemBase : public UObject
{
	GENERATED_BODY()

public:
	UItemBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText ItemDescription;

	FORCEINLINE const UTexture2D* GetIconImage() const { return IconImage; }
	FORCEINLINE const EItemType GetItemType() const { return ItemType; }
	
protected:
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* IconImage;

	UPROPERTY(VisibleAnywhere)
	EItemType ItemType;
};
