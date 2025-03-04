// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyPlayerController.h"
#include "MyGameState.h"
#include "MyGameInstance.h"
#include "EnhancedInputSubsystems.h"
#include "SDCheatManager.h"
#include "SDLogManager.h"
#include "KillLogWidget.h"
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
KillLogWidgetClass(nullptr),
KillLogWidgetInstance(nullptr),
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

UKillLogWidget* AMyPlayerController::GetKillLogWidget() const
{
	return KillLogWidgetInstance;
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
	ShowKillLog();
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
		// PlayTimeText
		if (UTextBlock* PlayTimeText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("PlayTimeText"))))
		{
			if (bIsRestart)
			{
				PlayTimeText->SetVisibility(ESlateVisibility::Visible);
				AMyGameState* MyGameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
				if (MyGameState)
				{
					MyGameState->UpdateHUD();
				}
				PlayTimeText->SetText(FText::FromString(
					FString::Printf(TEXT("%s"), *MyGameState->PlayTimeStr)
				));
			}
			else
			{
				PlayTimeText->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		// KillCountText
		if (UTextBlock* KillCountText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("KillCountText"))))
		{
			if (bIsRestart)
			{
				KillCountText->SetVisibility(ESlateVisibility::Visible);
				if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
				{
					KillCountText->SetText(FText::FromString(
						FString::Printf(TEXT("Defeated enemies : %d"), MyGameInstance->TotalKillCount)
					));
				}
			}
			else
			{
				KillCountText->SetVisibility(ESlateVisibility::Hidden);

			}
		}
		// AssaultRifleKillBullet
		if (UTextBlock* AssaultRifleKillBullet = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("AssaultRifleKillBullet"))))
		{
			if (bIsRestart)
			{
				AssaultRifleKillBullet->SetVisibility(ESlateVisibility::Visible);
				if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
				{
					AssaultRifleKillBullet->SetText(FText::FromString(
						FString::Printf(TEXT("▷Assault Rifle : %d with %d bullets"), MyGameInstance->AssaultKillCount, MyGameInstance->AssaultBulletCount)
					));
				}
			}
			else
			{
				AssaultRifleKillBullet->SetVisibility(ESlateVisibility::Hidden);

			}
		}
		// ShotgunKillBullet
		if (UTextBlock* ShotgunKillBullet = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("ShotgunKillBullet"))))
		{
			if (bIsRestart)
			{
				ShotgunKillBullet->SetVisibility(ESlateVisibility::Visible);
				if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
				{
					ShotgunKillBullet->SetText(FText::FromString(
						FString::Printf(TEXT("▷Shotgun : %d with %d bullets"), MyGameInstance->ShotgunKillCount, MyGameInstance->ShotgunBulletCount)
					));
				}
			}
			else
			{
				ShotgunKillBullet->SetVisibility(ESlateVisibility::Hidden);

			}
		}
		// SniperKillBullet
		if (UTextBlock* SniperKillBullet = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("SniperKillBullet"))))
		{
			if (bIsRestart)
			{
				SniperKillBullet->SetVisibility(ESlateVisibility::Visible);
				if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
				{
					SniperKillBullet->SetText(FText::FromString(
						FString::Printf(TEXT("▷Sniper : %d with %d bullets"), MyGameInstance->SniperKillCount, MyGameInstance->SniperBulletCount)
					));
				}
			}
			else
			{
				SniperKillBullet->SetVisibility(ESlateVisibility::Hidden);

			}
		}
		// RocketKillBullet
		if (UTextBlock* RocketKillBullet = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("RocketKillBullet"))))
		{
			if (bIsRestart)
			{
				RocketKillBullet->SetVisibility(ESlateVisibility::Visible);
				if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this)))
				{
					RocketKillBullet->SetText(FText::FromString(
						FString::Printf(TEXT("▷Rocket Launcher : %d with %d bullets"), MyGameInstance->RocketKillCount, MyGameInstance->RocketBulletCount)
					));
				}
			}
			else
			{
				RocketKillBullet->SetVisibility(ESlateVisibility::Hidden);

			}
		}

		if (bIsRestart)
		{
			UFunction* PlayClearAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayClearLogAnim"));
			if (PlayClearAnimFunc)
			{
				MainMenuWidgetInstance->ProcessEvent(PlayClearAnimFunc, nullptr);
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

void AMyPlayerController::ShowKillLog()
{
	if (KillLogWidgetInstance)
	{
		KillLogWidgetInstance->RemoveFromParent();
		KillLogWidgetInstance = nullptr;

	}

	if (KillLogWidgetClass)
	{
		if (KillLogWidgetInstance)
		{
			KillLogWidgetInstance->AddToViewport();
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

	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if (!GameInstance) return;

	USDLogManager* LogManager = USDLogManager::Get();
	if (!LogManager) return;

	// UI 위젯 생성 후 LogManager에 연결
	UKillLogWidget* KillLogWidget = CreateWidget<UKillLogWidget>(GetWorld(), KillLogWidgetClass);
	if (KillLogWidget)
	{
		KillLogWidget->AddToViewport();
		LogManager->SetKillLogWidget(KillLogWidget);
	}

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