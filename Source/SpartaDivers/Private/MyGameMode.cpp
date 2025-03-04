// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyGameMode.h"
#include "MyGameState.h"
#include "PlayerCharacter.h"
#include "MyPlayerController.h"
#include "KillLogWidget.h"
#include "SDLogManager.h"

AMyGameMode::AMyGameMode()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
	GameStateClass = AMyGameState::StaticClass();
}
