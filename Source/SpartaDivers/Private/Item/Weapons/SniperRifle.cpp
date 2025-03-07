// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapons/SniperRifle.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/OverlapResult.h"

USniperRifle::USniperRifle()
{
	ItemName = FName(TEXT("SniperRifle"));
	//ItemDescription = FText::FromString(TEXT("SniperRifleDescription"));

	Damage = 100.0f;
	FireRate = 1.8f;
	MaxAmmo = 10;
	CurAmmo = MaxAmmo;
	ReloadTime = 3.0f;

	CurRecoil = 3.0f;
	Penetration = 2;
}

void USniperRifle::Fire()
{
	Super::Fire();
	if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		MyGameInstance->SniperBulletCount++;
	}
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
	FVector BoxExtent(Distance / 2.0f, 0.1f, 0.1f);  // 박스 크기 설정

	FVector BoxCenter = (Start + End) / 2;
	FRotator BoxRotation = (End - Start).Rotation();

	// 박스 그리기 (디버깅용)
	//DrawDebugBox(GetWorld(), BoxCenter, BoxExtent, FQuat(BoxRotation), FColor::Red, false, 5.0f, 0, 5.5f);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(PlayerCharacter);

	TArray<FOverlapResult> OverlapResults;
	bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		BoxCenter,
		FQuat(BoxRotation),
		ECC_PhysicsBody,
		FCollisionShape::MakeBox(BoxExtent)
	);

	if (bHit)
	{
		int32 PenetrationCount = 0;
		TSet<AActor*> HitActors;

		// 플레이어 위치를 기준으로 정렬
		FVector PlayerLocation = PlayerCharacter->GetActorLocation();
		OverlapResults.Sort([PlayerLocation](const FOverlapResult& A, const FOverlapResult& B)
			{
				float DistanceA = FVector::Dist(A.GetActor()->GetActorLocation(), PlayerLocation);
				float DistanceB = FVector::Dist(B.GetActor()->GetActorLocation(), PlayerLocation);
				return DistanceA < DistanceB;
			});

		for (const FOverlapResult& Overlap : OverlapResults)
		{
			AActor* OverlappedActor = Overlap.GetActor();

			if (OverlappedActor)
			{
				UE_LOG(LogTemp, Warning, TEXT("Overlapping Actor: %s"), *OverlappedActor->GetName());

				if (OverlappedActor->ActorHasTag(TEXT("Enemy")))
				{
					if (HitActors.Contains(OverlappedActor) == false)
					{
						// 충돌된 액터가 스태틱 메시를 가지고 있는지 확인하고,
						// "HeadHitbox"에 해당하는 부위를 충돌했을 때 추가 데미지를 적용
						if (Overlap.Component->GetFName() == FName("HeadHitbox"))
						{
							bHitHead = true;
							Damage *= 2.0f; // 헤드샷 데미지
						}
						else
						{
							bHitHead = false;
						}
						
						// 일반 데미지 적용
						UGameplayStatics::ApplyDamage(
							OverlappedActor,
							Damage * FMath::FRandRange(0.9f, 1.1f),
							PlayerCharacter->GetController(),
							PlayerCharacter,
							UDamageType::StaticClass()
						);

						PenetrationCount++;
						HitActors.Add(OverlappedActor);

						if (PenetrationCount >= Penetration)
						{
							break;
						}
					}
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No overlapping actors detected!"));
	}

	//ApplyRecoil();
}