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
    if (KillLogs.Num() >= 10) // �α� �ִ� ���� ����
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
    
    // ���� �ð� �� ����
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
            return (CurrentTime - Log.Timestamp) > 5.0f; // 5�� ���� �α� ����
        });

    if (KillLogWidget)
    {
        KillLogWidget->UpdateLogDisplay(KillLogs);
    }
}
