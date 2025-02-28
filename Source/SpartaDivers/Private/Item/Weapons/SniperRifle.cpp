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

    // Start와 End 사이의 거리 계산
    float Distance = (End - Start).Size();

    // Box의 길이를 Start와 End 사이의 거리로 설정
    FVector BoxExtent(Distance / 2.0f, 0.1f, 0.1f);  // Box의 길이는 거리의 절반

    // Start와 End를 연결하는 방향으로 박스를 그립니다.
    FVector BoxCenter = (Start + End) / 2; // 박스의 중심은 Start와 End 사이의 중간점

    // Start와 End를 연결하는 방향에 맞는 회전값 계산 (벡터 방향을 Rotator로 변환)
    FRotator BoxRotation = (End - Start).Rotation();

    // 박스를 그립니다.
    DrawDebugBox(GetWorld(), BoxCenter, BoxExtent, FQuat(BoxRotation), FColor::Red, false, 5.0f, 0, 5.5f);

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(PlayerCharacter);

    TArray<FOverlapResult> OverlapResults;

    bool bHit = GetWorld()->OverlapMultiByChannel(
        OverlapResults,             // 결과를 저장할 배열
        BoxCenter,                  // 박스의 중심
        FQuat(BoxRotation),            // 회전값
        ECC_PhysicsBody, // 오브젝트 타입 (물리 본체)
        FCollisionShape::MakeBox(BoxExtent)  // 박스의 크기
    );

    if (bHit)
    {
        int32 PenetrationCount = 0;
        TSet<AActor*> HitActors;     // 이미 때린 액터들을 추적하는 Set

        // 플레이어 위치를 기준으로 가장 가까운 몬스터부터 정렬
        FVector PlayerLocation = PlayerCharacter->GetActorLocation();
        OverlapResults.Sort([PlayerLocation](const FOverlapResult& A, const FOverlapResult& B)
        {
            // 각 오버랩된 객체와 플레이어 간의 거리 계산
            float DistanceA = FVector::Dist(A.GetActor()->GetActorLocation(), PlayerLocation);
            float DistanceB = FVector::Dist(B.GetActor()->GetActorLocation(), PlayerLocation);

            return DistanceA < DistanceB;  // 가까운 순서대로 정렬
        });

        for (const FOverlapResult& Overlap : OverlapResults)
        {
            AActor* OverlappedActor = Overlap.GetActor();

            if (OverlappedActor)
            {
                // 충돌된 객체의 이름 출력
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
                        HitActors.Add(OverlappedActor);  // 이 액터는 이미 때린 액터로 기록

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