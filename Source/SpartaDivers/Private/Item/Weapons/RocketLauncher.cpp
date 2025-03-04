// Copyright Epic Games, Inc. All Rights Reserved.

#include "Item/Weapons/RocketLauncher.h"
#include "MyGameInstance.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "PlayerProjectile.h"

URocketLauncher::URocketLauncher()
{
    ItemName = FName(TEXT("RocketLauncher"));
    //ItemDescription = FText::FromString(TEXT("AssaultRifleDescription"));

    static ConstructorHelpers::FClassFinder<APlayerProjectile> ProjectileBP(TEXT("/Game/_Blueprint/Player/BP_PlayerProjectile.BP_PlayerProjectile"));
    if (ProjectileBP.Succeeded())
    {
        ProjectileClass = ProjectileBP.Class;
    }

    Damage = 30.0f;
    FireRate = 2.0f;
    MaxAmmo = 8;
    CurAmmo = MaxAmmo;
    ReloadTime = 3;
    CurRecoil = 10.0f;
    RecoilGap = 2.0f;
    MaxRecoil = 10.0f;
    bOnInfiniteBullet = false;
}

void URocketLauncher::Fire()
{
    if (bCanFire && CurAmmo > 0)
    {
        if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
        {
            MyGameInstance->RocketBulletCount++;
        }
        Damage = FMath::RandRange(200.0f, 500.0f);
        LaunchProjectile();
        Super::Fire();

        GetWorld()->GetTimerManager().SetTimer(
            FireCooldownTimer,
            this,
            &URocketLauncher::ResetFireCooldown,
            FireRate,
            false);
    }
}

void URocketLauncher::LaunchProjectile()
{
    FVector SocketLocation = PlayerCharacter->GetMesh()->GetSocketLocation(FireSocketName);
    FRotator LaunchRotation = GetLaunchRotation(); // 목표 방향을 가져옴

    APlayerProjectile* ProjectileInstance = GetWorld()->SpawnActor<APlayerProjectile>(
        ProjectileClass, 
        SocketLocation,
        LaunchRotation);

    if (ProjectileInstance)
    {
        FVector Direction = LaunchRotation.Vector();
        ProjectileInstance->SetExplosionDamage(Damage);
        ProjectileInstance->SetVelocity(Direction , VelocityOfProjectile);
    }
}

FRotator URocketLauncher::GetLaunchRotation()
{
    FVector Start = GetCameraLocation(); 
    FVector ForwardVector = PlayerCharacter->GetControlRotation().Vector();
    FVector End = Start + (ForwardVector * 10000.0f);

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(PlayerCharacter);

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams);

    if (bHit)
    {
        FVector TargetLocation = HitResult.ImpactPoint;
        DrawDebugPoint(GetWorld(), TargetLocation, 5.0f, FColor::Red, false, 2.0f);
        DrawDebugLine(GetWorld(), Start, TargetLocation, FColor::Green, false, 2.0f, 0, 1.5f);

        return (TargetLocation - PlayerCharacter->GetMesh()->GetSocketLocation(FireSocketName)).Rotation();
    }
    else
    {
        DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 2.0f, 0, 1.5f);
        return PlayerCharacter->GetControlRotation();
    }
}


void URocketLauncher::ResetFireCooldown()
{
    Super::ResetFireCooldown();
}

void URocketLauncher::Reload()
{
    Super::Reload();
}

