// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MyHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/UserWidget_MainMenu.h"
#include "Kismet/GameplayStatics.h"

void AMyHUD::ShowMainMenu()
{
	if (MainMenuWidgetInstance == nullptr)
	{
		if (MainMenuWidgetClass == nullptr) return;

		MainMenuWidgetInstance = Cast<UUserWidget_MainMenu>(CreateWidget(GetWorld(), MainMenuWidgetClass));
		if (MainMenuWidgetInstance == nullptr) return;

		MainMenuWidgetInstance->AddToViewport();
	}

	MainMenuWidgetInstance->SetVisibility(ESlateVisibility::Visible);
	MainMenuWidgetInstance->UpdateInventorySlot();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->bShowMouseCursor = true;
	}

	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void AMyHUD::HideMainMenu()
{
	if (MainMenuWidgetInstance == nullptr)
	{
		if (MainMenuWidgetClass == nullptr) return;

		MainMenuWidgetInstance = Cast<UUserWidget_MainMenu>(CreateWidget(GetWorld(), MainMenuWidgetClass));
		if (MainMenuWidgetInstance == nullptr) return;

		MainMenuWidgetInstance->AddToViewport();
	}

	MainMenuWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
	}

	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

