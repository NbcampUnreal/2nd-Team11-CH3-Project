// Copyright Epic Games, Inc. All Rights Reserved.

#include "Enemy/RangedMinion.h"
#include "EnemyProjectile.h"
#include "Components/StatusContainerComponent.h"

ARangedMinion::ARangedMinion()
{
	EnemyType = "Ranage Minion";
	Damage = FMath::RandRange(20.0f, 50.0f);
	KillScore = Damage * 3 + StatusContainerComponent->GetMaxHealth();
	StatusContainerComponent->SetMaxHealth(FMath::RandRange(100.0f, 200.0f));
	StatusContainerComponent->SetCurHealth(StatusContainerComponent->GetMaxHealth());
}

void ARangedMinion::Attack(int32 SkillIndex)
{
	if (AttackMontage == nullptr || ProjectileClass == nullptr) return;

	PlayAnimMontage(AttackMontage);
}

void ARangedMinion::ApplyAttackEffect(int32 EffectIndex)
{
	if (AttackMontage == nullptr || GetMesh() == nullptr) return;

	FVector SocketLocation = GetMesh()->GetSocketLocation(FireSocketName);
	FRotator SocketRotation = GetMesh()->GetSocketRotation(FireSocketName);
	AEnemyProjectile* ProjectileInstance = GetWorld()->SpawnActor<AEnemyProjectile>(ProjectileClass, SocketLocation, SocketRotation);

	if (ProjectileInstance)
	{
		ProjectileInstance->SetExplosionDamage(Damage);
		ProjectileInstance->InitProjectile(VelocityOfProjectile);
	}
}

void ARangedMinion::Fire()
{
}