// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameState.generated.h"

UCLASS()
class SPARTADIVERS_API AMyGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AMyGameState();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;

	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);

	TArray<AActor*> FoundMissionManagers;
	
	UFUNCTION(BlueprintCallable, Category = "Level")
	void StartGame();
	
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();

protected:
	virtual void BeginPlay() override;
};
