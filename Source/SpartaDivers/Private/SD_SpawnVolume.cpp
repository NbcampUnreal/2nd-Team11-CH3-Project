// Copyright Epic Games, Inc. All Rights Reserved.

#include "SD_SpawnVolume.h"
#include "Components/BoxComponent.h"


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

FEnemySpawnRow* ASD_SpawnVolume::GetRandomObject() const
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
	UE_LOG(LogTemp, Warning, TEXT("SpawnDataTable Changed"));
	CurrentSpawnDataTable = SetSpawnDataTable;
}

AActor* ASD_SpawnVolume::SpawnRandomEnemy(int32 MissionIndex)
{
	if (FEnemySpawnRow* SelectedRow = GetRandomObject())
	{
		if (UClass* ActualClass = SelectedRow->EnemyClass.Get())
		{
			return SpawnEnemy(ActualClass, MissionIndex);
		}
	}

	return nullptr;
}

AActor* ASD_SpawnVolume::SpawnEnemy(TSubclassOf<AActor> EnemyClass, int32 MissionIndex)
{
	if (!EnemyClass) return nullptr;

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
		EnemyClass,
		GetRandomPointInVolume(MissionIndex),
		GetRandomRotation()
	);

	return SpawnedActor;
}