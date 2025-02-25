// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MissionDataRow.generated.h"

UENUM(BlueprintType)
enum class EMissionType : uint8
{
    Eliminate UMETA(DisplayName = "Eliminate"), // �� ���� ����
    Survive UMETA(DisplayName = "Survive"),     // ���� �ð� ����
    Capture UMETA(DisplayName = "Capture"),     // ����
    BossCombat UMETA(DisplayName = "BossCombat")       // ���� ����
};

USTRUCT(BlueprintType)
struct FMissionDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
    // Text for UI
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText MissionText;
    // Type of Mission
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMissionType MissionType;
    // Score
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ScoreReward;
    // Enemy Count
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EnemyCount;
    // Time Limit
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SurviveTime;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CaptureTime;
    
};
