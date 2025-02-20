// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "GunBase.generated.h"

UCLASS()
class SPARTADIVERS_API UGunBase : public UItemBase
{
	GENERATED_BODY()

public:
	UGunBase();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun|Stats")
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun|Stats")
	float FireRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun|Stats")
	int32 Penetration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun|Stats")
	int32 Ammo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun|Stats")
	int32 MaxAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun|Stats")
	float ReloadTime;

	UFUNCTION(BlueprintCallable,Category = "Gun|Actions")
	void Fire();
	UFUNCTION(BlueprintCallable,Category = "Gun|Actions")
	void Reload();
};
