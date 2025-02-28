// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyPlayerController.h"
#include "MyGameState.h"
#include "MyGameInstance.h"
#include "EnhancedInputSubsystems.h"
#include "SDCheatManager.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

AMyPlayerController::AMyPlayerController() : InputMappingContext(nullptr),
MoveAction(nullptr),
JumpAction(nullptr),
LookAction(nullptr),
SprintAction(nullptr),
FireAction(nullptr),
ReloadAction(nullptr),
SwapAction(nullptr),
InteractAction(nullptr),
InventoryAction(nullptr),
ButtonOneAction(nullptr),
ButtonTwoAction(nullptr),
ButtonThreeAction(nullptr),
ButtonFourAction(nullptr),
HUDWidgetClass(nullptr),
HUDWidgetInstance(nullptr),
MainMenuWidgetClass(nullptr),
MainMenuWidgetInstance(nullptr),
CrosshairWidgetClass(nullptr),
CrosshairWidgetInstance(nullptr)
{
	CheatClass = USDCheatManager::StaticClass();
}

UUserWidget* AMyPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

UUserWidget* AMyPlayerController::GetMainMenuWidget() const
{
	return MainMenuWidgetInstance;
}

UUserWidget* AMyPlayerController::GetCrosshairWidget() const
{
	return CrosshairWidgetInstance;
}

void AMyPlayerController::ShowGameHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}
	}

	AMyGameState* MyGameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
	if (MyGameState)
	{
		MyGameState->UpdateHUD();
	}

	ShowCrosshair();
}

void AMyPlayerController::ShowMainMenu(bool bIsRestart)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}
	if (CrosshairWidgetInstance)
	{
		CrosshairWidgetInstance->RemoveFromParent();
		CrosshairWidgetInstance = nullptr;
	}

	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}

		// TitleText
		if (UTextBlock* TitleText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName("TitleText")))
		{
			if (bIsRestart)
			{
				TitleText->SetVisibility(ESlateVisibility::Hidden);
			}
			else
			{
				TitleText->SetVisibility(ESlateVisibility::Visible);
			}
		}
		// GameOverText
		if (UTextBlock* GameOverText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName("GameOverText")))
		{
			if (bIsRestart)
			{
				GameOverText->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				GameOverText->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		// TotalScoreText
		if (UTextBlock* TotalScoreText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName("ScoreText")))
		{
			if (bIsRestart)
			{
				TotalScoreText->SetVisibility(ESlateVisibility::Visible);
				if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
				{
					TotalScoreText->SetText(FText::FromString(
						FString::Printf(TEXT("Score: %d"), MyGameInstance->TotalScore)
					));
				}
			}
			else
			{
				TotalScoreText->SetVisibility(ESlateVisibility::Hidden);
			}

		}
		// BestScoreText
		if (UTextBlock* BestScoreText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName("BestScoreText")))
		{
			if (bIsRestart)
			{
				BestScoreText->SetVisibility(ESlateVisibility::Visible);
				if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
				{
					BestScoreText->SetText(FText::FromString(
						FString::Printf(TEXT("Best Score: %d"), MyGameInstance->BestScore)
					));
				}
			}
			else
			{
				BestScoreText->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		// StartButtonText
		if (UTextBlock* StartButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
		{
			if (bIsRestart)
			{
				StartButtonText->SetText(FText::FromString(TEXT("Restart!!")));
			}
			else
			{
				StartButtonText->SetText(FText::FromString(TEXT("Game Start!!")));
			}
		}
	}
}

void AMyPlayerController::ShowCrosshair()
{
	if (CrosshairWidgetInstance)
	{
		CrosshairWidgetInstance->RemoveFromParent();
		CrosshairWidgetInstance = nullptr;
	}

	if (CrosshairWidgetClass)
	{
		CrosshairWidgetInstance = CreateWidget<UUserWidget>(this, CrosshairWidgetClass);
		if (CrosshairWidgetInstance)
		{
			CrosshairWidgetInstance->AddToViewport();
		}
	}
}

void AMyPlayerController::StartGame()
{
	if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		MyGameInstance->TotalScore = 0;
		MyGameInstance->bGameStarted = true;
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("Main"));
	SetPause(false);
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("MainMenu"))
	{
		ShowMainMenu(false);
	}
	else if (CurrentMapName == "Main")  // Main 레벨이면 HUD 표시
	{
		ShowGameHUD();
	}
}