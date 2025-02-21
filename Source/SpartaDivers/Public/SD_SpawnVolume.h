// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnRow.h"
#include "SD_SpawnVolume.generated.h"

class UBoxComponent;
UCLASS()
class SPARTADIVERS_API ASD_SpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ASD_SpawnVolume();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    USceneComponent* Scene;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    UBoxComponent* SpawningBox;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    UDataTable* CurrentSpawnDataTable;
    UFUNCTION(BlueprintCallable, Category = "Spawning")
    void SetCurrentSpawnDataTable(UDataTable* SetSpawnDataTable);
    UFUNCTION(BlueprintCallable, Category = "Spawning")
    AActor* SpawnRandomEnemy(int32 MissionIndex);
    UFUNCTION(BlueprintCallable, Category = "Spawning")
    FVector GetRandomPointInVolume(int32 MissionIndex) const;
    UFUNCTION(BlueprintCallable, Category = "Spawning")
    FRotator GetRandomRotation() const;
    UFUNCTION(BlueprintCallable, Category = "Spawning")
    AActor* SpawnEnemy(TSubclassOf<AActor> EnemyClass, int32 MissionIndex);
    FEnemySpawnRow* GetRandomObject() const;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MissionType")
    EMissionType AssignedMissionType;
    UFUNCTION(BlueprintCallable, Category = "MissionType")
    EMissionType GetMissionType() const;

};
