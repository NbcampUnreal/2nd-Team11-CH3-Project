// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyGameState.h"
#include "MyGameInstance.h"

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

void AMyGameState::OnGameOver()
{

}
