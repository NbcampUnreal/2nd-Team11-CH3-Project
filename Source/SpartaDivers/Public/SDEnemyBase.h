// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "SDEnemyBase.generated.h"

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

	virtual float TakeDamage(
		float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
	virtual void Attack();

	UFUNCTION(BlueprintCallable)
	virtual void OnDeath();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
