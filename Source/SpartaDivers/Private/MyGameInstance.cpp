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
	UpdateBestScore(TotalScore);
}

void UMyGameInstance::UpdateBestScore(int32 Score)
{
	if (Score >= BestScore)
	{
		BestScore = Score;
	}
}