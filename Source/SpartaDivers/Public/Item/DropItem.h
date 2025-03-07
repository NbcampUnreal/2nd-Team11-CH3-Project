// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DropItem.generated.h"

class UItemBase;
class USphereComponent;

UCLASS()
class SPARTADIVERS_API ADropItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADropItem();

	UFUNCTION()
	void OnOverlapDropItem(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemBase> OwningItemClass;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* GetSound;

protected:
	UPROPERTY(EditDefaultsOnly)
	USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MeshComponent;
};
