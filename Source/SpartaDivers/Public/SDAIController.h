// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SDAIController.generated.h"

UCLASS()
class SPARTADIVERS_API ASDAIController : public AAIController
{
	GENERATED_BODY()

	ASDAIController(const FObjectInitializer& ObjectInitializer);

protected:
	void BeginPlay();

	UPROPERTY(EditDefaultsOnly);
	float CollisionQueryRange = 600.f;

private:
	UPROPERTY(EditDefaultsOnly);
	class UBehaviorTree* AIBehavior;
};