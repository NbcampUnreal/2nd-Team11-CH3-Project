// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MissionDataRow.generated.h"

UENUM(BlueprintType)
enum class EMissionType : uint8
{
    Eliminate UMETA(DisplayName = "Eliminate"), // 적 전부 섬멸
    Survive UMETA(DisplayName = "Survive"),     // 일정 시간 생존
    Capture UMETA(DisplayName = "Capture"),     // 점령
    Escape UMETA(DisplayName = "Escape")       // 탈출
};

USTRUCT(BlueprintType)
struct FMissionDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
    // Type of Mission
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMissionType MissionType;
    // Chance for Spawning Enemies
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<TSubclassOf<AActor>, float> EnemySpawnChances;
    // Score
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ScoreReward;
    // Time Limit
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SurviveTime;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EscapeTimeLimit;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CaptureTime;
    // Destinations for Escape
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetLocation;
};
