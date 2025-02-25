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

float ACharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser) / StatusContainerComponent->GetDepensePower();

	UE_LOG(LogTemp, Warning, TEXT("Enemy Damaged : %f, Current HP : %f"), ActualDamage, StatusContainerComponent->GetCurHealth());
	StatusContainerComponent->SetCurHealth(StatusContainerComponent->GetCurHealth() - ActualDamage);

	AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(HitMontage);
	if (StatusContainerComponent->GetCurHealth() <= 0)
	{
		OnDeath();
	}
	
	return ActualDamage;
}

void ACharacterBase::OnDeath()
{
	AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(DeathMontage);
}