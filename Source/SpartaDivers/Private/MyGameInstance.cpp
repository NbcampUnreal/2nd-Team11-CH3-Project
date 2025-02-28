// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	bGameStarted = false;
	BestScore = 0;
	TotalScore = 0;
}


void UMyGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Total Score : %d"), TotalScore);
}

void UMyGameInstance::UpdateBestScore(int32 Score)
{
	if (Score >= BestScore)
	{
		BestScore = Score;
	}
}