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
        LogContainer = Cast<UVerticalBox>(GetWidgetFromName(TEXT("LogContainer")));
    }
}

void UKillLogWidget::UpdateLogDisplay(const TArray<FKillLog>& Logs)
{
    if (!LogContainer) return;
   
    LogContainer->ClearChildren(); // 기존 로그 삭제

    for (const FKillLog& Log : Logs)
    {
        if (KillLogEntryClass)
        {
            UKillLogEntry* LogEntry = CreateWidget<UKillLogEntry>(this, KillLogEntryClass);
            if (LogEntry)
            {
                LogEntry->SetKillLog(Log.VictimName, Log.WeaponName, Log.bIsHeadshot);
                LogContainer->AddChild(LogEntry);
            }
        }
    }
}
