// Copyright Epic Games, Inc. All Rights Reserved.

#include "MissionManager.h"
#include "MyGameState.h"
#include "PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StatusContainerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MissionStartTrigger.h"
#include "SDEnemyBase.h"
#include "SD_SpawnVolume.h"

AMissionManager::AMissionManager()
{
	PrimaryActorTick.bCanEverTick = true;

	CaptureZone = CreateDefaultSubobject<UBoxComponent>(TEXT("CaptureZone"));
	CaptureZone->SetupAttachment(RootComponent);
	CaptureZone->OnComponentBeginOverlap.AddDynamic(this, &AMissionManager::OnObjectOverlap);
	CaptureZone->OnComponentEndOverlap.AddDynamic(this, &AMissionManager::OnObjectEndOverlap);

	CurrentMissionIndex = 0;
	MaxMissionCount = 4;
	CaptureProgress = 0.0f;
	bIsPlayerInCaptureZone = false;
	bIsPlayerOnMission = false;
}

void AMissionManager::BeginPlay()
{
	Super::BeginPlay();
}

void AMissionManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Measure Capture progress
	if (CurrentMissionData.MissionType == EMissionType::Capture)
	{
		if (bIsPlayerInCaptureZone && bIsPlayerOnMission)
		{
			CaptureProgress += DeltaTime;
			if (CaptureProgress >= CurrentMissionData.CaptureTime)
			{
				CheckMissionCompletion();
			}
		}
		else
		{
			CaptureProgress = FMath::Max(0.0f, CaptureProgress - (DeltaTime * 2));
		}
	}
}

void AMissionManager::StartMission()
{
	if (!MissionDataTable) return;
	if (bIsPlayerOnMission) return;

	TArray<FMissionDataRow*> AllMissions;
	static const FString ContextString(TEXT("MissionDataContext"));
	MissionDataTable->GetAllRows(ContextString, AllMissions);

	if (AllMissions.IsValidIndex(CurrentMissionIndex))
	{
		bIsPlayerOnMission = true;
		CurrentMissionData = *AllMissions[CurrentMissionIndex];
		switch (CurrentMissionData.MissionType)
		{
		case EMissionType::Eliminate:
		{
			SpawnedEnemyCount = 0;
			KilledEnemyCount = 0;

			TArray<AActor*> FoundAlreadySpawnedEnemies;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASDEnemyBase::StaticClass(), FoundAlreadySpawnedEnemies);
			for (AActor* Enemy : FoundAlreadySpawnedEnemies)
			{
				if (Enemy->ActorHasTag(CurrentMissionData.MissionName))
				{
					SpawnedEnemyCount++;
				}
			}

			SpawnEnemy();
			TArray<AActor*> FoundNewlySpawnedEnemies;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASDEnemyBase::StaticClass(), FoundNewlySpawnedEnemies);
			for (AActor* Enemy : FoundNewlySpawnedEnemies)
			{
				if (Enemy->ActorHasTag(CurrentMissionData.MissionName))
				{
					SpawnedEnemyCount++;
				}
			}
			break;
		}
		case EMissionType::Survive:
		{
			GetWorld()->GetTimerManager().SetTimer(
				SpawnTimerHandle,
				this,
				&AMissionManager::SpawnEnemy,
				1.0f,
				true);
			GetWorld()->GetTimerManager().SetTimer(
				SurvivalTimerHandle,
				this,
				&AMissionManager::CheckMissionCompletion,
				CurrentMissionData.SurviveTime,
				false);
			break;
		}
		case EMissionType::Capture:
		{
			GetWorld()->GetTimerManager().SetTimer(
				SpawnTimerHandle,
				this,
				&AMissionManager::SpawnEnemy,
				3.0f,
				true);
			break;
		}
		case EMissionType::BossCombat:
		{
			SpawnBoss();
			break;
		}
		default:
			break;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Mission Index: %d"), CurrentMissionIndex);
	}
}

void AMissionManager::CompleteMission()
{
	// Stop spawning
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);

	bIsPlayerOnMission = false;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMissionStartTrigger::StaticClass(), FoundMissionStartTriggers);
	for (AActor* FoundMissionStartTrigger : FoundMissionStartTriggers)
	{
		MissionStartTrigger = Cast<AMissionStartTrigger>(FoundMissionStartTrigger);

		if (MissionStartTrigger)
		{
			MissionStartTrigger->ActivateTrigger();
		}
	}

	DestroyEnemiesInCurrentMission(CurrentMissionIndex);
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetPawn()))
		{
			PlayerCharacter->GetGunItem(CurrentMissionData.RewardWeapon);
		}
	}

	CurrentMissionIndex++;

	if (CurrentMissionIndex == MaxMissionCount)
	{
		AMyGameState* MyGameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
		if (MyGameState)
		{
			MyGameState->OnGameOver();
		}
	}
}

void AMissionManager::OnObjectOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	TArray<AActor*> OverlappingActors;
	CaptureZone->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (OtherActor && OtherActor->ActorHasTag("Player") && !bIsPlayerInCaptureZone)
		{
			if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
			{
				bIsPlayerInCaptureZone = true;
			}
		}
	}
}

