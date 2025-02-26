// Copyright Epic Games, Inc. All Rights Reserved.

#include "SDEnemyBase.h"
#include "SDAIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StatusContainerComponent.h"
#include "Item/DropItem.h"

ASDEnemyBase::ASDEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ASDAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	EnemyType = FName(TEXT("DefaultEnemy"));
	MoveSpeed = 300.f;
	StatusContainerComponent->SetMaxHealth(100);
	StatusContainerComponent->SetCurHealth(StatusContainerComponent->GetMaxHealth());
}

float ASDEnemyBase::GetHealth() const
{
	return StatusContainerComponent->GetCurHealth();
}

float ASDEnemyBase::GetMaxHealth() const
{
	return StatusContainerComponent->GetMaxHealth();
}

void ASDEnemyBase::AddHealth(float Amount)
{
	StatusContainerComponent->SetCurHealth(StatusContainerComponent->GetCurHealth() + Amount);
}

FName ASDEnemyBase::GetEnemyType() const
{
	return EnemyType;
}

float ASDEnemyBase::TakeDamage(
	float DamageAmount,
	FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return ActualDamage;
}

void ASDEnemyBase::Attack()
{
}

void ASDEnemyBase::OnDeath()
{
	Super::OnDeath();
}

void ASDEnemyBase::ApplyAttackEffect(int32 EffectIndex)
{
}

void ASDEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	this->Tags.Add(TEXT("Enemy"));
}

void ASDEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

