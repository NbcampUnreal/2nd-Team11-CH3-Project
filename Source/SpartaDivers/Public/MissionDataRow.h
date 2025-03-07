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
    BossCombat UMETA(DisplayName = "BossCombat")       // 보스 전투
};

USTRUCT(BlueprintType)
struct FMissionDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
    // Type of Mission
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMissionType MissionType;
    // FName of MissionType
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName MissionName;
    // Reword Weapon
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName RewardWeapon;
    // Enemy Count
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EnemyCount;
    // Time Limit
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SurviveTime;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CaptureTime;
    
};
