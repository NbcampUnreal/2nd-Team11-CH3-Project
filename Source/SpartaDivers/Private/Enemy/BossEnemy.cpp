// Copyright Epic Games, Inc. All Rights Reserved.

#include "Enemy/BossEnemy.h"
#include "PlayerCharacter.h"
#include "Components/StatusContainerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyProjectile.h"

ABossEnemy::ABossEnemy()
{
	KillScore = Damage * 3 + StatusContainerComponent->GetMaxHealth();
	StatusContainerComponent->SetCurHealth(5000.f);
}

void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();

	this->Tags.Add(TEXT("Boss"));
	/*float InitCurHealth = StatusContainerComponent->GetMaxHealth();
	StatusContainerComponent->SetCurHealth(InitCurHealth);*/
}

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
	default:
		break;
	}
}

void ABossEnemy::ApplyAttackEffect(int32 EffectIndex)
{
	switch (EffectIndex)
	{
	case 0:
		ApplyBasicAttackEffect();
		break;
	case 1:
		ApplyJumpAttackEffect();
		break;
	case 2:
		ApplySpawnMinionEffect();
		break;
	case 3:
		ApplySpawnBombEffect();
		break;
	case 4:
		ApplyFireEffect();
		break;
	default:
		break;
	}
}

void ABossEnemy::ApplyBasicAttackEffect()
{
	TArray<FHitResult> OutHits;
	FVector Start = GetActorLocation();
	FCollisionQueryParams CollisionParams(NAME_Name, false, this);
	FCollisionObjectQueryParams ColisionObjectParams(ECollisionChannel::ECC_Pawn);

	bool OnHit = GetWorld()->SweepMultiByChannel(OutHits, Start, Start, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(BasicAttackRange), CollisionParams);
	if (OnHit == false) return;

	for (const FHitResult& OutHit : OutHits)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OutHit.GetActor());
		if (Player == nullptr) continue;

		UGameplayStatics::ApplyDamage(
			Player,
			Damage * BasicAttackRate,
			GetController(),
			this,
			UDamageType::StaticClass());
	}

	//DrawDebugSphere(GetWorld(), GetActorLocation(), BasicAttackRange, 12, FColor::Red, false, 5.f, 0, 2.f);
}

void ABossEnemy::ApplyJumpAttackEffect()
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
		FCollisionShape::MakeSphere(JumpAttackRange),
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

	//DrawDebugSphere(GetWorld(), GetActorLocation(), JumpAttackRange, 12, FColor::Red, false, 5.f, 0, 2.f);
}

void ABossEnemy::ApplySpawnMinionEffect()
{
	if (SpawnEnemies.IsEmpty()) return;
	if (SpawnMinionNum == 0) return;

	float AngleStep = 360.f / SpawnMinionNum;

	for (int i = 0; i < SpawnMinionNum; i++)
	{
		int32 RandomInt = FMath::RandRange(0, SpawnEnemies.Num() - 1);
		if (SpawnEnemies[RandomInt] == nullptr) return;

		float AngleInRadians = FMath::DegreesToRadians(i * AngleStep);

		float X = GetActorLocation().X + SpawnMinionRange * FMath::Cos(AngleInRadians);
		float Y = GetActorLocation().Y + SpawnMinionRange * FMath::Sin(AngleInRadians);

		FVector SpawnLocation = FVector(X, Y, GetActorLocation().Z + 200.f);

		GetWorld()->SpawnActor<AActor>(SpawnEnemies[RandomInt], SpawnLocation, GetActorRotation());
	}
}

void ABossEnemy::ApplySpawnBombEffect()
{
	float RandOffset = FMath::FRandRange(0.f, 360.f);

	for (int i = 0; i < 8; i++)
	{
		float Radian = FMath::DegreesToRadians(45.f * i);
		Radian += FMath::DegreesToRadians(RandOffset);
		FVector Direction = FVector(FMath::Cos(Radian), FMath::Sin(Radian), 0.0f);
		AEnemyProjectile* ProjectileInstance = GetWorld()->SpawnActor<AEnemyProjectile>(ProjectileClass, GetActorLocation() + GetActorUpVector() * 500.f + Direction * 200.f, Direction.Rotation());
		if (ProjectileInstance)
		{
			ProjectileInstance->SetExplosionDamage(Damage * BombDamageRate);
			ProjectileInstance->InitProjectile(FMath::FRandRange(500.f, 3000.f));
		}
	}
}

void ABossEnemy::ApplyFireEffect()
{
	FVector SocketLocation = GetMesh()->GetSocketLocation(FireSocketName);
	FRotator SocketRotation = GetMesh()->GetSocketRotation(FireSocketName);
	AEnemyProjectile* ProjectileInstance = GetWorld()->SpawnActor<AEnemyProjectile>(ProjectileClass, SocketLocation, SocketRotation);

	if (ProjectileInstance)
	{
		ProjectileInstance->SetExplosionDamage(Damage * BombDamageRate);
		ProjectileInstance->InitProjectile(FMath::FRandRange(3000.f, 5000.f));
	}
}
