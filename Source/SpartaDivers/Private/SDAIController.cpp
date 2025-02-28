// Copyright Epic Games, Inc. All Rights Reserved.

#include "SDAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/CrowdFollowingComponent.h"

ASDAIController::ASDAIController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathfollowingComponent"))
{
}

void ASDAIController::BeginPlay()
{
	Super::BeginPlay();

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);
		GetBlackboardComponent()->SetValueAsObject("Player", PlayerPawn);
	}

	if (UCrowdFollowingComponent* CrowdFollowingComponent = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		CrowdFollowingComponent->SetCrowdSimulationState(ECrowdSimulationState::Enabled);

		CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);

		CrowdFollowingComponent->SetAvoidanceGroup(1);
		CrowdFollowingComponent->SetGroupsToAvoid(1);
		CrowdFollowingComponent->SetCrowdCollisionQueryRange(CollisionQueryRange);
	}

}
