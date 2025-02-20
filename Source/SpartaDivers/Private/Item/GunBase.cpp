// Copyright Epic Games, Inc. All Rights Reserved.

#include "Item/GunBase.h"

UGunBase::UGunBase()
{
	ItemName = FName(TEXT("GunBaseName"));
	ItemDescription = FText::FromString(TEXT("GunBaseDescription"));

	Damage = 25.0f;
	FireRate = 2.5f;
	Penetration = 1;
	MaxAmmo = 30;
	Ammo = MaxAmmo;
}

void UGunBase::Fire()
{
	if (Ammo > 0)
	{
		Ammo--;
		UE_LOG(LogTemp, Warning, TEXT("%s fired! Ammo: %d/%d"), *ItemName.ToString(), Ammo, MaxAmmo);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Out of ammo! Reload needed."));
	}
}

void UGunBase::Reload()
{
	Ammo = MaxAmmo;
	UE_LOG(LogTemp, Warning, TEXT("%s reloaded! Ammo: %d/%d"), *ItemName.ToString(), Ammo, MaxAmmo);
}
