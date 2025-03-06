// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class USDLogManager;
class UKillLogWidget;

UCLASS()
class SPARTADIVERS_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* FireAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* ReloadAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SwapAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* InteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* InventoryAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* ButtonOneAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* ButtonTwoAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* ButtonThreeAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* ButtonFourAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* RollingAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* ZoomAction;

	UFUNCTION(BlueprintPure, Category = "UI|HUD")
	UUserWidget* GetHUDWidget() const;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI|HUD")
	UUserWidget* HUDWidgetInstance;

	UFUNCTION(BlueprintPure, Category = "UI|KillLog")
	UKillLogWidget* GetKillLogWidget() const;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|KillLog")
	TSubclassOf<UKillLogWidget> KillLogWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|KillLog")
	UKillLogWidget* KillLogWidgetInstance;

	UFUNCTION(BlueprintPure, Category = "UI|MainMenu")
	UUserWidget* GetMainMenuWidget() const;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|MainMenu")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI|MainMenu")
	UUserWidget* MainMenuWidgetInstance;

	UFUNCTION(BlueprintPure, Category = "UI|Crosshair")
	UUserWidget* GetCrosshairWidget() const;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Crosshair")
	TSubclassOf<UUserWidget> CrosshairWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI|Crosshair")
	UUserWidget* CrosshairWidgetInstance;

	UFUNCTION(BlueprintCallable, Category = "UI|HUD")
	void ShowGameHUD();
	UFUNCTION(BlueprintCallable, Category = "UI|MainMenu")
	void ShowMainMenu(bool bIsRestart);
	UFUNCTION(BlueprintCallable, Category = "UI|Crosshair")
	void ShowCrosshair();
	UFUNCTION(BlueprintCallable, Category = "UI|KillLog")
	void ShowKillLog();
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void StartGame();

	virtual void BeginPlay() override;
};
