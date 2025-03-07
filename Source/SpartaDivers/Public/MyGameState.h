// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameState.generated.h"

class UImage;
class UGunBase;
UCLASS()
class SPARTADIVERS_API AMyGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AMyGameState();

	FTimerHandle PlayTimeTimerHandle;
	float PlayTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;
	FString PlayTimeStr;
	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);

	FTimerHandle HUDUpdateTimerHandle;

	TArray<AActor*> FoundMissionManagers;
	
	UFUNCTION(BlueprintCallable, Category = "Level")
	void StartGame();
	void UpdatePlayTime();
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();
	UFUNCTION(BlueprintCallable)
	void UpdateHUD();
	UFUNCTION(BlueprintCallable)
	void UpdateWeaponImage(UImage* ImageWidget, UGunBase* Gun);
	UFUNCTION(BlueprintCallable)
	void UpdateCrossHair();
	UFUNCTION(BlueprintCallable)
	void UpdateHitUI();
	UFUNCTION(BlueprintCallable)
	void PlayWeaponUnlockedAnim();
	UFUNCTION(BlueprintCallable)
	void SwapUIAnim();

protected:
	virtual void BeginPlay() override;
};
