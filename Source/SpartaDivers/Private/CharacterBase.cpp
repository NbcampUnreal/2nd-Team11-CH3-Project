// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterBase.h"
#include "Components/StatusContainerComponent.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	StatusContainerComponent = CreateDefaultSubobject<UStatusContainerComponent>(TEXT("StatusContainerComponent"));
}

UStatusContainerComponent* ACharacterBase::GetStatusContainerComponent() const
{
	return StatusContainerComponent;
}

float ACharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamgeEvent, class AController* EnventInstigator, AActor* DamageCauser)
{
	const float Damage = Super::TakeDamage(DamageAmount, DamgeEvent, EnventInstigator, DamageCauser);
	if (Damage > 0)
	{
		CurrentHP -= Damage;
		AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(HitMontage);
		if (CurrentHP <= 0)
		{
			OnDeath();
		}
	}

	return Damage;
}

void ACharacterBase::OnDeath()
{
	AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(DeathMontage);
}

