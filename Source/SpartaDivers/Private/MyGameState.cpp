// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyGameState.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayerController.h"
#include "MissionManager.h"

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
			MyPlayerController->ShowCrosshair();
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

void AMyGameState::BeginPlay()
{
	Super::BeginPlay();

	StartGame();
}
