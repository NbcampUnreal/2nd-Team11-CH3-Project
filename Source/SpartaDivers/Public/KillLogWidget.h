// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillLogWidget.generated.h"

class UVerticalBox;
class UKillLogEntry;

UCLASS()
class SPARTADIVERS_API UKillLogWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void UpdateLogDisplay(const TArray<FKillLog>& Logs);

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UVerticalBox* LogContainer; // 로그를 담을 UI 컨테이너

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UKillLogEntry> KillLogEntryClass; // 로그 항목 클래스
};
