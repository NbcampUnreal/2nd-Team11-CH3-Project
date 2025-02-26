// Copyright Epic Games, Inc. All Rights Reserved.

#include "SDEnemyBase.h"
#include "SDAIController.h"
#include "MissionManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Item/DropItem.h"
#include "Item/ItemBase.h"

ASDEnemyBase::ASDEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ASDAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	EnemyType = FName(TEXT("DefaultEnemy"));
	MoveSpeed = 300.f;
	MaxHP = 100.0f;
	CurrentHP = MaxHP;
}

float ASDEnemyBase::GetHealth() const
{
	return CurrentHP;
}

float ASDEnemyBase::GetMaxHealth() const
{
	return MaxHP;
}

void ASDEnemyBase::AddHealth(float Amount)
{
	CurrentHP = FMath::Clamp(CurrentHP + Amount, 0.0f, MaxHP);
}

FName ASDEnemyBase::GetEnemyType() const
{
	return EnemyType;
}

float ASDEnemyBase::TakeDamage(
	float DamageAmount,
	FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return ActualDamage;
}

void ASDEnemyBase::Attack()
{
}

void ASDEnemyBase::OnDeath()
{
	OnDropItem();

	Super::OnDeath();
	AMissionManager* MissionManager = Cast<AMissionManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AMissionManager::StaticClass()));
	if (MissionManager && MissionManager->CurrentMissionData.MissionType == EMissionType::Eliminate)
	{
		MissionManager->KilledEnemyCount++;
		UE_LOG(LogTemp, Warning, TEXT("KilledEnemyCount : %d"), MissionManager->KilledEnemyCount);
		MissionManager->CheckMissionCompletion();
	}

	Destroy();
}

void ASDEnemyBase::OnDropItem()
{
	float CumDropWeight = 0.f;

	for (FDropItemInfo dropItemInfo : DropItemInfos)
	{
		CumDropWeight += dropItemInfo.dropWeight;
	}

	float RandomWeight = FMath::FRandRange(0.f, CumDropWeight);
	for (FDropItemInfo dropItemInfo : DropItemInfos)
	{
		RandomWeight -= dropItemInfo.dropWeight;
		if (RandomWeight < 0.f)
		{
			if (dropItemInfo.dropItemClass)
			{
				ADropItem* DropItemInstance = GetWorld()->SpawnActor<ADropItem>(DropItem);
				DropItemInstance->OwningItemClass = dropItemInfo.dropItemClass;
			}
		}

		break;
	}
}

void ASDEnemyBase::ApplyAttackEffect(int32 EffectIndex)
{
}

void ASDEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	this->Tags.Add(TEXT("Enemy"));
}

void ASDEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

