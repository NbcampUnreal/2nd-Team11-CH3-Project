// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SDEnemyBase.h"
#include "BossEnemy.generated.h"

class AEnemyProjectile;

UCLASS()
class SPARTADIVERS_API ABossEnemy : public ASDEnemyBase
{
	GENERATED_BODY()

public:
	ABossEnemy();

	virtual void BeginPlay() override;

	virtual void Attack(int32 SkillIndex) override;

	int32 CurCombo = 0;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* BasicAttackMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* JumpAttackMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* SpawnMinionMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* SpawnBombMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* FireMontage;

	virtual void ApplyAttackEffect(int32 EffectIndex) override;

	void ApplyBasicAttackEffect();
	void ApplyJumpAttackEffect();
	void ApplySpawnMinionEffect();
	void ApplySpawnBombEffect();
	void ApplyFireEffect();

	UPROPERTY(EditDefaultsOnly)
	float BasicAttackRange;

	UPROPERTY(EditDefaultsOnly)
	float BasicAttackRate;

	UPROPERTY(EditDefaultsOnly)
	float JumpAttackRange;

	UPROPERTY(EditDefaultsOnly)
	float JumpAttackRate;

	UPROPERTY(EditDefaultsOnly)
	float SpawnMinionNum;

	UPROPERTY(EditDefaultsOnly)
	float SpawnMinionRange;

	UPROPERTY(EditDefaultsOnly)
	float BombDamageRate;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<ASDEnemyBase>> SpawnEnemies;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemyProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	FName FireSocketName;
};
