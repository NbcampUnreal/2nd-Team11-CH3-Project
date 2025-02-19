// Copyright Epic Games, Inc. All Rights Reserved.

#include "MissionManager.h"
#include "MyGameState.h"
//#include "TestCharacterimkskku.h"
#include "Components/BoxComponent.h"

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
}

void AMissionManager::BeginPlay()
{
	Super::BeginPlay();
	StartMission();
}

void AMissionManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentMissionData.MissionType == EMissionType::Capture)
	{
		if (bIsPlayerInCaptureZone)
		{
			CaptureProgress += DeltaTime;
			UE_LOG(LogTemp, Log, TEXT("Capture Progress: %f / %f"), CaptureProgress, CurrentMissionData.CaptureTime);

			if (CaptureProgress >= CurrentMissionData.CaptureTime)
			{
				CompleteMission(true);
				bIsPlayerInCaptureZone = false;
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

	TArray<FMissionDataRow*> AllMissions;
	static const FString ContextString(TEXT("MissionDataContext"));
	MissionDataTable->GetAllRows(ContextString, AllMissions);

	UE_LOG(LogTemp, Warning, TEXT("Loaded MissionDataTable Rows: %d"), AllMissions.Num());

	if (AllMissions.IsValidIndex(CurrentMissionIndex))
	{
		CurrentMissionData = *AllMissions[CurrentMissionIndex];
		UE_LOG(LogTemp, Warning, TEXT("Mission %d started: %s"), CurrentMissionIndex, *UEnum::GetValueAsString(CurrentMissionData.MissionType));
		switch (CurrentMissionData.MissionType)
		{
		case EMissionType::Eliminate:
		{
			SpawnedEnemyCount = 0;
			KilledEnemyCount = 0;
			// Spawn enemies based on chance
			for (const auto& EnemySpawnChance : CurrentMissionData.EnemySpawnChances)
			{
				if (FMath::FRand() <= EnemySpawnChance.Value)
				{
					FVector SpawnLocation = GetActorLocation() + FVector(FMath::RandRange(-500, 500), FMath::RandRange(-500, 500), 0);
					FRotator SpawnRotation = FRotator::ZeroRotator;

					AActor* SpawnedEnemy = GetWorld()->SpawnActor<AActor>(EnemySpawnChance.Key, SpawnLocation, SpawnRotation);
					if (SpawnedEnemy)
					{
						SpawnedEnemyCount++;
						UE_LOG(LogTemp, Warning, TEXT("Spawned Enemy: %s at %s"), *SpawnedEnemy->GetName(), *SpawnLocation.ToString());
					}
				}
			}
			// if all enemies are dead
				// CompleteMission
			break;
		}
		case EMissionType::Survive:
		{
			// Spawn enemies based on chance
			for (const auto& EnemySpawnChance : CurrentMissionData.EnemySpawnChances)
			{
				UE_LOG(LogTemp, Warning, TEXT("Enemy %s spawned: %f"), *EnemySpawnChance.Key->GetName(), EnemySpawnChance.Value);
			}
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
			// Spawn enemies based on chance
			for (const auto& EnemySpawnChance : CurrentMissionData.EnemySpawnChances)
			{
				UE_LOG(LogTemp, Warning, TEXT("Enemy %s spawned: %f"), *EnemySpawnChance.Key->GetName(), EnemySpawnChance.Value);
			}
			// Check in  Tick() 
			break;
		}
		case EMissionType::Escape:
		{
			// Spawn enemies based on chance
			for (const auto& EnemySpawnChance : CurrentMissionData.EnemySpawnChances)
			{
				UE_LOG(LogTemp, Warning, TEXT("Enemy %s spawned: %f"), *EnemySpawnChance.Key->GetName(), EnemySpawnChance.Value);
			}
			// if Player Escape
			GetWorld()->GetTimerManager().SetTimer(
				EscapeTimerHandle,
				this,
				&AMissionManager::CheckMissionCompletion,
				CurrentMissionData.EscapeTimeLimit,
				false);
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

void AMissionManager::CompleteMission(bool bMissionSuccess)
{
	if (bMissionSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Mission %d Success!"), CurrentMissionIndex);

		if (AMyGameState* MyGameState = GetWorld()->GetGameState<AMyGameState>())
		{
			if (MyGameState)
			{
				MyGameState->AddScore(CurrentMissionData.ScoreReward);
			}
		}

		CurrentMissionIndex++;
		// 정비시간을 주고 StartMission

		StartMission();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Mission %d Failed!"), CurrentMissionIndex);
		CurrentMissionIndex++;
		// 정비시간을 주고 StartMission
		StartMission();
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
			/*if (ATestCharacterimkskku* PlayerCharacter = Cast<ATestCharacterimkskku>(OtherActor))
			{
				bIsPlayerInCaptureZone = true;
			}*/
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
			CompleteMission(true);
		}
		break;
	}
	case EMissionType::Survive:
	{
		if (1/*플레이어의 체력이 0보다 크면*/)
		{
			CompleteMission(true);
		}
		break;
	}
	case EMissionType::Capture:
	{
		if (CaptureProgress >= CurrentMissionData.CaptureTime)
		{
			CompleteMission(true);
		}
		break;
	}
	case EMissionType::Escape:
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			APawn* PlayerPawn = PlayerController->GetPawn();
			FVector CurrentLocation = PlayerPawn->GetActorLocation();
			UE_LOG(LogTemp, Warning, TEXT("Current Location : %s"), *CurrentLocation.ToString());
			if (PlayerPawn && FVector::Dist(PlayerPawn->GetActorLocation(), CurrentMissionData.TargetLocation) <= 500.0f)
			{
				CompleteMission(true);
			}
			else if (PlayerPawn && FVector::Dist(PlayerPawn->GetActorLocation(), CurrentMissionData.TargetLocation) > 500.0f)
			{
				CompleteMission(false);
			}
		}
		break;
	}
	default:
		break;
	}
}
