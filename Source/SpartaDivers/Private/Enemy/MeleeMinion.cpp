// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MeleeMinion.h"

void AMeleeMinion::Attack()
{
	if (AttackMontages.IsEmpty()) return;

	if (CurCombo >= AttackMontages.Num())
	{
		CurCombo = 0;
	}

	if (AttackMontages[CurCombo] == nullptr) return;

	PlayAnimMontage(AttackMontages[CurCombo]);
	CurCombo++;
}
