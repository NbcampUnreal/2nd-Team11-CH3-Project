// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UCLASS()
class SPARTADIVERS_API ACharacterBase : public ACharacter
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

};
