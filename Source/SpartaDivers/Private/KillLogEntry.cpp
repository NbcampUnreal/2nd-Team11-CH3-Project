// Copyright Epic Games, Inc. All Rights Reserved.

#include "KillLogEntry.h"
#include "SDLogManager.h"
#include "Components/TextBlock.h"

void UKillLogEntry::NativeConstruct()
{
	Super::NativeConstruct();
}

void UKillLogEntry::SetKillLog(const FString& VictimName, const FString& WeaponName, bool bIsHeadShot)
{
    if (VictimNameText)
    {
        VictimNameText->SetText(FText::FromString(VictimName));
    }

    if (WeaponNameText)
    {
        WeaponNameText->SetText(FText::FromString(WeaponName));
    }

    if (HeadshotText)
    {
        if (bIsHeadShot)
        {
            HeadshotText->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            HeadshotText->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}