// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/StatusContainerInterface.h"

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

	UPROPERTY(VisibleAnywhere, Category = "Status")
	UStatusContainerComponent* StatusContainerComponent;

public:
	// IStatusContainerInterface을(를) 통해 상속됨
	virtual UStatusContainerComponent* GetStatusContainerComponent() const override;

};
