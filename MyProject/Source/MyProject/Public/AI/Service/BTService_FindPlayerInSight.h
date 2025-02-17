// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_FindPlayerInSight.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBTService_FindPlayerInSight : public UBTService_BlackboardBase
{
	GENERATED_BODY()

	UBTService_FindPlayerInSight();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;	
};
