// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/StatusContainerInterface.h"
#include "Kismet/GameplayStatics.h"

#include "CharacterBase.generated.h"

class UStatusContainerComponent;

UCLASS()
class SPARTADIVERS_API ACharacterBase : public ACharacter, public IStatusContainerInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status")
	int32 KillScore;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status")
	float MoveSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status")
	float SprintSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status")
	float SprintSpeedMultiplier;

	UPROPERTY(EditAnywhere)
	UAnimMontage* HitMontage;

	UPROPERTY(VisibleAnywhere, Category = "Status")
	UStatusContainerComponent* StatusContainerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsDead = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* HitSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* DeathSound;

public:
	// IStatusContainerInterface��(��) ���� ��ӵ�
	virtual UStatusContainerComponent* GetStatusContainerComponent() const override;

	//TakeDamge
	UFUNCTION()
	virtual float TakeDamage(
		float DamageAmount, 
		FDamageEvent const& DamgeEvent,
		AController* EnventInstigator, 
		AActor* DamageCauser) override;
	virtual void OnDeath();
};
