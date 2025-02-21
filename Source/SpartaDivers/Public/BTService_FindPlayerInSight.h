// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_FindPlayerInSight.generated.h"

UCLASS()
class SPARTADIVERS_API UBTService_FindPlayerInSight : public UBTService_BlackboardBase
{
	GENERATED_BODY()

	UBTService_FindPlayerInSight();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
