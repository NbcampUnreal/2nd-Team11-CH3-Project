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

	UPROPERTY(EditAnywhere)
	UAnimMontage* HitMontage;

	UPROPERTY(VisibleAnywhere, Category = "Status")
	UStatusContainerComponent* StatusContainerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsDead = false;
public:
	// IStatusContainerInterface��(��) ���� ��ӵ�
	virtual UStatusContainerComponent* GetStatusContainerComponent() const override;

	//TakeDamge
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamgeEvent, class AController* EnventInstigator, AActor* DamageCauser) override;
	virtual void OnDeath();
};
