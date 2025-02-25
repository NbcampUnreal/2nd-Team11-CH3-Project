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
    // Make a character cannot Damaged
    UFUNCTION(Exec)
    void GodMode();

    // Kill Player
    UFUNCTION(Exec)
    void SDPlayerDead();
    
    // Kill All Enemies
    UFUNCTION(Exec)
    void SDKillAE();

    // Start Game
    UFUNCTION(Exec)
    void SDGStart();

    // Start Current Mission
    UFUNCTION(exec)
    void SDMStart();

    // Complete Current Mission
    UFUNCTION(exec)
    void SDMComple();
    
    // Make Bullet Infinite
    UFUNCTION(exec)
    void SDInfi();


};
