// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MyHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/UserWidget_MainMenu.h"
#include "Kismet/GameplayStatics.h"

void AMyHUD::ToggleMainMenu()
{
	if (MainMenuWidgetInstance == nullptr)
	{
		if (MainMenuWidgetClass == nullptr) return;

		MainMenuWidgetInstance = Cast<UUserWidget_MainMenu>(CreateWidget(GetWorld(), MainMenuWidgetClass));
		if (MainMenuWidgetInstance == nullptr) return;

		MainMenuWidgetInstance->AddToViewport();
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController == nullptr) return;

	if (bIsMainMenuVisible)
	{
		MainMenuWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		PlayerController->SetInputMode(FInputModeGameOnly());

		PlayerController->SetShowMouseCursor(false);

		bIsMainMenuVisible = false;

		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}
	else
	{
		MainMenuWidgetInstance->SetVisibility(ESlateVisibility::Visible);
		UpdateMainUI();
		PlayerController->SetInputMode(FInputModeUIOnly());

		PlayerController->SetShowMouseCursor(true);

		bIsMainMenuVisible = true;

		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

void AMyHUD::UpdateMainUI()
{
	MainMenuWidgetInstance->UpdateInventorySlot();
	MainMenuWidgetInstance->UpdateMainMenu();
}
