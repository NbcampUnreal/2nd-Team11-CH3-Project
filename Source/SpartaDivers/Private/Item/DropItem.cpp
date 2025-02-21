// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/DropItem.h"

// Sets default values
ADropItem::ADropItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ADropItem::InitDropItem(UItemBase* InItem)
{
	Item = InItem;
}

