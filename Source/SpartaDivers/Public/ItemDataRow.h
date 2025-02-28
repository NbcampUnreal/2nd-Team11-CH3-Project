// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataRow.generated.h"

class UTexture2D;
class UItemBase;

USTRUCT(BlueprintType)
struct FItemDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UItemBase> ItemClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* IconImage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText ItemDescription;
};