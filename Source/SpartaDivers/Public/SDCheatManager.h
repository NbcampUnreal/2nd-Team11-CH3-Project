// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "SDCheatManager.generated.h"

UCLASS()
class SPARTADIVERS_API USDCheatManager : public UCheatManager
{
	GENERATED_BODY()
	
public:
    UFUNCTION(Exec)
    void GodMode();

    UFUNCTION(Exec)
    void KillAllEnemies();
};
