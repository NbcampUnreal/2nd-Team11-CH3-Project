// Copyright Epic Games, Inc. All Rights Reserved.

#include "SD_SpawnVolume.h"
#include "Enemy/BossEnemy.h"
#include "Components/BoxComponent.h"
#include "MissionManager.h"
#include "Kismet/GameplayStatics.h"

ASD_SpawnVolume::ASD_SpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);
	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningArea"));
	SpawningBox->SetupAttachment(RootComponent);
}

FVector ASD_SpawnVolume::GetRandomPointInVolume(int32 MissionIndex) const
{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawningBox->GetComponentLocation();

	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
	);
}

FRotator ASD_SpawnVolume::GetRandomRotation() const
{
	FRotator RandomRotation = FRotator::ZeroRotator;
	RandomRotation.Yaw = FMath::FRandRange(-180.0f, 180.0f);
	return RandomRotation;
}

FEnemySpawnRow* ASD_SpawnVolume::GetRandomEnemy() const
{
	if (!CurrentSpawnDataTable) return nullptr;

	TArray<FEnemySpawnRow*> AllRows;
	static const FString ContextString(TEXT("EnemySpawnContext"));
	CurrentSpawnDataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty()) return nullptr;

	float TotalChance = 0.0f;
	for (const FEnemySpawnRow* Row : AllRows)
	{
		if (Row)
		{
			TotalChance += Row->SpawnChance;
		}
	}

	const float RandValue = FMath::FRandRange(0.0f, TotalChance);
	float AccumulateChance = 0.0f;

	for (FEnemySpawnRow* Row : AllRows)
	{
		AccumulateChance += Row->SpawnChance;
		if (RandValue <= AccumulateChance)
		{
			return Row;
		}
	}

	return nullptr;
}

EMissionType ASD_SpawnVolume::GetMissionType() const
{
	return AssignedMissionType;
}

void ASD_SpawnVolume::SetCurrentSpawnDataTable(UDataTable* SetSpawnDataTable)
{
	CurrentSpawnDataTable = SetSpawnDataTable;
}

AActor* ASD_SpawnVolume::SpawnRandomEnemy(int32 MissionIndex)
{
	static bool bBossSpawned = false;

	if (!bBossSpawned && MissionIndex == 3)
	{
		AActor* Boss = SpawnBoss();
		if (Boss)
		{
			bBossSpawned = true;
			return Boss; // 보스만 스폰하고 종료
		}
	}

	if (FEnemySpawnRow* SelectedRow = GetRandomEnemy())
	{
		if (UClass* ActualClass = SelectedRow->EnemyClass.Get())
		{
			return SpawnEnemy(ActualClass, MissionIndex);
		}
	}

	return nullptr;
}


AActor* ASD_SpawnVolume::SpawnBoss()
{
	BossEnemyClass = LoadClass<AActor>(nullptr, TEXT("/Game/_Blueprint/Enemy/Boss/BP_Boss.BP_Boss_C"));
	if (!BossEnemyClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load BossEnemyClass!"));
		return nullptr;
	}

	FVector BossSpawnLocation = GetActorLocation(); // 스폰 볼륨의 위치에 스폰
	FRotator BossSpawnRotation = FRotator::ZeroRotator;

	AActor* SpawnedBoss = GetWorld()->SpawnActor<AActor>(BossEnemyClass, BossSpawnLocation, BossSpawnRotation);
	if (SpawnedBoss)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawned Boss at %s"), *BossSpawnLocation.ToString());
		SpawnedBoss->Tags.Add("Boss");
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn Boss!"));
	}

	return SpawnedBoss;
}

AActor* ASD_SpawnVolume::SpawnEnemy(TSubclassOf<AActor> EnemyClass, int32 MissionIndex)
{
	if (!EnemyClass) return nullptr;

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
		EnemyClass,
		GetRandomPointInVolume(MissionIndex),
		GetRandomRotation()
	);
	// Add MissionName Tag
	if (SpawnedActor)
	{
		AMissionManager* MissionManager = Cast<AMissionManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AMissionManager::StaticClass()));
		if (MissionManager)
		{
			if (!SpawnedActor->Tags.Contains(MissionManager->CurrentMissionData.MissionName))
			{
				SpawnedActor->Tags.Add(MissionManager->CurrentMissionData.MissionName);
			}
		}
	}

	return SpawnedActor;
}