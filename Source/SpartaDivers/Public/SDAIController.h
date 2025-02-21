// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SDAIController.generated.h"

UCLASS()
class SPARTADIVERS_API ASDAIController : public AAIController
{
	GENERATED_BODY()

protected:
	void BeginPlay();

private:
	UPROPERTY(EditDefaultsOnly);
	class UBehaviorTree* AIBehavior;
};