// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "MissionDataRow.h"
#include "MissionManager.generated.h"

class UBoxComponent;
class AMissionStartTrigger;
UCLASS()
class SPARTADIVERS_API AMissionManager : public AActor
{
	GENERATED_BODY()

public:
	AMissionManager();

	// Survive TimerHandle
	FTimerHandle SurvivalTimerHandle;
	// RestTime before next Mission
	FTimerHandle NextMissionTimerHandle;

	// Enemy SpawnVolumes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TArray<AActor*> FoundVolumes;
	// Enemy Spawn DataTable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TArray<UDataTable*> SpawnDataTables;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MissionTrigger")
	TArray<AActor*> FoundMissionStartTriggers;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MissionTrigger")
	AMissionStartTrigger* MissionStartTrigger;

	// Mission DataTable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
	UDataTable* MissionDataTable;
	// Current Mission DataRow
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mission")
	FMissionDataRow CurrentMissionData;
	// Current Mission Index
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mission")
	int32 CurrentMissionIndex;
	// Max Mission Count
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mission")
	int32 MaxMissionCount;

	// Get All Rows and Set MaxMissionCount
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Mission")
	void StartMission();
	UFUNCTION(BlueprintCallable, Category = "Mission")
	void CompleteMission();
	UFUNCTION(BlueprintPure, Category = "Mission")
	FText GetCurrentMissionText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
	bool bIsPlayerOnMission;

	// Spawn enemies based on chance in SpawnDataTables
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnEnemy();
	// Destoy all enemies when Complete Mission
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void DestroyAllEnemies();

	// ============== Eliminate ==============
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Eliminate")
	int32 SpawnedEnemyCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Eliminate")
	int32 KilledEnemyCount;
	// ========================================

	// ============== Capture ============== 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Capture")
	float CaptureProgress;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capture")
	UBoxComponent* CaptureZone;
	UFUNCTION()
	virtual void OnObjectOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	UFUNCTION()
	void OnObjectEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
	// ========================================

private:
	/* ============== Capture ============== */
	bool bIsPlayerInCaptureZone;
	/* ======================================== */
	
	void CheckMissionCompletion();

};
