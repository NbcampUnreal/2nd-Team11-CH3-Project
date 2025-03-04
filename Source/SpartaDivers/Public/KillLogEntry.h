// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillLogEntry.generated.h"

class UTextBlock;
UCLASS()
class SPARTADIVERS_API UKillLogEntry : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetKillLog(const FString& VictimName, const FString& WeaponName, bool bIsHeadShot);

	virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* VictimNameText;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* WeaponNameText;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* HeadshotText;
};
