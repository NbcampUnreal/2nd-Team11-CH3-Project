// Copyright Epic Games, Inc. All Rights Reserved.

#include "SDEnemyBase.h"
#include "SDAIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

ASDEnemyBase::ASDEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ASDAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	EnemyType = FName(TEXT("DefaultEnemy"));
	MoveSpeed = 300.f;
	MaxHP = 100.0f;
	CurrentHP = MaxHP;
}

float ASDEnemyBase::GetHealth() const
{
	return CurrentHP;
}

float ASDEnemyBase::GetMaxHealth() const
{
	return MaxHP;
}

void ASDEnemyBase::AddHealth(float Amount)
{
	CurrentHP = FMath::Clamp(CurrentHP + Amount, 0.0f, MaxHP);
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
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CurrentHP = FMath::Clamp(CurrentHP - DamageAmount, 0.0f, MaxHP);
	UE_LOG(LogTemp, Warning, TEXT("Enemy Damaged : %f, Current HP : %f"), ActualDamage, CurrentHP);

	if (CurrentHP <= 0.0f)
	{
		OnDeath();
	}

	return ActualDamage;
}

void ASDEnemyBase::Attack()
{
}

void ASDEnemyBase::OnDeath()
{
	// Get the mesh component (Skeletal Mesh)
	USkeletalMeshComponent* MeshComp = GetMesh();

	if (MeshComp)
	{
		// Enable physics simulation to apply ragdoll physics
		MeshComp->SetSimulatePhysics(true);

		// Set the mesh collision to physics-based (ragdoll)
		MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MeshComp->SetCollisionProfileName("Ragdoll");  // Custom collision profile for ragdoll
	}

	// Log death for debugging
	UE_LOG(LogTemp, Warning, TEXT("Enemy DEAD"));
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

