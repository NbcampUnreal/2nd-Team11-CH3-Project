// Copyright Epic Games, Inc. All Rights Reserved.

#include "Item/Weapons/AssaultRifle.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "DrawDebugHelpers.h"

UAssaultRifle::UAssaultRifle()
{
    ItemName = FName(TEXT("AssaultRifle"));
    //ItemDescription = FText::FromString(TEXT("AssaultRifleDescription"));

    Damage = 25.0f;
    FireRate = 0.1f;
    MaxAmmo = 30;
    CurAmmo = MaxAmmo;
    ReloadTime = 1.5f;
    CurRecoil = 0.3f;
    RecoilGap = 0.05f;
    MaxRecoil = 1.0f;
    bOnInfiniteBullet = false;

}

void UAssaultRifle::Fire()
{
    Super::Fire();
    if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        MyGameInstance->AssaultBulletCount++;
    }
    PerformHitScan();
}

void UAssaultRifle::ResetFireCooldown()
{
    Super::ResetFireCooldown();
}

void UAssaultRifle::Reload()
{
    Super::Reload();
}

void UAssaultRifle::ApplyRecoil()
{
    Super::ApplyRecoil();
}

void UAssaultRifle::PerformHitScan()
{
    FVector Start = GetFireStartLocation();
    FVector End =  GetFireEndLocation();
    FHitResult HitResult;
    FVector ShotDirection = HitResult.Location - Start;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(PlayerCharacter); // Player characters ignore conflict

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_PhysicsBody, QueryParams);

    if (bHit)
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor && HitActor->ActorHasTag("Enemy"))
        {
            float FinalDamage = Damage; // 기본 데미지

            // 헤드샷 여부 판별
            if (HitResult.Component == Cast<UPrimitiveComponent>(HitActor->FindComponentByClass<UStaticMeshComponent>()))
            {
                bHitHead = true;
                FinalDamage *= 2.0f;
                UE_LOG(LogTemp, Warning, TEXT("Headshot! Extra damage applied."));
            }
            else
            {
                bHitHead = false;
            }
            
            UGameplayStatics::ApplyPointDamage(
                HitActor,
                FinalDamage * FMath::FRandRange(0.9f, 1.1f),
                ShotDirection,
                HitResult,
                PlayerCharacter->GetController(),
                PlayerCharacter,
                UDamageType::StaticClass());
        }

        // Show debug at crash point
        DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 5.0f, FColor::Red, false, 2.0f);
        DrawDebugLine(GetWorld(), Start, HitResult.ImpactPoint, FColor::Green, false, 2.0f, 0, 1.5f);
    }
    else
    {
        // If there is no collision, draw the debug line blue
        DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 2.0f, 0, 1.5f);
    }

    ApplyRecoil();
}
