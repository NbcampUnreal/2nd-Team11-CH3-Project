// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterBase.h"
#include "Components/StatusContainerComponent.h"
#include "Item/GunBase.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	StatusContainerComponent = CreateDefaultSubobject<UStatusContainerComponent>(TEXT("StatusContainerComponent"));

	KillScore = 0;
}

UStatusContainerComponent* ACharacterBase::GetStatusContainerComponent() const
{
	return StatusContainerComponent;
}

float ACharacterBase::TakeDamage(
	float DamageAmount,
	FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	if (bIsDead) return 0.f;

	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	bool bIsDeadByHeadshot = false;

	if (UGunBase* Gun = Cast<UGunBase>(DamageCauser))
	{
		bIsDeadByHeadshot = Gun->bHitHead;
	}

	StatusContainerComponent->SetCurHealth(StatusContainerComponent->GetCurHealth() - ActualDamage);
	GetMesh()->GetAnimInstance()->Montage_Play(HitMontage);
	if (StatusContainerComponent->GetCurHealth() <= 0)
	{
		if (bIsDeadByHeadshot)
		{
			KillScore *= 2;  // ��弦�̸� KillScore �� �� ����
		}
		OnDeath();
	}
	
	return ActualDamage;
}

void ACharacterBase::OnDeath()
{
	if (GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->StopAllMontages(0.25f);
	}

	bIsDead = true;
}