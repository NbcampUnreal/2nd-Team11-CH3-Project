// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/Weapons/AssaultRifle.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/SpringArmComponent.h"

UAssaultRifle::UAssaultRifle()
{
    ItemName = FName(TEXT("AssaultRifle"));
    ItemDescription = FText::FromString(TEXT("AssaultRifleDescription"));

    Damage = 5.0f;
    FireRate = 0.1f;
    MaxAmmo = 30;
    CurAmmo = MaxAmmo;
    ReloadTime = 1.5f;

    CurRecoil = 0.1f;
    MaxRecoil = 1.1f;
}

void UAssaultRifle::Fire()
{
    if (bCanFire && CurAmmo > 0)
    {
        CurAmmo--;
        bCanFire = false;

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
    FVector CameraLocation;
    FRotator CameraRotation;

    PlayerCharacter->GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);

    float SpringArmLength = 300.0f; // Default Value

    USpringArmComponent* SpringArm = PlayerCharacter->FindComponentByClass<USpringArmComponent>();
    if (SpringArm)
    {
        // Adjust the starting position of the muzzle by the length of the spring arm
        SpringArmLength = SpringArm->TargetArmLength;
    }

    FVector ForwardVector = CameraRotation.Vector();
    FVector MuzzleOffset = ForwardVector * SpringArmLength;
    FVector MuzzleLocation = CameraLocation + MuzzleOffset;

    FVector Start = MuzzleLocation;

    FVector End = Start + CameraRotation.Vector() * 10000.0f;

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(PlayerCharacter); // Player characters ignore conflict

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams);

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

        // Show debug at crash point
        DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 5.0f, FColor::Red, false, 2.0f);
        DrawDebugLine(GetWorld(), Start, HitResult.ImpactPoint, FColor::Green, false, 2.0f, 0, 1.5f);
    }
    else
    {
        // If there is no collision, draw the debug line blue
        DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 2.0f, 0, 1.5f);
    }
}