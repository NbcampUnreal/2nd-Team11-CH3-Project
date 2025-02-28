// Copyright Epic Games, Inc. All Rights Reserved.

#include "Enemy/MeleeMinion.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

AMeleeMinion::AMeleeMinion()
{
	Damage = 10.0f;
}

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
	FVector End = Start;

	FCollisionQueryParams CollisionParams(NAME_None, false, this);
	FCollisionObjectQueryParams CollisionObjectParams(ECollisionChannel::ECC_Pawn);

	bool OnHit = GetWorld()->SweepMultiByChannel(
		OutHits,
		Start,
		End,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(AttackRange),
		CollisionParams
	);

	if (!OnHit) return;

	for (const FHitResult& OutHit : OutHits)
	{
		AActor* HitActor = OutHit.GetActor();

		if (HitActor && HitActor->ActorHasTag("Player"))
		{
			UGameplayStatics::ApplyDamage(
				HitActor,
				Damage,
				GetController(),
				this,
				UDamageType::StaticClass()
			);
		}
	}

	//DrawDebugSphere(GetWorld(), Start, AttackRange, 12, FColor::Red, false, 5.f, 0, 2.f);
}
