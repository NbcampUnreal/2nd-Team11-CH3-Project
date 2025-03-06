// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterBase.h"
#include "Components/StatusContainerComponent.h"
#include "Item/GunBase.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	StatusContainerComponent = CreateDefaultSubobject<UStatusContainerComponent>(TEXT("StatusContainerComponent"));

	KillScore = 0;
	bHeadshot = false;
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

	if (UGunBase* Gun = Cast<UGunBase>(DamageCauser))
	{
		bHeadshot = Gun->bHitHead;
	}

	if (StatusContainerComponent->GetCurArmor() <= 0)
	{
		StatusContainerComponent->SetCurHealth(StatusContainerComponent->GetCurHealth() - ActualDamage);
		GetMesh()->GetAnimInstance()->Montage_Play(HitMontage);
	}
	else
	{
		StatusContainerComponent->SetCurArmor(StatusContainerComponent->GetCurArmor() - ActualDamage);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShieldFlash, GetActorLocation());

	}
	if (StatusContainerComponent->GetCurHealth() <= 0)
	{
		if (bHeadshot)
		{
			KillScore *= 2;  // 헤드샷이면 KillScore 두 배 증가
		}
		OnDeath();
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());
	return ActualDamage;
}

void ACharacterBase::OnDeath()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());

	if (GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->StopAllMontages(0.25f);
	}

	bIsDead = true;
}