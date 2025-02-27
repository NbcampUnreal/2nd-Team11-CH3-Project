// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/RangedMinion.h"
#include "EnemyProjectile.h"

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
		ProjectileInstance->InitProjectile(VelocityOfProjectile);
	}
}

void ARangedMinion::Fire()
{
}