void AMissionManager::OnObjectEndOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	TArray<AActor*> OverlappingActors;
	CaptureZone->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (OtherActor && OtherActor->ActorHasTag("Player") && bIsPlayerInCaptureZone)
		{
			if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
			{
				bIsPlayerInCaptureZone = false;
			}
		}
	}
}

void AMissionManager::CheckMissionCompletion()
{
	switch (CurrentMissionData.MissionType)
	{
	case EMissionType::Eliminate:
	{
		if (KilledEnemyCount >= SpawnedEnemyCount)
		{
			CompleteMission();
		}
		break;
	}
	case EMissionType::Survive:
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			APlayerCharacter* Player = Cast<APlayerCharacter>(PlayerController->GetPawn());
			if (Player->GetStatusContainerComponent()->GetCurHealth() >= 0)
			{
				CompleteMission();
			}
		}
		break;
	}
	case EMissionType::Capture:
	{
		if (CaptureProgress >= CurrentMissionData.CaptureTime)
		{
			CompleteMission();
			bIsPlayerInCaptureZone = false;
		}
		break;
	}
	case EMissionType::BossCombat:
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			APlayerCharacter* Player = Cast<APlayerCharacter>(PlayerController->GetPawn());
			if (Player->GetStatusContainerComponent()->GetCurHealth() >= 0)
			{
				CompleteMission();
			}
		}
		break;
	}
	default:
		break;
	}
}

void AMissionManager::SpawnEnemy()
{
	// Finds all SpawnVolumes in the game
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASD_SpawnVolume::StaticClass(), FoundVolumes);

	if (FoundVolumes.Num() > 0 && SpawnDataTables.IsValidIndex(CurrentMissionIndex))
	{
		const int32 EnemiesToSpawn = CurrentMissionData.EnemyCount;
		TArray<ASD_SpawnVolume*> MatchingVolumes;

		// Filter only SpawnVolumes that match the current mission type
		for (AActor* VolumeActor : FoundVolumes)
		{
			if (ASD_SpawnVolume* SpawnVolume = Cast<ASD_SpawnVolume>(VolumeActor))
			{
				if (SpawnVolume->GetMissionType() == CurrentMissionData.MissionType)
				{
					MatchingVolumes.Add(SpawnVolume);
				}
			}
		}

		// Spawn enemies if the appropriate SpawnVolume exists
		if (MatchingVolumes.Num() > 0)
		{
			ASD_SpawnVolume* SelectedVolume = MatchingVolumes[FMath::RandRange(0, MatchingVolumes.Num() - 1)];
			SelectedVolume->SetCurrentSpawnDataTable(SpawnDataTables[CurrentMissionIndex]);
			for (int32 i = 0; i < EnemiesToSpawn; i++)
			{
				SelectedVolume = MatchingVolumes[FMath::RandRange(0, MatchingVolumes.Num() - 1)];
				if (SelectedVolume)
				{
					SelectedVolume->SpawnRandomEnemy(CurrentMissionIndex);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Matching SpawnVolume found for Mission Type"));
		}
	}
}

void AMissionManager::SpawnBoss()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASD_SpawnVolume::StaticClass(), FoundVolumes);

	if (FoundVolumes.Num() > 0 && SpawnDataTables.IsValidIndex(CurrentMissionIndex))
	{
		const int32 EnemiesToSpawn = CurrentMissionData.EnemyCount;
		TArray<ASD_SpawnVolume*> MatchingVolumes;

		// Filter only SpawnVolumes that match the current mission type
		for (AActor* VolumeActor : FoundVolumes)
		{
			if (ASD_SpawnVolume* SpawnVolume = Cast<ASD_SpawnVolume>(VolumeActor))
			{
				if (SpawnVolume->GetMissionType() == CurrentMissionData.MissionType)
				{
					MatchingVolumes.Add(SpawnVolume);
				}
			}
		}

		// Spawn enemies if the appropriate SpawnVolume exists
		if (MatchingVolumes.Num() > 0)
		{
			ASD_SpawnVolume* SelectedVolume = MatchingVolumes[FMath::RandRange(0, MatchingVolumes.Num() - 1)];
			SelectedVolume->SetCurrentSpawnDataTable(SpawnDataTables[CurrentMissionIndex]);
			for (int32 i = 0; i < EnemiesToSpawn; i++)
			{
				SelectedVolume = MatchingVolumes[FMath::RandRange(0, MatchingVolumes.Num() - 1)];
				if (SelectedVolume)
				{
					SelectedVolume->SpawnBoss();
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Matching SpawnVolume found for Mission Type"));
		}
	}
}

void AMissionManager::DestroyEnemiesInCurrentMission(int MissionIndex)
{
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASDEnemyBase::StaticClass(), FoundEnemies);

	for (AActor* Enemy : FoundEnemies)
	{
		if (Enemy->ActorHasTag(CurrentMissionData.MissionName))
		{
			Enemy->Destroy();
		}
	}
}