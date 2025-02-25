// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "SDEnemyBase.generated.h"

class ADropItem;

UCLASS()
class SPARTADIVERS_API ASDEnemyBase : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	ASDEnemyBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	FName EnemyType;
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetMaxHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Status")
	float Damage;

	FName GetEnemyType() const;

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	virtual void Attack();

	void OnDeath() override;

	UFUNCTION(BlueprintCallable)
	virtual void ApplyAttackEffect(int32 EffectIndex);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADropItem> DropItem;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
