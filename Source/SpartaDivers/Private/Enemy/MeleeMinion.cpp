// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MeleeMinion.h"
#include "PlayerCharacter.h"

void AMeleeMinion::Attack(int32 SkillIndex)
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

void AMeleeMinion::ApplyAttackEffect(int32 EffectIndex)
{
    TArray<FHitResult> OutHits;
    FVector Start = GetActorLocation();
    FCollisionQueryParams CollisionParams(NAME_Name, false, this);
    FCollisionObjectQueryParams ColisionObjectParams(ECollisionChannel::ECC_Visibility);

    bool OnHit = GetWorld()->SweepMultiByChannel(OutHits, Start, Start, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(AttackRange), CollisionParams);
    if (OnHit == false) return;

	for (const FHitResult& OutHit : OutHits)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OutHit.GetActor());
		if (Player == nullptr) continue;

        UGameplayStatics::ApplyDamage(
            Player,
            Damage,
            GetController(),
            this,
            UDamageType::StaticClass());
	}

	//DrawDebugSphere(GetWorld(), GetActorLocation(), AttackRange, 12, FColor::Red, false, 5.f, 0, 2.f);
}
