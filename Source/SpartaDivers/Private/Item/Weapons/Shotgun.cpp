// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapons/Shotgun.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "DrawDebugHelpers.h"

UShotgun::UShotgun()
{
	ItemName = FName(TEXT("Shotgun"));
	//ItemDescription = FText::FromString(TEXT("ShotgunDescription"));

	Damage = 10.0f;
	FireRate = 0.3f;
	MaxAmmo = 15;
	CurAmmo = MaxAmmo;
	ReloadTime = 1.5f;

	CurRecoil = 2.0f;

    ShotgunPellets = 10;
    SpreadAngle = 5.0f;
}

void UShotgun::Fire()
{
	Super::Fire();
	Damage = FMath::RandRange(10.0f, 20.0f);
	FireShotgun();
}

void UShotgun::ResetFireCooldown()
{
	Super::ResetFireCooldown();
}

void UShotgun::Reload()
{
	Super::Reload();
}

void UShotgun::ApplyRecoil()
{
	Super::ApplyRecoil();
}

void UShotgun::FireShotgun()
{
    FRotator MuzzleRotation = PlayerCharacter->GetControlRotation();  // 발사 방향

    for (int32 i = 0; i < ShotgunPellets; ++i)
    {
        // 랜덤한 각도로 발사 범위를 만들어준다
        FRotator PelletDirection = MuzzleRotation + FRotator(
            FMath::RandRange(-SpreadAngle, SpreadAngle),  // X축 회전
            FMath::RandRange(-SpreadAngle, SpreadAngle),  // Y축 회전
            0.0f  // Z축 회전
        );

        // Raycast 방식으로 총알 발사
        FVector PelletDirectionVector = PelletDirection.Vector();  // 발사 방향 벡터
        FVector TraceStart = GetFireStartLocation();
        FVector TraceEnd = TraceStart + PelletDirectionVector * 10000.0f;  // 발사 거리 (총알의 최대 거리)

        // Raycast를 사용하여 충돌 감지
        FHitResult HitResult;
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(PlayerCharacter);

        // Raycast 실행
        if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_PhysicsBody, QueryParams))
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

                UGameplayStatics::ApplyDamage(
                    HitActor,
                    FinalDamage,
                    PlayerCharacter->GetController(),
                    PlayerCharacter,
                    UDamageType::StaticClass());
            }

            // Show debug at crash point
            DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 5.0f, FColor::Red, false, 2.0f);
            DrawDebugLine(GetWorld(), TraceStart, HitResult.ImpactPoint, FColor::Green, false, 2.0f, 0, 1.5f);
        }
        else
        {
            // If there is no collision, draw the debug line blue
            DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, 2.0f, 0, 1.5f);
        }
    }

    ApplyRecoil();
}
