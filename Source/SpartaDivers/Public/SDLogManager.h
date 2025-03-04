// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SDLogManager.generated.h"

USTRUCT(BlueprintType)
struct FKillLog
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString VictimName;

    UPROPERTY(BlueprintReadWrite)
    FString WeaponName;

    UPROPERTY(BlueprintReadWrite)
    bool bIsHeadshot;

    UPROPERTY(BlueprintReadWrite)
    float Timestamp; // 로그가 생성된 시간
};

class UKillLogWidget;

UCLASS()
class SPARTADIVERS_API USDLogManager : public UObject
{
	GENERATED_BODY()

private:
    UPROPERTY()
    TArray<FKillLog> KillLogs; // 스택 형태의 로그

    FTimerHandle LogCleanupTimer;

    void CleanupLogs();

    static USDLogManager* Instance;

public:
    static USDLogManager* Get();

    UWorld* GetWorld() const;

    UPROPERTY()
    UKillLogWidget* KillLogWidget;

    void AddKillLog(FString VictimName, FString WeaponName, bool bIsHeadshot);
    void SetKillLogWidget(UKillLogWidget* NewWidget) { KillLogWidget = NewWidget; }
    
    UFUNCTION(BlueprintCallable)
    const TArray<FKillLog>& GetLogs() const { return KillLogs; }
};
