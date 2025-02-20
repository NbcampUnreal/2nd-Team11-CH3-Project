// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "MissionDataRow.h"
#include "MissionManager.generated.h"

class UBoxComponent;
UCLASS()
class SPARTADIVERS_API AMissionManager : public AActor
{
    GENERATED_BODY()

public:
    AMissionManager();

    FTimerHandle SurvivalTimerHandle;
    FTimerHandle EscapeTimerHandle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
    UDataTable* MissionDataTable;
    UPROPERTY(BlueprintReadOnly, Category = "Mission")
    FMissionDataRow CurrentMissionData;
    UPROPERTY(BlueprintReadOnly, Category = "Mission")
    int32 CurrentMissionIndex;
    UPROPERTY(BlueprintReadOnly, Category = "Mission")
    int32 MaxMissionCount;

    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Mission")
    void StartMission();

    UFUNCTION(BlueprintCallable, Category = "Mission")
    void CompleteMission(bool bMissionSuccess);

    /* ============== Eliminate ============== */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Eliminate")
    int32 SpawnedEnemyCount;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Eliminate")
    int32 KilledEnemyCount;
    /* ======================================== */

    /* ============== Capture ============== */
    UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Capture")
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
    /* ======================================== */

private:
    /* ============== Capture ============== */
    bool bIsPlayerInCaptureZone;
    /* ======================================== */

    void CheckMissionCompletion();

};
