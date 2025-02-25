// Copyright Epic Games, Inc. All Rights Reserved.

#include "MissionManager.h"
#include "MyGameState.h"
#include "PlayerCharacter.h"
#include "Components/BoxComponent.h"
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
			UE_LOG(LogTemp, Log, TEXT("Capture Progress: %f / %f"), CaptureProgress, CurrentMissionData.CaptureTime);

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

	AMyGameState* MyGameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
	if (MyGameState)
	{
		MyGameState->UpdateHUD();
	}

	if (MissionStartTrigger)
	{
		MissionStartTrigger->DeactivateTrigger();
	}

	if (AllMissions.IsValidIndex(CurrentMissionIndex))
	{
		bIsPlayerOnMission = true;
		CurrentMissionData = *AllMissions[CurrentMissionIndex];
		UE_LOG(LogTemp, Warning, TEXT("Mission %d started: %s"), CurrentMissionIndex, *UEnum::GetValueAsString(CurrentMissionData.MissionType));
		switch (CurrentMissionData.MissionType)
		{
		case EMissionType::Eliminate:
		{
			SpawnedEnemyCount = 0;
			KilledEnemyCount = 0;
			SpawnEnemy();
			// Check When Enemy Dead
			break;
		}
		case EMissionType::Survive:
		{
			SpawnEnemy();
			/*GetWorld()->GetTimerManager().SetTimer(
				SurvivalTimerHandle,
				this,
				&AMissionManager::CheckMissionCompletion,
				CurrentMissionData.SurviveTime,
				false);*/
			break;
		}
		case EMissionType::Capture:
		{
			SpawnEnemy();
			break;
		}
		case EMissionType::BossCombat:
		{
			SpawnEnemy();
			// Check When Boss Dead
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
	UE_LOG(LogTemp, Warning, TEXT("Mission %d Success!"), CurrentMissionIndex);
	bIsPlayerOnMission = false;

	if (AMyGameState* MyGameState = GetWorld()->GetGameState<AMyGameState>())
	{
		if (MyGameState)
		{
			MyGameState->AddScore(CurrentMissionData.ScoreReward);
		}
	}
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMissionStartTrigger::StaticClass(), FoundMissionStartTriggers);
	for (AActor* FoundMissionStartTrigger : FoundMissionStartTriggers)
	{
		MissionStartTrigger = Cast<AMissionStartTrigger>(FoundMissionStartTrigger);

		if (MissionStartTrigger)
		{
			MissionStartTrigger->ActivateTrigger();
		}
	}

	DestroyAllEnemies();

	CurrentMissionIndex++;
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
	bIsPlayerInCaptureZone = false;
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
			if (1/*플레이어의 체력이 0보다 크면*/)
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
			if (1/*플레이어의 체력이 0보다 크면*/)
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
	// 게임 내의 모든 SpawnVolume을 찾음
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

		// 적절한 SpawnVolume이 있는 경우 적 소환
		if (MatchingVolumes.Num() > 0)
		{
			for (int32 i = 0; i < EnemiesToSpawn; i++)
			{
				ASD_SpawnVolume* SelectedVolume = MatchingVolumes[FMath::RandRange(0, MatchingVolumes.Num() - 1)];
				if (SelectedVolume)
				{
					SelectedVolume->SetCurrentSpawnDataTable(SpawnDataTables[CurrentMissionIndex]);
					SelectedVolume->SpawnRandomEnemy(CurrentMissionIndex);

					UE_LOG(LogTemp, Warning, TEXT("Enemy Spawned from Selected Volume"));
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Matching SpawnVolume found for Mission Type"));
		}
	}
}

void AMissionManager::DestroyAllEnemies()
{
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASDEnemyBase::StaticClass(), FoundEnemies);

	for (AActor* Enemy : FoundEnemies)
	{
		if (Enemy)
		{
			Enemy->Destroy();
		}
	}
}