// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BossEnemy.h"

void ABossEnemy::Attack(int32 SkillIndex)
{
	switch (SkillIndex)
	{
	case 0:
		if (BasicAttackMontage)
		{
			PlayAnimMontage(BasicAttackMontage);
		}
		break;
	case 1:
		if (BasicAttackMontage)
		{
			PlayAnimMontage(JumpAttackMontage);
		}
		break;
	case 2:
		if (BasicAttackMontage)
		{
			PlayAnimMontage(SpawnMinionMontage);
		}
		break;
	case 3:
		if (BasicAttackMontage)
		{
			PlayAnimMontage(SpawnBombMontage);
		}
		break;
	case 4:
		if (BasicAttackMontage)
		{
			PlayAnimMontage(FireMontage);
		}
		break;
	case 5:
		if (JumpToPlayerMontage)
		{
			PlayAnimMontage(JumpToPlayerMontage);
		}
		break;
	default:
		break;
	}
}
