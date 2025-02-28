// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapons/SniperRifle.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/OverlapResult.h"

USniperRifle::USniperRifle()
{
    //ItemName = FName(TEXT("SniperRifle"));
    //ItemDescription = FText::FromString(TEXT("SniperRifleDescription"));

    Damage = 100.0f;
    FireRate = 2.0f;
    MaxAmmo = 5;
    CurAmmo = MaxAmmo;
    ReloadTime = 2.0f;

    CurRecoil = 3.0f;
    Penetration = 2;
}

void USniperRifle::Fire()
{
    Super::Fire();

    PerformHitScanWithPenetration();
}

void USniperRifle::ResetFireCooldown()
{
    Super::ResetFireCooldown();
}

void USniperRifle::Reload()
{
    Super::Reload();
}

void USniperRifle::ApplyRecoil()
{
    Super::ApplyRecoil();
}

void USniperRifle::PerformHitScanWithPenetration()
{
    FVector Start = GetFireStartLocation();
    FVector End = GetFireEndLocation();

    // Start�� End ������ �Ÿ� ���
    float Distance = (End - Start).Size();

    // Box�� ���̸� Start�� End ������ �Ÿ��� ����
    FVector BoxExtent(Distance / 2.0f, 0.1f, 0.1f);  // Box�� ���̴� �Ÿ��� ����

    // Start�� End�� �����ϴ� �������� �ڽ��� �׸��ϴ�.
    FVector BoxCenter = (Start + End) / 2; // �ڽ��� �߽��� Start�� End ������ �߰���

    // Start�� End�� �����ϴ� ���⿡ �´� ȸ���� ��� (���� ������ Rotator�� ��ȯ)
    FRotator BoxRotation = (End - Start).Rotation();

    // �ڽ��� �׸��ϴ�.
    DrawDebugBox(GetWorld(), BoxCenter, BoxExtent, FQuat(BoxRotation), FColor::Red, false, 5.0f, 0, 5.5f);

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(PlayerCharacter);

    TArray<FOverlapResult> OverlapResults;

    bool bHit = GetWorld()->OverlapMultiByChannel(
        OverlapResults,             // ����� ������ �迭
        BoxCenter,                  // �ڽ��� �߽�
        FQuat(BoxRotation),            // ȸ����
        ECC_PhysicsBody, // ������Ʈ Ÿ�� (���� ��ü)
        FCollisionShape::MakeBox(BoxExtent)  // �ڽ��� ũ��
    );

    if (bHit)
    {
        int32 PenetrationCount = 0;
        TSet<AActor*> HitActors;     // �̹� ���� ���͵��� �����ϴ� Set

        // �÷��̾� ��ġ�� �������� ���� ����� ���ͺ��� ����
        FVector PlayerLocation = PlayerCharacter->GetActorLocation();
        OverlapResults.Sort([PlayerLocation](const FOverlapResult& A, const FOverlapResult& B)
        {
            // �� �������� ��ü�� �÷��̾� ���� �Ÿ� ���
            float DistanceA = FVector::Dist(A.GetActor()->GetActorLocation(), PlayerLocation);
            float DistanceB = FVector::Dist(B.GetActor()->GetActorLocation(), PlayerLocation);

            return DistanceA < DistanceB;  // ����� ������� ����
        });

        for (const FOverlapResult& Overlap : OverlapResults)
        {
            AActor* OverlappedActor = Overlap.GetActor();

            if (OverlappedActor)
            {
                // �浹�� ��ü�� �̸� ���
                UE_LOG(LogTemp, Warning, TEXT("Overlapping Actor: %s"), *OverlappedActor->GetName());

                if (OverlappedActor->ActorHasTag(TEXT("Enemy")))
                {
                    if (HitActors.Contains(OverlappedActor) == false)
                    {
                        UGameplayStatics::ApplyDamage(
                            OverlappedActor,
                            Damage,
                            PlayerCharacter->GetController(),
                            PlayerCharacter,
                            UDamageType::StaticClass());

                        PenetrationCount++;
                        HitActors.Add(OverlappedActor);  // �� ���ʹ� �̹� ���� ���ͷ� ���

                        if (PenetrationCount >= Penetration)
                        {
                            break;
                        }
                    }
                    else
                    {
                        UE_LOG(LogTemp, Error, TEXT("Already Exist Monster !"));
                    }
                }
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No overlapping actors detected!"));
    }

    ApplyRecoil();
}