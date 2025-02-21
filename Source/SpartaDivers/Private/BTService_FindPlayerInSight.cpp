// Copyright Epic Games, Inc. All Rights Reserved.

#include "BTService_FindPlayerInSight.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTService_FindPlayerInSight::UBTService_FindPlayerInSight()
{
	NodeName = "Find Player In Sight";
}

void UBTService_FindPlayerInSight::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (PlayerPawn && OwnerComp.GetAIOwner())
	{
		if (OwnerComp.GetAIOwner()->LineOfSightTo(PlayerPawn))
		{
			float Distance = FVector::Dist2D(PlayerPawn->GetActorLocation(), OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation());
			OwnerComp.GetBlackboardComponent()->SetValueAsFloat(GetSelectedBlackboardKey(), Distance);
			return;
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(GetSelectedBlackboardKey(), -1.f);
	return;
}
