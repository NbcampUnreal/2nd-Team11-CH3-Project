// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyGameState.h"
#include "MyGameInstance.h"
#include "MyPlayerController.h"
#include "MissionManager.h"
#include "PlayerCharacter.h"
#include "Enemy/BossEnemy.h"
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
	PlayTime = 0.0f;
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
	// 타이머 시작
	GetWorldTimerManager().SetTimer(
		PlayTimeTimerHandle,
		this,
		&AMyGameState::UpdatePlayTime,
		1.0f,
		true
	);

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

void AMyGameState::UpdatePlayTime()
{
	PlayTime += 1.0f;
}

void AMyGameState::OnGameOver()
{
	//GetWorldTimerManager().ClearTimer(PlayTimeTimerHandle);
	int32 Minutes = FMath::FloorToInt(PlayTime / 60);
	int32 Seconds = FMath::FloorToInt(PlayTime) % 60;

	PlayTimeStr = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);

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
				// Player Informations
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
							float ArmorPercent =
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
					if (!HUDWidget || !PlayerCharacter) return;  
					
					UImage* WeaponImage = Cast<UImage>(HUDWidget->GetWidgetFromName(TEXT("WeaponImage")));
					UpdateWeaponImage(WeaponImage, PlayerCharacter->GetEquippedGun());
					if (PlayerCharacter->GetEquippedGun())
					{
					UE_LOG(LogTemp, Warning, TEXT("GetEquippedGun : %s!!"),*PlayerCharacter->GetEquippedGun()->GetName());
					}
					if (PlayerCharacter->GetSubGun())
					{
						UE_LOG(LogTemp, Warning, TEXT("GetSubGun : %s!!"), *PlayerCharacter->GetSubGun()->GetName());
					}

					UImage* SubWeaponImage = Cast<UImage>(HUDWidget->GetWidgetFromName(TEXT("SubWeaponImage")));
					UpdateWeaponImage(SubWeaponImage, PlayerCharacter->GetSubGun());
				}
				// Mission Informations
				if (AMissionManager* MissionManager = Cast<AMissionManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AMissionManager::StaticClass())))
				{
					if (UTextBlock* MissionText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("MissionText"))))
					{
						if (MissionManager->bIsPlayerOnMission)
						{
							MissionText->SetText(FText::FromName(MissionManager->CurrentMissionData.MissionName));
						}
						else
						{
							MissionText->SetText(FText::FromString(FString::Printf(TEXT("Mission Success!"))));
						}
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
						if (UProgressBar* BossHealthBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("BossHealthBar"))))
						{
							TArray<AActor*> FoundActors;
							UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Boss")), FoundActors);

							// 각 액터의 체력 가져오기
							for (AActor* Actor : FoundActors)
							{
								if (Actor)
								{
									ABossEnemy* Boss = Cast<ABossEnemy>(Actor);
									if (UStatusContainerComponent* StatusComponent = Boss->GetStatusContainerComponent())
									{
										float CurrentHealth = StatusComponent->GetCurHealth();
										float MaxHealth = StatusComponent->GetMaxHealth();
										float BossHealthPercent = CurrentHealth / MaxHealth;
										BossHealthBar->SetVisibility(ESlateVisibility::Visible);
										BossHealthBar->SetPercent(BossHealthPercent);
									}
								}
							}
						}
						break;
					}
					default:
					{

						break;
					}
					}
				}
				// GameData
				if (UGameInstance* GameInstance = GetGameInstance())
				{
					UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
					if (MyGameInstance)
					{
						if (UTextBlock* CurrentScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("CurrentScoreText"))))
						{
							CurrentScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score : %d"), MyGameInstance->TotalScore)));
						}
					}
				}
			}

		}
	}
}

void AMyGameState::UpdateWeaponImage(UImage* ImageWidget, UGunBase* Gun)
{
	if (!ImageWidget) return; 

	if (Gun && Gun->GetIconImage())
	{
		ImageWidget->SetVisibility(ESlateVisibility::Visible);
		ImageWidget->SetBrushFromTexture(Gun->GetIconImage());
	}
	else
	{
		ImageWidget->SetVisibility(ESlateVisibility::Hidden);
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
			}
		}
	}
}

void AMyGameState::UpdateHitUI()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			if (UUserWidget* HitEffectWidget = MyPlayerController->GetHitEffectWidget())
			{
				UFunction* PlayAnimHitEffect = HitEffectWidget->FindFunction(FName("PlayAnimHitEffect"));
				if (PlayAnimHitEffect)
				{
					HitEffectWidget->ProcessEvent(PlayAnimHitEffect, nullptr);
				}
			}
		}
	}
}

void AMyGameState::PlayWeaponUnlockedAnim()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = MyPlayerController->GetHUDWidget())
			{
				UFunction* PlayClearAnim = HUDWidget->FindFunction(FName("PlayClearAnim"));
				if (PlayClearAnim)
				{
					HUDWidget->ProcessEvent(PlayClearAnim, nullptr);
				}
			}
		}
	}
}

void AMyGameState::SwapUIAnim()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = MyPlayerController->GetHUDWidget())
			{
				UFunction* PlaySwapAnim = HUDWidget->FindFunction(FName("PlaySwapAnim"));
				if (PlaySwapAnim)
				{
					HUDWidget->ProcessEvent(PlaySwapAnim, nullptr);
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