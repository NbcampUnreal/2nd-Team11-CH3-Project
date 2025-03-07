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

	// Start�� End ������ �Ÿ� ���
	float Distance = (End - Start).Size();
	FVector BoxExtent(Distance / 2.0f, 0.1f, 0.1f);  // �ڽ� ũ�� ����

	FVector BoxCenter = (Start + End) / 2;
	FRotator BoxRotation = (End - Start).Rotation();

	// �ڽ� �׸��� (������)
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

		// �÷��̾� ��ġ�� �������� ����
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
						// �浹�� ���Ͱ� ����ƽ �޽ø� ������ �ִ��� Ȯ���ϰ�,
						// "HeadHitbox"�� �ش��ϴ� ������ �浹���� �� �߰� �������� ����
						if (Overlap.Component->GetFName() == FName("HeadHitbox"))
						{
							bHitHead = true;
							Damage *= 2.0f; // ��弦 ������
						}
						else
						{
							bHitHead = false;
						}
						
						// �Ϲ� ������ ����
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