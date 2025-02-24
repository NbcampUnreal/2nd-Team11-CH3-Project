// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "SDCheatManager.h"

AMyPlayerController::AMyPlayerController() : InputMappingContext(nullptr),
MoveAction(nullptr),
JumpAction(nullptr),
LookAction(nullptr),
SprintAction(nullptr),
FireAction(nullptr),
ReloadAction(nullptr),
CrosshairWidgetClass(nullptr),
CrosshairWidgetInstance(nullptr)
{
	CheatClass = USDCheatManager::StaticClass();
}

void AMyPlayerController::ShowCrosshair()
{
	UE_LOG(LogTemp, Warning, TEXT("ShowCrosshair Function Called"));

	if (CrosshairWidgetInstance)
	{
		CrosshairWidgetInstance->RemoveFromParent();
		CrosshairWidgetInstance = nullptr;
	}

	if (!CrosshairWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("CrosshairWidgetClass is NULL!"));
		return;
	}

	if (CrosshairWidgetClass)
	{
		CrosshairWidgetInstance = CreateWidget<UUserWidget>(this, CrosshairWidgetClass);
		if (CrosshairWidgetInstance)
		{
			CrosshairWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
			UE_LOG(LogTemp, Warning, TEXT("Show Crosshair"));
		}
	}
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

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
}