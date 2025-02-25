// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/DropItem.h"
#include "Components/SphereComponent.h"
#include "PlayerCharacter.h"
#include "Components/InventoryComponent.h"
#include "Item/ItemBase.h"

// Sets default values
ADropItem::ADropItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionProfileName("Trigger");
	SetRootComponent(SphereComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(SphereComponent);
}

void ADropItem::OnOverlapDropItem(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;

	if (APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		if (OwningItemClass)
		{
			playerCharacter->InventoryComponent->AddItem(NewObject<UItemBase>(this, OwningItemClass));
		}

		Destroy();
	}
}

void ADropItem::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ADropItem::OnOverlapDropItem);
}
