// Copyright Epic Games, Inc. All Rights Reserved.

#include "Enemy/RangedMinion.h"
#include "EnemyProjectile.h"

void ARangedMinion::Attack()
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
		ProjectileInstance->InitProjectile(VelocityOfProjectile);
	}
}

void ARangedMinion::Fire()
{
}
