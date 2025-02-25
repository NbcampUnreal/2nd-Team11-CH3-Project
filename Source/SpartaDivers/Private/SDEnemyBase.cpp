// Copyright Epic Games, Inc. All Rights Reserved.

#include "SDEnemyBase.h"
#include "SDAIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Item/DropItem.h"

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

void ASDEnemyBase::Attack()
{
}

//void ASDEnemyBase::OnDeath()
//{
//	// Get the mesh component (Skeletal Mesh)
//	USkeletalMeshComponent* MeshComp = GetMesh();
//
//	if (MeshComp)
//	{
//		// Enable physics simulation to apply ragdoll physics
//		MeshComp->SetSimulatePhysics(true);
//
//		// Set the mesh collision to physics-based (ragdoll)
//		MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
//		MeshComp->SetCollisionProfileName("Ragdoll");  // Custom collision profile for ragdoll
//	}
//
//	// Log death for debugging
//	UE_LOG(LogTemp, Warning, TEXT("Enemy DEAD"));
//
//	GetWorld()->SpawnActor<AActor>(DropItem, GetActorLocation(), GetActorRotation());
//}

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

