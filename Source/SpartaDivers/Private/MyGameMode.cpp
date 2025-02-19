// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyGameMode.h"
#include "MyGameState.h"
//#include "TestCharacterimkskku.h"
//#include "TestPlayerControllerimkskku.h"

AMyGameMode::AMyGameMode()
{
	//DefaultPawnClass = ATestCharacterimkskku::StaticClass();
	//PlayerControllerClass = ATestPlayerControllerimkskku::StaticClass();
	GameStateClass = AMyGameState::StaticClass();
}