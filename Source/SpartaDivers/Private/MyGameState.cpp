// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyGameState.h"
#include "MyGameInstance.h"
#include "MyPlayerController.h"
#include "MissionManager.h"
#include "PlayerCharacter.h"
#include "Components/StatusContainerComponent.h"
#include "Item/GunBase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"

AMyGameState::AMyGameState()
{
	PrimaryActorTick.bCanEverTick = false;

	Score = 0;
}

int32 AMyGameState::GetScore() const
{
	return Score;
}

void AMyGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
		if (MyGameInstance)
		{
			MyGameInstance->AddToScore(Amount);
		}
	}
}

void AMyGameState::StartGame()
{
	AMissionManager* MissionManager = Cast<AMissionManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AMissionManager::StaticClass()));
	if (MissionManager)
	{
		MissionManager->StartMission();
	}

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			MyPlayerController->ShowGameHUD();
		}
	}
}

void AMyGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			MyPlayerController->SetPause(true);
			MyPlayerController->ShowMainMenu(true);
		}
	}
}

void AMyGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			// HudWidget
			if (UUserWidget* HUDWidget = MyPlayerController->GetHUDWidget())
			{
				// Health Bar & Ammo
				if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetPawn()))
				{
					if (UProgressBar* HealthBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("HealthBar"))))
					{
						if (PlayerCharacter->GetStatusContainerComponent()->GetMaxHealth() > 0)
						{
							float HealthPercent =
								PlayerCharacter->GetStatusContainerComponent()->GetCurHealth()
								/ PlayerCharacter->GetStatusContainerComponent()->GetMaxHealth();
							HealthBar->SetPercent(HealthPercent);
						}
					}
					if (UProgressBar* ArmorBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("ArmorBar"))))
					{
						if (PlayerCharacter->GetStatusContainerComponent()->GetMaxArmor() > 0)
						{
							float ArmorPercent=
								PlayerCharacter->GetStatusContainerComponent()->GetCurArmor()
								/ PlayerCharacter->GetStatusContainerComponent()->GetMaxArmor();
							ArmorBar->SetPercent(ArmorPercent);
						}
					}
					if (UTextBlock* AmmoText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("AmmoText"))))
					{
						if (!PlayerCharacter->GetEquippedGun()) return;
						int32 CurAmmo = PlayerCharacter->GetEquippedGun()->CurAmmo;
						int32 MaxAmmo = PlayerCharacter->GetEquippedGun()->MaxAmmo;
						AmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurAmmo, MaxAmmo)));
					}
				}
				// Mission Informations
				if (AMissionManager* MissionManager = Cast<AMissionManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AMissionManager::StaticClass())))
				{

					if (UTextBlock* MissionText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("MissionText"))))
					{
						MissionText->SetText(FText::FromName(MissionManager->CurrentMissionData.MissionName));
					}
					// Hide all Information
					if (UProgressBar* EliminateProgressBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("EliminateProgressBar"))))
					{
						EliminateProgressBar->SetVisibility(ESlateVisibility::Hidden);
					}
					if (UTextBlock* EliminateText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("EliminateText"))))
					{
						EliminateText->SetVisibility(ESlateVisibility::Hidden);
					}
					if (UProgressBar* SurviveProgressBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("SurviveProgressBar"))))
					{
						SurviveProgressBar->SetVisibility(ESlateVisibility::Hidden);
					}
					if (UProgressBar* CaptureProgressBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("CaptureProgressBar"))))
					{
						CaptureProgressBar->SetVisibility(ESlateVisibility::Hidden);
					}
					if (UProgressBar* BossHealthBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("BossHealthBar"))))
					{
						BossHealthBar->SetVisibility(ESlateVisibility::Hidden);
					}
					// Update ProgressBars Based On MissionType of CurrentMissionData
					switch (MissionManager->CurrentMissionData.MissionType)
					{
					case EMissionType::Eliminate:
					{
						if (UProgressBar* EliminateProgressBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("EliminateProgressBar"))))
						{
							if (MissionManager->SpawnedEnemyCount > 0)
							{
								EliminateProgressBar->SetVisibility(ESlateVisibility::Visible);
								float EliminateProgress = (static_cast<float>(MissionManager->KilledEnemyCount)) / MissionManager->SpawnedEnemyCount;
								EliminateProgressBar->SetPercent(EliminateProgress);
							}
						}
						if (UTextBlock* EliminateText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("EliminateText"))))
						{
							EliminateText->SetVisibility(ESlateVisibility::Visible);
							EliminateText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"),
								MissionManager->KilledEnemyCount,
								MissionManager->SpawnedEnemyCount)));
						}
						break;
					}
					case EMissionType::Survive:
					{
						if (UProgressBar* SurviveProgressBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("SurviveProgressBar"))))
						{
							if (MissionManager->CurrentMissionData.SurviveTime > 0)
							{
								SurviveProgressBar->SetVisibility(ESlateVisibility::Visible);
								float RemainingTime = GetWorldTimerManager().GetTimerRemaining(MissionManager->SurvivalTimerHandle);
								float SurviveProgress = RemainingTime / MissionManager->CurrentMissionData.SurviveTime;
								SurviveProgressBar->SetPercent(SurviveProgress);
							}
						}
						break;
					}
					case EMissionType::Capture:
					{
						if (UProgressBar* CaptureProgressBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("CaptureProgressBar"))))
						{
							if (MissionManager->CurrentMissionData.CaptureTime > 0)
							{
								CaptureProgressBar->SetVisibility(ESlateVisibility::Visible);
								float CaptureProgress = MissionManager->CaptureProgress / MissionManager->CurrentMissionData.CaptureTime;
								CaptureProgressBar->SetPercent(CaptureProgress);
							}
						}

						break;
					}
					case EMissionType::BossCombat:
					{

						break;
					}
					default:
					{

						break;
					}
					}
				}
			}

		}
	}
}

void AMyGameState::UpdateCrossHair()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			// CrosshairWidget
			if (UUserWidget* CrosshairWidget = MyPlayerController->GetCrosshairWidget())
			{
				UFunction* PlayAnimCrosshair = CrosshairWidget->FindFunction(FName("PlayCrosshairAnim"));
				if (PlayAnimCrosshair)
				{
					CrosshairWidget->ProcessEvent(PlayAnimCrosshair, nullptr);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("FailedToo......"));
				}
			}
		}
	}
}

void AMyGameState::BeginPlay()
{
	Super::BeginPlay();


	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
		if (MyGameInstance && MyGameInstance->bGameStarted)
		{
			StartGame();
		}
	}

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&AMyGameState::UpdateHUD,
		0.1f,
		true
	);
}