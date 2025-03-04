// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "SDEnemyBase.generated.h"

class ADropItem;
class UItemBase;
class UDamageTextComponent;

USTRUCT(BlueprintType)
struct FDropItemInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UItemBase> dropItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float dropWeight;
};

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

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "UI")
	UDamageTextComponent* DamageTextComp;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Status")
	float Damage;
	UFUNCTION(BlueprintPure, Category = "KillLog")
	FName GetEnemyType();
	UPROPERTY(VisibleAnywhere, Category = "Hitbox")
	UStaticMeshComponent* HeadHitbox;

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	virtual void Attack(int32 SkillIndex);

	void OnDeath() override;

	void UpdateGameData();

	void AddToLogManager();

	void OnDropItem();

	UFUNCTION(BlueprintCallable)
	virtual void ApplyAttackEffect(int32 EffectIndex);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADropItem> DropItem;

	UPROPERTY(EditDefaultsOnly)
	TArray<FDropItemInfo> DropItemInfos;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};