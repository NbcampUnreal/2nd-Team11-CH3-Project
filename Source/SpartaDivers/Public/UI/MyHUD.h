// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

class UUserWidget_MainMenu;

/**
 * 
 */
UCLASS()
class SPARTADIVERS_API AMyHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void ToggleMainMenu();

	void UpdateMainUI();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> MainMenuWidgetClass;

    UPROPERTY()
    UUserWidget_MainMenu* MainMenuWidgetInstance;

	bool bIsMainMenuVisible = false;
};
