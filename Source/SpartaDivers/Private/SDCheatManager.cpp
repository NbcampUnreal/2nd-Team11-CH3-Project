// Copyright Epic Games, Inc. All Rights Reserved.

#include "SDCheatManager.h"
#include "PlayerCharacter.h"
#include "SDEnemyBase.h"
#include "Kismet/GameplayStatics.h"

void USDCheatManager::GodMode()
{
    if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetOuterAPlayerController()->GetPawn()))
    {
        Player->SetCanBeDamaged(false);
        UE_LOG(LogTemp, Warning, TEXT("GodMode Activated!"));
    }
}

void USDCheatManager::KillAllEnemies()
{
    TArray<AActor*> Enemies;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASDEnemyBase::StaticClass(), Enemies);

    for (AActor* Enemy : Enemies)
    {
        Enemy->Destroy();
    }

    UE_LOG(LogTemp, Warning, TEXT("All enemies eliminated!"));
}