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
	float MoveSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status")
	float SprintSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status")
	float SprintSpeedMultiplier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status")
	float MaxHP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status")
	float CurrentHP;

	UAnimInstance* AnimInstance;
	UPROPERTY(EditAnywhere)
	UAnimMontage* DeathMontage;
	UPROPERTY(EditAnywhere)
	UAnimMontage* HitMontage;

	UPROPERTY(VisibleAnywhere, Category = "Status")
	UStatusContainerComponent* StatusContainerComponent;

	// IStatusContainerInterface을(를) 통해 상속됨
	virtual UStatusContainerComponent* GetStatusContainerComponent() const override;

	//TakeDamge
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamgeEvent, class AController* EnventInstigator, AActor* DamageCauser) override;

	virtual void OnDeath();
};
