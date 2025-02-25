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
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			MyPlayerController->ShowGameHUD();
		}
	}

	AMissionManager* MissionManager = Cast<AMissionManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AMissionManager::StaticClass()));
	if (MissionManager)
	{
		MissionManager->StartMission();
	}
}

void AMyGameState::OnGameOver()
{

}

void AMyGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = MyPlayerController->GetHUDWidget())
			{
				if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetPawn()))
				{
					// Health Bar 
					if (UProgressBar* HealthBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("HealthBar"))))
					{
						float HealthPercent =
							PlayerCharacter->GetStatusContainerComponent()->GetCurHealth()
							/ PlayerCharacter->GetStatusContainerComponent()->GetMaxHealth();
						HealthBar->SetPercent(HealthPercent);
					}
					// Ammo
					if (UTextBlock* AmmoText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("AmmoText"))))
					{
						if (!PlayerCharacter->GetEquippedGun()) return;
						int32 CurAmmo = PlayerCharacter->GetEquippedGun()->CurAmmo;
						int32 MaxAmmo = PlayerCharacter->GetEquippedGun()->MaxAmmo;
						AmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurAmmo , MaxAmmo)));
					}
				}
				
			}

			if (UUserWidget* CrosshairWidget = MyPlayerController->GetHUDWidget())
			{
				UFunction* PlayAnimCrosshair = CrosshairWidget->FindFunction(FName("CrossHairsAnimation"));
				if (PlayAnimCrosshair)
				{
					UE_LOG(LogTemp, Warning, TEXT("Anim Played"));
					CrosshairWidget->ProcessEvent(PlayAnimCrosshair, nullptr);
				}
			}
		}
	}
}

void AMyGameState::BeginPlay()
{
	Super::BeginPlay();

	StartGame();
}