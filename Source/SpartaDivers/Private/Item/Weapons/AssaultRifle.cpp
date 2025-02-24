// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/Weapons/AssaultRifle.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "DrawDebugHelpers.h"

UAssaultRifle::UAssaultRifle()
{
    ItemName = FName(TEXT("AssaultRifle"));
    ItemDescription = FText::FromString(TEXT("AssaultRifleDescription"));

    Damage = 5.0f;
    FireRate = 0.1f;
    MaxAmmo = 30;
    CurAmmo = MaxAmmo;
    ReloadTime = 2.0f;

    CurRecoil = 0.1f;
    MaxRecoil = 1.1f;
}

void UAssaultRifle::Fire()
{
    if (bCanFire && CurAmmo > 0)
    {
        CurAmmo--;
        bCanFire = false;  // 발사 제한

        UE_LOG(LogTemp, Warning, TEXT("AssaultRifle fired! Ammo: %d/%d"), CurAmmo, MaxAmmo);

        PerformHitScan();

        GetWorld()->GetTimerManager().SetTimer(
            FireCooldownTimer, 
            this, 
            &UGunBase::ResetFireCooldown,
            FireRate,
            false);
    }
    else if (CurAmmo <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Out of ammo! AssaultRifle Reload needed."));
    }
}

void UAssaultRifle::ResetFireCooldown()
{
    bCanFire = true;
    UE_LOG(LogTemp, Warning, TEXT("AssaultRifle Fire cooldown reset. Ready to shoot again!"));
}

void UAssaultRifle::Reload()
{
    CurAmmo = MaxAmmo;
    UE_LOG(LogTemp, Warning, TEXT("AssaultRifle reloaded! Ammo: %d/%d"), CurAmmo, MaxAmmo);
}

void UAssaultRifle::PerformHitScan()
{
    FHitResult HitResult;
    FVector Start = PlayerCharacter->GetActorLocation();
    Start += FVector::UpVector * 60.0f;
    FVector End = Start + PlayerCharacter->GetActorForwardVector() * 10000.0f;

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(PlayerCharacter);

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, QueryParams);

    if (bHit)
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor && HitActor->ActorHasTag("Enemy"))
        {
            UGameplayStatics::ApplyDamage(
                HitActor, 
                Damage, 
                PlayerCharacter->GetController(), 
                PlayerCharacter, 
                UDamageType::StaticClass());
        }

        DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Red, false, 2.0f);
        DrawDebugLine(GetWorld(), Start, HitResult.ImpactPoint, FColor::Green, false, 2.0f, 0, 2.0f);
    }
    else
    {
        DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 2.0f, 0, 2.0f);
    }
}