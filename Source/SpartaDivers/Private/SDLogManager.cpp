// Copyright Epic Games, Inc. All Rights Reserved.

#include "SDLogManager.h"
#include "KillLogWidget.h"

USDLogManager* USDLogManager::Instance = nullptr;

USDLogManager* USDLogManager::Get()
{
    if (!Instance)
    {
        Instance = NewObject<USDLogManager>();
        Instance->AddToRoot();
    }
    return Instance;
}

UWorld* USDLogManager::GetWorld() const
{
    if (GEngine && GEngine->GameViewport)
    {
        return GEngine->GameViewport->GetWorld();
    }
    return nullptr;
}


void USDLogManager::AddKillLog(FString VictimName, FString WeaponName, bool bIsHeadshot)
{
    if (KillLogs.Num() >= 10) // 로그 최대 개수 제한
    {
        KillLogs.RemoveAt(0);
    }

    FKillLog NewLog;
    NewLog.VictimName = VictimName;
    NewLog.WeaponName = WeaponName;
    NewLog.bIsHeadshot = bIsHeadshot;
    NewLog.Timestamp = GetWorld()->GetTimeSeconds();

    KillLogs.Add(NewLog);

    if (KillLogWidget)
    {
        KillLogWidget->UpdateLogDisplay(KillLogs);
    }
    
    // 일정 시간 후 삭제
    GetWorld()->GetTimerManager().SetTimer(
        LogCleanupTimer, 
        this, 
        &USDLogManager::CleanupLogs,
        5.0f,
        false);
}

void USDLogManager::CleanupLogs()
{
    float CurrentTime = GetWorld()->GetTimeSeconds();
    KillLogs.RemoveAll([CurrentTime](const FKillLog& Log)
        {
            return (CurrentTime - Log.Timestamp) > 5.0f; // 5초 지난 로그 삭제
        });

    if (KillLogWidget)
    {
        KillLogWidget->UpdateLogDisplay(KillLogs);
    }
}
