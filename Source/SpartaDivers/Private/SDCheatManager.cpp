// Copyright Epic Games, Inc. All Rights Reserved.

#include "SDCheatManager.h"
#include "PlayerCharacter.h"
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

void USDCheatManager::SDKillAE()
{
    AMissionManager* MissionManager = Cast<AMissionManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AMissionManager::StaticClass()));

    if (MissionManager)
    {
        MissionManager->DestroyAllEnemies();
        UE_LOG(LogTemp, Warning, TEXT("Cheat activated: Alle Enemies Destoyed!"));
    }
}

void USDCheatManager::SDStart()
{
    AMissionManager* MissionManager = Cast<AMissionManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AMissionManager::StaticClass()));

    if (MissionManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cheat activated: Mission started!"));
        MissionManager->StartMission();
    }
}

void USDCheatManager::SDComple()
{
    // 현재 월드에서 AMissionManager 찾기
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
    // 총알무한
}