// Copyright Epic Games, Inc. All Rights Reserved.

#include "KillLogWidget.h"
#include "KillLogEntry.h"
#include "SDLogManager.h"
#include "Components/VerticalBox.h"

void UKillLogWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!LogContainer)
    {
        UE_LOG(LogTemp, Error, TEXT("LogContainer is NULL! Trying to find it manually..."));

        LogContainer = Cast<UVerticalBox>(GetWidgetFromName(TEXT("LogContainer")));

        if (LogContainer)
        {
            UE_LOG(LogTemp, Warning, TEXT("LogContainer found manually!"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to find LogContainer manually!"));
        }
    }
}

void UKillLogWidget::UpdateLogDisplay(const TArray<FKillLog>& Logs)
{
    if (!LogContainer)
    {
        UE_LOG(LogTemp, Warning, TEXT("LogContainer is NULL"));

        return;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("LogContainer Called"));
    }
    LogContainer->ClearChildren(); // 기존 로그 삭제

    for (const FKillLog& Log : Logs)
    {
                UE_LOG(LogTemp, Warning, TEXT("UpdateLogDisplay Called"));
        if (KillLogEntryClass)
        {
            UKillLogEntry* LogEntry = CreateWidget<UKillLogEntry>(this, KillLogEntryClass);
                UE_LOG(LogTemp, Warning, TEXT("KillLogEntryClass Called"));
            if (LogEntry)
            {
                UE_LOG(LogTemp, Warning, TEXT("LogEntry Called"));

                LogEntry->SetKillLog(Log.VictimName, Log.WeaponName, Log.bIsHeadshot);
                LogContainer->AddChild(LogEntry);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("FAIL LogEntry Called"));

            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("FAIL KillLogEntryClass Called"));

        }
    }
}
