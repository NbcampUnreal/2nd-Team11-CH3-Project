// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyPlayerController.h"
#include "MyGameState.h"
#include "EnhancedInputSubsystems.h"
#include "SDCheatManager.h"
#include "Blueprint/UserWidget.h"

AMyPlayerController::AMyPlayerController() : InputMappingContext(nullptr),
MoveAction(nullptr),
JumpAction(nullptr),
LookAction(nullptr),
SprintAction(nullptr),
FireAction(nullptr),
ReloadAction(nullptr),
SwapAction(nullptr),
InventoryAction(nullptr),
InteractAction(nullptr),
ButtonOneAction(nullptr),
ButtonTwoAction(nullptr),
ButtonThreeAction(nullptr),
ButtonFourAction(nullptr),
CrouchAction(nullptr),
HUDWidgetClass(nullptr),
HUDWidgetInstance(nullptr),
CrosshairWidgetClass(nullptr),
CrosshairWidgetInstance(nullptr)
{
	CheatClass = USDCheatManager::StaticClass();
}

UUserWidget* AMyPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

UUserWidget* AMyPlayerController::GetCrosshairWidget() const
{
	return CrosshairWidgetInstance;
}

void AMyPlayerController::ShowGameHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}
	}

	AMyGameState* MyGameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
	if (MyGameState)
	{
		MyGameState->UpdateHUD();
	}

	ShowCrosshair();
}

void AMyPlayerController::ShowCrosshair()
{
	if (CrosshairWidgetInstance)
	{
		CrosshairWidgetInstance->RemoveFromParent();
		CrosshairWidgetInstance = nullptr;
	}

	if (CrosshairWidgetClass)
	{
		CrosshairWidgetInstance = CreateWidget<UUserWidget>(this, CrosshairWidgetClass);
		if (CrosshairWidgetInstance)
		{
			CrosshairWidgetInstance->AddToViewport();
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