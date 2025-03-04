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
    UVerticalBox* LogContainer; // �α׸� ���� UI �����̳�

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UKillLogEntry> KillLogEntryClass; // �α� �׸� Ŭ����
};
