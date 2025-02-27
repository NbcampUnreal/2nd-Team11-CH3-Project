// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_JumpToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class SPARTADIVERS_API UAnimNotifyState_JumpToPlayer : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference);

private:
	FVector TargetLocation;

	UPROPERTY(EditAnywhere)
	float Radius = 200.f;

	float Velocity;
};
