// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DropItem.generated.h"

class UItemBase;

UCLASS()
class SPARTADIVERS_API ADropItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADropItem();

	void InitDropItem(UItemBase* InItem);
	
protected:
	UPROPERTY()
	UItemBase* Item;
};
