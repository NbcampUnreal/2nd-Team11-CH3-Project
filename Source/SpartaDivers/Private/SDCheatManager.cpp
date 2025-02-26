// Copyright Epic Games, Inc. All Rights Reserved.

#include "SDCheatManager.h"
#include "MyGameState.h"
#include "PlayerCharacter.h"
#include "Item/GunBase.h"
#include "Item/Weapons/AssaultRifle.h"
#include "SDEnemyBase.h"
#include "MissionManager.h"
#include "Kismet/GameplayStatics.h"

void USDCheatManager::GodMode()
{
    if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetOuterAPlayerController()->GetPawn()))
    {
        Player->SetCanBeDamaged(false);
        UE_LOG(LogTemp, Warning, TEXT("GodMode Activated!"));
    }
}

void USDCheatManager::SDKillMe()
{
    if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetOuterAPlayerController()->GetPawn()))
    {
        Player->OnDeath();
        UE_LOG(LogTemp, Warning, TEXT("Player Dead By CheatManager!"));
    }
}

void USDCheatManager::SDKillAE()
{
    AMissionManager* MissionManager = Cast<AMissionManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AMissionManager::StaticClass()));

    if (MissionManager)
    {
        MissionManager->DestroyEnemiesInCurrentMission(MissionManager->CurrentMissionIndex);
        UE_LOG(LogTemp, Warning, TEXT("Cheat activated: Alle Enemies Destoyed!"));
    }
}

void USDCheatManager::SDGStart()
{
    AMyGameState* MyGameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;

    if (MyGameState)
    {
        MyGameState->StartGame();
    }
}

void USDCheatManager::SDMStart()
{
    AMissionManager* MissionManager = Cast<AMissionManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AMissionManager::StaticClass()));

    if (MissionManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cheat activated: Mission started!"));
        MissionManager->StartMission();
    }
}

void USDCheatManager::SDMComple()
{
    // Find the AMissionManager in the current world
    AMissionManager* MissionManager = Cast<AMissionManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AMissionManager::StaticClass()));

    if (MissionManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cheat activated: Mission completed!"));
        MissionManager->CompleteMission();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("MissionManager not found!"));
    }
}

void USDCheatManager::SDInfi()
{
    // Infinite Bullet
    if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetOuterAPlayerController()->GetPawn()))
    {
        if (Player)
        {
            UAssaultRifle* AssaultRifle = Cast<UAssaultRifle>(Player->GetEquippedGun());
            AssaultRifle->bOnInfiniteBullet = true;
        }
    }
}

void USDCheatManager::SDADamage(float NewDamage)
{
    if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetOuterAPlayerController()->GetPawn()))
    {
        if (Player)
        {
            UAssaultRifle* AssaultRifle = Cast<UAssaultRifle>(Player->GetEquippedGun());
            AssaultRifle->Damage = NewDamage;
        }
    }
}