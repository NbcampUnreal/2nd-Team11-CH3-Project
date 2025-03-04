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
    FRotator MuzzleRotation = PlayerCharacter->GetControlRotation();  // �߻� ����

    for (int32 i = 0; i < ShotgunPellets; ++i)
    {
        // ������ ������ �߻� ������ ������ش�
        FRotator PelletDirection = MuzzleRotation + FRotator(
            FMath::RandRange(-SpreadAngle, SpreadAngle),  // X�� ȸ��
            FMath::RandRange(-SpreadAngle, SpreadAngle),  // Y�� ȸ��
            0.0f  // Z�� ȸ��
        );

        // Raycast ������� �Ѿ� �߻�
        FVector PelletDirectionVector = PelletDirection.Vector();  // �߻� ���� ����
        FVector TraceStart = GetFireStartLocation();
        FVector TraceEnd = TraceStart + PelletDirectionVector * 10000.0f;  // �߻� �Ÿ� (�Ѿ��� �ִ� �Ÿ�)

        // Raycast�� ����Ͽ� �浹 ����
        FHitResult HitResult;
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(PlayerCharacter);

        // Raycast ����
        if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_PhysicsBody, QueryParams))
        {
            AActor* HitActor = HitResult.GetActor();
            if (HitActor && HitActor->ActorHasTag("Enemy"))
            {
                float FinalDamage = Damage; // �⺻ ������

                // ��弦 ���� �Ǻ�
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
