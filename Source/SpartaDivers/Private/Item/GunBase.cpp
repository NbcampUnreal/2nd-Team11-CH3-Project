// Copyright Epic Games, Inc. All Rights Reserved.

#include "Item/GunBase.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "DrawDebugHelpers.h"

UGunBase::UGunBase()
{
    PlayerCharacter = Cast<APlayerCharacter>(GetOuter());

    Damage = 0;
    FireRate = 0;
    MaxAmmo = 0;
    CurAmmo = MaxAmmo;
    ReloadTime = 0;

    CurRecoil = 0;
    MaxRecoil = 0;

    bCanFire = true;
}

void UGunBase::Fire()
{
    UE_LOG(LogTemp, Warning, TEXT("Base Fire"));
}

void UGunBase::ResetFireCooldown()
{
    UE_LOG(LogTemp, Warning, TEXT("Base ResetFireCooldown"));
}

void UGunBase::Reload()
{
    UE_LOG(LogTemp, Warning, TEXT("Base Reload"));
}
