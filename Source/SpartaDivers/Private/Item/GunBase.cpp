// Copyright Epic Games, Inc. All Rights Reserved.

#include "Item/GunBase.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "DrawDebugHelpers.h"

UGunBase::UGunBase()
{
    PlayerCharacter = Cast<APlayerCharacter>(GetOuter());

	ItemName = FName(TEXT("GunBaseName"));
	ItemDescription = FText::FromString(TEXT("GunBaseDescription"));

	Damage = 25.0f;
	FireRate = 2.5f;
	Penetration = 1;
	MaxAmmo = 30;
	Ammo = MaxAmmo;
    bCanFire = true;
}

void UGunBase::Fire()
{
    if (bCanFire && Ammo > 0)
    {
        Ammo--;
        bCanFire = false;  // 발사 제한
        UE_LOG(LogTemp, Warning, TEXT("%s fired! Ammo: %d/%d"), *ItemName.ToString(), Ammo, MaxAmmo);

        PerformHitScan();

        GetWorld()->GetTimerManager().SetTimer(
            FireCooldownTimer, 
            this, 
            &UGunBase::ResetFireCooldown,
            FireRate,
            false);
    }
    else if (Ammo <=0 )
    {
        UE_LOG(LogTemp, Warning, TEXT("Out of ammo! Reload needed."));
    }
}

void UGunBase::PerformHitScan()
{
    FHitResult HitResult;
    FVector Start = PlayerCharacter->GetActorLocation();
    Start += FVector::UpVector * 60.0f;
    FVector End = Start + PlayerCharacter->GetActorForwardVector() * 10000.0f;

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(PlayerCharacter);

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams);

    if (bHit)
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor && HitActor->ActorHasTag("Enemy"))
        {
            UGameplayStatics::ApplyDamage(HitActor, Damage, PlayerCharacter->GetController(), PlayerCharacter, UDamageType::StaticClass());
        }

        DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Red, false, 2.0f);
        DrawDebugLine(GetWorld(), Start, HitResult.ImpactPoint, FColor::Green, false, 2.0f, 0, 2.0f);
    }
    else
    {
        DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 2.0f, 0, 2.0f);
    }
}

void UGunBase::ResetFireCooldown()
{
    bCanFire = true;
    UE_LOG(LogTemp, Warning, TEXT("Fire cooldown reset. Ready to shoot again!"));
}

void UGunBase::Reload()
{
	Ammo = MaxAmmo;
	UE_LOG(LogTemp, Warning, TEXT("%s reloaded! Ammo: %d/%d"), *ItemName.ToString(), Ammo, MaxAmmo);
}
